# CUnit单元测试 - xiahouzuoxin - CSDN博客





2012年12月01日 17:07:22[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4101








**单元：**对于C语言来说，单元即函数。

**C语言单元测试：**对C语言中的功能函数进行正确性、效能等方面的测试。

**CUnit：**一个用于C语言单元测试的软件，下载地址为http://sourceforge.net/projects/cunit/，相关的例子可以在http://cunit.sourceforge.net/example.html找到。




**1、安装CUnit，环境为Linux**

./configure --prefix=/usr/local     # 指定安装路径为/usr/local

make

make install

**2、CUnit基本框架**



                       Test Registry

                            |

             ------------------------------

             |                            |

         TestSuite '1'      . . . .   TestSuite 'N'

             |                            |

       ---------------             ---------------

       |             |             |             |

    Test '11' ... Test '1M'     Test 'N1' ... Test 'NM'

        一次测试（Test Registry）可以运行多个测试包（Test Suite），而每个测试包可以包括多个测试用例（Test Case）。每个测试用例又包含一个或者多个断言类的语句。具体到程序的结构上，一次测试下辖多个Test Suite，它对应于程序中各个独立模块；一个Suite管理多个Test Case，它对应于模块内部函数实现。每个Suite可以含有setup和teardown函数，分别在执行suite的前后调用。
**3、CUnit测试接口模式**
CUnit共有4种测试接口模式，为用户提供交互和测试结果，如下
- 自动输出到XML文件，     非交互式 
- 基本方式，                     非交互式
- 控制台方式，                   交互式
- Curses图形接口，            交互

其中方式2比较常用，就是通常的控制台交互方式。

**4、CUnit用于C语言测试的流程**

参考CUnit安装软件中的doc文件，CUnit测试C函数单元分为6步：
- 书写代测试的函数（如果必要，需要写suite的init/cleanup函数）
- 初始化Test Registry - CU_initialize_registry()
- 把测试包（Test Suites）加入到Test Registry - CU_add_suite()
- 加入测试用例（Test Case）到测试包当中 - CU_add_test()
- 使用适当的接口来运行测试测试程序，例如 CU_console_run_tests()
- 清除Test Registry - CU_cleanup_registry()
**5、实例分析**
以下的测试源代码可以在http://cunit.sourceforge.net/example.html上找到**。**

我们这里将代码中的

CU_basic_run_tests();


修改为

CU_console_run_tests();


修改的目的：原来代码中使用“基本方式”作为测试接口模式，现在改成了控制台交互的方式。


基本方式直接执行程序后退出，交互方式有更多的可选择余地，可以看作是基本方式的扩展。




```cpp
/*
 *  Simple example of a CUnit unit test.
 *
 *  This program (crudely) demonstrates a very simple "black box"
 *  test of the standard library functions fprintf() and fread().
 *  It uses suite initialization and cleanup functions to open
 *  and close a common temporary file used by the test functions.
 *  The test functions then write to and read from the temporary
 *  file in the course of testing the library functions.
 *
 *  The 2 test functions are added to a single CUnit suite, and
 *  then run using the CUnit Basic interface.  The output of the
 *  program (on CUnit version 2.0-2) is:
 *
 *           CUnit : A Unit testing framework for C.
 *           http://cunit.sourceforge.net/
 *
 *       Suite: Suite_1
 *         Test: test of fprintf() ... passed
 *         Test: test of fread() ... passed
 *
 *       --Run Summary: Type      Total     Ran  Passed  Failed
 *                      suites        1       1     n/a       0
 *                      tests         2       2       2       0
 *                      asserts       5       5       5       0
 */

#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}

/* Simple test of fprintf().
 * Writes test data to the temporary file and checks
 * whether the expected number of bytes were written.
 */
void testFPRINTF(void)
{
   int i1 = 10;

   if (NULL != temp_file) {
      CU_ASSERT(0 == fprintf(temp_file, ""));
      CU_ASSERT(2 == fprintf(temp_file, "Q\n"));
      CU_ASSERT(7 == fprintf(temp_file, "i1 = %d", i1));
   }
}

/* Simple test of fread().
 * Reads the data previously written by testFPRINTF()
 * and checks whether the expected characters are present.
 * Must be run after testFPRINTF().
 */
void testFREAD(void)
{
   unsigned char buffer[20];

   if (NULL != temp_file) {
      rewind(temp_file);
      CU_ASSERT(9 == fread(buffer, sizeof(unsigned char), 20, temp_file));
      CU_ASSERT(0 == strncmp(buffer, "Q\ni1 = 10", 9));
   }
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of fprintf()", testFPRINTF)) ||
       (NULL == CU_add_test(pSuite, "test of fread()", testFREAD)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_console_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
```



当然，当涉及的测试单元比较多的时候，使用譬如



```cpp
if ((NULL == CU_add_test(pSuite, "test of fprintf()", testFPRINTF)) ||
       (NULL == CU_add_test(pSuite, "test of fread()", testFREAD)))
```
的测试方法是比较麻烦的。因此还有一种更简便的方法





```cpp
CU_TestInfo testcase[] = {
		{"TestFPRINTF: ", TestFPRINTF},
		{"TestFREAD: ", TestFREAD},
		CU_TEST_INFO_NULL
	};
	CU_SuiteInfo suite[] = {
		{"Testing func: ", InitSuite, EndSuite, testcase},
		CU_SUITE_INFO_NULL
	};

	/* Init registry */
	if(CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	/* register suite */
	if(CUE_SUCCESS != CU_register_suites(suite))
	{
		return 1;
	}

	CU_console_run_tests();

	CU_cleanup_registry();
```



为了能执行上面的程序，我们为程序编写makefile文件，



```
INC=-I/usr/local/include
LIB=-L/usr/local/lib

test:test.c
	gcc test.c -o test -lcunit -static
```



注意：makefile中要包含CUnit的头文件和库所在的路径，编译时使用-lcunit选项，并使用静态编译（-static）选项。

如下为运行结果，从光标处我们可以选择不同功能。


![](https://img-my.csdn.net/uploads/201212/01/1354353277_6798.png)


**6、总结及参考**

测试过程中，为了通用性，我们将测试代码分为一下几部分组成。

（1）测试单元（函数）源代码

（2）TestSuite.c添加测试包和测试用例

（3）main.c主程序框架，用于注册CUnit和取消注册CUnit

当然，我们也可以把测试单独放在一个文件中就行了。

更多使用细节参考帮助文档。

其它参考：http://blog.csdn.net/colin719/article/details/1420583

                http://cunit.sourceforge.net/index.html







