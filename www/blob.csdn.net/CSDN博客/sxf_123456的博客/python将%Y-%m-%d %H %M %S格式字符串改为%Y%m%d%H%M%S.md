# python将%Y-%m-%d %H:%M:%S格式字符串改为%Y%m%d%H%M%S - sxf_123456的博客 - CSDN博客
2018年11月27日 16:01:51[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：599
from datetime import datetime
time_str = None 或者 time_str = "" 或者 time_str = "2018-11-27 09:58:25"
try:
    dev_create_time = datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S").strftime("%Y%m%d%H%M%S")
except:
    dev_create_time = datetime.now().strftime("%Y%m%d%H%M%S")
print(dev_create_time)
