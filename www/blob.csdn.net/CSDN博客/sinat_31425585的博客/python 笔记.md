# python 笔记 - sinat_31425585的博客 - CSDN博客
2019年02月24日 20:54:03[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：31
个人分类：[python](https://blog.csdn.net/sinat_31425585/article/category/6402119)
**1、字符串编码**
ASCII码：美国人发明了计算机，最早只有127个字符（字母、数字和一些符号）被编码到计算机中，使用一个8bite的字节（byte）就足够了，这就是最早的ASCII码；
Unicode：对于不同语言，如汉语、韩语、日语，显然使用一个byte是不足以完成编码的，因此，为了区别于ASCII码，且能够完成对各自语言的编码，各个国家制定了不同的编码方式，如中国的GB2312， 韩国的EUC-kr，日本的Shift_JIS，但是问题又出现了，各国标准不同，必然会出现编码冲突，在多语言混合的文本中，就会出现乱码，这个时候，Unicode应运而生，将所有的语言采用一套统一的标准进行编码；
UTF-8：Unicode编码方式虽然解决了乱码的问题，但是会出现存储空间上的浪费，例如，如果文本中只有英文字母，使用Unicode编码方式比ASCII码的编码方式要多占用一倍内存，本着节约的精神，UTF-8（可变长编码）编码方式就出现了，UTF-8将一个Unicode字符根据不同数字大小编码成1-6个字节，这样就会大大的节省内存空间；
**2、常见数据类型**
1）list
list是一种有序集合，可随时添加和删除其中元素，可用len方法获取元素个数，插入元素方法有append(), insert()，删除元素方法有pop()；
2) tuple
tuple是一种有序元组，和list类似，但是tuple一旦初始化了，就不可以修改，这里不可以修改指的是元素指向不可以修改，当元素为列表时，列表中的元素是可以进行修改的；
3) dict
dict是一种键值对形式存储的数据格式，在其他语言中也叫map，具有较快的查找速度，可以通过对键的索引获取值，但是当dict中不存在当前键时，会报错，因此在采用键对值进行索引的方式之前，需要使用in的方式进行检查，也可以使用get的方法进行获取，当不存在时，返回为None，同时，删除一个键可以使用pop的方式；
4) set
set是一组key的集合，但是不存在值，所以在set中不存在重复的值，初始化的方式可以采用一个列表进行初始化，同时，向set中添加元素可以采用add的方法，删除键的方式可以采用remove的方法；
**3、常见条件判断**
1) if 语句
```python
if ...:
    ...
elif ...:
    ...
else:
```
2) while 语句
```python
while ...:
    ...
```
3) for 循环语句
```python
for ... in ...:
    ...
```
4、定义函数
1） 空函数，当没有想好实现内容时，使用pass占位
```python
def get(x):
    pass
```
2）函数一般形式：函数名、参数、函数体、返还值
```python
def Add(x, y):
    sum = x + y
    return sum
```
3) 函数参数形式：必选参数、默认参数、可变参数、关键字参数和命名关键字参数
a) 必选参数：调用函数必须填写的参数，不填写就会报错；
b) 默认参数：参数本身就被赋了默认值，不填写时，函数会默认改参数为默认值，不会报错；
c) 可变参数：传入参数是可变的，使用方法就是在参数前面加一个*号，如*argc，这样参数就可以list或tuple的形式传进来；
d) 关键字参数：传入参数可以是0个或任意个含参数名的参数，传入参数会自动封装成一个dict形式，关键字参数定义为**kw;
e) 命名关键字参数：对传入关键字参数进行限制；
最后举例看一下包含上述四种参数的函数长什么样：
```python
def person(name, age, **kw):
    if 'city' in kw:
        pass
    if 'job' in kw:
        pass
    print('name:', name, 'age:', age, 'other:', kw)
```
**5、IO操作**
**5.1 文件操作**
现代操作系统不允许普通程序直接操作磁盘， 所以读写文件就是请求操作系统打开一个文件对象，然后通过操作系统提供接口对这个文件对象进行读写操作
打开一个文件，可以用python内置的接口open()函数，传入文件名和标识符：
```python
# 后面标识符：'r'：可以对文件进行读操作， 'w'：可以对文件进行写操作，'b'：二进制格式
f = open(file_name, 'rb')
```
可以采用read()一次性读取文件中所有内容，返还一个字符串；也可以用readlines()，一次读取所有行，并按行返回一个list；还可以用readline()，一次读取一行，使用循环的方式读取所有内容， 需要注意：读写操作完毕后，需要调用close()来关闭，也可以用with关键字，出了with范围，文件可以自动关闭：
```python
with open('./data/result.txt', 'r') as f:
    for line in f.readlines():
        print(line.strip())    # 删除末尾的'\n'
```
写文件的过程与读文件类似，写的过程调用的是write方法，如：
```python
with open(file_name, 'wb') as f:
    f.write("心情很美丽！")
```
当从内存中进行文件读写操作，可以使用StringIO操作字符串数据，使用BytesIO操作二进制数据
```python
from io import StringIO
f = StringIO("hello world!")
while Ture:
    s = f.readline()
    if(s == ''):
        break;
    print(s.strip())
f.close()
```
```python
from io import BytesIO
f = BytesIO()
f.write("中文".encode('utf-8'))
print(f.getvalue())  # 使用getvalue获取写入的值
f.close()
```
**5.2 路径操作**
  获取系统信息：系统名、操作系统详细信息、系统环境变量及某一个环境变量的值：
```python
import os
print(os.name) # 打印操作系统类型
print(os.uname()) # 打印详细的系统信息
print(os.environ) # 打印操作系统定义的环境变量
print(os.environ.get('PATH')) # 获取'PATH'环境变量的值
```
  查看、创建、删除目录和分离路径：
```python
import os
print(os.path.abspath('.')) # 查看当前目录的绝对路径
new_dir = os.path.join('.'， 'test_dir')  # 构造要创建路径的路径名
os.mkdir(new_dir) # 创建路径
os.rmdir(new_dir) # 删除路径
os.split(new_dir) 
# 如果路径为 /home/mirror/test.txt，使用分离路径后，会返还两个部分：
# '/home/mirror', 'test.txt'
```
对文件进行重命名、删除操作：
```python
import os
os.rename('test.txt', 'new.py')
os.remove('new.py')
```
文件过滤
```python
import os
# 获取文件夹
dirs = [x for x in os.listdir('.') if os.path.isdir(x)]
# 获取文件
files = [x for x in os.listdir('.') if os.path.isfile(x) and os.path.splitext(x)[1] == '.py']
```
**6、序列化**
使用pickle模块实现序列化——将变量从内存中变成可存储或传输的过程，使用pickle的dumps方法对数据进行序列化：
```python
import pickle
d = dict(name='blob', age=20, score=88)
pickle.dumps(d) # 将任意对象序列化成一个bytes
```
将序列化之后的bytes写入到文件中，使用pickle的dump方法：
```python
import pickle
d= dict(name='blob')
with open('dump.txt', 'wb') as f:
    pickle.dump(d, f)  # 将数据写入文件中
```
将对象从磁盘中读取出来，使用pickle的load方法：
```python
with open('dump.txt', 'rb') as f:
    d = pickle.load(f)
```
**参考资料：**
[廖雪峰老师的python教程： https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)
