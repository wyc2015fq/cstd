# Python下载PTB数据集的方法（附NLP常用数据集） - Machine Learning with Peppa - CSDN博客





2019年01月28日 15:59:23[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：317








ptb数据集是语言模型学习中应用最广泛的数据集，常用该数据集训练RNN神经网络作为语言预测，tensorflow对于ptb数据集的读取也定义了自己的函数库用于读取，在python 1.0定义了models文件用于导入ptb库函数，然而当python升级后，导入models文件时就会出现：ModuleNotFountError错误，这时需要靠自己下载导入，github上有人共享了models文件，但是不清楚如何安装，网上教程很多，但是安装了还有很多的错误，本人捣鼓了一天算将其成功导入，因此写成教程，可以不用下载低版本tensorflow，注意：该教程适用于linux系统下tensorflow。

**步骤1：**在低版本tensorflow中，导入ptb库的语句为“from tensorflow.models.rnn.ptb import reader”，其形式与导入mnist库一样，因此我们需要查找安装models库的位置，在命令行中输入:

**locate tensorflow/examples/tutorials**

此时将会显示出有上面路径的文件，找到路径*/tensorflow/examples/tutorials/mnist，此时路径*/tensorflow就是我们安装models的路径，用cd命令进入该文件。

**步骤2：**进入上面tensorflow文件后，用git下载models文件夹，在命令行中输入命令：

**git clone –recurse-submodules[https://github.com/tensorflow/models](https://github.com/tensorflow/models)**

如果没有安装git，请自行百度如何安装git

**步骤3：**此时运行含有语句“from tensorflow.models.rnn.ptb import reader”还是会出错，主要是因为下载的文件内容与低版本的库有一定区别，可以逐步进入路径“*/tensorflow/models”发现，没有文件rnn，rnn文件存在与路径“*/tensorflow/models/tutorials/”文件下，因此我们需要将该语句改成

**“from tensorflow.models.tutorials.rnn.ptb import reader”**

**步骤4：**此时还会出错，提示ModuleNotFoundError:No module name ‘reader’，此时我们需要对ptb中的__init__.py文件进行修改，**将该文件中的“import reader”修改成“from tensorflow.models.tutorials.rnn.ptb import reader”，还有将“import util”修改成“from tensorflow.models.tutorials.rnn.ptb import util”** 此时再次运行程序，将成功导入ptb



Treebanks and annotated corpus useful for training POS tagger, parser etc

Penn Treebank http://www.cis.upenn.edu/~treebank/home.html

WSJ Corpus https://catalog.ldc.upenn.edu/LDC2000T43

NEGRA German corpus http://www.coli.uni-saarland.de/projects/sfb378/negra-corpus/

Tiger corpus http://www.ims.uni-stuttgart.de/projekte/TIGER/TIGERCorpus/

alpino Treebank http://odur.let.rug.nl/~vannoord/trees/

Bultreebank http://www.bultreebank.org/

Turin University Treebank http://www.di.unito.it/~tutreeb/

prague dependency Treebank http://ufal.mff.cuni.cz/pdt2.0/

Semantic relation annotated corpus

propbank 

Nombank http://nlp.cs.nyu.edu/meyers/NomBank.html

framenet http://framenet.icsi.berkeley.edu/

salsa http://www.coli.uni-saarland.de/projects/salsa/page.php?id=index

Text classification corpus

Reuters dataset http://www.daviddlewis.com/resources/testcollections/reuters21578/

news group datasets http://people.csail.mit.edu/jrennie/20Newsgroups/

Parallel corpus used in machine translation

EMILE http://www.lancs.ac.uk/fass/projects/corpus/emille/

Text summarization

DUC-2001, 2002, 2003, 2004, 2005, 2006, 2007 http://www-nlpir.nist.gov/projects/duc/data.html

TAC-2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 http://tac.nist.gov/data/

Gigawords https://catalog.ldc.upenn.edu/LDC2012T21

LCSTS http://icrc.hitsz.edu.cn/Article/show/139.html

Machine Reading

CNN http://datasets.maluuba.com/NewsQA

Microsoft https://arxiv.org/abs/1611.09268

Microsoft Marco http://www.msmarco.org/

SQuAD https://www.aclweb.org/anthology/D16-1264

Others

TREC

SemEval http://alt.qcri.org/semeval2017/index.php?id=tasks

Microsoft COCO: http://mscoco.org/




