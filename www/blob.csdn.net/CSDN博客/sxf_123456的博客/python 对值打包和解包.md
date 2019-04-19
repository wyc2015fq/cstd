# python 对值打包和解包 - sxf_123456的博客 - CSDN博客
2019年03月15日 17:06:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：15
reslt = {}
reslt['1'] = [1, 2]
reslt['2'] = [2, 3]
pickle.dump(reslt, open('test.rst', 'wb'))
result = pickle.load(open("test.rst", "rb"))
print(result)
