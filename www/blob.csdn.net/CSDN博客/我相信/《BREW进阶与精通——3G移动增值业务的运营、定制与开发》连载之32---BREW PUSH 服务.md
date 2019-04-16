# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之32---BREW PUSH 服务 - 我相信...... - CSDN博客





2009年12月07日 11:32:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1199标签：[brew																[终端																[服务器																[网络																[活动																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)







Brew PUSH业务是运营商的增值业务衍生服务之一。它是一种基于客户端/服务器应用的推送业务,服务器可以主动的将制定的内容发送到客户端,而无需客户端发出相应请求.具体的讲,通过PUSH服务器组,可以把特定的内容发送到支持BREW的终端上. 

为了更好地开展BREW业务，利用BREW PUSH是推广移动增值业务最有效的营销方式之一。运营商或者CP/SP为开展BREW Push业务，需要获取用户的手机号码与机型对照列表。然后，明确进行BREW Push的业务内容、目的、时间以及用户群范围，并获得应用的ItemID与机型对照列表，

运营商或者CP/SP在进行点对点BREW Push时可对每个用户先后发送两条短信，发送短信间隔自行确定。先发的一条为普通短信，为被推广业务的广告宣传，可以包含如下内容：

a)业务名称及广告内容

b)告知稍后将进行应用推送

c)用户还可进入BREW应用管理器使用搜索功能根据应用名称查找应用。

后发的一条短信为BREW Push短信，将用户直接推送到应用计费下载页面。运营商或者CP/SP可利用PUSH 通道提示用户最近业务的更新。如果同一SP有多项业务，可以向自有用户发送与其有非定制关系的业务的PUSH信息。对由用户端发起的，并要下发到目标用户的业务，SP可在权衡下对推荐次数和推荐人做一定限制，以免因为接口被利用造成大量下发而引起纠纷。运营商或者CP/SP可以通过BREW PUSH推荐其包含积分的业务来开展有奖活动。

通过BREW PUSH业务，运营商能够完善其移动增值业务品牌的特色内容，增加的访问量，增强BREW分发系统的目录表现力，增强基于BREW的业务市场穿透力。实现BREW PUSH业务的网络结构如图7-15所示。




**图 7-15：****BREW PUSH****业务的网络结构****图**

BREW PUSH服务的主要功能有：直接将应用下载目录推送给终端用户；直接将特定栏目推送给终端用户；直接将特定应用推送给终端用户；直接启动终端用户特定的BREW应用，并传递内容参数。](https://so.csdn.net/so/search/s.do?q=活动&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




