
# C++指针问题与学习 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月29日 23:25:49[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1073标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=string&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

1.指针进行初始化和赋值操作的约束
1.0值常量表达式 如编译时可获得0值的整型const对象或字面值常量0
2.类型匹配的对象的地址
3.另一对象之后的下一地址
4.同类型的另一有效指针
int val;
int zero = 0;
const int c_val = 0;
int *p1 = val;  //error int *p1 = &val
p1 = zero; //error
p1 = c_val; //ok
p1 = 0; //ok
int *p2 = NULL; //ok
double *p3 = &val; //error:类型不匹配
2.void*指针
可以指向任何类型对象的地址
但：void*指针只有几种操作：
1.与定义指针进行比较
2.向函数传递void*指针或从函数返回void*指针
3.给另一void*指针赋值
不允许使用void*指针操纵它所指向的对象
3.指针与引用的差别
1.引用总是指向某个对象，定义引用时没有初始化是错误的。
2.给引用赋值修改的是该引用所关联的对象的值，而并不是使引用与另一个对象关联。
3.引用一经初始化就始终指向同一个特定对象。
4.下标与指针
int a[10] = {0};
int *p1 = &a[10];
int j = p1[-2]; //j=a[2]
int k = p1[2]; //k=a[6]
a++; //error
5.指针与const
1.指向const对象的指针
const double *cptr; //指针cptr本身不是const的，而是它所指向的对象是const的
//在定义时可以对cptr不初始化，如果需要，就对其重新赋值，使其指向一个const对象，一旦赋值后就不能通过cptr指针修改其指向的对象的值，但是此指针可以指向别的对象
如
const int val = 56;
const int *cptr = &val;
*cptr = 10; //error
const对象的地址不能赋给非const对象的指针
如：
const double *pi = 3.14;
double *ptr = &pi;//error
const *cptr = &pi;//ok
不能使用void*指针指向const对象
如：
const int val = 10;
void *p1 = &val; //error
const void *p2 = &val; //ok
允许把非const对象的地址赋给指向const对象的指针
const double *cptr;
double val = 10.0；
cptr = &val; //ok
尽管val不是const的，但是企图通过cptr来修改val值都是错误的
2.const指针
指针本身的值不能改变，即该指针需要在定义时初始化，以后不能再指向别的对象，但是可以通过该指针修改所指对象的值
如：
int num1 = 10,num2 = 20;
int *const ptr = &num1;
*prt = 20;//ok ,modify num1 = 20
ptr = &num2; //error
3.指向const对象的const指针
int val = 10; //或者const int val = 10;
const int *const ptr = &val;
在定义该指针时必须初始化，之后该指针不能指向其他的对象，也不能通过该指针修改其指向的对象的值
int val_1 = 20;
ptr = &val_1;  //error
*ptr = 20; //error;
一个难搞的指针问题
typedef string* pString;
const pString cstr; 这句话等价于？
是 const string *cstr;
还是 string *const cstr;
还是const string *const cstr;
答案是string *const cstr;
相当于 const (string*) cstr; 等价于 string *const cstr;也等价于 pString const cstr;

