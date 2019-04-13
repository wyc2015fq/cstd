
# python读取txt， jieba分词并统计词频后输出结果到Excel和txt文档 - leofionn的博客 - CSDN博客


2018年05月22日 20:47:07[leofionn](https://me.csdn.net/qq_36142114)阅读数：1986


|1
|2
|3
|4
|5
|with|open|(|'1.txt'|,|'r'|) as fp:
|lines|=|fp.readlines()
|res|=|[line.split()[|1|][|9|:]|for|line|in|lines]
|with|open|(|'out.txt'|,|'w+'|) as out:
|out.write(|'\n'|.join(res))
|
\#encoding=utf-8
import jieba
import jieba.posseg as pseg
import re
filename='result.txt'
fileneedCut='test.txt'
fn=open(fileneedCut,"r")
f=open(filename,"w+")
for line in fn.readlines():
words=pseg.cut(line)
for w in words:
print >>f,str(w)
f.close()
fn.close()

importsys
reload(sys)
sys.setdefaultencoding('utf-8')
importjieba
importjieba.analyse
importxlwt\#写入Excel表的库
if__name__=="__main__":
wbk = xlwt.Workbook(encoding ='ascii')
sheet = wbk.add_sheet("wordCount")\#Excel单元格名字
word_lst = []
key_list=[]
forlineinopen('1.txt'):\#1.txt是需要分词统计的文档
item = line.strip('\n\r').split('\t')\#制表格切分
\# print item
tags = jieba.analyse.extract_tags(item[0])\#jieba分词
fortintags:
word_lst.append(t)
word_dict= {}
with open("wordCount.txt",'w') as wf2:\#打开文件
foriteminword_lst:
ifitemnotinword_dict:\#统计数量
word_dict[item] =1
else:
word_dict[item] +=1
orderList=list(word_dict.values())
orderList.sort(reverse=True)
\# print orderList
foriinrange(len(orderList)):
forkeyinword_dict:
ifword_dict[key]==orderList[i]:
wf2.write(key+' '+str(word_dict[key])+'\n')\#写入txt文档
key_list.append(key)
word_dict[key]=0

foriinrange(len(key_list)):
sheet.write(i,1, label = orderList[i])
sheet.write(i,0, label = key_list[i])
wbk.save('wordCount.xls')\#保存为 wordCount.xls文件

1.txt是你需要分词统计的文本内容，最后会生成wordCount.txt和wordCount.xls两个文件。下图是最后结果
![](https://img-blog.csdn.net/20170614111307333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDA3MDA4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




