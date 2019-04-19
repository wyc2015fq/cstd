# unittest报告出现dict() -> new empty dictionary错误解决办法 - _天枢 - 博客园
## [unittest报告出现dict() -> new empty dictionary错误解决办法](https://www.cnblogs.com/yhleng/p/9805125.html)
2018-10-17 16:45 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9805125)
unittest报告出现dict() -> new empty dictionary错误解决办法
说一下原因，这是由于unittest中采用了ddt驱动。  由于版本问题导致
 问题如图：
![](https://img2018.cnblogs.com/blog/1149221/201810/1149221-20181017163800139-1512002577.png)
解决办法：
重新安装ddt版本，降低其版本
1>查看ddt版本：
```
pip show ddt
```
![](https://img2018.cnblogs.com/blog/1149221/201810/1149221-20181017163912155-827072998.png)
2>卸载已安装的ddt
```
pip uninstall ddt
```
3>重新安装ddt 并指定其版本
```
pip install ddt==1.1.3
```
 对比一下两个版本代码：
旧版:1.1.3
```
for name, func in list(cls.__dict__.items()):
        if hasattr(func, DATA_ATTR):
            for i, v in enumerate(getattr(func, DATA_ATTR)):
                test_name = mk_test_name(name, getattr(v, "__name__", v), i)
　　　　　　　　　** #区别就在此处                **if hasattr(func, UNPACK_ATTR):
                    if isinstance(v, tuple) or isinstance(v, list):
                        add_test(cls, test_name, func, *v)
                    else:
                        # unpack dictionary
                        add_test(cls, test_name, func, **v)
                else:
                    add_test(cls, test_name, func, v)
            delattr(cls, name)
        elif hasattr(func, FILE_ATTR):
            file_attr = getattr(func, FILE_ATTR)
            process_file_data(cls, name, func, file_attr)
            delattr(cls, name)
    return cls
```
新版1.2.0
```
for name, func in list(cls.__dict__.items()):
        if hasattr(func, DATA_ATTR):
            for i, v in enumerate(getattr(func, DATA_ATTR)):
                test_name = mk_test_name(name, getattr(v, "__name__", v), i)
                **test_docstring ****= getattr(v, "__doc__", None)**
                if hasattr(func, UNPACK_ATTR):
                    if isinstance(v, tuple) or isinstance(v, list):
                        add_test(cls, test_name, **test_docstring**, func, *v)
                    else:
                        # unpack dictionary
                        add_test(cls, test_name, **test_docstring**, func, **v)
                else:
                    add_test(cls, test_name, **test_docstring**, func, v)
            delattr(cls, name)
        elif hasattr(func, FILE_ATTR):
            file_attr = getattr(func, FILE_ATTR)
            process_file_data(cls, name, func, file_attr)
            delattr(cls, name)
    return cls
```
从上边的两个ddt代码段，可以看出，新版新增加了一个test_docstring  这个是指定报告中输出的字符串。
这个新版增加了，应该在代码中指定描述。
