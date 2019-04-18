# string类型自带的函数 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:01:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：762








- 可以直接赋值

string s1, s2=" we are one"; 

s1=s2; 

- 可以初始化连续字符

string s1(8, 'A');  //AAAAAAAA


- 可以求长度

s1.length()


- 可以用+实现字符串的联结

s2=s3+' '+s4; 


- insert插入

s3.insert(7, "are one");  //在下标7的地方插入


- replace替换

s3.replace(2, 4, "are");  //把s3中2下标开始长度为4的子串替换为ar


- substr取子串

s1=s3.substr(6, 7);  //把s3中6下标开始长度为7的子串赋值给s1


- find查找

int pos=s3.find(s1);  //找出s3中是否存在s1，若存在返回首字母下标，若不存在返回string::opos


- erase删除

s3.erase(5, 8);  //删除s3中5下标开始长度为8的子串


- 比较大小

bool f=s1>s4;  //比较s1和s4的大小




































