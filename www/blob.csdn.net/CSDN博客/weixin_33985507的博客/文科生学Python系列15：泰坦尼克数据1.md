# 文科生学Python系列15：泰坦尼克数据1 - weixin_33985507的博客 - CSDN博客
2017年09月17日 17:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
# 第八课：案例分析 - 泰坦尼克数据[](https://link.jianshu.com?t=https://render.githubusercontent.com/view/ipynb?commit=1c19fa598b25733e773336768b6cca21a85e2370&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f446174614c6561726e696e675465616d2f507974686f6e446174612f316331396661353938623235373333653737333333363736386236636361323161383565323337302f78696e64612f6c6573736f6e30382f4c6573736f6e385f4361736553747564795f546974616e69632e6970796e62&nwo=DataLearningTeam%2FPythonData&path=xinda%2Flesson08%2FLesson8_CaseStudy_Titanic.ipynb&repository_id=98644486&repository_type=Repository#%E7%AC%AC%E5%85%AB%E8%AF%BE%EF%BC%9A%E6%A1%88%E4%BE%8B%E5%88%86%E6%9E%90---%E6%B3%B0%E5%9D%A6%E5%B0%BC%E5%85%8B%E6%95%B0%E6%8D%AE)
> 
本节课将重点分析泰坦尼克号沉船事故，我们将探索是**什么因素决定了最后是否生还**。
我们将将前面课程所学过的知识点融会贯通，举一反三
新增知识点：
       缺失值处理：pandas中的fillna()方法
       数据透视表：pandas中的pivot_table函数
