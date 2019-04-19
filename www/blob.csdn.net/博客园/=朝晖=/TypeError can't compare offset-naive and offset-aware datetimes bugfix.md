# TypeError: can't compare offset-naive and offset-aware datetimes bugfix - =朝晖= - 博客园
# [TypeError: can't compare offset-naive and offset-aware datetimes bugfix](https://www.cnblogs.com/dhcn/p/7116206.html)
参考：https://docs.djangoproject.com/en/1.8/topics/i18n/timezones/#naive-and-aware-datetime-objects
起因：正常的dateime.now()得到的日期不能和Django[数据库](http://lib.csdn.net/base/mysql)里面存储的日期数据做对比，两个解决办法：
1、是把Django配置里面的USE_TZ设置成False，这样Django的数据就没有时区信息了。
2、是在这个对比情景下，不要用datetime.now()来得当前数据，用以下代码：
```
from django.utils import timezone  
  
now = timezone.now()
```

