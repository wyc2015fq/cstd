# Makefile第一讲：一个简单的Makefile - DoubleLi - 博客园






摘要 假定你对linux已经比较的熟悉，假定你编程已经稍有经验，本文不会对文章作出太多基础性解释，看不懂莫怪，只当作给学习的朋友一个引导思路，我也是一个初学者，边学边写，将学会的教给大家，文章有错误之处误导了朋友，请轻喷

[makefile](http://www.oschina.net/search?scope=blog&q=makefile)




*假定你对linux已经比较的熟悉，假定你编程已经稍有经验，本文不会对文章作出太多基础性解释，看不懂莫怪，只当作给学习的朋友一个引导思路，我也是一个初学者，边学边写，将学会的教给大家，文章有错误之处误导了朋友，请轻喷*

#### main.cpp

```cpp
#include <iostream>
using namespace std;

int main() {
    cout <<"Hello makefile";
    return 0;
}
```

#### makefile

```
test:main.o
# 复制代码请注意，开头必须是一个tab，而不能是四个空格， 否则make会报错，这是必须的
    g++ main.o -o test

main.o:main.cpp
    g++ -c main.cpp -o main.o

clean:
    rm main.o test
```

#### 在此，我会一行一行的解释给大家，暂且把形如这样的一个代码块命名为编译块吧，它俩是密不可分的一个整体：

```
test:main.o
    g++ main.o -o test
```
- 要特别注意的是复制代码后，一定要把每行前边的空格换成tab，***在makefile中tab是命令的开始标志，tab后面的被当作命令执行*** ，如果是空格在make时肯定会报错
- `test:main.o`告诉make命令，执行一个叫test的编译块，这个编译块依赖`main.o`文件（这句啥意思？请继续看），test编译块会执行命令`g++ main.o -o test`。g++表明这是编译c++程序，如果是c程序，请用gcc。这就是题外话了，与makefile无关，如果想了解gcc和g++命令，请在终端输入: `g++ --help`
- `main.o:main.cpp`与上例类似，就不说了，上边提到test编译块依赖main.o文件，依赖的正是它，也就是说当执行test编译块时，test编译块需要main.o这个文件，于是它就到目录里去找main.o，但是它在目录里找不到main.o，这时他就在makefile文件里找main.o这样一个编译块，于是它就找到了`main.o:main.cpp`，接着它执行这个编译块下边的命令，然后生成了main.o这个文件，然后再执行test编译块中的命令`g++ main.o -o test`，最终生成可执行文件./test，由于这是一个最简单的，没任何依赖的C++程序，因此：其实main.o这个文件就是一个可执行文件，不信你在命令行试试：`./main.o`（可能需要改为可执行权限`chmod u+x main.o`）
- `clean:`编译块告诉make 当在终端执行`make clean`时执行`rm main.o test`命令，看到`make clean`你是否想到了在终端执行：`make main.o`和`make test`命令呢？没有？这学习态度不太好哟，当你执行了这两个命令，脑子里应该立刻出现一个疑问：为毛我执行`make clean`命令就执行clean编译块，而我执行`make`命令却执行test编译块，我并没有告诉make我要执行test语句块啊？make是有约定的，如果你不指定让它执行哪个编译块，他就默认执行第一个编译块，不妨把clean编译块和第一个互换试试！










