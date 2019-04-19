# map/set iterator not dereferencable - 三少GG - CSDN博客
2013年01月15日 12:57:37[三少GG](https://me.csdn.net/scut1135)阅读数：2131

#### //////Grant推荐:////////////
#### QT学习之路：从入门到精通
[http://blog.51cto.com/zt/20/1](http://blog.51cto.com/zt/20/1)
#### ///////////////////////

# [Crash when starting QuteCom 3.0 in Windows](http://www.mail-archive.com.sixxs.org/search?l=qutecom-dev@lists.qutecom.org&q=subject:%22Crash+when+starting+QuteCom+3.0+in+Windows%22)
[Robert
 Verspuy](http://www.mail-archive.com.sixxs.org/search?l=qutecom-dev@lists.qutecom.org&q=from:%22Robert+Verspuy%22)[Thu, 26 May 2011 07:56:59 -0700](http://www.mail-archive.com.sixxs.org/search?l=qutecom-dev@lists.qutecom.org&q=date:20110526)
But when I start Qutecom it crashes right away, with the following messages
 in the log:
> (error) 16:30:03 [Common] ConfigManagerFileStorage::loadSystemConfig: Could not open C:\Program
 Files\qutecom-2.99.0\/config/config.xml (fatal) 16:30:03 [Common] C:\QuteCom\QC3.0\libs\owutil\settings\src\Settings.cpp:163Settings::getAny:
 key 'config.version' does not exist
The cause of this is because Quotecom is looking for a config file in qutecom-2.99.0\/config/config.xml
 but the config file is installed in qutecom-2.99.0/config/config/config.xmlAlso other the subdirs like chat, css, sounds, webpages exist twice.
I'm looking into the nsis script to see if I can find a solution.
But I think this should not give a crash.
When debugging the crash with MS Visual C++ 2008, I saw that, because the
 config.xml could not be found there's somthing going wrong with reading the default configvalues.In owsettings  \ settings.cpp (line 160 -function Settings::getAny)
There is a _KeyMap.find(key) (where key is in this case config.version).
But the _KeyMap is not an object, it's just "(0)[]" (according to the debugger).So I think this _KeyMap object is not correctly initialized, when no configfile
 can be found.
"
Hi,  I just wanted to start wengophone in the debug directory. I know there is a resource parameter, but I can't find it out cause passing --help already tries to access the resource directory.  This is stupid. Initialization is done too early.
"
**解决办法： 将 Resources/config文件夹 移至程序所在的Debug目录下**
[**http://bbs.csdn.net/topics/240085292**](http://bbs.csdn.net/topics/240085292)
while(map_set!=map_temp->second.end())
   {
map_ciku=ciku.find(atoi((*map_set).c_str ()));  
result1.insert(make_pair(map_ciku->second,0));
++(map_result1->second);                           //程序在这里中断了，下面程序执行不了了，
++map_set;
}
怎么不给出具体出错行数？如果是因为直接双击错误定位到了STL库的头文件中：debug的时候看Call Stack（调用堆栈），从上到下一行行双击，最后定位到上面源代码的某一行，错误就在那一行上。
第一行如果find未找到你参数里指定的字符串，将返回ciku.end()，那么map_ciku->second就是错的。
所以一般在第一行后加入一个判断：
C/C++ code
[?](http://bbs.csdn.net/topics/240085292#clipboardWindow)
```
```cpp
if
```
```cpp
(map_ciku != ciku.end())
```
```cpp
{
```
```cpp
```
```cpp
result1.insert(make_pair(map_ciku->second,0));
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
// 未找到则做相应处理
```
```cpp
}
```
```cpp
```
```
# STL迭代器失效问题
**0**人收藏此文章, [我要收藏]()发表于8个月前(2012-04-24
 23:01) , 已有448次阅读 ，共[5](http://my.oschina.net/myspaceNUAA/blog/55053#comments)个评论
最近在项目开发中，遇到一个异常，经过测试，发现是迭代器失效问题，于是稍微总结了一下。
vector迭代器失效测试：
测试程序：
```; toolbar
void vectorTest()
{
    vector<int> container;
    for (int i = 0; i < 10; i++)
    {
        container.push_back(i);
    }
    vector<int>::iterator iter;
     for (iter = container.begin(); iter != container.end(); iter++)
    {
            if (*iter > 3)
              container.erase(iter);
    }
     for (iter = container.begin(); iter != container.end(); iter++)
    {
            cout<<*iter<<endl;
    }
}
```
测试结果如下：
![](http://static.oschina.net/uploads/space/2012/0424/230921_uN13_100374.gif)
结果对吗？肯定不对，结果应该是0,1,2,3。
在看看跟进去之后的报错情况：
![](http://static.oschina.net/uploads/space/2012/0424/231110_Ug1m_100374.gif)
迭代器在执行++操作时报错！
对于序列式容器，比如vector，删除当前的iterator会使后面所有元素的iterator都失效。这是因为顺序容器内存是连续分配，删除一个元素导致后面所有的元素会向前移动一个位置。但是erase方法可以返回下一个有效的iterator。所以代码做如下修改，就OK了。
```; toolbar
void vectorTest()
{
    vector<int> container;
    for (int i = 0; i < 10; i++)
    {
        container.push_back(i);
    }
    vector<int>::iterator iter;
    for (iter = container.begin(); iter != container.end(); )
    {
            if (*iter > 3)
              iter = container.erase(iter);    //erase的返回值是删除元素下一个元素的迭代器
            else{
                iter++;
            }
    }
     for (iter = container.begin(); iter != container.end(); iter++)
    {
            cout<<*iter<<endl;
    }
}
```
运行结果如下：
![](http://static.oschina.net/uploads/space/2012/0424/200341_InBV_100374.bmp)
结果是正确的。
再看一个MAP的示例，
```; toolbar
void mapTest()
{
    map<int, string> dataMap;
    for (int i = 0; i < 100; i++)
    {
           string strValue = "Hello, World";
            stringstream ss;
            ss<<i;
            string tmpStrCount;
            ss>>tmpStrCount;
            strValue += tmpStrCount;
            dataMap.insert(make_pair(i, strValue));
    }
    cout<<"MAP元素内容为："<<endl;
     map<int, string>::iterator iter;
    for (iter = dataMap.begin(); iter != dataMap.end(); iter++)
    {
            int nKey = iter->first;
            string strValue = iter->second;
            cout<<strValue<<endl;
    }
    cout<<"内容开始删除："<<endl;
    /////////////////////////////////////////////擦除操作引发迭代器失效
    for (iter = dataMap.begin(); iter != dataMap.end(); iter++)
    {
            int nKey = iter->first;
            string strValue = iter->second;
           if (nKey % 2 == 0)
           {
                dataMap.erase(iter);
           }
            cout<<iter->second<<endl;
    }
}
```
结果报错：
![](http://static.oschina.net/uploads/space/2012/0424/202401_l0VV_100374.gif)
看看报异常时跟进去在哪里报错：
![](http://static.oschina.net/uploads/space/2012/0424/202741_V5SW_100374.gif)
根据迭代器访问数据时失效！ map/set iterator not dereferencable
迭代器是不引用的。
正确的做法是：
```; toolbar
void mapTest()
{
    map<int, string> dataMap;
    for (int i = 0; i < 100; i++)
    {
          string strValue = "Hello, World";
          stringstream ss;
          ss<<i;
          string tmpStrCount;
          ss>>tmpStrCount;
          strValue += tmpStrCount;
          dataMap.insert(make_pair(i, strValue));
    }
    cout<<"MAP元素内容为："<<endl;
     map<int, string>::iterator iter;
    for (iter = dataMap.begin(); iter != dataMap.end(); iter++)
    {
          int nKey = iter->first;
          string strValue = iter->second;
          cout<<strValue<<endl;
    }
    cout<<"内容开始删除："<<endl;
    for (iter = dataMap.begin(); iter != dataMap.end(); )
    {
         int nKey = iter->first;
         string strValue = iter->second;
         if (nKey % 2 == 0)
         {
             dataMap.erase(iter++);
         }else
	 {
	      iter++;
         }
         if (iter != dataMap.end())
         cout<<iter->second<<endl;
	}
```
最后输出结果：
![](http://static.oschina.net/uploads/space/2012/0424/224552_r0Yw_100374.gif)
程序在一边删除一遍输出，而且能够顺利输出结束。
总结一下：
vector是一个顺序容器，在内存中是一块连续的内存，当删除一个元素后，内存中的数据会发生移动，以保证数据的紧凑。所以删除一个数据后，其他数据的地址发生了变化，之前获取的迭代器根据原有的信息就访问不到正确的数据。
所以为了防止vector迭代器失效，常用如下方法：
```; toolbar
for (iter = container.begin(); iter != container.end(); )
    {
            if (*iter > 3)
              iter = container.erase(iter);    //erase的返回值是删除元素下一个元素的迭代器
            else{
                iter++;
            }
    }
```
这样删除后iter指向的元素后，返回的是下一个元素的迭代器，这个迭代器是vector内存调整过后新的有效的迭代器。
万无一失！
map是关联容器，以红黑树或者平衡二叉树组织数据，虽然删除了一个元素，整棵树也会调整，以符合红黑树或者二叉树的规范，但是单个节点在内存中的地址没有变化，变化的是各节点之间的指向关系。
所以在map中为了防止迭代器失效，在有删除操作时，常用如下方法：
```; toolbar
for (iter = dataMap.begin(); iter != dataMap.end(); )
    {
         int nKey = iter->first;
         string strValue = iter->second;
         if (nKey % 2 == 0)
         {
               map<int, string>::iterator tmpIter = iter;
	       iter++;
               dataMap.erase(tmpIter);
               //dataMap.erase(iter++) 这样也行
         }else
	 {
	      iter++;
         }
   }
```
 map<int, string>::iterator tmpIter = iter; iter++;
dataMap.erase(tmpIter);
这几句的意思是,先保留要删除的节点迭代器，再让iter向下一个有意义的节点，然后删除节点。
所以这个操作结束后iter指向的是下一个有意义的节点，没有失效。
其实这三句话可以用在一句话代替，就是dataMap.erase(iter++);
解释是先让iter指向下一个有效的节点，但是返回给erase函数的是原来的iter副本。这个可能跟++这个操作的本身语法相关。
但是功能跟上面是一样的。

