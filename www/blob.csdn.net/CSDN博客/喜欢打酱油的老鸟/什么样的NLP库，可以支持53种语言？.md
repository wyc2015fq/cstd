
# 什么样的NLP库，可以支持53种语言？ - 喜欢打酱油的老鸟 - CSDN博客


2019年03月04日 08:43:27[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：105


[https://www.toutiao.com/a6663746829693223427/](https://www.toutiao.com/a6663746829693223427/)
在学习自然语言处理（NLP）时，常常会遇到一个挑战——我们可以为非英语语言建立模型吗？很长一段时间以来，这个问题都没有一个确定的答案。每种语言都有自己的语法模式和语言细微的差别，并且其他语言中并没有很多数据集可用。
直到出现了斯坦福大学最新的NLP库——Sanford NLP。作者声称StanfordNLP可以支持超过53种人类语言。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/ed52a5ad9d7e429bbb9742ad12544364)

它打开了一个无限可能的世界——Stanford NLP在其原稿中包含了印度语、汉语和日语等亚洲语言的预训练模型。
所有NLP爱好者都渴望NLP库能够具有兼容多种语言的能力。在本文中，我们将了解StanfordNLP是什么，为什么它如此重要，之后启动Python以使其生效。我们还将用印地语来研究一个案例，以此展示StanfordNLP的工作原理。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**什么是StanfordNLP，**
**为什么要使用它？**
以下是作者自己对StanfordNLP的描述：
> StanfordNLP是斯坦福团队在CoNLL 2018共享任务中使用的通用依赖性分析软件包的组合，以及该组合与Stanford CoreNLP软件的官方Python接口。
这里的信息量很大，让我们分解一下：
· CoNLL是一个关于自然语言学习的年度会议。来自世界各地的研究机构团队的代表试图解决一项基于NLP的任务。
· 去年的任务之一是“从原始文本到通用依赖项的多语言解析”。简单来说，它意味着将多种语言的非结构化文本数据解析为通用依赖项中的有用注释。
· Universal Dependencies（通用依赖项）是一个保持注释一致性的框架。无论要解析的语言如何，都会为文本生成这些注释。
· 斯坦福大学提交的报告在2017年排名第一。由于软件故障（最终排在第4位），他们错失了第一名。
StanfordNLP是一系列经过预训练的最先进模型。这些模型被研究人员用于2017和2018年康奈尔大学的竞赛。所有模型都基于PyTorch来构建，可以在你自己的注释数据上进行训练和评估。听上去是不是很棒？
![什么样的NLP库，可以支持53种语言？](http://p3.pstatp.com/large/pgc-image/9c092c009cd24ef9b26f1bcbf1f1de57)

此外，StanfordNLP还包含流行的巨型NLP库——CoreNLP的官方包装器。到目前为止，这在某种程度上仅限于Java生态系统。所以，本教程可以让你了解更多有关CoreNLP及其在Python中工作原理的信息。
以下是你应该查看此库的其他几个原因：
· Native Python的实现需要设置最少的工作量
· 用于强大文本分析的全神经网络通道，包括：
· 符号化
· 多字令牌（MWT）扩展
· 词形还原
· 词性（POS）和形态特征标记
· 依赖性分析
· 在73个树库中支持53种（人类）语言的预训练神经模型
· 一个稳定的官方维护CoreNLPPython接口
这些正是NLP爱好者所追求的。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**在Python中设置StanfordNLP**
最初，库中的一些奇怪的事情使不少人感到困惑。例如，你需要Python 3.6.8 / 3.7.2或更高版本才能使用StanfordNLP。为了安全起见，可以在Anaconda for Python 3.7.1中设置一个独立的环境。你可以这样做：
1.打开conda提示符并输入以下内容：
conda create-n stanfordnlp python=3.7.1
2.现在，激活软件包：
source activate stanfordnlp
3.安装StanfordNLP库：
pip install stanfordnlp
4.我们需要下载一个语言的特定模型来使用它。启动python shell并导入StanfordNLP：
import stanfordnlp
然后下载英语的语言模型（“en”）：
stanfordnlp.download('en')
这可能需要一段时间，具体取决于你的网络连接情况。这些语言模型所占空间非常庞大（英文版本为1.96GB）。
几项重要的标注
· StanfordNLP建立在PyTorch 1.0.0之上。如果您使用旧版本，它可能会崩溃。以下是检查计算机上安装的版本的方法：
pip freeze |grep torch
其输出应为torch==1.0.0
· 作者曾在联想ThinkpadE470（8GB RAM，Intel Graphics）上尝试使用不带GPU的库，很快Python中就出现了内存错误。因此，建议你切换到另一台支持GPU的机器上。你可以尝试Google Colab，它提供免费的GPU支持。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**使用StanfordNLP执行基本NLP任务**
StanfordNLP内置处理器，可执行五项基本NLP任务：
· 符号化
· 多字令牌扩展
· 词形归并
· 词性标注
· 依赖性分析
让我们从创建文本通道开始：
nlp =stanfordnlp.Pipeline(processors = "tokenize,mwt,lemma,pos")
doc = nlp("""Theprospects for Britain’s orderly withdrawal from the European Union on March 29have receded further, even as MPs rallied to stop a no-deal scenario. Anamendment to the draft bill on the termination of London’s membership of thebloc obliges Prime Minister Theresa May to renegotiate her withdrawal agreementwith Brussels. A Tory backbencher’s proposal calls on the government to come upwith alternatives to the Irish backstop, a central tenet of the deal Britainagreed with the rest of the EU.""")
参数用于指定任务。如果没有传递参数，则默认情况下会采用所有五个处理器。以下是处理器及其功能的简要概述：
![什么样的NLP库，可以支持53种语言？](http://p3.pstatp.com/large/pgc-image/2cb9c255ec6849e8bf92ef38a339d4ea)

让我们分别来看看他们的功能。
符号化
一旦运行令牌处理器，该过程就会隐式发生。它实际上非常快。你可以使用print_tokens（）查看令牌：
doc.sentences[0].print_tokens()

![什么样的NLP库，可以支持53种语言？](http://p9.pstatp.com/large/pgc-image/eeded9d7aa124798b024d4024e940791)

令牌对象包含句子中的令牌索引和单词对象列表（在多字令牌的情况下）。每个单词对象都包含有用的信息，如单词的索引，文本的引理，pos（词性）标签和feat（形态特征）标签。
词形归并
这涉及使用由引理处理器生成的单词的“引理”属性。这是获取所有单词的引理的代码：
import pandas as pd
\#extract_lemma
def extract_lemma(doc):
parsed_text = {'word':[], 'lemma':[]}
for sent in doc.sentences:
for wrd in sent.words:
\#extract text and lemma
parsed_text['word'].append(wrd.text)
parsed_text['lemma'].append(wrd.lemma)
\#return a dataframe
return pd.DataFrame(parsed_text)
\#call the function on doc
extract_lemma(doc)
这将返回每个单词的pandas数据框及其各自的引理：
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/33bfc9c49ab044ccba8eb8e75171ec74)

词性（PoS）标记
PoS标记器速度非常快，并且跨语言都能很好地工作。就像引理一样，PoS标签也很容易提取：
\#dictionary that contains pos tags and their explanations
pos_dict = {
'CC':'coordinating conjunction','CD':'cardinal digit','DT':'determiner',
'EX':'existential there (like: "there is" ... think of it like"there exists")',
'FW':'foreign word','IN': 'preposition/subordinating conjunction','JJ':'adjective 'big'',
'JJR':'adjective, comparative 'bigger'','JJS':'adjective, superlative 'biggest'',
'LS': 'listmarker 1)','MD': 'modalcould, will','NN': 'noun,singular 'desk'',
'NNS': 'nounplural 'desks'','NNP':'proper noun, singular 'Harrison'',
'NNPS':'proper noun, plural 'Americans'','PDT':'predeterminer 'all the kids'',
'POS':'possessive ending parent's','PRP':'personal pronoun I, he, she',
'PRP$':'possessive pronoun my, his, hers','RB': 'adverbvery, silently,',
'RBR':'adverb, comparative better','RBS':'adverb, superlative best',
'RP':'particle give up','TO': 'to go'to' the store.','UH':'interjection errrrrrrrm',
'VB': 'verb,base form take','VBD': 'verb,past tense took',
'VBG': 'verb,gerund/present participle taking','VBN': 'verb,past participle taken',
'VBP': 'verb,sing. present, non-3d take','VBZ': 'verb,3rd person sing. present takes',
'WDT':'wh-determiner which','WP':'wh-pronoun who, what','WP$':'possessive wh-pronoun whose',
'WRB':'wh-abverb where, when','QF' :'quantifier, bahut, thoda, kam (Hindi)','VM' : 'mainverb',
'PSP' :'postposition, common in indian langs','DEM' :'demonstrative, common in indian langs'
}
\#extract parts of speech
def extract_pos(doc):
parsed_text = {'word':[], 'pos':[],'exp':[]}
for sent in doc.sentences:
for wrd in sent.words:
if wrd.pos in pos_dict.keys():
pos_exp = pos_dict[wrd.pos]
else:
pos_exp = 'NA'
parsed_text['word'].append(wrd.text)
parsed_text['pos'].append(wrd.pos)
parsed_text['exp'].append(pos_exp)
\#return a dataframe of pos and text
return pd.DataFrame(parsed_text)
\#extract pos
extract_pos(doc)
注意到上面代码中的巨型库了吗？它只是PoS标签及其含义之间的映射。这有助于更好地理解我们文档的语法结构。
输出将是一个包含三列的数据框- word，pos和exp（explanation）。解释栏为我们提供了有关文本的大部分信息（因此非常有用）。
![什么样的NLP库，可以支持53种语言？](http://p3.pstatp.com/large/pgc-image/08ffe58a1c184a9d8cf4774deb294f96)

添加解释栏可以更轻松地评估处理器的精度。标签器对于大多数单词的标记都是准确的。它甚至可以提取一个单词的时态，无论是基本形式还是复数形式。
依赖项提取
依赖项提取是StanfordNLP的另一个现有的功能。您可以简单地在句子上调用print_dependencies（）来获取其所有单词的依赖关系：
doc.sentences[0].print_dependencies()

![什么样的NLP库，可以支持53种语言？](http://p3.pstatp.com/large/pgc-image/936f2830b1e54cc082a9fcecb93c3798)

该库在单次运行通道期间计算上述所有内容。在启用GPU的计算机上，仅需几分钟的时间。
我们现在已经找到了一种使用StanfordNLP执行基本文本处理的方法。现在是时候利用该方法来对其他50多种语言做同样的事情啦！
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**在印地语上实施StanfordNLP**
StanfordNLP在其性能和多语言文本解析支持方面确实非常突出。让我们深入探讨以下方面。
处理印地语文本（梵文脚本）
首先，我们必须下载印地语语言模型（相对较小）：
stanfordnlp.download('hi')
现在，用印地语作为我们的文本文件：
hindi_doc =nlp("""केंद्रकीमोदीसरकारनेशुक्रवारकोअपनाअंतरिमबजटपेशकिया. कार्यवाहकवित्तमंत्रीपीयूषगोयलनेअपनेबजटमेंकिसान, मजदूर, करदाता, महिलावर्गसमेतहरकिसीकेलिएबंपरऐलानकिए. हालांकि, बजटकेबादभीटैक्सकोलेकरकाफीकन्फ्यूजनबनारहा. केंद्रसरकारकेइसअंतरिमबजटक्याखासरहाऔरकिसकोक्यामिला, आसानभाषामेंयहांसमझें""")
这应该足以生成所有标签。我们来看看印地语的标签：
extract_pos(hindi_doc)

![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/ff931566d32442c8b428b4f81558faf2)

PoS标记器在印地语文本上的效果也令人惊讶。以“अपना”为例， PoS标签将其标记为代词 - 我，他，她 - 这是准确的。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**使用CoreNLP的API进行文本分析**
CoreNLP是经过时间考验的行业级NLP工具包，以其性能和准确性而闻名。StanfordNLP已被宣布作为CoreNLP的官方python接口。这对于这个库来说是一个巨大的胜利。
之前一直为CoreNLP创建Python包装程序包而努力，但没有什么比作者采用自己的官方版本实现的效果更好。这意味着该库将定期更新和改进。
StanfordNLP需要三行代码才能开始使用CoreNLP的复杂API。从字面上看，只需三行代码就可以设置它。
1. 下载CoreNLP包。打开Linux终端并键入以下命令：
wgethttp://nlp.stanford.edu/software/stanford-corenlp-full-2018-10-05.zip
2. 解压缩下载的包：
unzipstanford-corenlp-full-2018-10-05.zip
3. 启动CoreNLP服务器：
java -mx4g-cp "*" edu.stanford.nlp.pipeline.StanfordCoreNLPServer -port 9000-timeout 15000
注意：CoreNLP需要运行Java8。请确保你已安装JDK和JRE 1.8.x.p。
现在，确保StanfordNLP知道CoreNLP存在的位置。为此，你必须导出$CORENLP_HOME作为文件夹的位置。在作者的例子中，这个文件夹在the home里，所以路径应该是：
exportCORENLP_HOME=stanford-corenlp-full-2018-10-05/
完成上述步骤后，你可以启动服务器并使用Python代码发出请求。下面是启动服务器，发出请求以及从返回的对象访问数据的综合示例。
设置CoreNLPClient
from stanfordnlp.server import CoreNLPClient
\# exampletext
print('---')
print('inputtext')
print('')
text ="Chris Manning is a nice person. Chris wrote a simple sentence. He alsogives oranges to people."
print(text)
\# set up theclient
print('---')
print('startingup Java Stanford CoreNLP Server...')
\# set up theclient
with CoreNLPClient(annotators=['tokenize','ssplit','pos','lemma','ner','depparse','coref'],timeout=30000, memory='16G') as client:
\# submit the request to the server
ann = client.annotate(text)
\# get the first sentence
sentence = ann.sentence[0]
依赖性分析和POS
\# get thedependency parse of the first sentence
print('---')
print('dependency parse of first sentence')
dependency_parse =sentence.basicDependencies
print(dependency_parse)
\# get the first token of the first sentence
print('---')
print('first token of first sentence')
token = sentence.token[0]
print(token)
\# get the part-of-speech tag
print('---')
print('part of speech tag of token')
token.pos
print(token.pos)
命名实体识别和共同参考链
\# get the named entity tag
print('---')
print('named entity tag of token')
print(token.ner)
\# get an entity mention from the first sentence
print('---')
print('first entity mention in sentence')
print(sentence.mentions[0])
\# access the coref chain
print('---')
print('coref chains for the example')
print(ann.corefChain)
\# Use tokensregex patterns to find whowrote a sentence.
pattern = '([ner: PERSON]+) /wrote/ /an?/[]{0,3} /sentence|article/'
matches = client.tokensregex(text, pattern)
\# sentences contains a list with matchesfor each sentence.
assert len(matches["sentences"])== 3
\# length tells you whether or not there areany matches in this
assert matches["sentences"][1]["length"] == 1
\# You can access matches like most regexgroups.
matches["sentences"][1]["0"]["text"] =="Chris wrote a simple sentence"
matches["sentences"][1]["0"]["1"]["text"]== "Chris"
\# Use semgrex patterns to directly find whowrote what.
pattern = '{word:wrote} >nsubj{}=subject >dobj {}=object'
matches = client.semgrex(text, pattern)
\# sentences contains a list with matchesfor each sentence.
assert len(matches["sentences"])== 3
\# length tells you whether or not there areany matches in this
assert matches["sentences"][1]["length"] == 1
\# You can access matches like most regexgroups.
matches["sentences"][1]["0"]["text"] =="wrote"
matches["sentences"][1]["0"]["$subject"]["text"]== "Chris"
matches["sentences"][1]["0"]["$object"]["text"]== "sentence"
上面的例子几乎没有触及CoreNLP可以做什么，但它非常有趣，我们能够完成从基本的NLP任务，如词性标记，到命名实体识别，共同参考链提取和查找谁用几行Python代码在一个句子中写了什么。
![什么样的NLP库，可以支持53种语言？](http://p1.pstatp.com/large/pgc-image/0cac4011fddc412b800ace61b702b290)

**关于使用StanfordNLP的思考——优点和缺点**
探索新提出的库肯定是一个挑战，因为几乎没有关于StanfordNLP的文档，但这却是一次非常愉快的学习经历。
关于StanfordNLP未来，有几件事会令人兴奋不已：
1. 它为多种语言提供现成的支持。
2. 事实上，它将成为CoreNLP的官方Python接口。这意味着它会在功能和易用性方面有所改进。
3. 它的速度相当快（除了内存占用量较大）
4. 在Python中直接设置
然而，仍然有一些问题需要解决和改进：
1. 语言模型的所占空间太大（英文为1.9 GB，中文为~1.8 GB）
2. 库需要大量的代码，以生产出功能。将其与NLTK相比较，你可以在其中快速编写原型脚本——然而这对于StanfordNLP来说几乎是不可能的。
3. 目前缺少可视化功能。对依赖项分析等函数很有用。但与SpaCy等库相比，StanfordNLP在这里还存在一些不足。

