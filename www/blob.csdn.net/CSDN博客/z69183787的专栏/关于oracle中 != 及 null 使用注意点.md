# 关于oracle中 != 及 null 使用注意点 - z69183787的专栏 - CSDN博客
2013年07月09日 09:42:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12864
**titlestepname **
**关于继续开展下半年度集团早高峰保驾工作的通知传阅**
** 关于继续开展下半年度集团早高峰保驾工作的通知null**
**关于配合国资委协同办公平台统一权限配置的相关事宜null**
**执行如下语句：**
**select t.title,t.stepname from t_todo_item t where t.loginname='ST/G00100000161' and t.stepname!='传阅'**
**返回结果为空；**
**执行如下语句，结果才是希望得到的结果（stepName!='传阅'）**
**select t.title,t.stepname from t_todo_item t where t.loginname='ST/G00100000161' and (t.stepname!='传阅' or t.stepname is null)**
** 关于继续开展下半年度集团早高峰保驾工作的通知null关于配合国资委协同办公平台统一权限配置的相关事宜null**
**因为null 不能用 = 或 != 来作为判断，只能使用 is null 或者 is not null**
**所以当 stepname!='传阅'时，null 未符合条件，故第一句的sql 是错误的。**
**也可以使用 nvl 函数**
**select t.title,t.stepname from t_todo_item t where t.loginname='ST/G00100000161' and nvl(t.stepname,'123')!='传阅'**
**总之，需要注意的就是 oracle 中的 null 不能用 =及!=用来比较 ，只能使用**is null 或者 is not null，****
****当字段中有null时，需要区别对待。****
****说明： 1、等价于没有任何值、是未知数。 2、NULL与0、空字符串、空格都不同。 3、对空值做加、减、乘、除等运算操作，结果仍为空。 4、NULL的处理使用NVL函数。 5、比较时使用关键字用“is null”和“is not null”。 6、空值不能被索引，所以查询时有些符合条件的数据可能查不出来，count(*)中，用nvl(列名,0)处理后再查。 7、排序时比其他数据都大（索引默认是降序排列，小→大），所以NULL值总是排在最后。 ****
