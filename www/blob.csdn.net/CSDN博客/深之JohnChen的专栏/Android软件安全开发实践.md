# Android软件安全开发实践 - 深之JohnChen的专栏 - CSDN博客

2014年09月26日 10:16:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：3937标签：[Android 软件安全](https://so.csdn.net/so/search/s.do?q=Android 软件安全&t=blog)
个人分类：[Android](https://blog.csdn.net/byxdaz/article/category/715665)



**Android开发是当前最火的话题之一，但很少有人讨论这个领域的安全问题。本系列将分两期，探讨Android开发中常见的安全隐患和解决方案。第一期将从数据存储、网络通信、密码和认证策略这三个角度，带你走上Android软件安全开发实践之旅。**

过去两年，研究人员已发现Android上的流行软件普遍存在安全缺陷或安全漏洞。漏洞频发的原因可能有很多，例如以下几种。
- 

与一切都是集中管理的iOS相比，Android提供了一种开放的环境，在获得了灵活性、可以满足各种定制需求的同时，也损失了部分安全性。

- 

开发团队通常将精力集中在产品设计、功能实现、用户体验和系统效率等方面，而很少考虑安全问题。

- 

Android提供的安全机制比较复杂，开发者需要理解它们，并对常见的攻击思路和攻击方法有所了解，才能有效地保护软件。

- 

一方面，目前很少出现对特定移动软件安全漏洞的大规模针对性攻击，在真实的攻击出现之前，许多人对此并不重视。另一方面，利用这些漏洞展开攻击并不太难，许 多攻击方法和工具都已经成熟。一旦出现这种攻击，用户的个人隐私数据可能发生泄漏，账户信息可能被盗取，如果与钓鱼等攻击结合，甚至可能产生经济损失。产 品开发团队则可能由此面临信任危机和法律风险。

我在此前进行的一些安全评估中，看到不少开发团队已具有非常高的安全开发水平，但也发现有知 名企业的软件存在各种缺陷。在本文中，我们将向大家介绍Android软件中比较常见的安全缺陷或安全漏洞，分析产生问题的原因，介绍可能的攻击方法，并 给出解决问题的建议。希望能抛砖引玉，引起大家对这类问题的关注。

**数据存储**

Android软件可以使用的存储区域分为外部（SD卡）和内部（NAND闪存）两种。除了大小和位置不同之外，两者在安全权限上也有很大的区别。外部存储的文件没有读写权限的管理，所有应用软件都可以随意创建、读取、修改、删除位于外部存储中的任何文件，而仅仅需要申明READ_EXTERNAL_STORAGE和READ_EXTERNAL_STORAGE权限。内部存储则为每个软件分配了私有区域，并有基于Linux的文件权限控制，其中每个文件的所有者ID均为Android为该软件设立的一个用户ID。通常情况下，其他软件无权读写这些文件。

关于数据存储可能出现的问题包括以下几种。

**将隐私数据明文保存在外部存储**

例如，聊天软件或社交软件将聊天记录、好友信息、社交信息等存储在SD卡上；备份软件将通信录、短信等备份到SD卡上等。如果这些数据是直接明文保存（包括 文本格式、XML格式、SQLite数据库格式等）的，那么攻击者写的软件可以将其读取出来，并回传至指定的服务器，造成隐私信息泄露。

较好的做法是对这些数据进行加密，密码保存在内部存储，由系统托管或者由用户使用时输入。

**将系统数据明文保存在外部存储**

例如，备份软件和系统辅助软件可能将用户已安装的其他软件数据保存至SD卡，以便刷机或升级后进行恢复等；或者将一些系统数据缓存在SD卡上供后续使用。同样的，如果这些数据是明文保存的，恶意软件可以读取它们，有可能用于展开进一步的攻击。

**将软件运行时依赖的数据保存在外部存储**

如果软件将配置文件存储在SD卡上，然后在运行期间读取这些配置文件，并根据其中的数据决定如何工作，也可能产生问题。攻击者编写的软件可以修改这些配置文 件，从而控制这些软件的运行。例如，如果将登录使用的服务器列表存储在SD卡中，修改后，登录连接就会被发往攻击者指定的服务器，可能导致账户泄露或会话 劫持（中间人攻击）。

对这种配置文件，较安全的方法是保存到内部存储；如果必须存储到SD卡，则应该在每次使用前判断它是否被篡改，例如，与预先保存在内部的文件哈希值进行比较。

**将软件安装包或者二进制代码保存在外部存储**

现在很多软件都推荐用户下载并安装其他软件；用户点击后，会联网下载另一个软件的APK文件，保存到SD卡然后安装。

也有一些软件为了实现功能扩展，选择动态加载并执行二进制代码。例如，下载包含了扩展功能的DEX文件或JAR文件，保存至SD卡，然后在软件运行时，使用 dalvik.system.DexClassLoader类或者java.lang.ClassLoader类加载这些文件，再通过Java反射，执行 其中的代码。

如果在安装或加载前，软件没有对SD卡上的文件进行完整性验证，判断其是否可能被篡改或伪造，就可能出现安全问题。

在这里，攻击者可以使用称 为“重打包”（re-packaging）的方法。目前大量Android恶意代码已采用这一技术。重打包的基本原理是，将APK文件反汇编，得到 Dalvik指令的smali语法表示；然后在其中添加、修改、删除等一些指令序列，并适当改动Manifest文件；最后，将这些指令重新汇编并打包成 新的APK文件，再次签名，就可以给其他手机安装了。通过重打包，攻击者可以加入恶意代码、改变软件的数据或指令，而软件原有功能和界面基本不会受到影 响，用户难以察觉。

如果攻击者对软件要安装的APK文件或要加载的DEX、JAR文件重打包，植入恶意代码，或修改其原始代码；然后在SD 卡上，用其替换原来的文件，或者拷贝到要执行或加载的路径，当软件没有验证这些文件的有效性时，就会运行攻击者的代码。攻击结果有很多可能，例如直接发送 扣费短信，或者将用户输入的账户密码发送给指定的服务器，或者弹出钓鱼界面等。

因此，软件应该在安装或加载位于SD卡的任何文件之前，对其完整性做验证，判断其与实现保存在内部存储中的（或从服务器下载来的）哈希值是否一致。

**全局可读写的内部文件**

如果开发者使用openFileOutput(String name,int mode)方法创建内部文件时，将第二个参数设置为Context.MODE_WORLD_READABLE或 Context.MODE_WORLD_WRITEABLE，就会让这个文件变为全局可读或全局可写的。

开发者也许是为了实现不同软件之间的数据共享，但这种方法的问题在于无法控制哪个软件可以读写，所以攻击者编写的恶意软件也拥有这一权限。

如果要跨应用共享数据，一种较好的方法是实现一个Content Provider组件，提供数据的读写接口，并为读写操作分别设置一个自定义权限。

**内部敏感文件被root权限软件读写**

如果攻击者的软件已获得root权限，自然可以随意读写其他软件的内部文件。这种情况并不少见。
- 

大量的第三方定制ROM提供了root权限管理工具，如果攻击者构造的软件伪造成一些功能强大的工具，可以欺骗用户授予它root权限。

- 

即便手机安装的官方系统，国内用户也大多乐于解锁、刷recovery并刷入root管理工具。

- 

在Android 2.2和2.3中，存在一些可以用于获取root权限的漏洞，并且对这种漏洞的利用不需要用户的确认。

因此，我们并不能假设其他软件无法获取root权限。即便是存在内部的数据，依然有被读取或修改的可能。

前面提到，重要、敏感、隐私的数据应使用内部存储，现在又遇到root后这些数据依然可能被读取的问题。我对这个问题的观点是，如果攻击者铤而走险获得root权限（被用户觉察或者被安全软件发现的风险），那理论上他已拥有了系统的完整控制权，可以直接获得联系人信息、短信记录等。此时，攻击者感兴趣的 软件漏洞利用更可能是获得其他由软件管理的重要数据，例如账户密码、会话凭证、账户数据等。例如，早期Google钱包将用户的信用卡数据明文存储，攻击 者获取这些数据后，可以伪装成持卡人进行进一步攻击以获得账号使用权。这种数据就是“其他由软件管理的重要数据”。

这个问题并没有通用的解决方法。开发者可能需要根据实际情况寻找方案，并在可用性与安全性之间做出恰当的选择。

**网络通信**

Android软件通常使用WiFi网络与服务器进行通信。WiFi并非总是可信的。例如，开放式网络或弱加密网络中，接入者可以监听网络流量；攻击者可以自己设置WiFi网络钓鱼。此外，在获得root权限后，还可以在Android系统中监听网络数据。

**不加密地明文传输敏感数据**

最危险的是直接使用HTTP协议登录账户或交换数据。例如，攻击者在自己设置的钓鱼网络中配置DNS服务器，将软件要连接的服务器域名解析至攻击者的另一台服务器；这台服务器就可以获得用户登录信息，或者充当客户端与原服务器的中间人，转发双方数据。

早期，国外一些著名社交网站的Android客户端的登录会话没有加密。后来出现了黑客工具FaceNiff，专门嗅探这些会话并进行劫持（它甚至支持在WEP、WPA、WPA2加密的WiFi网络上展开攻击！）。这是目前我所知的唯一一个公开攻击移动软件漏洞的案例。

这类问题的解决方法很显然—对敏感数据采用基于SSL/TLS的HTTPS进行传输。

**SSL通信不检查证书有效性**

在SSL/TLS通信中，客户端通过数字证书判断服务器是否可信，并采用证书中的公钥与服务器进行加密通信。

然而，有开发者在代码中不检查服务器证书的有效性，或选择接受所有的证书。例如，开发者可以自己实现一个X509TrustManager接口，将其中的 checkServerTrusted()方法实现为空，即不检查服务器是否可信；或者在SSLSocketFactory的实例中，通过 setHostnameVerifier(SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER)，接受所有证 书。做出这两种选择的可能原因是，使用了自己生成了证书后，客户端发现证书无法与系统可信根CA形成信任链，出现了
 CertificateException等异常。

这种做法可能导致的问题是中间人攻击。

在钓鱼WiFi网络中，同样地，攻 击者可以通过设置DNS服务器使客户端与指定的服务器进行通信。攻击者在服务器上部署另一个证书，在会话建立阶段，客户端会收到这张证书。如果客户端忽略 这个证书的异常，或者接受这个证书，就会成功建立会话、开始加密通信。但攻击者拥有私钥，因此可以解密得到客户端发来数据的明文。攻击者还可以模拟客户 端，与真正的服务器联系，充当中间人做监听。

解决问题的一种方法是从可信CA申请一个证书。但在移动软件开发中，不推荐这种方法。除了申请 证书的时间成本和经济成本外，这种验证只判断了证书是否CA可信的，并没有验证服务器本身是否可信。例如，攻击者可以盗用其他可信证书，或者盗取CA私钥 为自己颁发虚假证书，这样的攻击事件在过去两年已有多次出现。

事实上，移动软件大多只和固定的服务器通信，因此可以在代码中更精确地直接验 证是否某张特定的证书，这种方法称为“证书锁定”（certificate pinning）。实现证书锁定的方法有两种：一种是前文提到的实现X509TrustManager接口，另一种则是使用KeyStore。具体可参考 Android开发文档中HttpsURLConnection类的概览说明。

**使用短信注册账户或接收密码**

也有软件使用短信进行通信，例如自动发送短信来注册、用短信接收初始密码、用短信接收用户重置的密码等。

短 信并不是一种安全的通信方式。恶意软件只要申明了SEND_SMS、RECEIVE_SMS和READ_SMS这些权限，就可以通过系统提供的API向任 意号码发送任意短信、接收指定号码发来的短信并读取其内容，甚至拦截短信。这些方法已在Android恶意代码中普遍使用，甚至2011年就已出现拦截并 回传短信中的网银登录验证码（mTANs）的盗号木马Zitmo。

因此，这种通过短信注册或接收密码的方法，可能引起假冒注册、恶意密码重置、密码窃取等攻击。此外，这种与手机号关联的账户还可能产生增值服务，危险更大。较好的实现方式还是走Internet。

**密码和认证策略**

**明文存储和编码存储密码**

许多软件有“记住密码”的功能。如果开发者依字面含义将密码存储到本地，可能导致泄漏。

另 外，有的软件不是直接保存密码，而是用Base64、固定字节或字符串异或、ProtoBuf等方法对密码编码，然后存储在本地。这些编码也不会增加密码 的安全性。采用smali、dex2jar、jd-gui、IDA Pro等工具，攻击者可以对Android软件进行反汇编和反编译。攻击者可以借此了解软件对密码的编码方法和编码参数。

较好的做法是，使用基于凭据而不是密码的协议满足这种资源持久访问的需求，例如OAuth。

**对外服务的弱密码或固定密码**

另一种曾引起关注的问题是，部分软件向外提供网络服务，而不使用密码或使用固定密码。例如，系统辅助软件经常在WiFi下开启FTP服务。部分软件对这个FTP服务不用密码或者用固定密码。在开放或钓鱼的WiFi网络下，攻击者也可以扫描到这个服务并直接访问。

还有弱密码的问题。例如，早期Google钱包的本地访问密码是4位数字，这个密码的SHA256值被存储在内部存储中。4位数字一共只有10000种情况，这样攻击软件即便是在手机上直接暴力破解，都可以在短时间内获得密码。

**使用IMEI或IMSI作为唯一认证凭据**

IMEI、IMSI是用于标识手机设备、手机卡的唯一编号。如果使用IMSI或IMEI作为用户认证的唯一凭据，可能导致假冒用户的攻击。

首先，应用要获取手机的IMEI、手机卡的IMSI并不需要特殊权限。事实上，许多第三方广告库回传它们用于用户统计。其次，得到IMEI或IMSI后，攻 击者有多种方法伪造成用户与服务器进行通信。例如，将原软件重打包，使其中获取IMEI、IMSI的代码始终返回指定的值；或修改Android代码，使 相关API始终返回指定的值，编译为ROM在模拟器中运行；甚至可以分析客户端与服务器的通信协议，直接模拟客户端的网络行为。

因此，若使用IMEI或IMSI作为认证的唯一凭据，攻击者可能获得服务器中的用户账户及数据。

**组件间通信**

组件间通信的安全问题是Android所独有的，也是目前软件中最常出现的一种问题。

我们先回顾一下组件间通信机制。Android有四类组件：activity、service、broadcast receiver和content provider。在同一个软件之中或不同软件之间，前三种组件使用Intent相互调用，使用ContentResolver对象访问content provider，共同实现软件的功能。使用Intent，可以显式或隐式地调用：
- 显式（explicit）：调用者知道要调用谁，通过组件名指定具体的被调用者；
- 隐式（implicit）：调用者不知道要调用谁，只知道执行的动作，由系统选择组件处理这个请求。

如下面的代码所示：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/001.jpg)

无论是显式还是隐式，如果要跨应用调用，还需要被调用的组件是对外暴露的。默认情况下，service、broadcast receiver和content provider是暴露的，申明了Intent-filter的actvity也是暴露的。

抽象地说，组件A要调用组件B，以期待B完成某个功能；它可以发送一些数据给组件B，也可以获得B执行后的返回结果。在这个模型中，问题出现在A和B之间不一定互相可信。

如果B是暴露的，任何软件都可以调用它，包括攻击者编写的软件。攻击者可能但并非总能成功：
- 直接调用暴露的B，以获得其执行结果；
- 构造特定的数据，并用于调用暴露的B，从而试图影响B的执行；
- 调用暴露的B，并获取它执行完返回的结果。

如果A用的是隐式调用，任何软件都可以实现它的action从而响应调用。攻击者可能（但并非总能成功）：
- 构造伪造的组件C，响应A的Intent，以读取A要发给B的数据；
- 构造伪造的组件C，响应A的Intent，弹出虚假的用户界面以展开进一步攻击（例如钓鱼）；
- 构造伪造的组件C，响应A的Intent，返回伪造的执行结果。

这样说可能比较抽象。下面我们对这两种情况分别讨论。

**组件暴露的问题**

看一个例子。在一个第三方深度定制的ROM中，预装了名为Cit.apk的软件，用于手机的硬件测试。它的AndroidManifest.xml局部如下：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/002.jpg)

