
# 语音的基本概念--译自CMU sphinx - zouxy09的专栏 - CSDN博客


2012年09月04日 10:53:23[zouxy09](https://me.csdn.net/zouxy09)阅读数：21930


**语音的基本概念--****译自****CMU sphinx**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
译自：http://cmusphinx.sourceforge.net/wiki/tutorialconcepts
这是CMU sphinx语音识别系统wiki的第一部分，主要是介绍语音的一些基本概念的。我试着翻译了一下。英语水平受限，翻译难免出错，请各位不吝指点！呵呵
**Basic concepts of speech**
**语音的基本概念**
Speech is a complex phenomenon. People rarely understand how is it produced and perceived. The naive perception is often that speech is built with words, and each word consists of phones. The reality is unfortunately very different. Speech is a dynamic process without clearly distinguished parts. It's always useful to get a sound editor and look into the recording of the speech and listen to it. Here is for example the speech recording in an audio editor.
语音是一个复杂的现象。我们基本上不知道它是如何产生和被感知的。我们最基础的认识就是语音是由单词来构成的，然后每个单词是由音素来构成的。但事实与我们的理解大相径庭。语音是一个动态过程，不存在很明显的部分划分。通过音频编辑软件去查看一个语音的录音对于理解语音是一个比较有效的方法。下面就是一个录音在音频编辑器里的显示的例子。
![](https://img-my.csdn.net/uploads/201209/04/1346727258_7303.jpg)
All modern descriptions of speech are to some degree probabilistic. That means that there are no certain boundaries between units, or between words. Speech to text translation and other applications of speech are never 100% correct. That idea is rather unusual for software developers, who usually work with deterministic systems. And it creates a lot of issues specific only to speech technology.
目前关于语音的所有描述说明从某种程度上面讲都是基于概率的（基于频谱？）。这意味着在语音单元或者单词之间并没有确定的边界。语音识别技术没办法到达100%的准确率。这个概念对于软件开发者来说有点不可思议，因为他们所研究的系统通常都是确定性的。另外，对于语音技术来说，它会产生很多和语言相关的特定的问题。
**Structure of speech**
**语音的构成**
In current practice, speech structure is understood as follows:
在本文中，我们是按照以下方式去理解语音的构成的：
Speech is a continuous audio stream where rather stable states mix with dynamically changed states. In this sequence of states, one can define more or less similar classes of sounds, or phones. Words are understood to be built of phones, but this is certainly not true. The acoustic properties of a waveform corresponding to a phone can vary greatly depending on many factors - phone context, speaker, style of speech and so on. The so called coarticulation协同发音makes phones sound very different from their“canonical” representation. Next, since transitions between words are more informative than stable regions, developers often talk about diphones - parts of phones between two consecutive phones. Sometimes developers talk about subphonetic units - different substates of a phone. Often three or more regions of a different nature can easily be found.
The number three is easily explained. The first part of the phone depends on its preceding phone, the middle part is stable, and the next part depends on the subsequent phone. That's why there are often three states in a phone selected for HMM recognition.
语音是一个连续的音频流，它是由大部分的稳定态和部分动态改变的状态混合构成。
一个单词的发声（波形）实际上取决于很多因素，而不仅仅是音素，例如音素上下文、说话者、语音风格等；
协同发音（指的是一个音受前后相邻音的影响而发生变化，从发声机理上看就是人的发声器官在一个音转向另一个音时其特性只能渐变，从而使得后一个音的频谱与其他条件下的频谱产生差异。）的存在使得音素的感知与标准不一样，所以我们需要根据上下文来辨别音素。将一个音素划分为几个亚音素单元。如：数字“three”，音素的第一部分与在它之前的音素存在关联，中间部分是稳定的部分，而最后一部分则与下一个音素存在关联，这就是为什么在用HMM模型做语音识别时，选择音素的三状态HMM模型。（上下文相关建模方法在建模时考虑了这一影响，从而使模型能更准确地描述语音，只考虑前一音的影响的称为Bi-Phone，考虑前一音和后一音的影响的称为Tri-Phone。）
Sometimes phones are considered in context. There are triphones or even quinphones. But note that unlike phones and diphones, they are matched with the same range in waveform as just phones. They just differ by name. That's why we prefer to call this object senone. A senone's dependence on context could be more complex than just left and right context. It can be a rather complex function defined by a decision tree, or in some other way.
有时候，音素会被放在上下文中考虑，这样就形成了三元音素或者多元音素。但它与亚音素不同，他们在波形中匹配时长度还是和单一音素一样。只是名字上的不同而已，所以我们更倾向于将这样的多元音素称为senone。一个senone的上下文依赖比单纯的左右上下文复杂得多，它是一个可以被决策树或者其他方式来定义的复杂函数。（英语的上下文相关建模通常以音素为基元，由于有些音素对其后音素的影响是相似的，因而可以通过音素解码状态的聚类进行模型参数的共享。聚类的结果称为senone。决策树用来实现高效的triphone对senone的对应，通过回答一系列前后音所属类别（元/辅音、清/浊音等等）的问题，最终确定其HMM状态应使用哪个senone。分类回归树CART模型用以进行词到音素的发音标注。）
Next, phones build subword units, like syllables. Sometimes, syllables are defined as “reduction-stable entities”. To illustrate, when speech becomes fast, phones often change, but syllables remain the same. Also, syllables are related to intonational contour. There are other ways to build subwords - morphologically-based in morphology-rich languages or phonetically-based. Subwords are often used in open vocabulary speech recognition.
音素phones构成亚单词单元，也就是音节syllables。音节是一个比较稳定的实体，因为当语音变得比较快的时候，音素往往会发生改变，但是音节却不变。音节与节奏语调的轮廓有关。有几种方式去产生音节：基于形态学或者基于语音学。音节经常在词汇语音识别中使用。
Subwords form words. Words are important in speech recognition because they restrict combinations of phones significantly. If there are 40 phones and an average word has 7 phones, there must be 40^7 words. Luckily, even a very educated person rarely uses more then 20k words in his practice, which makes recognition way more feasible.
亚单词单元（音节）构成单词。单词在语音识别中很重要，因为单词约束了音素的组合。假如共有40个音素，然后每个单词平均有7个音素，那么就会存在40^7个单词，但幸运的是就算一个受过优等教育的人也很少使用过20k个单词，这就使识别变得可行。
Words and other non-linguistic sounds, which we call fillers (breath, um, uh, cough), form utterances. They are separate chunks of audio between pauses. They don't necessary match sentences, which are more semantic concepts.
单词和一些非语言学声音构成了话语utterances，我们把非语言学声音称为fillers填充物，例如呼吸，um，uh，咳嗽等，它们在音频中是以停顿做分离的。所以它们更多只是语义上面的概念，不算是一个句子。
On the top of this, there are dialog acts like turns, but they go beyond the purpose of the document.
**Recognition process**
**识别过程**
The common way to recognize speech is the following: we take waveform, split it on utterances by silences then try to recognize what's being said in each utterance. To do that we want to take all possible combinations of words and try to match them with the audio. We choose the best matching combination. There are few important things in this match.
语音识别一般的方法是：录制语音波形，再把波形通过静音silences分割为多个utterances，然后去识别每个utterance所表达的意思。为了达到这个目的，我们需要用单词的所有可能组合去匹配这段音频，然后选择匹配度最高的组合。
在匹配中有几个关键的概念需要了解的：
First of all it's a concept of features. Since number of parameters is large, we are trying to optimize it. Numbers that are calculated from speech usually by dividing speech on frames. Then for each frame of length typically 10 milliseconds we extract 39 numbers that represent the speech. That's called feature vector. The way to generates numbers is a subject of active investigation, but in simple case it's a derivative from spectrum.
**特征：**
由于描述一个语音，需要的参数个数非常多，这样对处理速度的要求就很高（而且也没必要处理那么多的信息，我们只需要处理对识别有帮助的就行），所以我们需要做优化，进行降维。我们用帧frames去分割语音波形，每帧大概10ms，然后每帧提取可以代表该帧语音的39个数字，这39个数字也就是该帧语音的特征，用特征向量来表示。而如何提取特征向量是当下热门的研究课题，但这些提取方法都是由频谱衍生出来的。
Second it's a concept of the model. Model describes some mathematical object that gathers common attributes of the spoken word. In practice, for audio model of senone is gaussian mixture of it's three states - to put it simple, it's a most probable feature vector. From concept of the model the following issues raised - how good does model fits practice, can model be made better of it's internal model problems, how adaptive model is to the changed conditions.
**模型：**
模型是用来描述一些数学对象的。这些数学对象描述了一些口语的共同属性。在实际应用中，senone的音频模型就是三态高斯混合模型。简单的说，它就是一个最有可能的特征向量。对于模型，有几个问题需要考虑：模型到底多大程度上可以描述实际情况？在模型本身的局限情况下模型能表现得更优吗？自适应模型如何改变条件？
Third, it's a matching process itself. Since it would take a huge time more than universe existed to compare all feature vectors with all models, the search is often optimized by many tricks. At any points we maintain best matching variants and extend them as time goes producing best matching variants for the next frame.
**匹配算法：**
语音识别需要对所有的特征向量和所有的模型做比较匹配，这是一个非常耗时的工作。而在这方面的优化往往是使用一些技巧，在每一点的匹配时，我们通过保留最好的匹配variants，然后通过它在下一帧产生最好的匹配variants。？
**Models**
According to the speech structure, three models are used in speech recognition to do the match:
An acoustic model contains acoustic properties for each senone. There are context-independent models that contain properties (most probable feature vectors for each phone) and context-dependent ones (built from senones with context).
**声学模型****acoustic model****：**
一个声学模型包含每个senone的声学属性，其包括不依赖于上下文的属性（每个音素phone最大可能的特征向量）和依赖于上下文的属性（根据上下文构建的senone）。
A phonetic dictionary contains a mapping from words to phones. This mapping is not very effective. For example, only two to three pronunciation variants are noted in it, but it's practical enough most of the time. The dictionary is not the only variant of mapper from words to phones. It could be done with some complex function learned with a machine learning algorithm.
**语音学字典****phonetic dictionary****：**
字典包含了从单词words到音素phones之间的映射。
字典并不是描述单词words到音素phones之间的映射的唯一方法。可以通过运用机器学习算法去学习得到一些复杂的函数去完成映射功能。
A language model is used to restrict word search. It defines which word could follow previously recognized words (remember that matching is a sequential process) and helps to significantly restrict the matching process by stripping words that are not probable. Most common language models used are n-gram language models-these contain statistics of word sequences-and finite state language models-these define speech sequences by finite state automation, sometimes with weights. To reach a good accuracy rate, your language model must be very successful in search space restriction. This means it should be very good at predicting the next word. A language model usually restricts the vocabulary considered to the words it contains. That's an issue for name recognition. To deal with this, a language model can contain smaller chunks like subwords or even phones. Please note that search space restriction in this case is usually worse and corresponding recognition accuracies are lower than with a word-based language model.
**语言模型****language model****：**
语言模型是用来约束单词搜索的。它定义了哪些词能跟在上一个已经识别的词的后面（匹配是一个顺序的处理过程），这样就可以为匹配过程排除一些不可能的单词。大部分的语言模型都是使用n-gram模型，它包含了单词序列的统计。和有限状态模型，它通过有限状态机来定义语音序列。有时候会加入权值。为了达到比较好的识别准确率，语言模型必须能够很好的约束空间搜索，也就是说可以更好的预测下一个词。语言模型是约束词汇包含的单词的，这就出现一个问题，就是名字识别（因为名字可以随便由几个单词组成）。为了处理这种情况，语言模型可以包含更小的块，例如亚单词，甚至音素。但是这种情况，识别准确率将会低于基于单词的语言模型。
Those three entities are combined together in an engine to recognize speech. If you are going to apply your engine for some other language, you need to get such structures in place. For many languages there are acoustic models, phonetic dictionaries and even large vocabulary language models available for download.
特征、模型和搜索算法三部分构成了一个语音识别系统。如果你需要识别不同的语言，那么就需要修改这三个部分。很多语言，都已经存在声学模型，字典，甚至大词汇量语言模型可供下载了。
**Other concepts used**
**其他用到的概念**
A Lattice is a directed graph that represents variants of the recognition. Often, getting the best match is not practical; in that case, lattices are good intermediate formats to represent the recognition result.
**网格****Lattice**是一个代表识别的不同结果的有向图。一般来说，很难去获得一个最好的语音匹配结果。所以Lattices就是一个比较好的格式去存放语音识别的中间结果。
N-best lists of variants are like lattices, though their representations are not as dense as the lattice ones.
**N-best lists**和lattices有点像，但是它没有lattices那么密集（也就是保留的结果没有lattices多）。（N-best搜索和多遍搜索：为在搜索中利用各种知识源，通常要进行多遍搜索，第一遍使用代价低的知识源（如声学模型、语言模型和音标词典），产生一个候选列表或词候选网格，在此基础上进行使用代价高的知识源（如4阶或5阶的N-Gram、4阶或更高的上下文相关模型）的第二遍搜索得到最佳路径。）
Word confusion networks (sausages) are lattices where the strict order of nodes is taken from lattice edges.
**单词混淆网络**是从lattice的边缘得到的一个严格的节点顺序序列。
Speech database - a set of typical recordings from the task database. If we develop dialog system it might be dialogs recorded from users. For dictation system it might be reading recordings. Speech databases are used to train, tune and test the decoding systems.
**语音数据库**-一个从任务数据库得到的典型的录音集。如果我们开发的是一个对话的系统，那么数据库就是包含了多个用户的对话录音。而对于听写系统，包含的就是朗读的录音。语音数据库是来用训练，调整和测试解码系统的（也就是语音识别系统）。
Text databases - sample texts collected for language model training and so on. Usually, databases of texts are collected in sample text form. The issue with collection is to put present documents (PDFs, web pages, scans) into spoken text form. That is, you need to remove tags and headings, to expand numbers to their spoken form, and to expand abbreviations.
**文本数据库**-为了训练语言模型而收集的文本。一般是以样本文本的方式来收集形成的。而收集过程存在一个问题就是误把PDFs, web pages, scans等现成文档也当成口语文本的形式放进数据库中。所以，我们就需要把这些文件带进数据库里面的标签和文件头去掉，还有把数字展开为它们的语音形式（例如1展开为英文的one或者汉语的yi），另外还需要把缩写给扩大还原为完整单词。
**What is optimized**
**语音的优化**
When speech recognition is being developed, the most complex issue is to make search precise (consider as many variants to match as possible) and to make it fast enough to not run for ages. There are also issues with making the model match the speech since models aren't perfect.
随着语音识别技术的发展，最复杂的难题是如何使搜索（也就是语音解码，可以认为是需要匹配尽可能多的语音变体）更加准确和快速。还有在模型并不完美的前提下如何匹配语音和模型。
Usually the system is tested on a test database that is meant to represent the target task correctly.
一般来说系统需要通过一个测试数据库来验证准确性，也就是是否达到了我们的预定目标。
The following characteristics are used:
我们通过以下几个参数来表征系统的性能：
**Word error rate**. Let we have original text and recognition text of length of N words. From them the I words were inserted D words were deleted and S words were substituted Word error rate is
WER = (I + D + S) / N
WER is usually measured in percent.
单词错误率：我们有一个N个单词长度的原始文本和识别出来的文本。（对单词串进行识别难免有词的插入，替换和删除的误识）I代表被插入的单词个数，D代表被删除的单词个数，S代表被替换的单词个数，那么单词错误率就定义为：WER = (I + D + S) / N
单词错误率一般通过百分百来表示。
**Accuracy.**It is almost the same thing as word error rate, but it doesn't count insertions.
Accuracy = (N - D - S) / N
Accuracy is actually a worse measure for most tasks, since insertions are also important in final results. But for some tasks, accuracy is a reasonable measure of the decoder performance.
准确度。它和单词错误率大部分是相似的，但是它不计算插入单词的个数，它定义为：Accuracy = (N - D - S) / N
对于大部分任务来说，准确度事实上是一个比较差的度量方法，因为插入的情况对于识别结果的影响也是很重要的。但对于一些任务而言，准确度也是一个合理的评价解码器性能的参数。
**Speed.**Suppose the audio file was 2 hours and the decoding took 6 hours. Then speed is counted as 3xRT.
速度：假设音频文件是2个小时，而解码花费了6个小时，那么计算出来的速度就是3xRT。（3倍速）
**ROC curves.**When we talk about detection tasks, there are false alarms and hits/misses; ROC curves are used. A curve is a graphic that describes the number of false alarms vs number of hits, and tries to find optimal point where the number of false alarms is small and number of hits matches 100%.
ROC曲线：对于一个检测任务，检测会出现误报和命中两种情况。ROC曲线就是用来评价检测性能的。ROC曲线就是描述误报和命中的数目比例的。而且可以通过ROC曲线取寻找一个最优点，在这个点误报最小，而命中最大，也就是接近100%的命中率。
There are other properties that aren't often taken into account, but still important for many practical applications. Your first task should be to build such a measure and systematically apply it during the system development. Your second task is to collect the test database and test how does your application perform.
还有其他的方法来衡量识别性能，虽然这里没有提及，但对于很多的实际应用来说还是比较重要的。你的第一个工作应该是建立这样一个评价体系，然后系统地应用到开发过程中。第二个工作就是收集一个测试数据库来测试你的系统性能。

