# oracle 导出导入不含数据的空库 - 左直拳的马桶_日用桶 - CSDN博客
2016年06月02日 20:04:54[左直拳](https://me.csdn.net/leftfist)阅读数：7187
10g或之前，用exp导出，imp导入，带上rows=n参数
11g或以上，用expdp导出，impdp导入，带上CONTENT = METADATA_ONLY 参数 
expdp带上此参数，不导出数据；impdp带上此参数，则不导入数据，问你死未。
```
导出空结构
expdp system/s 
directory=backup_dir 
CONTENT=METADATA_ONLY 
dumpfile=testmetaddl.dmp 
schemas=testmeta 
logfile=testmetalog.$Date.log
impdp 'sys/123456@HNHY12 as sysdba' directory=dbbak dumpfile=PDBGJHY320160601.EXPDP logfile=PDBGJHY320160601.log schemas=hydg,sde CONTENT = METADATA_ONLY
```