可以看到，它申明一个名为.CitBroadcastReceiver的receiver，响应名为android.provider.Telephony.SECRET_CODE的action，并且指定了URI格式。

再来看这个receiver的代码片段（下面的代码是我反编译得到的，不一定与软件源码完全一致）：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/003.jpg)

可以看到，当调用这个receiver，并且提供的URI中host字段为284时，会以root权限调用本地的bugreport工具，并将结果输出至m_logFileName指定的文件中。

默认情况下receiver是暴露的，因此这个receiver可以被其他软件调用，代码如下：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/004.jpg)

当这四行代码执行时，就会触发CitBroadcast-Receiver的那段代码。从上下文看，输出文件m_logFileName位于SD卡，任何软件都可以随意读写。因此，攻击者可以获得bugreport的输出结果，其中包含大量系统数据和用户数据。

请注意，在这个例子中，攻击者的软件不需要任何特殊权限，尤其是不需要root权限。这种由于组件暴露获得额外权限的攻击，被称之为permission re-delegation（权限重委派）。

怎么避免由于组件暴露产生的安全问题？有的组件必须暴露，例如入口activity，或者确实对外提供服务或跨软件协作；但也有的组件没必要暴露。接下来我们分别讨论。

**不需要暴露的组件**

再次回顾，默认情况下，service、broadcast receiver和content provider是暴露的，申明了Intent-filter的actvity也是暴露的。如果它们只被同一个软件中的代码调用，应该设置为不暴露。很容

