
# C++操作XML之创建（1） - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月23日 16:03:58[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3797


**C++操作XML之创建（1）**
**-------------------------------------------XMLElement.h--------------------------------------------------**
**\#include<string>**
**\#include<vector>**
**\#include<map>**
**\#include<iostream>**
**using namespace std;**
**class XMLElement**
**{**
**public:**
**XMLElement();**
**void setElementName(const string &inName);**
**void setAttribute(const string &inAttributeName,const string &inAttributeValue);**
**void addSubElement(const XMLElement *inElement);**
**void setTextNode(const string &inValue);**
**string getMyInfo() const;**
**string getMySubElemInfo() const;**
**friend ostream& operator<<(ostream &outStream,const XMLElement &inElem);**
**protected:**
**void writeToStream(ostream& outStream,int inIndentLevel=0) const;**
**void indentStream(ostream& outStream,int inIndentLevel) const;**
**private:**
**string mElementName;**
**map<string,string> mAttributes;**
**vector<const XMLElement*> mSubElements;**
**string mTextNode;**
**};**
**---------------------------------------------XMLElement.cpp----------------------------------------------**
**\#include "stdafx.h"**
**\#include "XMLElement.h"**
**using namespace std;**
**XMLElement::XMLElement():mElementName("unnamed")**
**{**
**}**
**void XMLElement::setElementName(const std::string &inName)**
**{**
**mElementName = inName;**
**}**
**void XMLElement::setAttribute(const std::string &inAttributeName, const std::string &inAttributeValue)**
**{**
**mAttributes[inAttributeName] = inAttributeValue;**
**}**
**void XMLElement::addSubElement(const XMLElement *inElement)**
**{**
**mSubElements.push_back(inElement);**
**}**
**void XMLElement::setTextNode(const std::string &inValue)**
**{**
**mTextNode = inValue;**
**}**
**string XMLElement::getMyInfo() const**
**{**
**string info = mElementName + " " ;**
**for(map<string,string>::const_iterator it1 = mAttributes.begin(); it1 != mAttributes.end(); ++it1)**
**info += it1->first +" " + it1->second +" ";**
**info += mTextNode;**
**return info;**
**}**
**string XMLElement::getMySubElemInfo() const**
**{**
**string subInfo;**
**for(vector< const XMLElement* >::const_iterator it2 = mSubElements.begin(); it2 != mSubElements.end(); ++it2)**
**{**
**subInfo += (*it2)->getMyInfo() + "\n";**
**}**
**return subInfo;**
**}**
**ostream& operator<<(ostream& outStream,const XMLElement& inElem)**
**{**
**inElem.writeToStream(outStream);**
**return (outStream);**
**}**
**void XMLElement::writeToStream(std::ostream &outStream, int inIndentLevel) const**
**{**
**indentStream(outStream,inIndentLevel);**
**outStream<<"<"<<mElementName;**
**for(map<string,string>::const_iterator it1 = mAttributes.begin();it1 !=mAttributes.end(); ++it1)**
**{**
**outStream<<" "<<it1->first<<"=\""<<it1->second<<"\"";**
**}**
**outStream<<">";**
**if(mTextNode !="")**
**outStream<<mTextNode;**
**else**
**{**
**outStream<<endl;**
**for(vector< const XMLElement* >::const_iterator it2 = mSubElements.begin(); it2 != mSubElements.end(); ++it2)**
**(*it2)->writeToStream(outStream,inIndentLevel+1);**
**//(*it2)->writeToStream(outStream,inIndentLevel+1);**
**indentStream(outStream,inIndentLevel);**
**}**
**outStream<<"</"<<mElementName<<">"<<endl;**
**}**
**void XMLElement::indentStream(std::ostream &outStream, int inIndentLevel) const**
**{**
**for(int i=0;i<inIndentLevel;i++)**
**outStream<< '\t';**
**}**
**-------------------------------------------------主函数.cpp--------------------------------------------------**
**// C++andXML.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<string>**
**//\#include<iostream>**
**\#include "XMLElement.h"**
**//using namespace std;**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**XMLElement myXMLElement;**
**myXMLElement.setElementName("我的通讯录");**
**cout<<"请输入自己的名字：";**
**string myName;**
**getline(cin,myName);**
**cout<<"请输入自己的电话号码：";**
**string myPhone;**
**getline(cin,myPhone);**
**myXMLElement.setAttribute(myName,myPhone);**
**string friName;**
**string friPhone;**
**string friAttri;**
**int i = 1;**
**while(1)**
**{**
**cout<<"********************"<<endl;**
**XMLElement *friXMLElement = new XMLElement();**
**cout<<"请输入好友"<<i<<"的名字(以0结束)：";**
**getline(cin,friName);**
**if(friName=="0")**
**break;**
**cout<<"请输入他（她）的电话号码：";**
**getline(cin,friPhone);**
**cout<<"属性为：";**
**getline(cin,friAttri);**
**string name ="好友";**
**name += (char)(i+'0');**
**friXMLElement->setElementName(name);**
**friXMLElement->setAttribute(friName,friPhone);**
**friXMLElement->setTextNode(friAttri);**
**//cout<<friXMLElement->getMyInfo()<<endl;**
**myXMLElement.addSubElement(friXMLElement);**
**i++;**
**}**
**cout<<"我的通讯录为："<<endl;**
**//cout<<myXMLElement.getMyInfo()<<endl;**
**//cout<<myXMLElement.getMySubElemInfo()<<endl;**
**cout<<myXMLElement;**
**cout<<endl;**
**system("pause");**
**return 0;**
**}**
**------------------------------------------------程序测试-----------------------------------------------------**
**请输入自己的名字：heyong**
**请输入自己的电话号码：1455**
************************
**请输入好友1的名字(以0结束)：liudehua**
**请输入他（她）的电话号码：1345**
**属性为：friend**
************************
**请输入好友2的名字(以0结束)：zhangxueyou**
**请输入他（她）的电话号码：1873**
**属性为：friend**
************************
**请输入好友3的名字(以0结束)：liming**
**请输入他（她）的电话号码：1890**
**属性为：friend**
************************
**请输入好友4的名字(以0结束)：guofucheng**
**请输入他（她）的电话号码：1876**
**属性为：friend**
************************
**请输入好友5的名字(以0结束)：0**
**我的通讯录为：**
**<我的通讯录 heyong="1455">**
**<好友1 liudehua="1345">friend</好友1>**
**<好友2 zhangxueyou="1873">friend</好友2>**
**<好友3 liming="1890">friend</好友3>**
**<好友4 guofucheng="1876">friend</好友4>**
**</我的通讯录>**
**请按任意键继续. . .**




