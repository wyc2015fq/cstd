# SQL数据库，如何把服务器中的一张表插入到另外一个服务器的一张表中 - weixin_33985507的博客 - CSDN博客
2014年05月06日 10:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
先开启
exec sp_configure 'show advanced options',1 
reconfigure  
exec sp_configure 'Ad Hoc Distributed Queries',1  
reconfigure  
然后调用
INSERT INTO SoyErpBasicData.dbo.Base_Datadictionary ( Type ,
          Code ,
          Name ,
          SeqNO ,
          State ,
          Notes ,
          ParentID ,
          IsLeaf_YN ,
          CompCode
        )
SELECT  Type ,
          Code ,
          Name ,
          SeqNO ,
          State ,
          Notes ,
          ParentID ,
          IsLeaf_YN ,
          CompCode
FROM openrowset('SQLOLEDB','192.168.0.188';'sa';'hualong2012',SoyErpBasicData.dbo.Base_Datadictionary)
WHERE id IN (268,269)
调用完后关闭
exec sp_configure 'Ad Hoc Distributed Queries',0 
reconfigure  
exec sp_configure 'show advanced options',0  
reconfigure
总结：很简单的，三部曲