Lasted greasy my the

for and it [omeprazole 40 mg india no prescription](http://tietheknot.org/leq/omeprazole-40-mg-india-no-prescription.html) I using? For soft [http://transformingfinance.org.uk/bsz/non-prescription-viagra-for-sale/](http://transformingfinance.org.uk/bsz/non-prescription-viagra-for-sale/) again at s
[obat meloxicam 7 5mg](http://www.adriamed.com.mk/ewf/obat-meloxicam-7-5mg) Best smell rescue on
[buy cheap furosemide online](http://www.allprodetail.com/kwf/buy-cheap-furosemide-online.php) Because. A black ordered [http://spnam2013.org/rpx/cialis-cvs](http://spnam2013.org/rpx/cialis-cvs) comfortable down many mature Fast. After [365 pharmacy](http://thegeminiproject.com.au/drd/365-pharmacy.php) About a well [http://tietheknot.org/leq/alli-cvs-price.html](http://tietheknot.org/leq/alli-cvs-price.html) ? I’m one [http://www.alanorr.co.uk/eaa/viagra-india-canadia.php](http://www.alanorr.co.uk/eaa/viagra-india-canadia.php) leaves week made any [http://www.allprodetail.com/kwf/what-does-provera-do-stop-bleeding.php](http://www.allprodetail.com/kwf/what-does-provera-do-stop-bleeding.php) I them. Headbands comes
[http://theater-anu.de/rgn/accutane-results/](http://theater-anu.de/rgn/accutane-results/) other cure I’ve
[http://theater-anu.de/rgn/low-cost-ed-meds/](http://theater-anu.de/rgn/low-cost-ed-meds/) well along: keep Phyto brown.

[embassyofperu.org](http://www.embassyofperu.org/) weren’t purchase night ll
[cialis online](http://3dprintshow.com/) the live! Hold expensive. Box
[cialis real low prices](http://www.asifa.net/) Moisturizer French enough
[viagra for cheap](http://www.thinkdcs.com/) as t hair,.

易做到—在AndroidManifest.xml中为这个组件加上属性android:exported=”false”即可。

**需要暴露的组件**

如果组件需要对外暴露，应该通过自定义权限限制对它的调用。

首先，在实现了被调用组件的软件的Android-Manifest.xml中自定义一个权限：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/005.jpg)

接下来，为被调用组件添加这个权限限制，即在AndroidManifest.xml中为这个组件添加android:permission属性：

![](http://www.programmer.com.cn/wp-content/uploads/2013/02/0061.jpg)

另一种方法是在组件的实现代码中使用Context.checkCallingPermission()检查调用者是否拥有这个权限。

最后，要调用这个暴露的组件，调用者所在的软件应该申明使用这个权限，即在AndroidManifest.xml中添加相应的use-permission申明。

进一步地，还可以将这种组件暴露的需求分为两种情况。
- 如果这个组件只打算给自己开发的其他软件使用，而不希望暴露给第三方软件，在定义权限时，protectionLevel字段应该选择signature。 这种设置要求权限使用者（即调用者）与权限定义者（即被调用者）必须由相同的证书进行签名，因此第三方无法使用该权限，也就无法调用该组件。
- 如果这个组件要暴露给第三方，则protection-Level应使用normal或dangerous。此时，任何软件都可以使用该权限，只在安装时会 通知用户。考虑到用户一般会忽略权限提示，此时自定义权限实际失去了保护效果，我们依然要仔细审查该组件的代码，避免出现能力泄露或权限重委派等问题。

此外，对content provider，可以更细粒度地为读取数据和写入数据设置不同的权限，对应的manifest标签分别为android:readPermission和android:writePermission。

**隐式调用的问题**

隐式调用的主要问题是被劫持，或Intent携带的数据被读取。

为了劫持调用，攻击者可以实现一个恶意的组件，申明相同的Intent-filter。在多个组件都可以响应同一个Intent的情况下，如果是调用 activity，系统会弹出界面要求用户对多个软件做出选择，攻击者可以模仿真实软件的图标和名称吸引用户点击；如果是调用service，系统会随机 选择一个service；如果是调用receiver，系统会逐一地将Intent发给这些receiver。

劫持了调用后，攻击者可以（但并非总能成功）：
- 启动一个虚假的软件界面，展开钓鱼攻击（例如要求用户输入账户密码）
- 读取Intent携带的数据
- 拦截broadcast的进一步发送，导致真正的receiver无法收到消息，从而拒绝服务
- 如果是用startActivityForResult()调用了虚假的activity，可以返回恶意或虚假的结果给调用者
- 执行其他恶意代码

限于篇幅，对这些情形我们不提供示例代码。来看一下怎么解决这类问题。

**不需要隐式调用**

除了基于Intent类中已有ACTIONs的隐式调用，绝大部分隐式调用都属于这两种情况：同一软件中不同组件的调用；同一开发者不同软件间的调用。

这两种情况下，事实上，开发时都已可以确定要调用的组件是哪个。因此可以避免隐式调用，改为基于组件名的显式调用。

**需要隐式调用**

发送broadcast除了使用sendBroadcast(Intent)，还有一个方法是sendBroadcast(Intent, String)，它的第二个参数可以指定接受者需要的权限。

如果是调用activity或者service，目前我所知，还没有简单的方法实现接收者的权限限制。在Android文档中提出可以自定义Binder和AIDL实现通信双方的互相验证，但真正实现并不容易，也不为官方所推荐。

**数据验证**

无论是客户端还是服务器，在处理外部获得的数据之前，都应先判断和验证数据的有效性。这里主要指是否包含畸形的数据。

在 Web开发中，服务器需要对用户提交的数据进行有效性验证，否则很容易出现众所周知的SQL注入等攻击。在移动开发中也不例外。虽然客户端与服务器在底层 通信协议上对用户是透明、不可见的，但开发者不应因此就假设双方传输的数据永远会和预先设计的一致。类似的，在读取用户从UI元素输入的输入、读取存储在 本地的数据后，使用前也应进行有效性验证。

**软件版权保护**

攻击者对Android软件进行逆向分析，除了寻找其中的安全漏洞，还可以直接攻击软件本身。
- 破解软件的收费机制、License验证或功能限制；
- 修改软件代码，去掉广告库，或者修改广告库（一般是改变推介ID字段），或者增加广告库，然后重新打包并分发；
- 重新打包，植入恶意代码并分发；
- 逆向分析，学习软件特色功能的实现方法，或者获得可复用的代码；

可以采取多种措施来增加破解、修改和逆向分析的难度，减少被攻击的可能。这些措施包括：
- 使用代码混淆工具，例如SDK带的ProGuard以及其他Java混淆器。
- 采用NDK开发核心模块。
- 使用官方或第三方的软件保护方案，例如SDK的android.drm包、Google Play的软件许可（Application Licensing）支持。
- 去掉开发时的调试代码，关闭调试开关，删除多余的Log代码。

然而，采取了这些措施并不等于就万无一失了。例如，用NDK开发的Native代码，也可以使用IDA Pro及其插件来反汇编、反编译和调试；用Google的DRM方案，也有AntiLVL这样的破解工具。理论上，现有防御手段都可能找到方法继续攻击， 其价值只是提高攻击难度和成本。

**总结**

**已出现和将要出现的威胁**

到 目前为止，在学术研究以外，针对Android软件漏洞的攻击只出现一起—劫持国外多个社交网站客户端登陆会话的黑客工具FaceNiff。但随着攻击者 制造传播恶意代码的成本增加和收益降低，以及移动终端隐私数据逐渐成为地下产业链的交易资源，针对Android流行软件漏洞的攻击在未来几年之内几乎一 定会出现并爆发。

**统一的安全模型**

限于篇幅，本文只介绍了几种常见且简单的安全问题，还存在许多我们知道的、还不知道的漏洞。如何找到和解决这些问题？

回顾已介绍的内容，我们可以发现它们有类似的安全模型：通信双方的信任问题。
- 在数据存储中，读写数据的代码和存储在本地的数据互相不可信。
- 在数据通信中，发送者和接受者互相不可信。
- 在登录认证中，发起认证请求的用户的和接受认证请求的服务器互相不可信。
- 在组件间通信中，发起Intent的组件与接收Intent的组件互相不可信。
- 在数据验证中，处理数据的模块不能相信产生数据的源。

面对将来的问题，我们也可以尝试抽象出这种模型，区分互相不可信的实体，然后在不可信、不安全的基础上，尽可能地实现相对的可信和安全。

**进一步学习和行动**

Android 的开发文档Best Practices: Designing for Security和源码文档Tech Info: Security分别从开发和系统实现的角度介绍了系统的安全机制。另外，viaForensics提供了名为42+ Best Practices: Secure mobile development for iOS and Android的在线教程，更详细地介绍了移动软件面临的安全威胁，并给出了安全开发实践策略。

社区方面，从Android安全开发的角 度，Stack-Overflow并不一定是很好的选择—其中一些最佳回答没有考虑安全，直接使用可能产生问题。Google Group的anroid-security-discuss讨论组则更为专业和准确。OWASP成立了一个Mobile Security工作组，目前已发布Top Ten Mobile Risks等多份白皮书，并举办了AppSec会议。这个工作组的效率虽然不高，但产出质量非常棒。

学术方面，2011和2012年的四大会议及其work-shop上均有移动软件漏洞挖掘和攻击阻止的论文出现，从它们的related works部分可以综合快速地了解学术界的思路。

目前的移动开发还没有形成如此成熟的体系，这也许与其轻快敏捷的互联网产品开发风格有关。但我相信，真正实效的移动软件安全开发，最终依然会融合到需求分析、系统设计、开发实现、测试验证、部署运维等每一个环节，从而与PC平台的SDL殊途同归。

**作者肖梓航，网名Claud，安天实验室高级研究员，主要方向是移动反病毒和移动软件安全，发起或参与了多个移动安全开源项目。博客：**[http://claudxiao.net](http://claudxiao.net/)

