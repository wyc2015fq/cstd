# boost生成和解析json - 深之JohnChen的专栏 - CSDN博客

2018年08月30日 19:41:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：1425


一、boost生成和解析json

解析Json的方法有很多，也有不少的第三方开源工具。比如boost json、jsoncpp库。这里仅介绍其中的一种，用Bosst解析。Boost库是一个可移植、提供源代码的C++库，作为标准库的后备，是C++标准化进程的发动机之一。 Boost库由C++标准委员会库工作组成员发起，其中有些内容有望成为下一代C++标准库内容。在C++社区中影响甚大，是不折不扣的“准”标准库。Boost由于其对跨平台的强调，对标准C++的强调，与编写平台无关。大部分boost库功能的使用只需包括相应头文件即可，少数（如正则表达式库，文件系统库等）需要链接库。

property_tree可以解析xml，json，ini，info等格式的数据，用property_tree解析这几种格式使用方法很相似。

解析json很简单，命名空间为boost::property_tree，reson_json函数将文件流、字符串解析到ptree，write_json将ptree输出为字符串或文件流。其余的都是对ptree的操作。

ptree中put和add区别，add是添加一个元素，名称重复也添加；put也添加一个元素，名称重复的被新值覆盖。

ptree中put_child和add_child区别，也是与上面同理。put_child和add_child是添加一个子对象。

要解析Json，需要包括头文件。

```
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>
```

```
//生成
	boost::property_tree::ptree pt_allitem;
	unsigned char ucAttahId = 1;
	boost::property_tree::ptree pt_item, pt_subitem;
	pt_item.put("attachid", ucAttahId);
	pt_subitem.put("mileage", 100.89);
	pt_item.put_child("attachcontent", pt_subitem);
	pt_allitem.add_child("attach",pt_item);
	pt_allitem.add_child("attach", pt_item);
	pt_allitem.add_child("attach", pt_item);
	std::stringstream ss;
	boost::property_tree::write_json(ss, pt_allitem);
	std::string strContent = ss.str();
	
//解析
std::string strResponse = ""//需要解析的json字符串
boost::property_tree::ptree pt;
std::stringstream sstream(strResponse);
boost::property_tree::json_parser::read_json(sstream, pt);
int nErrorCode = pt.get<int>("errorCode");
boost::property_tree::ptree pChild_Payload;
pChild_Payload = pt.get_child("payload");
nTotalPageCount = pChild_Payload.get<int>("totalPage");
nCurrentPage = pChild_Payload.get<int>("curPage");
boost::property_tree::ptree pChild_ModelList = pChild_Payload.get_child("modelList");
//遍历数据
BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pChild_ModelList)
{
 boost::property_tree::ptree p = v.second;
 boost::property_tree::ptree ptDevice = p.get_child("device");
}
					
遍历读取到map
ptree m_pt;
string strAttrName；
BOOST_FOREACH(ptree::value_type &v1, m_pt.get_child(L"root"))
{
    if (v1.first == L"Item")
    {
          strAttrName=v1.second.get<string>(L"<xmlattr>.name");
    }

}

删除节点
删除也一样，遍历之后删除节点。
ptree& persons = pt.get_child("root.persons");
for(auto it = persons.begin(); it != persons.end();) 
{
   if(it->second.get<string>("name") == "dad")
        it = persons.erase(it);
    else
        ++it;
}

修改值
改也很简单，遍历出所要改的值在节点，直接赋值然后再写回去就行了。
iter2->second.put_value(value);

抛出异常
property_tree的异常分两种，一种是路径错误，一种是值错误很好判断，异常直接告诉你哪个属性等有问题
try
    {    
        .......
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
        m_error = e.what();
        return false;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
        m_error =e.what();
        return false;
    }
```

二、boost json使用过程中需要注意的问题

1、boost json不支持空数组的生成，即生成json数组name要么为""或其他字符串，不能没有name（name为键值对"name":value中name）；但可以解析空数组json字符串。

     如何生成空数组json字符串，折衷方法通过boost json先生成带名称的数组。再通过截取[]前后字符串。

```
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/typeof/typeof.hpp"
#include "boost/optional.hpp"
#include <list>
#include <boost/algorithm/string.hpp>

//带名字数组jons字符串转换成空数组json字符串
bool ConvertWithNameJsonArrayStringToWithoutNameJsonArrayString(std::string strWithNameJson, std::string & strWithoutNameJson)
{
	if (strWithNameJson.size() == 0)
	{
		return false;
	}
	strWithoutNameJson = strWithNameJson;
	boost::iterator_range<std::string::iterator> result = boost::algorithm::find_first(strWithoutNameJson, "[");
	if (!result.empty())
	{
		std::string strTemp = strWithoutNameJson.substr(result.begin() - strWithoutNameJson.begin());
		strWithoutNameJson = strTemp;
		result = boost::algorithm::find_last(strWithoutNameJson, "]");
		//result = boost::algorithm::ifind_first(strWithoutNameJson, "]");
		if (!result.empty())
		{
			strTemp = strWithoutNameJson.substr(0, (result.begin() - strWithoutNameJson.begin() + 1));
			strWithoutNameJson = strTemp;
		}
	}
	return true;
}

//示例代码
boost::property_tree::ptree ptRoot;
	boost::property_tree::ptree ptAllGps;
	boost::property_tree::ptree ptGps;
	for (int n = 0; n < 10; n++)
	{
		ptGps.put("deviceid", "1234567");
		ptGps.put("alarmFlag", 0);
		ptGps.put("deviceStatus", 1);
		ptGps.put("lng", 111.0990909);
		ptGps.put("lat", 28.1112323);
		ptGps.put("speed", 80.0);
		ptGps.put("direction", 0);
		ptGps.put("gpsTime", "2018-08-28 12:12:11");
		//insert
		ptAllGps.push_back(make_pair("", ptGps));
	}
	ptRoot.add_child("allgps", ptAllGps);
	std::stringstream sstream;
	boost::property_tree::write_json(sstream, ptRoot);
	std::string strRequestParams = sstream.str();
	std::string strWithoutJsonArray = "";
	ConvertWithNameJsonArrayStringToWithoutNameJsonArrayString(strRequestParams, strWithoutJsonArray);

	boost::property_tree::ptree ptRootAllGps;
	std::stringstream sstream1(strWithoutJsonArray);
	boost::property_tree::json_parser::read_json(sstream1, ptRootAllGps);
	int kkk = 0;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, ptRootAllGps)
	{
		boost::property_tree::ptree p = v.second;
		std::string strDeviceId = p.get<std::string>("deviceid");
		std::string strTime = p.get<std::string>("gpsTime");
		double fLng = p.get<double>("lng");
		kkk++;
	}
```

