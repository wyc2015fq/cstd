# python 自动识别 文件编码格式，并将原文件编码格式转化为utf-8格式 - sxf_123456的博客 - CSDN博客
2019年03月21日 15:22:02[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：48
"""
自动识别 文本编码格式
"""
import chardet
def detectCode(path):
    with open(path, 'rb') as file:
        data = file.read(20000)
        dicts = chardet.detect(data)
    return dicts["encoding"]
def print_data_1(path):
    """
    这种编码通过命令行 file -i 文件名获取编码格式，
    通过测试，使用file 命令获取的编码格式不能获取正确的编码数据
    :param path:
    :return:
    """
    with open(path, "r", encoding="iso-8859-1") as f:
        i = 0
        for line in f:
            print(line)
            i += 1
            if i == 5:
                break
    f.close()
def print_data_2(path):
    print("-------------------------------")
    with open(path, "r", encoding="{0}".format(detectCode(path))) as f:
        i = 0
        for line in f:
            b_line = line.encode("utf-8")  # 将文件内容转化为utf-8格式
            print(chardet.detect(b_line)['encoding'])  # 输出转化为内容格式
            i += 1
            if i == 5:
                break
    f.close()
if __name__ == '__main__':
    path = "test.txt"
    print(detectCode(path))
    # print_data_1(path)
    print_data_2(path)
