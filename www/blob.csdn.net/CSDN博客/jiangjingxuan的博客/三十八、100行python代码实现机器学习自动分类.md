# 三十八、100行python代码实现机器学习自动分类 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:56:18[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：5150












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_141_default_big.jpeg)



现在朋友圈、公众号、微博信息应接不暇，以微信公众号举例，看技术极客是怎么自动筛出自己想看的文章的，100行机器学习代码就能自动帮你归好类，要想找出想看的和不想看的，你再也不用刷朋友圈了

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 准备工作

1. 准备一张mysql数据库表，至少包含这些列：id、title(文章标题)、content(文章内容)、segment(中文切词)、isTec(技术类)、isSoup(鸡汤类)、isMR(机器学习类)、isMath(数学类)、isNews(新闻类)

2. 根据你关注的微信公众号，把更新的文章的title和content自动写入数据库中，具体方法见《[教你成为全栈工程师(Full Stack Developer) 三十一-利用微信搜索抓取公众号文章](http://www.shareditor.com/blogshow/?blogId=44)》

3. 收集一部分文章做人工标注，按照你自己的判断找几百篇文章标记出属于那种类别



## 100行python代码

代码如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
import jieba
from jieba import analyse
import MySQLdb
import numpy as np
from sklearn import metrics
from sklearn.svm import SVC

conn = MySQLdb.connect(host="127.0.0.1",user="myuser",passwd="mypasswd",db="mydatabase",charset="utf8")

def get_segment(all=False):
    cursor = conn.cursor()

    if True == all:
        # 找到全部文章的标题和内容
        sql = "select id, title, content from CrawlPage"
    else:
        # 找到尚未切词的文章的标题和内容
        sql = "select id, title, content from CrawlPage where segment is null"
    cursor.execute(sql)

    for row in cursor.fetchall():
        print "cutting %s" % row[1]
        # 对标题和内容切词
        seg_list = jieba.cut(row[1]+' '+row[2])
        line = ""
        for str in seg_list:
            line = line + " " + str
        line = line.replace('\'', ' ')

        # 把切词按空格分隔并去特殊字符后重新写到数据库的segment字段里
        sql = "update CrawlPage set segment='%s' where id=%d" % (line, row[0])
        try:
            cursor.execute(sql)
            conn.commit()
        except Exception,e:
            print line, e
            sys.exit(-1)

def classify():
    cursor = conn.cursor()

    # 一共分成5类，并且类别的标识定为0，1，3，4，5
    category_ids = range(0, 5)
    category={}
    category[0] = 'isTec'
    category[1] = 'isSoup'
    category[2] = 'isMR'
    category[3] = 'isMath'
    category[4] = 'isNews'

    corpus = []
    for category_id in category_ids:
        # 找到这一类的所有已分类的文章并把所有切词拼成一行，加到语料库中
        sql = "select segment from CrawlPage where " + category[category_id] + "=1"
        cursor.execute(sql)
        line = ""
        for result in cursor.fetchall():
            segment = result[0]
            line = line + " " + segment
        corpus.append(line)

    # 把所有未分类的文章追加到语料库末尾行
    sql = "select id, title, segment from CrawlPage where isTec=0 and isSoup=0 and isMR=0 and isMath=0 and isNews=0"
    cursor.execute(sql)
    line = ""
    update_ids = []
    update_titles = []
    need_predict = False
    for result in cursor.fetchall():
        id = result[0]
        update_ids.append(id)
        title = result[1]
        update_titles.append(title)
        segment = result[2]
        corpus.append(segment)
        need_predict = True

    if False == need_predict:
        return

    # 计算tf-idf
    vectorizer=CountVectorizer()
    csr_mat = vectorizer.fit_transform(corpus)
    transformer=TfidfTransformer()
    tfidf=transformer.fit_transform(csr_mat)
    y = np.array(category_ids)

    # 用前5行已标分类的数据做模型训练
    model = SVC()
    model.fit(tfidf[0:5], y)

    # 对5行以后未标注分类的数据做分类预测
    predicted = model.predict(tfidf[5:])

    # 把机器学习得出的分类信息写到数据库
    for index in range(0, len(update_ids)):
        update_id = update_ids[index]
        predicted_category = category[predicted[index]]
        print "predict title: %s <==============> category: %s" % (update_titles[index], predicted_category)
        sql = "update CrawlPage set %s=1 where id=%d" % (predicted_category, update_id)
        cursor.execute(sql)

    conn.commit()

if __name__ == '__main__':
    # 切词
    get_segment()
    # 分类
    classify()
    conn.close()
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 原理说明

python代码利用了scikit-learn机器学习库的svm（支持向量机）算法，先计算出tf-idf矩阵，这个矩阵包括已经标注好分类的前5行，和未进行分类5行以后的行，以前5行为样本交给svm做训练，然后用训练出来的模型对5行以后的部分做预测，并写回数据库

## 代码说明

get_segment()利用了jieba中文分词器对文章内容做分词，并保存到数据库的segment列中，如果已经做过分词，则不再重复做

classify()分五部分：

第一部分先取出同一类文章的segment数据拼成一行作为特征向量

第二部分取出未分类的文章的segment数据追加到后面，每篇文章一行

第三部分以第一部分为输入做SVM模型训练

第四部分用训练好的SVM模型对第二部分的行做预测

第五部分把预测出来的分类信息写到数据库中

## 效果

我人工标注了600多篇文章，然后对未标注的测试样本执行上面代码做预测，准确率在90%以上，以后就再也不用“刷”公众号文章了，我感兴趣的是机器学习和数学部分，不喜欢看的就是鸡汤文，以后就只看机器学习和数学类别的文章就行喽！看看我刚才随便抓了几篇文章的自动分类效果吧

```xml
predict title: 刷屏朋友圈的北京彩虹图，你看到的是它真实的颜色吗？ <==============> category: isNews
predict title: 玩儿游戏也能学编程？12个学习编程的游戏化平台 <==============> category: isNews
predict title: 互联网教父凯文·凯利：大数据时代没有旁观者 <==============> category: isNews
predict title: 对话周涛：抓住大数据最性感的方向 <==============> category: isNews
predict title: 视频 | MIT研发了一款可食用机器人，能移除胃部异物 <==============> category: isNews
predict title: 京津冀大数据峰会八月亮相北京大数据展 <==============> category: isNews
predict title: 世界这么大，一起去看看（大数据观察） <==============> category: isNews
predict title: 京筑合作共建大数据战略重点实验室 <==============> category: isNews
predict title: 细数IT巨头们那些年十大悔断肠的错误决定 <==============> category: isNews
predict title: 从BAT看企业构建大数据体系的六层级 <==============> category: isNews
predict title: 关于数学的恐怖故事：从前有棵树,叫高数,树上挂了很多人 <==============> category: isMath
predict title: 最前沿的数字内容大会上，他们这么创作 VR 内容 <==============> category: isNews
predict title: TED|数学告诉你 完美伴侣如何选择 <==============> category: isNews
predict title: 德鲁克：自我管理的七个维度 <==============> category: isSoup
predict title: 福利|听课免费,还送礼品:亚马逊云计算AWS在线技术干货52小时 <==============> category: isNews
predict title: 【重磅长文】技术和科学是不同的源流，一万字读懂人类技术史 <==============> category: isNews
predict title: Slideshare上火爆的PPT | 互联网世界的60秒 <==============> category: isNews
predict title: 【数据新闻周报】小米谷歌共同研发VR设备，阿里旅行发布未来酒店2.0战略 <==============> category: isNews
predict title: 手把手：使用OpenCV进行面部合成— C++ / Python <==============> category: isTec
predict title: “云南省经济社会大数据研究院”揭牌仪式正式举行 <==============> category: isNews
predict title: [50页报告下载]《中英开放数据发展和合作》报告新鲜发布 <==============> category: isNews
```






