#include <stdio.h>
//结果就是在end表示之前的元素都找到了匹配，而end元素之后的元素都未被匹配
int inter_i23(int size1, int* array1, int size2, int* array2)
{
  int end = size1;
  int i, j;
  bool swap = false;//标识变量，表示两种情况中的哪一种
  for (i = 0 ; i < end;) {
    swap = false;//开始假设是第一种情况
    for (j = i ; j < size2; j++) { //找到与该元素存在相同的元素，将这个相同的元素交换到与该元素相同下标的位置上
      if (array1[i] == array2[j]) { //第二种情况，找到了相等的元素
        int tmp = array2[i];//对数组2进行交换
        array2[i] = array2[j];
        array2[j] = tmp;
        swap = true;//设置标志
        break;
      }
    }
    if (swap != true) { //第一种情况，没有相同元素存在时，将这个元素交换到尚未进行比较的尾部
      int tmp = array1[i];
      array1[i] = array1[--end];
      array1[end] = tmp;
    }
    else {
      i++;
    }
  }
  return end;
}
int test_inter()
{
  int i;
  //定义两个数组
  int array1[] = {7, 1, 2, 5, 4, 3, 5, 6, 3, 4, 5, 6, 7, 3, 2, 5, 6, 6};
  int size1 = 18;
  int array2[] = {8, 2, 1, 3, 4, 5, 3, 2, 4, 5, 3, 2, 1, 3, 5, 4, 6, 9};
  int size2 = 18;
  int end = inter_i23(size1, array1, size2, array2);
  //先输出差集
  printf("only in array1\n");
  for (i = end ; i < size1; i++) {
    printf("%d ", array1[i]);
  }
  printf("\n");
  printf("only in array2\n");
  for (i = end ; i < size2; i++) {
    printf("%d ", array2[i]);
  }
  printf("\n");
  //输出交集
  printf("elements in array1 and array2\n");
  for (i = 0 ; i < end ; i++) {
    printf("%d ", array1[i]);
  }
  printf("\n");
  //输出并集
  printf("elements in array1 or array2\n");
  for (i = 0 ; i < end ; i++) {
    printf("%d ", array1[i]);
  }
  for (i = end ; i < size1; i++) {
    printf("%d ", array1[i]);
  }
  for (i = end ; i < size2; i++) {
    printf("%d ", array2[i]);
  }
  printf("\n");
  return 0;
}

