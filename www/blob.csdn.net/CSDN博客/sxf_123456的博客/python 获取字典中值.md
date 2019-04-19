# python 获取字典中值 - sxf_123456的博客 - CSDN博客
2017年12月25日 10:17:07[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：351
                
python 获取字典中的值，或字典中不存在则为None值
1、第一种
alarm_name = rs.get(key, None)
print(alarm_name)
2、第二种
if 'ALARMNAME' in rs.keys():
    print(rs['ALARMNAME'])
else:
    print(None)
或者
if 'ALARMNAME' in rs:
    print(rs['ALARMNAME'])
else:
    print(None)
            
