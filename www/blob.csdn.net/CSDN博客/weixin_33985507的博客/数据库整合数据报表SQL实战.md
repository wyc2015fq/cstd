# 数据库整合数据报表SQL实战 - weixin_33985507的博客 - CSDN博客
2016年09月02日 14:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
协助同事整理sql统计报表。
---建立由avalue的视图，要过滤重复数据
 CREATE VIEW vLectAnswerRecord
 as
 SELECT t2.OpenID,t2.Qguid,t1.AValue av FROM TbLectAnswer t1,TbLectAnswerRecord t2 WHERE t1.AID=t2.AID
 ---获取用户组合答案
 SELECT Qguid,OpenID, data=STUFF((
 SELECT ','+[t].[av] FROM vLectAnswerRecord t WHERE t.Qguid=t1.Qguid AND t.openid=t1.OpenID FOR XML PATH('') 
 ), 1, 1, '')
FROM vLectAnswerRecord t1 GROUP BY qguid,OpenID
---将组合答案插入到临时表
 INSERT into tbtmp( Qguid,OpenID,avs) 
 SELECT Qguid,OpenID, data=STUFF((
 SELECT ','+[t].[av] FROM vLectAnswerRecord t WHERE t.Qguid=t1.Qguid AND t.openid=t1.OpenID FOR XML PATH('') 
 ), 1, 1, '')
FROM vLectAnswerRecord t1 GROUP BY qguid,OpenID 
---将组合的逗号去掉
UPDATE tbtmp SET avs2=REPLACE(avs,',','')
---关联试题正确答案表，得出用户题目是否正确
---SELECT * FROM tbtmp 
UPDATE tbtmp SET istrue=1
FROM tbtmp INNER JOIN
      TBLectQuestion ON tbtmp.Qguid=TBLectQuestion.Qguid AND tbtmp.avs=TBLectQuestion.QResult
--- SELECT TOP 100 Lguid,QResult,Qguid,QType FROM dbo.TBLectQuestion WHERE qguid='5ff05fb1ff62ddbc31324dc30aae5c31'
