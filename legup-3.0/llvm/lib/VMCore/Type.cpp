//===-- Type.cpp - Implement the Type class -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Type class for the VMCore library.
//
//===----------------------------------------------------------------------===//

#include "LLVMContextImpl.h"
#include "llvm/Module.h"
#include <algorithm>
#include <cstdarg>
#include "llvm/ADT/SmallString.h"
using namespace llvm;

//===----------------------------------------------------------------------===//
//                         Type Class Implementation
//===----------------------------------------------------------------------===//

Type *Type::getPrimitiveType(LLVMContext &C, TypeID IDNumber) {
  switch (IDNumber) {
  case VoidTyID      : return getVoidTy(C);
  case FloatTyID     : return getFloatTy(C);
  case DoubleTyID    : return getDoubleTy(C);
  case X86_FP80TyID  : return getX86_FP80Ty(C);
  case FP128TyID     : return getFP128Ty(C);
  case PPC_FP128TyID : return getPPC_FP128Ty(C);
  case LabelTyID     : return getLabelTy(C);
  case MetadataTyID  : return getMetadataTy(C);
  case X86_MMXTyID   : return getX86_MMXTy(C);
  default:
    return 0;
  }
}

/// getScalarType - If this is a vector type, return the element type,
/// otherwise return this.
Type *Type::getScalarType() {
  if (VectorType *VTy = dyn_cast<VectorType>(this))
    return VTy->getElementType();
  return this;
}

/// isIntegerTy - Return true if this is an IntegerType of the specified width.
bool Type::isIntegerTy(unsigned Bitwidth) const {
  return isIntegerTy() && cast<IntegerType>(this)->getBitWidth() == Bitwidth;
}

/// isIntOrIntVectorTy - Return true if this is an integer type or a vector of
/// integer types.
///
bool Type::isIntOrIntVectorTy() const {
  if (isIntegerTy())
    return true;
  if (ID != Type::VectorTyID) return false;
  
  return cast<VectorType>(this)->getElementType()->isIntegerTy();
}

/// isFPOrFPVectorTy - Return true if this is a FP type or a vector of FP types.
///
bool Type::isFPOrFPVectorTy() const {
  if (ID == Type::FloatTyID || ID == Type::DoubleTyID || 
      ID == Type::FP128TyID || ID == Type::X86_FP80TyID || 
      ID == Type::PPC_FP128TyID)
    return true;
  if (ID != Type::VectorTyID) return false;
  
  return cast<VectorType>(this)->getElementType()->isFloatingPointTy();
}

// canLosslesslyBitCastTo - Return true if this type can be converted to
// 'Ty' without any reinterpretation of bits.  For example, i8* to i32*.
//
bool Type::canLosslesslyBitCastTo(Type *Ty) const {
  // Identity cast means no change so return true
  if (this == Ty) 
    return true;
  
  // They are not convertible unless they are at least first class types
  if (!this->isFirstClassType() || !Ty->isFirstClassType())
    return false;

  // Vector -> Vector conversions are always lossless if the two vector types
  // have the same size, otherwise not.  Also, 64-bit vector types can be
  // converted to x86mmx.
  if (const VectorType *thisPTy = dyn_cast<VectorType>(this)) {
    if (const VectorType *thatPTy = dyn_cast<VectorType>(Ty))
      return thisPTy->getBitWidth() == thatPTy->getBitWidth();
    if (Ty->getTypeID() == Type::X86_MMXTyID &&
        thisPTy->getBitWidth() == 64)
      return true;
  }

  if (this->getTypeID() == Type::X86_MMXTyID)
    if (const VectorType *thatPTy = dyn_cast<VectorType>(Ty))
      if (thatPTy->getBitWidth() == 64)
        return true;

  // At this point we have only various mismatches of the first class types
  // remaining and ptr->ptr. Just select the lossless conversions. Everything
  // else is not lossless.
  if (this->isPointerTy())
    return Ty->isPointerTy();
  return false;  // Other types have no identity values
}

