# （转）Python 用hashlib求中文字符串的MD5值 - 去向前方的博客 - CSDN博客





2018年08月28日 15:35:07[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：244








使用过hashlib库的朋友想必都遇到过以下的错误吧：“Unicode-objects must be encoded before hashing”，意思是在进行md5哈希运算前，需要对数据进行编码。而且在不同版本的Python下还有所不同，唉Python还需努力啊，接口和消息都很不稳定。

hashlib.md5(data)函数中，data参数的类型应该是bytes。也就是说我们在进行hash前必须把数据转换成bytes类型，对于C程序而言似乎没有类似问题，指针强制转换就OK了。

对于中文，有朋友建议转为utf8，哭同一中文字符串在gb2312和utf8下的值显然是不一样的（连长度都不一样），hash出来的md5会一样吗？ 

下面是正确的示例： 

data = “你好” 

m = hashlib.md5(data.encode(“gb2312”)) 

print(m.hexdigest()) 

如果需要utf8编码下的md5&值，就该是m = hashlib.md5(data.encode(“utf8”))。还可以试试：encode(“mbcs”)、encode(“unicode_escape”)以及encode(“raw_unicode_escape”)
问题解决了，但是心头的疑问却更多了。为什么不能直接hashlib.md5(data)，非要强制进行编码转换，设计者的初衷何在？中文字符在Python中是以什么形式存在？ 

print(‘%x’%ord(data[0])) 

4f60
说明中文字符在Python中是以unicode存在的。至此，所有的疑问都得以解除了。 

在hash前要求进行编码转换，是因为同一个字符串在不同的编码体系下有不同的值，为确保不发生歧义必须要进行一次显性转换。





