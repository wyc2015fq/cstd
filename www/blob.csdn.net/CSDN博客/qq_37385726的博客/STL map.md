# STL  map - qq_37385726的博客 - CSDN博客





2018年07月30日 10:09:35[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：28标签：[STL																[map](https://so.csdn.net/so/search/s.do?q=map&t=blog)](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[STL](https://blog.csdn.net/qq_37385726/article/category/7888287)








## **STL map可以直接调函数就实现的功能是：**

**1.插入元素 insert(pair<,>()),insert(map<,>::value_type()),数组方式**

**2.查找元素 find**

**3.迭代器 map<,>::iterator iter;**

**4.删除元素  先find,后erase**

**5.遍历元素  迭代器实现，数组实现**

**6.清空**



```cpp
​
​
​
#include <iostream>  
using namespace std;  
#include <map>  
#include <string>  

  
int main(void)  
{  
    map<int, string> mapStudent;  

//****************map插入元素********************//  
  	//pair<int, string>()方式插入元素，不支持覆盖 
    mapStudent.insert(pair<int, string>(1, "student_one"));  
    mapStudent.insert(pair<int, string>(2, "student_two"));  
    mapStudent.insert(pair<int, string>(3, "student_three"));  
    
    //map<int, string>::value_type()方式插入元素，不支持覆盖 
    mapStudent.insert(map<int, string>::value_type (4, "student_four")); 
    mapStudent.insert(map<int, string>::value_type (5, "student_five")); 
    mapStudent.insert(map<int, string>::value_type (6, "student_six")); 
    
    //mapStudent[i]="string",数组方式插入元素，支持覆盖 
	mapStudent[7]="student_seven"; 
  
  
  
  
//****************map遍历元素********************//  
  	map<int,string>::iterator iter;  //迭代器是指针   
  	
  	//正向遍历
  	cout<<"正向遍历"<<endl;
	for(iter=mapStudent.begin();iter!=mapStudent.end();iter++)
	{
		cout<<iter->first<<" "<<iter->second<<endl;
	} 
  	
  	cout<<endl<<endl;
  	
  	map<int, string>::reverse_iterator riter;  //迭代器是指针 
  	//反向遍历 
  	cout<<"反向遍历"<<endl;
    for(riter = mapStudent.rbegin(); riter != mapStudent.rend(); riter++)  
  	{
        cout<<riter->first<<"  "<<riter->second<<endl;  
	}  
	
	cout<<endl<<endl; 
//****************map查找元素********************//

	//使用find()函数结合迭代器来查找元素 
	cout<<"查找key为4的数据项"<<endl;
	map<int,string>::iterator iterfind;
	iterfind=mapStudent.find(4);
	if(iterfind!=mapStudent.end())
	{
		cout<<"res:  "<<iterfind->first<<"  "<<iterfind->second<<endl;	
	}
	
	cout<<endl<<endl;

//****************map删除元素********************// 
	
	//erase()函数结合迭代器删除元素
	cout<<"删除key为1的数据项"<<endl;
	map<int,string>::iterator itererase;
	itererase=mapStudent.find(1);
	if(itererase!=mapStudent.end())
	{
		mapStudent.erase(itererase);
	}
	//正向遍历
	for(iter=mapStudent.begin();iter!=mapStudent.end();iter++)
	{
		cout<<iter->first<<" "<<iter->second<<endl;
	} 
  	
  	cout<<endl<<endl;
	return 0;
}

​

​

​
```






