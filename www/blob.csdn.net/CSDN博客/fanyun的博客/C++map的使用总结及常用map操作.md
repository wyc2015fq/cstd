# C++map的使用总结及常用map操作 - fanyun的博客 - CSDN博客
2017年02月26日 21:12:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：713
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一、C++中map简介:**
C++中map容器提供一个键值对（key/value）容器，map与multimap差别仅仅在于multiple允许一个键对应多个值。需要包含头文件map。对于迭代器来说，可以修改实值，而不能修改key。Map会根据key自动排序。
使用时需要加入头文件：
  #include   <map> 
**二、C++ map定义和初始化**
    map<int,string>map1;                  //空map
**三、C++ map常用操作方法**
    map1[3] = "beijing";                    //添加元素
    map1.insert(map<int,string>::value_type(2,"xian"));//插入元素
    //map1.insert(pair<int,string>(1,"china"));
   map1.insert(make_pair<int,string>(4,"V5"));
    string str =map1[3];                  //根据key取得value，key不能修改
    map<int,string>::iteratoriter_map = map1.begin();//取得迭代器首地址
    int key =iter_map->first;             //取得eky
    string value =iter_map->second;       //取得value
    map1.erase(iter_map);                  //删除迭代器数据
   map1.erase(3);                         //根据key删除value
   map1.size();                       //元素个数
   map1.empty();                      //判断空
  map1.clear();                      //清空所有元素
 //3.遍历
    for(map<int,string>::iteratoriter = map1.begin();iter!=map1.end();iter++)
    {
       int keyk= iter->first;
       stringvaluev = iter->second;
    }
四、C++ map使用实例
（1）将mymap中itemstruct   的a大于100的项删除  
struct   itemstruct 
  { 
            int   a; 
            char   b[20]; 
  }; 
  map<string,   itemstruct   >     mymap. 
实例1：
```cpp
#include   <iostream> 
  #include   <ctime> 
  #include   <map> 
  using   namespace   std; 
  typedef   struct   itemstruct 
  { 
            int   a; 
            char   b[20]; 
  }itemS; 
  itemS   s[4]   =   {{102,"what"}, 
                          {33,   "hello"}, 
                          {198,"world"}, 
                          {45,   "c++"} 
                      };; 
  
  
  int     main() 
  { 
            map<string,   itemS>     mymap; 
            string   str[4]   =   {"1st","2nd","3rd","4th"}; 
            for(int   i   =   0;   i<4;   i++) 
            { 
                 mymap.insert(make_pair(str[i],   s[i])); 
            } 
            
            map<string,itemS>::iterator   it; 
            for(it=mymap.begin();   it!=mymap.end(); it++) 
            { 
               if(it->second.a >100){ 
                  i=mymap.erase(it);  ----->正确
    mymap.erase(it);     ----->it失效..
        }
            }
  //first是Key, second是value;
            for(it = mymap.begin();  it!=mymap.end(); it++) 
            { 
                          cout<<it->first<<"   "<<it->second.a<<"   "<<it->second.b<<endl; 
            } 
          system("pause"); 
          return   0; 
  }
```
实例2：
```cpp
#include<map> 
  #include<iterator> 
  #include<string> 
  #include<iostream> 
  #include<cstring> 
  using   namespace   std; 
  struct   itemstruct 
  { 
            int   a; 
            char   b[20]; 
            itemstruct(int   t,char*str) 
    { 
    a=t; 
    strcpy(b,str); 
    } 
  }; 
  int   main() 
  { 
  map<string,itemstruct>mymap; 
  mymap.insert(make_pair("a",itemstruct(10,"hanzhou"))); 
  mymap.insert(make_pair("ab",itemstruct(20,"fuzhou"))); 
  mymap.insert(make_pair("abc",itemstruct(30,"zhengzhou"))); 
  mymap.insert(make_pair("abcd",itemstruct(200,"wuhan"))); 
  mymap.insert(make_pair("abcde",itemstruct(150,"kunming"))); 
  mymap.insert(make_pair("abcdef",itemstruct(50,"xiamen"))); 
  map<string,itemstruct>::iterator   it=mymap.begin(); 
  while(it!=mymap.end()) 
  { 
  if((it->second).a>100)mymap.erase(it++); 
  else   it++; 
  } 
  it=mymap.begin(); 
  while(it!=mymap.end()) 
  { 
  cout<<it->first<<"   "<<(it->second).a<<"   "<<(it->second).b<<endl; 
  it++; 
  } 
  system("PAUSE"); 
  return   0; 
  }
```
实例3：
```cpp
for(map<string,   itemstruct>::iterator   i   =   mymap.begin();   i   !=   mymap.end();) 
  { 
          if(i->second.a   >   100) 
                  i   =   mymap.erase(i); 
          else 
                  ++i; 
  }
```
