# 剑指offer——二维数组中的查找 - westbrook1998的博客 - CSDN博客





2018年02月28日 13:50:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46标签：[算法																[查找																[数组																[最短路径](https://so.csdn.net/so/search/s.do?q=最短路径&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/westbrook1998/article/category/7472814)





show the code

```java
/**
     * 暴力法遍历二维数组
     * 运行时间：217ms
     * 占用内存：16632K
     * @param target
     * @param array
     * @return
     */
    public boolean Find(int target, int [][] array) {
        int m=array.length;
        int n=array[0].length;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(target==array[i][j]){
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * 利用有序的条件使用二分查找,并结合了最短路径
     * 运行时间：200ms
     * 占用内存：15768k
     * @param target
     * @param array
     * @return
     */
    public boolean Find_two(int target,int[][] array){
        int m=array.length;
        int n=array[0].length;
        int i=m-1,j=0;
        while(i>=0 && i<m && j>=0 && j<n){
            if(target==array[i][j]){
                return true;
            }
            else if(target>array[i][j]){
                j++;
            }
            else{
                i--;
            }
        }
        return false;
    }
```](https://so.csdn.net/so/search/s.do?q=数组&t=blog)](https://so.csdn.net/so/search/s.do?q=查找&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




