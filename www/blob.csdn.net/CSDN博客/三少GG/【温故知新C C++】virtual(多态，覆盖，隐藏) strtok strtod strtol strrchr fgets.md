# 【温故知新C/C++】virtual(多态，覆盖，隐藏)||strtok||strtod||strtol||strrchr||fgets - 三少GG - CSDN博客
2011年04月27日 15:51:00[三少GG](https://me.csdn.net/scut1135)阅读数：1083标签：[null																[class																[string																[character																[buffer																[stream](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
# VC++深入详解:函数的覆盖和隐藏
1．函数的覆盖
　　在上一节介绍多态性的时候，我们给出了下面的代码片段：
　　例2-19
　　class animal
　　{
　　public:
　　…
　　virtual void breathe()
　　{
　　cout<<"animal breathe"<　　}
　　};
　　class fish:public animal
　　{
　　public:
　　void breathe()
　　{
　　cout<<"fish bubble"<　　}
　　};
　　在基类animal的breathe函数前添加了virtual关键字，声明该函数为虚函数。在派生类fish中重写了breathe函数，我们注意到，fish类的breathe函数和animal类的breathe函数完全一样，无论函数名，还是参数列表都是一样的，这称为**函数的覆盖（override）。**构成函数覆盖的条件为：
　　n 基类函数必须是虚函数（使用virtual关键字进行声明）。
　　n 发生覆盖的两个函数要分别位于派生类和基类中。
　　n 函数名称与参数列表必须完全相同。
**　由于C++的多态性是通过虚函数来实现的，所以函数的覆盖总是和多态关联在一起。在函数覆盖的情况下，编译器会在运行时根据对象的实际类型来确定要调用的函数。**　　2．函数的隐藏
　　我们再看例2-20的代码：
　　例2-20
　　class animal
　　{
　　public:
　　…
　　void breathe()
　　{
　　cout<<"animal breathe"<　　}
　　};
　　class fish:public animal
　　{
　　public:
　　void breathe()
　　{
　　cout<<"fish bubble"<　　}
　　};
　　你看出来这段代码和例2-19所示代码的区别了吗？在这段代码中，派生类fish中的breathe函数和基类animal中的breathe 函数也是完全一样的，不同的是breathe函数不是虚函数，这种情况称为函数的隐藏。所谓隐藏，是指派生类中具有与基类同名的函数（不考虑参数列表是否相同），从而在派生类中隐藏了基类的同名函数。
　　初学者很容易把函数的隐藏与函数的覆盖、重载相混淆，我们看下面两种函数隐藏的情况：
　　（1）派生类的函数与基类的函数完全相同（函数名和参数列表都相同），只是基类的函数没有使用virtual关键字。此时基类的函数将被隐藏，而不是覆盖（请参照上文讲述的函数覆盖进行比较）。
　　（2）派生类的函数与基类的函数同名，但参数列表不同，在这种情况下，不管基类的函数声明是否有virtual关键字，基类的函数都将被隐藏。注意这种情况与函数重载的区别，重载发生在同一个类中。
　　下面我们给出一个例子，以帮助读者更好地理解函数的覆盖和隐藏，代码如例2-21所示。
　　例2-21
　　class Base
　　{
　　public:
　　virtual void fn();
　　};
　　class Derived : public Base
　　{
　　public:
　　void fn(int);
　　};
　　class Derived2 : public Derived
　　{
　　public:
　　void fn();
　　};
　　在这个例子中，Derived类的fn(int)函数隐藏了Base类的fn()函数，Derived类fn(int)函数不是虚函数（注意和覆盖相区别）。Derived2类的fn()函数隐藏了Derived类的fn(int)函数，由于Derived2类的fn()函数与Base类的fn ()函数具有同样的函数名和参数列表，因此Derived2类的fn()函数是一个虚函数，覆盖了Base类的fn()函数。注意，在Derived2类中，Base类的fn()函数是不可见的，但这并影响fn函数的覆盖。
**当隐藏发生时，如果在派生类的同名函数中想要调用基类的被隐藏函数，可以使用“基类名::函数名（参数）”的语法形式**。例如，要在Derived类的fn(int)方法中调用Base类的fn()方法，可以使用Base::fn()语句。
**有的读者可能会想，我怎样才能更好地区分覆盖和隐藏呢？实际上只要记住一点：函数的覆盖是发生在派生类与基类之间，两个函数必须完全相同，并且都是虚函数。那么不属于这种情况的，就是隐藏了。**　　最后，我们再给出一个例子，留给读者思考，代码如例2-22所示（EX09.CPP）。
　　例2-22
　　#include 
　　class Base
　　{
　　public:
　　virtual void xfn(int i)
　　{
　　cout<<"Base::xfn(int i)"<　　}
　　void yfn(float f)
　　{
　　cout<<"Base::yfn(float f)"<　　}
　　void zfn()
　　{
　　cout<<"Base::zfn()"<　　}
　　};
　　class Derived : public Base
　　{
　　public:
　　void xfn(int i) //覆盖了基类的xfn函数
　　{
　　cout<<"Drived::xfn(int i)"<　　}
　　void yfn(int c) //隐藏了基类的yfn函数
　　{
　　cout<<"Drived::yfn(int c)"<　　}
　　void zfn()      //隐藏了基类的zfn函数
　　{
　　cout<<"Drived::zfn()"<　　}
　　};
　　void main()
　　{
　　Derived d;
　　Base *pB=&d;
　　Derived *pD=&d;
　　pB->xfn(5);
　　pD->xfn(5);
　　pB->yfn(3.14f);
　　pD->yfn(3.14f);
　　pB->zfn();
　　pD->zfn();
　　}
函数名前面加virtual表示虚函数，如：virtual void a（）；**如果写成 virtual void a()=0;就表示纯虚函数**，所以定义纯虚函数是要等于0的，一个类中如果有纯虚函数，那么它不可以构建对象。 **纯虚函数是一种特殊的虚函数，它的一般格式如下：  class <类名>  {  virtual <类型><函数名>(<参数表>)=0;  …  };**
  在许多情况下，**在基类中不能对虚函数给出有意义有实现，而把它说明为纯虚函数，它的实现留给该基类的派生类去做**。这就是纯虚函数的作用。
[编辑本段]二、引入原因
  1、为了方便使用多态特性，我们常常需要在基类中定义虚拟函数。
  2、**在很多情况下，基类本身生成对象是不合情理的。例如，动物作为一个基类可以派生出老虎、孔雀等子类，但动物本身生成对象明显不合常理。**
  为了解决上述问题，引入了纯虚函数的概念，**将函数定义为纯虚函数（方法：virtual ReturnType Function()= 0;），**则编译器要求在派生类中必须予以重载以实现多态性。同时含有纯虚拟函数的类称为抽象类，它不能生成对象。这样就很好地解决了上述两个问题。
[编辑本段]三、相似概念
  1、多态性
  指相同对象收到不同消息或不同对象收到相同消息时产生不同的实现动作。C++支持两种多态性：编译时多态性，运行时多态性。
  a.编译时多态性：通过重载函数实现
  b 运行时多态性：通过虚函数实现。
  2、虚函数
  虚函数是在基类中被声明为virtual，并在派生类中重新定义的成员函数，可实现成员函数的动态重载
  3、抽象类
  包含纯虚函数的类称为抽象类。由于抽象类包含了没有定义的纯虚函数，所以不能定义抽象类的对象。
  程序举例：
  基类:
  class A 
  {
  public:
  A();
  virtual ~A();
  void f1();
  virtual void f2();
  virtual void f3()=0;
  };
  子类:
  class B : public A 
  {
  public:
  B();
  virtual ~B();
  void f1();
  virtual void f2();
  virtual void f3();
  };
  主函数:
  int main(int argc, char* argv[])
  {
  A *m_j=new B();
  m_j->f1();
  m_j->f2();
  m_j->f3();
  delete m_j;
  return 0;
  }
  f1()是一个隐藏,关于函数的隐藏,可以参考其它词条.？？？？？？？？？？？？？？？？？？？？？？？？？  调用m_j->f1();会去调用A类中的f1(),它是在我们写好代码的时候就会定好的.
  也就是根据它是由A类定义的,这样就调用这个类的函数.
  f2()是普通的重载.
  调用m_j->f2();会调用m_j中到底保存的对象中,对应的这个函数.这是由于new的B
  对象.
  f3()与f2()一样,只是在基类中不需要写函数实现. ++++++++++++++++++++++++++++++++覆盖就是看不见，隐藏就是通过类名::函数名可以访问到。如果基类被重写的函数是虚函数的话就是覆盖，否则就是隐藏。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////　char ***strtok**(char *s, const char *delim); 
## 功能
　　分解字符串为一组字符串。s为要分解的字符串，delim为分隔符字符串。 
## 说明
　　首次调用时，s指向要分解的字符串，之后再次调用要把s设成NULL。 　　s**trtok在s中查找包含在delim中的字符并用NULL('')来替换，直到找遍整个字符串。 **
**　　char * p = strtok(s,";"); ****　　p = strtok(null,";"); ****　　在调用的过程中，字串s被改变了，这点是要注意的。**
## 返回值
　　从s开头开始的一个个被分割的串。当没有被分割的串时则返回NULL。 　　所有delim中包含的字符都会被滤掉，并将被滤掉的地方设为一处分割的节点。
strtok函数会破坏被分解字符串的完整，调用前和调用后的s已经不一样了。如果 　　要保持原字符串的完整，可以使用strchr和sscanf的组合等。众所周知，strtok可以根据用户所提供的分割符（同时分隔符也可以为复数比如“，。”）
　　将一段字符串分割直到遇到"/0".
　　比如，分隔符=“，” 字符串=“Fred，John，Ann”
　　通过strtok 就可以把3个字符串 “Fred”     “John”      “Ann”提取出来。
　　上面的C代码为
　　QUOTE:
　　int in=0;
　　char buffer[]="Fred,John,Ann"
　　char *p[3];
　　char *buff = buffer;
　　while((p[in]=strtok(buf,","))!=NULL) {
　　i++;
　　buf=NULL; }
　　如上代码，第一次执行strtok需要以目标字符串的地址为第一参数（buf=buffer），之后strtok需要以NULL为第一参数 (buf=NULL)。指针列p[],则储存了分割后的结果，
p[0]="John",p[1]="John",p[2]="Ann"，而buf就变成     Fred/0John/0Ann/0。
# strtod
**strtod**（将字符串转换成浮点数） 
　　相关函数 
　　atoi，atol，strtod，strtol，strtoul 
　　表头文件 
　　#include<[stdlib.h](view/1347718.htm)> 
　　定义函数 
　　double strtod(const char *nptr,char **endptr); 
函数说明 
　　strtod()会扫描参数nptr字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，到出现非数字或字符串结束时('/0')才结束转换，并将结果返回。
若endptr不为NULL，则会将遇到不合条件而终止的nptr中的字符指针由endptr传回。参**数nptr字符串可包含正负号、小数点或E(e)来表示指数部分**。如123.456或123e-2。 
　　返回值 
　　返回转换后的浮点型数。 
　　附加说明 
　　参考atof()。 
　　范例 
　　#include<stdlib.h> 
　　main() 
　　{ 
char *endptr;
　　char a[]="12345.6789"; 
　　char b[]="1234.567qwer"; 
　　char c[]="-232.23e4"; 
　　printf("a=%lf/n",strtod(a,NULL));
　　printf("b=%lf/n",strtod(b,&endptr)); 
　　printf("endptr=%s/n",endptr); 
　　printf("c=%lf/n",strtod(c,NULL)); 
　　} 
　　执行 
　　a=12345.678900 
　　b=1234.567000 
　　endptr=qwer 
　　c=-2322300.000000 
# strtol
long int strtol(const char *nptr,char **endptr,int base);
　这个函数会将参数nptr字符串根据参数base来转换成长整型数。参数base范围从2至36，或0。
参数base代表采的进制方式，如base值为10则采用10进制，若base值为16则采用16进制等。
当base值为0时则是采用10进制做转换，但遇到如’0x’前置字符则会使用16进制做转换、遇到’0’前置字符而不是’0x’的时候会使用8进制做转换。
一开始strtol()会扫描参数nptr字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，
再遇到非数字或字符串结束时('/0')结束转换，并将结果返回。
若参数endptr不为NULL，则会将遇到不合条件而终止的nptr中的字符指针由endptr返回。
s**trtol是atoi的增强版**
# strrchr
char *strrchr(char *str, char c);
　strrchr()函数的作用是：查找一个字符串在另一个字符串中末次出现的位置，并返回从字符串中的这个位置起， 一直到字符串结束的所有字符
。(或者说 strrchr() 函数查找字符在指定字符串中从后面开始的第一次出现的位置，如果成功，则返回指向该位置的指针，如果失败，则返回 false。) 
因为ptr是指针指向string数组某个元素的地址，string是该数组的首元素（string[0]）地址，所以两个内存地址相减正好是该字符所在数组中的位置。
（注：数组在内存中的地址是连续的。如：string[15]在内存中的地址可能为...001,...002,...003....总之是连续的）
　　如果未能找到指定字符，那么函数将返回NULL。
函数名: strrchr 
功 能: 在串中查找指定字符的最后一个出现 
用 法: char *strrchr(char *str, char c); 
程序例:
#include <string.h> 
#include <stdio.h>
int main(void) 
{ 
   char string[15]; 
   char *ptr, c = 'r';
   strcpy(string, "This is a string"); 
   ptr = strrchr(string, c); //ptr=?空格算不算？
   if (ptr) 
      printf("The character %c is at position: %d/n", c, ptr-string); //这里为什么要ptr-string而且string是字符串怎么可以拿来减？还是我理解错了?请大侠指导下
   else 
      printf("The character was not found/n"); 
   return 0; 
}
# fgets
fgets函数　　从流中读一行或指定个字符， 
　　原型是char *fgets(char *s, int n, FILE *stream); 
　　从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。 
　　形参注释：*s结果数据的首地址；n-1:一次读入数据块的长度,其默认值为1k，即1024;stream文件指针 
　　例：如果一个文件的当前位置的文本如下 
　　Love ,I Have 
　　But ........ 
　　如果用　 
　　fgets(str1,4,file1); 
　　则执行后str1="Lov"，读取了4-1=3个字符， 
　　而如果用　 
　　fgets(str1,23,file1); 
　　则执行str="Love ,I Have"，读取了一行(包括行尾的'/n',并自动加上字符串结束符'/0')。
fgets函数用来从文件中读入字符串。fgets函数的调用形式如下：fgets（str，n，fp）；
此处，fp是文件指针；str是存放在字符串的起始地址；n是一个int类型变量。
**函数的功能是从fp所指文件中读入n-1个字符放入str为起始地址的空间内；**
**如果在未读满n-1个字符之时，已读到一个换行符或一个EOF（文件结束标志），则结束本次读操作，**
**读入的字符串中最后包含读到的换行符。**因此，确切地说，调用fgets函数时，最多只能读入n-1个字符。
读入结束后，系统将自动在最后加'/0'，并以str作为函数值返回。
