# STL之for_each, cout, cout_if, find, find_if - bigfacesafdasgfewgf - CSDN博客





2014年10月19日 08:23:23[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：565
个人分类：[C/C++](https://blog.csdn.net/puqutogether/article/category/2595123)









# STL之for_each, cout, cout_if, find_if

（需要添加#include<algorithm>头文件）




**1. for_each**

    在使用STL容器的时候，我们一般使用iterator迭代增量来遍历容器，但是每次使用iterator的时候，都需要for语句进行初始化，自增或者自减的运算操作，比较繁琐，而且也增加了代码量。

    STL中有一种方法可以简化这个过程，就是for_each. 使用的形式如下： 

             for_each(Iter.begin(), Iter.end(), print); 

我们可以发现，for_each中需要指定起始和终止，以及每次的操作。这之间都是用“，”连接的，而且print的操作也是可以变化的，可是是修改、赋值等操作。注意：操作的作用范围只能是[Iter.begin(), Iter.end())， 是不包括Iter.end()。我们可以通过下例来学会使用for_each: 


```cpp
#include<iostream>
#include<list>
#include<iterator>
#include<algorithm>
#include<string>
using namespace std; 

void print(string str)
{
	cout<<str; 
}

int main()
{
	list<string> words; 
	list<string> ::iterator wordsIterator; 

	words.push_front("we"); 
	words.push_back(" are"); 
	words.push_back(" one!"); 
	words.push_back("%"); 
	words.pop_back(); 

	for(wordsIterator=words.begin(); wordsIterator!=words.end(); ++wordsIterator) 
		cout<<*wordsIterator; 
	cout<<endl; 

	for_each(words.begin(), words.end(), print); 
	cout<<endl; 

	return 0; 

}
```




输出：

![](https://img-blog.csdn.net/20141019083056248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






**2. count和count_if**

    count和count_if也是STL中专门为容器设计的计数语句，先介绍简单的count. 

    count的基本使用形式为：

                       NumberOf100Scores=count (Scores.begin(), Scores.end(), 100);





```cpp
#include<iostream>
#include<list>
#include<algorithm>
using namespace std; 

int main () 
{
	list<int> Scores;

	Scores.push_back(100); Scores.push_back(80);
	Scores.push_back(45); Scores.push_back(75);
	Scores.push_back(99); Scores.push_back(100);

	int NumberOf100Scores(0); 
	NumberOf100Scores=count (Scores.begin(), Scores.end(), 100);

	cout << "There were " << NumberOf100Scores << " scores of 100" << endl;
	return 0; 
}
```


输出：

![](https://img-blog.csdn.net/20141019084019609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








    count_if是返回区间中满足指定条件的元素数目

输入：迭代器的起始位置；

           迭代器的终止位置后面的；

           需要满足的条件，称为谓词；

输出：计数个数。




显然，对于count_if而言，谓词条件是至关重要的，这个是用户自定义的函数对象。返回值是true或者是false。

如下例：


```cpp
#include <vector>
#include <algorithm>
#include <iostream>

bool greater10(int value)  //需要满足的条件，如果值大于10，那么就返回真
{
    return value >10;
}

int main()
{
    using namespace std;
    vector<int> v1;
    vector<int>::iterator Iter;

    v1.push_back(10);  //vector和list的区别之一
    v1.push_back(20);
    v1.push_back(10);
    v1.push_back(40);
    v1.push_back(10);

    cout << "v1 : ";
    for (Iter = v1.begin(); Iter != v1.end(); Iter++)
       cout << *Iter << " ";
    cout << endl;


    int  result1 = count_if(v1.begin(), v1.end(), greater10);  //count_if算法返回使谓词函数返回条件成立的元素个数
    cout << "The number of elements in v1 greater than 10 is: "
         << result1 << "." << endl;

    return 0;
}
```


输出：

![](https://img-blog.csdn.net/20141019085318038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








3. find和find_if

    find返回的是一个容器中值和被查找数相等的第一个数位置，没有返回end；

find_if返回的是一个容器中值满足条件的第一个数的位置，没有返回end；

使用形式如下：


```cpp
pos = find(intVec.begin(),intVec.end(),12);
pos = find_if(intVec.begin(),intVec.end(), not1(bind2nd(modulus<int>(),3)));
```


这样我们就会针对STL中的容器，使用上面几个方便的语句了。

















