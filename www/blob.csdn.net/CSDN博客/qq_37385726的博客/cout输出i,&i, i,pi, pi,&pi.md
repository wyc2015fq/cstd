# cout输出i,&i,*i,pi,*pi,&pi - qq_37385726的博客 - CSDN博客





2017年04月14日 23:39:21[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：107
个人分类：[C++](https://blog.csdn.net/qq_37385726/article/category/6862768)












从以下代码片可以学到：

1.*（取值运算符）和&（取地址运算符）的区别

2.int i=10;——i,&i,*i的区别

3.int *pi=&i，——pi，&pi,*pi的区别

4.char ch[10]="hello";——ch,*ch,&ch的区别

5.char *pch=ch;——pch,*pch,&pch的区别


```
#include
using namespace std;
#include
int main(void)
{
	int i = 10;
	char ch[10] = "hello";
	int *pi = &i;
	char *pch = ch;
	char *pchar = "hello";
	cout << i << "，" << &i << endl;//输出结果为10，i的地址
	                                //直接cout一个普通变量的名字，输出的是这个变量的值
	//cout << *i << endl;  //这个式子是不对的，因为*在这里作为取值运算符，它作用的对象只能是指针
	                       //（再次强调，不要把指针认为是一个很特殊的东西，它一点都不特殊，就是地址而已）
	cout << ch << "," << *ch << "," << &ch << endl;//输出结果为hello,h,数组ch的地址
	        //ch,本身就是“hello”字符串的首地址，cout这个ch，计算机会找到首地址并且读到‘\0’就结束，所以输出hello
	        //*ch,前面说过*是取值运算符，取的是该地址对应的值，在这里ch是这个字符串的首地址，所以*ch是这个字符串的首字母
	 //&ch,大小等于ch的首地址，但是与ch的首地址不同的是，&ch是数组的地址，而ch是数组的首地址，所以两者的输出会不同
	cout <
```