bool Type::isEmptyTy() const {
  const ArrayType *ATy = dyn_cast<ArrayType>(this);
  if (ATy) {
    unsigned NumElements = ATy->getNumElements();
    return NumElements == 0 || ATy->getElementType()->isEmptyTy();
  }

  const StructType *STy = dyn_cast<StructType>(this);
  if (STy) {
    unsigned NumElements = STy->getNumElements();
    for (unsigned i = 0; i < NumElements; ++i)
      if (!STy->getElementType(i)->isEmptyTy())
        return false;
    return true;
  }

  return false;
}

unsigned Type::getPrimitiveSizeInBits() const {
  switch (getTypeID()) {
  case Type::FloatTyID: return 32;
  case Type::DoubleTyID: return 64;
  case Type::X86_FP80TyID: return 80;
  case Type::FP128TyID: return 128;
  case Type::PPC_FP128TyID: return 128;
  case Type::X86_MMXTyID: return 64;
  case Type::IntegerTyID: return cast<IntegerType>(this)->getBitWidth();
  case Type::VectorTyID:  return cast<VectorType>(this)->getBitWidth();
  default: return 0;
  }
}

/// getScalarSizeInBits - If this is a vector type, return the
/// getPrimitiveSizeInBits value for the element type. Otherwise return the
/// getPrimitiveSizeInBits value for this type.
unsigned Type::getScalarSizeInBits() {
  return getScalarType()->getPrimitiveSizeInBits();
}

/// getFPMantissaWidth - Return the width of the mantissa of this type.  This
/// is only valid on floating point types.  If the FP type does not
/// have a stable mantissa (e.g. ppc long double), this method returns -1.
int Type::getFPMantissaWidth() const {
  if (const VectorType *VTy = dyn_cast<VectorType>(this))
    return VTy->getElementType()->getFPMantissaWidth();
  assert(isFloatingPointTy() && "Not a floating point type!");
  if (ID == FloatTyID) return 24;
  if (ID == DoubleTyID) return 53;
  if (ID == X86_FP80TyID) return 64;
  if (ID == FP128TyID) return 113;
  assert(ID == PPC_FP128TyID && "unknown fp type");
  return -1;
}

/// isSizedDerivedType - Derived types like structures and arrays are sized
/// iff all of the members of the type are sized as well.  Since asking for
/// their size is relatively uncommon, move this operation out of line.
bool Type::isSizedDerivedType() const {
  if (this->isIntegerTy())
    return true;

  if (const ArrayType *ATy = dyn_cast<ArrayType>(this))
    return ATy->getElementType()->isSized();

  if (const VectorType *VTy = dyn_cast<VectorType>(this))
    return VTy->getElementType()->isSized();

  if (!this->isStructTy()) 
    return false;

  // Opaque structs have no size.
  if (cast<StructType>(this)->isOpaque())
    return false;
  
  // Okay, our struct is sized if all of the elements are.
  for (subtype_iterator I = subtype_begin(), E = subtype_end(); I != E; ++I)
    if (!(*I)->isSized()) 
      return false;

  return true;
}

//===----------------------------------------------------------------------===//
//                          Primitive 'Type' data
//===----------------------------------------------------------------------===//

Type *Type::getVoidTy(LLVMContext &C) { return &C.pImpl->VoidTy; }
Type *Type::getLabelTy(LLVMContext &C) { return &C.pImpl->LabelTy; }
Type *Type::getFloatTy(LLVMContext &C) { return &C.pImpl->FloatTy; }
Type *Type::getDoubleTy(LLVMContext &C) { return &C.pImpl->DoubleTy; }
Type *Type::getMetadataTy(LLVMContext &C) { return &C.pImpl->MetadataTy; }
Type *Type::getX86_FP80Ty(LLVMContext &C) { return &C.pImpl->X86_FP80Ty; }
Type *Type::getFP128Ty(LLVMContext &C) { return &C.pImpl->FP128Ty; }
Type *Type::getPPC_FP128Ty(LLVMContext &C) { return &C.pImpl->PPC_FP128Ty; }
Type *Type::getX86_MMXTy(LLVMContext &C) { return &C.pImpl->X86_MMXTy; }

