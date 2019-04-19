# oracle如何去除字段的回车换行符 - 左直拳的马桶_日用桶 - CSDN博客
2017年06月30日 11:13:37[左直拳](https://me.csdn.net/leftfist)阅读数：5868
oracle如何去除字段的回车换行符？
可以用trim也可以用replace。区别在于，trim处理字符串两端，而replace中间也可以处理。
trim
```
select '全世界无产者
' || '联合起来！' from dual;
select trim(chr(13) from trim(chr(10) from '全世界无产者
')) || '联合起来！' from dual;
```
replace 
当然就是
```
select replace(replace('全世界无产者
',chr(13),''),chr(10),'') || '联合起来！' from dual;
```
> 
制表符 chr(9)  
  换行符 chr(10)  
  回车符 chr(13)
