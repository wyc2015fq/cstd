# 向现有表添加标识列（IDENTITY） - 左直拳的马桶_日用桶 - CSDN博客
2010年06月12日 18:13:00[左直拳](https://me.csdn.net/leftfist)阅读数：1185
可以向现有表添加标识列（IDENTITY）。
ALTER TABLE Table1 ADD [ID] INT IDENTITY(1,1) NOT NULL
运行以后，会发现ID列已经有值，值顺序就是表记录插入的顺序。
