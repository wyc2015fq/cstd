# pytreebank︱情感分析可视化——情感结构树 - 素质云笔记/Recorder... - CSDN博客





2017年11月01日 18:39:06[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1498








![这里写图片描述](https://img-blog.csdn.net/20171101181427267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

效果如上，一句话用树状来表达，具体案例可查询[stanford Sentiment Analysis网站](https://nlp.stanford.edu/sentiment/treebank.html)

如何用py来画，主要参考自[pytreebank-github](https://github.com/JonathanRaiman/pytreebank)

### 1  直接上案例，官网案例数据下载:

[http://nlp.stanford.edu/sentiment/trainDevTestTrees_PTB.zip](http://nlp.stanford.edu/sentiment/trainDevTestTrees_PTB.zip)

```
import pytreebank
dataset = pytreebank.load_sst("/path/to/sentiment/")
pytreebank.LabeledTree.inject_visualization_javascript()
# select and example to visualize
example = dataset["train"][0]
# display it in the page
example.display()
```

load_sst时候需要写入官网下载数据的链接具体在哪。 
![这里写图片描述](https://img-blog.csdn.net/20171101182210060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**可能报错：**
`UnicodeDecodeError: 'gbk' codec can't decode byte 0x92 in position 24783: illegal multibyte sequence `
所以一位读者给出的建议为：不得不修改了下源码，把报错的代码地方加了个encoding=’utf-8’编码。
` import_tag( "script", contents=format_replacements(open(scriptname,encoding='utf-8').read(), replacements), type="text/javascript" )`
### 2 那么案例里面的数据长什么样子：

```
for label, sentence in example.to_labeled_lines():
    print("%s has sentiment label %s" % (
        sentence,
        ["very negative", "negative", "neutral", "positive", "very positive"][label]))
```

具体如下：

```
The Rock is destined to be the 21st Century 's new `` Conan '' and that he 's going to make a splash even greater than Arnold Schwarzenegger , Jean-Claud Van Damme or Steven Segal . has sentiment label positive
The Rock has sentiment label neutral
The has sentiment label neutral
Rock has sentiment label neutral
is destined to be the 21st Century 's new `` Conan '' and that he 's going to make a splash even greater than Arnold Schwarzenegger , Jean-Claud Van Damme or Steven Segal . has sentiment label very positive
is destined to be the 21st Century 's new `` Conan '' and that he 's going to make a splash even greater than Arnold Schwarzenegger , Jean-Claud Van Damme or Steven Segal has sentiment label positive
is has sentiment label neutral
destined to be the 21st Century 's new `` Conan '' and that he 's going to make a splash even greater than Arnold Schwarzenegger , Jean-Claud Van Damme or Steven Segal has sentiment label very positive
destined has sentiment label neutral
to be the 21st Century 's new `` Conan '' and that he 's going to make a splash even greater than Arnold Schwarzenegger , Jean-Claud Van Damme or Steven Segal has sentiment label neutral
to be the 21st Century 's new `` Conan '' and has sentiment label neutral
to be the 21st Century 's new `` Conan '' has sentiment label neutral
to be the 21st Century 's new `` Conan has sentiment label neutral
to has sentiment label neutral
```

### 3 那么自己咋画呢：

```bash
pytreebank.LabeledTree.inject_visualization_javascript()

pytreebank.create_tree_from_string('(4 (0 你) (3 (2 是) (3 (3 (3 谁) (2 的)) (2 谁))))').display()
```

4代表总共有四个节点， 

(0 你)代表，‘你’这个字的情感点； 

编号的意义为： 

0–>”very negative”, 

1–> “negative” 

2–>, “neutral” 

3–>, “positive” 

4–>, “very positive”
![这里写图片描述](https://img-blog.csdn.net/20171101182424020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 4 其他

```bash
pytreebank.create_tree_from_string('(4 (0 你) (3 (2 是) (3 (3 (3 谁) (2 的)) (2 谁))))').to_dict()
```

把结果导出成dict形式

```bash
pytreebank.create_tree_from_string('(4 (0 你) (3 (2 是) (3 (3 (3 谁) (2 的)) (2 谁))))').to_json()
```

把结果导出成json形式

```bash
pytreebank.create_tree_from_string('(4 (0 你) (3 (2 是) (3 (3 (3 谁) (2 的)) (2 谁))))').to_lines()
```

把结果导出成一列文字形式

```bash
pytreebank.create_tree_from_string('(4 (0 你) (3 (2 是) (3 (3 (3 谁) (2 的)) (2 谁))))').to_lines()
```

把结果导出成to_labeled_lines形式





