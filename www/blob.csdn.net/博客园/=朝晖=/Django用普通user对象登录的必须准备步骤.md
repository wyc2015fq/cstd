# Django用普通user对象登录的必须准备步骤 - =朝晖= - 博客园
# [Django用普通user对象登录的必须准备步骤](https://www.cnblogs.com/dhcn/p/7105648.html)
zt from [http://segmentfault.com/q/1010000000343563](http://segmentfault.com/q/1010000000343563)
在stackoverflow找到了解答（[http://stackoverflow.com/questions/5775268/django-1-2-session-loss-so-hasnt-answered-this-question-successfully-yet](http://stackoverflow.com/questions/5775268/django-1-2-session-loss-so-hasnt-answered-this-question-successfully-yet)）.
```python
user.backend='django.contrib.auth.backends.ModelBackend'
```
重新设置用户的'backend'属性，然后就可以使用login（）函数登录该用户。

