# C语言的指针初始化特别注意一点 - maopig的专栏 - CSDN博客
2012年01月15日 16:59:52[maopig](https://me.csdn.net/maopig)阅读数：712标签：[语言																[c																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
void func2(int *value)
{
       *value = 2; /// value为空指针，不能被取值，所以*value是错误的
}
void func1()
{
       int *p = 0;//此处相当于P=NULL
       func2(p);
}
/////////////////////////////////////////////////////////////////
void func2(int *value)
{
       *value = 2; /// 正确}
void func1()
{
      int a=0; 
      int *p = &a;
       func2(p);
}
