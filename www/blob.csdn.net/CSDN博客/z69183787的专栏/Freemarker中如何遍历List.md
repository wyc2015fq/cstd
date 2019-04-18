# Freemarker中如何遍历List - z69183787的专栏 - CSDN博客
2015年09月21日 11:18:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2536
个人分类：[模板引擎-Freemarker](https://blog.csdn.net/z69183787/article/category/5652267)
Freemarker中如何遍历List（附源码）
关键词(Keyword)：Freemarker,Freemarker遍历list
在Freemarker应用中经常会遍历List获取需要的数据，并对需要的数据进行排序加工后呈现给用户。那么在Freemarker中如何遍历List，并对List中数据进行适当的排序呢？
通过下文的介绍，相信您一定会找到答案。
一、 Freemarker中list指令简单介绍
要想在Freemarker中遍历list,必须通过使用list指令,即<#list sequence as item>…</#list>
sequence是集合(collection)的表达式，item是循环变量的名字，不能是表达式。
当在遍历sequence时，会将遍历变量的值保存到item中。
举个例子说明吧：
<#list userList as user>
…
</#list>
userList中封装了很多个User对象，我们在遍历userList时候，会将遍历的User对象的值，保存到上述的user变量中。那么在取值时，我们可以通过${user.userName }来获取User对象的userName属性值。
List指令还隐含了两个循环变量：
item_index:当前迭代项在所有迭代项中的位置，是数字值。
item_has_next:用于判断当前迭代项是否是所有迭代项中的最后一项。
注意：在使用上述两个循环变量时，一定要将item换成你自己定义的循环变量名,item其实就是前缀罢了。
例如，如果你使用<# list list as l>..</#list>定义，那么就要使用l_index,l_has_next。
在循环过程中，如果您想跳出循环，那么可以使用结合break指令，即<#break>来完成。
二、 Freemarker中对List进行排序
通常我们的排序操作都是通过DAO层来实现的，如果我们想随时更改我们的排序，那么就必须修改我们的DAO层代码，确实不方便。但Freemarker为我们提供了这样的排序方法，解决了这个问题。
1. sort升序排序函数
sort对序列(sequence)进行排序，要求序列中的变量必须是：字符串（按首字母排序）,数字，日期值。
<#list list?sort as l>…</#list>
2. sort_by函数
sort_by有一个参数,该参数用于指定想要排序的子变量，排序是按照变量对应的值进行排序,如：
<#list userList?sort_by(“age”) as user>…</#list>
age是User对象的属性，排序是按age的值进行的。
3. reverse降序排序函数
<#list list? reverse as l>…</#list>。reverse使用同sort相同。reverse还可以同sort_by一起使用
如：想让用户按年龄降序排序，那么可以这个样写<#list userList?sort_by(“age”)?reverse as user>…</#list>
三、 Freemarker如何遍历List实例应用
创通过上面的介绍，相信您已经对Freemarker如何遍历List有了了解，那么我们就废话少说，开始做一个应用吧。
User类
public class User{
private String username;
private
（省略set和get方法）
}
user.ftl
<#--Freemarker遍历list-->
简单遍历list:
<#list userList as user>
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
</#list>
<#--Freemarker遍历list并应用list隐含变量item_index-->
item_index使用：
<#list userList as user>
第${user_index+1}个用户
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
</#list>
<#--Freemarker遍历list并应用list隐含变量item_has_next-->
item_has_next,size使用：
<#list userList as user>
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
<#if !user_has_next>
共有${userList?size}最后一个用户是:${user.userName}
</#if>
</#list>
<#--Freemarker遍历list并按用户年龄升序排序-->
按用户年龄升序排序：
<#list userList?sort_by("age") as user>
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
</#list>
<#--Freemarker遍历list并按用户年龄降序排序-->
按用户年龄降序排序：
<#list userList?sort_by("age")?reverse as user>
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
</#list>
<#--Freemarker遍历list当用户年龄大于21岁时，停止输出-->
list中应用break:
<#list userList?sort_by("age")?reverse as user>
用户名：${user.userName}
密 码：${user.userPassword}
年 龄: ${user.age}
<#if (user.age>21) >
<#break>
</#if>
</#list>
ClientTest类
public class ClientTest{
public static List<User> initUserList(){
User user1=new User();
user1.setUserName("张三");
user1.setUserPassword("123");
user1.setAge(20);
User user2=new User();
user2.setUserName("李四");
user2.setUserPassword("123");
user2.setAge(22);
User user3=new User();
user3.setUserName("王五");
user3.setUserPassword("123");
user3.setAge(21);
List<User> list=new ArrayList<User>();
list.add(user1);
list.add(user2);
list.add(user3);
return list;
}
public static void main(String[] args){
List<User> list=ClientTest.initUserList(); 
Map<String,Object> root=new HashMap<String,Object>();
root.put(“userList”,list);
FreeMarkertUtil.analysisTemplate(“user.ftl”,”UTF-8”,root);
//FreeMarkUtil类可以参考我上传得源码。
}
}
好了，到这里程序就结束了，您可以运行ClientTest类来查看输出结果了。
希望通过本文的介绍，可以让您对FreeMarker如何遍历List有一个清楚的了解。
