# Effective STL-各种容器正确删除元素方法 - yixianfeng41的专栏 - CSDN博客
2017年02月23日 10:27:41[第2梦](https://me.csdn.net/yixianfeng41)阅读数：585
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
### **一、去除一个容器中有特定值得所有对象**
1、如果容器是vector、string或deque，使用erase-remove惯用法
`c.erase(remove(c.begin,c.end(),value),c.end());//c为容器，value为待删值`
2、对于list
`c.remove(value);`
3、对于关联容器 
关联容器无remove成员函数，它是调用erase函数。
`c.erase(value);`
### **二、去除容器中满足特定判别式的所有值**
设判定式为
`bool badvalue(int x); //返回x是否是“bad"`
1、对于序列容器vector、string或deque，使用erase-remove-if惯用法。
`c.erase(remove_if(c.begin(),c.end(),badvalue),c.end());`
2、对于list,使用list::remove_if
```bash
c.remove_if(badvalue);
```
3、对于标准关联容器，方法是写一个循环来遍历容器元素，当把迭代器传给erase时，记得后置递增它。
首先来看看大家马上就可以写出来的错误代码：
```matlab
for(container<int> ::iterator i=c.begin();i!=c.end();++i）
{
    if(badvalue(*i))
       c.erase(i);
 }
```
上述代码是错的，为什么是错的了？因为关联容器删除一个元素后，指向这个元素的迭代器就失效了。所以这个代码是错误的。
我们要解决这个问题，那么就要得到下一个迭代器，方法较多，最容易的办法是，i使用后置递增。代码如下：
```
for(container<int> ::iterator i=c.begin();i!=c.end();）
    {
       if(badvalue(*i))
          c.erase(i++);
       esle
          ++i;
    }
```
上述代码i传给了erase，完成了删除，同时i自己又加了1,满足需求。
### **三、在循环中做某些其他事**
比如:写入日志文件
1、对于vector,string,deque，惯用的erase-remove_if 没法进行写入日志操作，因此，我们也得写个循环。要注意的是，这几个容器，当发生删除操作后，后面的迭代器全部失效。那么代码应该这样写：
```
for(container<int> ::iterator i=c.begin();i!=c.end();）
{
   if(badvalue(*i))
   {
       //logfile
       i=c.erase(i);
   }      
   esle
      ++i;
}
```
2、对于关联容器
```
for(container<int> ::iterator i=c.begin();i!=c.end();）
  {
     if(badvalue(*i))
     {
       //logfile
       c.erase(i++);
     }      
     esle
        ++i;
  }
```
