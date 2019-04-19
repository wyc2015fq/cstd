# python 求value最大的key值 - sxf_123456的博客 - CSDN博客
2018年09月02日 21:23:05[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：618
d = {"1": 9, "2": 0, "3": 7, "6": 3, "0": 4, "4": 1}
value_1 = max(d, key=d.get)
print(value_1)
value_2 = max(d,key=lambda x:d[x])
print(value_2)
value_3 =sorted(d,key=lambda x:d[x])[-1]
print(value_3)
