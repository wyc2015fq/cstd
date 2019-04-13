
# Python判断文件和字符串编码类型的实例 - jiahaowanhao的博客 - CSDN博客


2018年02月18日 21:11:06[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：554


[Python判断文件和字符串编码类型的实例](http://cda.pinggu.org/view/24772.html)
python判断文件和字符串编码类型可以用chardet工具包，可以识别大多数的编码类型。但是前几天在读取一个Windows记事本保存的txt文件时，GBK却被识别成了KOI8-R，无解。
然后就自己写了个简单的编码识别方法，代码如下：
coding.py
\# 说明：UTF兼容ISO8859-1和ASCII，GB18030兼容GBK，GBK兼容GB2312，GB2312兼容ASCII
CODES = ['UTF-8', 'UTF-16', 'GB18030', 'BIG5']
\# UTF-8 BOM前缀字节
UTF_8_BOM = b'\xef\xbb\xbf'
\# 获取文件编码类型
def file_encoding(file_path):
"""
获取文件编码类型\n
:param file_path: 文件路径\n
:return: \n
"""
with open(file_path, 'rb') as f:
return string_encoding(f.read())
\# 获取字符编码类型
def string_encoding(b: bytes):
"""
获取字符编码类型\n
:param b: 字节数据\n
:return: \n
"""
\# 遍历编码类型
for code in CODES:
try:
b.decode(encoding=code)
if 'UTF-8' == code and b.startswith(UTF_8_BOM):
return 'UTF-8-SIG'
return code
except Exception:
continue
return '未知的字符编码类型'
说明：file_encoding方法用于判断文件编码类型，参数为文件路径；string_encoding方法用于判断字符串编码类型，参数为字符串对应的字节数据
使用示例：
import coding
file_name = input('请输入待识别文件路径：\n')
encoding = coding.file_encoding(file_name)
print(encoding)
以上这篇Python判断文件和字符串编码类型的实例就是小编分享给大家的全部内容了

