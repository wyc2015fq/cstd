# python 3以上版本使用pickle.load读取文件报UnicodeDecodeError: 'gbk' codec can't decode byte 0xae in position 10 - ChihkAnchor的博客 - CSDN博客





2018年12月10日 17:27:47[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：119








工程代码之前使用Python2进行序列化，现，在Python3环境下反序列化报错：

UnicodeDecodeError: 'gbk' codec can't decode byte 0xae in position 10

原因是编码格式不一致导致的，这时需要指定反序列化的编码方式，

dict = pickle.load(open(r"./a.pkl","rb"),encoding='latin1')
其中，encoding的编码方式不限定于latin1，也可以是'iso-8859-1' ，'gbk' ,  'utf8' ，'bytes' 等



