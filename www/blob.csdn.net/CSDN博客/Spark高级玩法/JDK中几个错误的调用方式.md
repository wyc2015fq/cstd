# JDK中几个错误的调用方式 - Spark高级玩法 - CSDN博客
2019年03月04日 22:54:19[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：67
> 
来源：my.oschina.net/tommylemon/blog/2967187
1.Annotation引用非空enum数组返回空数组
首次发现时的环境：JDK 1.8
首次发现所在项目：APIJSON
测试用例：
```
public enum RequestRole {
 /**未登录，不明身份的用户
  */
 UNKNOWN,
 /**已登录的用户
  */
 LOGIN,
 /**联系人，必须已登录
  */
 CONTACT,
 /**圈子成员(CONTACT + OWNER)，必须已登录
  */
 CIRCLE,
 /**拥有者，必须已登录
  */
 OWNER,
 /**管理员，必须已登录
  */
 ADMIN;
 //似乎不管怎么做，外部引用后都是空值。并且如果在注解内的位置不是最前的，还会导致被注解的类在其它类中import报错。
 //虽然直接打印显示正常，但被@MethodAccess内RequestRole[] GET()等方法引用后获取的是空值
 public static final RequestRole[] ALL = {RequestRole.UNKNOWN};//values();//所有
 public static final RequestRole[] HIGHS;//高级
 static {
   HIGHS = new RequestRole[] {OWNER, ADMIN};
 }
 public static final String[] NAMES = {
     UNKNOWN.name(), LOGIN.name(), CONTACT.name(), CIRCLE.name(), OWNER.name(), ADMIN.name()
 };
}
@MethodAccess(
   GETS = RequestRole.ALL,
   HEADS = RequestRole.HIGHS
   )
public class Verify {
}
public class DemoVerifier {
 // <TableName, <METHOD, allowRoles>>
 // <User, <GET, [OWNER, ADMIN]>>
   public static final Map<String, Map<RequestMethod, RequestRole[]>> ACCESS_MAP;
 static { //注册权限
       ACCESS_MAP = new HashMap<String, Map<RequestMethod, RequestRole[]>>();
   ACCESS_MAP.put(Verify.class.getSimpleName(), getAccessMap(Verify.class.getAnnotation(MethodAccess.class)));
 }
 public static HashMap<RequestMethod, RequestRole[]> getAccessMap(MethodAccess access) {
   if (access == null) {
     return null;
   }
   HashMap<RequestMethod, RequestRole[]> map = new HashMap<>();
   map.put(GET, access.GET());
   map.put(HEAD, access.HEAD());
   map.put(GETS, access.GETS());
   map.put(HEADS, access.HEADS());
   map.put(POST, access.POST());
   map.put(PUT, access.PUT());
   map.put(DELETE, access.DELETE());
   return map;
 }
}
```
解决方案：
不抽象数组常量ALL,HIGHTS等，而是在每个用到的地方硬编码写死具体的值。
2.ArrayList可通过构造函数传入非指定泛型的List并在get时出错
首次发现时的环境：JDK 1.7
首次发现所在项目：APIJSON
测试用例：
```
JSONArray arr = new JSONArray(); //com.alibaba.fastjson.JSONArray
arr.add("s");
List<Long> list = new ArrayList<>(arr); 
list.get(0); //Exception cannot cast String to Long
```
解决方案：
1.改用 Open JDK8
2.升级 JDK
注：后面多次测试，已无法复现。
3.基本类型在三元表达式内可赋值为null，编译通过但运行出错
首次发现时的环境： JDK 1.7
测试用例：
`int i = true ? null : 0; //Exception in thread "main" java.lang.NullPointerException`
首次发现所在项目：ZBLibrary
解决方案：
在给基础类型用3元表达式赋值时，null 先转为基础类型的默认值。
最后再提2个不是bug，但容易引发编程bug的问题：
1.局部变量和同名的全局变量能在一个方法内，编译通过，运行也正常。
```
public class Test {
       
       int val;
       @Override
       public String toString() {
           val = 1;
           String val = "";
           return super.toString();
       }
   }
```
如果两个变量中间隔了比较长的其它代码，很可能会导致开发人员将两者混淆，导致逻辑认知错误，从而写出或改出有问题的代码。
解决方案：
命名局部变量前先搜素，确保没有已声明的同名全局变量。
2. （非 JDK bug）Gson 通过 TypeToken 转换 List<T> 能写入不属于 T 类型的数据，get 出来赋值给 T 类型的变量/常量报错。
```
String json = "[1, '2', 'a']";
       Type type = new TypeToken<Integer>(){}.getType();
       Gson gson = new Gson();
       List<Integer> list = gson.fromJson(json, type);
       
       Integer i = list == null || list.isEmpty() ? null : list.get(1); //Exception cannot cast String to Integer
```
解决方案：
1.手动检查列表内数据都符合泛型 T
2.改用 fastjson 等其它能静态检查类型的库。
最后，我建立几个微信技术群，里面有我不少大厂的朋友，主要分享一些技术，大厂招聘信息等，如果你也感兴趣的话，可以访问下面这条链接来加入我们。
[漫谈数据质量监控](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486497&idx=1&sn=b5a6d160f5ae64f50a9618ff595bc33c&chksm=9f38ef09a84f661f842d492ffdef5dca01b7bb7222f6d6ab18c8b642889f9cd9cf569953838b&scene=21#wechat_redirect)
[成为大数据高手的活法-晚上复盘](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486441&idx=1&sn=4ff73e34c15e39c79a171e8f1d7f4112&chksm=9f38e8c1a84f61d721f669ff35dfa8ca07c62ad67a06d91cd490688dba7f7a08403bbba8c9ea&scene=21#wechat_redirect)
**---END---**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
