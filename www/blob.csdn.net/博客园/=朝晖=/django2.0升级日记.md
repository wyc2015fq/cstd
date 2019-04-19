# django2.0升级日记 - =朝晖= - 博客园
# [django2.0升级日记](https://www.cnblogs.com/dhcn/p/8016665.html)
    1、大量的外键定义需要加on_delete参数
    2、'WSGIRequest' object has no attribute 'session'，这个问题是因为settings中middleware的定义参数变了，得把MIDDLEWARE_ClASSES改成MIDDLEWARE。

