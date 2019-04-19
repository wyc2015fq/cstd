# hexdump 的 5 个使用小技巧 - 文章 - 伯乐在线
原文出处： [fireaxe （ fireaxe_hq@hotmail.com ）](http://blog.chinaunix.net/uid-20528014-id-4087756.html)
开发时经常会需查看非文本文件内容，最常见的16 进制查看器就是 hexdump。hexdump 看似简单，但还是有些小技巧的。
以文本文件tmpfile为例：
- Happy New Year!
- Happy New Year!
- Happy New Year!
### 1)  最简单的方式
**hexdump testfile**


```
0000000 6148 7070 2079 654e 2077 6559 7261 0a21
*
0000030
```
这种方式是以两个字节为一组，其顺序取决于本机字节序。比如在x86架构上就是以blittle-endian方式显示，看起来会很费劲。
如第一行翻译成ascii码：
> 
6148 7070 2079 654e 2077 6559 7261 0a21
aH     pp      y     Ne      w     e      ae     !r
为了避免这种情况，就要用到下面的”以字节方式查看“
### 2)  以字节方式查看
**hexdump -C testfile -s skip -n number**


```
00000000  48 61 70 70 79 20 4e 65  77 20 59 65 61 72 21 0a  |Happy New Year!.|
*
00000030
```
这种方式就不会有字节序问题了，而且还能同时显示16进制与ascii码，但存在的问题是，如果某几行的内容相同，会省略掉后几行，这个有时也会造成麻烦。如何避免省略呢？
### 3) 不要省略
**hexdump -v testfile**


```
0000000 6148 7070 2079 654e 2077 6559 7261 0a21
0000010 6148 7070 2079 654e 2077 6559 7261 0a21
0000020 6148 7070 2079 654e 2077 6559 7261 0a21
0000030
```
现在省略是没有了，但又回到了两字节显示的老路上，所以我们要把两种方式结合起来。
### 4) 字节方式显示且不要省略
**hexdump -Cv testfile**


```
00000000  48 61 70 70 79 20 4e 65  77 20 59 65 61 72 21 0a  |Happy New Year!.|
00000010  48 61 70 70 79 20 4e 65  77 20 59 65 61 72 21 0a  |Happy New Year!.|
00000020  48 61 70 70 79 20 4e 65  77 20 59 65 61 72 21 0a  |Happy New Year!.|
00000030
```
现在就好看多了，既是以单字节方式显示，又不会省略了。但有时文件很大，我们只想查看其中一段，怎么办呢？
### 5) 显示某一段
**hexdump -Cv testfile -s skip -n number**


```
fireaxe $ hexdump -Cv tmp.txt -n 1
00000000  48                                                |H|
00000001
fireaxe $ hexdump -Cv tmp.txt -n 2
00000000  48 61                                             |Ha|
00000002
fireaxe $ hexdump -Cv tmp.txt -n 3
00000000  48 61 70                                          |Hap|
00000003
fireaxe $ hexdump -Cv tmp.txt -s 1 -n 1
00000001  61                                                |a|
00000002
fireaxe $ hexdump -Cv tmp.txt -s 2 -n 1
00000002  70                                                |p|
00000003
fireaxe $ hexdump-Cv tmp.txt -s 3 -n 1
00000003  70                                                |p|
00000004
```
OK， 整个显示过程都可以自由控制了。
