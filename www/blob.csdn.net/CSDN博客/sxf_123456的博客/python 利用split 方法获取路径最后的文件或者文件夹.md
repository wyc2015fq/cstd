# python 利用split 方法获取路径最后的文件或者文件夹 - sxf_123456的博客 - CSDN博客
2019年01月03日 15:32:20[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：324
import os
local_filename = "/root/path/treee"
filename = os.path.split(local_filename)[1]
print(filename)
