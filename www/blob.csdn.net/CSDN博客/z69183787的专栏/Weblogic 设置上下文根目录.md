# Weblogic 设置上下文根目录 - z69183787的专栏 - CSDN博客
2012年11月13日 15:17:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5097
**配置文件 weblogic.xml中 增加 **
**<weblogic-web-app><context-root>/workflowLocal</context-root><container-descriptor><prefer-web-inf-classes>true</prefer-web-inf-classes></container-descriptor></weblogic-web-app> **
**其中/workflowLocal为你的上下文名称**
