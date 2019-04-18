# So far as I know -- Part I: Sending a wap push message via SMPP/CMPP（上） - weixin_33985507的博客 - CSDN博客
2005年01月19日 12:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
最近一个字——忙啊……
最近我有4件事情可以忙：1、被某公司借调去帮忙完成一个POC项目；2、准备翻译某本书，先要做试译；3、继续关心公司的项目；4、准备回家。今天只聊技术，所以只准备说说第一件事情。
在最近帮忙做的POC项目里面，我负责的是mobile方面的事情，原因很简单：1、我们公司现在做的就是smart phone上面.net cf开发的；2、我在这方面做了很长时间的工作了；3、JGTM觉得这对于我来说也是一个锻炼机会（这也是我的一个要求）。于是我就来到这个POC的项目组里面了。
其实在来之前我是非常的担心的，因为我听说我要做的实际上是在PC上面发送wappush包一个模块，以及一些给PPC - Phone Edition看的ASP.NET的页面。关于wappush我以前根本就没有接触过，关于ASP.NET也没有什么经验，可以说到博客源里面随便抓一个也不会比我差到哪里去。不过我是认为接收新的挑战是一件很好的事情，可以让自己成长，再加上JGTM答应在后面做技术支持，于是就满口答应下来了。
这个项目已经进行了一个多礼拜了，至少我已经在这个项目组工作了这么长的时间了，在这段时间里面不敢说学到了多少东西，但至少经历了不少。比如说在没有足够的资料下，编写访问移动网关发送wappush数据包的事情就足够让人头痛了。在google上面搜索wap push加上其他关键字，能够搜索到的真正有用的中文资料为数不多，其中最有用的要数[这一个](http://www.spzone.net/bbs/topic.asp?topic_id=6415&forum_id=3)了。即使是那个“[最有用的](http://www.spzone.net/bbs/topic.asp?topic_id=6415&forum_id=3)”也只是一个example，没有太多的解释，如果成功了还好，不成功的话就会让人抓狂的。不幸的是，我遇到的情况是“既成功也不成功，一会儿成功一会儿不成功，这个手机成功那个手机不成功，这个Sim卡成功那个Sim卡不成功……”。英文资料也不会有太多有用的，很奇怪为什么没有比较新的Specification，也许是我没有搜索到吧。如果你也是做Wap push网关发送程序的，可以在Google上面搜索“WAP Push OTA Protocol Specification”，以及“WBXML Specification”，就可以找到一些相对有价值的资料了，并且这些文档里面还会连接到其他的Specification，可以帮助你理解整个的wap push方面的相关知识。如果你想对Wap push有一个快速的了解，那么我下面的简单介绍也许会有点帮助。
其实Wap push的发送应该有两种途径，一个是通过PAP协议，另外一个是通过SMPP协议。其中SMPP是一个基本协议，在中国主要有三个派生的协议：中国移动的CMPP协议，中国联通的SGIP（在CDMA上面好像是ETIP），以及小灵通的SMGP。据说中国移动没有在公网开放PAP协议，于是只好使用CMPP协议来进行wap push发送了，不知道其他的运营商是否也是如此。上面提到的各种协议都是公开的，通过Google搜索就可以得到。光得到协议没有什么意义，因为你还需要知道wap push网关的IP和Port，以及运营商给你发放的帐号和密码。而最为头痛的是，你研究下载下来的协议的时候，至少会遇到下列几个大问题：
1、协议可能不正确
2、协议内容语焉不详，或者具有二义性
3、没有任何实际例子
如果你能够进入运营商的实验室会更好，因为可以得到一些技术支持。不过也不要开心的太早，你还会遇到下列问题：
1、运营商并不清楚协议的技术细节，因为实际上是由运营商的硬件提供商所代为开发的（至少我看起来的是这样的）
2、硬件提供商的技术人员也不一定清楚非常深入的技术细节，天晓得到底谁才知道这些该死的协议的技术细节。
3、在实验室里面的技术人员至少可以帮你抓一个包，指出你的包跟标准的包有什么区别。如果没有大的区别的时候仍然出错，那技术人员也无能为力了。
在开发Wap push的时候跟实验室的技术人员聊天，据说以前其他的人员连接上去的时候，还曾经遇到过MD5算法不标准导致验证不通过的问题。幸亏我用的.NET，不会出现这种问题，不然验证不通过你还不知道是验证的明文格式不对，还是其他的什么问题，总之是不可能进行调试的。而我遇到的问题则主要是文档迷惑人造成的，比如说协议里面有一个字段叫做SourceAddress，实际上要求你填入的是SpID，而SourceID的字段却要求你填入SourceAddress。看完那堆文档我的大脑几乎要被搅成浆糊了，希望你会好一点。
实际上这个CMPP的协议是用来发送短消息的，包括普通的短消息、彩信以及WapPush等。具体你要发送什么类型的消息，需要设置MessageType字段和MessageContent字段。因此如果你要发送的事彩信，那么还得找跟彩信相关的资料，尤其是数据包结构。而我则非常郁闷的发现，关于WapPush数据包的资料非常的少，少的不可置信。前面那个中文资料算是帮了我一把，对比了实验室抓取的另外一个数据包之后，至少看懂了部分的内容，并且某个品牌的手机能够收到。可是里面提到的WapPush Header1之类的内容到底是什么意思，以及如何编写其他标记等，统统不知道。
Goo了半天才搞懂，原来WapPush包发送的内容实际上跟PAP发送的XML是一回事，但是经过了压缩。压缩之后的格式叫做WBXML，这种格式将一些标记用代码来表示，例如数值0x01表示上一个标记结束（</xxx>之类的）。然而WBXML的缩略标记分为两部分，一部分是所有类型的XML都通用的，另一部分是不同类型的XML有着不同的解释。而WBXML的Spec只提到了：
2  "-//WAPFORUM//DTD WML 1.0//EN" (WML 1.0) 
3  "-//WAPFORUM//DTD WTA 1.0//EN" (WTA Event 1.0) 
4  "-//WAPFORUM//DTD WML 1.1//EN" (WML 1.1) 
这三种类型，而我们所做的WapPush使用的是
5  "-//WAPFORUM//DTD SI 1.0//EN
关于这里面的内容我没有能够找到任何资料。即使到了这里，我们也只是了解了Header后面的内容，而MessageContent里面的内容则需要从
0B05040B8423F00003030101         ’Wap Push Header 1
29060603AE81EA8DCA   'Wap Push header 2
这里开始发送，但是这些是什么东东呢？我不知道，等高手来解答了。不过在这里我可以提供另外一组有效的Wap Push Header:
//    0x06, 0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0,     ' Wap Push Header 1
//    0x25, 0x06, 0x01, 0xAE,                                     ' Wap Push header 2
另外再奉送一下我实验成功的（至少在索爱T618上面必然成功的）一个MessageContent结构：
// 第一部分 WapPushHeader1：
  static private readonly byte[] WapPushHeader1 = new byte[]
   {
//    0x06, 0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0,
    0x0B, 0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0, 0x00, 0x03, 0x03, 0x01, 0x01,
   };
// 第二部分 WapPushHeader2：
  static private readonly byte[] WapPushHeader2 = new byte[]
   {
//    0x25, 0x06, 0x01, 0xAE,
    0x29, 0x06, 0x06, 0x03, 0xAE, 0x81, 0xEA, 0x8D, 0xCA,
   };
// 第三部分 WapPushIndicator (and some other things)：
  static private readonly byte[] WapPushIndicator = new byte[]
   {
    0x02, 0x05, 0x6A, 0x00, 0x45, 0xC6, 0x0C, 0x03,
   };
// 第四部分是一个UTF8编码的WapPush连接的Url地址，
// 用户在手机上收到该短信之后点击“下载”或者“连接”之类的按钮之后，
// 就会转到这一个地址。
// 注意，Url地址需要把"http://"去掉，
// 因为上面的WapPushIndicator倒数第二个0x0C就是href = "http://"
// 第五部分Wap Push Display Text Header：
// 其中的0x00是上面的WapPushUrl文字结束标志
// 0x01是上面的<indication>标志（Indicator倒数第三个的0xC6）的结束符
// </indication>
  static private readonly byte[] WapPushDisplayTextHeader = new byte[]
   {
    0x00, 0x01, 0x03,
   };
// 第六部分是一个UTF-8编码的消息文字，用户收到该消息之后就会显示这里面的内容。
// 第七部分是消息结束部分，0x00是消息文字的结束部分，后面两个是上面一些标志的结束符。
  static private readonly byte[] EndOfWapPush = new byte[]
   {
    0x00, 0x01, 0x01,
   };
将这几个部分顺序组合起来，就是MessageContent的内容了。
