# python c++ 重定向标准输入和 读入未知行数_用户5706034127_新浪博客
|||
**int main() {**
**#ifdef 12346**
**freopen("C:\\Users\\Desktop\\case.in", "r",stdin);**
**freopen("C:\\Users\\Desktop\\case.out", "w",stdout);**
**#endif**
**while(cin >> a >> b)//注意while处理多个case**
** cout << a+b << endl;**
**return 0;**
**}**
=========================================================
#coding:utf-8
def foo():
 for line in
sys.stdin:
 print line  
try :
 import tzl2093
 import sys
 _tmp_stdin =
sys.stdin
 sys.stdin =
open('testdata.txt')
 foo()
 sys.stdin.close()
 sys.stdin =
_tmp_stdin
except ImportError:
 foo()
========================================
c和c++重定向后，还要记得恢复
这个对经常在OJ上做题的童鞋们很有用。OJ基本都是用标准输入输出（USACO除外）。但如果你在调试的时候也都是从控制台输入，那就太浪费宝贵的时间了。我们可以重定向标准输入，调试的时候从文件读，提交时从标准输入读。
在[C语言](http://lib.csdn.net/base/c)中，方法比较简单。使用函数freopen()：
- **freopen("data.in","r",stdin); **
- **freopen("data.out","w",stdout); **
这样就把标准输入重定向到了data.in文件，标准输出重定向到了data.out文件。
这两句代码之后，scanf函数就会从data.in文件里读，而printf函数就会输出到data.out文件里了。
C++中，对流重定向有两个重载函数：
- 
streambuf* rdbuf () const; 
- 
streambuf* rdbuf (streambuf *) 
就相当于get/set方法。
- **streambuf *backup; **
- **ifstream fin; **
- **fin.open("data.in"); **
- **backup = cin.rdbuf(); // back up cin's streambuf**
- **cin.rdbuf(fin.rdbuf()); // assign file's streambuf to cin**
- **// ... cin will read from file**
- **cin.rdbuf(backup); // restore cin's original streambuf**
注意最后我们使用了cin.rdbuf(backup)把cin又重定向回了控制台
然而，如果用[c语言](http://lib.csdn.net/base/c)实现同样的功能就不那么优雅了。
因为标准控制台设备文件的名字是与[操作系统](http://lib.csdn.net/base/operatingsystem)相关的。
在Dos/Windows中，名字是con
freopen("con", "r", stdin);
在[Linux](http://lib.csdn.net/base/linux)中，控制台设备是/dev/console
freopen("/dev/console", "r", stdin);
另外，在类unix系统中，也可以使用dup系统调用来预先复制一份原始的stdin句柄。