2、空的字符串字段转换为数字会抛异常。

3、boost json线程不安全及其解决办法

原因是 boost json parser中用到ptree底层依赖boost::spirit，是线程不安全的，从而导致程序出core。解决方案：

在任何引入头文件的地方加上如下宏

```
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
```

4、put int，double都保存为string而不能保存int，double的解决方法
[https://stackoverflow.com/questions/2855741/why-boost-property-tree-write-json-saves-everything-as-string-is-it-possible-to](https://stackoverflow.com/questions/2855741/why-boost-property-tree-write-json-saves-everything-as-string-is-it-possible-to)

5、生成中文与解析中文问题。

boost的json格式生成带中文字符串时，出现中文乱码。

原因：使用boost的ptree输出json的时候，中文会被转换为\uxxxx的utf8编码形式，json的标准中对于中文是转换为\uxxxx的utf16编码形式。

解决方法：

方法一：使用wptree可解决此问题。

方法二：用ptree但需要修改boost\property_tree\detail\json_parser_write.hpp代码。如下：

```cpp
namespace boost { namespace property_tree { namespace json_parser
{
    // Create necessary escape sequences from illegal characters
    template<>
    std::basic_string<char> create_escapes(const std::basic_string<char> &s)
    {
        std::basic_string<char> result;
        std::basic_string<char>::const_iterator b = s.begin();
        std::basic_string<char>::const_iterator e = s.end();
        while (b != e)
        {
            // This assumes an ASCII superset. But so does everything in PTree.
            // We escape everything outside ASCII, because this code can't
            // handle high unicode characters.
            if (*b == 0x20 || *b == 0x21 || (*b >= 0x23 && *b <= 0x2E) ||
                (*b >= 0x30 && *b <= 0x5B) || (*b >= 0x5D && *b <= 0xFF)  //it fails here because char are signed
                || (*b >= -0x80 && *b < 0 ) ) // this will pass UTF-8 signed chars
                result += *b;
            else if (*b == char('\b')) result += char('\\'), result += char('b');
            else if (*b == char('\f')) result += char('\\'), result += char('f');
            else if (*b == char('\n')) result += char('\\'), result += char('n');
            else if (*b == char('\r')) result += char('\\'), result += char('r');
            else if (*b == char('/')) result += char('\\'), result += char('/');
            else if (*b == char('"'))  result += char('\\'), result += char('"');
            else if (*b == char('\\')) result += char('\\'), result += char('\\');
            else
            {
                const char *hexdigits = "0123456789ABCDEF";
                typedef make_unsigned<char>::type UCh;
                unsigned long u = (std::min)(static_cast<unsigned long>(
                                                 static_cast<UCh>(*b)),
                                             0xFFFFul);
                int d1 = u / 4096; u -= d1 * 4096;
                int d2 = u / 256; u -= d2 * 256;
                int d3 = u / 16; u -= d3 * 16;
                int d4 = u;
                result += char('\\'); result += char('u');
                result += char(hexdigits[d1]); result += char(hexdigits[d2]);
                result += char(hexdigits[d3]); result += char(hexdigits[d4]);
            }
            ++b;
        }
        return result;
    }
} } }
```

带中文的json字符串read_json解析出错。中文解析可以使用wptree，比如：

```
#include <atlbase.h>
		
		USES_CONVERSION;
		std::string strResponse = pDataBuffer;//输入字符串
		std::wstring strResponseNew = A2W(strResponse.c_str());
		//需要解析的json字符串
		boost::property_tree::wptree pt;
		std::wstringstream sstream(strResponseNew);
		boost::property_tree::json_parser::read_json(sstream, pt);
		boost::property_tree::wptree pNoedeDeviceList = pt.get_child(L"alldevices");
		//遍历数据
		std::wstring strDeviceId,strDeviceName;
		int nOnlineType = 0;
		BOOST_FOREACH(boost::property_tree::wptree::value_type &v, pNoedeDeviceList)
		{ 
			boost::property_tree::wptree ptDevice = v.second;
			strDeviceId = ptDevice.get<std::wstring>(L"id");
			nOnlineType = ptDevice.get<int>(L"onlinetype");
		}
```

6、简单的json解析与生成可以使用boost库实现，复杂的json解析与生成建议使用jsoncpp库。因为boost库对json的支持不是很好，有很多问题。

jsoncpp库下载代码：[https://github.com/open-source-parsers/jsoncpp](https://github.com/open-source-parsers/jsoncpp)

jsoncpp库的使用方法参考这篇文章：[https://www.cnblogs.com/liaocheng/p/4243731.html](https://www.cnblogs.com/liaocheng/p/4243731.html)

