# group by 与 order by - 左直拳的马桶_日用桶 - CSDN博客
2011年04月27日 17:36:00[左直拳](https://me.csdn.net/leftfist)阅读数：1709标签：[url](https://so.csdn.net/so/search/s.do?q=url&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)

USE [mydb]
go
WITH w AS(
    SELECT TOP 6 InfoId FROM(
        SELECT InfoId FROM(
            SELECT TOP 6
                b.InfoId
                FROM dbo.[BrowseLog] b,dbo.[Info] i
                WHERE b.InfoId=i.InfoId
                    AND i.CityId=3
                GROUP BY b.InfoId
                ORDER BY MAX(b.BrowseId)
 DESC
        ) i1
        UNION
        SELECT InfoId FROM(
            SELECT TOP 6
                InfoId
                FROM dbo.[Info]
                WHERE CityId=3
                ORDER BY InfoId DESC
        ) i2
    ) u
    ORDER BY InfoId ASC
)
SELECT     
    w.InfoID
    ,i.[desc]
    ,i.small_pic_url
    ,s.SiteUrl
    ,s.SiteName 
    FROM w,dbo.[Info] i,dbo.[Site] s
    WHERE w.InfoId=i.InfoId
        AND i.SiteId=s.SiteId
    ORDER BY w.InfoId DESC;
直接写 ORDER BY b.BrowseId
 DESC 是不行的，语法错误，用了MAX（）以后就可以了。其含义，就是用每个分组里面，browseId的最大值来排序。
