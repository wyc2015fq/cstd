# python 实现当大于本周五取本周五的日期，小于本周五取上周五的日期 - sxf_123456的博客 - CSDN博客
2019年03月15日 17:23:51[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：27
    import calendar
    from datetime import datetime, timedelta
    while True:
        today = input("输入今天日期(月日)")
        today_date = datetime.strptime(today, "%Y%m%d")
        if today_date.weekday() - calendar.FRIDAY > 0:
            this_friday = today_date - timedelta(days=(today_date.weekday() - calendar.FRIDAY))
            print(this_friday.strftime("%Y%m%d"))
        else:
            last_firday = today_date - timedelta(days=(today_date.weekday() - calendar.FRIDAY + 7))
            print(last_firday.strftime("%Y%m%d"))
