# 【Python】输出文件内名字 - YZXnuaa的博客 - CSDN博客
2018年03月06日 10:25:01[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：201
输出文件内所有名字，去除后标。
import os
def text_save(content,filename,mode='a'):
    # Try to save a list variable in txt file.
file = open(filename, mode)
    for i in range(len(content)):
        file.write(str(content[i])+'\n')
    file.close()
file_dir = "/home/ubuntu/视频/VR图片/pic"
def get_files(file_dir):
    namelist = []
    for file in os.listdir(file_dir):
        name = file.split('.')
        namelist.append(name[0])
    return(namelist)
a=get_files(file_dir)
text_save(a, '1.txt')
