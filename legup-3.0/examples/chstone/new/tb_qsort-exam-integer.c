#include "qsort-exam-integer.c"


int main()
{

  int arr[21] = {
  5, 4, 103, 11, 57, 100, 231, 111, 495, 99,
  10, 150, 22222, 101, 77, 44, 35, 2054, 9999, 8888,20 };

 // int arr1[21] = {7, 1, 16, 19, 6 ,11, 17, 3, 8, 14, 10, 13, 5, 12, 9, 0, 15, 2, 4,  18, 20};
 
  int i,r;
  
  //sort(20, arr);

  int size0 = 84; int size1= 80;

       *((volatile unsigned int *)0xf000f008) = (unsigned int)(arr);
       *((volatile unsigned int *)0xf000f00C) = 84;  
    

  r = sort(arr, size0, arr, size1 );
 // r = sort(arr1, size0, arr1, size1 );

  for(i=0;i<20;i++)
  {
    printf("%d		\n", arr[i]);
  }
 // for(i=0;i<20;i++)
 // {
  //  printf("%d		\n", arr1[i]);
 // }
  return 0;
}
