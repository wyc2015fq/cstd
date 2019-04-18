# SPRING-WS CLIENT 端 WSDL 试用 - z69183787的专栏 - CSDN博客
置顶2016年12月06日 16:08:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3408
wsdl [使用中国气象局的天气预报webservice](http://blog.csdn.net/is_zhoufeng/article/details/8485548)
地址：http://www.webxml.com.cn/WebServices/WeatherWebService.asmx?wsdl
直接使用wsimport 通过该地址生存java 文件时，会报错。因为该wsdl里面包含 ref = "s:schema" 这样的引用。而jaxb是不支持的。所以手动将该wsdl下载下来做下修改，然后再生成java文件。
修改方法为：
将所有的<s:element ref="s:schema"/> 改成 <s:any minOccurs="2" maxOccurs="2"/>
然后再通过wsimport生成java文件。 如果还是报错，就将wsdl文件里面的<wsdl:documentation 节点全部删除。应该就没问题了
也可以使用idea自带的 Generate java code from wsdl 生成：如下图
![](https://img-blog.csdn.net/20161206155140043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改后的 wsdl：
```
<?xml version="1.0" encoding="utf-8"?>
<wsdl:definitions xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" xmlns:tm="http://microsoft.com/wsdl/mime/textMatching/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:mime="http://schemas.xmlsoap.org/wsdl/mime/" xmlns:tns="http://WebXml.com.cn/" xmlns:s="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://schemas.xmlsoap.org/wsdl/soap12/" xmlns:http="http://schemas.xmlsoap.org/wsdl/http/" targetNamespace="http://WebXml.com.cn/" xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/">
  <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><a href="http://www.webxml.com.cn/" target="_blank">WebXml.com.cn</a> <strong>天气预报 Web 服务，数据每2.5小时左右自动更新一次，准确可靠。包括 340 多个中国主要城市和 60 多个国外主要城市三日内的天气预报数据。</br>此天气预报Web Services请不要用于任何商业目的，若有需要请<a href="http://www.webxml.com.cn/zh_cn/contact_us.aspx" target="_blank">联系我们</a>，欢迎技术交流。 QQ：8409035<br />使用本站 WEB 服务请注明或链接本站：http://www.webxml.com.cn/ 感谢大家的支持</strong>！<br /><span style="color:#999999;">通知：天气预报 WEB 服务如原来使用地址 http://www.onhap.com/WebServices/WeatherWebService.asmx 的，请改成现在使用的服务地址 http://www.webxml.com.cn/WebServices/WeatherWebService.asmx ，重新引用即可。</span><br /><br /> </wsdl:documentation>
  <wsdl:types>
    <s:schema elementFormDefault="qualified" targetNamespace="http://WebXml.com.cn/">
      <s:element name="getSupportCity">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="byProvinceName" type="s:string" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getSupportCityResponse">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="getSupportCityResult" type="tns:ArrayOfString" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:complexType name="ArrayOfString">
        <s:sequence>
          <s:element minOccurs="0" maxOccurs="unbounded" name="string" nillable="true" type="s:string" />
        </s:sequence>
      </s:complexType>
      <s:element name="getSupportProvince">
        <s:complexType />
      </s:element>
      <s:element name="getSupportProvinceResponse">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="getSupportProvinceResult" type="tns:ArrayOfString" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getSupportDataSet">
        <s:complexType />
      </s:element>
      <s:element name="getSupportDataSetResponse">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="getSupportDataSetResult">
              <s:complexType>
                <s:sequence>
                    <s:any minOccurs="2" maxOccurs="2"/>
                </s:sequence>
              </s:complexType>
            </s:element>
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getWeatherbyCityName">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="theCityName" type="s:string" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getWeatherbyCityNameResponse">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="getWeatherbyCityNameResult" type="tns:ArrayOfString" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getWeatherbyCityNamePro">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="theCityName" type="s:string" />
            <s:element minOccurs="0" maxOccurs="1" name="theUserID" type="s:string" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="getWeatherbyCityNameProResponse">
        <s:complexType>
          <s:sequence>
            <s:element minOccurs="0" maxOccurs="1" name="getWeatherbyCityNameProResult" type="tns:ArrayOfString" />
          </s:sequence>
        </s:complexType>
      </s:element>
      <s:element name="ArrayOfString" nillable="true" type="tns:ArrayOfString" />
      <s:element name="DataSet" nillable="true">
        <s:complexType>
          <s:sequence>
              <s:any minOccurs="2" maxOccurs="2"/>
          </s:sequence>
        </s:complexType>
      </s:element>
    </s:schema>
  </wsdl:types>
  <wsdl:message name="getSupportCitySoapIn">
    <wsdl:part name="parameters" element="tns:getSupportCity" />
  </wsdl:message>
  <wsdl:message name="getSupportCitySoapOut">
    <wsdl:part name="parameters" element="tns:getSupportCityResponse" />
  </wsdl:message>
  <wsdl:message name="getSupportProvinceSoapIn">
    <wsdl:part name="parameters" element="tns:getSupportProvince" />
  </wsdl:message>
  <wsdl:message name="getSupportProvinceSoapOut">
    <wsdl:part name="parameters" element="tns:getSupportProvinceResponse" />
  </wsdl:message>
  <wsdl:message name="getSupportDataSetSoapIn">
    <wsdl:part name="parameters" element="tns:getSupportDataSet" />
  </wsdl:message>
  <wsdl:message name="getSupportDataSetSoapOut">
    <wsdl:part name="parameters" element="tns:getSupportDataSetResponse" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameSoapIn">
    <wsdl:part name="parameters" element="tns:getWeatherbyCityName" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameSoapOut">
    <wsdl:part name="parameters" element="tns:getWeatherbyCityNameResponse" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProSoapIn">
    <wsdl:part name="parameters" element="tns:getWeatherbyCityNamePro" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProSoapOut">
    <wsdl:part name="parameters" element="tns:getWeatherbyCityNameProResponse" />
  </wsdl:message>
  <wsdl:message name="getSupportCityHttpGetIn">
    <wsdl:part name="byProvinceName" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getSupportCityHttpGetOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getSupportProvinceHttpGetIn" />
  <wsdl:message name="getSupportProvinceHttpGetOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getSupportDataSetHttpGetIn" />
  <wsdl:message name="getSupportDataSetHttpGetOut">
    <wsdl:part name="Body" element="tns:DataSet" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameHttpGetIn">
    <wsdl:part name="theCityName" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameHttpGetOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProHttpGetIn">
    <wsdl:part name="theCityName" type="s:string" />
    <wsdl:part name="theUserID" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProHttpGetOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getSupportCityHttpPostIn">
    <wsdl:part name="byProvinceName" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getSupportCityHttpPostOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getSupportProvinceHttpPostIn" />
  <wsdl:message name="getSupportProvinceHttpPostOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getSupportDataSetHttpPostIn" />
  <wsdl:message name="getSupportDataSetHttpPostOut">
    <wsdl:part name="Body" element="tns:DataSet" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameHttpPostIn">
    <wsdl:part name="theCityName" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameHttpPostOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProHttpPostIn">
    <wsdl:part name="theCityName" type="s:string" />
    <wsdl:part name="theUserID" type="s:string" />
  </wsdl:message>
  <wsdl:message name="getWeatherbyCityNameProHttpPostOut">
    <wsdl:part name="Body" element="tns:ArrayOfString" />
  </wsdl:message>
  <wsdl:portType name="WeatherWebServiceSoap">
    <wsdl:operation name="getSupportCity">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>查询本天气预报Web Services支持的国内外城市或地区信息</h3><p>输入参数：byProvinceName = 指定的洲或国内的省份，若为ALL或空则表示返回全部城市；返回数据：一个一维字符串数组 String()，结构为：城市名称(城市代码)。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportCitySoapIn" />
      <wsdl:output message="tns:getSupportCitySoapOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无； 返回数据：一个一维字符串数组 String()，内容为洲或国内省份的名称。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportProvinceSoapIn" />
      <wsdl:output message="tns:getSupportProvinceSoapOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无；返回：DataSet 。DataSet.Tables(0) 为支持的洲和国内省份数据，DataSet.Tables(1) 为支持的国内外城市或地区数据。DataSet.Tables(0).Rows(i).Item("ID") 主键对应 DataSet.Tables(1).Rows(i).Item("ZoneID") 外键。<br />Tables(0)：ID = ID主键，Zone = 支持的洲、省份；Tables(1)：ID 主键，ZoneID = 对应Tables(0)ID的外键，Area = 城市或地区，AreaCode = 城市或地区代码。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportDataSetSoapIn" />
      <wsdl:output message="tns:getSupportDataSetSoapOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数</h3><p>调用方法如下：输入参数：theCityName = 城市中文名称(国外城市可用英文)或城市代码(不输入默认为上海市)，如：上海 或 58367，如有城市名称重复请使用城市代码查询(可通过 getSupportCity 或 getSupportDataSet 获得)；返回数据： 一个一维数组 String(22)，共有23个元素。<br />String(0) 到 String(4)：省份，城市，城市代码，城市图片名称，最后更新时间。String(5) 到 String(11)：当天的 气温，概况，风向和风力，天气趋势开始图片名称(以下称：图标一)，天气趋势结束图片名称(以下称：图标二)，现在的天气实况，天气和生活指数。String(12) 到 String(16)：第二天的 气温，概况，风向和风力，图标一，图标二。String(17) 到 String(21)：第三天的 气温，概况，风向和风力，图标一，图标二。String(22) 被查询的城市或地区的介绍 <br /><a href="http://www.webxml.com.cn/images/weather.zip">下载天气图标<img src="http://www.webxml.com.cn/images/download_w.gif" border="0" align="absbottom" /></a>(包含大、中、小尺寸) <a href="http://www.webxml.com.cn/zh_cn/weather_icon.aspx" target="_blank">天气图例说明</a> <a href="http://www.webxml.com.cn/files/weather_eg.zip">调用此天气预报Web Services实例下载</a> (VB ASP.net 2.0)</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameSoapIn" />
      <wsdl:output message="tns:getWeatherbyCityNameSoapOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数（For商业用户）</h3><p>调用方法同 getWeatherbyCityName，输入参数：theUserID = 商业用户ID</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameProSoapIn" />
      <wsdl:output message="tns:getWeatherbyCityNameProSoapOut" />
    </wsdl:operation>
  </wsdl:portType>
  <wsdl:portType name="WeatherWebServiceHttpGet">
    <wsdl:operation name="getSupportCity">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>查询本天气预报Web Services支持的国内外城市或地区信息</h3><p>输入参数：byProvinceName = 指定的洲或国内的省份，若为ALL或空则表示返回全部城市；返回数据：一个一维字符串数组 String()，结构为：城市名称(城市代码)。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportCityHttpGetIn" />
      <wsdl:output message="tns:getSupportCityHttpGetOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无； 返回数据：一个一维字符串数组 String()，内容为洲或国内省份的名称。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportProvinceHttpGetIn" />
      <wsdl:output message="tns:getSupportProvinceHttpGetOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无；返回：DataSet 。DataSet.Tables(0) 为支持的洲和国内省份数据，DataSet.Tables(1) 为支持的国内外城市或地区数据。DataSet.Tables(0).Rows(i).Item("ID") 主键对应 DataSet.Tables(1).Rows(i).Item("ZoneID") 外键。<br />Tables(0)：ID = ID主键，Zone = 支持的洲、省份；Tables(1)：ID 主键，ZoneID = 对应Tables(0)ID的外键，Area = 城市或地区，AreaCode = 城市或地区代码。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportDataSetHttpGetIn" />
      <wsdl:output message="tns:getSupportDataSetHttpGetOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数</h3><p>调用方法如下：输入参数：theCityName = 城市中文名称(国外城市可用英文)或城市代码(不输入默认为上海市)，如：上海 或 58367，如有城市名称重复请使用城市代码查询(可通过 getSupportCity 或 getSupportDataSet 获得)；返回数据： 一个一维数组 String(22)，共有23个元素。<br />String(0) 到 String(4)：省份，城市，城市代码，城市图片名称，最后更新时间。String(5) 到 String(11)：当天的 气温，概况，风向和风力，天气趋势开始图片名称(以下称：图标一)，天气趋势结束图片名称(以下称：图标二)，现在的天气实况，天气和生活指数。String(12) 到 String(16)：第二天的 气温，概况，风向和风力，图标一，图标二。String(17) 到 String(21)：第三天的 气温，概况，风向和风力，图标一，图标二。String(22) 被查询的城市或地区的介绍 <br /><a href="http://www.webxml.com.cn/images/weather.zip">下载天气图标<img src="http://www.webxml.com.cn/images/download_w.gif" border="0" align="absbottom" /></a>(包含大、中、小尺寸) <a href="http://www.webxml.com.cn/zh_cn/weather_icon.aspx" target="_blank">天气图例说明</a> <a href="http://www.webxml.com.cn/files/weather_eg.zip">调用此天气预报Web Services实例下载</a> (VB ASP.net 2.0)</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameHttpGetIn" />
      <wsdl:output message="tns:getWeatherbyCityNameHttpGetOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数（For商业用户）</h3><p>调用方法同 getWeatherbyCityName，输入参数：theUserID = 商业用户ID</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameProHttpGetIn" />
      <wsdl:output message="tns:getWeatherbyCityNameProHttpGetOut" />
    </wsdl:operation>
  </wsdl:portType>
  <wsdl:portType name="WeatherWebServiceHttpPost">
    <wsdl:operation name="getSupportCity">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>查询本天气预报Web Services支持的国内外城市或地区信息</h3><p>输入参数：byProvinceName = 指定的洲或国内的省份，若为ALL或空则表示返回全部城市；返回数据：一个一维字符串数组 String()，结构为：城市名称(城市代码)。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportCityHttpPostIn" />
      <wsdl:output message="tns:getSupportCityHttpPostOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br /><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无； 返回数据：一个一维字符串数组 String()，内容为洲或国内省份的名称。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportProvinceHttpPostIn" />
      <wsdl:output message="tns:getSupportProvinceHttpPostOut" />
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>获得本天气预报Web Services支持的洲、国内外省份和城市信息</h3><p>输入参数：无；返回：DataSet 。DataSet.Tables(0) 为支持的洲和国内省份数据，DataSet.Tables(1) 为支持的国内外城市或地区数据。DataSet.Tables(0).Rows(i).Item("ID") 主键对应 DataSet.Tables(1).Rows(i).Item("ZoneID") 外键。<br />Tables(0)：ID = ID主键，Zone = 支持的洲、省份；Tables(1)：ID 主键，ZoneID = 对应Tables(0)ID的外键，Area = 城市或地区，AreaCode = 城市或地区代码。</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getSupportDataSetHttpPostIn" />
      <wsdl:output message="tns:getSupportDataSetHttpPostOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数</h3><p>调用方法如下：输入参数：theCityName = 城市中文名称(国外城市可用英文)或城市代码(不输入默认为上海市)，如：上海 或 58367，如有城市名称重复请使用城市代码查询(可通过 getSupportCity 或 getSupportDataSet 获得)；返回数据： 一个一维数组 String(22)，共有23个元素。<br />String(0) 到 String(4)：省份，城市，城市代码，城市图片名称，最后更新时间。String(5) 到 String(11)：当天的 气温，概况，风向和风力，天气趋势开始图片名称(以下称：图标一)，天气趋势结束图片名称(以下称：图标二)，现在的天气实况，天气和生活指数。String(12) 到 String(16)：第二天的 气温，概况，风向和风力，图标一，图标二。String(17) 到 String(21)：第三天的 气温，概况，风向和风力，图标一，图标二。String(22) 被查询的城市或地区的介绍 <br /><a href="http://www.webxml.com.cn/images/weather.zip">下载天气图标<img src="http://www.webxml.com.cn/images/download_w.gif" border="0" align="absbottom" /></a>(包含大、中、小尺寸) <a href="http://www.webxml.com.cn/zh_cn/weather_icon.aspx" target="_blank">天气图例说明</a> <a href="http://www.webxml.com.cn/files/weather_eg.zip">调用此天气预报Web Services实例下载</a> (VB ASP.net 2.0)</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameHttpPostIn" />
      <wsdl:output message="tns:getWeatherbyCityNameHttpPostOut" />
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><br><h3>根据城市或地区名称查询获得未来三天内天气情况、现在的天气实况、天气和生活指数（For商业用户）</h3><p>调用方法同 getWeatherbyCityName，输入参数：theUserID = 商业用户ID</p><br /></wsdl:documentation>
      <wsdl:input message="tns:getWeatherbyCityNameProHttpPostIn" />
      <wsdl:output message="tns:getWeatherbyCityNameProHttpPostOut" />
    </wsdl:operation>
  </wsdl:portType>
  <wsdl:binding name="WeatherWebServiceSoap" type="tns:WeatherWebServiceSoap">
    <soap:binding transport="http://schemas.xmlsoap.org/soap/http" />
    <wsdl:operation name="getSupportCity">
      <soap:operation soapAction="http://WebXml.com.cn/getSupportCity" style="document" />
      <wsdl:input>
        <soap:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <soap:operation soapAction="http://WebXml.com.cn/getSupportProvince" style="document" />
      <wsdl:input>
        <soap:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <soap:operation soapAction="http://WebXml.com.cn/getSupportDataSet" style="document" />
      <wsdl:input>
        <soap:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <soap:operation soapAction="http://WebXml.com.cn/getWeatherbyCityName" style="document" />
      <wsdl:input>
        <soap:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <soap:operation soapAction="http://WebXml.com.cn/getWeatherbyCityNamePro" style="document" />
      <wsdl:input>
        <soap:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
  </wsdl:binding>
  <wsdl:binding name="WeatherWebServiceSoap12" type="tns:WeatherWebServiceSoap">
    <soap12:binding transport="http://schemas.xmlsoap.org/soap/http" />
    <wsdl:operation name="getSupportCity">
      <soap12:operation soapAction="http://WebXml.com.cn/getSupportCity" style="document" />
      <wsdl:input>
        <soap12:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap12:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <soap12:operation soapAction="http://WebXml.com.cn/getSupportProvince" style="document" />
      <wsdl:input>
        <soap12:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap12:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <soap12:operation soapAction="http://WebXml.com.cn/getSupportDataSet" style="document" />
      <wsdl:input>
        <soap12:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap12:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <soap12:operation soapAction="http://WebXml.com.cn/getWeatherbyCityName" style="document" />
      <wsdl:input>
        <soap12:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap12:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <soap12:operation soapAction="http://WebXml.com.cn/getWeatherbyCityNamePro" style="document" />
      <wsdl:input>
        <soap12:body use="literal" />
      </wsdl:input>
      <wsdl:output>
        <soap12:body use="literal" />
      </wsdl:output>
    </wsdl:operation>
  </wsdl:binding>
  <wsdl:binding name="WeatherWebServiceHttpGet" type="tns:WeatherWebServiceHttpGet">
    <http:binding verb="GET" />
    <wsdl:operation name="getSupportCity">
      <http:operation location="/getSupportCity" />
      <wsdl:input>
        <http:urlEncoded />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <http:operation location="/getSupportProvince" />
      <wsdl:input>
        <http:urlEncoded />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <http:operation location="/getSupportDataSet" />
      <wsdl:input>
        <http:urlEncoded />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <http:operation location="/getWeatherbyCityName" />
      <wsdl:input>
        <http:urlEncoded />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <http:operation location="/getWeatherbyCityNamePro" />
      <wsdl:input>
        <http:urlEncoded />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
  </wsdl:binding>
  <wsdl:binding name="WeatherWebServiceHttpPost" type="tns:WeatherWebServiceHttpPost">
    <http:binding verb="POST" />
    <wsdl:operation name="getSupportCity">
      <http:operation location="/getSupportCity" />
      <wsdl:input>
        <mime:content type="application/x-www-form-urlencoded" />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportProvince">
      <http:operation location="/getSupportProvince" />
      <wsdl:input>
        <mime:content type="application/x-www-form-urlencoded" />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getSupportDataSet">
      <http:operation location="/getSupportDataSet" />
      <wsdl:input>
        <mime:content type="application/x-www-form-urlencoded" />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityName">
      <http:operation location="/getWeatherbyCityName" />
      <wsdl:input>
        <mime:content type="application/x-www-form-urlencoded" />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
    <wsdl:operation name="getWeatherbyCityNamePro">
      <http:operation location="/getWeatherbyCityNamePro" />
      <wsdl:input>
        <mime:content type="application/x-www-form-urlencoded" />
      </wsdl:input>
      <wsdl:output>
        <mime:mimeXml part="Body" />
      </wsdl:output>
    </wsdl:operation>
  </wsdl:binding>
  <wsdl:service name="WeatherWebService">
    <wsdl:documentation xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"><a href="http://www.webxml.com.cn/" target="_blank">WebXml.com.cn</a> <strong>天气预报 Web 服务，数据每2.5小时左右自动更新一次，准确可靠。包括 340 多个中国主要城市和 60 多个国外主要城市三日内的天气预报数据。</br>此天气预报Web Services请不要用于任何商业目的，若有需要请<a href="http://www.webxml.com.cn/zh_cn/contact_us.aspx" target="_blank">联系我们</a>，欢迎技术交流。 QQ：8409035<br />使用本站 WEB 服务请注明或链接本站：http://www.webxml.com.cn/ 感谢大家的支持</strong>！<br /><span style="color:#999999;">通知：天气预报 WEB 服务如原来使用地址 http://www.onhap.com/WebServices/WeatherWebService.asmx 的，请改成现在使用的服务地址 http://www.webxml.com.cn/WebServices/WeatherWebService.asmx ，重新引用即可。</span><br /><br /> </wsdl:documentation>
    <wsdl:port name="WeatherWebServiceSoap" binding="tns:WeatherWebServiceSoap">
      <soap:address location="http://www.webxml.com.cn/WebServices/WeatherWebService.asmx" />
    </wsdl:port>
    <wsdl:port name="WeatherWebServiceSoap12" binding="tns:WeatherWebServiceSoap12">
      <soap12:address location="http://www.webxml.com.cn/WebServices/WeatherWebService.asmx" />
    </wsdl:port>
    <wsdl:port name="WeatherWebServiceHttpGet" binding="tns:WeatherWebServiceHttpGet">
      <http:address location="http://www.webxml.com.cn/WebServices/WeatherWebService.asmx" />
    </wsdl:port>
    <wsdl:port name="WeatherWebServiceHttpPost" binding="tns:WeatherWebServiceHttpPost">
      <http:address location="http://www.webxml.com.cn/WebServices/WeatherWebService.asmx" />
    </wsdl:port>
  </wsdl:service>
</wsdl:definitions>
```
正常方式调用：
```java
WeatherWebService weatherWebService = new WeatherWebService();
        WeatherWebServiceSoap weatherWebServiceSoap = weatherWebService.getWeatherWebServiceSoap();
        ArrayOfString array = weatherWebServiceSoap.getWeatherbyCityName("上海");
        List<String> list = array.getString();
        int i = 0;
        for(String str:list){
            System.out.println(i++ + ": "+ str);
        }
```
Spring-ws 方式调用：
pom：
```
<!-- spring-ws-->
        <dependency>
            <groupId>org.springframework.ws</groupId>
            <artifactId>spring-ws-core</artifactId>
            <version>2.4.0.RELEASE</version>
        </dependency>
```
bean：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.1.xsd">
    <!--<context:component-scan base-package="spring-ws" />-->
    <!--<context:annotation-config />-->
    <bean id="messageFactory" class="org.springframework.ws.soap.saaj.SaajSoapMessageFactory" />
    <bean id="jaxb2Marshaller" class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
        <property name="contextPath" value="springWs" />
    </bean>
    <bean id="webServiceTemplate" class="org.springframework.ws.client.core.WebServiceTemplate">
        <constructor-arg ref="messageFactory" />
        <property name="marshaller" ref="jaxb2Marshaller"></property>
        <property name="unmarshaller" ref="jaxb2Marshaller"></property>
        <property name="messageSender">
            <bean class="org.springframework.ws.transport.http.HttpComponentsMessageSender" />
        </property>
        <property name="defaultUri" value="http://www.webxml.com.cn/WebServices/WeatherWebService.asmx"></property>
    </bean>
</beans>
```
main：自测下来 返回 302  ，可能是因为 webservice地址 是 https 的 当中有一个跳转，导致spring-ws自带的不可用。
但上述正常的调用方式可用。
```java
WebServiceTemplate webServiceTemplate = (WebServiceTemplate) context.getBean("webServiceTemplate");
        //http://WebXml.com.cn/getWeatherbyCityName
        GetWeatherbyCityName request = new GetWeatherbyCityName();
        request.setTheCityName("上海");
        GetWeatherbyCityNameResponse response = (GetWeatherbyCityNameResponse) webServiceTemplate.marshalSendAndReceive(
                request,
                new SoapActionCallback(
                        "http://WebXml.com.cn/getWeatherbyCityName"));
        System.out.println(response);
```
302 error：暂时不知道如何解决
```
Exception in thread "main" org.springframework.ws.client.WebServiceTransportException: Found [302]
	at org.springframework.ws.client.core.WebServiceTemplate.handleError(WebServiceTemplate.java:699)
	at org.springframework.ws.client.core.WebServiceTemplate.doSendAndReceive(WebServiceTemplate.java:609)
	at org.springframework.ws.client.core.WebServiceTemplate.sendAndReceive(WebServiceTemplate.java:555)
	at org.springframework.ws.client.core.WebServiceTemplate.marshalSendAndReceive(WebServiceTemplate.java:390)
	at org.springframework.ws.client.core.WebServiceTemplate.marshalSendAndReceive(WebServiceTemplate.java:383)
	at springWs.client.Main.main(Main.java:34)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at com.intellij.rt.execution.application.AppMain.main(AppMain.java:134)
```
例2：
配置方面均与一一致
wsdl 生成java文件：
![](https://img-blog.csdn.net/20161207150526222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Main
**区别：这次反向生成的java类中的 请求与响应pojo 均包含jaxbElement。调用时可用同时生成的ObjectFactory进行 create**
**原因：http://blog.csdn.net/z69183787/article/details/53505941**
```java
public class ddMain {
    public static void main(String[] args){
        ApplicationContext context = new ClassPathXmlApplicationContext("classpath*:spring-ws-dd-core.xml");
        String json = "{\"Key\":\"peopleslkvnfn09\",\"RequestType\":{\"Type\":\"0\"},\"RequestObjectList\":[{\"CITY_NAME\":\"上海市\"}]}";
        //普通调用
        BaseInfoService baseInfoService = new BaseInfoService();
        IBaseInfoService iBaseInfoService = baseInfoService.getBasicHttpBindingIBaseInfoService();
        String name = iBaseInfoService.getCity(json);
        System.out.println(name);
        //spring-ws 调用
        WebServiceTemplate webServiceTemplate = (WebServiceTemplate) context.getBean("ddWebServiceTemplate");
        //http://WebXml.com.cn/getWeatherbyCityName
        springWs.dd.ObjectFactory objectFactory = new ObjectFactory();
        GetCity request = new GetCity();
        request.setInputParam(objectFactory.createGetCityInputParam(json));
        GetCityResponse response = (GetCityResponse) webServiceTemplate.marshalSendAndReceive(
                request,
                new SoapActionCallback(
                        "http://tempuri.org/IBaseInfoService/GetCity"));
        System.out.println(response.getGetCityResult().getValue());
    }
```