IntegerType *Type::getInt1Ty(LLVMContext &C) { return &C.pImpl->Int1Ty; }
IntegerType *Type::getInt8Ty(LLVMContext &C) { return &C.pImpl->Int8Ty; }
IntegerType *Type::getInt16Ty(LLVMContext &C) { return &C.pImpl->Int16Ty; }
IntegerType *Type::getInt32Ty(LLVMContext &C) { return &C.pImpl->Int32Ty; }
IntegerType *Type::getInt64Ty(LLVMContext &C) { return &C.pImpl->Int64Ty; }

IntegerType *Type::getIntNTy(LLVMContext &C, unsigned N) {
  return IntegerType::get(C, N);
}

PointerType *Type::getFloatPtrTy(LLVMContext &C, unsigned AS) {
  return getFloatTy(C)->getPointerTo(AS);
}

PointerType *Type::getDoublePtrTy(LLVMContext &C, unsigned AS) {
  return getDoubleTy(C)->getPointerTo(AS);
}

PointerType *Type::getX86_FP80PtrTy(LLVMContext &C, unsigned AS) {
  return getX86_FP80Ty(C)->getPointerTo(AS);
}

PointerType *Type::getFP128PtrTy(LLVMContext &C, unsigned AS) {
  return getFP128Ty(C)->getPointerTo(AS);
}

PointerType *Type::getPPC_FP128PtrTy(LLVMContext &C, unsigned AS) {
  return getPPC_FP128Ty(C)->getPointerTo(AS);
}

PointerType *Type::getX86_MMXPtrTy(LLVMContext &C, unsigned AS) {
  return getX86_MMXTy(C)->getPointerTo(AS);
}

PointerType *Type::getIntNPtrTy(LLVMContext &C, unsigned N, unsigned AS) {
  return getIntNTy(C, N)->getPointerTo(AS);
}

PointerType *Type::getInt1PtrTy(LLVMContext &C, unsigned AS) {
  return getInt1Ty(C)->getPointerTo(AS);
}

PointerType *Type::getInt8PtrTy(LLVMContext &C, unsigned AS) {
  return getInt8Ty(C)->getPointerTo(AS);
}

PointerType *Type::getInt16PtrTy(LLVMContext &C, unsigned AS) {
  return getInt16Ty(C)->getPointerTo(AS);
}

PointerType *Type::getInt32PtrTy(LLVMContext &C, unsigned AS) {
  return getInt32Ty(C)->getPointerTo(AS);
}

PointerType *Type::getInt64PtrTy(LLVMContext &C, unsigned AS) {
  return getInt64Ty(C)->getPointerTo(AS);
}


//===----------------------------------------------------------------------===//
//                       IntegerType Implementation
//===----------------------------------------------------------------------===//

IntegerType *IntegerType::get(LLVMContext &C, unsigned NumBits) {
  assert(NumBits >= MIN_INT_BITS && "bitwidth too small");
  assert(NumBits <= MAX_INT_BITS && "bitwidth too large");
  
  // Check for the built-in integer types
  switch (NumBits) {
  case  1: return cast<IntegerType>(Type::getInt1Ty(C));
  case  8: return cast<IntegerType>(Type::getInt8Ty(C));
  case 16: return cast<IntegerType>(Type::getInt16Ty(C));
  case 32: return cast<IntegerType>(Type::getInt32Ty(C));
  case 64: return cast<IntegerType>(Type::getInt64Ty(C));
  default: 
    break;
  }
  
  IntegerType *&Entry = C.pImpl->IntegerTypes[NumBits];
  
  if (Entry == 0)
    Entry = new (C.pImpl->TypeAllocator) IntegerType(C, NumBits);
  
  return Entry;
}

