# static用法1 - leegang12的专栏 - CSDN博客
2011年05月21日 18:32:00[leegang12](https://me.csdn.net/leegang12)阅读数：359
|文件a.cstaticint i; //只在a文件中用int j;   //在工程里用staticvoid init()         //只在a文件中用{}void callme()          //在工程中用{staticint sum;}上面的全局i变量和init()函数只能用在a.c文件中,全局变量sum的作用域只在callme里。变量j和函数callme()的全局限扩充到整个工程文件。所以可以在下面的b.c中用extern关键字调用。extern告诉编译器这个变量或者函数在其他文件里已经被定义了。文件b.cexternint j;    //调用a文件里的externvoid callme();  //调用a文件里的int main(){...}extern的另外用法是当C和C++混合编程时如果c++调用的是c源文件定义的函数或者变量，那么要加extern来告诉编译器用c方式命名函数：文件A.cpp调用a.c里面的变量i和函数callme()extern"C"  //在c++文件里调用c文件中的变量{int j;   void callme();}int main(){   callme();}|文件a.cstaticint i; //只在a文件中用int j;   //在工程里用staticvoid init()         //只在a文件中用{}void callme()          //在工程中用{staticint sum;}|文件b.cexternint j;    //调用a文件里的externvoid callme();  //调用a文件里的int main(){...}|文件A.cpp调用a.c里面的变量i和函数callme()extern"C"  //在c++文件里调用c文件中的变量{int j;   void callme();}int main(){   callme();}|
|----|----|----|----|
|文件a.cstaticint i; //只在a文件中用int j;   //在工程里用staticvoid init()         //只在a文件中用{}void callme()          //在工程中用{staticint sum;}| | | |
|文件b.cexternint j;    //调用a文件里的externvoid callme();  //调用a文件里的int main(){...}| | | |
|文件A.cpp调用a.c里面的变量i和函数callme()extern"C"  //在c++文件里调用c文件中的变量{int j;   void callme();}int main(){   callme();}| | | |
