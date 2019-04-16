# OCLint：静态代码分析工具（C、C++和Objective-C） - 资源 - 伯乐在线

# OCLint：静态代码分析工具（C、C++和Objective-C）

**[C语言](http://hao.jobbole.com/tag/c/) , [OCLint](http://hao.jobbole.com/tag/oclint/) , [静态代码分析](http://hao.jobbole.com/tag/%e9%9d%99%e6%80%81%e4%bb%a3%e7%a0%81%e5%88%86%e6%9e%90/)


本资源由 [伯乐在线](http://www.jobbole.com) - [Juliesand](http://www.jobbole.com/members/Juliesh) 整理

[![oclint](http://jbcdn1.b0.upaiyun.com/2016/10/af274f8581de06fce130510cb6ad06eb.png)](http://jbcdn1.b0.upaiyun.com/2016/10/af274f8581de06fce130510cb6ad06eb.png)

OCLint是一个静态代码分析工具，通过检查C、C++和Objective-C代码提高程序质量，减少瑕疵，同时搜索潜在的问题。



## 主要功能
- 可能的错误 – 空的ifelse/try/catch/finally语句
- 未使用的代码 – 未使用的本地变量和参数
- 复杂的代码 – 高秩复杂度、NPath（N路径）复杂度和大量非注释代码块（NCSS – Non Commenting Source Statements）
- 冗余的代码 – 冗余的if语句和无效的括号
- 代码异味 – 过长的函数和参数列表
- 不良设计 – 逻辑颠倒和参数重复赋值
- ……

静态代码分析是一个重要的技术，能够检测那些编译器注意不到的缺陷。OCLint自动完成如下高级检查功能：
- 依靠源代码的抽象语法树达到更高的精确性和有效性；极大的减少误检，避免有效的结果沉没在误检消息中。
- 在运行时也能动态向系统中加载检测规则。
- 灵活性和可扩展配置确保用户可以自定义检测工具的功能。
- 命令行调用有助于在开发时对代码进行连续的整合和检查，可以及早的修复技术缺陷，降低维护成本。

最新版本 

OCLint是一个在Linux和Mac OS X平台运行的独立工具。用户可以在随后的网站下载最新版本：[https://github.com/oclint/oclint/releases](https://github.com/oclint/oclint/releases)
- 版本：0.11
- 发布日期：2016年9月18日
- License：Modified BSD License

安装

OCLint最终为预编译二进制版本和本地编译包。文件树和下面相似：



























XHTML





oclint-release
|-bin
|-lib
|---clang
|-----<llvm/clang version>
|-------include
|-------lib
|---oclint
|-----rules
|-----reporters
|-include
|---c++
|-----v1

|12345678910111213|oclint-release|-bin|-lib|---clang|-----<llvm/clang version>|-------include|-------lib|---oclint|-----rules|-----reporters|-include|---c++|-----v1|
|----|----|



可以直接从bin文件夹中调用OCLint，不需要安装。

为了简化调用过程，建议将OCLint的bin文件夹添加到系统路径PATH中，此环境变量将告诉系统去哪个目录搜索执行文件。

安装教程：[http://docs.oclint.org/en/stable/intro/installation.html](http://docs.oclint.org/en/stable/intro/installation.html)

示例

创建一个sample.cpp文件，如下所示。



























C++





int main() {
    int i = 0, j = 1;
    if (j) {
        if (i) {
            return 1;
            j = 0;
        }
    }
    return 0;
}

|12345678910|intmain(){inti=0,j=1;if(j){if(i){return1;j=0;}}return0;}|
|----|----|



下一步是编译代码。其实不需要在使用OCLint检测该代码之前编译它。然而，由于寻找正确的参数成为最经常被问的问题，所以下列语句用于设置OCLint所需的编译标志。



























C++





$ CC -c sample.cpp // 步骤1：编译生成 sample.o
$ CC -o sample sample.o // 步骤2：连接生成sample执行文件
// 将C编译器修改为你最喜爱的兼容编译器，例如g++和clang++
$ ./sample // 执行二进制文件
$ echo $? // 如果输出是0则代码编译成功

|12345|$CC-csample.cpp// 步骤1：编译生成 sample.o$CC-osample sample.o// 步骤2：连接生成sample执行文件// 将C编译器修改为你最喜爱的兼容编译器，例如g++和clang++$./sample// 执行二进制文件$echo$?// 如果输出是0则代码编译成功|
|----|----|



现在，编译标志是-c，被检查的文件是sample.cpp

OCLint检查单个文件使用如下形式：



























C++





oclint [options] <source> -- [compiler flags]

|1|oclint[options]<source>--[compiler flags]|
|----|----|



所以分析sample源文件使用的命令是：



























C++





$ oclint sample.cpp -- -c

|1|$oclint sample.cpp---c|
|----|----|



可以将上述语句中options改成OCLint的其他功能，也可以使用不同的编译标志（compiler flags）。复杂一点的例子如下所示：



























C++





$ oclint -report-type html -o report.html sample.cpp -- -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -I/usr/include -I/usr/local/include -c

|1|$oclint-report-type html-oreport.html sample.cpp---D__STDC_CONSTANT_MACROS-D__STDC_LIMIT_MACROS-I/usr/include-I/usr/local/include-c|
|----|----|



资源链接
- 教程：[http://docs.oclint.org/en/stable/intro/tutorial.html](http://docs.oclint.org/en/stable/intro/tutorial.html)
- 手册：[http://docs.oclint.org/en/stable/manual/oclint.html](http://docs.oclint.org/en/stable/manual/oclint.html)
- 文档：[http://docs.oclint.org/en/stable/](http://docs.oclint.org/en/stable/)

官方网站：[http://oclint.org/](http://oclint.org/)
开源地址：[https://github.com/oclint](https://github.com/oclint)

