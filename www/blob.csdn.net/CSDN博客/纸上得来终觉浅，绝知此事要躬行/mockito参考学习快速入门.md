# mockito参考学习快速入门 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月25日 15:43:16[boonya](https://me.csdn.net/boonya)阅读数：475标签：[java																[mockito																[junit](https://so.csdn.net/so/search/s.do?q=junit&t=blog)
个人分类：[Junit/Mock](https://blog.csdn.net/boonya/article/category/1731095)





## 学习资源参考地址

官方网站：[http://site.mockito.org/](http://site.mockito.org/)


中文翻译：[https://github.com/hehonghui/mockito-doc-zh](https://github.com/hehonghui/mockito-doc-zh)



API文档：[http://docs.mockito.googlecode.com/hg/org/mockito/Mockito.html](http://docs.mockito.googlecode.com/hg/org/mockito/Mockito.html)

项目源码：[https://github.com/mockito/mockito](https://github.com/mockito/mockito)

## Maven项目依赖

mockito依赖:




```
<dependency>  
          <groupId>org.mockito</groupId>  
          <artifactId>mockito-all</artifactId>  
          <version>2.7.22</version>  
          <scope>test</scope>  
</dependency>
```
junit依赖:

```
<dependency>  
      <groupId>junit</groupId>  
      <artifactId>junit</artifactId>  
      <version>4.11</version>  
      <scope>test</scope>  
    </dependency>
```


## Java简单测试样例



```java
@Test  
    public void verify_behaviour(){  
        //模拟创建一个List对象  
        List mock = mock(List.class);  
        //使用mock的对象  
        mock.add(1);  
        mock.clear();  
        //验证add(1)和clear()行为是否发生  
        verify(mock).add(1);  
        verify(mock).clear();  
    }
```](https://so.csdn.net/so/search/s.do?q=mockito&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




