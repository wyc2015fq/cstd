# 使用MSXML解析XML文件  - ljx0305的专栏 - CSDN博客
2008年03月11日 21:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：764
使用MSXML解析XML文件
本文以如下的XML文件为例，读写这个XML文件ASKLog.xml
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<?xml version="1.0" encoding="utf-8"?>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<Log>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<LogNode>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<RecordTime>2007-11-2815:23:14</RecordTime>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<RecordState>Running</RecordState>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<RecordMessage>Finished level1 loop</RecordMessage>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</LogNode>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</Log>
(1)需要使用的头文件，dll和namespace(2)在_tmain函数中实现对XML文件的读写
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#import <msxml.dll>//利用MSXML解析XML文件以及生成XML文件
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#include "msxml.h"
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)usingnamespace MSXML;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)usingnamespace std;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)int(3)读取XML文档(4)往XML文件写入数据引用: [http://www.cnblogs.com/xulei/archive/2007/12/11/975983.html](http://www.cnblogs.com/xulei/archive/2007/12/11/975983.html) _tmain(int argc, _TCHAR* argv[])
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    MSXML::IXMLDOMDocumentPtr pXmlDoc;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    MSXML::IXMLDOMElementPtr pXmlRoot;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    CoInitialize(NULL);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//创建DOMDocument对象
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    HRESULT hr = pXmlDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(!SUCCEEDED(hr)) 
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){         
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)int _tmain(int argc, _TCHAR* argv[])
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif).
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//在上面代码的基础上加入如下代码
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) argv[1] = _T("F://2007//11//28//ASKLog.xml");  //XML文件的地址，存放于命令行参数
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->load(argv[1]); //加载XML文件
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlRoot = pXmlDoc->documentElement; //获取root节点的指针，也就是xml文件中的Log节点
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) MSXML::IXMLDOMNodeListPtr pNodeList = pXmlRoot->childNodes;       //获取指向Root节点子节点的指针数组，也就是获得
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//指向所有LogNode节点的指针
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) MSXML::IXMLDOMNodeListPtr pNextNodeList = pNodeList->item[0]->childNodes; //获取第一个LogNode节点的所有子节点的
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//指针数组，也就是RecordTime，RecordState和RecordMessage
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//节点的指针
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) cout<< pNextNodeList->item[0]->text;  //这样就能输出第一个节点：RecordTime的值了(字符串)类型 
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)int _tmain(int argc, _TCHAR* argv[])
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif)![](http://www.cnblogs.com/Images/dot.gif).
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//在上面(1)代码的基础上加入如下代码
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) MSXML::IXMLDOMElementPtr pXmlNewNode;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_createElement((_bstr_t)(char*)"LogNode", &pXmlNewNode);  //创建节点LogNode，并获得指向它的指针pXmlNewNode
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlRoot->raw_appendChild(pXmlNewNode,NULL);   //将创建的节点挂在Root节点的下面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//这里需要注意的是，假如ASKLog.xml文件不存在或为空，需要我们创建，需要使用如下代码
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)----------------------------------------------------------------------------------------------------------
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_createElement((_bstr_t)(char*)"Log", &pXmlRoot);        //创建Root节点Log，并获得指向它的指针pXmlRoot
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_appendChild(pXmlRoot, NULL);                  //将pXmlRoot挂在pXmlDoc下面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)----------------------------------------------------------------------------------------------------------
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) MSXML::IXMLDOMElementPtr pChildNode;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_createElement((_bstr_t)(char*)"RecordTime", &pChildNode); //创建节点RecordTime，并获得指向它的指针pChildNode
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pChildNode->Puttext(_T("2007-11-28 16:29:30"));    //设置节点的值，使用Puttext方法
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlNewNode->raw_appendChild(pChildNode,NULL);  //将创建的节点挂在新的上层节点pXmlNewNode下面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_createElement((_bstr_t)(char*)"RecordState", &pChildNode);  //创建节点RecordState，并获得指向它的指针pChildNode
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pChildNode->Puttext("Running");      //设置节点的值，使用Puttext方法
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlNewNode->raw_appendChild(pChildNode,NULL);//将创建的节点挂在新的上层节点pXmlNewNode下面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->raw_createElement((_bstr_t)(char*)"RecordMessage", &pChildNode);  //创建节点RecordMessage，并获得指向它的指针pChildNode
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pChildNode->Puttext("Finished level2 loop");   //设置节点的值，使用Puttext方法
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlNewNode->raw_appendChild(pChildNode,NULL);//将创建的节点挂在新的上层节点pXmlNewNode下面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif) pXmlDoc->save(argv[1]); //存储XML文件，如果存在就覆盖，不存在就创建
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
