# Boost::filesystem的使用笔记 - wishchinYang的专栏 - CSDN博客
2013年09月22日 16:15:02[wishchin](https://me.csdn.net/wishchin)阅读数：4713
### [Boost::filesystem 使用小笔记](http://blog.csdn.net/dourgulf/article/details/8589207)
[http://blog.csdn.net/dourgulf/article/details/8589207](http://blog.csdn.net/dourgulf/article/details/8589207)
boost 官方文档：[http://en.highscore.de/cpp/boost/filesystem.html](http://en.highscore.de/cpp/boost/filesystem.html)
1. path对象就是一个跨平台的路径对象。有许多方法访问路径的各个部分，也用它的iterator迭代路径中的各个部分；
     path构造目录结构的时候使用“/”运算符，非常直观。
     比如path p1;
     path p2 = p1/"something"; p1 /= "xxx.xxx";
2. filesystem名字空间一下有一些全局的函数，比如exists可以判断path是不是存在，is_directory函数判断是不是目录，file_size获得大小--该大小是一个夸平台的类型，可以表示32位或者64的大小；
     其他is方法还有：
```cpp
is_empty
     is_other
     is_regular_file
     is_symlink
```
3. 最方便的一个功能是遍历path里的所有内容。directory_iterator。
     path p;
     directory_iterator(p)就是迭代器的起点，无参数的directory_iterator()就是迭代器的终点。
还可以递归迭代，把上面的directory_iterator换成recursive_directory_iterator即可。
4. 创建目录。这里特别要提到一个方法是bool create_directories(const path& p);  如果p是一个目录（也就是is_diretory返回true）。它会递归的创建整个目录结构，免去自己一个一个创建的烦恼了。
其他创建方法还有：
```cpp
create_directories
create_directory
create_hard_link
create_symlink
```
5. 还可以复制目录  
```cpp
copy_directory
copy_file
copy_symlink
```
6. 删除remove 递归删除remove_all
7. 改名字rename
8. 如果包含了<boost/filesystem/fstream.hpp>的话，还可以让fstream接受path作为参数。
BTW，使用filesystem需要编译boost，现在版本只要执行一个脚本就能编译完成了，比以前方便多了。
不过，还是要提一下Xcode环境下使用的话，要选择GCC C++ standard library。
![](https://img-my.csdn.net/uploads/201302/18/1361194241_7412.png)
GCC的哭虽然没有LLVM 的库先进（我是说支持C++11方面），不过，大部分的C++11标准还是支持的。
目前发现是GCC的库没有std::regex。不过，既然用boost的话那就用boost的regex好了。反正std的也是从boost拿过来而已。
**遗忘的角落的日志 比较详细**
[http://hi.baidu.com/lingyu125/item/7bb0a8cf73a8fe11b77a2460](http://hi.baidu.com/lingyu125/item/7bb0a8cf73a8fe11b77a2460)
## Boost::fileSystem
ZZ：http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#resources
对于 I/O 操作较多的程序，这样的不一致就意味着需要进行大量的工程工作才能在平台间移植代码。正是因为这个原因，我们才引入了 Boost Filesystem Library。这个广泛使用的库提供了安全、可移植且易用的`C++` 接口，用于执行文件系统操作。可以从[Boost](http://www.boost.org/libs/filesystem) 站点免费下载此库。
使用 boost::filesystem 的第一个程序
在深入研究 Boost Filesystem Library 的更多细节之前，请看一下[清单 1](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list1) 中所示的代码；此代码使用 Boost API 确定某个文件的类型是否为 Directory。
```cpp
#include <stdio.h>
#include “boost/filesystem.hpp”
int main()
{
boost::filesystem::path path("/usr/local/include"); // random pathname
bool result = boost::filesystem::is_directory(path);
printf(“Path is a directory : %d"n”, result);
return 0;
}
```
此代码非常明了易懂，您并不需要了解任何系统特定的例程。此代码经过验证，能在不用修改的情况下在 gcc-3.4.4 和 cl-13.10.3077 上成功编译。
**了解 Boost path 对象**
了解 Boost Filesystem Library 的关键是 `path` 对象，因为 Filesystem Library 中定义的多个例程都要对相应的`path` 对象操作。文件系统路径通常依赖于操作系统。例如，众所周知，UNIX 和 Linux 系统使用正斜杠 (` /`) 字符作为目录分隔符，而 Windows 将反斜杠 (`"`) 字符用于类似的用途。`boost::filesystem::path`
 旨在准确地抽象此特性。`path` 对象可以通过多种方式进行初始化，最常见的方式是使用`char*` 或 
```
std::string
```
 进行初始化，如[清单 2](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list2) 中所示。
**清单 2. 创建 Boost path 对象的方法**
```cpp
path(); // empty path
path(const char* pathname);
path(const std::string& pathname);
path(const char* pathname, boost::filesystem::path::name_check checker);
path(const char* pathname, boost::filesystem::path::name_check checker);
```
在初始化 `path` 对象时，可以采用本机格式或可移植操作系统接口（Portable Operating System Interface，POSIX）委员会定义的可移植格式提供 PATHNAME 变量。这两种方法在实际中各有优缺点。考虑以下情况：您希望操作软件所创建的目录，此目录在 UNIX 和 Linux 系统上位于 /tmp/mywork，而在 Windows 上位于 C:"tmp"mywork。可以采用多种方法处理问题。[清单
 3](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list3) 显示了面向本机格式的方法。
**清单 3. 使用本机格式初始化 path**
```cpp
#ifdef UNIX
boost::filesystem::path path("/tmp/mywork");
#else
boost::filesystem::path path("C:""tmp""mywork ");
#endif
```
需要单个 `#ifdef` 来按操作系统初始化 path 对象。不过，如果您喜欢使用可移植格式，请参见[清单 4](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list4)。
**清单 4. 使用可移植格式初始化 path**
boost::filesystem::path path("/tmp/mywork"); 
请注意，`path::name_check` 指的是一个名称检查函数原型。如果其参数输入 PATHNAME 对于特定的操作系统或文件系统有效，名称检查函数将返回“True”。Boost Filesystem Library 提供了多个名称检查函数，而且也欢迎您提供自己的变体。常用的名称检查函数是 Boost 提供的`portable_posix_name` 和`windows_name`。
path 成员函数概述
`path` 对象提供了多个成员方法。这些成员例程并不会修改文件系统，但会根据 path 名称提供有用的信息。此部分提供了其中几个例程的概述：
- **`const std::string& string( )`：**此例程会返回用于初始化 path 的字符串的副本，其格式符合 path 语法规则。
- **`std::string root_directory( )`：**在提供了路径的情况下，此 API 将返回根目录，否则将返回空字符串。例如，如果路径包含*/tmp/var1*，则此例程将返回`/`，即 UNIX 文件系统的根。不过，如果路径是相对路径，如*../mywork/bin*，此例程将返回空字符串。
- **`std::string root_name( )`：**在给定从文件系统根目录开始的路径的情况下，此例程将返回包含 PATHNAME 的第一个字符的字符串。
- **`std::string leaf( )`：**在给定绝对路径名称（例如，/home/user1/file2）的情况下，此例程将提供与文件名称对应的字符串（即 file2）。
- **`std::string branch_path( )`：**这是与 `leaf` 互补的例程。在给定路径的情况下，将会返回其构造所用的所有元素（除了最后一个元素）。例如，对于使用*/a/b/c* 初始化的 path，`path.branch_path( )` 将返回`/a/b`。对于包含单个元素的路径，如*c*，此例程将返回空字符串。
- **`bool empty( )`：**如果 path 对象包含空字符串（例如 *path path1("")*），则此例程将返回 True。
- **`boost::filesystem::path::iterator`：**此例程用于遍历 path 的各个元素。请看[清单 5](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list5) 所示的代码。
**清单 5. 使用 path::iterator（begin 和 end 接口）**
- 
```cpp
#include <iostream>
#include “boost/filesystem.hpp”
int main()
{
boost::filesystem::path path1("/usr/local/include"); // random pathname
boost::filesystem::path::iterator pathI = path1.begin();
while (pathI != path1.end())
{
std::cout << *pathI << std::endl;
++pathI;
}
return 0;
}
// result: 1
```
上述程序的输出依次是 `/`、`usr`、`local`、`include`，代表了该目录的层次结构。
- **`path operator / (char* lhs, const path& rhs)`：**此例程是`path` 的非成员函数。它将返回使用`lhs` 和`rhs` 形成的路径的串联值。它将自动插入`/` 作为路径分隔符，如[清单
 6](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list6) 中所示。
**清单 6. 路径字符串的串联**
```cpp
#include <iostream>
#include “boost/filesystem.hpp”
int main()
{
boost::filesystem::path path1("/usr/local/include"); // random pathname
boost::filesystem::path::iterator pathI = path1.begin();
while (pathI != path1.end())
{
std::cout << *pathI << std::endl;
++pathI;
}
return 0;
}
// result: 1
```
**错误处理**
文件系统操作经常遇到意外的问题，Boost Filesystem Library 将使用 `C++` 异常报告运行时错误。`boost::filesystem_error` 类派生自`std::runtime_error` 类。库中的函数使用`filesystem_error` 异常报告操作错误。与不同的可能错误类型对应，Boost 头文件定义了相应的错误代码。用户代码通常驻留在`try...catch`
 块内，使用`filesystem_error` 异常来报告相关错误消息。[清单 7](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list7) 提供了重命名文件的小示例，在`from` 路径中的文件不存在时引发异常。
**清单 7. Boost 中的错误处理**
```cpp
#include <iostream>
#include “boost/filesystem.hpp”
int main()
{
try {
boost::filesystem::path path("C:""src""hdbase""j1");
boost::filesystem::path path2("C:""src""hdbase""j2");
boost::filesystem::rename(path, path2);
}
catch(boost::filesystem::filesystem_error e) {
// do the needful
}
return 0;
}
```
Boost Filesystem Library 中的函数类别`boost::filesystem` 提供了不同类别的函数：有些函数（如`is_directory`）用于查询文件系统，而其他函数（如`create_directory`）则主动对文件系统进行修改。根据各自功能的不同，这些函数可以大略归入以下类别：
- **属性函数：**提供杂项信息，如文件大小、磁盘使用量等。
- **文件系统操作函数：**用于创建常规文件、目录和符号链接；复制和重命名文件；提供删除功能。
- **实用工具：**测试文件的扩展名等。
- **杂项常规函数：**以编程方式更改文件扩展名等。
属性函数
Boost Filesystem Library 包括以下属性函数：
- **`uintmax_t file_size(const path&)`：**返回常规文件的大小（以字节为单位）
- **`boost::filesystem::space_info space(const path&)`：**接受路径作为输入，并返回定义如下的`space_info` 结构： struct space_info {
uintmax_t capacity;
uintmax_t free;
uintmax_t available;
}; 根据文件系统所属的磁盘分区，此流程将对该分区的所有目录返回相同的磁盘使用量统计数据（以字节为单位）。例如，对于 C:"src"dir1 和 C:"src"dir2，都会返回相同的磁盘使用数据。
- **`std::time_t last_write_time(const path&)`：**返回文件的最后修改时间。
- **`void last_write_time(const path&, std::time_t new_time)`：**修改文件的最后修改时间。
- **`const path& current_path( )`：**返回程序的当前工作目录的完整路径（注意，此路径与最初运行程序的路径*可能不同*，因为可能采用编程方式更改目录）。
文件系统操作函数
这组函数负责进行新文件和目录创建、文件删除等操作：
- **`bool create_directory(const path&)`：**此函数使用给定的路径名称创建目录。（请注意，如果 PATHNAME 本身包含无效字符，则结果经常是由平台定义的。例如，在 UNIX 和 Windows 系统中，星号 (`*`)、问号 (`?`) 及其他此类字符视为无效，不能出现在目录名称中。）
- **`bool create_directories(const path&)`：**与创建单个目录相对，您可以使用此 API 创建目录树。例如，以目录树 /a/b/c 为例，必须在 /tmp 文件夹内创建此目录树。可调用此 API 完成任务，但使用相同的参数调用`create_directory` 时将引发异常。
- **`bool create_hard_link (const path& frompath, const path& topath)`：**此函数在`frompath` 和`topath` 间创建硬链接。
- **`bool create_symlink(const path& frompath, const path& topath)`：**此函数在`frompath` 和`topath` 间创建符号（软）链接。
- **`void copy_file(const path& frompath, const path& topath)`：**将`frompath` 引用的文件的内容和属性复制到`topath` 引用的文件中。例程**expects a destination file to be absent**；如果存在目标文件，则会引发异常。因此，此函数与 UNIX 中系统指定的`cp`
 命令并不等效。另外，此函数还预期`frompath` 变量将引用正确的常规文件。请看以下示例：`frompath` 引用符号链接 /tmp/file1，而后者反过来引用文件 /tmp/file2；而`topath` 可以为 /tmp/file3。在这种情况下，`copy_file` 将失败。这是此 API 与`cp` 命令相比的另一个差别。
- **`void rename(const path& frompath, const path& topath)`：**此函数是用于重命名文件的 API。可以通过在`topath` 参数中指定完整路径名来同时重命名和更改文件的位置，如[清单
 8](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list8) 中所示。
**清单 8. Boost 中的重命名功能**
```cpp
#include <stdio.h>
#include “boost/filesystem.hpp”
int main()
{
boost::filesystem::path path("/home/user1/abc");
boost::filesystem::rename(path, "/tmp/def");
return 0;
}
// abc is renamed def and moved to /tmp folder
```
- **`bool remove(const path& p)`：**此例程将尝试删除路径 *p* 所引用的文件或目录。对于目录的情况，如果目录的内容不为空，则此例程将引发异常。警告：此例程*并不考虑所删除的内容*，即使其他程序在访问同一文件也如此！
- **`unsigned long remove_all(const path& p)`：**此 API 尝试删除路径*p* 所引用的文件或目录。与`remove` 不同，此函数并不会特殊考虑不为空的目录。此函数是 UNIX`rm –rf` 命令的 Boost 对等项。
实用工具
Boost Filesystem Library 包含以下实用工具：
- **`bool exists(const path&)`：**此函数检查文件的扩展名。文件可以为任何类型：常规文件、目录、符号链接等等。
- **`bool is_directory(const path&)`：**此函数检查路径是否与目录对应。
- **`bool is_regular(const path&)`：**此函数检查普通文件（即此文件不是目录、符号链接、套接字或设备文件）。
- **`bool is_other(const path&)`：**通常，此函数检查设备文件（如 /dev/tty0）或套接字文件。
- **`bool is_empty(const path&)`：**如果路径与文件夹对应，此函数将检查文件夹是否为空，并据此返回“True”或“False”。如果路径与文件对应，此函数将检查文件的大小是否等于 0。对于文件的硬链接或符号链接的情况，此 API 将检查原始文件是否为空。
- **`bool equivalent(const path1& p1, const path2& p2)`：**此 API 非常实用，可用于比较相对路径和绝对路径名。请看[清单 9](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list9)：
**清单 9. 测试两个路径是否等效**
```cpp
#include <stdio.h>
#include “boost/filesystem.hpp”
int main()
{
boost::filesystem::path path1("/usr/local/include"); // random pathname
boost::filesystem::path path2("/tmp/../usr/local/include");
bool result = boost::filesystem::is_equivalent(path1, path2);
printf(“Paths are equivalent : %d"n”, result);
return 0;
}
// result: 1
```
- **`path system_complete(const path&)`：**此函数是与 `bool equivalent(const path1& p1, const path2& p2)` 同一系列的另一个 API。在给定当前工作目录中任意文件路径的情况下，此 API 将返回该文件的绝对路径。例如，如果用户位于目录 /home/user1 并查询文件 ../user2/file2，此函数将返回`/home/user2/file2`，即文件
 file2 的完整路径名。
杂项函数
Boost Filesystem Library 包括以下杂项函数：
- **`std::string extension(const path&)`：**此函数以前面带句点 (`.`) 的形式返回给定文件名的扩展名。例如，对于文件名为*test.cpp* 的文件，`extension` 将返回`.cpp`。对于文件没有扩展名的情况，此函数将返回空字符串。对于隐藏文件（即 UNIX 系统中文件名以`.` 开始的文件），此函数将相应地计算扩展名类型或返回空字符串（因此，对于*.test.profile*，此例程将返回`.profile`）。
- **`std::string basename(const path&)`：**这是与 `extension` 互补的例程。它将返回文件名中`.` 之前的字符串。请注意，即使提供了绝对文件名，此 API 仍然仅会返回属于文件名的直接部分，如[清单
 10](http://www.ibm.com/developerworks/cn/aix/library/au-boostfs/#list10) 中所示。
**清单 10. 使用 boost::basename**
```cpp
#include <stdio.h>
#include <cstring>
#include “boost/filesystem.hpp”
use namespace std;
int main()
{
boost::filesystem::path path1("/tmp/dir1/test1.c ");
boost::filesystem::path path2("/tmp/dir1/.test1.profile");
string result1 = boost::filesystem::basename (path1);
string result2 = boost::filesystem::basename (path2);
printf(“Basename 1: %s Basename2 : %s"n”, result1.c_str(), result2.c_str());
return 0;
}
// result: Basename1: test1 Basename2: .test1
```
- **`std::string change_extension(const path& oldpath, const std::string new_extension)`：**此 API 将返回反映更改后的名称的新字符串。请注意，与`oldpath` 对应的文件保持**不变**。这只是一个常规函数。另请注意，您必须显式地在扩展名中指定*点*。例如，
```
change_extension("test.c",
 "so")
```
 会得到`testso`，而不是 `test.so`。
from the boost.org
ONLY. That may be useful for dealing with legacy compilers or operating systems.
