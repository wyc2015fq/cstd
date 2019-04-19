# oracle trunc 函数处理日期格式 - 零度的博客专栏 - CSDN博客
2016年04月28日 09:42:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：741
**select to_char(sysdate,'yyyy-mm-dd hh24:mi:ss') from dual;  --显示当前时间**2011-12-29 16:24:34
**select trunc(sysdate,'year') from dual; --截取到年（本年的第一天）**2011-1-1
**select trunc(sysdate,'q') from dual; --截取到季度（本季度的第一天）**2011-10-1
**select trunc(sysdate,'month') from dual; --截取到月（本月的第一天）**2011-12-1
**select trunc(sysdate,'') from dual; **空
**select to_char(trunc(sysdate),'yyyymmdd hh24:mi:ss') from dual; --默认截取到日（当日的零点零分零秒）**20111229 00:00:00
**select trunc(sysdate,'day') from dual; --截取到周（本周第一天，即上周日）**2011-12-25
**select to_char(trunc(sysdate,'dd'),'yyyymmdd hh24:mi:ss') from dual;--截取到日（当日的零点零分零秒）**20111229 00:00:00
**select trunc(sysdate,'hh24') from dual;  --截取到小时（当前小时，零分零秒）**2011-12-29 16:00:00
**select trunc(sysdate,'mi') from dual; --截取到分（当前分，零秒）**2011-12-29 16:24:00
**select trunc(sysdate,'ss') from dual ;--报错，没有精确到秒的格式**
