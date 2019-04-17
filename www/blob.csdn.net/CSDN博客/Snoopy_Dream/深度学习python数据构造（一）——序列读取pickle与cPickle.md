# 深度学习python数据构造（一）——序列读取pickle与cPickle - Snoopy_Dream - CSDN博客





2018年09月27日 11:34:04[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：188
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









pickle与cPickle；两者的关系：“cPickle – A faster pickle”



**import cPickle as pickle**

**常用函数：pickle.dump(data,handle**,0 or 1 or 2)

                  接受一个文件句柄和一个数据对象作为参数[序列化]  0:txt,1 ,2:老新二进制

**  pickle.load(handle)[**反序列化]



**【先f=open(,"wb")打开一个文件句柄，将数据按照自己想要的格式整理读入】**



```python
def get_all_pics(self,
                     imagepath="/home/lmzwhu/programs/DATAS/HWDB/train/",
                     dict_file="/home/lmzwhu/programs/DATAS/HWDB/train_im_addr_dict"):
       
        '''
        返回指定路径下的所有图片类别（字）ID号和对应的图片路径集合
        {0:[filepath1, filepath2, ...], 1:[...], ...}
        '''
        # 如果保存文件存在，则读取文件，并返回
        if os.path.exists(dict_file):
            with open(dict_file, "rb") as f:
                return pickle.load(f)
        
        # 如果没有保存的文件，则读取图片路径
        codes = os.listdir(imagepath)
        im_addr = {}
        for code in codes:
            path = os.path.join(imagepath, code)
            if not os.path.isdir(path):
                continue
            addresses = glob(os.path.join(path, "*.png"))
            im_addr[int(code)] = addresses

        # 将读取的图片字典信息保存
        with open(dict_file, "wb") as f:
            pickle.dump(im_addr, f)

        return im_addr
```



一、dump： 将python对象序列化保存到本地的文件。【永久保存】比如：将**网上爬下来的数据，保存到本地**

```
>>> import cPickle
>>> data = range(1000)
>>> cPickle.dump(data,open("test\\data.pkl","wb"))
```

        dump函数需要指定两个参数，第一个是需要序列化的python对象名称，第二个是本地的文件，需要注意的是，在这里需要使用open函数打开一个文件，并指定“写”操作。

二、 load：载入本地文件，恢复python对象
`>>> data = cPickle.load(open("test\\data.pkl","rb"))`
        同dump一样，这里需要使用open函数打开本地的一个文件，并指定“读”操作

三、 dumps：将python对象序列化保存到一个**字符串变量**中【临时保存】
`>>> data_string = cPickle.dumps(data)`
四、 loads：从字符串变量中载入python对象
`>>> data = cPickle.loads(data_string)`
例子：

```
# -*- coding: utf-8 -*- 
import pickle
# 也可以这样：
# import cPickle as pickle 
obj = {"a": 1, "b": 2, "c": 3} 
# 将 obj 持久化保存到文件 tmp.txt 中
pickle.dump(obj, open("tmp.txt", "w")) 
# do something else ... 
# 从 tmp.txt 中读取并恢复 obj 对象
obj2 = pickle.load(open("tmp.txt", "r")) 
print obj2
```

例子：**神经网络中模型参数的保存**

```
if best_accuracy<val_accuracy:
    best_accuracy = val_accuracy
    cPickle.dump(model,open("./model.pkl","wb"))
```



```python
import pickle

b=7
i=13000000
fa=99.056
s='中国人民 123abc'

f=open('sample_pickle.dat','wb')    #以写模式打开二进制文件
try:
    pickle.dump(b,f)    #对象个数
    pickle.dump(i,f)    #写入整数
    pickle.dump(fa,f)    #写入字符串
except:
    print('写文件异常')
finally:
    f.close()

'''
使用pickle模块读取'sample_pickle.dat'内的二进制内容
'''
f=open('sample_pickle.dat','rb')
n=pickle.load(f)    #读出文件的数据个数
for i in range(n):
    x=pickle.load(f)
    print(x)
f.close()

'''输出
13000000
99.056
中国人民 123abc

'''
```

将文本文件转化为二进制文件

```python
'''
将文本文件转化为二进制文件
'''
with open('test.txt')as src,open('test_pickle.dat','wb')as dest:
    lines=src.readline()
    pickle.dump(len(lines),dest)
    for line in lines:
        pickle.dump(line,dest)
with open('test_pickle.dat','rb')as fp:
    n=pickle.load(fp)
    for i in range(n):
        print(pickle.load(fp))
```





