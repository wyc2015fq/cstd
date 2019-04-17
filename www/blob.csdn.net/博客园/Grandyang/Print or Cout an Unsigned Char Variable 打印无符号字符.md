# Print or Cout an Unsigned Char Variable 打印无符号字符 - Grandyang - 博客园







# [Print or Cout an Unsigned Char Variable 打印无符号字符](https://www.cnblogs.com/grandyang/p/4749473.html)







在C++中，unsigned char用来表示一个字节，也就是8位大小的值，那么我们如何来打印出其值呢，用cout直接打印会乱码，我们可以通过下面两种方法来打印：



```
cout << static_cast<unsigned>(screen[i]) << endl;
printf("%u\n", screen[i]);
```














