# 【Java】【WEB】XML配置和注解配置的区别 2018-9-23 - csdn_baotai的博客 - CSDN博客

2018年09月23日 10:18:04[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：137


# XML配置和注解配置的区别

### 使用配置,可以使用XML文件也可以使用注解:

**问题：**XML和注解各自的优缺点是什么,怎么选择?

XML :
- 优点:XML配置很清晰,很直观,因为和Java代码相分离,维护性较高.
- 缺点:XML配置导致配置文件臃肿,开发效率较低.

注解:
- 优点:开发效率高,配置文件不臃肿,也方便快速定位.
- 缺点:和Java代码耦合在一起,维护性较低.

抉择:

一般的,在企业中,现在注解使用的非常之多.

而我们做开发,XML文件用于做通用配置,个别配置使用注解来.

学习阶段:先学XML配置,再学习注解配置.

## 示例：

使用XML做Servlet配置:

![在这里插入图片描述](https://img-blog.csdn.net/20180923101847538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用注解做配置：
![在这里插入图片描述](https://img-blog.csdn.net/20180923101910688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

