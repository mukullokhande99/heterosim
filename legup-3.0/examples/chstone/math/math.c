#include "snipmath.h"
#include <math.h>
#include <stdio.h>
//#include "cubic.c"
#include "isqrt.c"
#include "rad2deg.c"

/* The printf's may be removed to isolate just the math calculations */

int main(void)
{
  double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
  double  a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
  double  a3 = 1.0, b3 = -3.5, c3 = 22.0, d3 = -31.0;
  double  a4 = 1.0, b4 = -13.7, c4 = 1.0, d4 = -35.0;
  double  x[3];
  double X;
  int     solutions;
  int i;
  unsigned int l = 0x3fed0169L;
  struct int_sqrt q;
  long n = 0;

  /* solve soem cubic functions */
/*  printf("********* CUBIC FUNCTIONS ***********\n");

  SolveCubic(a1, b1, c1, d1, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  SolveCubic(a2, b2, c2, d2, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");
  SolveCubic(a3, b3, c3, d3, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");
  SolveCubic(a4, b4, c4, d4, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[i]);
  printf("\n");

  for(a1=1;a1<10;a1++) {
    for(b1=10;b1>0;b1--) {
      for(c1=5;c1<15;c1+=0.5) {
	for(d1=-1;d1>-11;d1--) {
	  SolveCubic(a1, b1, c1, d1, &solutions, x);  
	  printf("Solutions:");
	  for(i=0;i<solutions;i++)
	    printf(" %f",x[i]);
	  printf("\n");
	}
      }
    }
  }
  */
  printf("********* INTEGER SQR ROOTS ***********\n");
  /* perform some integer square roots */
  int size0,size1;
  int a;
  size0 = 4; size1 = 8;
  int num[100];
  for (i = 0; i < 100; ++i)
  { num[i]=i;
          *((volatile unsigned int *)0xf000f008) = (unsigned int)(&num[i]);
      *((volatile unsigned int *)0xf000f00C) = 4;

   }   


  for (i = 0; i < 100; ++i)
    { 
      a = usqrt(&num[i],size0, &q, size1);
			// remainder differs on some machines
     // printf("sqrt(%3d) = %2d, remainder = %2d\n",
     //printf("sqrt(%3d) = %2d\n",
	//     i, q.sqrt);
    }

      *((volatile unsigned int *)0xf000f008) = (unsigned int)(&l);
      *((volatile unsigned int *)0xf000f00C) = 4;
    a = usqrt(&l,size0, &q,size1);
  //printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);
  //printf("\nsqrt(%lX) = %X\n", l, q.sqrt);


 // printf("********* ANGLE CONVERSION ***********\n");
  /* convert some rads to degrees */
  for (X = 0.0; X <= 360.0; X += 1.0)
      deg2rad(X);
    //printf("%3.0f degrees = %.12f radians\n", X, deg2rad(X));
  //puts("");
  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180))
rad2deg(X);
    //printf("%.12f radians = %3.0f degrees\n", X, rad2deg(X));
  
  
  return 0;
}
