# C#打印系统读取和写入XML配置文件 - Big Smile - CSDN博客
2017年11月12日 21:28:40[王啸tr1912](https://me.csdn.net/tr1912)阅读数：430
        还是那个打印系统的项目，在设计阶段，由于考虑到，需要把一些东西做活，需要用到一些自定义的，可以修改的配置文件，所以用到了XML这个很是方便的东西来进行一些关于配置的操作，下面是实现的过程。
# 一、配置设计
        由于是一个打印系统，并且涉及到了两个打印机的操作，所以对于打印机的操作，有着设定的操作，由于用的是windows的打印机，所以受到了默认打印机的限制，如果在打印的时候不指定打印机的话，他自动使用的是系统默认打印机来进行打印，所以对于打印机的配置是非常重要的。
        由于是打印系统，所以对于打印的模板，打印的边距，打印的纸张尺寸，打印的字号，打印的缓存图片等很多的东西都有需要配置的地方，所以这些都需要在配置文件中体现。具体布局：
![](https://img-blog.csdn.net/20171112215602644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、具体封装实现
        首先看我的配置XML结构（模板的太多，不展示）：
```
<?xml version="1.0" encoding="utf-8" ?>
<root>
  <!--打印设置-->
  <printerSettings>
    <!--小票POS机-->
    <posPrinter>GP-L80160 Series</posPrinter>
    <!--标签打印机-->
    <labelPrinter>Godex G500</labelPrinter>
  </printerSettings>
  <!--页面设置-->
  <pageSettings>
    <!--左边距-->
    <leftMargin>2</leftMargin>
    <!--上边距-->
    <topMargin>2</topMargin>
    <!--字号-->
    <fontSize>9</fontSize>
  </pageSettings>
  <!--系统设置-->
  <systemSettings>
    <!--订单刷新时间间隔/单位：秒-->
    <freshInternal>1</freshInternal>
  </systemSettings>
</root>
```
        可以看到这里我是分了三大块，一是打印机的设置，二是页面的设置，三是系统的设置，统统都在二级节点，然后我们通过直接获得配置项（三级节点）来获得我们需要的值，在设置的时候也是把配置项节点的数据直接进行修改。
        这里我为了读写方便，写了一个对于节点读写的helper，感觉方便了一些，这里仅供参考。
```
using System;
using System.Data;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using System.IO;
using System.Xml;
namespace echuUI
{
    public class XMLhelper
    {
        //获得配置的xml文件位置
       private static string xmlFile = System.Configuration.ConfigurationManager.AppSettings["setXMLfile"];
        /// <summary>
        /// 按节点插入打印机设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static void addPrinterSettings(string nodeName, string value) {
            AddStringXML("/root/printerSettings/" + nodeName.Trim(), value.Trim());
        }
        /// <summary>
        /// 按节点读取打印机设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static string readPrinterSettings(string nodeName)
        {
            return readStringXML("/root/printerSettings/" + nodeName.Trim());
        }
        /// <summary>
        /// 按节点插入页面设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static void addPageSettings(string nodeName, string value)
        {
            AddStringXML("/root/pageSettings/" + nodeName.Trim(), value.Trim());
        }
        /// <summary>
        /// 按节点读取页面设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static string readPageSettings(string nodeName)
        {
            return readStringXML("/root/pageSettings/" + nodeName.Trim());
        }
        /// <summary>
        /// 按节点插入系统设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static void addSystemSettings(string nodeName, string value)
        {
            AddStringXML("/root/systemSettings/" + nodeName.Trim(), value.Trim());
        }
        /// <summary>
        /// 按节点读取系统设置
        /// </summary>
        /// <param name="nodeName"></param>
        /// <param name="value"></param>
        public static string readSystemSettings(string nodeName)
        {
            return readStringXML("/root/systemSettings/" + nodeName.Trim());
        }
        /// <summary>
        /// 添加数据到xml文件制定节点
        /// </summary>
        /// <param name="xmlPath"></param>
        /// <param name="value"></param>
        private static void AddStringXML(string xmlPath, string value)
        {
            //初始化xml对象
            XmlDocument xmldoc = new XmlDocument();
            //加载配置文件
            xmldoc.Load(xmlFile);
            //获得根节点
            XmlNode root = xmldoc.DocumentElement;
            //获得要选择的节点
            XmlNode aimNode = root.SelectSingleNode(xmlPath);
            //写入节点数据
            aimNode.InnerText = value;
            //保存对象中的数据到文件
            xmldoc.Save(xmlFile);
        }
        /// <summary>
        /// 获得指定节点的数据
        /// </summary>
        /// <param name="xmlPath"></param>
        /// <returns></returns>
        private static string readStringXML(string xmlPath)
        {
            //初始化xml对象
            XmlDocument xmldoc = new XmlDocument();
            //加载配置文件
            xmldoc.Load(xmlFile);
            //获得根节点
            XmlNode root = xmldoc.DocumentElement;
            //获得要选择的节点
            XmlNode aimNode = root.SelectSingleNode(xmlPath);
            //返回节点值
            return aimNode.InnerText;
        }
    }
}
```
        这里可以再对于节点的访问和写入做一些抽象，由于时间的原因，我是直接引入了一些代码，有兴趣的同志可以试一试自己封装这种东西。
            
