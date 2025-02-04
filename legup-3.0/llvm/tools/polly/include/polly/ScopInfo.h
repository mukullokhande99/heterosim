//===------ polly/ScopInfo.h - Create Scops from LLVM IR --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Create a polyhedral description for a static control flow region.
//
// The pass creates a polyhedral description of the Scops detected by the Scop
// detection derived from their LLVM-IR code.
//
// This represantation is shared among several tools in the polyhedral
// community, which are e.g. CLooG, Pluto, Loopo, Graphite.
//
//===----------------------------------------------------------------------===//

#ifndef POLLY_SCOP_INFO_H
#define POLLY_SCOP_INFO_H

#include "llvm/Analysis/RegionPass.h"

using namespace llvm;

namespace llvm {
  class SCEV;
  class ScalarEvolution;
  class SCEVAddRecExpr;
  class Loop;
  class LoopInfo;
  class Type;
  class PHINode;
}

struct isl_map;
struct isl_basic_map;
struct isl_set;
struct isl_ctx;
struct isl_dim;
struct isl_constraint;

namespace polly {

class Scop;
class ScopStmt;
class ScopInfo;
class TempScop;
class SCEVAffFunc;
class Comparison;

//===----------------------------------------------------------------------===//
/// @brief Represent memory accesses in statements.
class MemoryAccess {
  // DO NOT IMPLEMENT
  MemoryAccess(const MemoryAccess &);
  // DO NOT IMPLEMENT
  const MemoryAccess &operator=(const MemoryAccess &);

public:
  /// @brief The access type of a memory access
  ///
  /// There are three kind of access types:
  ///
  /// * A read access
  ///
  /// A certain set of memory locations are read and may be used for internal
  /// calculations.
  ///
  /// * A write access
  ///
  /// A certain set of memory locactions is definitely written. The old value is
  /// replaced by a newly calculated value. The old value is not read or used at
  /// all.
  ///
  /// * A may write access
  ///
  /// A certain set of memory locactions may be written. The memory location may
  /// contain a new value if there is actually a write or the old value may
  /// remain, if no write happens.
  enum AccessType {
    Read,
    Write,
    MayWrite
  };

private:
  isl_map *AccessRelation;
  enum AccessType Type;

  const Value* BaseAddr;
  std::string BaseName;
  isl_basic_map *createBasicAccessMap(ScopStmt *Statement);
  void setBaseName();
  ScopStmt *statement;

  /// Updated access relation read from JSCOP file.
  isl_map *newAccessRelation;
public:
  // @brief Create an affine memory access.
  //
  // @param AffFunc    The access function.
  // @param Statement  The Statement that contains this access.
  // @param SE         The ScalarEvolution analysis.
  MemoryAccess(const SCEVAffFunc &AffFunc, ScopStmt *Statement);

  // @brief Create a read all access.
  //
  // @param BaseAddress The base address of the memory accessed.
  // @param Statement   The Statement that contains this access.
  MemoryAccess(const Value *BaseAddress, ScopStmt *Statement);

  ~MemoryAccess();

  /// @brief Is this a read memory access?
  bool isRead() const { return Type == MemoryAccess::Read; }

  isl_map *getAccessFunction() { return AccessRelation; }
  isl_map *getAccessFunction() const { return AccessRelation; }

  /// @brief Get an isl string representing this access function.
  std::string getAccessFunctionStr() const;

  const Value *getBaseAddr() const {
    return BaseAddr;
  }

  const std::string &getBaseName() const {
    return BaseName;
  }

  /// @brief Get the stride of this memory access in the specified domain
  ///        subset.
  isl_set *getStride(const isl_set *domainSubset) const;

  /// @brief Is consecutive memory accessed for a given
  ///        statement instance set?
  bool isStrideOne(const isl_set *domainSubset) const;

  /// @brief Is always the same memory accessed for a given
  ///        statement instance set?
  bool isStrideZero(const isl_set *domainSubset) const;

  /// @brief Get the statement that contains this memory access.
  ScopStmt *getStatement() const { return statement; }

  /// @brief Set the updated access relation read from JSCOP file.
  void setNewAccessFunction(isl_map *newAccessRelation);
  /// @brief Print the MemoryAccess.
  ///
  /// @param OS The output stream the MemoryAccess is printed to.
  void print(raw_ostream &OS) const;

  /// @brief Print the MemoryAccess to stderr.
  void dump() const;
};

//===----------------------------------------------------------------------===//
/// @brief Statement of the Scop
///
/// A Scop statement represents an instruction in the Scop.
///
/// It is further described by its iteration domain, its schedule and its data
/// accesses.
/// At the moment every statement represents a single basic block of LLVM-IR.
class ScopStmt {
  //===-------------------------------------------------------------------===//
  // DO NOT IMPLEMENT
  ScopStmt(const ScopStmt &);
  // DO NOT IMPLEMENT
  const ScopStmt &operator=(const ScopStmt &);


