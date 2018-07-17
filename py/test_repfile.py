import hashlib

md5=hashlib.md5('字符串'.encode('utf-8')).hexdigest()

print(md5)

#求文件md5

import hashlib
import file

#文件位置中的路径，请用双反斜杠，

# 如’D:\\abc\\www\\b.msi’ file='[文件位置]’

file = 'test_bayes.py'
md5file=open(file,'rb')
md5=hashlib.md5(md5file.read()).hexdigest()
print(type(md5))
md5file.close()

print(md5)