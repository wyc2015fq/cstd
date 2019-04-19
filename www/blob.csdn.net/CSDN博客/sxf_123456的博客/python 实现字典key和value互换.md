# python 实现字典key和value互换 - sxf_123456的博客 - CSDN博客
2018年12月20日 15:53:40[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：52
"""
将字典 {
    "zx": "中兴",
    "hw": "华为"
}
转化为
{
"中兴": "zx",
"华为": "hw"
}
"""
test_dict = {
    "zx": "中兴",
    "hw": "华为"
}
local_reverse = {test_dict[k]: k for k in test_dict}
for key, value in local_reverse.items():  # 迭代输出key,value
    print(key, value)
for key in local_reverse:  # 输出字典所有key
    print(key)