  /// Polyhedral description
  //@{

  /// The Scop containing this ScopStmt
  Scop &Parent;

  /// The iteration domain describes the set of iterations for which this
  /// statement is executed.
  ///
  /// Example:
  ///     for (i = 0; i < 100 + b; ++i)
  ///       for (j = 0; j < i; ++j)
  ///         S(i,j);
  ///
  /// 'S' is executed for different values of i and j. A vector of all
  /// induction variables around S (i, j) is called iteration vector.
  /// The domain describes the set of possible iteration vectors.
  ///
  /// In this case it is:
  ///
  ///     Domain: 0 <= i <= 100 + b
  ///             0 <= j <= i
  ///
  /// A pair of statment and iteration vector (S, (5,3)) is called statment
  /// instance.
  isl_set *Domain;

  /// The scattering map describes the execution order of the statement
  /// instances.
  ///
  /// A statement and its iteration domain do not give any information about the
  /// order in time in which the different statement instances are executed.
  /// This information is provided by the scattering.
  ///
  /// The scattering maps every instance of each statement into a multi
  /// dimensional scattering space. This space can be seen as a multi
  /// dimensional clock.
  ///
  /// Example:
  ///
  /// <S,(5,4)>  may be mapped to (5,4) by this scattering:
  ///
  /// s0 = i (Year of execution)
  /// s1 = j (Day of execution)
  ///
  /// or to (9, 20) by this scattering:
  ///
  /// s0 = i + j (Year of execution)
  /// s1 = 20 (Day of execution)
  ///
  /// The order statement instances are executed is defined by the
  /// scattering vectors they are mapped to. A statement instance
  /// <A, (i, j, ..)> is executed before a statement instance <B, (i', ..)>, if
  /// the scattering vector of A is lexicographic smaller than the scattering
  /// vector of B.
  isl_map *Scattering;

  /// The memory accesses of this statement.
  ///
  /// The only side effects of a statement are its memory accesses.
  typedef SmallVector<MemoryAccess*, 8> MemoryAccessVec;
  MemoryAccessVec MemAccs;
  std::map<const Instruction*, MemoryAccess*> InstructionToAccess;

  //@}

  /// The BasicBlock represented by this statement.
  BasicBlock *BB;

  /// @brief Whether this statement is a reduction.
  bool IsReduction;

  /// @brief The loop induction variables surrounding the statement.
  ///
  /// This information is only needed for final code generation.
  std::vector<std::pair<PHINode*, Loop*> > IVS;

  std::string BaseName;

  /// Build the statment.
  //@{
  isl_set *toUpperLoopBound(const SCEVAffFunc &UpperBound, isl_dim *dim,
			    unsigned BoundedDimension) const;
  isl_set *toConditionSet(const Comparison &Cmp, isl_dim *dim) const;
  void addConditionsToDomain(TempScop &tempScop, const Region &CurRegion);
  void buildIterationDomainFromLoops(TempScop &tempScop);
  void buildIterationDomain(TempScop &tempScop, const Region &CurRegion);
  void buildScattering(SmallVectorImpl<unsigned> &Scatter);
  void buildAccesses(TempScop &tempScop, const Region &CurRegion);
  //@}

  /// Create the ScopStmt from a BasicBlock.
  ScopStmt(Scop &parent, TempScop &tempScop, const Region &CurRegion,
           BasicBlock &bb, SmallVectorImpl<Loop*> &NestLoops,
           SmallVectorImpl<unsigned> &Scatter);

  /// Create the finalization statement.
  ScopStmt(Scop &parent, SmallVectorImpl<unsigned> &Scatter);

  friend class Scop;
public:

  ~ScopStmt();

  /// @brief Get an isl_ctx pointer.
  isl_ctx *getIslContext();

  /// @brief Get the iteration domain of this ScopStmt.
  ///
  /// @return The iteration domain of this ScopStmt.
  isl_set *getDomain() const;

  /// @brief Get an isl string representing this domain.
  std::string getDomainStr() const;

  /// @brief Get the scattering function of this ScopStmt.
  ///
  /// @return The scattering function of this ScopStmt.
  isl_map *getScattering() const { return Scattering; }
  void setScattering(isl_map *scattering) { Scattering = scattering; }

  /// @brief Get an isl string representing this scattering.
  std::string getScatteringStr() const;

