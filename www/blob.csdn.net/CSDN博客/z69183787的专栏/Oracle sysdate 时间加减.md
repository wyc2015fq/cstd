# Oracle sysdate 时间加减 - z69183787的专栏 - CSDN博客
2014年03月28日 16:23:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18903
加法 
 select sysdate,add_months(sysdate,12) from dual;        --加1年
 select sysdate,add_months(sysdate,1) from dual;        --加1月
 select sysdate,to_char(sysdate+7,'yyyy-mm-dd HH24:MI:SS') from dual;   --加1星期
 select sysdate,to_char(sysdate+1,'yyyy-mm-dd HH24:MI:SS') from dual;   --加1天
 select sysdate,to_char(sysdate+1/24,'yyyy-mm-dd HH24:MI:SS') from dual;  --加1小时
 select sysdate,to_char(sysdate+1/24/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --加1分钟
 select sysdate,to_char(sysdate+1/24/60/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --加1秒
减法
 select sysdate,add_months(sysdate,-12) from dual;        --减1年
 select sysdate,add_months(sysdate,-1) from dual;        --减1月
 select sysdate,to_char(sysdate-7,'yyyy-mm-dd HH24:MI:SS') from dual;   --减1星期
 select sysdate,to_char(sysdate-1,'yyyy-mm-dd HH24:MI:SS') from dual;   --减1天
 select sysdate,to_char(sysdate-1/24,'yyyy-mm-dd HH24:MI:SS') from dual;  --减1小时
 select sysdate,to_char(sysdate-1/24/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --减1分钟
 select sysdate,to_char(sysdate-1/24/60/60,'yyyy-mm-dd HH24:MI:SS') from dual;  --减1秒
