# Java Mail 附件名太长导致接收端附件名解析出错 - z69183787的专栏 - CSDN博客
2018年02月02日 14:28:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2476
[http://blog.csdn.net/fl_zxf/article/details/60126910](http://blog.csdn.net/fl_zxf/article/details/60126910)
[http://blog.csdn.net/wty19/article/details/50607411](http://blog.csdn.net/wty19/article/details/50607411)
0x00（测试条件）
附件名：aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.txt
0x01（现象）
新浪邮箱解析出错：![](https://img-blog.csdn.net/20170303101625719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmxfenhm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
QQ邮箱解析出错：![](https://img-blog.csdn.net/20170303101639021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmxfenhm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
0x02（分析问题）
本地没问题，到 Linux 环境才出错。所以抓了本地的包和 Linux 下的包比较。
本地的包(之前用比较长的中文名测试抓的包)：
------=_Part_18_1324418920.1488440125843
Content-Type: application/octet-stream;
name="=?UTF-8?Q?=E4=B8=AD=E6=96=87=E5=AD=97=E7=AC=A6201711=2Edocx?="
Content-Transfer-Encoding: base64
Content-Disposition: attachment;
filename="=?UTF-8?Q?=E4=B8=AD=E6=96=87=E5=AD=97=E7=AC=A6201711=2Edocx?="
Linux 的包：
------=_Part_0_528597028.1488450122516
Content-Type: text/plain; charset=us-ascii;
name*0=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;
name*1=a.txt
Content-Transfer-Encoding: 7bit
Content-Disposition: attachment;
filename*0=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;
filename*1=a.txt
可以看到问题是文件名太长被拆分了
0x03（查找根源）
看 Java 代码
String encodeName = MimeUtility.encodeWord(name);
UrlResource inputStreamSource = new UrlResource(attach.getUrl());
helper.addAttachment(encodeName, inputStreamSource); // 自己的业务代码addAttachment(attachmentFilename, inputStreamSource, contentType);// 跟进去addAttachment(attachmentFilename, dataSource);// 跟进去
mimeBodyPart.setFileName(MimeUtility.encodeText(attachmentFilename)); // 跟进去setFileName(this, filename);// 跟进去part.setHeader("Content-Disposition", cd.toString()); // 跟进去sb.append(list.toString(sb.length() + 21));// 跟进去
if (value.length() > 60 &&
      splitLongParameters && encodeParameters) {
    int seg = 0;
name += "*"; // 省略...
已经看出是怎么回事了
0x04（解决）
在 Main方法下加
System.setProperty("mail.mime.splitlongparameters", "false"); // linux 会默认为 true，会截断附件名
0x05（PS）
这是 RFC2231 的规定，估计国内的还不支持(个人猜想)
0x06(走过的弯路)
之前没头绪。以为是 Linux 的限制；也怀疑过“MimeUtility.encodeWord(name)”的问题。
跟踪源码的时候看错了 jar 包，应该是 com.sun.mail:javax.mail:1.5.6，还不是 javax.mail:mail:1.4.5
------------  记一个JavaMail
 附件乱码的问题
     说到乱码，很多人都遇到过，“哎呀，你这个编码是不是UTF-8！”，“你这个会不会操作系统不一致导致的？”，“肯定是两边编码不一致”。不过我们今天说的问题，还真不是这个问题导致的。
**问题**
   用JavaMail 发邮件，带上附件，闪电邮客户端收到后，附件名有时乱码，有时非乱码。查看Java端代码：
```java
String name = MimeUtility.encodeText(name, null) + ".xlsx";
		messageBodyPart.setFileName(name);
```
貌似已经做过编码转换了。
令人感到奇怪的是，稍微改变附件名，乱码就消失了。
**发现**
   收集各种信息的时候，突然发现 Ubuntu的小伙伴 在 ThunderBird下收邮件表示没有异常，Mac党表示自带的邮件工具收件也没有问题。那是不是操作系统字符编码问题呢？ 可是遗憾的是，在网页版上，依旧是乱码。 这时候初步怀疑是邮件系统不兼容的问题了，来看邮件源码：
```java
Content-Type: application/octet-stream; 
	name*0="=?utf-8?B?5rWL6K+V5qCH6aKYLS0tMDAx5oiR6KaB5LiK?==?utf-8?B?5"; 
	name*1="a2mQUJDREXvvIzlkKzor7TopoHotrPlpJ/plb8=?=.xlsx"
Content-Transfer-Encoding: base64
Content-Disposition: attachment; 
	filename*0="?utf-8?B?5rWL6K+V5qCH6aKYLS0tMDAx5oiR6KaB5LiK?==?utf-8?B?5"; 
	filename*1="a2mQUJDREXvvIzlkKzor7TopoHotrPlpJ/plb8=?=.xlsx"
```
这串就是 有些系统乱码有些系统 正常显示的 邮件源码。。
对比在邮件客户端上的非乱码邮件：
```java
```
```
Content-Type: application/octet-stream; name="=?utf-8?B?5rWL6K+V?=.xlsx"
Content-Transfer-Encoding: base64
Content-Disposition: attachment; filename="=?utf-8?B?5rWL6K+V?=.xlsx"
```
 filename*0，filename*1 和 filename 的区别，猜测也许就是邮件客户端不支持这种filename*0，filename*1 协议导致的问题。
**分析**
  有了以上的想法，就开始来看源码。
   MimeBodyPart 这个类中的 setFileName 方法 用到一个 ParameterList 在ParameterList 的 toString 类中找到下面一段：
```java
if (v instanceof MultiValue) {
		// ....
			ns = name + i + "*";
		//...
		}
	    } else if (v instanceof Value) {
		/// ...
	    } else {
		
		if (value.length() > 60 &&
				splitLongParameters && encodeParameters) {
		    int seg = 0;
		    name += "*";
		    /// ....
		}
```
这个类在邮件附件属于 MultiValue 会把 名字用name + i 隔开 ，在名字大于 60个字符的时候也会主动截断，这也就是 javamail 中 附件的命名规则，**名字太长会被截断**～～！
中文在base64 加密后，超过60个字符那是妥妥的有可能。这种截断文件名的模式在某些客户端，并不能很好的支持。
**解决**
  解决就很容易了， 代码里有 splitLongParameters 这个参数， 观察了下 对应于一个环境变量，如果想不截断文件名，只要在程序运行之初加上：
```java
System.setProperty("mail.mime.splitlongparameters","false");
```
  就可以了。测试，解决。。。
  另外由于发现了这个问题，google到了 java mail 的完整配置，
[JAVA Mail System 环境变量](http://docs.oracle.com/javaee/6/api/javax/mail/internet/package-summary.html)  包括是否自动编码等配置，曾经没怎么关注过。
**总结**
   邮件系统的不兼容是导致这个错误的根本原因，还真不是编码问题，所以有的时候看问题还不能那么想当然。这个测试未必能测出，毕竟要满足名字足够长这个条件。在这里分享这个问题，以免JAVA党同学重复踩坑