  /// @brief Get the BasicBlock represented by this ScopStmt.
  ///
  /// @return The BasicBlock represented by this ScopStmt.
  BasicBlock *getBasicBlock() const { return BB; }

  MemoryAccess &getAccessFor(const Instruction *Inst) {
    return *InstructionToAccess[Inst];
  }

  void setBasicBlock(BasicBlock *Block) { BB = Block; }

  typedef MemoryAccessVec::iterator memacc_iterator;
  memacc_iterator memacc_begin() { return MemAccs.begin(); }
  memacc_iterator memacc_end() { return MemAccs.end(); }

  unsigned getNumParams() const;
  unsigned getNumIterators() const;
  unsigned getNumScattering() const;

  Scop *getParent() { return &Parent; }
  const Scop *getParent() const { return &Parent; }

  const char *getBaseName() const;
  /// @brief Get the induction variable for a dimension.
  ///
  /// @param Dimension The dimension of the induction variable
  /// @return The induction variable at a certain dimension.
  const PHINode *getInductionVariableForDimension(unsigned Dimension) const;

  /// @brief Get the loop for a dimension.
  ///
  /// @param Dimension The dimension of the induction variable
  /// @return The loop at a certain dimension.
  const Loop *getLoopForDimension(unsigned Dimension) const;

  /// @brief Return the SCEV for a loop dimension.
  const SCEVAddRecExpr *getSCEVForDimension(unsigned Dimension) const;

  /// @brief Is this statement the final read statement?
  ///
  /// A final read statement is scheduled after all statements to model
  /// that all data used in the Scop is read after the Scop.
  bool isFinalRead() { return getBasicBlock() == NULL; }

  bool isReduction() { return IsReduction; }

  /// @brief Print the ScopStmt.
  ///
  /// @param OS The output stream the ScopStmt is printed to.
  void print(raw_ostream &OS) const;

  /// @brief Print the ScopStmt to stderr.
  void dump() const;
};

/// @brief Print ScopStmt S to raw_ostream O.
static inline raw_ostream& operator<<(raw_ostream &O, const ScopStmt &S) {
  S.print(O);
  return O;
}

//===----------------------------------------------------------------------===//
/// @brief Static Control Part
///
/// A Scop is the polyhedral representation of a control flow region detected
/// by the Scop detection. It is generated by translating the LLVM-IR and
/// abstracting its effects.
///
/// A Scop consists of a set of:
///
///   * A set of statements executed in the Scop.
///
///   * A set of global parameters
///   Those parameters are scalar integer values, which are constant during
///   execution.
///
///   * A context
///   This context contains information about the values the parameters
///   can take and relations between different parameters.
class Scop {
  //===-------------------------------------------------------------------===//
  // DO NOT IMPLEMENT
  Scop(const Scop &);
  // DO NOT IMPLEMENT
  const Scop &operator=(const Scop &);

  ScalarEvolution *SE;

  /// The underlying Region.
  Region &R;

  /// Max loop depth.
  unsigned MaxLoopDepth;

  typedef std::vector<ScopStmt*> StmtSet;
  /// The Statments in this Scop.
  StmtSet Stmts;

  /// Parameters of this Scop
  typedef SmallVector<const SCEV*, 8> ParamVecType;
  ParamVecType Parameters;

  /// Constraints on parameters.
  isl_set *Context;

  /// Create the static control part with a region, max loop depth of this
  /// region and parameters used in this region.
  Scop(TempScop &TempScop, LoopInfo &LI, ScalarEvolution &SE);

  /// @brief Check if a basic block is trivial.
  ///
  /// A trivial basic block does not contain any useful calculation. Therefore,
  /// it does not need to be represented as a polyhedral statement.
  ///
  /// @param BB The basic block to check
  /// @param tempScop TempScop returning further information regarding the Scop.
  ///
  /// @return True if the basic block is trivial, otherwise false.
  static bool isTrivialBB(BasicBlock *BB, TempScop &tempScop);

  /// Build the Scop and Statement with precalculate scop information.
  void buildScop(TempScop &TempScop, const Region &CurRegion,
                  // Loops in Scop containing CurRegion
                  SmallVectorImpl<Loop*> &NestLoops,
                  // The scattering numbers
                  SmallVectorImpl<unsigned> &Scatter,
                  LoopInfo &LI);

  /// Helper function for printing the Scop.
  void printContext(raw_ostream &OS) const;
  void printStatements(raw_ostream &OS) const;

  friend class ScopInfo;
public:

  ~Scop();

  ScalarEvolution *getSE() const;

  /// @brief Get the count of parameters used in this Scop.
  ///
  /// @return The count of parameters used in this Scop.
  inline ParamVecType::size_type getNumParams() const {
    return Parameters.size();
  }

