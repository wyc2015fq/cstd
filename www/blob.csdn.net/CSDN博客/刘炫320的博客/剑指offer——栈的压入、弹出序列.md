# 剑指offer——栈的压入、弹出序列 - 刘炫320的博客 - CSDN博客
2017年07月14日 18:56:42[刘炫320](https://me.csdn.net/qq_35082030)阅读数：517标签：[栈																[剑指offer-java实现																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)](https://so.csdn.net/so/search/s.do?q=栈&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否为该栈的弹出顺序。假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4，5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。（注意：这两个序列的长度是相等的）
# 2. 解决方案
## 2.1 level1
这其实也是在学数据结构的时候一道经典例题。它就是讲解了栈的特性。如果我们拿到手后，首先想的是它的表现出的性质。那么通过观察我们可以看出这样一个特性：
> 
入栈的最后一个数，如果出现在出栈的队列中，那么这个数的左边是按照原有顺序排放的，而右边则是按照倒序排放的。即左边的数在入栈的队列中的相对位置不发生变化，而右边的数在入栈的队列中的相对位置恰好相反。
于是我们就能写出这个算法来了：
```
public static boolean IsPopOrder(int [] pushA,int [] popA) {
          //最后一个入的左边是顺序，右边是倒序。
        List<Integer> leftlist=new ArrayList<Integer>();
        List<Integer> rightlist=new ArrayList<Integer>();
        List<Integer> pushAlist=new ArrayList<Integer>();
        for(int i=0;i<pushA.length-1;i++){
            pushAlist.add(pushA[i]);
        }
        int last=pushA[pushA.length-1];
        int flag=0;
        for(int i=0;i<popA.length;i++){
            if(popA[i]==last){
                flag=1;
            }
            else{
                if(flag==0){
                    leftlist.add(popA[i]);
                }
                else{
                    rightlist.add(popA[i]);
                }
            }
        }
        if(flag==0){
            return false;
        }
        else{
            return (isorder(leftlist,pushAlist)&&isreorder(rightlist,pushAlist));
        }
    }
    public static boolean isorder(List<Integer> Aim,List<Integer> push){
        if(Aim.size()==0||Aim.size()==1)
            return true;
        else{
            for(int i=0;i<Aim.size()-1;i++){
            if(push.indexOf(Aim.get(i))>push.indexOf(Aim.get(i+1))){
                return false;
                }
            }
            return true;
        }
    }
    public static boolean isreorder(List<Integer> Aim,List<Integer> push){
        if(Aim.size()==0||Aim.size()==1)
            return true;
        else{
            for(int i=0;i<Aim.size()-1;i++){
            if(push.indexOf(Aim.get(i))<push.indexOf(Aim.get(i+1))){
                return false;
                }
            }
            return true;
        }
    }
```
## 2.2 level2
上面看起来很复杂， 其主要原因有，第一数组形式不容易操作，第二，判断相对位置不发生变化很复杂。其实，这个题目完全可以使用栈来实操一下，这样就知道是否存在这种解了：
```java
public static boolean IsPopOrder(int [] pushA,int [] popA) {
        if(pushA.length == 0 || popA.length == 0)
            return false;
        Stack<Integer> s = new Stack<Integer>();
        //用于标识弹出序列的位置
        int popIndex = 0;
        for(int i = 0; i< pushA.length;i++){
            s.push(pushA[i]);
            //如果栈不为空，且栈顶元素等于弹出序列
            while(!s.empty() &&s.peek() == popA[popIndex]){
                //出栈
                s.pop();
                //弹出序列向后一位
                popIndex++;
            }
        }
        return s.empty();
    }
```
# 3. 小结
从这题我们可以看出两种思路，第一种思路是根据结果找性质，找到性之后，就很容易解答了。第二种就是我实际操作一遍，如果这个理论是符合实践的，我就认为是正确的。这两种方法在科学实践中都是非常常用的思维方式。
