# sql server t-sql脚本转成oracle plsql - 左直拳的马桶_日用桶 - CSDN博客
2017年05月03日 19:19:25[左直拳](https://me.csdn.net/leftfist)阅读数：2808
将一份SQL SERVER数据库生成的T-SQL脚本，转成ORACLE的PL/SQL，其复杂繁琐程度，远远出乎我的意料。
这份SQL SERVER脚本，里面有表，有视图，还有存储过程，以及一些自定义函数。目前，仅勉强将表、视图移植到oracle，存储过程和自定义函数，只能先放弃了。
总结如下：
**一、转换工具**
oracle自带工具：Oracle SQL Developer。选工具 -“移植”- 草稿编辑器 
![这里写图片描述](https://img-blog.csdn.net/20170503184604447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在下拉框中选择 T-SQL 到 PL/SQL（默认） 
![这里写图片描述](https://img-blog.csdn.net/20170503184637703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有个地方要特别注意，也是这个工具比较脑残的地方，T-SQL脚本一定要通过粘贴的途径输入到左边的输入框，这样才有转换的界面；假如你通过打开文件的方式，系统会把它当成PLSQL，根本没有下拉框出来。我刚开始就是直接打开脚本文件，心想怎么跟网上说的对不上号，几番折腾，才发现这个转换的功能。
我还下载了一个名叫“IspirerMnMTK”的东东，根本安装不上，换了几台机器都不行，不知道啥垃圾。
**二、转换**
**1、这个ORACLE自带的工具转换功能并非万能**，好多T-SQL语句都无法识别。
有这些语句存在，它就乱译一通，根本无法执行。比如一些选项，一些字段注释，甚至一些代码注释
```
SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[HDBH_BHGH_TJ_PROPVALUE]') AND type in (N'U'))
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'海啸发生次数' ,@level0type=N'SCHEMA', @level0name=N'dbo', @level1type=N'TABLE', @level1name=N'HDTJ_FORM_TJ9', @level2type=N'COLUMN', @level2name=N'TSU_TIMES'
```
诸如此类 
像这种情况，只有先自行去掉，再给工具翻译。
**2、ORACLE不支持**
我这次遇到的有两个： 
**1）NVARCHAR(MAX)**
工具翻译成 NVARCHAR2(4000)。问题是，假如一张表里，有2个或以上这样的字段，ORACLE建表时就报字段超长的错。而且，窃以为翻译成NVARCHAR2(4000)也是不准确的，所以手动改为clob。
**2）标识符超长**
我拿到的SQL SERVER脚本，有些字段名字起的非常长，而oracle只能支持30个字符以内，所以，也要手动修改。
**三、执行**
好不容易翻译好的脚本，如何执行？
用PLSQL Developer的SQL窗口，只能执行头三条语句；应当用命令窗口。但命令窗口对格式要求又比较严，有时出现一段SQL脚本，断句报错的情况。
还有就是，像这种以`END;`结束的，一定要另起一行，来个“/”。如：
```
CREATE OR REPLACE TRIGGER EVSYS_SETUPGUIDE_ID_TRG
   BEFORE INSERT 
   ON EVSYS_SETUPGUIDE
   FOR EACH ROW
   BEGIN
      SELECT EVSYS_SETUPGUIDE_ID.NEXTVAL INTO :NEW.ID FROM DUAL;
   END;
/
```
**四、总结**
转换工具并非万能，尽量精简之后再进行翻译；翻译之后，还要进行修正。我最终，还是写了一个工具来做这些辅助工作。比如，字段注释，可以在自行开发的工具里使用正则表达式进行自动替换：
```
--SQL SERVER 的字段注解
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'海啸发生次数' ,@level0type=N'SCHEMA', @level0name=N'dbo', @level1type=N'TABLE', @level1name=N'HDTJ_FORM_TJ9', @level2type=N'COLUMN', @level2name=N'TSU_TIMES'
```
```
--oracle 的字段注解
COMMENT ON COLUMN "PCB_HDDM2"."TJDISTCODE" IS '用于统计的行政区划代码';
```
替换的正则表达式
```
EXEC\s+sys\.sp_addextendedproperty\s+@name=N'MS_Description',\s+@value=N'(?<Comment>[^']+)'\s*,\s+@level0type=N'SCHEMA',@level0name=N'dbo',\s+@level1type=N'TABLE',@level1name=N'(?<Table>[^']+)',\s+@level2type=N'COLUMN',@level2name=N'(?<Column>[^']+)'
替换成：
COMMENT ON COLUMN "${Table}"."${Column}" IS '${Comment}';
```
