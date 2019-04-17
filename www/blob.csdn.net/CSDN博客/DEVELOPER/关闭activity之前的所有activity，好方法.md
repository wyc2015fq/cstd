# 关闭activity之前的所有activity，好方法 - DEVELOPER - CSDN博客





2017年01月10日 13:52:16[学术袁](https://me.csdn.net/u012827205)阅读数：3272







**之前写的应用项目，当遇到要关闭一个activity页面的activity时，一直就是使用这种方式：“****当有多个Activity时，****通过键值对容器动态添加Activity，然后根据key删除相对应的Activity就可以进行跨Activity来关闭界面”；一般这种方式只是针对于零散的方式。对于直接关闭一个activity之前的所有Activity页面很简单，清理activity堆栈就好了：**
```java
Intent inent = new Intent(this, cls);
intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
startActivity(intent);
```
**有更好方法的朋友可以共同交流：***QQ 423923313 蹦蹦哒Android*


