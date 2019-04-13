
# main(int argc, char --argv) - peghoty - CSDN博客


2013年11月12日 16:24:07[皮果提](https://me.csdn.net/peghoty)阅读数：1916标签：[C																](https://so.csdn.net/so/search/s.do?q=C&t=blog)[main																](https://so.csdn.net/so/search/s.do?q=main&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C&t=blog)个人分类：[编程知识																](https://blog.csdn.net/peghoty/article/category/1451023)



C 语言中有关命令行参数涉及到程序的主函数 main() 中的 int argc, char *argv[] （或者 char **argv） 这样两个参数，其中，int argc 表示命令行参数的个数（包括可执行程序名本身），char
 *argv[] 表示每个参数的具体内容，argv[0] 为命令行中可执行程序名本身，argv[1] 为命令行中第二个参数的内容，依次类推。

如下例输出命令行参数的个数及参数的内容：

```python
main (int argc,char *argv[])
{
   int i;
   printf("\n命令行中可执行文件名为：％s", argv[0]);
   printf("\n总共有％d个参数：", argc);
   i = 0;
   while (argc >= 1)
   {
      printf(″％s 　", argv[i++]);
      argc--;
   }
}
```



