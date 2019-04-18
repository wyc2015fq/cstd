# URLs,URIs,Proxies和Passwords 解析 - ljx0305的专栏 - CSDN博客
2007年04月08日 18:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：906标签：[passwords																[string																[scheme																[java																[服务器																[query](https://so.csdn.net/so/search/s.do?q=query&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=scheme&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=passwords&t=blog)
**类URLEncoder 和 类URLDecoder **web设计者面临的众多难题之一便是怎样处理不同操作系统间的差异性。这些差异性能引起URL方面的问题：例如，一些操作系统允许文件名中含有空格符，有些又不允许。大多数操作系统不会认为文件名中含有符号“#”会有什么特殊含义；但是在一个URL中，符号“#”表示该文件名已经结束，后面会紧跟一个fragment（部分）标识符。其他的特殊字符，非字母数字字符集，它们在URL或另一个操作系统上都有其特殊的含义，表述着相似的问题。为了解决这些问题，我们在URL中使用的字符就必须是一个ASCII字符集的固定字集中的元素，具体如下：
1.大写字母A-Z
2.小写字母a-z
3.数字 0-9
4.标点符 - _ . ! ~ * ' (和 ,)
诸如字符: / & ? @ # ; $ + = 和 %也可以被使用，但是它们各有其特殊的用途，如果一个文件名包括了这些字符（ / & ? @ # ; $ + = %），这些字符和所有其他字符就应该被编码。
编码过程非常简单，任何字符只要不是ASCII码数字，字母，或者前面提到的标点符，它们都将被转换成字节形式，每个字节都写成这种形式：一个“%”
后面跟着两位16进制的数值。空格是一个特殊情况，因为它们太平常了。它除了被编码成“%20”以外，还能编码为一个“+”。加号（+）本身被编码为%2B。当/ # = & 和?作为名字的一部分来使用时，而不是作为URL部分之间的分隔符来使用时，它们都应该被编码。
WARNING这种策略在存在大量字符集的异构环境中效果不甚理想。例如：在U.S. Windows 系统中, é 被编码为 %E9. 在 U.S. Mac中被编码为%8E。这种不确定性的存在是现存的URI的一个明显的不足。所以在将来URI的规范当中应该通过国际资源标识符(IRIs)进行改善。
类URL并不自动执行编码或解码工作。你能生成一个URL对象，它可以包括非法的ASCII和非ASCII字符和/或%xx。当用方法getPath() 和toExternalForm( ) 作为输出方法时，这种字符和转移符不会自动编码或解码。你应对被用来生成一个URL对象的字符串繁荣对象负责，确保所有字符都会被恰当地编码。
幸运的是，java提供了一个类URLEncoder把string编码成这种形式。Java1.2增加了一个类URLDecoder它能以这种形式解码string。这两个类都不用初始化
**URLEncoder**
在java1.3和早期版本中，类java.net.URLEncoder包括一个简单的静态方法encode( ) 它对string以如下规则进行编码：
public static String encode(String s)
这个方法总是用它所在平台的默认编码形式，所以在不同系统上，它就会产生不同的结果。结果java1.4中，这个方法被另一种方法取代了。该方法要求你自己指定编码形式：
两种关于编码的方法，都把任何非字母数字字符转换成%xx（除了空格，下划线(_)，连字符（—）,句号（。）,和星号（*））。
两者也都编码所以的非ASCII字符。空格被转换成一个加号。这些方法有一点过分累赘了；它们—也把“~”，“‘”，“（）”转换成%xx，即使它们完全用不着这样做。尽管这样，但是这种转换并没被URL规范所禁止。所以web浏览器会自然地处理这些被过分编码后的URL。
两中关于编码的方法都返回一个新的被编码后的string，java1.3的方法encode( ) 使用了平台的默认编码形式，得到%xx。这些编码形式典型的有：在 U.S. Unix 系统上的ISO-8859-1, 在U.S. Windows 系统上的Cp1252,在U.S. Macs上的MacRoman，和其他本地字符集等。因为编码解码过程都是与本地操作平台相关的，所以这些方法是另人不爽的，不能跨平台的。这就明确地回答了为什么在java1.4中这种方法被抛弃了，转而投向了要求以自己指定编码形式的这种方法。尽管如此，如果你执意要使用所在平台的默认编码形式，你的程序将会像在java1.3中的程序一样，是本地平台相关的。在另一种编码的方法中，你应该总是事业UTF-8，而不是其他什么。UTF-8比起你选的其他的编码形式来说，它能与新的web浏览器和更多的其他软件相兼容。
例子7-8是使用URLEncoder.encode( ) 来打印输出各种被编码后的string。它需要在java1.4或更新的版本中编译和运行。
Example 7-8. x-www-form-urlencoded strings 
下面就是它的输出。需要注意的是这些代码应该以其他编码形式被保存而不是以ASCII码的形式，还有就是你选择的编码形式应该作为一个参数传给编译器，让编译器能据此对源代码中的非ASCII字符作出正确的解释。
特别需要注意的是这个方法编码了符号，“/”
,&,=,和：。它不会尝试着去规定在一个URL中这些字符怎样被使用。由此，所以你不得不分块编译你的URL,而不是把整个URL一次传给这个方法。这是很重要的，因为对类URLEncoder最通常的用法就是查询string，为了和服务器端使用GET方法的程序进行交互。例如，假设你想编码这个查询sting，它用来搜索AltaVista网站：
这段代码对其进行编码：
不幸的是，得到的输出是:
出现这个问题就是方法URLEncoder.encode( ) 在进行盲目地编码。它不能区分在URL或者查询string中被用到的特殊字符（象前面string中的“＝”，和“&”）和确实需要被编码的字符。由此，所以URL需要像下面这样一次只编码一块：
这才是你真正想得到的输出：
例子7-9是一个QueryString　类。在一个java对象中，它使用了类URLEncoder　来编码连续的属性名和属性值对，这个java对象全被用来发送数据到服务器端的程序。当你在创建一个QueryString　对象时，你可以把查询string中的第一个属性对传递给类QueryString　的构造函数，得到初始string　。如果要继续加入后面的属性对，就应调用方法add（），它也能接受两个string作为参数，能对它们进行编码。方法getQuery( ) 返回一个属性对被逐个编码后得到的整个string。
Example 7-9. -The QueryString class
利用这个类，现在我们就能对前面那个例子中的string进行编码了:
**URLDecoder**
与URLEncoder 类相对应的URLDecoder 类有两种静态方法。它们解码以x-www-form-url-encoded这种形式编码的string。也就是说，它们把所有的加号（+）转换成空格符，把所有的%xx分别转换成与之相对应的字符：
第一种解码方法在java1.3和java1.2中使用。第二种解码方法在java1.4和更新的版本中使用。如果你拿不定主意用哪种编码方式，那就选择UTF-8吧。它比其他任何的编码形式更有可能得到正确的结果。
如果string包含了一个“%”，但紧跟其后的不是两位16进制的数或者被解码成非法序列，该方法就会抛出IllegalArgumentException 异常。当下次再出现这种情况时，它可能就不会被抛出了。这是与运行环境相关的，当检查到有非法序列抛不抛出IllegalArgumentException 异常，这时到底会发生什么是不确定的。在Sun's JDK 1.4中，不会抛出什么异常，它会把一些莫名其妙的字节加进不能被顺利编码的string中。这的确令人头疼，可能就是一个安全漏洞。
由于这个方法没有触及到非转义字符，所以你可以把整个URL作为参数传给该方法，不用像之前那样分块进行。例如：
**The URI Class**
URI是URL的一个抽象,它不仅包括了统一资源定位符(URL),还包括了统一资源名(URN).大多数实际应用中使用的URI都是URL,但是许多规范和标准像XML都是用URI来定义的.在java1.4和更新的版本中, URI被java.net.URI 类所表示.这个类与java.net.URL 相比有如下3点重要的区别:
·        URI 类只关心资源的标识和对URI的解析.它没有方法来检索它的URI所标识的资源。
·        URI 类与URL 类相比，它更能适应相关的规范。
·        一个URI 对象能表示一个相对URI 。URL 类在存放之前，就已经对所有的URI进行了“绝对化”的处理。
简而言之，一个URL 对象就是网络应用层协议进行网络检索的一个代理，而一个URI 对象就只纯粹地做string的解析和操作的工作。URI 类没有进行网络检索的能力。URL 类有一些进行string解析的方法。比如getFile( ) 和 getRef( ) 方法，但很多都是蹩脚的方法，总是不完全像有关的规范上所说的那样好用。假如你现在用的是java1.4版本或更新的版本，这时你就可以做出选择，如果你想下载一个URL指示的内容时，你应该使用 URL类；如果你想使用URI类来进行标识的工作而不是用来检索的时候， 你应该用URI类。例如，去标识一个XML namespace 的URI。在一些情况下，当你同时需要实现这两种功能时，你可以用方法toURL( ) 把一个URI 转换成一个 URL 。在java1.5中，你还能用类URL 中的方法toURI( )  把一个URL 转换成一个URI 。
**构造一个 URI**
URI是由string构成的。不像类URL ，类URI 
不依靠底层协议的处理程序。只要URI是语义上正确的，java就不需要知道它的什么协议，然后才创建其代表的URI对象。因此，不象类URL ，类URI 可以被用到新的试验性的URI协议中去。
**public URI(String uri) throws URISyntaxException**
这是一个简单的构造函数，它用一个合适的string创建了一个新的URI 对象。例如：
如果参数string不符合URI的语法规则—比如，如果它以“：”开始—构造函数就会抛出URISyntaxException 异常。这是一个需要被检察的异常，所以你要么捕获该异常，要么在构造函数会被执行的方法中，声明这个方法会抛出该异常。尽管如此，但是有一条语法规则是不能被检察到的。与URI规范相矛盾的是，在URI中使用的字符不会被限制在ASCII字符集上。它们可以包含其它的Unicode字符，比如ø 和 é。从语义上讲，对URI几乎就没有什么限制，特别是一旦不需要对非ASCII字符进行编码时，一切URI都是被允许的。几乎任何的string都可以被看成是URI。
**public URI(String scheme, String schemeSpecificPart, String fragment) throws URISyntaxException**
这个构造函数主要是用在不存在有层次关系的URI中。Scheme是指URI的协议，比如http, urn, tel等。它必须专门有ASCII字母，数字和标点符（+, -, 和.）组成。第一个位置上必须是一个字母。如果省略了参数scheme，就用null代替。这时创建的是一个相对URI。例如：
scheme-specific部分的内容遵从于参数scheme给出的URI协议；对一个http 协议的URL来说，是一种形式，对于mail协议的 URL来说又是另一种形式，对于tel协议的 URI又是其它的什么形式。因为类URI 会以%xx对非法字符进行编码，所以显而易见，你不会在这步犯语法方面的错误。最后，只要有需要的话，第三个参数可以是指一个部分标识符。同样地，在这个部分标识符中，字符的合法性也顺理成章地不需要检察。如果省略了参数fragment，就用null代替。
**public URI(String scheme, String host, String path, String fragment) throws URISyntaxException**
这个构造函数被用在有层次关系的URI中，比如http 和 ftp 协议的URL。主机名和路径名（用“/”分开）一起构成URI的scheme-specific部分。例如：
如果构造函数从得到的几个参数中不能获得一个有合法层次关系的URI—例如，假如给定了参数scheme，它要求这个URI是一个绝对的URI可是path参数却又不是一“/”开始的—它就会抛出异常URISyntaxException 
**Public URI(String scheme, String authority, String path, String query, String fragment) throws URISyntaxException**
这个构造函数和前边所讲的那个基本相同，除了它增加了一个进行查询的string 参数。例如：
与通常一样，遇到任何的语法错误它都会抛出异常URISyntaxException ，如果其中的哪个参数被省略，就用null代替。
**public URI(String scheme, String userInfo, String host, int port, String path, String query, String fragment) throws URISyntaxException**
这是构造一个有层次关系的URI的父亲构造函数，也就是说，前面提到的两个构造函数都会调用它。它把认证信息（authority）分成了user info, host, 和 port几部分。它们中的每一个都具备其自身的语法规则。比如：
尽管这样，但是最后得到的URI仍然得遵从URI的一般规则。同样地，如果任何一个参数被省略，就用null代替。
**public static URI create(String uri)**
它不是个构造函数，而是个静态factory方法。不象构造函数那样，它不会抛出异常URISyntaxException 。如果你确信你的URI一定会是合法的，会遵从任一条规则，那么你就可以用该方法。比如，这次调用创建了一个用来匿名登陆一个FTP的URI ，它用一个邮箱地址当密码：
如果该URI确实存在问题，这个方法会抛出异常IllegalArgumentException 。它是在运行时才抛出的异常，所以你不能声明或捕获它。
**The Parts of the URI**
一个URI最多就三部分：scheme, scheme-specific , fragment identifier。一般形式： 
如果省略了scheme，这个URI就是相对的。如果fragment identifier被省略了，这个URI就很简单了。类URI有返回方法，它们能返回每个URI对象的这三部分。方法getRawFoo( ) 返回编码后的URI，与此类似的还有方法getFoo() ，它先解码%xx形式的字符，然后返回解码后的URI。
**注意：** 这里没有方法getRawScheme( )  ，因为在URI规范中所有的协议名应专门由合法的ASCII字符组成。不允许%xx在协议名中出现。
如果URI 对象不具备相关的内容，这些方法都返回null：例如，对一个没指定协议的相对URI，或一个没有fragment identifier的http 协议的URI。一个指定了协议（scheme）的URI是一个绝对的URI。没有指定协议的URI是相对的URI。方法isAbsolute()  返回true如果这个URI是绝对的，如果是相对的就返回false：
scheme-specific在细节上的变化取决于scheme的类型。比如在一个tel URL中，scheme-specific需要用类似电话号码的语法。尽管如此，但是在许多有用的URI中，包括很常见的file 和 http URL里面，scheme-specific都有其独特的描述层次关系的模式，包括authority, path, query string。Authority又被细分为user info, host,  port。方法isOpaque()  返回false如果这是个有层次关系的URI，否则返回true—也就是说，如果是层次模糊的：
如果URI是层次模糊的，所有你能得到的就只是scheme, scheme-specific ,  fragment identifier。尽管如此，但是如果URI是有层次关系的，下面的getter方法还是能获取该URI的所有内容：
所有这些方法返回的都是解码后的内容；换句话说，这种形式%xx，例如%3c，都会被转换成它本身所表示的字符，比如符号“<”。如果你想得到未经解码的URI，这里有5个相并列的方法：
记住类URI  与URI规范是不同的。所以像é 和 ü 这种非ASCII字符绝不会在第一位置以%xx形式出现，所以在用方法getRawFoo() 返回得到的string中，仍将回出现在其中；除非本来被用做构建URI对象的string已被编码。
注意: 这里没有方法getRawPort( ) 和 getRawHost( )  ，因为这些部分总是被保证由ASCII字符构成的，至少现在是这样的。国际化的域名时代即将来临，希望在java以后的版本中能重新思考这一做法。
事实上，具体的URI不会包括这些信息—例如，这个URI，http://www.example.com 就没有user info, path, port, 或 query string—相应的方法就返回null。方法getPort( )  是唯一例外。因为它被声明为返回一个int ，所以它不能返回null。它就用返回-1来代替，来表示没有port的信息。
由于各种技术上的也不会有多大实际影响的原因，java不会一开始就在认证部分做语法错误检察。这样做的一个很自然的后果就是不会返回认证的各个部分：port, host,  user info。这种情况下，你可以调用parseServerAuthority()  让认证（authority）被强制再解析：
这个最初的URI 不会发生变化（URI对象是永远不会变的），但是URI 的返回值将会是authority被分开后的部分：user info, host,  port。如果这个authority不能被解析，会抛出URISyntaxException 异常。
例子7-10使用这些方法把输入在命令行上的URI分成了它的那几个组成部分。这与例子7-4相似，只是它处理的是语法上正确的URI，而不要求JDK必须提供一个合适的protocal 处理器。
Example 7-10. The parts of a URI 
下面就是运行这三个URI实例后的结果：
**处理相对的 URI**
类URI 提供了三种方法进行相对的和绝对的URI之间的转换。
**public URI resolve(URI uri)**
它比较参数uri 和这个URI ，然后用它构成一个新的URI 对象，它会有一个绝对的URI。例如，思考这三行代码：
在它们执行后，resolved 会有一个绝对的URI
http://www.example.com/images/logo.png.
如果这个被调用的URI 它本身就没有绝对的URI，那么方法resolve( )  会尽量的处理这个URI，返回一个新的相对的URI对象。比如，下面这三个语句：
在执行后，resolved 现在有一个相对的URI
javafaq/books/jnp3/examples/07/index.html
就没有scheme（协议）和authority（认证）
**public URI resolve(String uri)**
这是个简便的方法，它能方便地把参数string 转化成一个URI，然后与被调用的这个URI组合后，返回一个新的URI对象。也就是，它与resolve(newURI(str)) 是等价的。使用这个方法，前面的两个例子就可以重写如下：
反向操作这个过程也可以；也就是，从一个绝对的URI到一个相对的URI。方法relativize( )  用参数uri 创建一个新的URI 对象，这个uri 对被调用的URI 是相对的。参数没有改变。例如：
现在的 URI 对象包含了一个相对的 URI images/logo.png.
**一些有用的方法**
类URI 有不少普通的但很实用的方法：equals(), hashCode( ), toString( ), 和 compareTo( )
public boolean equals(Object o)
URI之间的比较与你期待中的一样好。它不是string之间的直接做比较。相同的URI必须要么是有层次关系的要么就是层次关系模糊的。Scheme和authority部分的比较是不区分大小写的。也就是http 和 HTTP 都表示的是相同的协议，authority 的比较www.example.com 和 www.EXAMPLE.com 也是一样的。URI中剩下的部分就得区分大小写了，除了用来转义非法字符的16进制的数字以外。在做比较前，%xx还没有被解码。
http://www.example.com/A and http://www.example.com/%41 are unequal URIs.
public int hashCode( )
方法hashCode( )  是个普通的hashCode( )  方法，没有什么特别的。相同的URI就会有相同的哈希码，不同的URI就很难拥有相同的哈希码了。
public int compareTo(Object o)
URI是可以排序的。排序是基于对各个部分字符比较的结果上的，以下面的顺序：
·        如果scheme是不同的，就比较scheme，不考虑大小写。
·        否则，如果scheme是相同的，我们 就认为层次关系模糊的URI比有层次关系的URI优先级高。
·        如果比较的两个URI都是层次关系模糊的，这时就以它们的scheme-specific进行排序。
·        如果scheme和层次关系模糊的scheme-specific都相同了，就比较URI的fragment
·        如果做比较的URI都是有层次关系的，就看它们的authority，比较其中的user info, host, port。
·        如果scheme和authority都相同了，就看它们的path。
·        如果path也相同了，接着比较query string。
·        如果query string也相同了，就比较fragment
URI除了和它们自身比较以外，不能和任何其他的类型做比较。把一个URI 和任何除了URI 以外的做比较会抛出异常ClassCastException 
**public String toString( )**
方法toString( )  返回一个URI 形式的未经编码的string。也就是说，像é 和 /这种字符就不会被转换成%xx，除非它们在用来生成URI 的string中已经被转换了。因此，调用这个方法后得到的结果不能保证它是一个语法正确的URI 。有时，这种形式有助于增强可读性，但是对检索来说就不一样了。
**public String toASCIIString( )**
方法toASCIIString( ) 返回一个编码后URI 形式的string。像é 和 /这样的字符总是是被转换成了%xx的，不管它们最初是否已经被转换。很多时间你就应该使用这种string形式 的URI。尽管toString( ) 返回的形式对于人来说更易于理解，但是它们仍然可能被复制粘贴到一些期望得到合法URI的地方。而方法toASCIIString( ) 返回语法正确的URI。
**Proxies（代理服务）**
许多系统都通过proxy 服务器访问网站和其他Internet上的不使用HTTP的地方。代理服务器接受从本地客户机发向远程服务器的请求，再把这个结果发送给本地客户机。这样做有时出于安全的原因，比如阻止远程服务器获取关于本地客户机网络配置的信息。有时通过过滤发出的请求和限制被访问的网站，阻止用户去访问那些被禁止访问的的网站。比如像一所小学就会阻止对http://www.playboy.com 的访问。当然有时这样做是单纯地出于对效率的考虑，当大量用户需要下载一台远程服务器上的相同文件时,就让他们从一个本地cache上下载,而不是不停地从远程服务器上下载。Java中基于URL 类的程序可以在大多数代理服务器和协议工作。的确，这就是你宁愿想选择使用URL 类而不愿意基于SOCKET自己编写HTTP包或者其他客户端程序的原因。
**System Properties（系统属性）**
对于基本操作而言，你需要做的就是设置一些系统属性指定你的本地代理服务器的地址。如果你使用的是一个单纯的HTTP代理，把http.proxyHost 设置为域名或者为你的代理服务器的IP地址，把http.proxyPort 设置为代理服务器的端口号（默认为80）。这里有许多种方法可以实现这个操作。包括在你的java代码中调用System.setProperty() 或者当启动程序时，使用命令选项“-D”。下面这个例子就把代理服务器设置为192.168.254.254，端口设置为9000：
你如果不想让某个主机的访问通过代理服务，想直接进行连接，那么就把系统属性http.nonProxyHosts 设置为它的主机名或ip地址。如果你需要对多个主机都进行这种操作，就用“|”把它们分开。比如，这段代码片段表示它会代理所有的除去java.oreilly.com 和 xml.oreilly.com:
你也可以使用“*”作为一个通配符，表示对所有对所有有特殊域或子域的主机的访问不需要被代理。例如，对除了对oreilly.com 域的主机不代理外，对其他的都可以：
如果你使用的是个FTP代理服务器，可以用相同的方法设置系统属性，像ftp.proxyHost, ftp.proxyPort, 和 ftp.nonProxyHosts 。java不支持任何其他应用层的代理，除非如果你对所有的TCP连接都使用传输层的SOCKS代理，你可以使用相关的socksProxyHost 和 socksProxyPort 来设置系统属性。对于SOCKS，java没有提供对是否需要代理的选择。你要么选择都需要代理，要么选择都不需要代理。
**The Proxy 类**
Java1.5允许java程序对代理服务器进行更精细的操作。特别的是，你可以对不同的远程主机选择不同的代理服务器。代理本身是由java.net.Proxy 类的实例表示。这里仍然只有三种代理方式，HTTP, SOCKS, 和直接连接（也就是不需要代理），由Proxy.Type 型迭代器中的三个常量表示：
·        Proxy.Type.DIRECT
·        Proxy.Type.HTTP
·        Proxy.Type.SOCKS
除了代理的类型外，其他关于代理重要信息包括它的地址和端口，由SocketAddress 的对象表示。例如，这个代码片段创建了一个代理对象，它表示proxy.example.com 端口80上的一个HTTP代理服务器：
尽管这里仅有三种代理对象，但是对于不同主机上的不同代理服务器，会有许多相同类型的代理。
**The ProxySelector 类**
每个java1.5的jvm都有一个java.net.ProxySelector 对象。它可以对不同的连接委派合适的代理服务器。默认的ProxySelector 仅仅是检测一下各种系统属性和URL协议，然后决定怎样连接不同的主机。尽管如此，但是你可以安装你自己的ProxySelector 子类来代替默认的选择器（selector），让它基于协议，主机，路径，日期，或其它标准来选择不同的代理。
这个类的关键就是抽象方法select( ) ：
java传一个表示需要被连接的主机的URI 对象给该方法。用类URL生成的这个连接对象通常是这种形式：http://www.example.com/ 或者 ftp://ftp.example.com/pub/files/, 或者其他类似的。对于由Socket类生成的单纯的TCP连接， URI会是这种形式：socket://host:port:,看一个实例，socket://www.example.com:80 。对象ProxySelector 对这种类型的对象会选择合适的代理，然后再以形式List<Proxy> 返回它们。这个类的第二个你必须实现的抽象方法是connectFailed( ):
这是个反馈方法，它警告程序代理服务器连接不成功。例子7-11表示一个ProxySelector 试图对所有的HTTP连接都使用在proxy.example.com 上的代理服务器，除非这个代理服务器在前面，对某个特殊的URL的连接已经失败了。在这种情况下，它会建议用一个直接连接代替。
Example 7-11. A ProxySelector that remembers what it can connect to 
我已经说过的，每个运行的jvm都肯定会有一个ProxySelector 。为了改变这个ProxySelector ，我们给静态方法ProxySelector.setDefault( )  传一个新的选择器，像这样：
从这以后，这个jvm打开的所有连接都会询问这个ProxySelector 该使用哪个合适的代理。正常情况下，你不要在共享环境中使用这段代码。例如，在一个servlet中，你就不要去改变这个ProxySelector 了，因为对在相同容器中的所有servlet来说，它们的ProxySelector 也都被改变了。
**通过GET与服务器端程序进行交互**
类URL 使java applet和application与服务器端程序比如CGIs, servlets, PHP页面，和其他使用GET 方法程序的交互，变得更容易了。（我们将在15章讨论服务器端使用POST 方法会用到URLConnection 类的程序。）所有你需要知道的是程序希望接受名字和值之间采取怎样的连接，生成一个带有查询string的URL，这个string会提供正需要的名字和值。所有的名字和值必须是这样x-www-form-url-encoded—被方法URLEncoder.encode()  编码的，这在前面已经讨论过了。
对一个特定程序的查询string，这里有许多方法来决定它的语法规则。如果你自己已经写过服务器端程序，那么你就已经知道了满足这种程序的名字和值对。如果你已经在自己的服务器上安装了第三方的程序，这个程序的说明文档会告诉你它需要什么样的名字和值对。另一方面，如果你正在与第三方服务器上的程序进行对话，可能就得麻烦多了。你可以总是问在远程服务器上的管理员，让他给你提供怎样和他的站点进行对话的规范说明。尽管如此，即使他们不介意这样做，恐怕也没有谁的工作就是“告诉与我们没有任何关系的黑客，怎样才能进入我们的服务器。”因此，除非你恰巧碰到一个特别热心的或者一个无聊的人，他除了写一封长长的email邮件来告诉你如何进入他们的服务器以外，就没有什么其他事干，否则的话，你不得不做一些逆向工程。 
**注意:**  这种情况正在得到改变。不少的网站开始认识到对第三方开发人员开放他们系统的重要性，以及发布一些开发人员需要的插件的重要性。这些插件提供了详细的信息，利用这些信息来构造URL，对他们网站上的服务进行访问。像站点Safari 和 Amazon都提供了RESTful, URL-based接口，通过URL 类，它们都很容易被访问。像eBay和 Google的 SOAP-based服务就很难使用了。
许多程序被设计用来处理表格输入，如果真是这种情况，就可以直截了当地指出程序需要的是什么输入就行了。表格使用的方法应该是FORM 元素的METHOD 属性的值。这个值要么是GET ，这时你可以使用这里所说的这种处理流程，要么是POST ，这时你可以使用将在15章讲的处理流程。FORM 元素的ACTION 属性的值给出了URL查询string前面的内容。需要注意这可能是个相对URL，这时你应该找到与之相对应的绝对URL。最后，名字和值对（name-value pairs）就是INPUT 元素的NAME 属性，TYPE 属性的值是submit的INPUT 元素例外。
比如，看一下我的Cafe con Leche站点上用于本地搜索引擎的HTML表格。你可以看见它使用了GET 方法。通过URL http://www.google.com/search ，这个处理表格的程序能被访问。它有四对名字和值对，它们中的三个有默认值：
INPUT 的类型并不重要——例如，它是一个复选框的集合，一个下拉菜单条，或者是一个文本域都不重要——只有你给的每个INPUT 的名字和值才是重要的。只有submit型的input是个例外，它告知web浏览器什么时候发送数据，但不会给出服务器任何其他信息。在一些情况下，你可能会发现hidden型的INPUT 必须有一个特定的要求的默认值。这个表格有三个hidden型的INPUT 。在一些情形下，正在与你对话的程序不可能处理有歧义的文本型的string输入值。尽管如此，因为表格是拿来给大家看和填写的，所以它应该提供足够的线索来提示它需要得到什么样的输入；比如，某个域应该是一个两个字母的缩写或者是一个电话号码。
不返回表格的程序对于逆向工程来说会更难。例如，在http://www.ibiblio.org/nywc/bios.phtml,你会发现大量到PHP页面的链接，它们和数据库对话，用某个作曲家的名字来检索一堆音乐作品。尽管这样，这里没有与这个程序相关的表格。这些都被hardcoded的URL处理了。这种情况下，你最多能做的就是尽可能多的查看URL，看你通过他们能不能猜出服务器到底期望得到什么。如果这个设计者不想搞得太晦涩，这些信息是不难得到的。比如，看看在那个页面上所有能得到的URL：
看到这些，你可以猜出这个程序希望得到三个输入，分别是first, middle, 和 last,它们的值分别由一个作曲家的first, middle, 和 last名构成。有时的输入不会是那种让人一看就明白的名字。这时，你需要做些实验，首先复制一些已知的值，然后慢慢提炼出哪些值是能或者不能被接受的。在java程序中，你不需要这样做。你可以简单地在你的web浏览器窗口上的地址栏中编辑URL。
**注意: **由于其他的黑客可能也会对你的服务器端程序做这样的实验，所以你有理由让它们更健壮以应对那些意外的输入。
不管你怎样确定了这个服务器希望得到的名字和值对，一旦你知道了如何同他们进行交互，这就简单了。所有你需要做的就是创建一个包括必需的名字和值对的查询string，然后生成一个包含这个查询string的URL。发送这个查询string到服务器和读取它的反馈的方法，与你连接服务器和检索一个静态HTML页面的方法是相同的。一旦URL被创建，这里就没有什么特殊的协议需要被遵守了。（POST 方法需要遵守一个特殊的协议，尽管这样，这就是为什么它需要在第15章才讨论。）为了表明这个过程，让我们写一个非常简单的命令行程序来看看Netscape Open Directory (http://dmoz.org/)上的标题。站点如图7-3所示，它有变得真正简单的优势。 
![image](http://www.matrix.org.cn/resource/upload/forum/2006_01_23_235201_qBVEgWpmcd.gif)
图7-3。Open Directory的基本用户界面
Open Directory的基本用户界面是一个简单的表格，它拥有一个名为search 的输入框；其中的输入会发送到http://search.dmoz.org/cgi-bin/search 上的一个CGI程序，它执行真正的搜索。表格的HTML语句像下面这样：
表格里只有两个input：一个Submit按纽和一个名为Search的文本框。因此，如果要向Open Directory发送一个搜索请求，你需要收集搜索string ，把它编码成一个查询string，然后发送到http://search.dmoz.org/cgi-bin/search。例如，为了搜索“java”，你需要打开一个连接URL http://search.dmoz.org/cgi-bin/search?search=java 的连接，然后读取这个结果输入流。例子7-12就是按照这样来做的。
Example 7-12. Do an Open Directory search 
当然，在解析和显示结果上需要更多的努力。但是请注意与服务器进行对话的代码是多么的简单呀！。除去看上有趣的URL和组成它的一些部分需要被x-www-form-url-encoded的可能性更大以外，使用GET 与服务器进行对话的程序不比检索任何其他HTML页面的工作困难。
**Accessing Password-Protected Sites**
（进入受密码保护的站点）
许多流行的站点，像TheWall Street Journal,需要一个用户名和密码才能被访问。一些网站，像W3C的会员页面，通过HTTP认证正规地执行这些。其他的像Java Developer Connection，通过cookies 和 HTML非正规地执行这些认证。Java的URL类能访问使用http认证的站点，尽管你也需要告诉它你的用户名和密码。Java对那些使用非标准的、基于cookie认证的站点是提供支持的。一部分原因是在java1.4和早期版本中，java实际上是不支持cookie的，还有部分原因是：如果这样做，需要解析和发送html表格，最后是因为cookie与web架构是完全抵触的。（Java 1.5的确增加了对cookie的支持，我们将在下一章讨论它。尽管如此，但它没有对有认证功能的cookie与其他cookie区分开。）你可以自己提供这种支持， 
使用URLConnection 类来读和写cookie被设置或被返回的http头文件。但是，要想这样做并不简单，经常需要你要连接站点的本地代码。这个难度不压于让web浏览器完全实现对html表格和cookie的支持。访问受标准的http认证保护的站点就简单多了。
**Authenticator 类**
包java.net 中有一个Authenticator 类，你能用它给受http验证保护的站点提供一个用户名和密码：
public abstract class Authenticator extends Object // Java 1.2
因为Authenticator 是一个抽象类，你必须生成它的子类。不同的子类会用不同的方法检索信息。例如，一个字符模式的程序可能会叫用户输入用户名和密码，用System.in 接受它们。一个GUI程序可能会显示一个对话框，像图7-4那样。一个自动机器会从一个加密文件中读出用户名。
![image](http://www.matrix.org.cn/resource/upload/forum/2006_01_23_235333_BczenHuZNU.gif)
图7-4。验证对话框
为了要让URL 类使用Authenticator 类的子类，就把它传递给静态方法Authenticator.setDefault() ，这样就把它安装成了默认的authenticator：
比如，如果你已经写好了一个叫DialogAuthenticator 的Authenticator 子类，你得这样安装它：
你只需要这样做一次就行了。从这以后，当URL 类需要用户名和密码时，它会叫DialogAuthenticator 使用静态方法Authenticator.requestPasswordAuthentication()  ：
参数address 就是需要进行验证的主机。参数port 就是那个主机上的端口，参数protocol 就是访问这个站点时所用的应用层协议。http服务器会提供参数prompt 。一般就是指需要被验证的访问域。（像www.ibiblio.org 这样的大网站会有许多的访问域，每个都要求用不同的用户名和密码。）参数scheme 是用来做验证的模式。（这里的单词scheme 与protocol （协议）不是同义词。而是一种基本的http验证模式。）
不被信任的applet不允许让用户提供用户名和密码。受信任的applet可以这样做，只要它们有requestPasswordAuthenticationNetPermission 
否则的话，Authenticator.requestPasswordAuthentication( ) 抛出SecurityException 异常。Authenticator 的子类必须覆盖getPasswordAuthentication( )  方法。这个方法里，你需要从用户或其他来源处取到用户名和密码，然后作为java.net.PasswordAuthentication 类的实例返回：
java.net.PasswordAuthentication class:
如果你不想验证这个请求，就返回null，java回告诉服务器它不知道该怎样验证这个连接。如果你发送一个不正确的用户名或密码，java会再次调用getPasswordAuthentication( ) ，再给你一次机会提交正确的数据。正常情况下，你有5次机会得到正确的用户名和密码；在那以后，openStream( )  会抛出ProtocolException 异常。
用户名和密码被放在相同的虚拟机对话中。一旦你对一个访问域使用了正确的密码进行访问，你就不会被要求再次使用密码了，除非你已经很明确地清空了保存它的char 数组。你可以通过调用这些从超类Authenticator 继承来的方法获取更多详细的信息：
这些方法要么返回上次被requestPasswordAuthentication( ) 调用后得到的信息，要么返回null，如果那些信息已经不能使用了。（getRequestingPort( ) 返回-1，如果那个端口不能被使用了。）最后一个方法，getRequestingHost( ) ，只有在java1.4和更新的版本中才能被使用；在早期版本里，你可以调用getRequestingSite( ).getHostName( ) 代替。Java1.5又增加了两个方法到这个类中：
方法getRequestingURL( )  会返回一个被要求使用验证的完整的URL——这是个重要的 ，如果一个站点的不同文件需要使用不同的用户名和密码。方法getRequestorType( ) 会返回下面两个常量中的一个 Authenticator.RequestorType.PROXY 或者 Authenticator.RequestorType.SERVER  ，以此来区分是服务器还是代理服务器请求验证。
**PasswordAuthentication 类**
PasswordAuthentication 是一个非常简单的final类，它提供两个只读属性：用户名和密码。用户名是string存放的。密码被存放在char 数组中，所以当不需要时，可以擦除密码。一个string在它可以被擦除前，要等待垃圾回收器来收集，即使这时它仍有可能存在本地系统内存上的某个地方，也有可能在磁盘上，如果这时这个内存块已经被交换到作为虚拟内存的磁盘区域上。用户名和密码被设置在这个构造函数中：
每个都是通过getter 方法被访问的：
**JPasswordField 类**
Swing中JPasswordField 组件有个用来向用户获取密码的有用工具：
这个轻量级组件和文本框的行为几乎是一样的。但是用户输入的会以“*”形式显示出来。这样的话，就可以防止其他人在后面看见用户输入的密码了。
JPasswordField 也用char 数组来存放密码，这样当你不再需要是，你就可以清空它了。方法getPassword( )  用来返回密码：
不然的话，大多数时候你得使用继承超类JTextField 后得到的方法。例子7-13展示了一个来自Swing的Authenticator 的子类，它生成一个对话框向用户获取他的用户名和密码。下面的大多数代码都是用来生成GUI。JPasswordField 提取密码，JTextField 提取用户名。图7-4就是下面代码所生成的一个简单的对话框。
Example 7-13. A GUI authenticator 
例子7-14是经过修改后的SourceViewer 程序，通过DialogAuthenticator 类向用户获取用户名和密码。
Example 7-14. 用来下载受密码保护网页的程序 
