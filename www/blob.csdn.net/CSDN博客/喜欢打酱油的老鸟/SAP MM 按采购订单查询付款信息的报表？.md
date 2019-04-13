
# SAP MM 按采购订单查询付款信息的报表？ - 喜欢打酱油的老鸟 - CSDN博客


2018年09月13日 08:54:14[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：325


SAP MM 按采购订单查询付款信息的报表？
上午给P3项目采购部门用户做了一个采购相关的报表的培训。培训过程中，客户的采购部门经理提出了一个问题：有没有报表能查询到各个采购订单的付款情况，显示采购订单号，已付多少，未付多少，未付金额系统建议的付款期又是哪一天等等。并且对方说，业务现在使用的Ariba系统里就有这种报表。
笔者从没有用过与见过传说中的Ariba，也木有玩过SAP FICO模块。所以对于用户的这个问题，笔者当即答复客户说，容我回去查查然后给答复。
笔者了解到FI模块有个报表FBL1N，可以查询open或者cleared的应付款的。但是笔者对于这个报表并不了解，带着用户的问题，笔者饶有兴趣的去研究了FBL1N这只报表。在客户的Quality系统上，按照一定查询条件得到了如下结果：
![](https://img-blog.csdn.net/20180913085221324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

笔者惊喜的发现，通过调整报表布局，可以调出“采购订单号码“字段，显示在报表里。
可是笔者又失望的发现，报表中所有open/cleared item中“采购订单号”列都是空，即使相关的应付款财务凭证是与一个采购订单相关的发票有关。如下图：
![](https://img-blog.csdn.net/20180913085323893?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
以第一个行项目5100025794为例，
![](https://img-blog.csdn.net/20180913085334988?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
相关的Invoice号码是5808387670/2014，显示该Invoice信息，如下图：
![](https://img-blog.csdn.net/20180913085307702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以得知该Invoice是与某个采购订单(8500529766)有关的。但是该采购订单号（8500529766），竟然不能显示在FBL1N的结果里。我们都知道，SAP很讲究保持各个单据之间的Link关系，FBL1N报表里以全空的方式显示采购订单号，不合SAP风格啊。
这到底为啥子呢？这个又引起了笔者的好奇心。
笔者上网查资料得知，这是标准SAP的逻辑。因为FBL1N报表里的open item，会与某个invoice号码有关，但是一个invoice里可能会出现多个采购订单号，所以SAP系统是没办法抓取采购订单号显示在FBL1N报表结果里。
一些同行说，可以通过增强加代码，显示invoice里的第一个item里的采购订单号，比如某同行说：
*I have done it with ABAP changes :*
*-change with trans. FIBF event 1650 : Function module LINE_ITEMS_GET_GKONT*
*- then change in FM get_gkont, so we get the first PO in that Field.*
还有同行提到说可以考虑在Assignment里显示PO号码，通过定义vendor master data里的Sort Key的方式。对于这种方案，有同行表示异议：*We have the purchase order set at the sort key in the vendor master but the assignment field is coming thru as '00000' on FBL1N.*
结合查询资料的结果，以及咨询了若干资深FICO顾问，笔者认为：SAP系统里不存在满足业务这个要求的报表。
2018-09-12 写于杭州市.


