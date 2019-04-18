# 分析函数 OVER(PARTITION BY... ORDER BY...)的讲解 - z69183787的专栏 - CSDN博客
2018年02月27日 10:59:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1848
[http://blog.csdn.net/east_mrchiu/article/details/70183428](http://blog.csdn.net/east_mrchiu/article/details/70183428http://)
这个函数长这个样子：OVER(PARTITION BY... ORDER BY...)
顾名思义，PARTITION 中文是分割的意思，ORDER 是排序的意思，所以翻译一下就是先把一组数据按照制定的字段进行分割成各种组，然后组内按照某个字段排序。
以实际案例来说明这个函数的使用，
首先，我们先看一组普通排序的数据：
![](https://img-blog.csdn.net/20170415113716401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后对其进行新加一个伪劣ROWNUM，看看数据原本的顺序：
![](https://img-blog.csdn.net/20170415113808813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
OVER(ORDER BY ...)的意思是以按照某个字段排序，所以和直接对表的order by效果是一样的：
![](https://img-blog.csdn.net/20170415114052283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里就显示了OVER(ORDER BY ...)的第一个功能，就是对新查询得到的数据进行重新编号，即RNO的值，由于这里没有PARTITION BY...，所以可以比作整个列就是一个大块，然后对大块的内容进行排序，这个时候再加上PARTITION BY...，查询结果：
![](https://img-blog.csdn.net/20170415114426393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
它会按照制定的字段，把相同值的排在一起，分为一个块，也就是分组，然后组内排序编号，这样你就可以取不同组内的任意第几个值，类似于TOP-N的分析。
在这里over之前使用的函数是ROW_NUMBER()，就是对数据的编号。
# 与OVER(PARTITION BY... ORDER BY...)匹配使用的函数
## row_number() over()、rank() over()和dense_rank() over()
在上面的例子里，使用ROW_NUMBER()可以对数据编号，但是有一个问题，例子中的MI_ID是不可以重复的，如果在可以重复的情况下，就有并列的情况，这样就无法取出并列的数据，只能取单一排序的数据。所以这里可以换成 rank() over()和dense_rank() over()来进行编号：（rank() over()和dense_rank() over()的区别如图）
![](https://img-blog.csdn.net/20170415125430710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170415125438804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## sum() over()，first_value() over()和last_value() over()的使用
sum() over()分组求和
first_value() over()求分组第一条
last_value() over()求分组最后一条
其中用row_number() over()取编号第一条的也可以实现first_value() over()的效果
[sql][view
 plain](http://blog.csdn.net/east_mrchiu/article/details/70183428#)[copy](http://blog.csdn.net/east_mrchiu/article/details/70183428#)
- SELECTDISTINCT *  
- FROM (SELECT T1.MI_ID,  
-                T3.I_IDENTITY_CARD,  
- SUM(NVL(T2.IS_VISHEARTPROMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) PRO_NUM,  
- SUM(NVL(T2.IS_VISHEARTEXPMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) EXP_NUM,  
- SUM(NVL(T2.IS_VISHEARTPOLMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) POL_NUM,  
-                              T1.CREATE_TIME,  
-                <span style="color:#ff0000;">FIRST_VALUE</span>(T2.CREATE_DATE) OVER(PARTITION BY T3.I_IDENTITY_CARD ORDERBY T2.CREATE_DATE DESC) FIRST_CREATE_DATE  
- FROM T_ZZ_PETITIONERS T1  
- LEFTJOIN T_ZZ_VISIT_RECORD T2  
- ON T1.CI_RS_ID = T2.CI_RS_ID  
- AND T2.STATUS != '003'
- LEFTJOIN T_DC_CI_RS_TOP T3  
- ON T1.CI_RS_ID = T3.CI_RS_ID  
- AND T3.STATUS = '1'
- WHERE T1.IS_ADD_HEARTPROTECT = '1'
- AND T1.STATUS = '1') C  
- ORDERBY C.CREATE_TIME DESC
![](https://img-blog.csdn.net/20170415145007506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[sql][view
 plain](http://blog.csdn.net/east_mrchiu/article/details/70183428#)[copy](http://blog.csdn.net/east_mrchiu/article/details/70183428#)
- SELECT *  
- FROM (SELECT T1.MI_ID,  
-                T3.I_IDENTITY_CARD,  
- SUM(NVL(T2.IS_VISHEARTPROMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) PRO_NUM,  
- SUM(NVL(T2.IS_VISHEARTEXPMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) EXP_NUM,  
- SUM(NVL(T2.IS_VISHEARTPOLMEMBER, 0)) OVER(PARTITION BY T3.I_IDENTITY_CARD) POL_NUM,  
-                T1.CREATE_TIME,  
-                <span style="color:#ff0000;">ROW_NUMBER()</span> OVER(PARTITION BY T3.I_IDENTITY_CARD ORDERBY T2.CREATE_DATE DESC) RNO  
- FROM T_ZZ_PETITIONERS T1  
- LEFTJOIN T_ZZ_VISIT_RECORD T2  
- ON T1.CI_RS_ID = T2.CI_RS_ID  
- AND T2.STATUS != '003'
- LEFTJOIN T_DC_CI_RS_TOP T3  
- ON T1.CI_RS_ID = T3.CI_RS_ID  
- LEFTJOIN T_DC_GRID T4  
- ON T1.REGION_CODE = T4.INFO_ORG_CODE  
- WHERE T1.IS_ADD_HEARTPROTECT = '1'
- AND T1.STATUS = '1'
- AND T3.STATUS = '1'
- AND T4.STATUS = '001') C  
- WHERE <span style="color:#ff0000;">C.RNO = 1</span>  
- ORDERBY C.CREATE_TIME DESC
![](https://img-blog.csdn.net/20170415145115242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdF9NckNoaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ignore nulls：过滤掉空值
常用的分析函数如下所列:
row_number() over(partition by ... order by ...)
rank() over(partition by ... order by ...)
dense_rank() over(partition by ... order by ...)
count() over(partition by ... order by ...)
max() over(partition by ... order by ...)
min() over(partition by ... order by ...)
sum() over(partition by ... order by ...)
avg() over(partition by ... order by ...)
first_value() over(partition by ... order by ...)
last_value() over(partition by ... order by ...)
lag() over(partition by ... order by ...)
lead() over(partition by ... order by ...)
