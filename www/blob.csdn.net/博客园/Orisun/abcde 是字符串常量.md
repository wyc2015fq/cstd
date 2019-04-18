# "abcde"是字符串常量 - Orisun - 博客园







# ["abcde"是字符串常量](https://www.cnblogs.com/zhangchaoyang/articles/2347709.html)





main(){

　　char *p1="abcde";　　　　//p1是main函数中的局部变量，存放在栈区；"abcde"是字符串常量，存放在数据区

　　char *p2=(char*)malloc(10);　　//p2存放在栈区；malloc来的10个字节位于堆区

　　char *p3=dup(p1);　　//dup背后调用了malloc

　　p1[0]='w';　　　　//错误，字符串常量不可修改

　　char *p4=index(p1,'d');　　//p4指向了数据区中的"abcde"中的'd'

　　p4='w';　　//错误，字符串常量不可修改

　　char *p5=index(p3,'d');　　//p5指向了堆区中的"abcde"中的'd'

　　p5='w';　　//正确

　　printf("%s\n",p5);　　　　//输出we

　　printf("%s\n",p3);　　　　//输出abcwe

　　free(p2);

　　free(p3);

}












