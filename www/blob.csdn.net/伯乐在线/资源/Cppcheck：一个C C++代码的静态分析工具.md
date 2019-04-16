# Cppcheck：一个C/C++代码的静态分析工具 - 资源 - 伯乐在线

# Cppcheck：一个C/C++代码的静态分析工具

**[C/C++](http://hao.jobbole.com/tag/cc/) , [cppcheck](http://hao.jobbole.com/tag/cppcheck/) , [静态代码分析](http://hao.jobbole.com/tag/%e9%9d%99%e6%80%81%e4%bb%a3%e7%a0%81%e5%88%86%e6%9e%90/)


本资源由 [伯乐在线](http://www.jobbole.com) - [Juliesand](http://www.jobbole.com/members/Juliesh) 整理

[![cppcheck](http://jbcdn1.b0.upaiyun.com/2016/10/85f7fab4aa580a65e7f2cd9575a0145d.png)](http://jbcdn1.b0.upaiyun.com/2016/10/85f7fab4aa580a65e7f2cd9575a0145d.png)

Cppcheck是一个C/C++代码的静态分析工具。与C++编译器和其他许多分析工具不同，Cppcheck不检测代码中的语法错误，只检测那些编译器通常无法检测到的bug类型，目的是只检测代码中真正的错误。

## 特点
- 检查边界溢出
- 检查内存泄漏
- 检查可能的空指针间接引用
- 检查未初始化的变量
- 检查无效的STL使用
- 检查异常安全
- 警告如果过时的或者不安全的函数使用
- 警告未使用的或者冗余的代码
- 检测各种潜在bugs的可疑代码等

所有检查项可以查看这理：[https://sourceforge.net/p/cppcheck/wiki/ListOfChecks/](https://sourceforge.net/p/cppcheck/wiki/ListOfChecks/)

CppCheck可以扩展，你可以：
- 查找简单模式，查看手册中章节rules；
- 使用.cfg文件为函数定义规则，查看手册中章节library；
- 使用脚本扩展cppcheck，查看手册中章节addons。

## 客户端和插件

Cppcheck已经和许多流行的开发工具集成。例如：Code::Blocks、CodeDX等。有些工具需要下载插件使用，例如CLion、gedit等。

## 其他静态分析工具

通常使用一组工具比单个工具更好，因此我们建议你同时使用其他的工具检测代码。

Cppcheck专注检测bugs而不是风格问题，因此添加一个专注检测风格问题的工具会更好。

Cppcheck希望尽量避免误报。有时人们想要检测出所有的bugs，即使这样可能检测出很多错误的warnings。例如当开发一个发行版本时想要确认其中没有任何的bugs。这时添加一个比Cppcheck更嘈杂的工具也许是一个不错的选择。

甚至一些和Cppcheck有相同设计目标的工具也可能是较好的补充。静态分析是一个很大的领域，Cppcheck只是覆盖了很小的一部分。没有工具可以涵盖所有领域。由于一些工具的出现而导致手动测试淘汰的那一天还很遥远。

## 最新版本
- 版本：1.76.1
- 发布日期：2016年10月13日
- 作者：Daniel Marjamaki

## 安装

在官方网站下载对应系统的最新版本，下载后解压安装。

应该使用一个与你的目标匹配的平台配置。如果代码在本地编译和执行，Cppcheck 默认使用运行良好的本地平台配置。

Cppcheck 内置了一些适用于Unix和 Windows平台的参数，利用–platform 命令行标志可以轻松的使用它们。

还可以创建 XML 文件，设置自定义平台配置。

## 示例

### Visual Studio

在单独项目文件（*.vcxproj）或者整个解决方案（*.sln）中都可以运行Cppcheck。



























C++





# run cppcheck on a whole solution
$ cppcheck --project=foobar.sln

# run cppcheck on a individual project
$ cppcheck --project=foobar.vcxproj

|12345|# run cppcheck on a whole solution$cppcheck--project=foobar.sln# run cppcheck on a individual project$cppcheck--project=foobar.vcxproj|
|----|----|



提示：需要下载相应的插件才能在Visual Studio内部使用Cppcheck。

### 代码检测



































int main()
{
    char a[10];
    a[10] = 0;
    return 0;
}

|123456|intmain(){chara[10];a[10]=0;return0;}|
|----|----|



将代码保存为文件file1.c，执行命令



























Shell





cppcheck file1.c

|1|cppcheck file1.c|
|----|----|



输出为：



























Shell





Checking file1.c...

[file1.c:4]: (error) Array 'a[10]' index 10 out of bounds

|123|Checking file1.c...[file1.c:4]:(error)Array'a[10]'index10out of bounds|
|----|----|





## 开发资源
- **手册：**[http://cppcheck.sourceforge.net/manual.html](http://cppcheck.sourceforge.net/manual.html) 可以在官网下载PDF版。
- **评价：**[https://sourceforge.net/projects/cppcheck/reviews?source=navbar](https://sourceforge.net/projects/cppcheck/reviews?source=navbar)
- ****新闻: ****[https://sourceforge.net/p/cppcheck/news/?source=navbar](https://sourceforge.net/p/cppcheck/news/?source=navbar) 介绍每一个新版本的bug修复、原有功能增强和新添加的功能。
- ****讨论: ****[https://sourceforge.net/p/cppcheck/discussion/general/](https://sourceforge.net/p/cppcheck/discussion/general/)
- **示例: **另外，可以在Cppcheck资源库中下载几个特别的检查
- [https://github.com/danmar/cppcheck/tree/master/rules](https://github.com/danmar/cppcheck/tree/master/rules)
- [https://github.com/danmar/cppcheck/tree/master/addons](https://github.com/danmar/cppcheck/tree/master/addons)




官方网站：[http://cppcheck.sourceforge.net/](http://cppcheck.sourceforge.net/)
开源地址：[https://github.com/danmar/cppcheck](https://github.com/danmar/cppcheck)

