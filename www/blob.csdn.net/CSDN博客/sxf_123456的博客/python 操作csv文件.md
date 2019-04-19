# python 操作csv文件 - sxf_123456的博客 - CSDN博客
2018年11月27日 17:25:35[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：37标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
import csv
import os
def write_csv(path):
    #newline=""是行与行之间不是用换行符分开，默认是用换行符
    with open(path + "test.csv", "w", newline="") as f:
        for i in range(100):
            list_ = []
            list_.append(i)
            csv_w = csv.writer(f, dialect="excel")
            csv_w.writerow(list_)
    f.close()
def read_csv(path):
    with open(path+'test.csv',"r") as f:
        r_csv = csv.reader(f)
        sum_value = 0
        for row in r_csv:
            sum_value += int(row[0])
        print(sum_value)
    f.close()
if __name__ == '__main__':
    _path = "E:\\公司\\案例数据采集\\data\\"
    if os.path.exists(_path):
        pass
    else:
        os.makedirs(_path)
    write_csv(_path) #写入csv文件
    read_csv(_path) #读取csv文件
