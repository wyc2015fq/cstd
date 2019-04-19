# C++将类模板和函数模板的分文件实现 - fanyun的博客 - CSDN博客
2017年02月26日 21:10:58[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：374
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
例如：
类模板
template<int maxLength>
class DString{
public:
char text[maxLength];
public:
static const int size=maxLength;
friend ofstream& operator <<(ofstreamoutput,const DString& str1);
friend ifstream& operator >>(ofstreaminput,const DString& str1);
};
//其友元函数函数也涉及模板
template<int maxLength>
ofstream& operator <<(ofstreamoutput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
output.put(str1.text[i]);
}
}
template<int maxLength>
ifstream& operator >>(ifstreaminput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
input.get();
}
}
如果要求是
template<int maxLength>
class DString{
public:
char text[maxLength];
public:
static const int size=maxLength;
friend ofstream& operator <<(ofstreamoutput,const DString& str1);
friend ifstream& operator >>(ofstream input,constDString& str1);
};
放在一个.h文件中，而
template<int maxLength>
ofstream& operator <<(ofstreamoutput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
output.put(str1.text[i]);
}
}
template<int maxLength>
ifstream& operator >>(ifstream input,constDString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
input.get();
}
}
放在一个cpp文件中，这种写法是符合C++标准的，但是目前的编译器基本不支持，据说有一个商业编译器支持。
可以参考下boost，一般模板类都是全部写在一个.h文件中。
另外上面的程序有好几个警告。
以下修改过，用g++编译通过。
#include <iostream>
#include <fstream>
using namespace std;
template<int maxLength>
class DString{
public:
char text[maxLength];
public:
static const int size=maxLength;
friend ofstream& operator <<(ofstreamoutput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
output.put(str1.text[i]);
}
return output;
}
friend ifstream& operator >><>(ofstream input,const DString& str1);
}
//其[友元函数](https://www.baidu.com/s?wd=%E5%8F%8B%E5%85%83%E5%87%BD%E6%95%B0&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YdP1IWPvR4ujmsPvRdPjfk0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6snjfzrjb1)函数也涉及模板
/*
template<int maxLength>
ofstream& operator <<(ofstreamoutput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
output.put(str1.text[i]);
}
return output;
}*/
template<int maxLength>
ifstream& operator >>(ifstreaminput,const DString<maxLength>& str1){
for(int i=0;i<maxLength;i++){
input.get();
}
return input;
}
int main(int argc, char *argv[])
{
return 0;
}
