# EIGRP和OSPF的双向双点路由重分布 - weixin_33985507的博客 - CSDN博客
2013年09月05日 16:25:17[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：51

**一.概述：**
  做路由重分布的时候一般都需要做双向重分布，但是如果有冗余路径，进行双向双点的路由重分布，就有可能会出现路由环路或次优路由的问题。
  对于EIGRP外部路由条目，只能通过route-map来修改metric，而不能用distance命令把路由用ACL挑出来修改管理距离，因此对于EIGRP的外部路由的次优路径，如果通过EIGRP来解决，只能修改metric。参考下面的链接：![spacer.gif](http://333234.blog.51cto.com/e/u/themes/default/images/spacer.gif)[http://www.groupstudy.com/archives/ccielab/200309/msg00018.html](http://www.groupstudy.com/archives/ccielab/200309/msg00018.html)
You can change the distance of EIGRP on a per prefix basis, but only for internal EIGRP routes.
**二.测试拓扑：**
![162109175.jpg](http://blog.51cto.com/attachment/201309/162109175.jpg)
**三.双向双点重分布带来的问题及解决思路：**
**A.EIGRP外部路由5.5.5.0/24**
---把EIGRP 500的5.5.5.0/24作为外部路由引入到EIGRP 100，SW1学习到的管理距离为170，假如R1先学习到该路由，那它会把该条路由以OE2的方式重分布到OSPF,那么R4就可以通过OSPF和EIGRP分别学习到该条路由，因为OSPF的管理距离为110，比EIGRP的外部路由的管理距离170要小，所以R4认为到达5.5.5.0/24的路由走OSPF，而非左侧的eigrp，出现了次优路由。
----紧接着R4又会把OSPF路由重分布到EIGRP，从而使得SW1学习到两条到5.5.5.0/24的等价路由，一个走R4，一个走R5。
**解决思路一：****通过调整distance来解决**
①用ACL把所有EIGRP的外部路由都抓出来，再在R3和R4上OSPF中用distance指定router-id和ACL,将管理距离设置大于170
**解决实例：**
**R3：**
access-list 5 permit 5.5.5.0 0.0.0.255
router ospf 1
     router-id 3.3.3.3
     distance 171 4.4.4.4 0.0.0.0 5
**R4：**
access-list 5 permit 5.5.5.0 0.0.0.255
router ospf 1
     router-id 4.4.4.4
     distance 171 3.3.3.3 0.0.0.0 5
②因为是EIGRP外部路由，无法通过distance、IP Source address和ACL来设定指定路由的管理距离，调整EIGRP外部路由的管理距离能解决5.5.5.0/24的次优路由，但是又会产生新的次优路由，不建议使用。
**解决思路二:通过路由重分布打tag解决**
①每个分布点将EIGRP重分布到OSPF的时候，打上tag，另外一个重分布点用route-map将所有打tag的路由丢弃。
----这样配置能解决次优路径和路由环路的问题，但是其中一个分布点与EIGRP的邻居出现问题时，也没有了冗余路径,所以GNS中采用思路一的方式解决。
**解决实例：**
**R3和R4：**
route-map tag10 deny 10
match tag 10
route-map tag10 permit 20
router ospf 1
      redistribute eigrp 100 subnets tag 10
      distribute-list route-map tag10 in
**B.分布点OSPF宣告的直连路由或****重分布的直连路由**
----比如R3的直连路由3.3.3.0/24，以OSPF宣告出去，它会把这条路由发布到OSPF，同时也会把这条OSPF路由重分布到EIGRP，R4通过OSPF学习到这条路由，也会把它重分布到EIGRP中，从而导致SW1有两条去往3.3.3.0/24的等价路径，一个走R3，一个走R4，显然走R4的这条属于次优路径。
----如果分布点的直连路由在EIGRP和OSPF都没有宣告，而是以重分布方式进入，如果EIGRP和OSPF都宣告的话，同样会出现B的两条等价路由，分别走两个分布点。
**解决思路：****通过调整metric来解决**
①通过ACL，每个重分布点把另外一个分布点的直连路由或重分布进来的路由给抓出来，在重分布点EIGRP的连接的out方向利用route-map修改metric值
**解决实例：**
**R3：**
access-list 4 permit 4.4.4.0 0.0.0.255
route-map 4addmetric permit 10
      match ip address 4
      set metric 361120
route-map 4addmetric permit 20
router eigrp 100
      distribute-list route-map 4addmetric out Ethernet0/0
**R4：**
access-list 3 permit 3.3.3.0 0.0.0.255
route-map 3addmetric permit 10
      match ip address 3
      set metric 361120
route-map 3addmetric permit 20
router eigrp 100
      distribute-list route-map 3addmetric out Ethernet0/0
**四.GNS3拓扑和配置文件：**
----参见上传的附件。
