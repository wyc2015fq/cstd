# 【学界】运筹优化问题求解工具 Lingo - 知乎
# 

> **文章作者：步知IE**
本篇文章来自**微信公众号工业工程共学社：**[【步知IE】运筹优化问题求解工具 Lingo](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/MY_-hoGdky5Nb7OoAOo3Gg)
*欢迎原链接转发，转载请前往*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*的主页获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**陆续发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

## **1、Lingo软件概述**

Lingo是Linear Interaction and General Optimizer的缩写，中文名称为“交互式线性和通用优化求解器”，是由美国LINDO系统公司开发的一套专门用于求解最优化问题的软件包。Lingo可用于求解线性规划、二次规划、整数规划、非线性规划等问题。Lingo的特点是，简单易学，上手快，能够很方便的与Excel、数据库等交互。个人感觉此软件尤其擅长线性规划问题，速度很快。若是优化模型规模很大，则感觉Lingo力不从心了，还有两款很知名的优化问题求解工具，可以很快的求解大规模的优化问题，尤其整数规划，即Gurobi和Cplex，大家可自主了解下这两款软件。

Lingo的默认文件格式扩展名为.lg4，这是一种特殊的二进制格式文件，保存了模型窗口中的所有文本和其他信息。Lingo根据变量、约束条件等不同，也是分不同版本的。有Demo、Solve Suite、Super、Hyper、Industrial、Extended六种，其中Demo版免费，Extended版最高级。通过下表可以有个直观的感觉：
![](https://pic3.zhimg.com/v2-ed4a593b0862ed8469c6404f36f89472_b.jpg)
Lingo的相关资料网上很多，下载链接也很多，也有很多相关的教材，目前最高版本为Lingo17；本文所演示的版本为Lingo11。Lingo相关参考资料：

LINDO公司官网主页（Lindo介绍）:

[https://www.lindo.com/index.php/products/lingo-and-optimization-modeling](https://link.zhihu.com/?target=https%3A//www.lindo.com/index.php/products/lingo-and-optimization-modeling)

《优化建模与LINDO/LINGO软件》（谢金星，薛毅编著，清华大学出版社，2005年7月）

**腾讯课堂上有个视频教程，但好像收费。**

## **2、Lingo主界面及菜单栏介绍**

Lingo的界面比较简洁，主要分为菜单栏、工具栏、模型编辑窗口等
![](https://pic1.zhimg.com/v2-87d8ef2d574f8a9e1e9ba96bfc200098_b.jpg)

下图为Lingo工具栏，在图中标出了主要按钮的功能
![](https://pic2.zhimg.com/v2-ef56fafee7ecac4a0e22ead3e884f505_b.jpg)

## **3、优化模型的在lingo中的编译**

当你已经建好优化模型，接下来关键的一步是，如何通过lingo，将其转化为lingo可识别的语言。此部分是lingo学习的主体，内容也很多，本作者也是在学习熟练过程中，故在此只是大概介绍。

**1）直译法**

对于简单的模型，可以直接照搬到lingo上，lingo是可以识别的，一般做学生作业可以这样来操作，如：
![](https://pic2.zhimg.com/v2-af4f24f8f8922c6e8214ead6e68b253d_b.jpg)

**2）使用Lingo语言编译**

Lingo语言比较容易掌握，通过一个选址问题的小例子来说明：
![](https://pic1.zhimg.com/v2-86ece069916e98dc0b2a3673a8b2f270_b.jpg)
建模如下：通过分析模型，成本包括运输成本、配送中心建设成本。简单来说，可分别设为：TranCost、BuildCost；设x为运送量，D为运输距离，C为运费；i=1，2，代表工厂，j=1，2，3 代表候选点，k=1,2,3,4,5 代表客户。y为选建点，F为建设费用。 

最终所建模型如下：

目标函数：

*min = TranCost + BuildCost*
![](https://pic4.zhimg.com/v2-23a5d11a1b24bd0748a415f2fdfb0a4b_b.jpg)
将模型转换为Lingo语言：

```
sets:
ii/1 2/;
jj/1..3/: y, F, Cap;
kk/1..5/: Demand;
link1(ii,jj):xij,Dij,Cij;
link2(jj,kk):xjk,Djk,Cjk;
endsets
!目标函数;
[goal]min =TranCost+BuildCost;
!运输成本;
[TC]TranCost=@sum(link1(i,j):xij(i,j)*Dij(i,j)*Cij(i,j) )+@sum(link2(j,k):xjk(j,k)*Djk(j,k)*Cjk(j,k));
!配送中心建设成本;
[CC]BuildCost =@sum(jj(j):y(j)*F(j));
!配送中心容量约束;
@for(jj(j):[CT_a]@sum(ii(i):xij(i,j))<= y(j)*Cap(j));
!客户需求约束;
@for(kk(k):[CT_b]@sum(jj(j): xjk(j,k))=Demand(k));
!配送中心进出守恒约束;
@for(jj(j):[CT_c]@sum(ii(i):xij(i,j))=@sum(kk(k):xjk(j,k)));
!配送中心是否建设（0-1）约束;
@for(jj:[CT_01] @bin(y));
!运输单位成本赋值;
@for(link1:[CT_p1]Cij=2);
@for(link2:[CT_p2]Cjk=2);
data:
Dij,Djk,Demand,F,Cap
=@ole('Second_data1.xlsx','C2:E3','C6:G8','C11:G11', 'C15:E15','C16:E16');
Enddata
```

![](https://pic4.zhimg.com/v2-f88bb50040359fcb5c1b731252799dc3_b.jpg)

其中“Second_data1.xlsx”数据格式如下图Excel所示：
![](https://pic1.zhimg.com/v2-ac8ae191a540eb5f32d447e42aaf9a40_b.jpg)

**Lingo语法**

1．LINGO根据“MAX=”或“MIN=”寻找目标函数，而除注释和TITLE语句外的其他语句都是约束条件，因此语句的顺序并不重要；

2．每个语句必须以分号“;”结束，每行可以有多个语句，语句可以跨行；

3．变量名称必须以字母(A~Z)开头，由字母、数字(0~9)和下划线组成，长度不超过32个字符，函数一律需要以“@”开头，不区分大小写；

4．可以给语句加上标号，例如：

`    [OBJ]  MAX=200*X1+300*X2；`

5．以!开头，以“;”号结束的语句是注释语句；

6．如果对变量的取值范围没有作特殊说明，则默认所有决策变量都非负；

7．LINGO模型以语句“MODEL:”开头，以“E

**模型集合设置：**

以“ SETS:” 开始， “ENDSETS”结束，定义必要的集合变量（SET）及其元素（MEMBER，含义类似于数组的下标）和属性（ATTRIBUTE，含义类似于数组）；

**模型主体部分：**

必须包含目标函数，求解Min或Max，涉及函数@min和@max，返回指定的一个表达式的最小值或最大值。约束部分，最常用到求和函数@SUM和循环函数@FOR。

@sum返回遍历指定的集成员的一个表达式的和。

例 求向量[5，1，3，4，6，10]前5个数的和。

```
data:
N=6;
enddata
sets:
number/1..N/:x;
endsets
data:
x = 5 1 3 4 6 10;
enddata
s=@sum(number(I) | I #le# 5: x);
```

@for 用来产生对集成员的约束。基于建模语言的标量需要显式输入每个约束，不过@for函数允许只输入一个约束，然后LINGO自动产生每个集成员的约束。

例 产生序列{1,4,9,16,25}

```
sets:
number/1..5/:x;
endsets
@for(number(I): x(I)=I^2);
```

**模型变量赋值部分：**

data开头，enddata结尾，可放置于模型开头、中间、末尾。

@ole为读取数据函数;

@OLE是从EXCEL中引入或输出数据的接口函数，它是基于传输的OLE技术。OLE传输直接在内存中传输数据，并不借助于中间文件。当使用@OLE时，LINGO先装载EXCEL，再通知EXCEL装载指定的电子数据表，最后从电子数据表中获得Ranges。

## **4、求解状态窗口**

当点击求解按钮后，求解结束，会首先跳出来状态窗口（Solver Status），描述模型的参数情况和求解情况等，如下所示：
![](https://pic3.zhimg.com/v2-79f10cc6477a762d5961cb50c3c4de52_b.jpg)

## **5、求解报告（Solution Report）**

当求解完成时，跳出求解状态窗口的同时，也会同时跳出求解报告窗口，通过打开LINGO菜单->solution，可以调节求解报告的输出方式，如选择输出“Text”格式等。
![](https://pic3.zhimg.com/v2-200b67d64380e19063f271b38587b19a_b.jpg)
optimal solution found：表示迭代后得到全局最优解。

Objective value:224800.0 表示最优目标值为224800.0。

Value列：给出最优解中各变量的值

Slack or Surplus列：给出松驰变量的值

Reduced Cost列：给出了最优单纯形表中判别数所在行的变量的系数，表示当变量有微小变动时, 目标函数的变化率。

DUAL PRICE列：（对偶价格）表示当对应约束有微小变动时, 目标函数的变化率 。

## **6、总结**

本文对lingo做了一个大概的介绍，希望读者能从整体上感知Lingo是一个怎样的样子；读者若想能够实际熟练操作和运用Lingo，还需自己多学习，多查找资料，多动手练习。若想学习功能更强大的优化求解器，可以直接学习Gurobi或Cplex，通过教育邮箱（.[http://edu.cn](https://link.zhihu.com/?target=http%3A//edu.cn)），可以从官网免费下载。

如果你是运筹学/人工智能硕博或在读，请在下图的公众号后台留言：**“加微信群”**。系统会自动辨认你的关键字，并提示您进一步的加群要求和步骤，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

同时我们有：【**运筹学|优化爱好者**】【**供应链|物流**】【**人工智能**】【**数据科学|分析**】千人QQ群，想入群的小伙伴可以关注下方公众号**点击“加入社区”按钮**，获得入群传送门。

学术界|工业界招聘、征稿等信息**免费发布**，请见下图：
![](https://pic2.zhimg.com/v2-95bfcc88cc182f0e1a6952413c78fe35_b.jpg)

