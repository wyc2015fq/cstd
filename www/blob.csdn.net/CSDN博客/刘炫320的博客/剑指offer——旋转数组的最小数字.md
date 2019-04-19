# 剑指offer——旋转数组的最小数字 - 刘炫320的博客 - CSDN博客
2017年03月22日 20:15:01[刘炫320](https://me.csdn.net/qq_35082030)阅读数：365标签：[算法																[查找																[二分法																[剑指offer																[旋转数组的最小数字](https://so.csdn.net/so/search/s.do?q=旋转数组的最小数字&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=二分法&t=blog)](https://so.csdn.net/so/search/s.do?q=查找&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 
  输入一个非递减排序的数组的一个旋转，输出旋转数组的最小元素。 
  例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 
  NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。
# 2.求解方法
这个题目第一眼看过去，就是查找算法。如果你首先想到的是遍历，那么还需要多思考思考，如果你首先想到的是找特性，那么还可以，如果你首先想到的是基于比较查找的下限$O（logn）$，那么恭喜你，想到了比较好的算法。
这就是Level1、Level2、Level3的算法。
## 2.1 Level1
遍历一遍，找到最小值。
这个方法应该大家都会，如果对于时间要求不是太高，我们可以这么做。
```java
public static int minNumberInRotateArray0(int [] array) {
        int minnum=0;
        if(array.length==0){
            return minnum;
        }
        else{
            minnum=array[0];
            for(int i=0;i<array.length;i++){
                if(array[i]<minnum){
                    minnum=array[i];
                }
            }
            return minnum;
        }
    }
```
## 2.2 Level2
找到它的特性，就是下一个比上一个会小，那么下一个就是最小值。
如果使用这个方法，那么比上一个方法会优秀很多，因为它可以提前终止遍历，从而减少运行时间。
```java
public static int minNumberInRotateArray1(int [] array) {
        int minnum=0;
        if(array.length==0){
            return minnum;
        }
        else{
            minnum=array[0];
            for(int i=0;i<array.length-1;i++){
                //如果下一个比上一个还要小，那么肯定下一个就是最小值
                if(array[i]>array[i+1]){
                    minnum=array[i+1];
                    break;
                }
            }
            return minnum;
        }
    }
```
## 2.3 Level3
如果是想到基于比较的查找下限$O（logn）$，那说明你是一个追求极致优化的人，首先想到的是目前可执行的最优方法。
```java
public static int minNumberInRotateArray2(int [] array) {
        if(array.length==0){
            return 0;
        }
        else{
                //第一个就是中间变量
                int midnum=array[0];
                int start=0;
                int end=array.length-1;
                //最终局面start比end小1，而且肯定是start大于midnum，end小于等于midnum,最终小的肯定在end
                while(start<end-1){
                    int mid=(start+end)/2;
                    //如果中间比开始的大，说明符合规律，突变在后面
                    if(array[mid]>midnum){
                        start=mid;
                    }
                    //否则说明突变在前面
                    else {
                        end=mid;
                    }
                }
                return array[end];
        }
    }
```
# 3. 算法分析
## 3.1 就事论事
如果我们使用传统的时间复杂度的分析来看，你就知道这3个方法的差距是多少。
第一种方案，无论什么情况下，都需要执行n次（最少是n-1次，第一个不用比较，但不影响大局），其中每个循环中，需要比较1次，赋值1次，总共是2n次。
第二种方案，最好情况下，执行一次就结束，最坏情况下，需要执行n-1次，平均就是$\frac {n}{2} $次，其中每个循环中，需要比较1次，赋值1次，平均执行n次。
第三种方案，二分查找，执行次数为$log(n)$次，每个循环中，需要比较2次，赋值2次，共计是$4 \times log(n)$次。 
可能我们并不能直观的感觉到$n和log(n)$的数量级上面的差距，下面我们给出一幅图，你就可以知道他们的差距了。 
![每个数据的比较值](https://img-blog.csdn.net/20170322200240847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过这个图大家可以直观的感受到他们之间的差距了吧。对于$log(n)$来说，近乎是一条直线，现在图上是n取1-1000的范围，差距就是如此之大，可想而知如果数据量超大的时候，他们之间的差距是多少。当n是1000时，$4 \times log(n)$只是39.86，仅此而已。
## 3.2 思维层面
我们在考虑一个算法题目时，如果是在第一层面，说明我们的思维还是喜欢简单粗暴的，尽可能减少我们的思考量，而把重复的工作交给计算机。这其实很容易解释。 
第一点，我们人本身就使用经验做事，能不想的尽量不想。做什么事情，都是先为我们自己考虑。
第二点，现在我们又处于一个计算能力过剩的年代，对于个人开发者来讲，很难接触到超大规模数据的应用，因此对于超大规模数据的复杂程度还没有一个直观的了解。因此对于数据优化并不是那么潜意识。
那么如果到了第二层面，那说明至少我们的脑海里出现过第一层面的算法，而想进一步优化。这是个好的习惯，不安于现状，积极进取。但是优化程度只是通过发现一些局部的特性，只能在一定程度上减少复杂度，并不能发生质的变化。
如果到了第三层面，有可能刚开始根本没有考虑第一层、第二层。而是直接奔着最优算法去的。这种思想是比较好的，至少可以把事情在第一时间想到最优的办法。从而在接下来的时间里节省大量的精力。这其实就要求我们对于经典算法的要十分熟悉才行。
# 4.小结
好了，今天的算法就到这了。大家可以体会一下。
对于第一种方案，只是运用了它的基本性质，这种性质具有普适性，查找与遍历。
而第二种方案，则是找到了一个局部特性，那就是突变点在那个下一个比上一个小的地方。
第三种方案，除了使用二分查找以外，还掌握了全局特性，那就是在这个数组中，变化点前的数都比第一个数要大于等于，而变化点后的数都要比第一个数小于等于。这样通过二分查找就很容易找到突变点，也就是最小点。
