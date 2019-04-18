# 5分钟了解Mockito - z69183787的专栏 - CSDN博客
2016年10月14日 18:25:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1206
 一、什么是mock测试，什么是mock对象？
先来看看下面这个示例：
![](http://dl.iteye.com/upload/attachment/0067/5882/02030f95-ba9a-3104-b0f1-d7d8f02029fd.png)
从上图可以看出如果我们要对A进行测试，那么就要先把整个依赖树构建出来，也就是BCDE的实例。
一种替代方案就是使用mocks
![](http://dl.iteye.com/upload/attachment/0067/5884/2fab8997-d489-396e-9365-2ae1fe94b6c2.png)
从图中可以清晰的看出
mock对象就是在调试期间用来作为真实对象的替代品。
mock测试就是在测试过程中，对那些不容易构建的对象用一个虚拟对象来代替测试的方法就叫mock测试。
知道什么是mock测试后，那么我们就来认识一下mock框架---Mockito
二、什么是Mockito
除了有一个好记的名字外，Mockito尝试用不一样的方法做mocking测试，是简单轻量级能够替代EasyMock的框架。使用简单，测试代码可读性高，丰富的文档包含在javadoc中，直接在IDE中可查看文档，实例，说明。更多信息：[http://code.google.com/p/mockito/](http://code.google.com/p/mockito/)
三、Stub和Mock
相同点：Stub和Mock对象都是用来模拟外部依赖，使我们能控制。
不同点：而stub完全是模拟一个外部依赖，用来提供测试时所需要的测试数据。而mock对象用来判断测试是否能通过，也就是用来验证测试中依赖对象间的交互能否达到预期。在mocking框架中mock对象可以同时作为stub和mock对象使用，两者并没有严格区别。 更多信息：[http://martinfowler.com/articles/mocksArentStubs.html](http://martinfowler.com/articles/mocksArentStubs.html)
四、mockito入门实例
Maven依赖：(没用maven管理的可以下载相关jar包导入classpath)
Xml代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- <dependencies>
- <dependency>
- <groupId>org.mockito</groupId>
- <artifactId>mockito-all</artifactId>
- <version>1.8.5</version>
- <scope>test</scope>
- </dependency>
- </dependencies>
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- importstatic org.mockito.Mockito.*;  
- 
- import java.util.List;  
- 
- import org.junit.Assert;  
- import org.junit.Test;  
- 
- /**
-  * 
-  * @author lzjun
-  * @version 0.1
-  * @date 2012-5-5
-  * {@link http://weibo.com/u/1697702241} 
-  *
-  */
- publicclass SimpleTest {  
- 
- @Test
- publicvoid simpleTest(){  
- 
- //创建mock对象，参数可以是类，也可以是接口
-         List<String> list = mock(List.class);  
- 
- //设置方法的预期返回值
-         when(list.get(0)).thenReturn("helloworld");  
- 
-         String result = list.get(0);  
- 
- //验证方法调用(是否调用了get(0))
-         verify(list).get(0);  
- 
- //junit测试
-         Assert.assertEquals("helloworld", result);  
-     }  
- }  
好了，五分钟差不多了，还想继续了解那就可以往下面看![](http://liuzhijun.iteye.com/images/smiles/icon_biggrin.gif)
创建mock对象不能对final，Anonymous ，primitive类进行mock。
可对方法设定返回异常
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- when(list.get(1)).thenThrow(new RuntimeException("test excpetion"));  
stubbing另一种语法(设置预期值的方法)，可读性不如前者
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- doReturn("secondhello").when(list).get(1);  
没有返回值的void方法与其设定(支持迭代风格，第一次调用donothing,第二次dothrow抛出runtime异常)
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- doNothing().doThrow(new RuntimeException("void exception")).when(list).clear();  
- list.clear();  
- list.clear();  
- verify(list,times(2)).clear();  
五、参数匹配器(Argument Matcher)
Matchers类内加你有很多参数匹配器  anyInt、anyString、anyMap.....Mockito类继承于Matchers,Stubbing时使用内建参数匹配器，下例：
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- @Test
- publicvoid argumentMatcherTest(){  
- 
-     List<String> list = mock(List.class);  
- 
-     when(list.get(anyInt())).thenReturn("hello","world");  
- 
-     String result = list.get(0)+list.get(1);  
- 
-     verify(list,times(2)).get(anyInt());  
- 
-     Assert.assertEquals("helloworld", result);  
- 
- }  
 需要注意的是：如果使用参数匹配器，那么所有的参数都要使用参数匹配器，不管是stubbing还是verify的时候都一样。
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- @Test
- publicvoid argumentMatcherTest2(){  
- 
-     Map<Integer,String> map = mock(Map.class);  
-     when(map.put(anyInt(),anyString())).thenReturn("hello");//anyString()替换成"hello"就会报错
-     map.put(1, "world");  
-     verify(map).put(eq(1), eq("world")); //eq("world")替换成"world"也会报错
- 
- }  
 六、方法调用的验证(具体的调用次数、至少一次，一次也没有)
Java代码  ![收藏代码](http://liuzhijun.iteye.com/images/icon_star.png)
- @Test
- publicvoid verifyInvocate(){  
- 
-     List<String> mockedList = mock(List.class);  
- //using mock 
-      mockedList.add("once");  
-      mockedList.add("twice");  
-      mockedList.add("twice");  
- 
-      mockedList.add("three times");  
-      mockedList.add("three times");  
-      mockedList.add("three times");  
- 
- /**
-       * 基本的验证方法
-       * verify方法验证mock对象是否有没有调用mockedList.add("once")方法
-       * 不关心其是否有返回值，如果没有调用测试失败。
-       */
-      verify(mockedList).add("once");   
-      verify(mockedList, times(1)).add("once");//默认调用一次,times(1)可以省略
- 
- 
-      verify(mockedList, times(2)).add("twice");  
-      verify(mockedList, times(3)).add("three times");  
- 
- //never()等同于time(0),一次也没有调用
-      verify(mockedList, times(0)).add("never happened");  
- 
- //atLeastOnece/atLeast()/atMost()
-      verify(mockedList, atLeastOnce()).add("three times");  
-      verify(mockedList, atLeast(2)).add("twice");  
-      verify(mockedList, atMost(5)).add("three times");  
- 
- }  
一次写不完，慢慢分析。。。
参考：
[http://mockito.googlecode.com/svn/branches/1.6/javadoc/org/mockito/Mockito.html](http://mockito.googlecode.com/svn/branches/1.6/javadoc/org/mockito/Mockito.html)
[http://www.sizovpoint.com/2009/03/java-mock-frameworks-comparison.html](http://www.sizovpoint.com/2009/03/java-mock-frameworks-comparison.html)
[http://wenku.baidu.com/view/8def451a227916888486d73f.html](http://wenku.baidu.com/view/8def451a227916888486d73f.html)
[http://qiuguo0205.iteye.com/blog/1443344](http://qiuguo0205.iteye.com/blog/1443344)
