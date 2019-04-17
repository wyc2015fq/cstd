# python   hashlib密码加密 - 墨流觞的博客 - CSDN博客





2018年09月21日 18:54:08[墨氲](https://me.csdn.net/dss_dssssd)阅读数：188







- hashlib.md5(data)函数中，data参数的类型应该是bytes。

hash前必须把数据转换成bytes类型hash前必须把数据转换成bytes类型
```python
```python
from hashlib import md5
 c = md5("helloworld")
# TypeError: Unicode-objects must be encoded before hashing
 c = md5("helloworld".encode("utf-8"))
```
```

- 函数说明
- hashlib.md5(data)
- digest() :返回加密后字符串的16进制表示，其位数为digest_size
- digest_size

- 程序实例
```python
```python
import numpy as np
import hashlib

a = hashlib.md5("64".encode("utf-8"))
print(a)
print(a.digest())
print(a.digest_size)
print(a.digest()[-1])
b = hashlib.md5("64".encode("utf-8"))
print(a.digest() == b.digest())
```
```


程序输出：

> 
first output:

<md5 HASH object @ 0x000001F196B02418>

b’\xea]/\x1cF\x08#.\x07\xd3\xaa=\x99\x8eQ5’

16

53

True
second output:

<md5 HASH object @ 0x0000016759262418>

b’\xea]/\x1cF\x08#.\x07\xd3\xaa=\x99\x8eQ5’

16

53

True
注意：
- 两次运行程序输出对象a虽然不同，但digest()输出完全相同
- a和b的digest()完全相同，
- 
利用上述两点可以做成简单的密码加密系统，增加密码的安全性

```python
```python
# table存储着{nickname: md5_password},昵称和密码的md5的加密值
    # 检查账号
    def in_database(name, passw):
    	if hashlib.md5(passw.encode("utf-8")).digest() == table[name].digest():
    		print('账户在系统中')
    		# 进入程序
    	else:
    		print('账号和密码不一致')
```
```

- 
此程序中利用此haslib库来确定某一项是否要进入测试集中

hash值的最后一下项小于256*test_ratio，则放入测试集中， date为pandas对象

```python
```python
import hashlib
	def test_set_check(identifier, test_ratio, hash):
        return hash(np.int64(identifier)).digest()[-1] < 256 * test_ratio
	def split_train_test_by_id(data, test_ratio, id_column, hash=hashlib.md5):
		ids = data[id_column]
		in_test_set = ids.apply(lambda id_: test_set_check(id_, test_ratio, hash))
		return data.loc[~in_test_set], data.loc[in_test_set]
```
```