bool IntegerType::isPowerOf2ByteWidth() const {
  unsigned BitWidth = getBitWidth();
  return (BitWidth > 7) && isPowerOf2_32(BitWidth);
}

APInt IntegerType::getMask() const {
  return APInt::getAllOnesValue(getBitWidth());
}

//===----------------------------------------------------------------------===//
//                       FunctionType Implementation
//===----------------------------------------------------------------------===//

FunctionType::FunctionType(Type *Result, ArrayRef<Type*> Params,
                           bool IsVarArgs)
  : Type(Result->getContext(), FunctionTyID) {
  Type **SubTys = reinterpret_cast<Type**>(this+1);
  assert(isValidReturnType(Result) && "invalid return type for function");
  setSubclassData(IsVarArgs);

  SubTys[0] = const_cast<Type*>(Result);

  for (unsigned i = 0, e = Params.size(); i != e; ++i) {
    assert(isValidArgumentType(Params[i]) &&
           "Not a valid type for function argument!");
    SubTys[i+1] = Params[i];
  }

  ContainedTys = SubTys;
  NumContainedTys = Params.size() + 1; // + 1 for result type
}

// FunctionType::get - The factory function for the FunctionType class.
FunctionType *FunctionType::get(Type *ReturnType,
                                ArrayRef<Type*> Params, bool isVarArg) {
  // TODO: This is brutally slow.
  std::vector<Type*> Key;
  Key.reserve(Params.size()+2);
  Key.push_back(const_cast<Type*>(ReturnType));
  for (unsigned i = 0, e = Params.size(); i != e; ++i)
    Key.push_back(const_cast<Type*>(Params[i]));
  if (isVarArg)
    Key.push_back(0);
  
  LLVMContextImpl *pImpl = ReturnType->getContext().pImpl;
  FunctionType *&FT = pImpl->FunctionTypes[Key];
  
  if (FT == 0) {
    FT = (FunctionType*) pImpl->TypeAllocator.
      Allocate(sizeof(FunctionType) + sizeof(Type*)*(Params.size()+1),
               AlignOf<FunctionType>::Alignment);
    new (FT) FunctionType(ReturnType, Params, isVarArg);
  }

  return FT;
}


FunctionType *FunctionType::get(Type *Result, bool isVarArg) {
  return get(Result, ArrayRef<Type *>(), isVarArg);
}


/// isValidReturnType - Return true if the specified type is valid as a return
/// type.
bool FunctionType::isValidReturnType(Type *RetTy) {
  return !RetTy->isFunctionTy() && !RetTy->isLabelTy() &&
  !RetTy->isMetadataTy();
}

/// isValidArgumentType - Return true if the specified type is valid as an
/// argument type.
bool FunctionType::isValidArgumentType(Type *ArgTy) {
  return ArgTy->isFirstClassType();
}

//===----------------------------------------------------------------------===//
//                       StructType Implementation
//===----------------------------------------------------------------------===//

// Primitive Constructors.

StructType *StructType::get(LLVMContext &Context, ArrayRef<Type*> ETypes, 
                            bool isPacked) {
  // FIXME: std::vector is horribly inefficient for this probe.
  std::vector<Type*> Key;
  for (unsigned i = 0, e = ETypes.size(); i != e; ++i) {
    assert(isValidElementType(ETypes[i]) &&
           "Invalid type for structure element!");
    Key.push_back(ETypes[i]);
  }
  if (isPacked)
    Key.push_back(0);
  
  StructType *&ST = Context.pImpl->AnonStructTypes[Key];
  if (ST) return ST;
  
  // Value not found.  Create a new type!
  ST = new (Context.pImpl->TypeAllocator) StructType(Context);
  ST->setSubclassData(SCDB_IsLiteral);  // Literal struct.
  ST->setBody(ETypes, isPacked);
  return ST;
}

