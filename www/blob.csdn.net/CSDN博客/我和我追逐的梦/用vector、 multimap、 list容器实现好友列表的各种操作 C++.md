
# 用vector、 multimap、 list容器实现好友列表的各种操作 C++ - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月21日 18:34:41[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1401标签：[vector																](https://so.csdn.net/so/search/s.do?q=vector&t=blog)[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)[iterator																](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)[cmd																](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[pair																](https://so.csdn.net/so/search/s.do?q=pair&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=string&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
**用vector、 multimap、 list容器实现好友列表的各种操作 C++**
**--------------------------------------------------BuddyList.h------------------------------------------------****\#include<map>**
**\#include<string>**
**\#include<list>**
**//using std::multimap;**
**//using std::string;**
**//using std::list;**
**using namespace std;**
**struct Info**
**{**
**string id;**
**string name;**
**string otherInfo;**
**};**
**class BuddyList**
**{**
**public:**
**BuddyList();**
**Info getMyInfo();**
**void setMyInfo(const Info &info);**
**void addBuddy(const Info &friInfo);**
**bool remove(const string &friId);**
**bool isBuddy(const string &friId);**
**list<Info> getBuddies() const;**
**protected:**
**multimap<string,Info>mBuddies;**
**private:**
**Info myInfo;**
**//BuddyList(const BuddyList &src);**
**//BuddyList & operator=(const BuddyList &rhs);**
**};**
**BuddyList::BuddyList(){};**
**Info BuddyList::getMyInfo()**
**{**
**return myInfo;**
**}**
**void BuddyList::setMyInfo(const Info &info)**
**{**
**myInfo = info;**
**}**
**void BuddyList::addBuddy(const Info &friInfo)**
**{**
**if( !isBuddy(friInfo.id))**
**mBuddies.insert(make_pair(myInfo.id,friInfo));**
**}**
**bool BuddyList::remove(const std::string &friId)**
**{**
**if(!isBuddy(friId))**
**return false;**
**multimap<string,Info>::iterator start,end;**
**start = mBuddies.lower_bound(myInfo.id);**
**end = mBuddies.upper_bound(myInfo.id);**
**for(;start != end; ++start)**
**{**
**if(start->second.id == friId)**
**{**
**mBuddies.erase(start);**
**return true;**
**}**
**}**
**}**
**bool BuddyList::isBuddy(const std::string &friId)**
**{**
**multimap<string,Info>::iterator start,end;**
**start = mBuddies.lower_bound(myInfo.id);**
**end = mBuddies.upper_bound(myInfo.id);**
**for(;start != end; ++start)**
**{**
**if(start->second.id == friId)**
**return true;**
**}**
**return false;**
**}**
**list<Info> BuddyList::getBuddies() const**
**{**
**/***
**pair<multimap<string,Info>::const_iterator,**
**multimap<string,Info>::const_iterator> its;**
**its = mBuddies.equal_range(myInfo.id);**
***/**
**list<Info> buddies;**
**multimap<string,Info>::iterator it;**
**for(multimap<string,Info>::const_iterator it = mBuddies.begin(); it != mBuddies.end(); ++it)**
**buddies.push_back(it->second);**
**return buddies;**
**}**

