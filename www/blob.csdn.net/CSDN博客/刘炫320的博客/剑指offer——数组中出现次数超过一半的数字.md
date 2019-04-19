# 剑指offer——数组中出现次数超过一半的数字 - 刘炫320的博客 - CSDN博客
2017年07月07日 19:33:30[刘炫320](https://me.csdn.net/qq_35082030)阅读数：289标签：[剑指offer-java实现																[超过一半的数																[hashmap底层实现原理																[快速排序](https://so.csdn.net/so/search/s.do?q=快速排序&t=blog)](https://so.csdn.net/so/search/s.do?q=hashmap底层实现原理&t=blog)](https://so.csdn.net/so/search/s.do?q=超过一半的数&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组{1,2,3,2,2,2,5,4,2}。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。如果不存在则输出0。
# 2. 解答方法
这是一个非常古老的题目了，但是仍然十分经典。
## 2.1 level1
这个题目按照普通的想法，应该是进行字典统计，在之前一直喜欢用数组，但是数组还是比较慢的，比较好的方法是使用HashMap,这样速度会快一些：
```cpp
public static int MoreThanHalfNum_Solution(int [] array) {
        if(array.length==0||array==null)
            return 0;
        Map<Integer,Integer> map=new HashMap<Integer,Integer>();
        for(int i=0;i<array.length;i++){
            if(map.containsKey(array[i])){
                map.put(array[i], map.get(array[i])+1);
            }else{
                map.put(array[i], 1);
            }
        }
        for (Map.Entry<Integer, Integer> entry : map.entrySet()) {  
            if(entry.getValue()>array.length/2)
                return entry.getKey();
        }  
        return 0;
    }
```
## 2.2 level2
上面那种算法，需要的时间复杂度是O（n），空间复杂度是O（n），算是比较优良的解法了，那么我们还有没有更好的方法呢？当然有，这里有一个比较奇特的想法，那就是使用排序来辅助，只需要排序好以后，再去找到中间的那个，统计中间那个的个数即可。
```
public  static int MoreThanHalfNum_Solution(int [] array) {
        if(array.length == 0 || array == null){
            return 0;
        }
        java.util.Arrays.sort(array);
        int mid = array[array.length/2];
        int j = 0;
        for (int i : array){
            if (i == mid){
                j++;
            }
        }
        return j > array.length/2 ? mid : 0;
    }
```
这样是时间复杂度是O（nlogn）(如果是快排的话)，空间复杂度是O（1）,当然，还有更加巧妙的算法了。
## 2.3 level3
更加巧妙的算法是注意到了题目给出的特定条件，那就是这个数要比其他所有的数目之和还要多，如果是1：1对对碰消失的话，如果有一个超过半数的数，那么它一定是剩下来的数。于是就有了“打擂台”的方法：
```
public static int MoreThanHalfNum_Solution(int [] array) {
        if(array.length==0){
            return 0;
        }
        int count=1;
        int aim=array[0];
        for(int i=1;i<array.length;i++){
            if(aim==array[i]){
                count++;
            }
            else{
                count--;
                if(count==-1){
                    aim=array[i];
                    count=0;
                }
            }
        }
        if(count!=0){
            return aim;
        }
        else{
            return 0;
        }
    }
```
这样时间复杂度是O（n）,空间复杂度是O（1）。基本上是最优的了。
# 3. 小结
通过上述的解答层次，我们逐渐明白要充分利用题目给出的信息量，任何一个有用的特征都有助于减少题目的熵，这样我们就能用更加简单的方法来解决问题。
