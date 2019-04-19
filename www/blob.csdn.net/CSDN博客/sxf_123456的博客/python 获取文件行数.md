# python 获取文件行数 - sxf_123456的博客 - CSDN博客
2019年01月05日 13:22:26[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：727
方法一：
"对于文件内容比较多，采用enumerate获取文件行数"
count = 0
for index, line in enumerate(f):
    count += 1
print(count)
方法二:
"对于文件内容比较少，采用该方法获取文件行数"
with open("data/aggregation.txt", 'r') as f:
    print(len(f.readlines()))
