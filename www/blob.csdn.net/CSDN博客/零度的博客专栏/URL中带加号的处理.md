# URL中带加号的处理 - 零度的博客专栏 - CSDN博客
2016年05月20日 17:41:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：1524
问题起因：  
    客户订购了一关键字为"e+h 变送器" ， 在首页推荐广告中，会根据用户在search 搜索过的关键字进行一个匹配投放。技术实现是UED 通过JS 获取cookie 中的h_keys 内容，拼装到 [http://xxxxx/advert/ctp_advert.htm?num=4&keyword=](http://small.china.alibaba.com/advert/ctp_advert.htm?num=4&keyword=) {keyword} 。
 这里取出来对应的cookie 信息为中文，最后通过一个ajax 发起一个ＧＥＴ请求。
    所以针对最后的请求是：[http://xxxxxx/advert/ctp_advert.htm?num=](http://small.china.alibaba.com/advert/ctp_advert.htm?num=) 4&keyword=e+h 变送器。 而在服务端接受到对应的请求参数时，发现参数为：e h 变送器， + 号没了。　初步怀疑跟ＵＲＬ规范相关，需要进行url
 encode。
问题分析： 
    查了下JS encode 的相关内容，　总于发现+ 号的秘密。
   html中因为一些非标准的做法，将+ 等同于空格进行处理 （当Html 的表单被提交时， 每个表单域都会被Url 编码之后才在被发送。由于历史的原因，表单使用的Url 编码实现并不符合最新的标准。例如对于空格使用 的编码并不是%20 ，而是+ 号，如果表单使用的是Post 方法提交的，我们可以在HTTP 头中看到有一个Content-Type 的header ，值为 application/x-www-form-urlencoded ，大部分应用程序均能处理这种非标准实现的Url 编码）。
    在搜索引擎中做了下尝试：　
    keyword =  e h 变送器  , url = [http: //www.google.cn/search?hl=zh-CN&newwindow=1&q=e+h变送器](http://www.google.cn/search?hl=zh-CN&newwindow=1&q=e+h%B1%E4%CB%CD%C6%F7)   ( 空格被转化为+ 号)
    keyword = e+ h 变送器 , url = [http: //www.google.cn/search?hl=zh-CN&newwindow=1&q=e%2Bh变送器](http://www.google.cn/search?hl=zh-CN&newwindow=1&q=e%2Bh%B1%E4%CB%CD%C6%F7)  (+ 号被进行了转义为%2B ，程序才能正常处理)
问题解决：
思路1:
    1.  要想正常传输+ 号而不被转义为空格，需要进行进行编码为%2B 。查了下几个编码函数，发现只有encodeURIComponent 才会对+ 号进行编码处理。
    2. encodeURIComponent 默认为采用UTF-8 字符集，理论上只需要在原先的请求中添加_input_charset=utf-8(由 pipeline 中的SetLocaleValve 进行解析) ，就可以得到正确的 e+h 变送器。 
    在实施过程中，发现结果并不是预期的那样。 客户端通过js encode 后，在服务端解析后一直是乱码。 查了下byte ，发现服务端一直是用GBK 在进行解析， 针对变送器的UTF-8 编码的byte 为{-27,-113,-104,-23,-128,-127} ，客户端用GBK 解析后变为{-27.-113.- 104.-23,-63,-63} ，针对最后两byte 因为字符不可见，导致全部被替换为-63 。网上查了下，针对 utf-8 -> gbk -> utf-8 在一定情况下就会出现该问题([http://lingqi1818.iteye.com/blog/348953](http://lingqi1818.iteye.com/blog/348953) ) 。
思路2 ： 
    继续追查对应的_input_charset=utf-8 未生效的原因，DEBUG 看到在SetLocaleValve 中的确设置了request.setCharsetEncoding 为utf-8 。初步怀疑是否跟jboss server 的配置有关，查了下跟URIEncoding 和useBodyEncodingForURI 设置有关。 目前公司所使用的jboss 为4.05 ，对应俄tomact 配置中只指定了对应的URIEncoding=GBK 。正因为这样，导致设置的_input_charset 针对GBK 的提交没有效果 ，还是按照GBK 进行解析。
    1.  考虑将请求由GET 换成POST ， 这样就可以使用_input_charset
    但在实施过程中，和UED 沟通过程，针对POST 的会引起一个跨域请求的问题。此方案又只能做罢
思路3 (实践成功) ： 
    1.  UED 进行伪url encode 的实现 , 将+ 号进行%2B 的编码。 因为目前JS 中没有现成的函数，这里只是通过replace(/\+/g, '%2B') 进行了转化。
总 结
针对+ 号的处理，针对不同的业务场景需要不同的处理方案，描述下几种场景:
1. 非Ajax 请求
    可以直接使用Form 表单的 GET ,POST 的urlencode 协议，自动实现+ => %2B 的转化
2.  Ajax 请求
    * GET 请求 : 很无奈，只能使用方案3 ，人为进行+ 号转化。
    * POST 请求( 同一应用，非跨域请 求) :  使用encodeURIComponent +  _input_charset=utf-8 指定编码进行处理。
ps: 前面提的这几种方案，都是基于+ 号是正常的业务场景进行考虑。同时我们也可以从业务层面进行一个梳理，+ 号处理是否有其必要性，能从业务数据入口直接规避 那就最好了。 
背景知识：
URIEncoding 和useBodyEncodingForURI
    对于URL 提交的数据和表单中GET 方式提交的数据，在接收数据的JSP 中设置request.setCharacterEncoding参数是不行的， 因为在Tomcat5.0 中，默认情况下使用ISO- 8859-1 对URL 提交的数据和表单中GET 方式提交的数据进行重新编码（解码），而不使用该参数对URL 提交的数据和表单中GET 方式提交的数据进行 重新编码（解码）。要解决该问题，应该在Tomcat 的配置文件的Connector 标签中设置useBodyEncodingForURI 或者URIEncoding 属性，其中useBodyEncodingForURI 参数表示是否用 request.setCharacterEncoding 参数对URL提交的数据和表单中GET 方式提交的数据进行重新编码 ，在默认情
 况下，该参数为false （Tomcat4.0 中该参数默认为true ）； URIEncoding参数 指定对所有GET 方式请求（包括URL 提交的数据和表单中GET 方式提交的数据）进行统一的重新编码（解码）的编码 。 URIEncoding 和useBodyEncodingForURI 区别是，URIEncoding 是对所有GET 方式的请求的数据进行统一的重新编码
 （解码），而useBodyEncodingForURI 则是根据响应该请求的页面的request.setCharacterEncoding 参数对数 据进行的重新编码（解码），不同的页面可以有不同的重新编码（解码）的编码。所以对于URL 提交的数据和表单中GET 方式提交的数据，可以修改 URIEncoding 参数为浏览器编码或者修改useBodyEncodingForURI 为true ，并且在获得数据的JSP 页面中 request.setCharacterEncoding参数设置成浏览器编码。
为什么需要Url 编码
1.  Url 中有些字符会引起歧义 , =,& 号等
2.  Url 的编码格式采用的是ASCII 码，而不是Unicode ，这也就是说你不能在Url 中包含任何非ASCII 字符，例如中文
哪些字符需要编码
RFC3986 文档规定，Url 中只允许包含英文字母（a-zA-Z ）、数字（0-9 ）、-_.~4 个特殊字符以及所有保留字符。
Url 可以划分成若干个组件，协议、主机、路径等。RFC3986 中指定了以下字符为保留字符：　! * ' ( ) ; : @ & =+ $ , / ? # [ ]
如何对Url 中的非法字符进行编码
Url 编码通常也被称为百分号编码（Url Encoding ，also known as percent-encoding ），是因为它的编码方式非常简单，使用% 百分号加上两位的字符——0123456789ABCDEF—— 代表一个字节的 十六进制形式。Url 编码默认使用的字符集是US-ASCII 。例如a 在US-ASCII 码中对应的字节是0x61 ，那么Url 编码之后得到的就是% 61 ，我们在地址栏上输入[http:
 //g.cn/search?q=%61%62%63，实际上就等同于在google 上搜索abc 了。又如@ 符号在](http://g.cn/search?q=abc%A3%AC%CA%B5%BC%CA%C9%CF%BE%CD%B5%C8%CD%AC%D3%DA%D4%DAgoogle%C9%CF%CB%D1%CB%F7abc%C1%CB%A1%A3%D3%D6%C8%E7@%B7%FB%BA%C5%D4%DA)ASCII 字符集中对应的字节为0x40 ，经过Url 编码之后得到的是%40 。
Javascript 中的escape,encodeURI 和encodeURIComponent 的区别 
Javascript 中提供了3 对函数用来对Url 编码以得到合法的Url ，它们分别是escape / unescape,encodeURI / decodeURI 和encodeURIComponent / decodeURIComponent 。解码和编码的过程是可逆的.
**兼容性不同 **
escape 函数是从Javascript1.0 的时候就存在了，其他两个函数是在Javascript1.5 才引入的。但是由于Javascript1.5 已经非常普及了，所以实际上使用encodeURI 和encodeURIComponent 并不会有什么兼容性问题。
**对Unicode 字符的编码方式不同 **
这三个函数对于ASCII 字符的编码方式相同，均是使用百分号+ 两位十六进制字符来表示。但是对于Unicode 字符，escape 的编码方式是% uxxxx ，其中的xxxx 是用来表示unicode 字符的4 位十六进制字符。这种方式已经被W3C 废弃了。但是在ECMA-262 标准中仍然保留着 escape 的这种编码语法。encodeURI 和encodeURIComponent 则使用UTF-8 对非ASCII 字符进行编码，然后再进行百分号 编码。这是RFC 推荐的。因此建议尽可能的使用这两个函数替代escape 进行编码。
**适用场合不同 **
encodeURI 被用作对一个完整的URI 进行编码，而encodeURIComponent 被用作对URI 的一个组件进行编码。
**安全字符不同 **
escape （69 个） */@+-._0-9a-zA-Z   
encodeURI （82 个） !#$&'()*+,/:;=?@-._~0-9a-zA-Z  
encodeURIComponent （71 个） !'()*-._~0-9a-zA-Z   (注意+ 号未在其安全字符里)
其他和Url 编码相关的问题
对于包含中文的Url 的处理问题，不同浏览器有不同的表现。例如对于IE ，如果你勾选了高级设置“ 总是以UTF-8发送Url” ，那么Url 中的路径部分 的中文会使用UTF-8 进行Url 编码之后发送给服务端，而查询参数中的中文部分使用系统默认字符集进行Url 编码。为了保证最大互操作性，建议所有放到 Url 中的组件全部显式指定某个字符集进行Url 编码，而不依赖于浏览器的默认实现。
另外，很多HTTP 监视工具或者浏览器地址栏等在显示Url 的时候会自动将Url 进行一次解码（使用UTF-8 字符集），这就是为什么当你在 Firefox 中访问Google 搜索中文的时候，地址栏显示的Url 包含中文的缘故。但实际上发送给服务端的原始Url 还是经过编码的。你可以在地址栏 上使用Javascript 访问location.href 就可以看出来了。在研究Url 编解码的时候千万别被这些假象给迷惑了。
