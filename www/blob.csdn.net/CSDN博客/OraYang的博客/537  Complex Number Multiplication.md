
# 537. Complex Number Multiplication - OraYang的博客 - CSDN博客

2017年12月06日 10:54:09[OraYang](https://me.csdn.net/u010665216)阅读数：184标签：[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[stream																](https://so.csdn.net/so/search/s.do?q=stream&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[math																](https://so.csdn.net/so/search/s.do?q=math&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=stream&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=stream&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=stream&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=string&t=blog)



## 题目
Given two strings representing two complex numbers.
You need to return a string representing their multiplication. Note i2 = -1 according to the definition.
Example 1:
Input: “1+1i”, “1+1i”
Output: “0+2i”
Explanation: (1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i, and you need convert it to the form of 0+2i.
Example 2:
Input: “1+-1i”, “1+-1i”
Output: “0+-2i”
Explanation: (1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i, and you need convert it to the form of 0+-2i.
Note:
The input strings will not have extra blank.
The input strings will be given in the form of a+bi, where the integer a and b will both belong to the range of [-100, 100]. And the output should be also in this form.
## 思路
这道题目本质上是求解实数相乘，输入格式是a+bi形式的字符串格式。我们花了不到一分钟的时间就构造出了解决问题的总体架构：
> 首先将每个字符串的整数部分分隔开

> 然后按照实数相乘的法则，求对应部分的乘积和

> 最后将结果以字符串形式表示
那么我们很明显，略显麻烦的是字符串的分割，我们可以使用sring类的成员函数split，但是感觉这还是不够友好，幸好，C++STL提供了stringstream类，这种类方法可以实现各种输入输出流的转换及分割。嗯，我们先来了解下stringstream。
### stringstream
C++引入了ostringstream、istringstream、stringstream这三个类，要使用他们创建对象就必须包含sstream.h头文件。
> istringstream类用于执行C++风格的串流的输入操作。

> ostringstream类用于执行C风格的串流的输出操作。

> strstream类同时可以支持C风格的串流的输入输出操作。

> istringstream类是从istream和stringstreambase派生而来，ostringstream是从ostream和 stringstreambase派生而来， stringstream则是从iostream类和stringstreambase派生而来。

> 他们的继承关系如下图所示:
![stringstream](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 所以stringstream，能够根据给定类型的变量，在stringstream内部做好分割。我们结合该题的代码，做进一步了解。

> [
](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 代码

> [

](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> class
> Solution {
> public
> :
> string
> complexNumberMultiply(
> string
> a,
> string
> b) {
> // define the var
> int
> an,ai,bn,bi;
> stringstream
> aa(a);
> stringstream
> bb(b);
> char
> plus;
> stringstream
> res;
> // split
> aa>>an>>plus>>ai;
        bb>>bn>>plus>>bi;
        res<<an*bn-ai*bi<<
> "+"
> <<an*bi+bn*ai<<
> "i"
> ;
> return
> res.str(); 
    }
};
> [
](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 代码：

> [

](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> aa>>an>>plus>>ai;
bb>>bn>>plus>>bi;
> [
](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 这里stringstream对象将遇到的第一个整数输入给an，字符‘+’输入给plus，第二个整数输入给ai。

> [            ](https://img-blog.csdn.net/20171206104926627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

