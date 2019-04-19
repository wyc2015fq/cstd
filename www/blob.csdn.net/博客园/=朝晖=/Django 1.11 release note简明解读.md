# Django 1.11 release note简明解读 - =朝晖= - 博客园
# [Django 1.11 release note简明解读](https://www.cnblogs.com/dhcn/p/7130775.html)
1、首先1.11这个版本是一个LTS版本
2、第一个支持python3.6的版本，最后一个支持python2.*的版本
3、Deprecating warnings 默认不再显示，同时建议第三方包开始放弃对1.11之前版本的支持
**下面为大的新特性：**
4、Class-based model indexes，基于类的model索引，在meta部分定义索引
5、基于模板的widget渲染
6、子查询表达式
**下面为小的新特性摘记：**
密码加密的次数增加20%
Added QuerySet.union(), intersection(), and difference().
ImageField now has a default validate_image_file_extension validator.
Added FileExtensionValidator to validate file extensions and validate_image_file_extension to validate image files.
**下面是一些不向前兼容的修改：**
Dropped support for PostgreSQL 9.2 and PostGIS 2.0
To simplify Django’s timezone handling, pytz is now a required dependency. It’s automatically installed along with Django.

