# 在IntelliJ IDEA上将WSDL生成Java代码 - z69183787的专栏 - CSDN博客
2016年12月06日 13:25:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3635
> 
转自：http://blog.csdn.net/yishichangan1/article/details/51861970
最近接触了一个android项目，但是需要自己用webService的方法获取数据。而且只给了一个wsdl的地址，今天介绍如何利用wsdl生成Java客户端并且进行测试。
## 一、安装一个IntelliJ IDEA
- Windows平台(自行选择x86和x64) 
点击：[https://www.jetbrains.com/idea/download/#section=windows](https://www.jetbrains.com/idea/download/#section=windows)
- OSX平台， 
点击:[https://www.jetbrains.com/idea/download/#](https://www.jetbrains.com/idea/download/#)
## 二、安装JDK
没有安装的，请点击下载：[http://pan.baidu.com/s/1qYRCDha](http://pan.baidu.com/s/1qYRCDha)
如果想顺便装一下jre的请点击下载：[http://pan.baidu.com/s/1dFe9ZxB](http://pan.baidu.com/s/1dFe9ZxB)
建议将JDK与JRE安装在同一个的根目录的不同子目录下因为这样方便寻找，而且，装在同一个文件下会出错。 
安装以后，进行环境配置，这里不再赘述。如果没有写过Java程序不会安装和配置，请点击查看教程： 
[http://jingyan.baidu.com/article/6dad5075d1dc40a123e36ea3.html](http://jingyan.baidu.com/article/6dad5075d1dc40a123e36ea3.html)
## 三、IDEA新建一个project
- 
File ->New->Project 
出现以下界面： 
![这里写图片描述](https://img-blog.csdn.net/20160708154951490)
- 
选择左边的Java选项，右边会跳出Java的相关设置： 
导入你的JDK，然后点击NEXT： 
![这里写图片描述](https://img-blog.csdn.net/20160708155051773)
- 
选择一个Hello的最简单的Java项目 
![这里写图片描述](https://img-blog.csdn.net/20160708155209290)
- 
填写项目名称，并点击Finish 
![这里写图片描述](https://img-blog.csdn.net/20160708155315135)
完成以后，在IDEA的左边就会显示你新建的Java项目 
![这里写图片描述](https://img-blog.csdn.net/20160708155425796)
- 
点击你的项目名称并且右键找到WebService->Gernerate Java Code From Wsdl 
![这里写图片描述](https://img-blog.csdn.net/20160708155728610)
- 
web Service wsdl url 输入wsdl的地址以搜索世界天气的wsdl地址为例（[http://www.webservicex.com/globalweather.asmx?WSDL](http://www.webservicex.com/globalweather.asmx?WSDL)） 
package prefix是包名，表示会在/src的文件夹下，新建一个你输入的包名，然后将所有生成的代码放在这个文件夹下。 
webService platform是一个协议解析工具，我选的是IDEA自带的不用下载的JAX-WS 2.2，如果你之前接触过其他的比如CXF等也都可以选择。(Attention:不同的协议生成的Java代码会不一样，调用的函数也会不一样，但是原理是一样的) 
![这里写图片描述](https://img-blog.csdn.net/20160708160310133)
![这里写图片描述](https://img-blog.csdn.net/20160708160325285)
- 
这是他生成的所有的类 
![这里写图片描述](https://img-blog.csdn.net/20160708160724630)
- 
这是他生成的所有class文件 
![这里写图片描述](https://img-blog.csdn.net/20160708160829912)
- 
iml文件和asmx文件 
![这里写图片描述](https://img-blog.csdn.net/20160708161009272)
![这里写图片描述](https://img-blog.csdn.net/20160708161033831)
有的时候还会生成一个wsdl文件
- 客户端入口程序 
![这里写图片描述](https://img-blog.csdn.net/20160708161133444)
测试代码
```
import lqy.GlobalWeather;
import lqy.GlobalWeatherSoap;
public class Main {
    public static void main(String[] args) {
        //System.out.println("Hello World!");
        //查询上海的天气
        GlobalWeather service = new GlobalWeather();
        GlobalWeatherSoap port = service.getGlobalWeatherSoap();
        String body = port.getWeather("shanghai", "China");
        System.out.println(body);
        System.out.println();
        //查找中国的所有城市
        GlobalWeather service1 = new GlobalWeather();
        GlobalWeatherSoap port1= service1.getGlobalWeatherSoap();
        String country=port1.getCitiesByCountry("China");
        System.out.println(country);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
测试样例输出
1.上海的天气
![这里写图片描述](https://img-blog.csdn.net/20160708161455477)
2.中国的城市（太多了下面就不截图了） 
![这里写图片描述](https://img-blog.csdn.net/20160708161604510)
