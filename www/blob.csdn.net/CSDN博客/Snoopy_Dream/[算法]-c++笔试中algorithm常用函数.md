# [算法] - c++笔试中algorithm常用函数 - Snoopy_Dream - CSDN博客





2019年04月09日 18:02:11[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：457








#include<algorithm>

用#include<bits/stdc++.h>万能头文件好了

常用功能：交换swap，逆序reverse ，旋转rotate，排序sort，排列组合permutation，查找find，划分（partition）去重set vec（不是alg的），统计count，初始化fill（不是alg的）

**目录**

[erase函数 左闭右开：](#erase%E5%87%BD%E6%95%B0%C2%A0%E5%B7%A6%E9%97%AD%E5%8F%B3%E5%BC%80%EF%BC%9A)

[char 变成 int  ('10'-'0')](#char%20%E5%8F%98%E6%88%90%20int%C2%A0%20(' rel=)

[1. 交换两个元素 swap()](#1.%20%E4%BA%A4%E6%8D%A2%E4%B8%A4%E4%B8%AA%E5%85%83%E7%B4%A0%20swap())

[2. 反转元素的次序 reverse()](#2.%20%E5%8F%8D%E8%BD%AC%E5%85%83%E7%B4%A0%E7%9A%84%E6%AC%A1%E5%BA%8F%20reverse())

[2. rotate/rotate_copy旋转 rotate_copy（input.first(),middle,input.last()，res.first()）](#2.%20rotate%2Frotate_copy%E6%97%8B%E8%BD%AC%20rotate_copy%EF%BC%88input.first()%2Cmiddle%2Cinput.last()%EF%BC%8Cres.first()%EF%BC%89)

[3. sort(str.begin(),str.end(),cmp);](#3.%20%E4%BB%A5%E5%BE%88%E5%A5%BD%E7%9A%84%E5%B9%B3%E5%9D%87%E6%95%88%E7%8E%87%E6%8E%92%E5%BA%8F%20sort())

[4. 按字典序的上/下一个排列 next_permutation(start，end，cmp)](#4.%20%E6%8C%89%E5%AD%97%E5%85%B8%E5%BA%8F%E7%9A%84%E4%B8%8A%2F%E4%B8%8B%E4%B8%80%E4%B8%AA%E6%8E%92%E5%88%97%20next_permutation(start%EF%BC%8Cend%EF%BC%8Ccmp))

[4. 字典序lexicographical_compare（first.begin(),first.end(),last.begin(),last.end(),cmp）默认ascii码从小到大，默认前小，返回true](#4.%20%E5%AD%97%E5%85%B8%E5%BA%8Flexicographical_compare%EF%BC%88first.begin()%2Cfirst.end()%2Clast.begin()%2Clast.end()%2Ccmp%EF%BC%89%E9%BB%98%E8%AE%A4ascii%E7%A0%81%E4%BB%8E%E5%B0%8F%E5%88%B0%E5%A4%A7%EF%BC%8C%E9%BB%98%E8%AE%A4%E5%89%8D%E5%B0%8F%EF%BC%8C%E8%BF%94%E5%9B%9Etrue)

[字符串拼接 9后面跟着n个0 (直接+=)](#%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%8B%BC%E6%8E%A5%209%E5%90%8E%E9%9D%A2%E8%B7%9F%E7%9D%80n%E4%B8%AA0%20(%E7%9B%B4%E6%8E%A5%2B%3D))

[5. 查找find ，find_first_of ， find_if ， adjacent_find](#5.%C2%A0%E6%9F%A5%E6%89%BEfind%20%EF%BC%8Cfind_first_of%20%EF%BC%8C%20find_if%20%EF%BC%8C%20adjacent_find)

[6. 前后划分partition/stable_partition（如：奇数在前，偶数在后。前后的这种划分）](#6.%20%E5%89%8D%E5%90%8E%E5%88%92%E5%88%86partition%2Fstable_partition%EF%BC%88%E5%A6%82%EF%BC%9A%E5%A5%87%E6%95%B0%E5%9C%A8%E5%89%8D%EF%BC%8C%E5%81%B6%E6%95%B0%E5%9C%A8%E5%90%8E%E3%80%82%E5%89%8D%E5%90%8E%E7%9A%84%E8%BF%99%E7%A7%8D%E5%88%92%E5%88%86%EF%BC%89)

[来一道今年前些天华为的真题试试威力（字符串输入合法非法去重左移外排）](#%E6%9D%A5%E4%B8%80%E9%81%93%E4%BB%8A%E5%B9%B4%E5%89%8D%E4%BA%9B%E5%A4%A9%E5%8D%8E%E4%B8%BA%E7%9A%84%E7%9C%9F%E9%A2%98%E8%AF%95%E8%AF%95%E5%A8%81%E5%8A%9B%EF%BC%88%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%BE%93%E5%85%A5%E5%90%88%E6%B3%95%E9%9D%9E%E6%B3%95%E5%8E%BB%E9%87%8D%E5%B7%A6%E7%A7%BB%E5%A4%96%E6%8E%92%EF%BC%89)

### erase函数 **左闭右开**：

（1）erase(pos,n); 删除从pos开始的n个字符，比如erase(0,1)就是删除第一个字符 （2）erase(position);删除position处的一个字符(position是个string类型的迭代器) （3）erase(first,last);删除从first到last之间的字符（first和last都是迭代器）** 左闭右开**

### **char 变成 int  ('10'-'0')**

'10'->10   '10'-'0'



### 1. 交换两个元素 swap()

```
int x = 10, y = 20;
swap(x, y);
```

### 2. 反转元素的次序 reverse()
- **全部逆序**

```
string str("hello");
//如何才能只反转ello，即变成holle？如下
reverse(str.begin()+1,str.end());
```
- **部分逆序，实现左移n为字符串的功能**

如：abcdef,左移2，是cdefab

```cpp
reverse(str.begin(),str.begin()+2);

reverse(str.begin()+2,str.end());

reverse(str.begin(),str.end());
```

### 2. rotate/rotate_copy旋转 rotate_copy（input.`first(),middle,`input.`last()，res.first()`）

其实和上面的用reverse实现的左移的功能是一样的。

```
template <class ForwardIterator>
  void rotate (ForwardIterator first, ForwardIterator middle,
               ForwardIterator last)
{
  ForwardIterator next = middle;
  while (first!=next)
  {
    swap (*first++,*next++);
    if (next==last) next=middle;
    else if (first==middle) middle=next;
  }
}
```

```cpp
int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

  std::rotate(myvector.begin(),myvector.begin()+3,myvector.end());
                                                  // 4 5 6 7 8 9 1 2 3
  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```



```
template <class ForwardIterator, class OutputIterator>
  OutputIterator rotate_copy (ForwardIterator first, ForwardIterator middle,
                              ForwardIterator last, OutputIterator result)
{
  result=std::copy (middle,last,result);
  return std::copy (first,middle,result);
}
```

```cpp
int main () {
  int myints[] = {10,20,30,40,50,60,70};

  std::vector<int> myvector (7);

  std::rotate_copy(myints,myints+3,myints+7,myvector.begin());

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
`myvector contains: 40 50 60 70 10 20 30`
### 3. sort(str.begin(),str.end(),cmp);

是一种根据实际数据智能选择排序算法的函数，sort()会将区间**[beg,end)**内的元素排序，默认升序
`sort(v.begin(), v.end()); //实现排序，正序排列`
```
bool cmp(int a,int b)
{
    return a>b;//降序排列
}
int array[5]={1,2,3,4,5};
sort(array,array+4,cmp);//通过自定义cmp函数将其改为降序排序
```

输出为：4 3 2 1 5
- **字符串内部排序**

sort(str.begin(),str.end());
- **字符串数组排序，按照字典序排序，不用管长度**

先放到vector<string>vec当中

然后sort(vec.begin(),vec.end());
- **复杂排序**

题目大致是：输入若干行由0和1组成的串，将这些串排序输出。

排序规则是：首先按长度排序，长度相同时，按1 的个数多少进行排序，1 的个数相同时再按ASCII 码值排序。

```cpp
bool cmp(string s1, string s2)
{
    int t1 = count(str1.begin(),str1.end(), '1');
    int t2 = count(str2.begin(),str2.end(), '1');
           
    if(s1.length() !=s2.length()) 
    { 
        return s1.length() <s2.length(); 
    }
    return (t1 != t2 ? t1 < t2: s1 < s2);
}
 
int main()
{
    ……
     sort(s.begin(), s.end(), cmp);
    ……
    return 0;
}
```
- **(2)其他排序方式的实现     **

     对于非增序排列，若要实现，需重载内置的比较函数cmp，下面以减序为例，编写比较函数有两种方法。

     ①如果元素不是结构体，那么，可以编写比较函数。

```cpp
struct cmp
{
   bool operator()(const int &a,const int &b)
    {
           return a>b;
    }
};
```

```cpp
int main()
{
   map<char, int, cmp> m;   //以键char的值进行降序排序
   for(int i=0; i<10; i++)
       m['A'+i] = 10-i;
   map<char, int, cmp>::iterator it;
   for(it=m.begin(); it!=m.end(); it++)
        cout<<(*it).first<<":"<<(*it).second<<endl;
   cout<<endl;
   return 0;
}
```

     ②如果元素是结构体，那么，可以直接把比较函数写在结构体内。

```cpp
struct Info
{
   char letter;
   bool operator < (const Info &a) const
    {
           return a.letter<letter;
    }
};
 
int main()
{
   map<Info, int> m;   //以键Info的score的值进行降序排序
   Info info;
   for(int i=0; i<10; i++)
    {
       info.letter = 'A'+i;
       m[info] = 10-i;
    }
   map<Info, int>::iterator it;
   for(it=m.begin(); it!=m.end(); it++)
       cout<<(*it).first.letter<<":"<<(*it).second<<endl;
   cout<<endl;
   return 0;
}
```

### **4. 按字典序的上/下一个排列 next_permutation(start，end，cmp)**

按字典序的下一个排列**`next_permutation`()**，按字典序的前一个排列 **`prev_permutation`()**，需要**先将其sort升序排列**好，再用这个

```cpp
do//do-while循环和next_permutation、prev_permutation更配喔~
{
    res.push_back(str);
}while(next_permutation(str.begin(),str.end()));
//next_permutation()执行一次 当前的str序列abc 就变成了 升序中的下一个str序列acb
//当 当前的str序列 是 最后一种str序列cba ，那么它就会变成最初的str序列abc
```
- **可以通过next_permutation(start,end)中的参数，实现部分排列**

while(next_permutation(num,num+3))中的3改为2时，输出就变为了：

![](https://img-blog.csdnimg.cn/2019040920410590.png)
- 
通过cmp参数，可实现自定义排列的顺序。


如修改字典序为：'A'<'a'<'B'<'b'<...<'Z'<'z'.请按照这个依次写出排列的内容。

```cpp
//我把cmp中char a看成是前一个内容，b是后一个内容，return a<b，返回ture，前比后小，做升序的
bool cmp(char a,char b) 
{//如果转换成统一的，不相等，那嚒按照转换后的排序就好了
 if(tolower(a)!=tolower(b))//tolower 是将大写字母转化为小写字母.
     return tolower(a)<tolower(b);
 else
     return a<b;//说明是同一个字母的大小写，直接就好，
}
```

### 4. 字典序lexicographical_compare（first.begin(),first.end(),last.begin(),last.end(),cmp）默认ascii码从小到大，默认前小，返回true

![](https://img-blog.csdnimg.cn/20190409231310573.png)

```cpp
// lexicographical_compare example
#include <iostream>     // std::cout, std::boolalpha
#include <algorithm>    // std::lexicographical_compare
#include <cctype>       // std::tolower

// a case-insensitive comparison function:
bool mycomp (char c1, char c2)
{ return std::tolower(c1)<std::tolower(c2); }

int main () {
  char foo[]="Apple";
  char bar[]="apartment";
// 默认ascii的话,第一个A<a,不用看后面的了，第一个已经小了，所以返回true
  std::cout << std::boolalpha;

  std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";

  std::cout << "Using default comparison (operator<): ";
  std::cout << std::lexicographical_compare(foo,foo+5,bar,bar+9);
  std::cout << '\n';

  std::cout << "Using mycomp as comparison object: ";
  std::cout << std::lexicographical_compare(foo,foo+5,bar,bar+9,mycomp);
  std::cout << '\n';

  return 0;
}
```

### 字符串拼接 9后面跟着n个0 (直接+=)

9后面跟着n个0

```cpp
string a="9";
string b="0"
for(int i =0;i<n;i++){
    a+=b;
}
```

### 5. 查找find ，find_first_of ， find_if ， adjacent_find
- **find(vec.begin(),vec.end(),value);**

```cpp
vector<int>::itertation iter = find(vec.begin(),vec.end(),value);
if(iter!=vec.end())
//说明存在，iter就是地址，*iter就是value，
//这个是因为，函数设定成这样了，如果find不存在，自动返回vec.end()的地址。
```

类似地，由于**指针的行为与作用在内置数组上的迭代器**一样，因此也可以使用find来搜索数组：

```cpp
int ia[6] = {27 , 210 , 12 , 47 , 109 , 83};
int search_value = 83;
int *result = find(ia , ia + 6 , search_value);
cout<<"The value "<<search_value<<(result == ia + 6 ? " is not present" : "is present")
```
- **find_first_of (**roster1.begin(),roster1.end()，roster2.begin()，roster​​​​2.end()**) 在**roster1中查找第一个roster2中的元素

roster1可以是list对象，而**roster2则可以是vector对象、 deque对象**或者是其他序列。**只要这两个序列的的元素可使用相等（==）操作符进行比较即可**。如果roster1是list< string>对象，则roster2可以使vector<char*>对象，因为string标准库为string对象与char* 对象定义了相等（==）操作符。

这个算法带有两对迭代器参数来标记两端元素范围：**第一段范围内查找与第二段范围中任意元素匹配的元素**，**然后返回一个迭代器，指向第一个匹配的元素**。如果找不到匹配元素，则返回第一个范围的end迭代器。

假设roster1和roster2是两个存放名字的list对象，可使用find_first_of统计有多少个名字同时出现在这两个列表中：

```cpp
size_t cnt = 0;
list<string>::iterator it = roster1.begin();
 
// look in roster1 for any name also in roster2
while((it = find_first_of(it , roster1.end() , roster2.begin() , roster2.end())) != roster1.end())
{
    ++cnt;
    // we got a match , increment it to look in the rest of roster1
    ++it;
}
cout<<"Found "<<cnt<<"  names on both rosters "<<endl;
```
- **find_if(first,end,pre) pre是一个bool的判断谓词**

```cpp
//谓词判断函数 divbyfive : 判断x是否能5整除

bool divbyfive(int x)

{return x % 5 ? 0 : 1;}
```
- **adjacent_find 用于查找相等或满足条件的邻近元素对**

参考自：[https://blog.csdn.net/hyg0811/article/details/10390591](https://blog.csdn.net/hyg0811/article/details/10390591)

```cpp
bool myfunction (int i, int j) {
  return (i==j);
}

int main () {
  int myints[] = {5,20,5,30,30,20,10,10,20};
  std::vector<int> myvector (myints,myints+8);
  std::vector<int>::iterator it;

  // using default comparison:
  it = std::adjacent_find (myvector.begin(), myvector.end());

  if (it!=myvector.end())
    std::cout << "the first pair of repeated elements are: " << *it << '\n';

  //using predicate comparison:
  it = std::adjacent_find (++it, myvector.end(), myfunction);

  if (it!=myvector.end())
    std::cout << "the second pair of repeated elements are: " << *it << '\n';

  return 0;
}
```

Output:

the first pair of repeated elements are: 30

the second pair of repeated elements are: 10


### 6. 前后划分partition/stable_partition（如：奇数在前，偶数在后。前后的这种划分）
- partition是会打乱原先的顺序，stable_partition则不会
- 和std:: partition不同的是，stable_partition是稳定的，保持原有元素的相对顺序。 

双向迭代器 BidirectionalIterator stable_partition (BidirectionalIterator first, BidirectionalIterator last, UnaryPredicate pred)

**返回值**:指向第二组（**哪些使pred返回false）第一个元素**的迭代器，或者第二组为空，返回***last***. 

如：奇数在前，偶数在后，传统的方法，是定义前后指针，前面的指针如果指向的值是奇数就遍历，偶数就不动（while），后指针，如果指向的是偶数就遍历，奇数就不动，然后再前后交换，使得奇数在前，偶数在后，但是会发生位置的相对变化。利用partition的方法。

```cpp
bool IsOdd (int i) { return (i%2)==1; }

int main () {
  std::vector<int> myvector;

  // set some values:
  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

  std::vector<int>::iterator bound;
  bound = std::stable_partition (myvector.begin(), myvector.end(), IsOdd);

  // print out content:
  std::cout << "odd elements:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=bound; ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "even elements:";
  for (std::vector<int>::iterator it=bound; it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```

```
odd elements: 1 3 5 7 9
even elements: 2 4 6 8
```

### 来一道今年前些天华为的真题试试威力（字符串输入合法非法去重左移外排）

图片摘自网络

![](https://img-blog.csdnimg.cn/20190410153458225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190410153508740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctype.h>
#include<set>
using namespace std;

int main() {
	vector<string>valid_vec;//有效的字符串
	vector<string>invalid_vec;//无效字符串
	char input[100];
	do{
		cin.getline(input, 100);
		bool valid = true;
		int i = 0;
		do{
			int c = input[i];
			if (!isalnum(c)) {
				valid = false;
				break;
			}
			i++;
		} while (input[i] != '\0');
		if (!valid)
			invalid_vec.push_back(input);
		else
			valid_vec.push_back(input);
	} while (input[0] != '\0');
	// 去重valid_vec存储去重后的
	
	set<string>st(valid_vec.begin(), valid_vec.end());
	valid_vec.assign(st.begin(), st.end());
	
	//输出去重后的合法字符串
	int nums_valid = valid_vec.size();
	for (int i = 0; i < nums_valid-1; i++)
	{
		cout << valid_vec[i] << " ";
	}
	cout << valid_vec[nums_valid - 1]<<endl;
	//输出非法字符串
	int nums_invalid = invalid_vec.size();
	for (int i = 0; i < nums_invalid - 1; i++)
	{
		cout << invalid_vec[i] << " ";
	}
	cout << invalid_vec[nums_invalid - 1] << endl;

	//输出左移10位的
	
	nums_valid = valid_vec.size();
	for (int i = 0; i < nums_valid - 1; i++)
	{
		int remove = 10 % valid_vec[i].length();
		rotate(valid_vec[i].begin(), valid_vec[i].begin()+ remove, valid_vec[i].end());
		cout << valid_vec[i] << " ";
	}
	rotate(valid_vec[nums_valid - 1].begin(), valid_vec[nums_valid - 1].begin() + 10 % valid_vec[nums_valid - 1].length(), valid_vec[nums_valid - 1].end());
	cout << valid_vec[nums_valid - 1] << endl;

	//输出左移然后排序之后的
	sort(valid_vec.begin(), valid_vec.end());
	for (int i = 0; i < nums_valid - 1; i++)
	{
		int remove = 10 % valid_vec[i].length();
		rotate(valid_vec[i].begin(), valid_vec[i].begin() + remove, valid_vec[i].end());
		cout << valid_vec[i] << " ";
	}
	rotate(valid_vec[nums_valid - 1].begin(), valid_vec[nums_valid - 1].begin() + 10 % valid_vec[nums_valid - 1].length(), valid_vec[nums_valid - 1].end());
	cout << valid_vec[nums_valid - 1] << endl;
	return 0;
}
```





