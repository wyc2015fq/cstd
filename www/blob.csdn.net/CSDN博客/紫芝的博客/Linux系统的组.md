# Linux系统的组 - 紫芝的博客 - CSDN博客





2018年10月20日 16:07:37[紫芝](https://me.csdn.net/qq_40507857)阅读数：17
个人分类：[Linux基础](https://blog.csdn.net/qq_40507857/article/category/8126592)









# 什么是组？

与用户一样，组也有名称（groupname）和编号（GID），本地组在/etc/group中定义

## 主要组：
- 每个用户有且只有一个主要组
- 对于本地用户，主要组由/etc/passwd 第四个字段中列出的组的GID编号定义；
- 用户创建的新文件由主要组所有
- 新建用户的主要组是名称与用户名相同的新建组，这个用户是此用户专用组（UPG）的唯一成员

## 补充组：
- 用户可以是0个或多个补充组的成员；
- 属于本地组补充成员的用户列在 /etc/group中组条目的最后一个字段
- 对于本地组，用户成员身份由 /etc/group中组条目的最后一个字段中，由逗号分隔用户列表确定
- 补充组成员身份用于确保用户对文件及其他资源的访问权限

**groupname：password：GID：list of users in this group;**

# 管理本地组账户

## groupadd创建组

groupadd groupname：使用/etc/login.defs文件中指定范围的下一个可用GID

groupadd -g GID groupname：指定具体的组编号

![](https://img-blog.csdn.net/20181020171309406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

groupadd -r groupname：使用/etc/login.defs文件中指定范围的下一个可用GID

![](https://img-blog.csdn.net/20181020172638288?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## groupmod修改现有的组

groupmod将组名更改为GID映射

-n选项用于指定新的组名称

![](https://img-blog.csdn.net/20181020172412148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

-g选项用于指定新的GID

![](https://img-blog.csdn.net/20181020172738349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## groupdel删除组

![](https://img-blog.csdn.net/2018102017295340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





