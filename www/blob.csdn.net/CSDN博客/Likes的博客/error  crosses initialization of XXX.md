# error:   crosses initialization of XXX - Likes的博客 - CSDN博客
2018年12月20日 14:08:15[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：32
### 错误代码：
```cpp
int main()
{
  int n=2;
  switch(n)
  {
    case 1:
      int temp=9;
    case 2:
    default:
  }
  return 0;
}
```
### 错误原因：
所有的case和default没有使用{ }，这意味着使用的是一个共同的作用域（switch的作用域）。编译器认为在case1中定义的temp可能在case2中使用（可能，但是代码中没有使用），如果在case2中使用temp，则会出现未定义，于是编译器“机智”地向你报告了这一个错误。
### 修改：
```cpp
int main()
{
  int n=2;
  switch(n)
  {
    case 1:
    {
        int temp=9;
    }
    case 2:
    {
    }
    default:
    {
    }
  }
  return 0;
}
```
每个case和default中使用{ }限定作用域即可。
其实养成一个好的编程习惯是最重要的，每个case下应该习惯性地加上{ }
