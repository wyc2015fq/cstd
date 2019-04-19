# Django2.1更新日志 - =朝晖= - 博客园
# [Django2.1更新日志](https://www.cnblogs.com/dhcn/p/9418659.html)
       前两天本来想把2.1的release note做个中文摘要，后来发现没什么大的改动。
       不过更新以后还是发下一个bug，或者是已经feature本身的改动，就是QueryDict这个类的实例不能再往里放整数了，否则会报错：
```
File "/usr/local/lib/python3.5/dist-packages/django/http/request.py" in urlencode
  524.                 for v in list_
File "/usr/local/lib/python3.5/dist-packages/django/http/request.py" in <genexpr>
  524.                 for v in list_
Exception Type: AttributeError at /zhaohuogan/archives/
Exception Value: 'int' object has no attribute 'encode'
```
      修改方法，整数强制类型转换为字符串，然后递交给QueryDict实例。
      这个bug会是一个临时bug，我向Django官方提交了这个bug，不过和别人的dup了，官方响应：会恢复原先的特征行为。

