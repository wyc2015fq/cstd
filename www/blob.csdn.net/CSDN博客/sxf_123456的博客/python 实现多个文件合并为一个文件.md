# python 实现多个文件合并为一个文件 - sxf_123456的博客 - CSDN博客
2018年10月22日 11:52:25[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：336
```
"""
合并多个文件，将第一个文件的第一列追加到第二个文件的第二列，
并用逗号分隔
"""
import os
def merge(path):
    """
    对多个文件进行合并
    前一个文件的列作为第二个文件的第二列，
    以此类推
    :param path:
    :return:
    """
    list_name = []
    if os.path.isdir(path):
        for filename in os.listdir(path):
            file_path = os.path.join(path, filename)
            if os.path.isdir(file_path):
                merge(file_path)
            else:
                if file_path.endswith(".txt"):
                    list_name.append(file_path)
    list_name.reverse() #列表翻转
    for i in range(len(list_name) - 1):
        read_file_1 = open(list_name[i], "r")
        read_file_2 = open(list_name[i + 1], "r")
        write_file = open(path + "\\temp.txt", "w")
        line_1_list = []
        for line in read_file_1:
            line_1_list.append(line.strip())
        j = 0
        for line_2 in read_file_2:
            line_ = line_2.strip() + "," + line_1_list[j] + "\n"
            write_file.write(line_)
            j = j + 1
            if (j == len(line_1_list)):
                break
        write_file.close()
        read_file_1.close()
        read_file_2.close()
        os.remove(list_name[i + 1])
        os.rename(path + "\\temp.txt", list_name[i + 1])
if __name__ == '__main__':
    path = "E:\\数据处理\\merge"
    merge(path)
```
