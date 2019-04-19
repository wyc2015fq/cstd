# 什么是RFC？ - Koma Hub - CSDN博客
2019年04月14日 10:28:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：9
个人分类：[RFCs](https://blog.csdn.net/Rong_Toa/article/category/8856653)
名词：
- IETF - 互联网工程任务组 - The Internet Engineering Task Force
- RFC - Request For Comments - 请求评论（直译）
以下内容来自百度百科。
**Table of Contents**
[RFC （一系列以编号排定的文件）](#RFC%20%EF%BC%88%E4%B8%80%E7%B3%BB%E5%88%97%E4%BB%A5%E7%BC%96%E5%8F%B7%E6%8E%92%E5%AE%9A%E7%9A%84%E6%96%87%E4%BB%B6%EF%BC%89)
[编辑机制](#%E7%BC%96%E8%BE%91%E6%9C%BA%E5%88%B6)
[处理过程](#%E5%A4%84%E7%90%86%E8%BF%87%E7%A8%8B)
[历史](#%E5%8E%86%E5%8F%B2)
[文件架构](#%E6%96%87%E4%BB%B6%E6%9E%B6%E6%9E%84)
[发展历程](#%E5%8F%91%E5%B1%95%E5%8E%86%E7%A8%8B)
[分类](#%E5%88%86%E7%B1%BB)
[1.STD RFC](#1.STD%20RFC)
[2.BCP RFC](#2.BCP%20RFC)
[3.FYI RFC](#3.FYI%20RFC)
[4.其他RFC](#4.%E5%85%B6%E4%BB%96RFC)
[文档阅读](#%E6%96%87%E6%A1%A3%E9%98%85%E8%AF%BB)
[参考资料](#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)
# RFC （一系列以编号排定的文件）
Request For Comments（RFC），是一系列以编号排定的文件。文件收集了有关互联网相关信息，以及UNIX和互联网社区的软件文件。目前RFC文件是由Internet Society（ISOC）赞助发行。基本的互联网通信协议都有在RFC文件内详细说明。RFC文件还额外加入许多的论题在标准内，例如对于互联网新开发的协议及发展中所有的记录。因此几乎所有的互联网标准都有收录在RFC文件之中。
## 编辑机制
Request For Comments (RFC），是一系列以编号排定的文件。文件收集了有关因特网相关资讯，以及UNIX和因特网社群的软件文件。目前RFC文件是由Internet Society（ISOC）所赞助发行。
基本的因特网通讯协定都有在RFC文件内详细说明。RFC文件还在标准内额外加入了许多的论题，例如对于因特网新开发的协定及发展中所有的记录。因此几乎所有的因特网标准都收录在RFC文件之中。
RFC(Request For Comments)-意即“请求协议”，包含了关于Internet的几乎所有重要的文字资料。如果你想成为网络方面的专家，那么RFC无疑是最重要也是最经常需要用到的资料之一，所以RFC享有网络知识圣经之美誉。通常，当某家机构或团体开发出了一套标准或提出对某种标准的设想，想要征询外界的意见时，就会在Internet上发放一份RFC，对这一问题感兴趣的人可以阅读该RFC并提出自己的意见；绝大部分网络标准的指定都是以RFC的形式开始，经过大量的论证和修改过程，由主要的标准化组织所指定的，但在RFC中所收录的文件并不都是正在使用或为大家所公认的，也有很大一部分只在某个局部领域被使用或并没有被采用，一份RFC具体处于什么状态都在文件中作了明确的标识
RFC由一系列草案组成，起始于1969年（第一个RFC文档发布于1969年4月7日，参见“RFC30年”，RFC2555”），RFC文档是一系列关于Internet（早期为ARPANET）的技术资料汇编。这些文档详细讨论了计算机网络的方方面面，重点在网络协议，进程，程序，概念以及一些会议纪要，意见，各种观点等。
“RFC编辑者”是RFC文档的出版者，它负责RFC最终文档的编辑审订。“RFC编辑者”也保留有RFC的主文件，称为RFC索引，用户可以在线检索。在RFC近30年的历史中，“RFC编辑者”一直由约翰·普斯特尔（Jon Postel）来担任，而现在“RFC编辑者”则由一个工作小组来担任，这个小组受到“互联网协会”（Internet Society）的支持和帮助。
RFC编辑者负责RFC以及RFC的整体结构文档，并维护RFC的索引。Internet协议族的文档部分（由Internet工程委员会“因特网工程师任务组”IETF以及IETF 下属的“因特网工程师指导组”IESG 定义），也做为RFC文档出版。因此，RFC在Internet相关标准中有着重要的地位。
RFC编辑者的职责是由Internet 中的大家提议形成的，所出版的语言也就和Internet一样。IETF和ISOC是代表了世界各地的国际性组织，英语是IETF的第一工作语言，也是IETF的正式出版语言。RFC 2026 "The Internet Standards Process -- Revision 3" 允许RFC翻译成其他不同的语言。但是不能保证其翻译版本是完全正确的。因此，RFC编辑不对非英语的版本负责，而只是指明了哪里有非英语的版本，将这些信息列在WEB页上。
## 处理过程
一个RFC文件在成为官方标准前一般至少要经历4个阶段【RFC2026】：因特网草案、建议标准、草案标准、因特网标准。
第一步RFC的出版是作为一个Internet 草案发布，可以阅读并对其进行注释。准备一个RFC草案，我们要求作者先阅读IETF的一个文档"Considerations for Internet Drafts". 它包括了许多关于RFC以及Internet草案格式的有用信息。作者还应阅读另外一个相关的文档RFC 2223 "Instructions to Authors"。
一旦文档有了一个ID号后，你就可以向rfc-editor @rfc-editor. org发送e-mail ，说你觉得这个文档还可以，能够作为一个有价值或有经验的RFC文档。RFC编辑将会向IESG请求查阅该文档并给其加上评论和注释。你可以通过RFC队列来了解你的文档的进度。一旦你的文档获得通过，RFC编辑就会将其编辑并出版。如果该文档不能出版，则会有email通知作者不能出版的原因。作者有48个小时的时间来校对RFC编辑的意见。我们强烈建议作者要检测拼写错误和丢字的错误，应该确保有引用，联系和更新相关的信息。如你的文档是一个MIB，我们则要你对你的代码作最后一次检测。一旦RFC文档出版，我们就不会对其进行更改，因此你应该对你的文档仔细的检查。
有时个别的文档会被正从事同一个项目的IETF工作组收回，如是这种情况，则该作者会被要求和IETF进行该文档的开发。在IETF中，Area Directors (ADs) 负责相关的几个工作组。这些工作者所开发的文档将由ADs 进行校阅，然后才作为RFC的出版物。
如要获得关于如何写RFC文档和关于RFC的Internet标准制定过程的更多详细信息，请各位参见：
RFC 2223 "Instructions to RFC Authors"。
RFC 2026 "The Internet Standards Process -- Revision 3"。
实际上，在Internet上，任何一个用户都可以对Internet某一领域的问题提出自己的解决方案或规范，作为Internet草案（Internet Draffs，ID）提交给Internet工程任务组（IETF）。草案存放在美国、[欧洲](https://baike.baidu.com/item/%E6%AC%A7%E6%B4%B2)和亚太地区的工作文件站点上，供世界多国自愿参加的IETF成员进行讨论、测试和审查。最后，由Internet工程指导组（IESG）确定该草案是否能成为Internet的标准。
如果一个Internet草案在IETF的相关站点上存在6个月后仍未被IESG建议作为标准发布，则它将被从上述站点中删除。事实上，在任何时候，一个Internet 草案都有可能被新的草案版本所替换掉，并重新开始6个月的存放期。
如果一个Internet草案被IESG确定为Internet的正式工作文件，则被提交给Internet体系结构委员会（IAB），并形成具有顺序编号的RFC文档，由[Internet协会](https://baike.baidu.com/item/Internet%E5%8D%8F%E4%BC%9A)（ISOC）通过Internet向全世界颁布。每个Internet标准文件在被批准后都会分配一个独立于RFC的永久编号，这就是STD编号。有一个不断被更新的文件RFC-INDEX.TXT按照RFC的编号来索引所有的文件，对于因特网标准文件还列出了其相应的STD编号。
RFC文档必须被分配RFC编号后才能在网络上发布。例如，RFC2026的内容是“Internet标准进程-修订版3”、RFC1543的内容为“RFC作者指导”等等。需要时，可以复制或打印这些联机文档。用户也可以通过遍布全世界的数个联机资料数据库中获得RFC文档。
作为标准的RFC又分为几种，第一种是提议性的，就是说建议采用这个作为一个方案摆出来，Draft是已经有一部分在用了，希望被采用为正式的标准，还有一种就是完全被认可的标准，这种是大家都在用，而且是不应该改变的。还有一种就是现在的最佳实践法，它相当于一种介绍。这些文件产生的过程是一种从下往上的过程，而不是从上往下，也就是说不是一个由主席，或者由工作组负责人的给一个指令，说是要做什么，要做什么，而是有下边自发的提出，然后在工作组里边讨论，讨论了以后再交给刚才说的工程指导委员会进行审查。但是工程指导委员会只做审查不做修改，修改还是要打回到工作组来做。IETF工作组文件的产生就是任何人都可以来参加会议，任何人都可以提议，然后他和别人进行讨论，大家形成了一个共识就可以产出这样的文件。
## 历史
RFC[文件格式](https://baike.baidu.com/item/%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F)最初作为ARPA网计划的基础起源于1969年。如今，它已经成为IETF、Internet Architecture Board (IAB）还有其他一些主要的公共[网络研究](https://baike.baidu.com/item/%E7%BD%91%E7%BB%9C%E7%A0%94%E7%A9%B6)社区的正式出版物发布途径。
最初的RFC作者使用[打字机](https://baike.baidu.com/item/%E6%89%93%E5%AD%97%E6%9C%BA)撰写文档，并在[美国国防部](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD%E5%9B%BD%E9%98%B2%E9%83%A8)国防前沿研究项目署（ARPA）研究成员之间传阅。1969年12月，他们开始通过ARPANET途径来发布新的RFC文档。第一份RFC文档由[洛杉矶加利福尼亚大学](https://baike.baidu.com/item/%E6%B4%9B%E6%9D%89%E7%9F%B6%E5%8A%A0%E5%88%A9%E7%A6%8F%E5%B0%BC%E4%BA%9A%E5%A4%A7%E5%AD%A6)（UCLA）的Steve Crocker撰写，在1969年4月7日公开发表的RFC 1。当初Crocker为了避免打扰他的室友，是在浴室里完成这篇文档的。
在1970年代，很多后来的RFC文档同样来自UCLA，这不仅得益于UCLA的学术质量，同时也因为UCLA是ARPANET第一批Interface Message Processors (IMPs）成员之一。
由Douglas Engelbart领导的，位于Stanford Research Institute的Augmentation Research Center (ARC）是四个最初的ARPANET结点之一，也是最初的Network Information Centre，同时被社会学家Thierry Bardini记录为早期大量RFC文档的发源地。
从1969年到1998年，Jon Postel一直担任RFC文档的编辑职务。随着美国政府赞助合同的到期，Internet Society（代表IETF），和[南加州大学](https://baike.baidu.com/item/%E5%8D%97%E5%8A%A0%E5%B7%9E%E5%A4%A7%E5%AD%A6)（USC）Information Sciences Institute的网络部门合作，（在IAB领导下）负责RFT文档的起草和发布工作。Jon Postel继续担任RFC编辑直到去世。随后，由Bob Braden接任整个项目的领导职务，同时Joyce Reynolds继续在团队中的担任职务。
庆祝RFC的30周年的RFC文件是RFC 2555。
## 文件架构
RFC文件只有新增，不会有取消或中途停止发行的情形。但是对于同一主题而言，新的RFC文件可以声明取代旧的RFC文件。RFC文件是纯 ASCII文字档格式，可由电脑程序自动转档成其他档案格式。RFC文件有封面、目录及页首页尾和页码。RFC的章节是数字标示，但数字的小数点后不补零，例如4.9的顺序就在4.10前面，但9的前面并不补零。RFC1000这份文件就是RFC的指南。 [1]
RFC文件是由Internet Society审核后给定编号并发行。虽然经过审核，但RFC也并非全部严肃而生硬的技术文件，偶有恶搞之作出现，尤其是4月1日愚人节所发行的，例如RFC 1606: A Historical Perspective On The Usage Of IP Version 9 （参见IPv9）、RFC 2324: “超文本咖啡壶控制协议”（Hyper Text Coffee Pot Control Protocol，乍有其事的写了HTCPCP这样看起来很专业的术语缩写字）。以及如前面所提到纪念RFC的30周年庆的RFC文件。
## 发展历程
在Internet从诞生到不断发展壮大的过程中，出现过各种各样的协议和思想讨论，从最初的NCP协议到现代Internet的基石TCP/IP协议族，无一不闪耀着研究人员的智慧光芒，正是这些成百上千各种协议的发明、讨论和完善，才使得人类社会逐步进入到互联网时代。而这些闪耀着人类智慧结晶的思想成果大都以一种称为RFC的文档格式记录起来。
1969年，S·Crocker首先建立了RFC机制，其目的是建立一种快速共享Internet网络研究思想的方式，最初RFC是以书面形式分发的，后来有了FTP、Email,RFC就以在线电子文本的形式提供，当然现在通过WWW在很多站点可以很方便地访问RFC文档。RFC一直以来主要是用于Internet的标准化，RFC是Internet开放性的产物，任何人都可以访问RFC,Internet这一致力于信息共享的网络首先共享的就是以RFC形式出现的涉及其自身研究、设计和使用的信息。这一独特的方式对于Internet的发展、完善具有相当关键的作用。发展到现在，RFC文档已不仅仅是关于Internet标准的文档了，而且也不局限于TCP/IP范围，它几乎包含了与计算机通信有关的任何内容，全面反映Internet研究、发展的过程。RFC主要是IAB、IETF、IESG、ISOC的工作成果，主要由IETF起草，由IAB指导下的RFC 编辑（Editor）直接负责RFC的发表。每一个RFC文档有一个编号，这个编号永不重复，也就是说，由于技术进步等原因，即使是关于同一问题的RFC，也要使用新的编号，而不会使用原来的编号，时至今日[2015/2/2]，RFC编号已经排到7443，在查找RFC时，一定要注意最新的RFC。
## 分类
RFC文档大致可以分为以下几类。
### 1.STD RFC
按照RFC1311的定义，STD RFC是指那些已经或者致力于成为Internet标准的RFC。只有经过完全Internet标准化过程的RFC才可以有STD编号，STD编号是不变的，而其涉及到的 RFC文档可能不只一个，其RFC编号也会更新。如STD13(Domain Name System）就涉及RFC1 034和RFC1035。STD的标准化过程要经过几个步骤，首先由IETF起草标准（也可能是其他组织和个人，但一般都是和IETF共同完成的），形成Internet Draft(ID),ID没有RFC编号。如果ID在6个月内IESG没有建议成为RFC，则取消此ID。成为RFC后，还要经过一系列的审查、修订、测试等才能最终成为Internet标准。
### 2.BCP RFC
由于Internet应用领域广泛，各种不同的组织有不同的使用目的和使用规则，IETF除了建议STD以外，也有必要对于Internet的使用和管理提供一些一般性的指导，同时也为I ETF、IAB、IESG提供一种渠道，以便推动某一方面的工作，反映其技术趋向，反映这些组织本身的工作进展。于是，1995年以RFC1818定义了BCP，即Best Current Practice。BCP同时有一个BCP编号和一个RFC编号，一旦约定了一个BCP编号，就不会再变，而其RFC编号则可能会经过修订不断更新。例如反映Internet标准化工作程序的BCP9的RFC编号就从RFC16 02上升到RFC2026，相应地就废弃了RFC1602。BCP在发表以前，以电子邮件的形式广泛征求IETF的意见，经过IESG的审查，通过后即正式发表。但是BCP本身不是Internet标准。
### 3.FYI RFC
FYI是For Your Information的简写，1990年发表的RFC1150(FYI1）定义了FYI,FYI也同时有一个FYI编号和一个RFC编号，FYI编号是固定的。FYI主要是提供有关Internet的知识性内容。如FYI4(RFC1594),"Answers to Commonly asked New Internet User Quest ions"。所有的FYI在提交到RFC编辑以前，必须先经过IETF的User Services WorkingGro up审查。
### 4.其他RFC
除了STD、BCP、FYI以外还有其他一些RFC。从RFC899开始，所有以99结尾的RFC都是对此前99个RFC的一个概括。如RFC1999就是对RFC1900到RFC1999的一个简单概括。除了上述分类以外，还有一些描述RFC的方法。与Internet标准化过程（Internet Standards Process）有关的规范可以分为两类，即 Technical Specification(TS),Applicability Statement(AS）。TS是对协议、规则、格式、实用程序的描述。AS是描述在何种环境，以及怎样在Internet中使用TS;AS所涉及的并不一定全是Internet标准，比如IEEE、ITU、ISO组织的一些标准，大家所熟悉的ASCII标准就是一例。AS应该对其涉及的TS规定相应的级别"Requirement Level",这些"Require ment Level"如下： ·Required(Req），相当于必须实现，如IP、ICMP; ·Recommended(Rec），鼓励使用，如TELNET; ·Elective(Elc），可选择的； ·Limited Use，只限于特定的用户，一般说来用于对一些新的协议做试验； ·Not Recommended，不要使用，很可能是过时的。"Maturity Level"也是用来描述TS和AS的一种方式，它反映这些标准是否成熟。对于致力于成为STD的TS和AS有三种"Maturity Level"。·Proposed Standard，基本成熟，但还需要进一步的试验证实其可行性。除非是用来验证该协议的可行性，不要将其视为标准实现。·Draft Standard，需要两个独立的，而且具有相互操作性的实例验证该协议的每一个方面。可以将其视为最终的标准草案； ·Internet Standard，最终的Internet标准，同时赋予一个STD编号。除此之外的TS和AS分为以下几种"Maturity Level"。·Experimental，一般是反映一些研究和开发的成果，只应将此看作是一般性的信息。·Informational，反映与Internet标准有关的一般性信息。有些也是有关非Intern et组织开发的一些协议，但必须得到协议开发者的许可。·Historic，是一些被新的标准取代或者是已经过时废弃不用的标准。STD1(RFC2200）——Internet Official Protocol Standards，定期更新，反映最新的 Internet标准。另外，对于关注Internet的人来说，应该经常注意查阅BCP9的最新内容。
## 文档阅读
一是需要确定它是最新的文档，二是需要注意RFC文档的类别；
所有的RFC文档都要经历评论和反馈过程，并且在这一段时间内它们会被划分为不同的类别；
RFC文档一旦被提交，IETF和IAB组织将审查RFC文档，通过后可以成为一项标准；
RFC文档按照它发展与成熟的过程可以分为标准、草案标准、提案标准、实验性的、信息性或历史性的；
RFC文档又可以分为被要求、被推荐、被选择、受限制使用或不被推荐；
RFC文档是什么。
## 参考资料
- 1.[](https://baike.baidu.com/item/RFC/1840?fr=aladdin#ref_%5B1%5D_5073687)[RFC 1000 - Request For Comments reference guide](https://baike.baidu.com/reference/1840/252f3AVTaqF1ddoefjln0G7RBIAipd42i0lXS2t-X81HOrepMUMpABl4XOHgHdbFmxDQ_hmbB_AIfYuh-e6KimJT) ．Internet FAQ Archives．1987-8[引用日期2014-01-14]

