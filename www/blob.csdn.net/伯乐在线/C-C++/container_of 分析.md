# container_of 分析 - 文章 - 伯乐在线
原文出处： [冀博](http://blog.csdn.net/tigerjibo/article/details/8299589)
#### 1.container_of宏
1 > Container_of在Linux内核中是一个常用的宏，用于从包含在某个结构中的指针获得结构本身的指针，通俗地讲就是通过结构体变量中某个成员的首地址进而获得整个结构体变量的首地址。
2 > 接口：
container_of(ptr, type, member)
 ptr:表示结构体中member的地址
 type:表示结构体类型
 member:表示结构体中的成员
通过ptr的地址可以返回结构体的首地址
3 > container_of的实现：


```
#define container_of(ptr, type, member) ({         
 const typeof( ((type *)0)->member ) *__mptr = (ptr);      
  (type *)( (char *)__mptr - offsetof(type,member) );})
```
其实它的语法很简单，只是一些指针的灵活应用，它分两步：
第一步，首先定义一个临时的数据类型（通过typeof( ((type *)0)->member )获得）与ptr相同的指针变量__mptr，然后用它来保存ptr的值。
说明：typeof是GNU C对标准C的扩展，它的作用是根据变量获取变量的类型《typeof关键字在linux 内核中很常见》
第二步，用(char *)__mptr减去member在结构体中的偏移量，得到的值就是整个结构体变量的首地址（整个宏的返回值就是这个首地址）。
关于offsetof的用法可参见offsetof宏的使用。
#### 2. 举例来说明 container_of 的使用
1 > 正确示例:


```
#include <stdio.h>
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define  container_of(ptr, type, member) ({                      
                      const typeof( ((type *)0)->member ) *__mptr = (ptr);    
                       (type *)( (char *)__mptr - offsetof(type,member) );})
struct test_struct {
           int num;
          char ch;
          float f1;
  };
 int main(void)
  {
          struct test_struct *test_struct;
          struct test_struct init_struct ={12,'a',12.3};
          char *ptr_ch = &init_struct.ch;
          test_struct = container_of(ptr_ch,struct test_struct,ch);
          printf("test_struct->num =%d\n",test_struct->num);
          printf("test_struct->ch =%c\n",test_struct->ch);
          printf("test_struct->ch =%f\n",test_struct->f1);
          return 0;
  }
```
执行结果：


```
jibo@jibo-VirtualBox:~/cv_work/work/list/container_of $ ./main
test_struct->num =12
test_struct->ch =a
test_struct->ch =12.300000
```
2 > 错误示例：


```
#include <stdio.h> 
  #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
  #define  container_of(ptr, type, member) ({                     
                           const typeof( ((type *)0)->member ) *__mptr = (ptr);    
                          (type *)( (char *)__mptr - offsetof(type,member) );}) 
   struct test_struct {
           int num;
          char ch;
          float f1;
  }; 
  int main(void)
  {
          struct test_struct *test_struct;
          char real_ch = 'A';
          char *ptr_ch = &real_ch;
          test_struct = container_of(ptr_ch,struct test_struct,ch);
          printf("test_struct->num =%d\n",test_struct->num);
          printf("test_struct->ch =%c\n",test_struct->ch);
          printf("test_struct->ch =%f\n",test_struct->f1);
          return 0;
  }
```
执行结果为:


```
jibo@jibo-VirtualBox:~/cv_work/work/list/container_of1 $ ./main
test_struct->num =0
test_struct->ch =A
test_struct->ch =0.000000
```
注意，由于这里并没有使用一个具体的结构体变量，所以成员num和f1的值是不确定的。
