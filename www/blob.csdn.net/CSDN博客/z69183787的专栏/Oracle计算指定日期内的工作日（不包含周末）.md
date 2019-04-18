# Oracle计算指定日期内的工作日（不包含周末） - z69183787的专栏 - CSDN博客
2014年06月25日 10:40:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15207
```
1、获取当天是礼拜几：select to_char(sysdate,'d') from dual;  --礼拜天为1，礼拜一为2，类推
2、获取 两个时间段间的 工作日：
select (trunc(&end_dt - &start_dt) - 
         ((case
         WHEN (8 - to_number(to_char(&start_dt,'D'))) > trunc(&end_dt - &start_dt) + 1 THEN 0
         ELSE
          trunc((trunc(&end_dt - &start_dt) -
                (8 - to_number(to_char(&start_dt,'D'))))/7) + 1 END) + 
         (case
         WHEN mod(8 - to_char(&start_dt, 'D'), 7) > trunc(&end_dt - &start_dt) - 1 THEN 0
         ELSE
          trunc((trunc(&end_dt - &start_dt) - (mod(8 - to_char(&start_dt,'D'),7) + 1))/7) + 1  END)))
         as workingdays
  from dual
下面的sql可以直接运行：
select (trunc(to_date('2010-11-11','yyyy-mm-dd') - to_date('2010-11-07','yyyy-mm-dd')) - 
         ((case
         WHEN (8 - to_number(to_char(to_date('2010-11-07','yyyy-mm-dd'),'D'))) > trunc(to_date('2010-11-11','yyyy-mm-dd') - to_date('2010-11-07','yyyy-mm-dd')) + 1 THEN 0
         ELSE
          trunc((trunc(to_date('2010-11-11','yyyy-mm-dd') - to_date('2010-11-07','yyyy-mm-dd')) -
                (8 - to_number(to_char(to_date('2010-11-07','yyyy-mm-dd'),'D'))))/7) + 1 END) + 
         (case
         WHEN mod(8 - to_char(to_date('2010-11-07','yyyy-mm-dd'), 'D'), 7) > trunc(to_date('2010-11-11','yyyy-mm-dd') - to_date('2010-11-07','yyyy-mm-dd')) - 1 THEN 0
         ELSE
          trunc((trunc(to_date('2010-11-11','yyyy-mm-dd') - to_date('2010-11-07','yyyy-mm-dd')) - (mod(8 - to_char(to_date('2010-11-07','yyyy-mm-dd'),'D'),7) + 1))/7) + 1  END)))
         as workingdays
  from dual
```
