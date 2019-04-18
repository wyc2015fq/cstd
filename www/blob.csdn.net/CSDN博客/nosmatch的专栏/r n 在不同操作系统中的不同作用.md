# \r\n"在不同操作系统中的不同作用 - nosmatch的专栏 - CSDN博客
2012年10月18日 11:19:22[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1860标签：[windows																[unix																[编程																[语言																[存储																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
在Windows中：
'\r'  回车，回到当前行的行首，而不会换到下一行；
'\n'  换行，换到当前位置的下一行，而不会回到行首；
回车”（Carriage Return）和“换行”（Line Feed）这两个概念的来历和区别。
 在计算机还没有出现之 前，有一种叫做电传打字机（Teletype Model 33，Linux/Unix下的tty概念也来自于此）的玩意，每秒钟可以打10个字符。但是它有一个问题，就是打完一行换行的时候，要用去0.2秒，正 好可以打两个字符。要是在这0.2秒里面，又有新的字符传过来，那么这个字符将丢失。
于是，研制人员想了个办法解决这个问题，就是在每行后面加两个表示结束的字符。一个叫做“回车”，告诉打字机把打印头定位在左边界；另一个叫做“换行”，告诉打字机把纸向下移一行。这就是“换行”和“回车”的来历，从它们的英语名字上也可以看出一二。
后来，计算机发明了，这两个概念也就被般到了计算机上。那时，存储器很贵，一些科学家认为在每行结尾加两个字符太浪费了，加一个就可以。于是，就出现了分歧。
Unix系统里，每行结尾只有“<换行>”，即"\n"；Windows系统里面，每行结尾是“<换行><回 车>”，即“\n\r”；Mac系统里，每行结尾是“<回车>”，即"\n"；。一个直接后果是，Unix/Mac系统下的文件在 Windows里打开的话，所有文字会变成一行；而Windows里的文件在Unix/Mac下打开的话，在每行的结尾可能会多出一个^M符号。
c语言编程时（windows系统）
/r 就是return 回到 本行 行首 这就会把这一行以前的输出 覆盖掉。
如： 
最后只显示 xixi 而 hahaha 背覆盖了
int main ()     
{                                                                                                          
      cout << "hahaha" << "\r" << "xixi" ; 
}
/n 是回车＋换行 把光标 先移到 行首 然后换到下一行 也就是 下一行的行首拉
int main()
{ 
      cout << "hahaha" << "\n" << "xixi" ; 
}
