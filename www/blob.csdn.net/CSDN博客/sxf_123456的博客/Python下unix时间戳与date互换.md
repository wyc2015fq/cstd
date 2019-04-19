# Python下unix时间戳与date互换 - sxf_123456的博客 - CSDN博客
2018年05月23日 19:57:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：73
Python下unix时间戳与date互换
1、date换unix
from datetime import datetime
import time
dtime = datetime.now()
unix_time = time.mktime(dtime.timetuple())
print(unix_time)
1527076390
2、unix时间戳转化为date
from datetime import datetime
unix_time = 1527076390
date_time = datetime.utcfromtimestamp(unix_time)
print(date_time)
2018-05-23 11:53:10
