# 剑指offer——二维数组中的查找 - 刘炫320的博客 - CSDN博客
2017年07月02日 23:25:36[刘炫320](https://me.csdn.net/qq_35082030)阅读数：317
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
# 2. 解答思路
由题目可知，这个题目有2个天然顺序，也就是从左到右是递增的，从上到下是递增的。由信息熵理论也可以知道，这条信息的信息熵并不大，因为我们需要的是有辨别度的信息，也就是说，如果数目大了会去哪，数目小了会去哪，这样我们才能够逐步缩小我们的搜索范围。因此，如果题目这样描述，从左到右是递增的，从下到上是递减的，那么我们很容易得出从左下角往右上角进行比较。
## 2.1 level1
当然，如果使用暴力求解的话，其实时间复杂度也并不大，比如逐行使用二分检索，如果假定这个矩阵实$n \times m$的话，那么时间复杂度为O（nlogm），我们下面给出代码：
```java
public boolean Find(int target，int [][] array) {         
        for(int i=0;i<array.length;i++){
            int low=0;
            int high=array[i].length-1;
            while(low<=high){
                int mid=(low+high)/2;
                if(target>array[i][mid])
                    low=mid+1;
                else if(target<array[i][mid])
                    high=mid-1;
                else
                    return true;
            }
        }
        return false;
    }
```
## 2.1 level2
但是事实上，这种方式肯定不是最优解，但是仍然可以接受的。不过在前面我们已经提供了一个非常正常的想法，那就是从左下角或者右上角开始，我们这里以左下角开始为例，如果目标数比当前数大，那么它就向左移一格，如果比当前数目小，那么就向上一格。这样就可以遍历到整个数组了。下面给出代码：
```cpp
public boolean Find(int target, int [][] array) {
        int d1=array.length;
        int x=d1-1,y=0;
        int d2=array[x].length;
        while(x>=0&&y<d2){
            if(target==array[x][y])
                return true;
            else if(target>array[x][y]){
                y++;
            }
            else{
                x--;
            }
        }
        return false;
    }
```
## 2.3 level3
当然，还有说上面那种并非是最优解，下面这种思路才是，通过行列二分的手段，找到二维数组中近似的中位数，然后再比较与中位数的大小，如果比近似中位数大，则分别在近似中位数的下边，右下和右边3块区域递归查找，否则在其左边，左上和上面3块区域递归查找，时间复杂度在$O(log_4(n*m))$，应该会好一些。代码如下所示：
```java
public static boolean Find(int target, int[][] array, int rfront, int cfront, int rend, int cend) {
        if(rfront > rend || cfront > cend) {
            return false;
        }
        int rmid = (rfront+rend)/2;
        int cmid = (cfront+cend)/2;
        if(target == array[rmid][cmid]) {
            return true;
        }
        else if(target > array[rmid][cmid]) {
            if(Find(target, array, rmid+1, cfront, rend, cmid) ||
               Find(target, array, rfront, cmid+1, rmid, cend) ||
               Find(target, array, rmid+1, cmid+1, rend, cend)) {
                return true;
            }
        }
        else {
            if(Find(target, array, rmid, cfront, rend, cmid-1) ||
               Find(target, array, rfront, cmid, rmid-1, cend) ||
               Find(target, array, rfront, cfront, rmid-1, cmid-1)) {
                return true;
            }
        }
        return false;
    }
    public static boolean Find3(int target, int [][] array) {
        if(array==null) {
            return false;
        }   
        return Find(target, array, 0, 0, array.length-1, array[0].length-1);
    }
```
## 2.4 level4
但是这里忽略了一个重要的问题，那就是这个二维数组是不是矩阵，也就是说是不是每行都一样长，如果不一样长的话，则只有第一种遍历每行二分查找还有用了，其他的都没有用了。下面提供第二种的非矩阵的算法：
```java
public boolean Find2(int target, int [][] array) {
        int r=array.length-1,c=0,maxCol=0;
        for (int i=0;i<=r;i++)
            if (array[i].length>maxCol)
                maxCol=array[i].length;//元素最多的一行，元素数量为maxCol
        while (r>=0&&c<maxCol){
            if (c >= array[r].length)r--; //若该行元素较少，不存在第c列元素，继续上移；
            else if (array[r][c]<target)c++;
            else if (array[r][c]>target)r--;
            else if (array[r][c]==target)return true;
        }
        return false;
    }
```
# 3 小结
这样，我们这道二维数组中的查找就算解答完毕了，事实上，这道题还是可以使用字符串匹配的方式来做的，这里除了我了解了java自带的contains是多么强大外，另外也知道了很多种字符串匹配的方法，有关内容将会在后面的文章中给出，例如我们最常见的朴素查找、KMP算法，还有像BM算法，Horspool算法和Sunday算法等，以及AC自动机算法都是非常优秀的算法。今天就到这里了！下次见！
