
// #include <algorithm>
int test_partial_sort()
{
  int a[]={8,10,9,7,6,5,4,3,2,1,0};
  int i, n=countof(a);
  
  //前6个最小元素排序
  PARTIAL_SORT(n,a,6,LESS,int);
  //SORT_HEAP(n,a,LESS,int);
  for(i=0;i<n;++i){
    printf("%d ",a[i]);
  }
  return 0;
}
