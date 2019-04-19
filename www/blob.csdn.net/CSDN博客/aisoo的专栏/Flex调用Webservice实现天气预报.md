# Flex调用Webservice实现天气预报 - aisoo的专栏 - CSDN博客
2010年08月13日 09:53:00[aisoo](https://me.csdn.net/aisoo)阅读数：1035
Flex调用Webservice有多种方法，可以通过Flex Builder提供的管理Webservices的工具解析WSDL文档生成本地调用类。也可以通过Action Script中Webservice类来实现。还有就是使用<Webservice>组件实现。这次我使用的是最后一种，感觉这种方法较为简洁
完整代码如下
<?xml version = "1.0" encoding = "UTF-8"?>
<mx:Application xmlns:mx = "[http://www.adobe.com/2006/mxml](http://www.adobe.com/2006/mxml)"
                width = "330"
                height = "155"
                layout = "absolute"
                fontSize = "12"
                viewSourceURL = "srcview/index.html">
    <mx:Script>
        <![CDATA[
            import mx.collections.ArrayCollection;
            import mx.controls.Alert;
            //保存接收到的结果
            public var arrayResult:ArrayCollection; 
            //处理当输入栏获得焦点时，输入栏清空
            public function getfocus():void
            {
                city.text = "";
            }
            public function resultOK():void
            { 
               //显示接受的结果
                arrayResult = new ArrayCollection();
                arrayResult = ws.getWeatherbyCityName.lastResult;
                //显示今天的情况
                jin1.text =arrayResult[6].toString().substring(0,arrayResult[6].toString().indexOf(" "));
                jin2.text =arrayResult[6].toString().substring((arrayResult[6].toString().indexOf(" ")) + 1);
                jin3.text = arrayResult[5];
                jin4.text = arrayResult[7];
                //显示明天的情况
                ming1.text =arrayResult[13].toString().substring(0,arrayResult[13].toString().indexOf(" "));
                ming2.text =arrayResult[13].toString().substring((arrayResult[13].toString().indexOf(" ")) + 1);
                ming3.text = arrayResult[12];
                ming4.text = arrayResult[14];
                //显示后天的情况
                hou1.text =arrayResult[18].toString().substring(0,arrayResult[18].toString().indexOf(" "));
                hou2.text =arrayResult[18].toString().substring((arrayResult[18].toString().indexOf(" ")) + 1);
                hou3.text = arrayResult[17];
                hou4.text = arrayResult[19];
            }
            //显示说明
            public function showMessage():void
            {
                var alert:Alert = Alert.show("请输入城市中文名称(国外城市可用英文)。web服务来源于[http://www.webxml.com.cn](http://www.webxml.com.cn)，数据来源于中国气象局" + "，包括340多个中国" + "主要城市和60多个国外主要城市三日内的天气情况。作者EMAIL：[anhulife@gmail.com](mailto:anhulife@gmail.com)","说明", Alert.YES);
            }
        ]]>
    </mx:Script>
    <mx:WebService id = "ws"
                   wsdl = "[http://www.webxml.com.cn/WebServices/WeatherWebService.asmx?wsdl](http://www.webxml.com.cn/WebServices/WeatherWebService.asmx?wsdl)"
                   useProxy = "false"
                   showBusyCursor = "true">
        <mx:operation name = "getWeatherbyCityName"
                      result = "resultOK()">
            <mx:request>
                <theCityName>
                    {city.text.toString()}
                </theCityName>
            </mx:request>
        </mx:operation>
    </mx:WebService>
    <mx:TextInput id = "city"
                  width = "200"
                  text = "请输入城市名，支持国内外主要城市"
                  x = "10"
                  y = "10"
                  focusIn = "getfocus()"
                  fontSize = "12"
                  color = "#909697"/>
    <mx:Button id = "check"
               label = "查询"
               x = "230"
               y = "10"
               click = "ws.getWeatherbyCityName.send()"/>
    <!--显示结果的Label-->
    <mx:Label id = "jin1"
              width = "90"
              text = ""
              x = "10"
              y = "42"/>
    <mx:Label id = "jin2"
              width = "90"
              text = ""
              x = "10"
              y = "70"/>
    <mx:Label id = "jin3"
              width = "90"
              text = ""
              x = "10"
              y = "98"/>
    <mx:Label id = "jin4"
              width = "90"
              text = ""
              x = "10"
              y = "126"/>
    <mx:Label id = "ming1"
              width = "90"
              text = ""
              x = "120"
              y = "42"/>
    <mx:Label id = "ming2"
              width = "90"
              text = ""
              x = "120"
              y = "70"/>
    <mx:Label id = "ming3"
              width = "90"
              text = ""
              x = "120"
              y = "98"/>
    <mx:Label id = "ming4"
              width = "90"
              text = ""
              x = "120"
              y = "126"/>
    <mx:Label id = "hou1"
              width = "90"
              text = ""
              x = "230"
              y = "41"/>
    <mx:Label id = "hou2"
              width = "90"
              text = ""
              x = "230"
              y = "69"/>
    <mx:Label id = "hou3"
              width = "90"
              text = ""
              x = "230"
              y = "97"/>
    <mx:Label id = "hou4"
              width = "90"
              text = ""
              x = "230"
              y = "125"/>
    <!--显示说明的Label-->
    <mx:Label width = "32"
              text = "说明"
              x = "288"
              y = "12"
              click = "showMessage()"
              textDecoration = "underline"
              color = "#FAFBFB"/>
</mx:Application>
