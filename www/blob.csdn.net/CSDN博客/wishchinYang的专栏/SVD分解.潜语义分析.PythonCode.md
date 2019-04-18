# SVD分解.潜语义分析.PythonCode - wishchinYang的专栏 - CSDN博客
2014年01月18日 15:51:55[wishchin](https://me.csdn.net/wishchin)阅读数：2402
原文链接：[http://www.cnblogs.com/appler/archive/2012/02/02/2335886.html](http://www.cnblogs.com/appler/archive/2012/02/02/2335886.html)
原始英文链接：[http://www.puffinwarellc.com/index.php/news-and-articles/articles/33.html](http://www.puffinwarellc.com/index.php/news-and-articles/articles/33.html)
**潜语义分析LSA介绍**
Latent Semantic Analysis (LSA), also known as Latent Semantic Indexing (LSI) literally means analyzing documents to find the underlying meaning or concepts of those documents. If each word only meant one concept, and each concept
 was only described by one word, then LSA would be easy since there is a simple mapping from words to concepts.
Latent Semantic Analysis (LSA)也被叫做Latent Semantic Indexing (LSI)，从字面上的意思理解就是通过分析文档去发现这些文档中潜在的意思和概念。假设每个词仅表示一个概念，并且每个概念仅仅被一个词所描述，LSA将非常简单**（**从词到概念存在一个简单的映射关系）
![one to one mapping between words and concepts](http://www.puffinwarellc.com/images/stories/diagram1.png)
Unfortunately, this problem is difficult because English has different words that mean the same thing (synonyms), words with multiple meanings, and all sorts of ambiguities that obscure the concepts to the point where even people
 can have a hard time understanding.
不幸的是，这个问题并没有如此简单，因为存在不同的词表示同一个意思（同义词），一个词表示多个意思，所有这种二义性（多义性）都会混淆概念以至于有时就算是人也很难理解。
![confused mapping between words and concepts](http://www.puffinwarellc.com/images/stories/diagram2.png)
For example, the word bank when used together with mortgage, loans, and rates probably means a financial institution. However, the word bank when used together with lures, casting, and fish probably means a stream or river bank.
例如,银行这个词和抵押、贷款、利率一起出现时往往表示金融机构。但是，和鱼饵，投掷、鱼一起出现时往往表示河岸。
**How Latent Semantic Analysis Works**
**潜语义分析工作原理**
Latent Semantic Analysis arose from the problem of how to find relevant documents from search words. The fundamental difficulty arises when we compare *words* to find relevant documents, because what we really
 want to do is compare the *meanings or concepts behind the words*. LSA attempts to solve this problem by mapping both words and documents into a "concept" space and doing the comparison in this space.
潜语义分析（Latent Semantic Analysis）源自问题：如何从搜索query中找到相关的文档。当我们试图通过比较词来找到相关的文本时，存在着难以解决的局限性，那就是在搜索中我们实际想要去比较的不是词，而是隐藏在词之后的意义和概念。潜语义分析试图去解决这个问题，它把词和文档都映射到一个‘概念’空间并在这个空间内进行比较（注：也就是一种降维技术）。
Since authors have a wide choice of words available when they write, the concepts can be obscured due to different word choices from different authors. This essentially random choice of words introduces noise into
 the word-concept relationship. Latent Semantic Analysis filters out some of this noise and also attempts to find the smallest set of concepts that spans all the documents.
当文档的作者写作的时候，对于词语有着非常宽泛的选择。不同的作者对于词语的选择有着不同的偏好，这样会导致概念的混淆。这种对于词语的随机选择在 词-概念 的关系中引入了噪音。LSA滤除了这样的一些噪音，并且还能够从全部的文档中找到最小的概念集合（为什么是最小？）。
In order to make this difficult problem solvable, LSA introduces some dramatic simplifications.
1.     Documents are represented as "bags of words", where the order of the words in a document is not important, only how many times each word appears in a document.
2.     Concepts are represented as patterns of words that usually appear together in documents. For example "leash", "treat", and "obey" might usually appear in documents about dog training.
3.     Words are assumed to have only one meaning. This is clearly not the case (banks could be river banks or financial banks) but it makes the problem tractable.
To see a small example of LSA, take a look at the next section.
为了让这个难题更好解决，LSA引入一些重要的简化：
    1. 文档被表示为”一堆词（bags of words）”，因此词在文档中出现的位置并不重要，只有一个词的出现次数。
    2. 概念被表示成经常出现在一起的一些词的某种模式。例如“leash”（栓狗的皮带）、“treat”、“obey”（服从）经常出现在关于训练狗的文档中。
    3. 词被认为只有一个意思。这个显然会有反例（bank表示河岸或者金融机构），但是这可以使得问题变得更加容易。（这个简化会有怎样的缺陷呢？）
接下来看一个LSA的小例子，Next Part：
**A Small Example**
**一个例子**
As a small example, I searched for books using the word “investing” at Amazon.com and took the top 10 book titles that appeared. One of these titles was dropped because it had only one index word in common with the
 other titles. An index word is any word that:
- appears in 2 or more titles, and
- is not a very common word such as “and”, “the”, and so on (known as stop words). These words are not included because do not contribute much (if any) meaning.
In this example we have removed the following stop words: “and”, “edition”, “for”, “in”, “little”, “of”, “the”, “to”.
一个小例子，我在amazon.com上搜索”investing”(投资) 并且取top 10搜索结果的书名。其中一个被废弃了，因为它只含有一个索引词（index word）和其它标题相同。索引词可以是任何满足下列条件的词：
    1. 在2个或者2个以上标题中出现 并且
    2. 不是那种特别常见的词例如 “and”, ”the” 这种（停用词-stop word）。这种词没有包含进来是因为他们本身不存在什么意义。
在这个例子中，我们拿掉了如下停用词：“and”, “edition”, “for”, “in”, “little”, “of”, “the”, “to”.
Here are the 9 remaining tiles. The index words (words that appear in 2 or more titles and are not stop words) are underlined.
下面就是那9个标题，索引词（在2个或2个以上标题出现过的非停用词）被下划线标注：
1.     The Neatest Little 
Guide to Stock Market Investing
2.     Investing ForDummies, 4th Edition
3.     The Little Book of Common SenseInvesting: The Only Way to Guarantee Your Fair Share ofStock Market
 Returns
4.     The Little Book ofValue Investing
5.     Value Investing: From Graham to Buffett and Beyond
6.     Rich Dad's Guide toInvesting: What theRich Invest in, That the Poor and the Middle Class Do Not!
7.     Investing inReal Estate, 5th Edition
8.     Stock Investing ForDummies
9.     Rich Dad's Advisors: The ABC's ofReal Estate Investing: The Secrets of Finding Hidden Profits Most Investors Miss
Once Latent Semantic Analysis has been run on this example, we can plot the index words and titles on an XY graph and identify clusters of titles. The 9 titles are plotted with blue circles and the 11 index words
 are plotted with red squares. Not only can we spot clusters of titles, but since index words can be plotted along with titles, we can label the clusters. For example, the blue cluster, containing titles T7 and T9, is about real estate. The green cluster, with
 titles T2, T4, T5, and T8, is about value investing, and finally the red cluster, with titles T1 and T3, is about the stock market. The T6 title is an outlier, off on its own.
在这个例子里面应用了LSA，我们可以在XY轴的图中画出词和标题的位置（只有2维），并且识别出标题的聚类。蓝色圆圈表示9个标题，红色方块表示11个索引词。我们不但能够画出标题的聚类，并且由于索引词可以被画在标题一起，我们还可以给这些聚类打标签。例如，蓝色的聚类，包含了T7和T9，是关于real estate（房地产）的，绿色的聚类，包含了标题T2，T4，T5和T8，是讲value investing（价值投资）的，最后是红色的聚类，包含了标题T1和T3，是讲stock market（股票市场）的。标题T6是孤立点（outlier）
![](http://pic002.cnblogs.com/images/2012/236361/2012020315434871.png)
In the next few sections, we'll go through all steps needed to run Latent Semantic Analysis on this example.
在下面的部分，我们会通过这个例子介绍LSA的整个流程。
## Part 1 - Creating the Count Matrix
## 第一部分 - 创建计数矩阵
The first step in Latent Semantic Analysis is to create theword by title (or document) matrix. In this matrix, each index word is a rowand each title is a column. Each cell contains the number of times that wordoccurs in that
 title. For example, the word "book" appears one timein title T3 and one time in title T4, whereas "investing" appears onetime in every title. In general, the matrices built during LSA tend to be verylarge, but also very sparse (most cells contain 0). That
 is because each titleor document usually contains only a small number of all the possible words.This sparseness can be taken advantage of in both memory and time by moresophisticated LSA implementations.
LSA的第一步是要去创建词到标题（文档）的矩阵。在这个矩阵里，每一个索引词占据了一行，每一个标题占据一列。每一个单元（cell）包含了这个词出现在那个标题中的次数。例如，词”book”出现在T3中一次，出现在T4中一次，而” investing”在所有标题中都出现了一次。一般来说，在LSA中的矩阵会非常大而且会非常稀疏（大部分的单元都是0）。这是因为每个标题或者文档一般只包含所有词汇的一小部分。更复杂的LSA算法会利用这种稀疏性去改善空间和时间复杂度。
In the following matrix, we have left out the 0's to reduceclutter.
|**Index Words**|**Titles**| | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|
||T1|T2|T3|T4|T5|T6|T7|T8|T9|
|book|||1|1||||||
|dads||||||1|||1|
|dummies||1||||||1||
|estate|||||||1||1|
|guide|1|||||1||||
|investing|1|1|1|1|1|1|1|1|1|
|market|1||1|||||||
|real|||||||1||1|
|rich||||||2|||1|
|stock|1||1|||||1||
|value||||1|1|||||
### Python - Getting Started
Download the python code [here](http://www.puffinwarellc.com/lsa.py).
Throughout this article, we'll givePython code that implements all the steps necessary for doing Latent SemanticAnalysis. We'll go through the code section by section and explain everything.The Python code used in this article
 can be downloaded [here](http://www.puffinwarellc.com/lsa.py) and then run in Python. You need to havealready installed the Python NumPy and SciPy libraries.
在这篇文章中，我们用python代码去实现LSA的所有步骤。我们将介绍所有的代码。Python代码可以在这里被下到（见上）。需要安装NumPy 和 SciPy这两个库。
### Python - Import Functions
First we need to import a few functions from Python librariesto handle some of the math we need to do. NumPy is the Python numericallibrary, and we'll import zeros, a function that creates a matrix of zeros thatwe use when building
 our words by titles matrix. From the linear algebra partof the scientific package (scipy.linalg) we import the svd function thatactually does the singular value decomposition, which is the heart of LSA.
NumPy是python的数值计算类，用到了zeros（初始化矩阵），scipy.linalg这个线性代数的库中，我们引入了svd函数也就是做奇异值分解,LSA的核心。
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- from numpy import zeros  
- from scipy.linalg import svd  
### Python - Define Data
Next, we define the data that we are using. Titles holds the9 book titles that we have gathered, stopwords holds the 8 common words that weare going to ignore when we count the words in each title, and ignorechars hasall the punctuation
 characters that we will remove from words. We use Python'striple quoted strings, so there are actually only 4 punctuation symbols we areremoving: comma (,), colon (:), apostrophe ('), and exclamation point (!).
Stopwords 是停用词 ignorechars是无用的标点
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- titles =  
- 
- [   
- "The Neatest Little Guide to Stock Market Investing",   
- "Investing For Dummies, 4th Edition",   
- "The Little Book of Common Sense Investing: The Only Way to Guarantee Your Fair Share of Stock Market Returns",   
- "The Little Book of Value Investing",   
- "Value Investing: From Graham to Buffett and Beyond",   
- "Rich Dad's Guide to Investing: What the Rich Invest in, That the Poor and the Middle Class Do Not!",   
- "Investing in Real Estate, 5th Edition",   
- "Stock Investing For Dummies",   
- "Rich Dad's Advisors: The ABC's of Real Estate Investing: The Secrets of Finding Hidden Profits Most Investors Miss"
- ]  
- 
- stopwords = ['and','edition','for','in','little','of','the','to']   
- ignorechars = ''''',:'!'''
### Python - Define LSA Class
The LSA class has methods for initialization, parsingdocuments, building the matrix of word counts, and calculating. The firstmethod is the __init__ method, which is called whenever an instance of the LSAclass is created. It stores
 the stopwords and ignorechars so they can be usedlater, and then initializes the word dictionary and the document countvariables.
这里定义了一个LSA的类，包括其初始化过程wdict是词典，dcount用来记录文档号。
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- class LSA(object):  
- 
- def __init__(self, stopwords, ignorechars):  
- 
- self.stopwords = stopwords   
- self.ignorechars = ignorechars   
- self.wdict = {}   
- self.dcount = 0
### Python - Parse Documents
The parse method takes a document, splits it into words, removesthe ignored characters and turns everything into lowercase so the words can becompared to the stop words. If the word is a stop word, it is ignored and wemove on
 to the next word. If it is not a stop word, we put the word in thedictionary, and also append the current document number to keep track of whichdocuments the word appears in.
The documents that each word appears in are kept in a listassociated with that word in the dictionary. For example, since the word bookappears in titles 3 and 4, we would have self.wdict['book'] = [3, 4] after alltitles are parsed.
After processing all words from the current document, weincrease the document count in preparation for the next document to be parsed.
这个函数就是把文档拆成词并滤除停用词和标点，剩下的词会把其出现的文档号填入到wdict中去，例如，词book出现在标题3和4中，则我们有self.wdict['book'] = [3, 4]。相当于建了一下倒排。
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- def parse(self, doc):  
- 
-     words = doc.split();  for w in words:  
- 
-      w = w.lower().translate(None, self.ignorechars)   
- 
- if w inself.stopwords:  
- 
- continue
- 
- elif w inself.wdict:  
- 
- self.wdict[w].append(self.dcount)  
- 
- else:  
- 
- self.wdict[w] = [self.dcount]  
- 
- self.dcount += 1
### Python - Build the Count Matrix
Once all documents are parsed, all the words (dictionarykeys) that are in more than 1 document are extracted and sorted, and a matrix isbuilt with the number of rows equal to the number of words (keys), and thenumber of columns
 equal to the document count. Finally, for each word (key) anddocument pair the corresponding matrix cell is incremented.
所有的文档被解析之后，所有出现的词（也就是词典的keys）被取出并且排序。建立一个矩阵，其行数是词的个数，列数是文档个数。最后，所有的词和文档对所对应的矩阵单元的值被统计出来。
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- def build(self):  
- 
- self.keys = [k for k inself.wdict.keys() if len(self.wdict[k]) > 1]   
- 
- self.keys.sort()   
- 
- self.A = zeros([len(self.keys), self.dcount])   
- 
- for i, k in enumerate(self.keys):  
- 
- for d inself.wdict[k]:  
- 
- self.A[i,d] += 1
### Python - Print the Count Matrix
The printA() method is very simple, it just prints out thematrix that we have built so it can be checked.
把矩阵打印出来
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- def printA(self):  
- 
- printself.A  
### Python - Test the LSA Class
After defining the LSA class, it's time to try it out on our9 book titles. First we create an instance of LSA, called mylsa, and pass itthe stopwords and ignorechars that we defined. During creation, the __init__method is called
 which stores the stopwords and ignorechars and initializes theword dictionary and document count.
Next, we call the parse method on each title. This methodextracts the words in each title, strips out punctuation characters, convertseach word to lower case, throws out stop words, and stores remaining words in adictionary along
 with what title number they came from.
Finally we call the build() method to create the matrix ofword by title counts. This extracts all the words we have seen so far, throwsout words that occur in less than 2 titles, sorts them, builds a zero matrix ofthe right size,
 and then increments the proper cell whenever a word appears ina title.
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- mylsa = LSA(stopwords, ignorechars)   
- for t in titles:  
- 
- mylsa.parse(t)  
- 
- mylsa.build()   
- mylsa.printA()  
Here is the raw output produced by printA(). As you can see,it's the same as the matrix that we showed earlier.
在刚才的测试数据中验证程序逻辑，并查看最终生成的矩阵：
**[python]**[view plain](http://blog.csdn.net/yihucha166/article/details/6795112)[copy](http://blog.csdn.net/yihucha166/article/details/6795112)[print](http://blog.csdn.net/yihucha166/article/details/6795112)[?](http://blog.csdn.net/yihucha166/article/details/6795112)
- [[ 0.0.1.1.0.0.0.0.0.]  
- [ 0.0.0.0.0.1.0.0.1.]  
- [ 0.1.0.0.0.0.0.1.0.]  
- [ 0.0.0.0.0.0.1.0.1.]  
- [ 1.0.0.0.0.1.0.0.0.]  
- [ 1.1.1.1.1.1.1.1.1.]  
- [ 1.0.1.0.0.0.0.0.0.]  
- [ 0.0.0.0.0.0.1.0.1.]  
- [ 0.0.0.0.0.2.0.0.1.]  
- [ 1.0.1.0.0.0.0.1.0.]  
- [ 0.0.0.1.1.0.0.0.0.]] 
