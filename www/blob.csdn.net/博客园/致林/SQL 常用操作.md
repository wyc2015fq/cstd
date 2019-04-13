
# SQL 常用操作 - 致林 - 博客园






# [SQL 常用操作](https://www.cnblogs.com/bincoding/p/5895539.html)
今天网龙笔试遇到了几个SQL题，现在顺便就总结一下常用的SQL操作。
内连接：只将符合条件的行显示出来
SELECTs.name,m.markFROMstudent s,mark mWHEREs.id=m.studentid;SELECTs.name,m.markFROMstudent sINNERJOINmark mONs.id=m.studentid
左连接：将左表所有的行显示，右表只显示符合条件的行
SELECTs.name,m.markFROMstudent sLEFTJOINmark mONs.id=m.studentid；
![](https://images2015.cnblogs.com/blog/771778/201609/771778-20160922112954215-417042747.png)
全连接：将所有行都显示出来，不论两表是否有符合的对应项
SELECTs.name,m.markFROMstudent sFULLJOINmark mONs.id=m.studentid
复制A表
CREATETable_BLIKETable_A;SELECT*INTOTable_BFROMTable_A;/*B表不存在，自动创建B表*/INSERTINTOTable_B(fld1, fld2)SELECTfld1,5fromTable_A;/*B表存在*/
将A表的column为1的结果抽取给B表
INSERTTable_BSELECT*FROMTable_AWHEREcolumn=1;
根据column为B表建立降序索引
CREATEINDEXIndex_BONTable_B(column desc);
根据date统计B表
SELECTCOUNT(*)FROMTable_BGROUPBYdate;
在B表中删除id=1的列
DELETEFROMTabe_BWHEREid=1;
删除index和删除B表
DROPINDEXIndex_B;DROPTABLETable_B:





