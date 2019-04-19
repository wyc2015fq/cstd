# vector中erase用法注意事项 - 三少GG - CSDN博客
2014年08月18日 09:26:24[三少GG](https://me.csdn.net/scut1135)阅读数：556
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
以前就发现了vector中的erase方法有些诡异(^_^)，稍不注意，就会出错。今天又一次遇到了，就索性总结一下，尤其是在循环体中用erase时，由于vector.begin() 和vector.end()是变化的，因此就引入了错误的可能性。
erase的函数原型有两种形式:
iterator erase(iterator position);
iterator erase(iterator first, iterator last);
vector<int> veci;
veci.push_back(1);
veci.push_back(2);
veci.push_back(3);
veci.push_back(4);
veci.push_back(5);
veci.push_back(3);
veci.push_back(2);
veci.push_back(3);
for(vector<int>::iterator iter=veci.begin(); iter!=veci.end(); iter++)
{
      if( *iter == 3)
             veci.erase(iter);
}
乍一看这段代码，很正常。其实这里面隐藏着一个很严重的错误：当veci.erase(iter)之后，iter就变成了一个野指针，对一个野指针进行 iter++ 是肯定会出错的。
查看MSDN，对于erase的返回值是这样描述的：An iterator that designates the first element remaining beyond any elements removed, or a pointer to the end of the vector if no such element exists，于是改代码：
for(vector<int>::iterator iter=veci.begin(); iter!=veci.end(); iter++)
{
      if( *iter == 3)
             iter = veci.erase(iter);
}
这段代码也是错误的**：1）无法删除两个连续的"3"； **2）**当3位于vector最后位置的时候**，也会出错（在veci.end()上执行 ++ 操作）
正确的代码应该为：
for(vector<int>::iterator iter=veci.begin(); iter!=veci.end(); )
{
     if( *iter == 3)
          iter = veci.erase(iter);
      else
            iter ++ ;
}
