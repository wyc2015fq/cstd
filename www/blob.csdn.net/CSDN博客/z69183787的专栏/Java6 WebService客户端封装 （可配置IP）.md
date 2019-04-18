# Java6 WebService客户端封装 （可配置IP） - z69183787的专栏 - CSDN博客
2014年04月14日 16:59:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4549
在开发WebService客户端应用的时候，面临的最大挑战是事先不知道服务端要部署在哪里，端口是什么，因此Hello World的例子并不适合生产环境，还需要进一步的做改进。
改进的总体思想是将静态的部分替换为动态的，也就是说，将服务的ip和端口，做成动态参数，然后根据服务端的部署灵活改变这些配置参数。
下面针对前面的例子对客户端做一个改进。
1、写WebService服务的配置参数文件
wscfg.properties
#Java6WS服务的配置 
Java6WS.wsip=192.168.14.117 
Java6WS.wsport=8888
2、写配置参数获取工具
SysParamsToolkit.java
package lavasoft.common; 
import java.io.IOException; 
import java.util.Properties; 
/** 
* 媒资系统配置文件参数获取工具 
* 
* @author leizhimin 2009-11-16 15:21:22 
*/
publicfinalclass SysParamsToolkit { 
privatestatic Properties prop; 
private SysParamsToolkit() { 
        } 
static { 
                reload(); 
        } 
publicstaticboolean reload() { 
boolean flag = true; 
                prop = new Properties(); 
try { 
                        prop.load(SysParamsToolkit.class.getResourceAsStream("/wscfg.properties")); 
                        flag = false; 
                } catch (IOException e) { 
                        e.printStackTrace(); 
                } 
return flag; 
        } 
publicstatic Properties getSysProperties() { 
return prop; 
        } 
/** 
         * 获取指定的系统属性值 
         * 
         * @param key 指定的属性名称 
         * @return 指定的系统属性值 
         */
publicstatic String getProperty(String key) { 
return prop.getProperty(key); 
        } 
/** 
         * 获取指定的系统属性值 
         * 
         * @param key                指定的属性名称 
         * @param defaultVal 默认值 
         * @return 指定的系统属性值 
         */
publicstatic String getProperty(String key, String defaultVal) { 
return prop.getProperty(key, defaultVal); 
        } 
publicstaticvoid main(String[] args) { 
                Properties prop = getSysProperties(); 
        } 
}
3、修改Java6工具生成的代码，改静态的IP和端口为动态的参数。
这一步是最关键的一步，前面的都是准备工作，这一步才是实质性的工作，首先你要找到所生成客户端代码的服务接口实现类，在此是Java6WSService类。
@WebServiceClient(name = "Java6WSService", targetNamespace = "http://server.lavasoft/", wsdlLocation
 = "http://192.168.14.117:8080/java6ws/Java6WS?wsdl") 
publicclass Java6WSService 
extends Service { 
privatefinalstatic URL JAVA6WSSERVICE_WSDL_LOCATION; 
privatefinalstatic Logger logger = Logger.getLogger(lavasoft.wsclient.Java6WSService.class.getName()); 
static { 
                String wsip = SysParamsToolkit.getProperty("wsip", "192.168.14.117"); 
                String wsport = SysParamsToolkit.getProperty("wsport", "8888"); 
                URL url = null; 
try { 
                        URL baseUrl; 
                        baseUrl = lavasoft.wsclient.Java6WSService.class.getResource("."); 
                        url = new URL(baseUrl, "http://" + wsip + ":" + wsport + "/java6ws/Java6WS?wsdl"); 
                } catch (MalformedURLException e) { 
                        logger.warning("Failed to create URL for the wsdl Location: '"+"http://"
 + wsip + ":" + wsport + "/java6ws/Java6WS?wsdl"+"', retrying as a local file"); 
                        logger.warning(e.getMessage()); 
                } 
                JAVA6WSSERVICE_WSDL_LOCATION = url; 
        } 
......
到此，大功告成了，你的客户端代码具有了可配置功能了。可以不再担心服务端如何部署了。
运行一把看看：
![](http://img1.51cto.com/attachment/200911/200911161258356730906.png)
由于客户端生成的代码超多，不能一一贴出，也没必要，详细源码请查看附件。
