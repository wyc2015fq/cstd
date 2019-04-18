# GSON使用笔记（1） -- 序列化时排除字段的几种方式 - z69183787的专栏 - CSDN博客
2016年09月16日 17:13:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1022
[GSON](http://code.google.com/p/google-gson/)是Google发布的[JSON](http://json.org/)序列化/反序列化工具，非常容易使用。本文简要讨论在使用GSON将Java对象转成JSON时，如何排除某些字段。
## 最简单的用法
假设有下面这个类：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- class MyObj {  
- 
- publicint x;  
- publicint y;  
- 
- public MyObj(int x, int y) {  
- this.x = x;  
- this.y = y;  
-     }  
- 
- }  
最简单的GSON用法如下所示：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- @Test
- publicvoid gson() {  
-     MyObj obj = new MyObj(1, 2);  
-     String json = new Gson().toJson(obj);  
-     Assert.assertEquals("{\"x\":1,\"y\":2}", json);  
- }  
## 方法1：排除null字段
null字段，默认就不会序列化的，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
- class MyObj {  
- 
- privateint intField;  
- private String strField;  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
- @Test
- publicvoid gson() {  
-     MyObj obj = new MyObj();  
-     Assert.assertEquals("{\"intField\":0}", new Gson().toJson(obj));  
- }  
要想序列化null字段，需要显示的进行设置：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
- @Test
- publicvoid serializeNulls() {  
-     MyObj obj = new MyObj();  
-     Gson gson = new GsonBuilder().serializeNulls().create();  
-     Assert.assertEquals("{\"intField\":0,\"strField\":null}", gson.toJson(obj));  
- }  
## 方法2：排除transient字段
这个方法最简单，给字段加上**transient**修饰符就可以了，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- class MyObj {  
- 
- publictransientint x; // <---
- publicint y;  
- 
- public MyObj(int x, int y) {  
- this.x = x;  
- this.y = y;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- @Test
- publicvoid gson() {  
-     MyObj obj = new MyObj(1, 2);  
-     String json = new Gson().toJson(obj);  
-     Assert.assertEquals("{\"y\":2}", json); // <---
- }  
## 方法3：排除Modifier为指定类型的字段
这个方法需要用GsonBuilder定制一个GSON实例，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- class MyObj {  
- 
- protectedint x; // <---
- publicint y;  
- 
- public MyObj(int x, int y) {  
- this.x = x;  
- this.y = y;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- @Test
- publicvoid gson() {  
-     Gson gson = new GsonBuilder()  
-             .excludeFieldsWithModifiers(Modifier.PROTECTED) // <---
-             .create();  
- 
-     MyObj obj = new MyObj(1, 2);  
-     String json = gson.toJson(obj); // <---
-     Assert.assertEquals("{\"y\":2}", json);  
- }  
## 方法4：使用@Expose注解
注意，没有被**@Expose**标注的字段会被排除，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- class MyObj {  
- 
- publicint x;  
- @Exposepublicint y; // <---
- 
- public MyObj(int x, int y) {  
- this.x = x;  
- this.y = y;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- @Test
- publicvoid gson() {  
-     Gson gson = new GsonBuilder()  
-             .excludeFieldsWithoutExposeAnnotation() // <---
-             .create();  
- 
-     MyObj obj = new MyObj(1, 2);  
-     String json = gson.toJson(obj);  
-     Assert.assertEquals("{\"y\":2}", json);  
- }  
## 方法5：使用ExclusionStrategy定制字段排除策略
这种方式最灵活，下面的例子把所有以下划线开头的字段全部都排除掉：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- class MyObj {  
- 
- publicint _x; // <---
- publicint y;  
- 
- public MyObj(int x, int y) {  
- this._x = x;  
- this.y = y;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21471005#)[copy](http://blog.csdn.net/zxhoo/article/details/21471005#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/243069/fork)
- @Test
- publicvoid gson() {  
-     ExclusionStrategy myExclusionStrategy = new ExclusionStrategy() {  
- 
- @Override
- publicboolean shouldSkipField(FieldAttributes fa) {  
- return fa.getName().startsWith("_"); // <---
-         }  
- 
- @Override
- publicboolean shouldSkipClass(Class<?> clazz) {  
- returnfalse;  
-         }  
- 
-     };  
- 
-     Gson gson = new GsonBuilder()  
-             .setExclusionStrategies(myExclusionStrategy) // <---
-             .create();  
- 
-     MyObj obj = new MyObj(1, 2);  
-     String json = gson.toJson(obj);  
-     Assert.assertEquals("{\"y\":2}", json);  
- }  
- 
