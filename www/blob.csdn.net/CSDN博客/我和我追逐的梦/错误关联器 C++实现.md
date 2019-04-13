
# 错误关联器 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月20日 18:18:33[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：954标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[processing																](https://so.csdn.net/so/search/s.do?q=processing&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[user																](https://so.csdn.net/so/search/s.do?q=user&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=file&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=class&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=class&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=processing&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
-----------------------------------------------ErrorCorrelator.h----------------------------------------------\#include<ostream>
\#include<string>
\#include<queue>
\#include<stdexcept>
//error 类声明
class Error
{
public:
Error(int priority,std::string errMsg):
mPriority(priority),mError(errMsg){}
int getPriority() const {return mPriority;}
std::string getErrorString() const {return mError;}
friend bool operator<(const Error& lhs,const Error &rhs);
friend std::ostream& operator<<(std::ostream &str,const Error &err);
protected:
int mPriority;
std::string mError;
};
//error类容器，返回最高优先级的错误
class ErrorCorrelator
{
public:
ErrorCorrelator(){};
////向优先队列中添加元素
void addError(const Error& error);
//取得最优先元素
Error getError() throw (std::out_of_range);
protected:
std::priority_queue<Error> mErrors;
private:
//prevent assignment and pass-by-refernence
ErrorCorrelator(const ErrorCorrelator &src);
ErrorCorrelator & operator=(const ErrorCorrelator &rhs);
};
//比较符<重载
bool operator<(const Error& lhs,const Error &rhs)
{
return lhs.mPriority<rhs.mPriority;
}
//输出符<<重载
std::ostream& operator<<(std::ostream &str,const Error &err)
{
str << err.mError <<"(priority  "<<err.mPriority<<")"<<std::endl;
return (str);
}
//向优先队列中添加元素
void ErrorCorrelator::addError(const Error &error)
{
mErrors.push(error);
}
//取得最优先元素
Error ErrorCorrelator::getError() throw(std::out_of_range)
{
//判断优先队列是否为空
if(mErrors.empty())
{
throw(std::out_of_range("No elements!"));
}
//获取对头元素
Error top = mErrors.top();
// 弹出队头元素
mErrors.pop();
return top;
}
---------------------------------------------------主函数-------------------------------------------------------
\#include "stdafx.h"
\#include<iostream>
\#include "ErrorCorrelator.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
ErrorCorrelator ec;
ec.addError(Error(3,"Unable to read file!"));
ec.addError(Error(1,"Incorrect entry from user!"));
ec.addError(Error(10,"Unable to allocate memory!"));
while(1)
{
try
{
Error e = ec.getError();
cout << e << endl;
}
catch(out_of_range&)
{
cout<<"Finished processing errors!"<<endl;
break;
}
}
system("pause");
return 0;
}
-----------------------------------------------------程序测试--------------------------------------------------Unable to allocate memory!(priority  10)
Unable to read file!(priority  3)
Incorrect entry from user!(priority  1)
Finished processing errors!
请按任意键继续. . .


