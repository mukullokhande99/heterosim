/*
+--------------------------------------------------------------------------+
| CHStone : a suite of benchmark programs for C-based High-Level Synthesis |
| ======================================================================== |
|                                                                          |
| * Collected and Modified : Y. Hara, H. Tomiyama, S. Honda,               |
|                            H. Takada and K. Ishii                        |
|                            Nagoya University, Japan                      |
|                                                                          |
| * Remark :                                                               |
|    1. This source code is modified to unify the formats of the benchmark |
|       programs in CHStone.                                               |
|    2. Test vectors are added for CHStone.                                |
|    3. If "main_result" is 0 at the end of the program, the program is    |
|       correctly executed.                                                |
|    4. Please follow the copyright of each benchmark program.             |
+--------------------------------------------------------------------------+
*/
/*
 * Copyright (C) 2008
 * Y. Hara, H. Tomiyama, S. Honda, H. Takada and K. Ishii
 * Nagoya University, Japan
 * All rights reserved.
 *
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis. The authors disclaims any and all warranties, 
 * whether express, implied, or statuary, including any implied warranties or 
 * merchantability or of fitness for a particular purpose. In no event shall the
 * copyright-holder be liable for any incidental, punitive, or consequential damages
 * of any kind whatsoever arising from the use of these programs. This disclaimer
 * of warranty extends to the user of these programs and user's customers, employees,
 * agents, transferees, successors, and assigns.
 *
 */
#include <stdio.h>
//#include "softfloat.h"
#include "softfloat.c"
double
ullong_to_double (unsigned long long x)
{
  union
  {
    double d;
    unsigned long long ll;
  } t;

  t.ll = x;
  return t.d;
}

/*
+--------------------------------------------------------------------------+
| * Test Vectors (added for CHStone)                                       |
|     a_input, b_input : input data                                        |
|     z_output : expected output data                                      |
+--------------------------------------------------------------------------+
*/
#define N 46
const float64 a_input[N] = {
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xC000000000000000ULL		/* -2.0 */
};

const float64 b_input[N] = {
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL,	/* 1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x7FF0000000000000ULL,	/* inf */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xFFF0000000000000ULL,	/* -inf */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xC000000000000000ULL,	/* -2.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFF0000000000000ULL,	/* -1.0 */
  0x8000000000000000ULL,	/* -0.0 */
  0xBFF8000000000000ULL,	/* -1.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x4000000000000000ULL,	/* 2.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x0000000000000000ULL,	/* 0.0 */
  0x3FF8000000000000ULL		/* 1.5 */
};

const float64 z_output[N] = {
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x400C000000000000ULL,	/* 3.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x4000000000000000ULL,	/* 2.0 */
  0x400C000000000000ULL,	/* 3.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x0000000000000000ULL,	/* 0.0 */
  0x4004000000000000ULL,	/* 2.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC00C000000000000ULL,	/* -3.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xC000000000000000ULL,	/* -2.0 */
  0xC00C000000000000ULL,	/* -3.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0x8000000000000000ULL,	/* -0.0 */
  0xC004000000000000ULL,	/* -2.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FFFFFFFFFFFFFFFULL,	/* nan */
  0x0000000000000000ULL,	/* 0.0 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFE0000000000000ULL,	/* -0.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FE0000000000000ULL,	/* 0.5 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FFFFFFFFFFFFFFFULL,	/* nan */
  0x0000000000000000ULL,	/* 0.0 */
  0x7FF8000000000000ULL,	/* nan */
  0x7FF0000000000000ULL,	/* inf */
  0x3FF0000000000000ULL,	/* 1.0 */
  0x3FE0000000000000ULL,	/* 0.5 */
  0xFFF8000000000000ULL,	/* nan */
  0xFFF0000000000000ULL,	/* -inf */
  0xBFF0000000000000ULL,	/* -1.0 */
  0xBFE0000000000000ULL		/* -0.5 */
};

  float64 buff[92];
  float64 result[46];

int
main ()
{
  int main_result;
  int i;
  int m,n,a;
  m = 16; n=8;

      main_result = 0;

	for(i=0;i<N;i++) 
       { 
          buff[i*2+0] = a_input[i];
	  buff[i*2+1] = b_input[i];

        }
     
       *((volatile unsigned int *)0xf000f008) = (unsigned int)(&buff[0]);
       *((volatile unsigned int *)0xf000f00C) = 8*2*N;  
     
       for(i=0;i<N;i++) 
          a = float64_add (&buff[i*2],m, &result[i],n);       
     
       for(i=0;i<N;i++)
         { 
          printf("%llx ",result[i]);
	  main_result += (result[i] != z_output[i]);
         }
	 // printf
	 //   ("a_input=%016llx b_input=%016llx expected=%016llx output=%016llx (%lf)\n",
	  //   a_input[i], b_input[i], z_output[i], result,
	   //  ullong_to_double (result));
   printf("\n");
      printf ("%d\n", main_result);
      return main_result;
   }






