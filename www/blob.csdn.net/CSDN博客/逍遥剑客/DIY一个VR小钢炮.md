# DIY一个VR小钢炮 - 逍遥剑客 - CSDN博客
2016年06月12日 13:57:40[xoyojank](https://me.csdn.net/xoyojank)阅读数：9786标签：[VR																[DIY																[ITX](https://so.csdn.net/so/search/s.do?q=ITX&t=blog)](https://so.csdn.net/so/search/s.do?q=DIY&t=blog)](https://so.csdn.net/so/search/s.do?q=VR&t=blog)
个人分类：[虚拟现实																[乱七八糟](https://blog.csdn.net/xoyojank/article/category/259560)](https://blog.csdn.net/xoyojank/article/category/5954851)
# 背景
去年做了一个VR Demo后, 总是有各种参观啊, 演示啊, 讲座啊等, 然后很少有合格的电脑跑得动…有时又想现场演示下, 就只能把工作站抬过去了.  
来回折腾几次就下定决心要装一台VR专用的演示机, 要求很明确: 小. 
品牌主机如果想要GTX980级别显卡配置的, 几乎要么体积巨大, 要么价格巨贵, 排除.  
那就自己DIY小机箱的PC吧, 只能选ITX规格了, 要求满足以下条件:
- 能上独立显卡, 最好是顶级公版显卡(NV信仰核弹)
- 散热要好, [几年前配的屌丝伯](http://blog.csdn.net/xoyojank/article/details/8136177)就是个闷罐子
- 必须能上多块硬盘, SSD是必需的, 数据盘也要有
- 满足以上条件的情况下, 体积尽量小
- 方便携带, 最好有个提手
几年前也折腾过ITX机箱, 其实那次挺失败的. 业余泡Chiphell论坛的同时, 也考察了多款ITX机箱, 如FT03Mini, RS07, FTZ01, ML08等, 直到看到有人在晒定制机箱, 就像发现了一块新大陆. 最后在孙总和赵总的作品中选择了mk3s, 因为它能加装提手. 这一等就等了两个多月…还好机箱的做工非常满意, 最终效果颜值也算惊艳. 
# 配置清单
|配置|型号|来源|备注|
|----|----|----|----|
|机箱|H’z Mk3s mITX|淘宝|侧透+提手+风道+扩展性+体积+做工, 都是优点|
|CPU|intel i7 6700K|代购|本想省钱上E3的, 结果新平台没有ITX民用主板支持了|
|主板|技嘉B150N Phoenix-WIFI|京东|我不超频(Z170就算了), USB3.0口要多(可恨的OculusRift+Touch+手柄就占用了4个), 要有WIFI(家里没网口), 最重要的是它有灯(机箱侧透)|
|内存|金士顿 DDR4 2400 8GBx2|京东|说是送耳机, 结果一算帐就是打包卖的价-_-|
|显卡|公版GTX980Ti|借的|等GTX1070上市了来一发, 1080跺不了手|
|SSD(SATA)|OCZ Arc100 240G|京东|看性价比不错, 论坛评价性能也过得去|
|SSD(M.2)|三星PM951 512G|淘宝|简直白菜价, 买一个当数据盘先用着, 3.5寸硬盘就先不上了, 家里有块2T的|
|灯板|小熊mod|淘宝|为了逼格, 跟mao989学的|
|CPU散热器|利民 Macho90|淘宝|没上猫扇(丑), 这款颜值跟机箱比较搭, 而且还送背板了. 本来还要纠结要不要上灯扇的, 为了风道放弃了|
|机箱风扇|安耐美 白蝠 14cm|淘宝|主要是静音, 灯是次要的|
|电源|银欣SFX-L 500W|闲鱼|正好有一哥们跟定制线打包卖, 收了, 巧的是同款机箱用的|
|定制线材|自制|闲鱼|同上|
|防尘网|银欣14cm x1, 12cm x2|京东|可有可无的东西|
![这里写图片描述](https://img-blog.csdn.net/20160612133509213)
# 装机过程
这块板的槽点: USB Type-C接口在上了后置挡板后根本插到不底, 不知道设计人员上机试过没有-_- 
![这里写图片描述](https://img-blog.csdn.net/20160612133546856)
CPU没什么好说的, 别装反了就行 
![这里写图片描述](https://img-blog.csdn.net/20160612133710450)
散热器装起来有点小麻烦, 扣具配件真多 
![这里写图片描述](https://img-blog.csdn.net/20160612133807388)
跟散热器一比, 主板显得好小 
![这里写图片描述](https://img-blog.csdn.net/20160612133852296)
一定要先上内存再上散热器, 还好没有冲突 
![这里写图片描述](https://img-blog.csdn.net/20160612133928264)
M.2接口的SSD, 有新接口就上一个玩玩, 还能省点重量 
![这里写图片描述](https://img-blog.csdn.net/20160612134010420)
机箱送的东西挺多的, 没想到有小号十字螺丝, 没准备好工具, 真是个悲剧 
![这里写图片描述](https://img-blog.csdn.net/20160612134102049)
这机箱很好装, 上下左右前后的面板都能拆下来, 所以不会像其它ITX机箱那样有手伸不进去的尴尬 
![这里写图片描述](https://img-blog.csdn.net/20160612134223749)
ITX的静音电源, 功率大的好像没什么好的选择. 话说银欣的LOGO挺好看的 
![这里写图片描述](https://img-blog.csdn.net/20160612134328302)
灯板上早了…后面装机箱风扇和信号线又要拆下来, 装ITX一定要想好顺序啊 
![这里写图片描述](https://img-blog.csdn.net/20160612134436422)
上了灯板后, 14cm的风扇螺丝就装不上了, 最后用理线扎带绑上的…… 
![这里写图片描述](https://img-blog.csdn.net/20160612134555079)
![这里写图片描述](https://img-blog.csdn.net/20160612134825699)
上显卡. 如果用短卡的话, 机箱应该还能小一号 
![这里写图片描述](https://img-blog.csdn.net/20160612134845325)
又犯了一个错误, 理线时绑到外面了….然后侧板装不上了, 重理 
![这里写图片描述](https://img-blog.csdn.net/20160612134921451)
信号线好难插, 把机箱风扇拆了才插上, 还是安装顺序问题 
![这里写图片描述](https://img-blog.csdn.net/20160612135003877)
合体完毕 
![这里写图片描述](https://img-blog.csdn.net/20160612135054578)
# 最终效果
![这里写图片描述](https://img-blog.csdn.net/20160612135129112)
![这里写图片描述](https://img-blog.csdn.net/20160612135148064)
![这里写图片描述](https://img-blog.csdn.net/20160612135200814)
先拿HTC Vive试下机, Oculus Touch因为NDA的关系就不晒了 
![这里写图片描述](https://img-blog.csdn.net/20160612135211955)
![这里写图片描述](https://img-blog.csdn.net/20160612135221862)
![这里写图片描述](https://img-blog.csdn.net/20160612135231502)
颜值超出预期, 重量也超出预期-_-, 单手拎还是有点累的. 玻璃侧板也是个问题, 有时间去汽车店贴个深色防爆膜~
# 参考资料
平衡的小塔箱——H’z Mk3s 
[https://www.chiphell.com/thread-1311219-1-1.html](https://www.chiphell.com/thread-1311219-1-1.html)
[第二届机王争霸赛]——by（mao989）12月15日更新装机完成 
[https://www.chiphell.com/thread-1422411-1-1.html](https://www.chiphell.com/thread-1422411-1-1.html)
暴走的明日香 EVA的ITX小钢炮 装机SHOW 
[https://www.chiphell.com/thread-1344449-1-1.html](https://www.chiphell.com/thread-1344449-1-1.html)
终于等到你：定制机箱MK3S装机 
[http://post.smzdm.com/p/432218/](http://post.smzdm.com/p/432218/)
小胖变身高富帅——13L极简垂直风道小钢炮show 
[https://www.chiphell.com/thread-1377292-1-1.html](https://www.chiphell.com/thread-1377292-1-1.html)
红色有角明日香：UMX1 PLUS骚红限量版装机SHOW 
[http://post.smzdm.com/p/348176](http://post.smzdm.com/p/348176)
