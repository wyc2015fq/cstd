# 基於MySQL+PHP的羽毛球費用查询网站设计 - 小平子的专栏 - CSDN博客





2016年08月26日 17:20:56[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：982








# 问题来源

我们有一个羽毛球小组，不定期组织打球，会产生场地费和球费，每次收钱特别琐碎，所以组长（现任组长其实就是我）每次开学就让大家先交100块，然后记账再从交的费用里面扣。前任组长专业是CS的（其实现任组长也是，也就是我），他设了一个网站（其实是学校的一个服务器），但是这个网站是基于XML的，每次更新都要打开XML文件一项一项地更新，而且还要自己计算每个人的费用。在前任组长把工作转交给我之后我第一件事就是对网站改版，因为作为程序员，简单重复性工作真的是不能忍的事！

但其实我没学过网页编程，不会Javascript和PHP，但是我懂数据库呀！我会C++啊！秉着天下语言源出一宗的思想，我想只要会一种语言，其它都应该是很容易学的吧！结果根本不是……没有网络基础这些语言真的好难学……难虽然难，但还是设计出了一个简易的网站让我可以在网页上更新费用，让大家可以在网页上查询羽球小组的财务状况。服务器用的是实验室的NAS.

# 需求

羽球小组大概有二十多个人，每年都有新进成员，也有人退出（毕业了），每学期大概缴费一次。每次活动产生的费用包括球费和场地费，这些费用由所有参加活动的成员平摊。

因此这个数据库需要记录：
- 小组成员信息
- 成员每次缴费的信息
- 每次活动产生的费用
- 每次活动参加的人员

小组成员可以：
- 查询每次活动的费用记录
- 查询每次活动的出席人员
- 查询自己会费的余额
- 查询自己的缴费记录

我要：
- 输入每次活动的日期
- 输入每次活动参加的人员
- 输入每次活动的费用

# 设计

根据上述需求，该业务的ER图如下。为了充分满足数据库设计的范式要求，设计了三个实体，两个关系，所以一共要有五张表。下图的设计至少满足第二范式（2NF），是否满足第三范式呢？仔细看了一下，好像满足。若要满足更高的范式，就需要拆分更多的表，SQL语句也会更复杂。注意到下图中没有一张表会存储『**某个组员在一次球事中花费了多少钱**』这样的信息，也没有『**某个组员的会费余额是多少**』这样的信息。因此查询这样的信息时SQL语句会稍微复杂一点，但这样的设计会减少表的信息冗余。 
![ER图](https://img-blog.csdn.net/20160826170729913)
# 数据库搭建

数据库使用的是MySQL。Mac上推荐MyAdmin套件，Ubuntu上推荐phpMyAdmin套件。各种设置都设定好了，可以让人直接关注业务。根据以上ER图，设计了五张表，名称分别为：
- memeber_t
- memact_t（对应Attend）
- activity_t
- fee_t
- memfee_t （对应Pay_Fee）

整个网站搭设在实验室的NAS上。NAS真是个好东西，写好后直接往上一丢即可。

# 编码工作与SQL语句编写

数据库的设计不难，难的是网页查询展示的工作以及SQL语句的编写。这个网站后台用的是PHP语言。但我用PHP的时候被实验室的人鄙视了……觉得PHP太古老，应该用Ruby。

在编写网站时用到了AJAX技术，但其实AJAX不是我的研究重点，所以这篇文章先略过吧……

下面给出5张表的截图，针对实际的查询需求编写一些SQL语句。
- 
**member_t**
![member_t](https://img-blog.csdn.net/20160826170849475)- 
**activity_t**
![activity_t](https://img-blog.csdn.net/20160826170946492)- 
**memact_t** （对应Attend） 
![memact_t](https://img-blog.csdn.net/20160826171101915)- 
**fee_t**
![fee_t](https://img-blog.csdn.net/20160826171140915)- 
**memfee_t** （对应Pay_Fee） 
![memfee_t](https://img-blog.csdn.net/20160826171230134)
发现fper_income这个字段表示组员交的会费。发现这个字段中有负数？负数的意思就是退钱给组员，使得总结余为0. 通常是毕业的组员这个字段会出现负数。

所有的查询都是根据姓名来查询，姓名对应member_t表中的**name**字段。下图是这个网站的查询页面（还有个管理页面）截图，很丑，实验室的人看到只有一片揶揄……不过初始目的和主要功能都满足了，作为一个功能性的网站，就不要太注重外观了。 
![页面截图](https://img-blog.csdn.net/20160826171654729)- **根据姓名（name）查询该组员参加的所有打球场次的信息，包括打球日期，场地费，球费，该场次的人数，该组员应付的费用，打球举办的场所（INNER JOIN）**

上述查询横跨三张表的信息，因此需要连接三张表。该组员应付的费用意思是场地费加上球费再除以参与打球的人数。但是有时候有些会员会带朋友来打球，而那位朋友不在系统里，所以这个会员此时就要帮他朋友支付费用，因此在memact_t表中会多出一个**adj_balance**的字段。

实际操作中在后台并不是根据姓名来查询，而是根据**member_t.mid**字段（主键）来查询。假设已经知道user_id，SQL语句如下：

```
SELECT A.adate, A.asite_fee, A.aball_fee, A.aperson_num, A.aplace, B.adj_balance, (A.asite_fee+A.aball_fee)/A.aperson_num + B.adj_balance
FROM
        activity_t AS A, memact_t AS B
WHERE
        memact_t.mid = user_id
AND
        memact_t.aid = activity_t.aid
```

上述SQL语句实际是**INNER JOIN**操作，也可以使用INNER JOIN关键字将上述语句改写成下面的形式：

```
SELECT A.adate, A.asite_fee, A.aball_fee, A.aperson_num, A.aplace, memact_t.adj_balance 
FROM 
    activity_t AS A
INNER JOIN 
    memact_t ON memact_t.aid = activity_t.aid 
AND 
    memact_t.mid = 2
```
- **根据姓名（name）查询该组员的缴费记录，包括缴费日期，每次缴费的费用（INNER JOIN）**

上述操作仍然是INNER JOIN操作，SQL语句如下：

```
SELECT memfee_t.fper_income, fee_t.date 
FROM 
    memfee_t, fee_t 
WHERE 
    memfee_t.mid = user_id AND memfee_t.fid = fee_t.fid
```

这段SQL也可以改写成用关键字INNER JOIN的形式。
- **根据姓名（name）查询该组员的费用结余以，总缴费与总支出**

一个会员的费用结余 = 总缴费 - 总支出。总缴费和总支出都没有直接记录在表中，横跨四张表，要将这三个结果同时写到一个SQL语句有点复杂……所以实际的编码中是分开写成两个SQL语句，一个是总缴费，一个是总支出。

**总缴费：**
`SELECT SUM(memfee_t.fper_income) FROM memfee_t WHERE mid = user_id`
**总支出：**

```
SELECT SUM((A.asite_fee + A.aball_fee) / A.aperson_num + B.adj_balance)
FROM
    activity_t AS A, memact_t AS B
WHERE
    B.mid = user_id AND A.aid = B.aid
```
- **根据日期查询这天参加打球的组员名单（IN语句）**

上述查询需要用到IN语句。IN语句可以在WHERE字句内指定多个值，很好用。下面是这个查询的SQL语句，用了两个IN语句，第一个IN字句中嵌套了第二个IN。原因是因为同一个日期有可能会返回多个activity_aid（就是一天打了不止一场球）。

```
SELECT member_t.chname 
FROM 
    member_t 
WHERE 
    member_t.mid IN (SELECT memact_t.mid FROM memact_t WHERE memact_t.aid 
    IN (SELECT activity_t.aid FROM activity_t WHERE activity_t.adate = '2015-03-14'))
```

# 写本文的动机

这个网站在我入学的时候就写好了，现在已经差不多过去两年了。因为需要将工作转交给下一任组长，因此需要将整个网站一起移交。所以需要有一份文档记录。同时，因为面试总是会被问到SQL语句的问题。当初写这个网站时查询了很多SQL语句的写法，但作为一个比较『激进』的实验室，大家都不太喜欢传统的关系型数据库，所以很久不用会忘记，每次面试我的回答都是『需要用的时候我会查』。感觉这个答案不太好，所以写这篇文章也是为了复习一下SQL的语法……

另外一直想在改进一下这个网站。目前主要功能还是查询，其实是为了公示每次打球的费用，没有线上自动交费的功能，如果线上自动线上交费我也就不用那么辛苦每个人收钱了……















