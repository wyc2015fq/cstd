# C++、C#写的WebService相互调用 - 深之JohnChen的专栏 - CSDN博客

2012年01月05日 09:51:41[byxdaz](https://me.csdn.net/byxdaz)阅读数：2752


首先感谢永和兄提供C++的WebService服务器端及客户端，并且陪我一起熬夜；然后是火石和我做接口的兄弟，虽然都不知道你叫什么，如果没有你的合作，东西也没那么快完成。

一、由于公司运营火石的《西游Q记》，火石采用的是C++作为开发语言，Unix平台，而我们一直使用Windows操作平台，.NET快速开发。我们之间需要数据的通讯，所以需要利用WebService实现跨平台的数据通讯。尽管WebService是跨平台的，但是实现起来却并不容易。

二、用C#实现WebService是相当简单的事情，我们只要创建一个Web服务程序，在方法名上面加上[WebMethod]，部署到IIS上，就能像访问Web站点一样访问WebService。用C#编写客户端时，只需要将WebService添加到引用，就能像调用本地方法一样去调用WebService。像这样的例子也比比皆是，在这就不多讲。

三、用C++实现WebService，一般会用到gsoap，具体方法见：[http://www.cppblog.com/qiujian5628/archive/2008/06/19/54019.html](http://www.cppblog.com/qiujian5628/archive/2008/06/19/54019.html)

四、当做完了这些之后，并不代表WebService就能相互通讯了，现在我简单列举一下问题：

1、C#提供的WebService的URL一般形如：[http://localhost/WebService.asmx](http://localhost/WebService.asmx)，但是，C++能提供的只能是：[http://localhost/](http://localhost/)。C++做客户端的时候调用没有问题，但是当C#做客户端的时候，引用C++提供的RUL时，会提示没用执行方法（**HTTP GET method not implemented**）。做C#开发的大部分会认为C++方提供的不是WebService，或者说提供的WebService根本就不全，都不带.asmx文件。做C++开发的会认为他传输的数据符合soap协议，靠http传输数据，他就是WebService。

2、当我们解决了第一步后，紧接着会发现另外一个问题。当我们需要传输自定义数据类型时（在C++中称结构体，在C#中称实体），从C++返回的信息中，C#无法构建出实体类。

3、当传输的信息中带有中文字符时，乱码满天飞。

五、为了解决这些问题，我们先简单了解一下WebService。

Web Service互操作协议栈：

<A>、服务发现 (UDDI)

<B>、服务描述（WSDL）

<C>、服务调用(SOAP)

<D>、消息编码 (XML)

<E>、传输网络层(HTTP, TCP/IP)

其中WSDL描述WebService都有什么方法、方法有什么参数，什么返回值等。SOAP（简单对象访问协议(Simple Object Access Protocol）是一种轻量的、简单的、基于XML的协议。传输的数据就需要遵循这个协议。我比较简单得认为传输的数据需要遵循这种格式。

借用微软的这个图描述下WebService的调用过程：

![C++、C#写的WebService相互调用](http://s12.sinaimg.cn/bmiddle/4e7d3826t5386042cd42b)

六、开始解决问题。作为.NET开发人员，我们根本就接触不到底层的东西，全被封装了。

C++做的确实是WebService，只是他们需要给提供一个描述文档，即.WSDL文件。使用.NET提供的wsdl.exe工具，使用命令：wsdl /o: c:\webservice.cs c:\webservice.wsdl。通过webservice.wsdl文档，生成代理类，将代理类写入webservice.cs文件中。我们拷贝这个cs文件到项目中，将URL指向

[http://localhost/](http://localhost/)，就能像以往那样使用WebService了。

当出现无法传递复杂类型数据时，是因为使用gsoap生成的wsdl文件与.Net中生成的wsdl文件不一样。具体代码如下：

<!-- operation response element -->
  <element name="result">
   <complexType>
    <sequence>
     <element name="a" type="xsd:int" minOccurs="1" maxOccurs="1"/>
     <element name="b" type="xsd:int" minOccurs="1" maxOccurs="1"/>
    </sequence>
   </complexType>
  </element>

以上为gsoap生成的。返回实体result，实体有两个属性：a，b。

<s:element name="TestResponse">
    <s:complexType>
     <s:sequence>
      <s:element minOccurs="0" maxOccurs="1" name="TestResult" type="tns:result" />
     </s:sequence>
    </s:complexType>
   </s:element>
   <s:complexType name="result">
    <s:sequence>
     <s:element minOccurs="1" maxOccurs="1" name="a" type="s:int" />
     <s:element minOccurs="1" maxOccurs="1" name="b" type="s:int" />
    </s:sequence>
   </s:complexType>

以上是.NET生成的。

在下面的文件中，多出

<s:element name="TestResponse">
    <s:complexType>
     <s:sequence>
      <s:element minOccurs="0" maxOccurs="1" name="TestResult" type="tns:result" />
     </s:sequence>
    </s:complexType>
   </s:element>
这个便是.NET中用来构造实体的。当我们出现情况4.2时，gsoap中尽量使用.NET生成的wsdl文档，生成.h文件，以避免C++中的结构无法在C#中转换成实体。

第三个问题，我们是通过将中文转换成16进制后传输过来，然后再转换成中文。下面提供C#转换的代码：

/// <summary>
        /// 从16进制转换成汉字
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        public static string GetChsFromHex(string hex)
        {
            if (hex == null)
                throw new ArgumentNullException("hex");
            if (hex.Length % 2 != 0)
            {
                hex += "20";//空格
                //throw new ArgumentException("hex is not a valid number!", "hex");
            }
            // 需要将 hex 转换成 byte 数组。
            byte[] bytes = new byte[hex.Length / 2];

for (int i = 0; i < bytes.Length; i++)
            {
                try
                {
                    // 每两个字符是一个 byte。
                    bytes[i] = byte.Parse(hex.Substring(i * 2, 2),
                        System.Globalization.NumberStyles.HexNumber);
                }
                catch
                {
                    // Rethrow an exception with custom message.
                    throw new ArgumentException("hex is not a valid hex number!", "hex");
                }
            }

// 获得 GB2312，Chinese Simplified。
            System.Text.Encoding chs = System.Text.Encoding.GetEncoding("gb2312");

            return chs.GetString(bytes);
        }

/// <summary>
        /// 从汉字转换到16进制
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        public static string GetHexFromChs(string s)
        {
            if ((s.Length % 2) != 0)
            {
                s += " ";//空格
                //throw new ArgumentException("s is not valid chinese string!");
            }

System.Text.Encoding chs = System.Text.Encoding.GetEncoding("gb2312");

byte[] bytes = chs.GetBytes(s);

string str = "";

for (int i = 0; i < bytes.Length; i++)
            {
                str += string.Format("{0:X}", bytes[i]);
            }

return str;
        }

注：以上来转换代码源于网络，C++中转换的代码也可以在网上找到。

三大难题到此结束，其实在整个过程中还有个最大的难题，那就是人与人的交流。因为一方使用C++，一方使用C#，语言不同，各自想问题的方式也不一样，所以需要相互理解，相互站在对方的角度想问题。多交流、多沟通才是解决问题之道。请不要抱怨C#弱智，也请不要怪C++繁琐，语言既然存在则有他的价值。

