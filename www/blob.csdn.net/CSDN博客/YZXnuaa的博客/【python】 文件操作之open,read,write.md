# 【python】 文件操作之open,read,write - YZXnuaa的博客 - CSDN博客
2018年04月10日 18:28:37[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：232
1、open
#open(filepath , 'mode')
file = open(‘D:\test\test.txt’，‘w’)   #存在问题见FAQ1
一般常用模式：r(只读)、w(只写)、a(追加)、b(二进制)
组合：r+(读写)、w+(读写)  #存在问题见FQA2
2、读文件（r）： read()         readline()           readlines()
file = open('D/test/test.txt','r')   #只读模式打开file
all_txt = file.read()  #读全部
one_line_txt = file.readline()   #读一行
all_line_txt = file.readlines()  #读所有行
3、读固定字节文件
file = open('D/test/test.txt','b')   #只读模式打开file
200_byte = file.read(200)
4、写文件（w）
file = open('D/test/test.txt','w')   #只写模式打开file
 input= file.write('11111')
list = ['1111','22222','33333']
input_lines = file.writelines(list)  #写入的参数放入一个列表中，写多行