  /// @brief Get a set containing the parameters used in this Scop
  ///
  /// @return The set containing the parameters used in this Scop.
  inline const ParamVecType &getParams() const { return Parameters; }

  /// @name Parameter Iterators
  ///
  /// These iterators iterate over all parameters of this Scop.
  //@{
  typedef ParamVecType::iterator param_iterator;
  typedef ParamVecType::const_iterator const_param_iterator;

  param_iterator param_begin() { return Parameters.begin(); }
  param_iterator param_end()   { return Parameters.end(); }
  const_param_iterator param_begin() const { return Parameters.begin(); }
  const_param_iterator param_end()   const { return Parameters.end(); }
  //@}

  /// @brief Get the maximum region of this static control part.
  ///
  /// @return The maximum region of this static control part.
  inline const Region &getRegion() const { return R; }
  inline Region &getRegion() { return R; }

  /// @brief Get the maximum depth of the loop.
  ///
  /// @return The maximum depth of the loop.
  inline unsigned getMaxLoopDepth() const { return MaxLoopDepth; }

  /// @brief Get the scattering dimension number of this Scop.
  ///
  /// @return The scattering dimension number of this Scop.
  inline unsigned getScatterDim() const {
    unsigned maxScatterDim = 0;

    for (const_iterator SI = begin(), SE = end(); SI != SE; ++SI)
      maxScatterDim = std::max(maxScatterDim, (*SI)->getNumScattering());

    return maxScatterDim;
  }

  /// @brief Get the name of this Scop.
  std::string getNameStr() const;

  /// @brief Get the constraint on parameter of this Scop.
  ///
  /// @return The constraint on parameter of this Scop.
  inline isl_set *getContext() const { return Context; }

  /// @brief Get an isl string representing the context.
  std::string getContextStr() const;

  /// @name Statements Iterators
  ///
  /// These iterators iterate over all statements of this Scop.
  //@{
  typedef StmtSet::iterator iterator;
  typedef StmtSet::const_iterator const_iterator;

  iterator begin() { return Stmts.begin(); }
  iterator end()   { return Stmts.end();   }
  const_iterator begin() const { return Stmts.begin(); }
  const_iterator end()   const { return Stmts.end();   }

  typedef StmtSet::reverse_iterator reverse_iterator;
  typedef StmtSet::const_reverse_iterator const_reverse_iterator;

  reverse_iterator rbegin() { return Stmts.rbegin(); }
  reverse_iterator rend()   { return Stmts.rend();   }
  const_reverse_iterator rbegin() const { return Stmts.rbegin(); }
  const_reverse_iterator rend()   const { return Stmts.rend();   }
  //@}

  /// @brief Print the static control part.
  ///
  /// @param OS The output stream the static control part is printed to.
  void print(raw_ostream &OS) const;

  /// @brief Print the ScopStmt to stderr.
  void dump() const;

  /// @brief Get the isl context of this static control part.
  ///
  /// @return The isl context of this static control part.
  isl_ctx *getCtx() const;
};

/// @brief Print Scop scop to raw_ostream O.
static inline raw_ostream& operator<<(raw_ostream &O, const Scop &scop) {
  scop.print(O);
  return O;
}

//===---------------------------------------------------------------------===//
/// @brief Build the Polly IR (Scop and ScopStmt) on a Region.
///
class ScopInfo : public RegionPass {
  //===-------------------------------------------------------------------===//
  // DO NOT IMPLEMENT
  ScopInfo(const ScopInfo &);
  // DO NOT IMPLEMENT
  const ScopInfo &operator=(const ScopInfo &);

  // The Scop
  Scop *scop;

  void clear() {
    if (scop) {
      delete scop;
      scop = 0;
    }
  }

public:
  static char ID;
  explicit ScopInfo() : RegionPass(ID), scop(0) {}
  ~ScopInfo() { clear(); }

  /// @brief Try to build the Polly IR of static control part on the current
  ///        SESE-Region.
  ///
  /// @return If the current region is a valid for a static control part,
  ///         return the Polly IR representing this static control part,
  ///         return null otherwise.
  Scop *getScop() { return scop; }
  const Scop *getScop() const { return scop; }

  /// @name RegionPass interface
  //@{
  virtual bool runOnRegion(Region *R, RGPassManager &RGM);
  virtual void getAnalysisUsage(AnalysisUsage &AU) const;
  virtual void releaseMemory() { clear(); }
  virtual void print(raw_ostream &OS, const Module *) const {
    if (scop)
      scop->print(OS);
    else
      OS << "Invalid Scop!\n";
  }
  //@}
};

} //end namespace polly

#endif
