# ORACLE几个关于工作日数的函数（原创） - z69183787的专栏 - CSDN博客
2014年06月25日 10:57:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6318
最近做到一个关于销售日报的模块，而其中需要有月达成率等几项需要用到工作日做分母的情况
这样就要得到工作日的数量
怎样得到两个日期之间除了星期天，星期六的天数，oracle中可以使用下面的句子
select count(*)
from ( select rownum rnum
       from all_objects
      where rownum <= to_date('&1') - to_date('&2')+1 )
      where to_char( to_date('&2')+rnum-1, 'DY' ) 
      not in ( 'SAT', 'SUN' )
而我想得到一个不定月的工作日，这样首先要得到该月的最后一天和第一天
第一天用
select trunc(sysdate,'mm') from dual
最后一天用
select Add_months(trunc(sysdate,'mm'),1)-1 from dual
这样就不难得到该月的工作日了
该月工作日为
select count(*)
  from (select rownum rnum
          from all_objects
         where rownum <=
               add_months(trunc(sysdate, 'mm'), 1) - trunc(sysdate, 'mm'))
 where to_char(trunc(sysdate, 'mm') + rnum - 1, 'D') not in ('1', '7')
该月已过工作日为
select count(*)
      from (select rownum rnum
              from all_objects
             where rownum <= sysdate -trunc(sysdate,'mm') + 1)
     where to_char(trunc(sysdate,'mm') + rnum - 1, 'D') not in ('1', '7')
剩余的工作日一减就OK了
example：
               select count(*)
  from (select rownum rnum
          from all_objects
         where rownum <
               to_date('2014-06-25','yyyy-mm-dd') - to_date('2014-06-13','yyyy-mm-dd'))
 where to_char(trunc(sysdate, 'mm') + rnum - 1, 'D') not in ('1', '7')
