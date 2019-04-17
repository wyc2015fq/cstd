# python pickle 模块的使用 - lincappu - 博客园







# [python pickle 模块的使用](https://www.cnblogs.com/lincappu/p/8296078.html)





用于序列化的两个模块
　　json：用于字符串和Python数据类型间进行转换
　　pickle: 用于python特有的类型和python的数据类型间进行转换
　　json提供四个功能：dumps,dump,loads,load
　　pickle提供四个功能：dumps,dump,loads,load

### pickle可以存储什么类型的数据呢？
- 所有python支持的原生类型：布尔值，整数，浮点数，复数，字符串，字节，None。
- 由任何原生类型组成的列表，元组，字典和集合。
- 函数，类，类的实例

### **pickle模块中常用的方法有：**

    1. pickle.dump(obj, file, protocol=None,)

    必填参数obj表示将要封装的对象

    必填参数file表示obj要写入的文件对象，file必须以二进制可写模式打开，即“wb”

    可选参数protocol表示告知pickler使用的协议，支持的协议有0,1,2,3，默认的协议是添加在Python 3中的协议3。　　　
- Protocol version 0 is the original “human-readable” protocol and is backwards compatible with earlier versions of Python.
- Protocol version 1 is an old binary format which is also compatible with earlier versions of Python.
- Protocol version 2 was introduced in Python 2.3. It provides much more efficient pickling of [*new-style class*](http://www.cnblogs.com/)es. Refer to [**PEP 307**](http://www.python.org/dev/peps/pep-0307) for information about improvements brought by protocol 2.
- Protocol version 3 was added in Python 3.0. It has explicit support for [bytes](http://www.cnblogs.com/) objects and cannot be unpickled by Python 2.x. This is the default protocol, and the recommended protocol when compatibility with other Python 3 versions is required.
- Protocol version 4 was added in Python 3.4. It adds support for very large objects, pickling more kinds of objects, and some data format optimizations. Refer to [**PEP 3154**](http://www.python.org/dev/peps/pep-3154) for information about improvements brought by protocol 4. 

    2. pickle.load(file,*,fix_imports=True, encoding="ASCII", errors="strict")

    必填参数file必须以二进制可读模式打开，即“rb”，其他都为可选参数

    3. pickle.dumps(obj)：以字节对象形式返回封装的对象，不需要写入文件中

    4. pickle.loads(bytes_object): 从字节对象中读取被封装的对象，并返回

 pickle模块可能出现三种异常：

    1. PickleError：封装和拆封时出现的异常类，继承自Exception

    2. PicklingError: 遇到不可封装的对象时出现的异常，继承自PickleError

    3. UnPicklingError: 拆封对象过程中出现的异常，继承自PickleError

### 应用：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

1 # dumps功能
2 import pickle
3 data = ['aa', 'bb', 'cc']  
4 # dumps 将数据通过特殊的形式转换为只有python语言认识的字符串
5 p_str = pickle.dumps(data)
6 print(p_str)            
7 b'\x80\x03]q\x00(X\x02\x00\x00\x00aaq\x01X\x02\x00\x00\x00bbq\x02X\x02\x00\x00\x00ccq\x03e.

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

```
1 # loads功能
2 # loads  将pickle数据转换为python的数据结构
3 mes = pickle.loads(p_str)
4 print(mes)
5 ['aa', 'bb', 'cc']
```

```
1 # dump功能
2 # dump 将数据通过特殊的形式转换为只有python语言认识的字符串，并写入文件
3 with open('D:/tmp.pk', 'w') as f:
4     pickle.dump(data, f)
```

```
1 # load功能
2 # load 从数据文件中读取数据，并转换为python的数据结构
3 with open('D:/tmp.pk', 'r') as f:
4     data = pickle.load(f)
```












