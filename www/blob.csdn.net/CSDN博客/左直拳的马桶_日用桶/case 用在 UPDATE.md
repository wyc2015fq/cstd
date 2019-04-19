# case 用在 UPDATE - 左直拳的马桶_日用桶 - CSDN博客
2010年08月31日 18:06:00[左直拳](https://me.csdn.net/leftfist)阅读数：1044标签：[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
    UPDATE [Table1]
        SET [StatuID] = CASE WHEN  [StatuID] - 1 < 0 THEN 0 ELSE [StatuID] - 1 END
        WHERE [IndexId] = @IndexId;
IsExcellent = CASE ISNULL(ex.MmId,0) WHEN 0 THEN 0 ELSE 1 END 
