# 利用BeautifulSoup和python处理xml - lyx的专栏 - CSDN博客





2015年11月28日 22:02:01[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4457








SUMO的软件有些地方真是不好。很多SUMO中要用到的xml文件之前都是手工完成的。今天用BeautifulSoup模块写了一个代码，解决了最麻烦的排序问题。

问题其实很简单，就是根据xml标签中的一个特定属性进行排序，然后再保存成文件。

譬如下面这个xml文件，我们希望标签的顺序能够根据depart中的数值来排序。在SUMO软件中，这个就是车辆的出发时间。






```python
import bs4#导入BeautifulSoup的包
soup = bs4.BeautifulSoup(open('E:/test.xml'))#读取需要排序的xml文件
soup=soup.body
soup = soup.routes#根据xml文件的结构，做一个简单的处理
 list = []#构建一个List
for child in soup.children:#遍历子节点，并存在List中，用来排序
    print cnt
    list.append(child)
    cnt = cnt+1

kk = list[0]
for i in range (0, len(list)):#删除冗余的节点信息
    if list[i] == kk:
        print i
        list.pop(i)

listBackup = list[:]#复制一份List，怕出什么意外



def fun(x):#define the sort function
    return float(x['depart'])
    
list.sort(key=fun) #sort the list 


    
f=file("E:/sorted2.xml","a+")
for cnt in range(0,len(list)):#write the string to the file
    f.write(str(list[cnt])+'\n') 
f.close    #close the file handler
```
上面的代码能很好实现这样的功能，以后再也不用复制黏贴了~~