void StructType::setBody(ArrayRef<Type*> Elements, bool isPacked) {
  assert(isOpaque() && "Struct body already set!");
  
  setSubclassData(getSubclassData() | SCDB_HasBody);
  if (isPacked)
    setSubclassData(getSubclassData() | SCDB_Packed);
  
  Type **Elts = getContext().pImpl->
    TypeAllocator.Allocate<Type*>(Elements.size());
  memcpy(Elts, Elements.data(), sizeof(Elements[0])*Elements.size());
  
  ContainedTys = Elts;
  NumContainedTys = Elements.size();
}

void StructType::setName(StringRef Name) {
  if (Name == getName()) return;

  // If this struct already had a name, remove its symbol table entry.
  if (SymbolTableEntry) {
    getContext().pImpl->NamedStructTypes.erase(getName());
    SymbolTableEntry = 0;
  }
  
  // If this is just removing the name, we're done.
  if (Name.empty())
    return;
  
  // Look up the entry for the name.
  StringMapEntry<StructType*> *Entry =
    &getContext().pImpl->NamedStructTypes.GetOrCreateValue(Name);
  
  // While we have a name collision, try a random rename.
  if (Entry->getValue()) {
    SmallString<64> TempStr(Name);
    TempStr.push_back('.');
    raw_svector_ostream TmpStream(TempStr);
   
    do {
      TempStr.resize(Name.size()+1);
      TmpStream.resync();
      TmpStream << getContext().pImpl->NamedStructTypesUniqueID++;
      
      Entry = &getContext().pImpl->
                 NamedStructTypes.GetOrCreateValue(TmpStream.str());
    } while (Entry->getValue());
  }

  // Okay, we found an entry that isn't used.  It's us!
  Entry->setValue(this);
    
  SymbolTableEntry = Entry;
}

//===----------------------------------------------------------------------===//
// StructType Helper functions.

StructType *StructType::create(LLVMContext &Context, StringRef Name) {
  StructType *ST = new (Context.pImpl->TypeAllocator) StructType(Context);
  if (!Name.empty())
    ST->setName(Name);
  return ST;
}

StructType *StructType::get(LLVMContext &Context, bool isPacked) {
  return get(Context, llvm::ArrayRef<Type*>(), isPacked);
}

StructType *StructType::get(Type *type, ...) {
  assert(type != 0 && "Cannot create a struct type with no elements with this");
  LLVMContext &Ctx = type->getContext();
  va_list ap;
  SmallVector<llvm::Type*, 8> StructFields;
  va_start(ap, type);
  while (type) {
    StructFields.push_back(type);
    type = va_arg(ap, llvm::Type*);
  }
  return llvm::StructType::get(Ctx, StructFields);
}

StructType *StructType::create(LLVMContext &Context, ArrayRef<Type*> Elements,
                               StringRef Name, bool isPacked) {
  StructType *ST = create(Context, Name);
  ST->setBody(Elements, isPacked);
  return ST;
}

StructType *StructType::create(LLVMContext &Context, ArrayRef<Type*> Elements) {
  return create(Context, Elements, StringRef());
}

StructType *StructType::create(LLVMContext &Context) {
  return create(Context, StringRef());
}


StructType *StructType::create(ArrayRef<Type*> Elements, StringRef Name,
                               bool isPacked) {
  assert(!Elements.empty() &&
         "This method may not be invoked with an empty list");
  return create(Elements[0]->getContext(), Elements, Name, isPacked);
}

StructType *StructType::create(ArrayRef<Type*> Elements) {
  assert(!Elements.empty() &&
         "This method may not be invoked with an empty list");
  return create(Elements[0]->getContext(), Elements, StringRef());
}

StructType *StructType::create(StringRef Name, Type *type, ...) {
  assert(type != 0 && "Cannot create a struct type with no elements with this");
  LLVMContext &Ctx = type->getContext();
  va_list ap;
  SmallVector<llvm::Type*, 8> StructFields;
  va_start(ap, type);
  while (type) {
    StructFields.push_back(type);
    type = va_arg(ap, llvm::Type*);
  }
  return llvm::StructType::create(Ctx, StructFields, Name);
}


