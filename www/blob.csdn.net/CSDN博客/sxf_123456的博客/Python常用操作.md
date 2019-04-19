# Python常用操作 - sxf_123456的博客 - CSDN博客
2018年04月27日 10:23:39[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：41
                1、字符串拆分之后，存入列表中
    row = []
    line='1,2,3,4'
    row.extend(line.split(','))
    print(row)
2、
row = []
line='1,2,3,4'
line_str = line.split(',')
for i in range(line.count(',')+1):
    row.append(line_str[i])
print(row)
将字符串line中的值存入row列表中，并将列表中的NONE替换为None
row.extend(line.split(','))  # mac, type, factory_name, model_name
row = [None if x.strip() == 'NONE' else x for x in row]
3、组合文件的绝对路径
os.path.abspath(os.path.join(os.path.dirname(__file__),'conf/mysql.json'))            