#### **数据分析的流程**
> 
**问题**：数据分析的核心是问题，提出一个有价值的问题就像瞄准了正确的靶心，才能使后续的动作变得有意义。
**数据采集：**有了问题之后就要收集原始的数据。但是数据来源丰富多样，格式不尽相同，所以需要进行下一步——
**数据清洗**：清洗这些格式多样的数据，纠正其中可能的错误。这个步骤比较繁杂，占整个过程的工作量比例较大，却是整个分析流程必不可少的环节。
**数据探索**：这时候一般会用到统计和可视化的方法进行分析，对数据有全面的认识，为后续的进一步分析做铺垫。
**建模分析**：这一步经常用到机器学习／数据挖掘的算法（此课不做介绍）。
**交流结果**：通过报告或者可视化图表的形式，展示结论，并与他人和交流。
**决策**：通过交流进行决策。
整个数据分析的流程就是从问题到决策的过程。但是真实的情况往往不会那么顺利。比如在数据采集／清洗和探索的过程中，就会进行来回倒腾。就像在探索的时候发现数据支撑不够，要到前面一个步骤进行更多的采集和清洗。又像有时候我们只有数据没有具体的问题，通过采集／清洗和探索这些步骤之后，对数据有了更深的认识，这时可以进一步提出更有意义的问题了。
我们第八课的课程里面，也会用这样一个流程，对数据进行处理。
我们今天的问题是：**哪些问题会影响最后的生还概率。**
#### **泰坦尼克号背景介绍**[](https://link.jianshu.com?t=https://render.githubusercontent.com/view/ipynb?commit=1c19fa598b25733e773336768b6cca21a85e2370&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f446174614c6561726e696e675465616d2f507974686f6e446174612f316331396661353938623235373333653737333333363736386236636361323161383565323337302f78696e64612f6c6573736f6e30382f4c6573736f6e385f4361736553747564795f546974616e69632e6970796e62&nwo=DataLearningTeam%2FPythonData&path=xinda%2Flesson08%2FLesson8_CaseStudy_Titanic.ipynb&repository_id=98644486&repository_type=Repository#%E6%B3%B0%E5%9D%A6%E5%B0%BC%E5%85%8B%E5%8F%B7%E8%83%8C%E6%99%AF%E4%BB%8B%E7%BB%8D)
泰坦尼克号是一艘奥林匹克级邮轮，于1912年4月首航时撞上冰山后沉没。泰坦尼克号由位于北爱尔兰贝尔法斯特的哈兰·沃尔夫船厂兴建，是当时最大的客运轮船，由于其规模相当一艘现代航空母舰，因而号称“上帝也沉没不了的巨型邮轮”。在泰坦尼克号的首航中，从英国南安普敦出发，途经法国瑟堡-奥克特维尔以及爱尔兰昆士敦，计划横渡大西洋前往美国纽约市。但因为人为错误，于1912年4月14日船上时间夜里11点40分撞上冰山；2小时40分钟后，即4月15日凌晨02点20分，船裂成两半后沉入大西洋，死亡人数超越1500人，堪称20世纪最大的海难事件，同时也是最广为人知的海难之一。
![3229514-5ff04755b320327a.jpeg](https://upload-images.jianshu.io/upload_images/3229514-5ff04755b320327a.jpeg)
当时报道
整份数据分为数值变量和分类变量，这边着重分一下，因为它们在后面的使用也是不一样的。因为我们今天案例主题是 什么因素会影响生还，所以这份数据里面“survival” 这一列的数据是需要重点分析的，然后会对分类变量 “Pclass” 舱位和 “Sex” 性别，还有数值变量 “Age” 年龄，对生还情况的影响。
在里面可以看到很多NaN的显示，表示此处是缺失值。第二部分我们将着重来看，怎么来处理这些缺失值。
### 1、数据导入和观察
![3229514-2cb29ac94a4c2f8a.jpeg](https://upload-images.jianshu.io/upload_images/3229514-2cb29ac94a4c2f8a.jpeg)
读取常用包
然后导入这次课程的数据，csv格式的文件。用 Pandas 的 csv 的读取方式来读取，赋值于 titanic_df 这个数据数据变量中。我在前面的文章也不止讲了3遍怎么导入数据了，讨论区的答疑整理几乎每一次都有人问数据导入的问题，所以到今天要是还不会导入数据的，可以翻我前面的文章。
![3229514-5070f4f7aed86938.jpeg](https://upload-images.jianshu.io/upload_images/3229514-5070f4f7aed86938.jpeg)
导入并读取数据
**数据字段说明：**
> 
PassengerId: 乘客的id
Survival: 是否幸存 0 = No, 1 = Yes
Pclass: 舱位 class 1 = 1st, 2 = 2nd, 3 = 3rd
Name: 姓名
Sex: 性别
Age: 年龄
SibSp: 船上兄弟姐妹以及配偶的个数
Parch: 船上父母以及者子女的个数
Ticket: 船票号码
Fare: 票价
Cabin: 船舱号码
Embarked: 登船码头 C = Cherbourg, Q = Queenstown, S = Southampton
**变量类型**
> 
数值变量：Age, SibSp, Parch, 和Fare。
分类变量：Pcalss, Sex, Cabin, 和Embarked。Survived是结果。
下面我们 describe 函数来观察数值变量和分类变量。
### 数据的统计描述[](https://link.jianshu.com?t=https://render.githubusercontent.com/view/ipynb?commit=1c19fa598b25733e773336768b6cca21a85e2370&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f446174614c6561726e696e675465616d2f507974686f6e446174612f316331396661353938623235373333653737333333363736386236636361323161383565323337302f78696e64612f6c6573736f6e30382f4c6573736f6e385f4361736553747564795f546974616e69632e6970796e62&nwo=DataLearningTeam%2FPythonData&path=xinda%2Flesson08%2FLesson8_CaseStudy_Titanic.ipynb&repository_id=98644486&repository_type=Repository#%E6%95%B0%E6%8D%AE%E7%9A%84%E7%BB%9F%E8%AE%A1%E6%8F%8F%E8%BF%B0)
数值变量
![3229514-3a2693a45beeb860.jpeg](https://upload-images.jianshu.io/upload_images/3229514-3a2693a45beeb860.jpeg)
describe函数查看部分变量的分布
用方括号的形式，这四个参数重新组建一个 DataFrame，用describe 进行观察。运行得到四列数据的统计值。
**count**表示的是非空的数值个数。因为一行数据统计出来的结果都是891，只有 Age 是714，所以我们从这份数据可以观察到，Age这列数据是有缺失值的。下面的是一些统计量，**mean** 代表平均值，**std **是标准差，也就是数据相对于均值的波动情况。**min** 是最小值，**max **是最大值，**25%**／**50%**／**75%** 分别是这份数据的四分位数。
通过这份 describe 函数，我们可以对整份数据有大体的认识。比如从年龄那一列看，平均年龄是29.6岁，标准差是14.5，最小的年龄是0.42，最大的80岁，年龄的中位数是28岁。因为 Survived 是0-1变量，所以均值就是幸存人数的百分比，这个用法非常有用，也就是 Survivied 这列数据，由于生还与否是用 0 和 1 来进行表达，所以这里的0.38代表的是生还率。这个看起来很好用啊！
同样的，我们可以用describe来看分类变量的数据。但是要是直接使用，这里显示的也只能是数据，所以用到了 include=[ np.object ] 这个参数，表示把数据里面不是数值的可以用字符串的形式表达出来。
![3229514-7f6fffbe7442fe47.jpeg](https://upload-images.jianshu.io/upload_images/3229514-7f6fffbe7442fe47.jpeg)
使用 include=[np.object] 来查看分类变量
**count **表示非空数值的个数。**unique **表示非重复数的个数，比如 name在这里的非重复数是891，那么和上面的非空数值对比，就知道是没有重名的人在里面。**top **表示最高频的一个词，在性别 sex 中表示的是男性比女性多。**freq** 代表的是上面面最高频的数量，这里告诉我们男性出现次数最多，有577次。所以我们在这个表格可以看到，添加了 include=[ np.object ] 这个参数之后，用 describe 函数不仅能表示数值类的函数，也能表达字符类的函数。
#### 不同舱位的分布情况是怎样的呢？[](https://link.jianshu.com?t=https://render.githubusercontent.com/view/ipynb?commit=1c19fa598b25733e773336768b6cca21a85e2370&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f446174614c6561726e696e675465616d2f507974686f6e446174612f316331396661353938623235373333653737333333363736386236636361323161383565323337302f78696e64612f6c6573736f6e30382f4c6573736f6e385f4361736553747564795f546974616e69632e6970796e62&nwo=DataLearningTeam%2FPythonData&path=xinda%2Flesson08%2FLesson8_CaseStudy_Titanic.ipynb&repository_id=98644486&repository_type=Repository#%E4%B8%8D%E5%90%8C%E8%88%B1%E4%BD%8D%E7%9A%84%E5%88%86%E5%B8%83%E6%83%85%E5%86%B5%E6%98%AF%E6%80%8E%E6%A0%B7%E7%9A%84%E5%91%A2%EF%BC%9F)
*我们通过这个例子来复习 value_counts 和 group_by*
![3229514-c311d273dcfbe634.jpeg](https://upload-images.jianshu.io/upload_images/3229514-c311d273dcfbe634.jpeg)
value_counts频数统计
取到舱位这一批数据，然后用到 value_counts( )，因为它默认是从小到大排列的，所以可以看到三等舱的人最多。
为了求到相应的占比，我们可以除以相应的总量，用 len（ ）函数，然后可以看到三等舱的人数占了55%。
![3229514-a2ee66d837ada446.jpeg](https://upload-images.jianshu.io/upload_images/3229514-a2ee66d837ada446.jpeg)
查看不同舱位的分布
这是用 value_counts 使用的第一种方法，下面还有另一种方法：group_by。
![3229514-00c270c9ecc1072a.jpeg](https://upload-images.jianshu.io/upload_images/3229514-00c270c9ecc1072a.jpeg)
titanic_df.groupby('Pclass').agg('size')
将数据按舱位来分组，分组之后在计算每一组数据的大小。这里用列名 Pclass，代表此处是用舱名来分类的，然后使用聚合函数agg，用的是 size ，是用来计算分组之后的数据大小的。同样我们可以得到每一个舱位的人数。
同样需要得到相应的占比，也是除以 len( tatanic_df )
![3229514-307c1488ec17d798.jpeg](https://upload-images.jianshu.io/upload_images/3229514-307c1488ec17d798.jpeg)
titanic_df.groupby('Pclass').agg('size')/len(titanic_df)
要是想要让数据从大到小进行排序，我们用的是 sort_values 这个方法进行操作:
![3229514-15ba51a837f46a4f.jpeg](https://upload-images.jianshu.io/upload_images/3229514-15ba51a837f46a4f.jpeg)
sort_values 将结果排序
注意哦，这里是先把前面的内容用圆括号扩起来，代表这里的优先级更高，然后再使用 sort_values。后面需要设置参数 =False，因为此处是默认从小到大排列的。
所以这两种方法使用下来，能看出方法1是简单一点的，有默认排序功能，方法2还需要语句来设定。啊，我这种懒人！
#### **作业8-1：**[](https://link.jianshu.com?t=https://render.githubusercontent.com/view/ipynb?commit=1c19fa598b25733e773336768b6cca21a85e2370&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f446174614c6561726e696e675465616d2f507974686f6e446174612f316331396661353938623235373333653737333333363736386236636361323161383565323337302f78696e64612f6c6573736f6e30382f4c6573736f6e385f4361736553747564795f546974616e69632e6970796e62&nwo=DataLearningTeam%2FPythonData&path=xinda%2Flesson08%2FLesson8_CaseStudy_Titanic.ipynb&repository_id=98644486&repository_type=Repository#%E4%BD%9C%E4%B8%9A8-1%EF%BC%9A)
> 
查看Embarked(登船码头)人数分布的百分比
第八课的这篇文章我码到现在，都是在用以前的知识点来进行操作的。下一篇文章将会有新的知识点 “ 缺失值处理：pandas中的fillna()方法 ” 和 “ 数据透视表：pandas中的pivot_table函数 ”
老师在GitHub 预告的，我这两天继续更新！！
