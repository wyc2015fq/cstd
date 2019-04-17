# GitHub出现一个大型中文NLP资源，宣称要放出亿级语料库 - 知乎
# 



> 乾明 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

中文信息很多，但要找到合适的中文语料很难。

有人看不下去了，在GitHub上开了一个项目，专门贡献中文语料资源。

他说，要为解决中文语料难找贡献一份力量。



![](https://pic4.zhimg.com/v2-30c4d6108f0f69e91e6d4d1565c43957_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='392'></svg>)



## **什么样的资源？**

目前，这个项目中一共有3种json版资源：

包含104万个词条的维基百科资源，包含250万篇新闻的新闻语料，以及包含150万个问答的百科类问答资源。



![](https://pic4.zhimg.com/v2-aafe7c1961318e10553d81b452398733_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='847'></svg>)



一般来说，这些资源可以作为通用的中文语料，用于预训练或者构建词向量等等。


不同的资源，用处也有不同，比如维基百科和问答百科，可以用来构建知识问答等等。

新闻语料资源，囊括了标题、关键词、描述和正文，也可以用来训练标题生成模型、关键词生成模型等等。

此外，在对数据集划分过的新闻语料和百科类问答资源中，只提供训练集和验证集，不提供测试集数据的下载。

是因为——

## **希望更多人参与**

资源的贡献者表示，希望大家报告模型在验证集上的准确率，并提供模型信息、方法描述、运行方式，以及可运行的源代码（可选）。

这些信息都有的话，资源贡献者会在测试集上测试模型，并给出准确率。

他表示，项目中的语料库将会不断扩充，号召大家多多贡献资源，并给出了相应的目标：

> 到2019年5月1日，放出10个百万级中文语料&3个千万级中文语料。
到2019年12月31日，放出30个百万级中文语料 & 10个千万级中文语料 & 1个亿级中文语料。

从目前已经有的资源来看，一个语料可以是一个问答，也可以是一个词条等等。

这份资源的贡献者，名为徐亮，杭州实在智能的算法专家，主要关注文本分类、意图识别、问答和面向任务的对话。

如果你有兴趣，请收好资源传送门：

[https://github.com/brightmart/nlp_chinese_corpus](https://link.zhihu.com/?target=https%3A//github.com/brightmart/nlp_chinese_corpus)

此外，量子位之前也介绍过几份中文NLP资源，也一并附于此：

[有人收罗了40个中文NLP词库，放到了GitHub上](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247508058%26idx%3D3%26sn%3D4af173316cf8ec4ad95daf7141c8c29f%26chksm%3De8d01728dfa79e3e786866fc6934849fd77c68d023b4352b9c3a3d2833dec00a7870510713ed%26scene%3D21%23wechat_redirect)

[腾讯AI Lab开源800万中文词的NLP数据集](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247506252%26idx%3D4%26sn%3D1f883532975737c9df02212fc2ae1ca5%26chksm%3De8d06e3edfa7e728ee0f1ae592e03cee5c818a5b731a0f0560672323894a2304758698b52f37%26scene%3D21%23wechat_redirect)

[非正式汉语数据集资源上线，帮你训练网络语言处理](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247509236%26idx%3D4%26sn%3D398c4fe7771fa2445e94ed8146f5bc5a%26chksm%3De8d01386dfa79a906da41ab53671675cf7c61af584d62afc0c7f5ccc9b468b171ebb93e3a2ee%26scene%3D21%23wechat_redirect)

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


