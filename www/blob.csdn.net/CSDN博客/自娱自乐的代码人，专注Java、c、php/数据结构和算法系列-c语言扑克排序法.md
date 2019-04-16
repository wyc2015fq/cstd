# 数据结构和算法系列 - c语言扑克排序法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 12:32:30[initphp](https://me.csdn.net/initphp)阅读数：1568








> 
插入排序算法类似于玩扑克时抓牌的过程，玩家每拿到一张牌都要插入到手中已有的牌里，使之从小到大排好序。






```cpp
#include <stdio.h>
void main() {
  printf("扑克牌排序算法：\n");
  int arr[5] = {1,200,3,45,2};
  int i, k, j;
  for (i = 0; i < 5; i++) {
     k = arr[i];
     j = i - 1;
     while(j >= 0 && arr[j] > k) {
        arr[j+1] = arr[j];
        j--;
     }
     arr[j+1] = k;
  }
  printf("%d %d %d %d %d", arr[0], arr[1], arr[2], arr[3], arr[4]);
}
```









