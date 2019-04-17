# python︱flashtext高效字符串查找与替换 - 素质云笔记/Recorder... - CSDN博客





2017年11月13日 16:41:33[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1541








项目链接：[https://github.com/vi3k6i5/flashtext](https://github.com/vi3k6i5/flashtext)

原文：[资源 | 十五分钟完成Regex五天任务：FastText，语料库数据快速清理利器](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650733038&idx=3&sn=e1a21cc0b038c1fc4512a6848ccaa4e2&chksm=871b3d90b06cb48614f6020c68da6483a7a1f08481d3e28fadff3b60cebef4b60c84474f429a&mpshare=1&scene=1&srcid=1110sMpSiurMd27FFtNbZ3lH#rd)

.
## 性能：

![这里写图片描述](https://camo.githubusercontent.com/53e63b19336a7dfbe5d2874b70d73e37c4cd744d/68747470733a2f2f74686570726163746963616c6465762e73332e616d617a6f6e6177732e636f6d2f692f7872756635306e367a31723337746938726438392e706e67)

![这里写图片描述](https://camo.githubusercontent.com/28e8b327359b6f93bf3ac4733b92c5dec0576851/68747470733a2f2f74686570726163746963616c6465762e73332e616d617a6f6e6177732e636f6d2f692f6b343467687770386f373132646d35386465626a2e706e67)

# 案例：字符串查找

英文：

```
# pip install flashtext
from flashtext.keyword import KeywordProcessor
keyword_processor = KeywordProcessor()
keyword_processor.add_keyword('Big Apple', 'New York')
keyword_processor.add_keyword('Bay Area')
keywords_found = keyword_processor.extract_keywords('I love Big Apple and Bay Area.')
keywords_found
# ['New York', 'Bay Area']
```

中文：

```python
from flashtext import KeywordProcessor
keyword_processor = KeywordProcessor()
keyword_processor.add_keyword('你好', '您好')  # 前面一个单词为住   后面一个单词为替换 
keyword_processor.add_keyword('不要')
keywords_found = keyword_processor.extract_keywords('你好，请不要随便践踏草坪。')  #显示的单词为替换之后的
keywords_found
```

最后显示：
`['您好', '不要']`
add_keyword（查找字符，替换字符），也就是先找到句子中的’你好’，然后显示出来的是add_keyword的替换字符 

.

# 案例：字符串替换

英文：

```
from flashtext.keyword import KeywordProcessor
keyword_processor = KeywordProcessor()
keyword_processor.add_keyword('Big Apple', 'New York')
keyword_processor.add_keyword('New Delhi', 'NCR region')
new_sentence = keyword_processor.replace_keywords('I love Big Apple and new delhi.')
new_sentence
# 'I love New York and NCR region.'
```

中文：

```python
from flashtext import KeywordProcessor
keyword_processor = KeywordProcessor()
keyword_processor.add_keyword('你好', '您好')  # 前面一个单词为住   后面一个单词为替换 
new_sentence = keyword_processor.replace_keywords('你好，请不要随便践踏草坪。')
new_sentence
```

最后显示：

```bash
'您好，请不要随便践踏草坪。'
```

其他功能觉得在中文场景不太好用。。 




