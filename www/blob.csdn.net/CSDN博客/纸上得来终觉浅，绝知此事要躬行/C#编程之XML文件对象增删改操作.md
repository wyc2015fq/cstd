# C#编程之XML文件对象增删改操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月30日 19:26:53[boonya](https://me.csdn.net/boonya)阅读数：1416








 在windows mobile开发中，网络不畅的情况下数据可以暂存在XML文件中，等网络通畅时再进行数据库同步传输。但是，此种方式处理本地存储弊端很大：XML文件中对象在不断增多的过程中，程序需要遍历对象的次数也在增多，所以不推荐使用XML作为本地暂存方案；推荐使用sdf本地数据库来实现少量数据的本地操作。

## 1、创建C#控制台应用程序

 此时可以同时生成其解决方案，如我的项目在：D:\VS_PROGRAM_PROJECT\ConsoleAppXMLoperate，项目名称为ConsoleAppXMLoperate。

![](https://img-blog.csdn.net/20131130191914781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 2、在主程序中编写XML操作代码



```
using System;
using System.Collections.Generic;
using System.Xml.Schema;
using System.Text;

namespace ConsoleAppXMLoperate
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("yyyy-MM-dd HH:mm:ss"  + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            //Program.insertNode();
            //Program.updateNode();
            Program.deleteNode();
        }

        /**
         * 添加节点信息
         * 
         * 往<bookstore>节点中插入一个<book>节点
         */
        public static void insertNode() {
            System.Xml.XmlDocument xmlDoc = new System.Xml.XmlDocument();
            xmlDoc.Load("bookstore.xml");
            //查找<bookstore>
            System.Xml.XmlNode root = xmlDoc.SelectSingleNode("bookstore");
            //创建一个<book>节点
            System.Xml.XmlElement book = xmlDoc.CreateElement("book");
            //设置该节点genre属性
            book.SetAttribute("genre", "boonya");
            //设置该节点ISBN属性
            book.SetAttribute("ISBN", "2-3631-4");

            System.Xml.XmlElement bookNode1 = xmlDoc.CreateElement("title");
            //设置文本节点
            bookNode1.InnerText = "Java";
            //添加到<book>节点中
            book.AppendChild(bookNode1);
            System.Xml.XmlElement bookNode2 = xmlDoc.CreateElement("author");
            bookNode2.InnerText = "Simith Talent";
            book.AppendChild(bookNode2);
            System.Xml.XmlElement bookNode3 = xmlDoc.CreateElement("price");
            bookNode3.InnerText = "58.3";
            book.AppendChild(bookNode3);

            //添加到<bookstore>节点中
            root.AppendChild(book);
            xmlDoc.Save("bookstore.xml");
        }

        /**
         * 修改节点信息
         * 
         * 将genre属性值为“bonnya“的节点的genre值改为“updateboonya”，将该节点的子节点<author>的文本修改为“boonya”。
         */
        public static void updateNode() {
            System.Xml.XmlDocument xmlDoc = new System.Xml.XmlDocument();
            xmlDoc.Load("bookstore.xml");
            //获取bookstore节点的所有子节点
            System.Xml.XmlNodeList nodeList = xmlDoc.SelectSingleNode("bookstore").ChildNodes;
            foreach (System.Xml.XmlNode xn in nodeList)//遍历所有子节点
            {
                //将子节点类型转换为XmlElement类型
                System.Xml.XmlElement xe = (System.Xml.XmlElement)xn;
                //如果genre属性值为“bonnya”,则修改该属性为“updatebonnya”
                if (xe.GetAttribute("genre").Equals("bonnya"))
                {
                    xe.SetAttribute("genre", "updatebonnya");
                    System.Xml.XmlNodeList nls = xe.ChildNodes;
                    //继续遍历获取xe子节点的所有子节点
                    foreach (System.Xml.XmlNode xn1 in nls)
                    {
                        System.Xml.XmlElement xe2 = (System.Xml.XmlElement)xn1;
                        if (xe2.Name == "author")
                        {
                            xe2.InnerText = "boonya";
                            break;
                        }
                    }
                    break;
                }
            }
            xmlDoc.Save("bookstore.xml");
        }

        /**
         * 删除节点信息
         * 
         * 删除 <book genre="bonnya" ISBN="2-3631-4">节点。
         */
        public static void deleteNode() {
            System.Xml.XmlDocument xmlDoc = new System.Xml.XmlDocument();
            xmlDoc.Load("bookstore.xml");

            System.Xml.XmlNodeList xnl = xmlDoc.SelectSingleNode("bookstore").ChildNodes;

            foreach (System.Xml.XmlNode xn in xnl)
            {
                System.Xml.XmlElement xe = (System.Xml.XmlElement)xn;
                if (xe.GetAttribute("genre") == "boonya")
                {
                    //删除book对象
                    xe.ParentNode.RemoveChild(xe);
                }
            }
            xmlDoc.Save("bookstore.xml");
        }
    }
}
```



## 3、添加XML文件



在项目路径的bin\Debug下添加你的XML文件，如下图：

![](https://img-blog.csdn.net/20131130192217453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

bookstore.xml文件的内容如下：



```
<?xml version="1.0" encoding="utf-8"?>
<bookstore>
  <book ISBN="1-3631-4">
    <title>Oberon's Legacy</title>
    <author>Corets, Eva</author>
    <price>5.95</price>
  </book>
</bookstore>
```



## 4、测试程序

依次打开main函数的方法进行测试，也可以全部打开在里面设置断点调试就行。



```
static void Main(string[] args)
        {
            Console.WriteLine("yyyy-MM-dd HH:mm:ss"  + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
            //Program.insertNode();
            //Program.updateNode();
            Program.deleteNode();
        }
```
资源下载地址：[http://download.csdn.net/detail/boonya/6636299](http://download.csdn.net/detail/boonya/6636299)






