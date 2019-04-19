# 将字典中所有的key转化为列表 - sxf_123456的博客 - CSDN博客
2018年02月01日 11:25:03[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：563
dict = {**"10.162"**: **"SC_CD"**, **"10.164"**: **"SC_CD"**, **"10.165"**: **"SC_CD"**}
list_ip = []
**for **i **in **range(len(dict)):
    list_ip = list(dict.keys())
print(list_ip)
