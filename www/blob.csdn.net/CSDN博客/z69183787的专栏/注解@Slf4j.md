# 注解@Slf4j - z69183787的专栏 - CSDN博客
2019年01月04日 19:37:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：60
个人分类：[工具-Lombok](https://blog.csdn.net/z69183787/article/category/5834225)
[https://blog.csdn.net/cslucifer/article/details/80953400](https://blog.csdn.net/cslucifer/article/details/80953400)
　如果不想每次都写private  final Logger logger = LoggerFactory.getLogger(XXX.class); 可以用注解@Slf4j
一、在pom文件加入依赖
<dependency>
   <groupId>org.projectlombok</groupId>
   <artifactId>lombok</artifactId>
</dependency>
二、代码
package com.sell;
import lombok.extern.slf4j.Slf4j;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.junit.runner.RunWith;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
*/***
* *   日志测试*
* */*
@RunWith(SpringRunner.class)
@SpringBootTest
@Slf4j
public class LoggerTest {
    private  final Logger logger = LoggerFactory.*getLogger*(LoggerTest.class);
*/***
*     * 一、传统方式实现日志*
*     */*
@Test
    public  void test1(){
        logger.debug("debug message");
        logger.warn("warn message");
        logger.info("info message");
        logger.error("error message");
        logger.trace("trace message");
    }
*/***
*     * 二、注解方式实现日志*
*     */*
@Test
   public  void test2(){
*log*.debug("debug message");
*log*.warn("warn message");
*log*.info("info message");
*log*.error("error message");
*log*.trace("trace message");
    }
}
输出如下：
![](https://images2017.cnblogs.com/blog/727732/201710/727732-20171007005038333-722076757.png)
因为默认是输出 info之上的 ， 由下图可知 debug，trace不输出
![](https://images2017.cnblogs.com/blog/727732/201710/727732-20171007005125724-514302687.png)
三、注意：如果注解@Slf4j注入后找不到变量log，那就给IDE安装lombok插件，、
下面以idea为例
1、File  → settings →  Plugins,  然后点击“Browse repositories” 如图
![](https://images2017.cnblogs.com/blog/727732/201710/727732-20171007005153630-156372538.png)
2、输入 lombok 搜索插件， 点install安装，安装完重启idea
![](https://images2017.cnblogs.com/blog/727732/201710/727732-20171007005212755-557704490.png)
这时候输入 log就可以有提示了
