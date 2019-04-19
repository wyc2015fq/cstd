# C/C++解析XML，pugixml库的使用 - u013366022的专栏 - CSDN博客
2017年11月20日 13:47:33[slitaz](https://me.csdn.net/u013366022)阅读数：1053
## 前言：
本文介绍c++编写的xml解析库——pugixml，能解析xml内容，支持xpath解析，同时能够跨linux平台，非常方便。
总结一下使用步骤和简单的使用方法：
使用pugixml库需要三个文件：pugiconfig.h/pugixml.h/pugixml.cpp，可直接从gugixml官网下载，将其加入工程，使用处如包含头文件pugiconfig.h/pugixml.h。
注意：如果想只是包含头文件pugixml.h，则需要在在pugiconfig.hpp中：
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- // Uncomment this to switch to header-only version
-  #define PUGIXML_HEADER_ONLY
-  #include "pugixml.cpp"
只需要将这两行的注释去掉即可。
- 加载xml文件，使用xml_document类的load_file接口:
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- std::strFile = "../test.xml";  
- pugi::xml_document doc;  
- if (!doc.load_file(strFile.c_str()))   
- {  return ;}  
- 加载xml格式的字符串，使用xml_document类的load接口:
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- std::strText = "testing";  
-   pugi::xml_document doc;  
- if (!doc.load(strText.c_str()))   
-   {  return ;}  
- xml节点读取，如：xml文件params.xml：
[html][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <root>
- <!-- 输入参数配置 -->
- <formip="10.2.134.243"port="80"action="sisserver.php">
-        <inputname="data_type"value="POI"/>
-        <inputname="query_type"value="TQUERY"/>
-        <inputname="category"value=""/>
- **</form>**
- <!-- 查询词的返回结果xpath配置 -->
- <xpathpoiroot="//list/poi"idfield="pguid"namefield="name"/>
- <!-- 评分权重配置 r1~r4-期望结果的权重，n1~n10-实际查询结果的排名权重-->
- <weight>
- <!-- 查询词正常得分阀值 -->
- <threshold>3</threshold>
- <!-- 计算分数分布情况的步长值 -->
- <step>0.5</step>
- </weight>
- </root>
    读取代码：
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- std::string strFile = "/bak/workspace/test/src/params.xml";  
-     pugi::xml_document doc;  
- if (!doc.load_file(strFile.c_str()))   
-     {return 0;}  
-     pugi::xml_node form = doc.child("root").child("form");  
-     std::string ip = form.attribute("ip").value();  
-     std::string port = form.attribute("port").value();  
- 
- char cBuf[2083];  
-     sprintf(cBuf, "http://%s:%s/%s?", ip.c_str(), port.c_s());  
-     std::string strTemp(cBuf);  
-     std::string m_strURLBase = strTemp;  
- 
- for (pugi::xml_node input = form.first_child(); input;input = input.next_sibling())  
-     {  
-         std::string strValue = input.attribute("value").value();  
- if (!strValue.empty())   
-         {  
-             std::string strName = input.attribute("name").value();  
-             sprintf(cBuf, "%s=%s&", strName.c_str(), strValue.c_str());  
-             std::string strTemp(cBuf);  
-             m_strURLBase += strTemp;  
-         }  
-     }  
- 
- //读取xpath
-     pugi::xml_node xpath = doc.child("root").child("xpath");  
-     std::string m_strPOIRoot = xpath.attribute("poiroot").value();  
-     std::string m_strPOIID = xpath.attribute("idfield").value();  
- 
- //读取评分权重
-     pugi::xml_node weight = doc.child("root").child("weight");  
- float m_fThred = atof(weight.child_value("threshold"));  
- float m_fStep = atof(weight.child_value("step"));  
- xpath解析，如xml格式的字符串strWebContent:
[html][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <root>
- <listcount="3" time"10">
- <poi>
- <pguid>123</pguid>
- <name>xx1</name>
- </poi>
- <poi>
- <pguid>456</pguid>
- <name>xx2</name>
- </poi>
- <poi>
- <pguid>789</pguid>
- <name>xx3</name>
- </poi>
- </list>
- </root>
  其中，xpath根路径：m_strPOIRoot="//list/poi", 
  需要取值的项：strPOIID=“pguid”，strPOINam=“name”。
  读取代码：
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- //从strWebContent内容中解析出pguid和name
-  pugi::xml_document doc;  
-  pugi::xml_parse_result result = doc.load(strWebContent.c_str());  
- if (!result)  
-  {return -1;}  
-  pugi::xpath_node_set tools = doc.select_nodes(m_strPOIRoot.c_str());  
- for (pugi::xpath_node_set::const_iterator it = tools.begin();   
-      it !=  tools.end(); ++it)  
-  {  
-     pugi::xpath_node node = *it;  
-     string strPOI = node.node().child_value(m_strPOIID.c_str());  
-     string strName = node.node().child_value(m_strPOIName.c_str());  
-  }  
## 写入xml方法：
先构造一个结构体
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- struct xml_string_writer : pugi::xml_writer  
- {  
-     std::string result;  
- virtualvoid write(constvoid* data, size_t size)  
-     {  
-         result += std::string(static_cast<constchar*>(data), size);  
-     }  
- };  
[cpp][view
 plain](http://blog.csdn.net/qingzai_/article/details/44982725#)[copy](http://blog.csdn.net/qingzai_/article/details/44982725#)
- void save_xml_config(const std::string &sql_ip, const std::string &sql_port, const std::string& user, const std::string& psw)  
- {  
-     pugi::xml_document doc;  
-     pugi::xml_node pre = doc.prepend_child(pugi::node_declaration);  
-     pre.append_attribute("version") = "1.0";  
-     pre.append_attribute("encoding") = "UTF-8";  
-     pre.append_attribute("standalone") = "no";  
- 
-     pugi::xml_node node = doc.append_child("sys");  
-     pugi::xml_node child_node = node.append_child("item");  
-     child_node.append_attribute("name").set_value("mysqlIp");  
-     child_node.append_attribute("value").set_value(sql_ip.c_str());  
- 
-     child_node = node.append_child("item");  
-     child_node.append_attribute("name").set_value("mysqlPort");  
-     child_node.append_attribute("value").set_value(sql_port.c_str());  
- 
-     child_node = node.append_child("item");  
-     child_node.append_attribute("name").set_value("userName");  
-     child_node.append_attribute("value").set_value(user.c_str());  
- 
-     child_node = node.append_child("item");  
-     child_node.append_attribute("name").set_value("userPassword");  
-     child_node.append_attribute("value").set_value(psw.c_str());  
-     std::string strXmlData;  
-     xml_string_writer writer;  
-     doc.save(writer);  
-     strXmlData = writer.result;  
- }  
- /************************************************************************/
- /*
- 上述字符串内容显示结果
- <?xml version="1.0" encoding="UTF-8" standalone="no"?>
-   <sys>
-   <item name="mysqlIp" value="127.0.0.1" />
-   <item name="mysqlPort" value="8888" />
-   <item name="userName" value="admin" />
-   <item name="userPassword" value="123465" />
-   </sys>
- */
- /************************************************************************/
