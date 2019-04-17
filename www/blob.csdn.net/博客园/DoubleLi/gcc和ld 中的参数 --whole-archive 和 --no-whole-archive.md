# gcc和ld 中的参数 --whole-archive 和 --no-whole-archive - DoubleLi - 博客园






首先 --whole-archive 和 --no-whole-archive 是ld专有的命令行参数，gcc 并不认识，要通gcc传递到 ld，需要在他们前面加 -Wl，字串。

--whole-archive 可以把 在其后面出现的静态库包含的函数和变量输出到动态库，--no-whole-archive 则关掉这个特性。

比如你要把 liba.a  libb.a libc.a 输出到 libabc.dll(或libabc.so)时应该这么写：

libabc.dll:liba.c libb.a libc.a

       gcc  -shared -o $@ -L. -Wl,--whole-archive -la -lb -lc -Wl,--no-whole-archive

在--whole-archive作用下的库里不能有函数同名。