#if 1

StructType *StructType::createNamed(LLVMContext &Context, StringRef Name,
                                    ArrayRef<Type*> Elements, bool isPacked) {
  StructType *ST = createNamed(Context, Name);
  ST->setBody(Elements, isPacked);
  return ST;
}

StructType *StructType::createNamed(LLVMContext &Context, StringRef Name) {
  StructType *ST = new (Context.pImpl->TypeAllocator) StructType(Context);
  if (!Name.empty())
    ST->setName(Name);
  return ST;
}


StructType *StructType::createNamed(StringRef Name, ArrayRef<Type*> Elements,
                                    bool isPacked) {
  assert(!Elements.empty() &&
         "This method may not be invoked with an empty list");
  return createNamed(Elements[0]->getContext(), Name, Elements, isPacked);
}

StructType *StructType::createNamed(StringRef Name, Type *type, ...) {
  assert(type != 0 && "Cannot create a struct type with no elements with this");
  LLVMContext &Ctx = type->getContext();
  va_list ap;
  SmallVector<llvm::Type*, 8> StructFields;
  va_start(ap, type);
  while (type) {
    StructFields.push_back(type);
    type = va_arg(ap, llvm::Type*);
  }
  return llvm::StructType::createNamed(Ctx, Name, StructFields);
}
#endif

StringRef StructType::getName() const {
  assert(!isLiteral() && "Literal structs never have names");
  if (SymbolTableEntry == 0) return StringRef();
  
  return ((StringMapEntry<StructType*> *)SymbolTableEntry)->getKey();
}

void StructType::setBody(Type *type, ...) {
  assert(type != 0 && "Cannot create a struct type with no elements with this");
  va_list ap;
  SmallVector<llvm::Type*, 8> StructFields;
  va_start(ap, type);
  while (type) {
    StructFields.push_back(type);
    type = va_arg(ap, llvm::Type*);
  }
  setBody(StructFields);
}

bool StructType::isValidElementType(Type *ElemTy) {
  return !ElemTy->isVoidTy() && !ElemTy->isLabelTy() &&
         !ElemTy->isMetadataTy() && !ElemTy->isFunctionTy();
}

/// isLayoutIdentical - Return true if this is layout identical to the
/// specified struct.
bool StructType::isLayoutIdentical(StructType *Other) const {
  if (this == Other) return true;
  
  if (isPacked() != Other->isPacked() ||
      getNumElements() != Other->getNumElements())
    return false;
  
  return std::equal(element_begin(), element_end(), Other->element_begin());
}


/// getTypeByName - Return the type with the specified name, or null if there
/// is none by that name.
StructType *Module::getTypeByName(StringRef Name) const {
  StringMap<StructType*>::iterator I =
    getContext().pImpl->NamedStructTypes.find(Name);
  if (I != getContext().pImpl->NamedStructTypes.end())
    return I->second;
  return 0;
}


//===----------------------------------------------------------------------===//
//                       CompositeType Implementation
//===----------------------------------------------------------------------===//

Type *CompositeType::getTypeAtIndex(const Value *V) {
  if (StructType *STy = dyn_cast<StructType>(this)) {
    unsigned Idx = (unsigned)cast<ConstantInt>(V)->getZExtValue();
    assert(indexValid(Idx) && "Invalid structure index!");
    return STy->getElementType(Idx);
  }
  
  return cast<SequentialType>(this)->getElementType();
}
Type *CompositeType::getTypeAtIndex(unsigned Idx) {
  if (StructType *STy = dyn_cast<StructType>(this)) {
    assert(indexValid(Idx) && "Invalid structure index!");
    return STy->getElementType(Idx);
  }
  
  return cast<SequentialType>(this)->getElementType();
}
bool CompositeType::indexValid(const Value *V) const {
  if (const StructType *STy = dyn_cast<StructType>(this)) {
    // Structure indexes require 32-bit integer constants.
    if (V->getType()->isIntegerTy(32))
      if (const ConstantInt *CU = dyn_cast<ConstantInt>(V))
        return CU->getZExtValue() < STy->getNumElements();
    return false;
  }
  
  // Sequential types can be indexed by any integer.
  return V->getType()->isIntegerTy();
}

