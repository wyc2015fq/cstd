# SQLite大漏洞！所有Chromium浏览器中招，安卓iOS应用也受殃及 - 知乎
# 



> 允中 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

一个SQLite大bug，影响之大、范围之广，不容小觑。

今天，腾讯Blade安全团队发现的SQLite漏洞披露，他们将该漏洞称为“Magellan”，即麦哲伦。

利用麦哲伦漏洞，攻击者可以在用户电脑上远程运行恶意代码，还会导致程序内存泄露或程序崩溃。


而且由于SQLite被嵌入到数千款应用中，因此麦哲伦漏洞会影响许多软件。

范围涵盖物联网设备和桌面软件，甚至包括网络浏览器、安卓和iOS应用。

SQLite太大，现在责任也越大。

## **SQLite？**

SQLite，全球最受欢迎的数据库之一。

在2017年发布的调查中，排名第三。

SQLite一方面可以被用于嵌入式的开发中，在一些内存很小的设备上面，它天生骄傲， 比MySQL、SQL Server之类的数据库管理软件不知高到哪里去。

SQLite占用内存小，但满足关系数据库的大部分特征，同时执行SQL语句的效率都是非常高。

另一方面，还能在后端使用来提高性能，在相同的情况下，一样的查询，它比MySQL什么的数据库快得多。

作为基础组件库的SQLite也作为扩展库被许多程序使用。例如PHP、Python、Java等等,攻击者可通过攻击代码，在一些进程的上下文中在本地或远程任意执行代码，或导致软件的拒绝服务。

并且SQLite作为嵌入式数据库，支持大多数SQL标准，实现了无服务器、零配置、事务性的SQL数据库引擎，在网页浏览器、操作系统、嵌入式系统中使用较为广泛。

具体什么样的产品会中招？
![](https://pic1.zhimg.com/v2-23631816276cf2bacc4a2c438442c2b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='728' height='380'></svg>)



## **Chromium系浏览器中招**

只要是支持SQLite和Web SQL API的浏览器，就有中招可能。

因为攻击者可以将破解代码转变成常规的SQL语法，便可在用户访问网页时对其加以利用。

火狐和Edge并不支持这种API，但基于Chromium的开源浏览器都支持这种API。

也就是说，谷歌Chrome、Vivaldi、Opera和Brave都会受到影响。

不光网络浏览器会遭受攻击，其他产品和应用也会受到影响。例如，Google Home就面临安全威胁。

发现麦哲伦漏洞的腾讯Blade团队报告称，他们利用这个漏洞成功黑进了Google Home.
![](https://pic2.zhimg.com/v2-9401bbdd5f48b64a729299aa610e7d59_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)



## **有补丁但隐患仍在**

目前，官方补丁已出。

腾讯Blade安全团队表示，他们曾在今年秋初向SQLite团队报告过麦哲伦漏洞，12月1日已经通过SQLite 3.26.0发送了补丁。

上周发布的谷歌Chrome 71，也已经修补该漏洞。Vivaldi和Brave等基于Chromium的浏览器，都采用最新版本的Chromium。

但Opera仍在运行较老版本的Chromium，因此仍会受到影响。

另外，虽然并不支持Web SQL，但火狐也会受到这个漏洞的影响，原因在于他们使用了可以在本地访问的SQLite数据库，因此本地攻击者也可以使用这个漏洞执行代码。

而且，由于升级所有桌面、移动或网页应用的底层数据库引擎，危风险不小，经常导致数据损坏，所以多数程序员都会尽可能向后推迟。

这样一来，即使SQLite团队发布了官方补丁，但很多应用仍会在今后几年面临威胁。



![](https://pic2.zhimg.com/v2-64007cd997e86b90316531b093ce8935_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='223'></svg>)



## **处置建议**

腾讯Blade安全团队建议，使用Chromium系产品的团队，请尽快更新至官方稳定版本71.0.3578.80，如果使用产品中涉及SQLite，请更新到3.26.0.

另外，如暂时没有条件采用官方提供的修补方案，也有一些应急建议方案：
- 1）关闭SQLite中的fts3功能；
- 2）禁用WebSQL：编译时不编译third-party的sqlite组件。由于WebSQL没有任何规范，目前仅有Chrome、Safari支持。

最后，验证方法：重新编译后的内核应无法在控制台调用openDatabase函数。

## **问答腾讯Blade安全团队**

**量子位：腾讯Blade团队如何发现该漏洞？**

**Tencent Blade Team：**麦哲伦是一组漏洞的统称，我们在安全研究过程中，通过人工代码审计与自动化测试发现了这些漏洞。

**量子位：预计该漏洞存在的时间有多长了？**

**Tencent Blade Team：**根据SQLite的官方提交记录，其中一个危害严重的漏洞可能已经存在了8年之久。

**量子位：有什么处置、修补方案建议？**

**Tencent Blade Team：**如果有软件使用了SQLite，建议升级到最新的SQLite 3.26.0 版本。如果您的产品使用了Chromium浏览器内核，请更新至官方最新稳定版本71.0.3578.80，即可解决此安全问题。也请参考[https://blade.tencent.com/magellan/](https://link.zhihu.com/?target=https%3A//blade.tencent.com/magellan/)，我们会定期同步相关的更新信息。

**量子位：有评价说“开发者很少更新代码库及其应用的组件部分，所以这会是一次会影响数年的漏洞”，你们认同吗？有无建议？**

**Tencent Blade Team：**基本认同，这个漏洞影响面非常广泛，涉及各种PC/手机/物联网设备，许多设备中的软件更新十分缓慢甚至已经停止更新，我们预计这个漏洞在未来3年内都将产生持续的影响。在国际上，我们联合了Google，Apple，Facebook，Microsoft及SQLite官方安全团推动漏洞修复进展，在国内我们通过CNCERT发布了漏洞预警公告。我们也建议开发者及时更新安全补丁，避免导致用户数据泄露等风险。

**量子位：腾讯Blade团队还利用SQLite bug攻破了Google Home，意味着该漏洞在IoT领域也会被利用？难度大吗？有试过其他国内公司的IoT设备吗？**

**Tencent Blade Team：**是的，该漏洞也影响众多IoT设备，因为现在许多IoT设备系统与软件中同样使用了SQLite数据库。漏洞的利用难度取决于设备是否有一个能够触发漏洞利用代码的攻击面，在Google Home上，我们实现无接触式的局域内远程攻击，利用条件十分简单。我们相信国内其它公司的IoT设备也同样可能会受此漏洞影响。

**量子位：终端用户防护层面，是否有什么建议？**

**Tencent Blade Team：**建议用户及时关注系统与软件更新通知。

**传送门：**

漏洞报告：[https://blade.tencent.com/magellan/index_en.html](https://link.zhihu.com/?target=https%3A//blade.tencent.com/magellan/index_en.html)

chrom更新：[https://chromereleases.googleblog.com/2018/12/stable-channel-update-for-desktop.html](https://link.zhihu.com/?target=https%3A//chromereleases.googleblog.com/2018/12/stable-channel-update-for-desktop.html)

SQLite补丁：[https://www.sqlite.org/releaselog/3_26_0.html](https://link.zhihu.com/?target=https%3A//www.sqlite.org/releaselog/3_26_0.html)

— **完** —
量子位 · QbitAI
վ'ᴗ' ի 追踪AI技术和产品新动态

[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)
**诚挚招聘**
量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！
相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。




