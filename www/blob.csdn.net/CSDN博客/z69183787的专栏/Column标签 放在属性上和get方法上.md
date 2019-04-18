# Column标签 放在属性上和get方法上 - z69183787的专栏 - CSDN博客
2014年10月22日 15:30:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3653
Column标签上放在 属性上 ，若原始字段为 LOGIN_NAME，而映射字段为LOGINNAME，
则最后声称的hql 中 _ 下划线会被省略，从而造成hql报错
而Column放在get方法上就不会，不知道为什么？
