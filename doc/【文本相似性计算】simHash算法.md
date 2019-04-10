# 【文本相似性计算】simHash算法

 																				2018年09月25日 13:41:26 					[夜风之南](https://me.csdn.net/weixin_43098787) 						阅读数：168 										

 									

# simHash算法

------

## 原理

simHash算法的主要思想是降维，将高维的特征向量映射成一个f-bit的指纹(fingerprint)，通过比较两篇文章的f-bit指纹的Hamming Distance来确定文章是否重复或者高度近似。

simHash算法很精巧，但却十分容易理解和实现，具体的simHash过程如下：

> 1. 首先将文章转换为一组加权的特征值构成的向量。
> 2. 初始化一个f维的向量V，其中每一个元素初始值为0。
> 3. 对于文章的特征向量集中的每一个特征，做如下计算： 利用传统的hash算法映射到一个f-bit的签名。对于这个f-  bit的签名，如果签名的第i位上为1，则对向量V中第i维加上这个特征的权值，否则对向量的第i维减去该特征的权值。利用传统的hash算法映射到一个f-bit的签名。对于这个f-bit的签名，如果签名的第i位上为1，则对向量V中第i维加上这个特征的权值，否则对向量的第i维减去该特征的权值。
> 4. 对整个特征向量集合迭代上述运算后，根据V中每一维向量的符号来确定生成的f-bit指纹的值，如果V的第i维为正数，则生成f-bit指纹的第i维为1，否则为0。

## 代码

simhash算法有python包实现，可通过命令`pip install simhash`来安装。一个使用示例如下：

```python
from simhash import Simhash
print Simhash('aa').distance(Simhash('bb'))
print Simhash('aa').distance(Simhash('aa'))
123123
```

更多使用方法可参考该包的[官网](https://leons.im/posts/a-python-implementation-of-simhash-algorithm/)。

另外，也在这里展示博客[2]提供的python实现源码。

```python
class simhash:

    #构造函数
    def __init__(self, tokens='', hashbits=128):        
        self.hashbits = hashbits
        self.hash = self.simhash(tokens);

    #toString函数    
    def __str__(self):
        return str(self.hash)

    #生成simhash值    
    def simhash(self, tokens):
        v = [0] * self.hashbits
        for t in [self._string_hash(x) for x in tokens]: #t为token的普通hash值           
            for i in range(self.hashbits):
                bitmask = 1 << i
                if t & bitmask :
                    v[i] += 1 #查看当前bit位是否为1,是的话将该位+1
                else:
                    v[i] -= 1 #否则的话,该位-1
        fingerprint = 0
        for i in range(self.hashbits):
            if v[i] >= 0:
                fingerprint += 1 << i
        return fingerprint #整个文档的fingerprint为最终各个位>=0的和

    #求海明距离
    def hamming_distance(self, other):
        x = (self.hash ^ other.hash) & ((1 << self.hashbits) - 1)
        tot = 0;
        while x :
            tot += 1
            x &= x - 1
        return tot

    #求相似度
    def similarity (self, other):
        a = float(self.hash)
        b = float(other.hash)
        if a > b : return b / a
        else: return a / b

    #针对source生成hash值   (一个可变长度版本的Python的内置散列)
    def _string_hash(self, source):        
        if source == "":
            return 0
        else:
            x = ord(source[0]) << 7
            m = 1000003
            mask = 2 ** self.hashbits - 1
            for c in source:
                x = ((x * m) ^ ord(c)) & mask
            x ^= len(source)
            if x == -1:
                x = -2
            return x
            
if __name__ == '__main__':
    s = 'This is a test string for testing'
    hash1 = simhash(s.split())

    s = 'This is a test string for testing also'
    hash2 = simhash(s.split())

    s = 'honghong huohuo huanghuang huhu'
    hash3 = simhash(s.split())

    print(hash1.hamming_distance(hash2) , "   " , hash1.similarity(hash2))
    print(hash1.hamming_distance(hash3) , "   " , hash1.similarity(hash3))
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697012345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970
```

------

## 参考资料

[1]simhash与Google的网页去重
 <http://leoncom.org/?p=650607>
 [2]python实现simhash算法实例
 <https://www.jb51.net/article/49365.htm>