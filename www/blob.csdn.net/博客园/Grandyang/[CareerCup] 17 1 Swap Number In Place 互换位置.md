# [CareerCup] 17.1 Swap Number In Place 互换位置 - Grandyang - 博客园







# [[CareerCup] 17.1 Swap Number In Place 互换位置](https://www.cnblogs.com/grandyang/p/5402959.html)







17.1 Write a function to swap a number in place (that is, without temporary variables).



这道题让我们交换两个数，但是不能用额外空间，那么我们可以先做差值，存入a中，然后再加上b，存入b中，那么此时的b即为原来的a，因为整个相当于做了一个a - b + b的过程，那么现在b是原来的a，而a中现在保存的是差值，，那么原来的b值就可以通过b-a来得到，保存到a中即可：



解法一：

```
void swap(int a, int b) {
    a = a - b;
    b = b + a;
    a = b - a;
    cout << a << " " << b << endl;
}
```



下面这种做法跟上面的方法思路相同，但是用到了异或操作符，a异或b求的实际上就是a与b的差值，然后结果再异或b得到a，这就相当于a^b^b，即a^(b^b)，由于b^b为0，任何数异或0都为其本身，所以b就成了a，然后a再异或差值就得到了b，完成了交换：



解法二：

```
void swap(int a, int b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    cout << a << " " << b << endl;
}
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