bool CompositeType::indexValid(unsigned Idx) const {
  if (const StructType *STy = dyn_cast<StructType>(this))
    return Idx < STy->getNumElements();
  // Sequential types can be indexed by any integer.
  return true;
}


//===----------------------------------------------------------------------===//
//                           ArrayType Implementation
//===----------------------------------------------------------------------===//

ArrayType::ArrayType(Type *ElType, uint64_t NumEl)
  : SequentialType(ArrayTyID, ElType) {
  NumElements = NumEl;
}


ArrayType *ArrayType::get(Type *elementType, uint64_t NumElements) {
  Type *ElementType = const_cast<Type*>(elementType);
  assert(isValidElementType(ElementType) && "Invalid type for array element!");
    
  LLVMContextImpl *pImpl = ElementType->getContext().pImpl;
  ArrayType *&Entry = 
    pImpl->ArrayTypes[std::make_pair(ElementType, NumElements)];
  
  if (Entry == 0)
    Entry = new (pImpl->TypeAllocator) ArrayType(ElementType, NumElements);
  return Entry;
}

bool ArrayType::isValidElementType(Type *ElemTy) {
  return !ElemTy->isVoidTy() && !ElemTy->isLabelTy() &&
         !ElemTy->isMetadataTy() && !ElemTy->isFunctionTy();
}

//===----------------------------------------------------------------------===//
//                          VectorType Implementation
//===----------------------------------------------------------------------===//

VectorType::VectorType(Type *ElType, unsigned NumEl)
  : SequentialType(VectorTyID, ElType) {
  NumElements = NumEl;
}

VectorType *VectorType::get(Type *elementType, unsigned NumElements) {
  Type *ElementType = const_cast<Type*>(elementType);
  assert(NumElements > 0 && "#Elements of a VectorType must be greater than 0");
  assert(isValidElementType(ElementType) &&
         "Elements of a VectorType must be a primitive type");
  
  LLVMContextImpl *pImpl = ElementType->getContext().pImpl;
  VectorType *&Entry = ElementType->getContext().pImpl
    ->VectorTypes[std::make_pair(ElementType, NumElements)];
  
  if (Entry == 0)
    Entry = new (pImpl->TypeAllocator) VectorType(ElementType, NumElements);
  return Entry;
}

bool VectorType::isValidElementType(Type *ElemTy) {
  return ElemTy->isIntegerTy() || ElemTy->isFloatingPointTy();
}

//===----------------------------------------------------------------------===//
//                         PointerType Implementation
//===----------------------------------------------------------------------===//

PointerType *PointerType::get(Type *EltTy, unsigned AddressSpace) {
  assert(EltTy && "Can't get a pointer to <null> type!");
  assert(isValidElementType(EltTy) && "Invalid type for pointer element!");
  
  LLVMContextImpl *CImpl = EltTy->getContext().pImpl;
  
  // Since AddressSpace #0 is the common case, we special case it.
  PointerType *&Entry = AddressSpace == 0 ? CImpl->PointerTypes[EltTy]
     : CImpl->ASPointerTypes[std::make_pair(EltTy, AddressSpace)];

  if (Entry == 0)
    Entry = new (CImpl->TypeAllocator) PointerType(EltTy, AddressSpace);
  return Entry;
}


PointerType::PointerType(Type *E, unsigned AddrSpace)
  : SequentialType(PointerTyID, E) {
  setSubclassData(AddrSpace);
}

PointerType *Type::getPointerTo(unsigned addrs) {
  return PointerType::get(this, addrs);
}

bool PointerType::isValidElementType(Type *ElemTy) {
  return !ElemTy->isVoidTy() && !ElemTy->isLabelTy() &&
         !ElemTy->isMetadataTy();
}
