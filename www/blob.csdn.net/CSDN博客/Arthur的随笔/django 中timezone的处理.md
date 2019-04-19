# django 中timezone的处理 - Arthur的随笔 - CSDN博客
2012年06月09日 10:31:41[largetalk](https://me.csdn.net/largetalk)阅读数：11747
[http://www.cnblogs.com/c9com/archive/2012/06/06/2507912.html](http://www.cnblogs.com/c9com/archive/2012/06/06/2507912.html) 中说了django中fromtimestamp的一个奇怪问题，我试了试，我本地没这问题，后来看到后面写到是因为重庆时区变更导致得，就翻了下django代码，想看看django是怎么处理时区得。
我们在settings文件中加入， TIME_ZONE='Asia/Shanghai', 就可以用上海本地时间了
一般我们自己很少去管理django中得时区，都是django帮我们弄好了。搜索了下代码，发现在 utils/timezone.py文件中函数get_default_timezone
```python
def get_default_timezone():
    """
    Returns the default time zone as a tzinfo instance.
    This is the time zone defined by settings.TIME_ZONE.
    See also :func:`get_current_timezone`.
    """
    global _localtime
    if _localtime is None:
        if isinstance(settings.TIME_ZONE, basestring) and pytz is not None:
            _localtime = pytz.timezone(settings.TIME_ZONE)
        else:
            _localtime = LocalTimezone()
    return _localtime
```
原来django 是用了一个叫pytz得东西来管理时区得， 到pytz代码下搜索timezone发现
```python
def timezone(zone):
zone = _unmunge_zone(zone)
    if zone not in _tzinfo_cache:
        if zone in all_timezones_set:
            fp = open_resource(zone)
            try:
                _tzinfo_cache[zone] = build_tzinfo(zone, fp)
            finally:
                fp.close()
        else:
            raise UnknownTimeZoneError(zone)
    return _tzinfo_cache[zone]
```
open_resource功能是读取zoneinfo文件夹下对应文件，不过这些文件都是二进制，实在看不出来什么，追踪也就到此为止了。
通过看代码，能知道很多原来不知道得东西。比如pytz这个东西，原来使用datetime时老是搞不清楚tzinfo这个东西，这次之后就都明白了，而且pytz又变成了我以后可用得工具，不错。以前代码量比较少，最近要多看代码，多写代码。
######################################################
2013.03.29
最近一个项目使用mongodb作为数据库，mongodb里存的数据就是UTC+0的时间，但他并，没有保存时区信息，只是在保存数据时，如果时间有时区，就转成UTC时间然后保存，如果没有时区信息，就默认为是UTC时间直接保存。为了编程方便和避免错误，我把django的TIME_ZONE设为‘Africa/Accra' （UTC+0）， 但页面上还是需要根据本地环境来设置时间的，不过django这些都还不错，基本不用自己来做： [https://docs.djangoproject.com/en/dev/topics/i18n/timezones/#selecting-the-current-time-zone](https://docs.djangoproject.com/en/dev/topics/i18n/timezones/#selecting-the-current-time-zone)
另外，稍微修改了下从mongodb读取时间类型的代码，使得带上时区，这样在后端处理的时候就很方便了
