# hive expecting Identifier near 'table' in table name 报错 - zj360202的专栏 - CSDN博客





2013年02月20日 17:17:18[zj360202](https://me.csdn.net/zj360202)阅读数：2447








```
hive> drop table 'fct_pf_vs_hour';
FAILED: ParseException line 1:11 mismatched input ''fct_pf_vs_hour'' expecting Identifier near 'table' in table name
```
正常的hql语句执行出问题，解决方法是将 'fct_pf_vs_hour'两边的单引号改成 `fct_pf_vs_hour`; 键盘上面数字1前面的~下面的符号
```
hive> drop table `fct_pf_vs_hour`;                   
OK
Time taken: 2.825 seconds
```




