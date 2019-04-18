# 亚马逊向GuardDuty服务添加三种新的威胁检测规则 - weixin_33985507的博客 - CSDN博客
2019年02月26日 08:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
亚马逊在[GuardDuty](%5C)服务中添加了另一组新的威胁检测。这三种新的威胁检测中有两种是新的渗透测试检测和一种策略违规检测。
Amazon GuardDuty是AWS提供的威胁检测服务，可持续监控恶意或未经授权的行为，帮助客户保护其AWS账户和工作负载。当检测到威胁时，该服务将向GuardDuty控制台和AWS CloudWatch事件发送详细的安全警报——可以基于警报采取行动，且易于集成到现有事件管理和工作流系统中。
![\"\"](https://static001.infoq.cn/resource/image/51/69/5160cca3bece3556b8a4da73860a0469.png)
来源：[https://aws.amazon.com/guardduty/](%5C)
Angela Karl最近在Techgenix发表的一篇有关GuardDuty的文章，提到该服务最重要的特性：
- 
帐户级威胁检测——通过几近实时的持续监控来准确地检测帐户危害。
- 
跨AWS账户的持续监控——监控和分析AWS CloudTrail、VPC Flow Logs和DNS Logs上所有AWS账户和工作负载的事件数据，无需额外的安全软件或基础设施。
- 
针对云开发和优化的威胁检测——内置检测技术专门针对云进行开发和优化。GuardDuty还集成了威胁情报，如界领先的第三方安全合作伙伴Proofpoint和CrowdStrike。
- 
威胁严重性级别——具有低、中和高威胁严重性级别，客户可以相应地做出响应。
通过这三种新的威胁检测，亚马逊继续为在其AWS账户中启用Amazon GuardDuty的客户提供全托管的威胁检测库。此外，去年11月中旬，亚马逊也增加了三种威胁检测：
- 
两种用于检测与Tor网络相关的可疑活动；
- 
第三种用来帮助客户识别与加密货币挖掘相关的活动。
最近发布的三种威胁检测中有两种与渗透测试相关，它们会提醒用户运行Parrot Linux或Pentoo Linux的机器使用AWS凭证进行API调用。这些新扩展扩展了现有的Kali Linux检测。
第三种新检测是一种新的[策略违反检测](%5C)，它会向用户发出警报，提醒用户有人使用AWS帐户根凭据发送请求。此外，检测将通知用户有人使用根AWS帐户凭据向AWS服务发出编程式请求或登录到AWS管理控制台。
有关Amazon GuardDuty可用性的更多详细信息，请访问[AWS Regions](%5C)页面。此外，服务的定价详细信息可在[定价](%5C)页面上找到。
**查看英文原文**：[https://www.infoq.com/news/2019/02/aws-guardduty-threat-detections](%5C)
