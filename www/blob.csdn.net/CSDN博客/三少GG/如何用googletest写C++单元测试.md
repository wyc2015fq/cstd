# 如何用googletest写C++单元测试 - 三少GG - CSDN博客
2013年08月22日 22:15:25[三少GG](https://me.csdn.net/scut1135)阅读数：1091
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
googletest是一个用来写C++单元测试的框架，它是跨平台的，可应用在windows、linux、Mac等OS平台上。下面，我来说明如何使用最新的1.6版本gtest写自己的单元测试。
本文包括以下几部分：1、获取并编译googletest（以下简称为gtest）；2、如何编写单元测试用例；3、如何执行单元测试。4、google test内部是如何执行我们的单元测试用例的。
**1. 获取并编译gtest**
gtest试图跨平台，理论上，它就应该提供多个版本的binary包。但事实上，gtest只提供源码和相应平台的编译方式，这是为什么呢？google的解释是，我们在编译出gtest时，有些独特的工程很可能希望在编译时加许多flag，把编译的过程下放给用户，可以让用户更灵活的处理。这个仁者见仁吧，反正也是免费的BSD权限。
源码的获取地址：http://code.google.com/p/googletest/
svn checkout *http*://googletest.googlecode.com/svn/trunk/
 googletest-read-only
怎么编译呢？
先进入gtest目录（解压gtest.zip包过程就不说了），执行以下两行命令：
**[cpp]**[view
 plain](http://blog.csdn.net/russell_tao/article/details/7333226#)[copy](http://blog.csdn.net/russell_tao/article/details/7333226#)
- g++ -I./include -I./ -c ./src/gtest-all.cc  
- ar -rv libgtest.a gtest-all.o  
之后，生成了libgtest.a，这个就是我们要的东东了。以后写自己的单元测试，就需要libgtest.a和gtest目录下的include目录，所以，这1文件1目录我们需要拷贝到自己的工程中。
编译完成后怎么验证是否成功了呢？
**[cpp]**[view
 plain](http://blog.csdn.net/russell_tao/article/details/7333226#)[copy](http://blog.csdn.net/russell_tao/article/details/7333226#)
- cd ${GTEST_DIR}/make  
-   make  
-   ./sample1_unittest  
如果看到：
**[cpp]**[view
 plain](http://blog.csdn.net/russell_tao/article/details/7333226#)[copy](http://blog.csdn.net/russell_tao/article/details/7333226#)
- Running main() from gtest_main.cc  
- [==========] Running 6 tests from 2 test cases.  
- [----------] Global test environment set-up.  
- [----------] 3 tests from FactorialTest  
- [ RUN      ] FactorialTest.Negative  
- [       OK ] FactorialTest.Negative (0 ms)  
- [ RUN      ] FactorialTest.Zero  
- [       OK ] FactorialTest.Zero (0 ms)  
- [ RUN      ] FactorialTest.Positive  
- [       OK ] FactorialTest.Positive (0 ms)  
- [----------] 3 tests from FactorialTest (0 ms total)  
- 
- [----------] 3 tests from IsPrimeTest  
- [ RUN      ] IsPrimeTest.Negative  
- [       OK ] IsPrimeTest.Negative (0 ms)  
- [ RUN      ] IsPrimeTest.Trivial  
- [       OK ] IsPrimeTest.Trivial (0 ms)  
- [ RUN      ] IsPrimeTest.Positive  
- [       OK ] IsPrimeTest.Positive (0 ms)  
- [----------] 3 tests from IsPrimeTest (0 ms total)  
- 
- [----------] Global test environment tear-down  
- [==========] 6 tests from 2 test cases ran. (0 ms total)  
- [  PASSED  ] 6 tests.  
那么证明编译成功了。
2.**可以编译自己的程序**
1> 把libgtest.a 放在你自己定义的lib中，如/home/John/myGtest**/lib**
2>把GTest需要的头文件放在你自己定义的头文件中，如/home/john/myGtest**/include**(头文件只需要从源码中拷贝就可以)
3>      $cd /home/john/myGtest
          $**g++** -I./include your_test.cc-o
 your_test –L./lib -lgtest -lpthread  **(验证ok)**
(**-I**参数可以用相对路径，比如头文件在当前目录，可以用-I.来指定。**-L**参数跟着的是库文件所在的目录名)
或者写Makefile文件。
**3. 示例1**
```cpp
#include <gtest/gtest.h>
// Returns n! (the factorial of n). For negative n, n! is defined to be 1.
int Factorial(int n) {
int result = 1;
for (int i = 1; i <= n; i++) {
   result *= i;
}
return result;
}
// Tests factorial
TEST(FactorialTest, Negative)
{
EXPECT_EQ(1, Factorial(-5));
EXPECT_EQ(1, Factorial(0));
EXPECT_EQ(6, Factorial(3));
}
int main(int argc, char* argv[])
{
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}
```
**4. 示例2**
首先我们建立自己的测试文件为sample.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/veranal/article/details/6617916#)[copy](http://blog.csdn.net/veranal/article/details/6617916#)
- int sample(inta,intb)  
- {  
- if(a==0||b==0)  
- {  
- throw"Don‘t do this";  
- }  
- int c= a%b;  
- if(c==0)  
- return b;  
- return sample(b,c);  
- }  
**其次写自己测试程序的头文件sample.h**
- int sample(int a,int b);  
写GTest测试程序为gtestSample.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/veranal/article/details/6617916#)[copy](http://blog.csdn.net/veranal/article/details/6617916#)
- #include<gtest/gtest.h>
- #include “sample.h”
- TEST(sampleTest,HandleNoneZeroInput)  
- {  
- EXPECT_EQ(2, sample(4,10));  
- EXPECT_EQ(6, sample(30,18));  
- }  
最后写我们相关的Makefile文件
**[cpp]**[view
 plain](http://blog.csdn.net/veranal/article/details/6617916#)[copy](http://blog.csdn.net/veranal/article/details/6617916#)
- GTEST_DIR = ..  
- USER_DIR = ../program  
- CPPFLAGS += -I$(GTEST_DIR)/include  
- CXXFLAGS += -g -Wall -Wextra  
- 
- TARGET = sample_unittest  
- OBJ = sample.o gtestSample.o  
- 
- # All Google Test headers.  Usually you shouldn't change this
- # definition.
- GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \  
-                 $(GTEST_DIR)/include/gtest/internal/*.h  
- CC = g++  
- $(TARGET) : $(OBJ)   
-     $(CC) $(CPPFLAGS) $(CXXFLAGS)  -L../lib -lgtest -lpthread $^ -o $@  
- sample.o : $(USER_DIR)/sample.cpp $(USER_DIR)/sample.h $(GTEST_HEADERS)  
-     $(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample.cpp  
- 
- gtestSample.o : $(USER_DIR)/gtestSample.cpp $(USER_DIR)/sample.h $(GTEST_HEADERS)  
-     $(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/gtestSample.cpp  
- .PHONY: clean  
- clean:  
-     rm -f $(TARGET) $(OBJ)  
执行make
./ sample_unittest
运行结果：
**[cpp]**[view
 plain](http://blog.csdn.net/veranal/article/details/6617916#)[copy](http://blog.csdn.net/veranal/article/details/6617916#)
- Running main() from gtest_main.cc  
- [==========] Running 1 test from 1 test case.  
- [----------] Global test environment set-up.  
- [----------] 1 test from sampleTest, where TypeParam =   
- [ RUN      ] sampleTest.HandleNoneZeroInput  
- [       OK ] sampleTest.HandleNoneZeroInput (0 ms)  
- [----------] 1 test from sampleTest (0 ms total)  
- 
- [----------] Global test environment tear-down  
- [==========] 1 test from 1 test case ran. (0 ms total)  
- [  PASSED  ] 1 test.  
Congratulations！
**************************************************参考文献******************************************************
1. [用google
 mock模拟C++对象](http://blog.csdn.net/russell_tao/article/details/7344739)
google mock是用来配合google test对C++项目做单元测试的。它依赖于googletest（参见我上篇文章《如何用googletest写单元测试》： [http://blog.csdn.net/russell_tao/article/details/7333226](http://blog.csdn.net/russell_tao/article/details/7333226)），下面我来说说linux上怎么用它来做单元测试。
本文包括：1、如何获取、编译google mock；2、如何使用gmock（下面用gmock来代称google mock）配合gtest做单元测试。
# 2. [C/C++单元测试工具](http://guiquanz.github.io/2013/03/23/c-c%2B%2B-unit-test)（汇总）
# 3. [跨平台C++单元测试框架GTest -- Linux下试用](http://graybull.is-programmer.com/posts/37871.html)