**-------------------------------------------------主函数.cpp----------------------------------------------------****// 好友列表实现.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#include "BuddyList.h"**
**\#include<string>**
**\#include<cstring>**
**\#include<vector>**
**using namespace std;**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**Info info1,info2;**
**vector<BuddyList> usersBudList;**
**cout<<"***********************请选择需要进行的操作******************"<<endl;**
**cout<<"---------------建立所有用户好友列表(C/c)------------------"<<endl;**
**cout<<"-------------------新增用户好友(I/i)----------------------"<<endl;**
**cout<<"---------------查找指定用户所有好友(S/s)------------------"<<endl;**
**cout<<"---------------删除指定用户某位好友(R/r)------------------"<<endl;**
**cout<<"------------------------退出(E/e)-------------------------"<<endl;**
**cout<<endl<<endl<<endl;**
**char cmd;**
**cin>>cmd;**
**while(1)**
**{**
**if(cmd=='C'||cmd=='c')**
**{**
**cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;**
**cout<<"请输入所有用户"<<endl;**
**cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;**
**while(1)**
**{**
**cout<<endl<<endl;**
**cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;**
**cout<<"请输入账号（以0结束）：";**
**cin>>info1.id;**
**if(info1.id=="0")**
**break;**
**cout<<"请输入他的姓名：";**
**cin>>info1.name;**
**cout<<"请输入他的其他的信息：";**
**cin>>info1.otherInfo;**
**BuddyList buddyList;**
**buddyList.setMyInfo(info1);**
**///////////////////////////////////////////////usersBudList.push_back(buddyList);**
**cout<<endl;**
**cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;**
**cout<<"请输入他所有好友的信息"<<endl;**
**while(1)**
**{**
**cout<<"**********************************"<<endl;**
**cout<<"请输入好友账号（以0结束）：";**
**cin>>info2.id;**
**if(info2.id=="0")**
**break;**
**cout<<"请输入他的姓名：";**
**cin>>info2.name;**
**cout<<"请输入他的其他的信息：";**
**cin>>info2.otherInfo;**
**buddyList.addBuddy(info2);**
**}**
**usersBudList.push_back(buddyList);**
**}**
**}**
**else if(cmd == 'I' || cmd=='i')**
**{**
**cout<<endl<<endl;**
**string userId;**
**cout<<"****************************************"<<endl;**
**cout<<"请输入用户账号（以0结束）："<<endl;**
**cin>>userId;**
**if(userId=="0")**
**break;**
**vector<BuddyList>::iterator it1;**
**int i =0;**
**for( it1= usersBudList.begin(); it1 != usersBudList.end(); ++it1,++i)**
**{**
**if(it1->getMyInfo().id == userId)**
**{**
**cout<<endl;**
**cout<<"**********************************"<<endl;**
**cout<<"请输入新增好友的信息"<<endl;**
**Info info1;**
**while(1)**
**{**
**cout<<"**********************************"<<endl;**
**cout<<"请输入好友账号（以0结束）：";**
**cin>>info1.id;**
**if(info1.id=="0")**
**break;**
**cout<<"请输入他的姓名：";**
**cin>>info1.name;**
**cout<<"请输入他的其他的信息：";**
**cin>>info1.otherInfo;**
**BuddyList *pBuddyList = &usersBudList.at(i);**
**//cout<<pBuddyList->getMyInfo().id<<endl;**
**if(pBuddyList->isBuddy(info1.id))**
**{**
**cout<<"他的好友列表中已存在此账号！"<<endl;**
**continue;**
**}**
**pBuddyList->addBuddy(info1);**
**cout<<"添加成功！"<<endl;**
**}**
**break;**
**}**
**}**
**if(it1 == usersBudList.end())**
**cout<<"无此账号的用户！"<<endl;**
**}**
**else if(cmd=='S'||cmd=='s')**
**{**
**cout<<endl<<endl;**
**string userId;**
**list<Info> userList;**
**while(1)**
**{**
**cout<<"****************************************"<<endl;**
**cout<<"请输入用户账号（以0结束）："<<endl;**
**cin>>userId;**
**if(userId=="0")**
**break;**
**vector<BuddyList>::iterator it1;**
**for( it1= usersBudList.begin(); it1 != usersBudList.end(); ++it1)**
**{**
**if(it1->getMyInfo().id == userId)**
**{**
**userList = it1->getBuddies();**
**if(userList.size()==0)**
**{**
**cout<<"此账号用户的好友列表为空！"<<endl;**
**break;**
**}**
**cout<<"他的好友有："<<endl;**
**for(list<Info>::iterator it2 = userList.begin(); it2 != userList.end(); ++it2)**
**{**
**cout<<"账号："<<it2->id<<"  姓名："<<it2->name<<"  其他信息："<<it2->otherInfo<<endl;**
**}**
**break;**
**}**
**}**
**if(it1 == usersBudList.end())**
**cout<<"无此账号的用户！"<<endl;**
**}**
**}**
**else if(cmd=='R'||cmd=='r')**
**{**
**cout<<endl<<endl;**
**string userId;**
**string friId;**
**list<Info> userList;**
**while(1)**
**{**
**cout<<"****************************************"<<endl;**
**cout<<"请输入用户账号（以0结束）："<<endl;**
**cin>>userId;**
**if(userId=="0")**
**break;**
**vector<BuddyList>::iterator it1 = usersBudList.begin();**
**for(; it1 != usersBudList.end(); ++it1)**
**{**
**if(it1->getMyInfo().id == userId)**
**{**
**cout<<endl;**
**cout<<"****************************************"<<endl;**
**cout<<"请输入需要删除的好友的账号："<<endl;**
**cin>>friId;**
**bool flag = it1->remove(friId);**
**if(flag)**
**cout<<"删除成功！"<<endl;**
**else**
**cout<<"他的好友中没有此账号！"<<endl;**
**break;**
**}**
**}**
**if(it1 == usersBudList.end())**
**cout<<"无此账号的用户！"<<endl;**
**}**
**}**
**else if(cmd=='E'||cmd=='e')**
**{**
**break;**
**}**
**else**
**{**
**cout<<"输入命令不正确！请重新输入！"<<endl;**
**}**
**cout<<"*************************************************************"<<endl;**
**cout<<"***********************请选择需要进行的操作******************"<<endl;**
**cout<<"*************************************************************"<<endl;**
**cout<<endl<<endl<<endl;**
**cin>>cmd;**
**}**
**system("pause");**
**return 0;**
**}**
**----------------------------------------------程序测试---------------------------------------------------------**
*************************请选择需要进行的操作********************
**---------------建立所有用户好友列表(C/c)------------------**
**-------------------新增用户好友(I/i)----------------------**
**---------------查找指定用户所有好友(S/s)------------------**
**---------------删除指定用户某位好友(R/r)------------------**
**------------------------退出(E/e)-------------------------**
**c**
**%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%**
**请输入所有用户**
**%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%**
**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**
**请输入账号（以0结束）：2008550820**
**请输入他的姓名：heyong**
**请输入他的其他的信息：cs**
**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**
**请输入他所有好友的信息**
**************************************
**请输入好友账号（以0结束）：0894042210**
**请输入他的姓名：luoyao**
**请输入他的其他的信息：ac**
**************************************
**请输入好友账号（以0结束）：2008550802**
**请输入他的姓名：zhouqian**
**请输入他的其他的信息：cs**
**************************************
**请输入好友账号（以0结束）：2008552980**
**请输入他的姓名：lina**
**请输入他的其他的信息：sw**
**************************************
**请输入好友账号（以0结束）：0**
**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**
**请输入账号（以0结束）：0894042210**
**请输入他的姓名：luoyao**
**请输入他的其他的信息：ac**
**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**
**请输入他所有好友的信息**
**************************************
**请输入好友账号（以0结束）：0897508652**
**请输入他的姓名：liuhuan**
**请输入他的其他的信息：tg**
**************************************
**请输入好友账号（以0结束）：0812345678**
**请输入他的姓名：lijun**
**请输入他的其他的信息：tf**
**************************************
**请输入好友账号（以0结束）：0**
**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&**
**请输入账号（以0结束）：0**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**i**
********************************************
**请输入用户账号（以0结束）：**
**0894042210**
**************************************
**请输入新增好友的信息**
**************************************
**请输入好友账号（以0结束）：2008550819**
**请输入他的姓名：lixun**
**请输入他的其他的信息：cs**
**0894042210**
**添加成功！**
**************************************
**请输入好友账号（以0结束）：2008550820**
**请输入他的姓名：heyong**
**请输入他的其他的信息：cs**
**0894042210**
**添加成功！**
**************************************
**请输入好友账号（以0结束）：2008550820**
**请输入他的姓名：heyong**
**请输入他的其他的信息：cs**
**0894042210**
**他的好友列表中已存在此账号！**
**************************************
**请输入好友账号（以0结束）：0**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**s**
********************************************
**请输入用户账号（以0结束）：**
**2008550820**
**他的好友有：**
**账号：0894042210  姓名：luoyao  其他信息：ac**
**账号：2008550802  姓名：zhouqian  其他信息：cs**
**账号：2008552980  姓名：lina  其他信息：sw**
********************************************
**请输入用户账号（以0结束）：**
**0894042210**
**他的好友有：**
**账号：0897508652  姓名：liuhuan  其他信息：tg**
**账号：08123456  姓名：lijun  其他信息：tf**
**账号：2008550819  姓名：lixun  其他信息：cs**
**账号：2008550820  姓名：heyong  其他信息：cs**
********************************************
**请输入用户账号（以0结束）：**
**2008550819**
**无此账号的用户！**
********************************************
**请输入用户账号（以0结束）：**
**0**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**r**
********************************************
**请输入用户账号（以0结束）：**
**0894042210**
********************************************
**请输入需要删除的好友的账号：**
**08123456**
**删除成功！**
********************************************
**请输入用户账号（以0结束）：**
**0**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**s**
********************************************
**请输入用户账号（以0结束）：**
**0894042210**
**他的好友有：**
**账号：0897508652  姓名：liuhuan  其他信息：tg**
**账号：2008550819  姓名：lixun  其他信息：cs**
**账号：2008550820  姓名：heyong  其他信息：cs**
********************************************
**请输入用户账号（以0结束）：**
**0**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**w**
**输入命令不正确！请重新输入！**
*****************************************************************
*************************请选择需要进行的操作********************
*****************************************************************
**e**
**请按任意键继续. . .**


