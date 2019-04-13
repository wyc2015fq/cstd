
# 利用python将json数据转换为csv格式的方法 - jiahaowanhao的博客 - CSDN博客


2018年05月30日 21:45:38[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：418


[利用python将json数据转换为csv格式的方法](http://cda.pinggu.org/view/25684.html)
下面小编就为大家分享一篇利用python将json数据转换为csv格式的方法，具有很好的参考价值，希望对大家有所帮助。一起跟随小编过来看看吧
假设.json文件中存储的数据为：
{"type": "Point", "link": "http://www.dianping.com/newhotel/22416995", "coordinates": [116.37256372996957, 40.39798447055443], "category": "经济型", "name": "北京荷塘山庄", "count": "278", "address": "北京市怀柔区黄花城村安四路", "price": "380"}
{"type": "Point", "link": "http://www.dianping.com/newhotel/19717653", "coordinates": [116.56881588256466, 40.43310967948417], "category": "经济型", "name": "慕田峪长城鱼师傅乡村酒店", "count": "89", "address": "北京市怀柔区渤海镇苇店村(慕田峪长城下3公里处，近怀黄路)", "price": "258"}
{"type": "Point", "link": "http://www.dianping.com/newhotel/58365289", "coordinates": [116.62874974822378, 40.45610264855833], "category": "经济型", "name": "北京蜜桃儿亲子客栈", "count": "119", "address": "北京市怀柔区神堂峪风景区下官地11号", "price": "549"}
现在需要将上面的这些数据存为csv格式，其中字典的keys为csv中的属性名称，字典的values为csv中属性对应的值。
如果只需要按照json的keys来生成csv，那么操作比较简单，直接按照下面的方法即可：
\#-*-coding:utf-8-*-
import csv
import json
import sys
import codecs
def trans(path):
jsonData = codecs.open(path+'.json', 'r', 'utf-8')
\# csvfile = open(path+'.csv', 'w') \# 此处这样写会导致写出来的文件会有空行
\# csvfile = open(path+'.csv', 'wb') \# python2下
csvfile = open(path+'.csv', 'w', newline='') \# python3下
writer = csv.writer(csvfile, delimiter='\t')
flag = True
for line in jsonData:
dic = json.loads(line[0:-1])
if flag:
\# 获取属性列表
keys = list(dic.keys())
print (keys)
writer.writerow(keys) \# 将属性列表写入csv中
flag = False
else:
\# 读取json数据的每一行，将values数据一次一行的写入csv中
writer.writerow(list(dic.values()))
jsonData.close()
csvfile.close()
if __name__ == '__main__':
path=str(sys.argv[1]) \# 获取path参数
print (path)
trans(path)
在python3下运行，命令行输入
python C:\Users\MaMQ\Documents\jsonToCsv.py C:\Users\MaMQ\Documents\data\geoFood
其中第三个参数为需要转换的文件的路径和其名称，将其后缀删除。运行文件后即可得到转换后的csv文件。
如果需要对json文件中每个字典的key字段进行修改，比如需要将上面dict中的coordinate中的经纬度数据取出来存为x、y数据，则可以按照下面的方法（此方法还可以调整每个属性显示的顺序，效果更好一点）：
import csv
import json
import sys
import codecs
def trans(path):
jsonData = codecs.open(path+'.json', 'r', 'utf-8')
\# csvfile = open(path+'.csv', 'w') \# 此处这样写会导致写出来的文件会有空行
\# csvfile = open(path+'.csv', 'wb') \# python2下
csvfile = open(path+'.csv', 'w', newline='') \# python3下
writer = csv.writer(csvfile, delimiter='\t')
keys=['id', 'name', 'category', 'price', 'count', 'type', 'address', 'link', 'x', 'y']
writer.writerow(keys)
i = 1
for dic in jsonData:
dic = json.loads(dic[0:-1])
x = dic['coordinates'][0]
y = dic['coordinates'][1]
writer.writerow([str(i),dic['name'],dic['category'],dic['price'],dic['count'],dic['type'],dic['address'],dic['link'],x,y])
i += 1
jsonData.close()
csvfile.close()
if __name__ == '__main__':
path = str(sys.argv[1])
print (path)
trans(path)
运行方法同上。
json文件是我在大众点评抓取的数据，存储格式为utf-8。建议使用codecs包来读取json数据，可指定编码方式。
jsonData = codecs.open(path + '.json', 'r', encoding='utf-8')
以上这篇利用python将json数据转换为csv格式的方法就是小编分享给大家的全部内容了

