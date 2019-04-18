# 关于toString方法的重写工具ToStringBuilder - z69183787的专栏 - CSDN博客
2017年07月26日 14:41:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1840
apache的commons-lang3的工具包里有一个ToStringBuilder类，这样在打日志的时候可以方便的打印出类实例中的各属性的值。
具体用法如下：
[java][view
 plain](http://blog.csdn.net/zhaowen25/article/details/39521899#)[copy](http://blog.csdn.net/zhaowen25/article/details/39521899#)
- import org.apache.commons.lang3.builder.ToStringBuilder;  
- import org.apache.commons.lang3.builder.ToStringStyle;  
- 
- publicclass Message {  
- 
- private String from;  
- 
- private String to;  
- 
- private String body;  
- 
- public String getFrom() {  
- return from;  
-     }  
- 
- publicvoid setFrom(String from) {  
- this.from = from;  
-     }  
- 
- public String getTo() {  
- return to;  
-     }  
- 
- publicvoid setTo(String to) {  
- this.to = to;  
-     }  
- 
- public String getBody() {  
- return body;  
-     }  
- 
- publicvoid setBody(String body) {  
- this.body = body;  
-     }  
- 
- @Override
- public String toString() {  
- return ToStringBuilder.reflectionToString(this, ToStringStyle.MULTI_LINE_STYLE);  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         Message msg = new Message();  
-         msg.setFrom("vince");  
-         msg.setTo("mike");  
-         msg.setBody("hello");  
-         System.out.println(msg.toString());  
-     }  
- }  
而且支持多种打印格式
多行输出的：
com.vince.im.dto.Message@af72d8[
  from=vince
  to=mike
  body=hello
]
默认一行的：
com.vince.im.dto.Message@af72d8[from=vince,to=mike,body=hello]
NO_FIELD_NAMES_STYLE：
com.vince.im.dto.Message@af72d8[vince,mike,hello]
SHORT_PREFIX_STYLE：
Message[from=vince,to=mike,body=hello]
SIMPLE_STYLE：
vince,mike,hello
原理其实就是通过JAVA的reflect（反射）获取值，然后组成一个Buffer。
里面部分源码：
[java][view
 plain](http://blog.csdn.net/zhaowen25/article/details/39521899#)[copy](http://blog.csdn.net/zhaowen25/article/details/39521899#)
- /**
-      * <p>Append to the <code>toString</code> the start of data indicator.</p>
-      * 拼装结果的
-      * @param buffer  the <code>StringBuffer</code> to populate
-      * @param object  the <code>Object</code> to build a <code>toString</code> for
-      */
- publicvoid appendStart(final StringBuffer buffer, final Object object) {  
- if (object != null) {  
-             appendClassName(buffer, object);  
-             appendIdentityHashCode(buffer, object);  
-             appendContentStart(buffer);  
- if (fieldSeparatorAtStart) {  
-                 appendFieldSeparator(buffer);  
-             }  
-         }  
-     }  
- 
- /**
-      * <p>Append the {@link System#identityHashCode(java.lang.Object)}.</p>
-      * 拼装对象hashcode
-      * @param buffer  the <code>StringBuffer</code> to populate
-      * @param object  the <code>Object</code> whose id to output
-      */
- protectedvoid appendIdentityHashCode(final StringBuffer buffer, final Object object) {  
- if (this.isUseIdentityHashCode() && object!=null) {  
-             register(object);  
-             buffer.append('@');  
-             buffer.append(Integer.toHexString(System.identityHashCode(object)));  
-         }  
-     }  
需要注意的是：
Builds a `toString` value
 using the default `ToStringStyle` through
 reflection.
It uses `AccessibleObject.setAccessible` to
 gain access to private fields. This means that it will throw a security exception if run under a security manager, if the permissions are not set up correctly. It is also not as efficient as testing explicitly.
Transient
 members will be not be included, as they are likely derived. Static fields will not be included. Superclass fields will be appended.
也就是说transient和static修饰的属性不能打印出来，但是父类的是可以打印出来的，使用的时候一定要注意了。
