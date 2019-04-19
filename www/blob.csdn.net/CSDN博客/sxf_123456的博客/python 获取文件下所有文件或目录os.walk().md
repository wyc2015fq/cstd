# python 获取文件下所有文件或目录os.walk() - sxf_123456的博客 - CSDN博客
2017年09月05日 21:43:45[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3954
                
在python3.6版本中去掉了os.path.walk()函数
os.walk()
函数声明：walk(top,topdown=True,oneerror=None)
1、参数top表示需要遍历的目录树的路径
2、参数农户topdown默认是"True"，表示首先返回根目录树下的文件，然后，再遍历目录树的子目录。topdown的值为"False",则表示先遍历目录树的子目录，返回子目录下的文件，最后返回根目录下的文件
3、参数oneerror的默认值是"None",表示忽略文件遍历时产生的错误，如果不为空，则提供一个自定义函数提示错误信息，后边遍历抛出异常
4、函数返回一个元组，该元组有3个元素，这3个元素分别表示'每次遍历的路径名，目录列表和文件列表'
os.walk()实例：
import  os
def walk(path):
    if not os.path.exists(path):
        return -1
    for root,dirs,names in os.walk(path):
        for filename in names:
            print(os.path.join(root,filename)) #路径和文件名连接构成完整路径
if __name__=='__main__':
    path = "C:\\Users\\Administrator\\Desktop\\2017-9-1"
    walk(path)
输出结果:
C:\Users\Administrator\Desktop\2017-9-1\2017-9-1.txt
C:\Users\Administrator\Desktop\2017-9-1\2017-9-1storage.txt
C:\Users\Administrator\Desktop\2017-9-1\apk.conf
C:\Users\Administrator\Desktop\2017-9-1\数据采集导入质量统计_2017-09-01.docx
C:\Users\Administrator\Desktop\2017-9-1\test1\2017-9-1.txt
C:\Users\Administrator\Desktop\2017-9-1\test2\2017-9-1.txt
            
