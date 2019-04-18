# Jieba分词包（一）——解析主函数cut - bigfacesafdasgfewgf - CSDN博客





2014年11月03日 14:26:46[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：6225








# Jieba分词包（一）——解析主函数cut

## 1. 解析主函数cut

    Jieba分词包的主函数在jieba文件夹下的__init__.py中，在这个py文件中有个cut的函数，这个就是控制着整个jieba分词包的主函数。

    cut函数的定义如下：


```python
def cut(sentence,cut_all=False,HMM=True):
```


其给出的官方注释为：    

'''The main function that segments an entire sentence that contains 

    Chinese characters into seperated words. 

    Parameter:

        - sentence: The String to be segmented

        - cut_all: Model. True means full pattern, false means accurate pattern.   

        - HMM: Whether use Hidden Markov Model.

    '''

说白了就是，其中sentence是需要分词的句子样本；cut_all是分词的模式，稍微钻研过分词的小伙伴们都知道，jieba分词有全模式和精准模式两种，分别用true和false来选择，默认是false也就是精准模式；HMM就是隐马尔可夫链，这个是在分词的理论模型中用到的，默认是开启的。




下面我就接着cut函数中的注释，来说一下cut函数的整体框架和局部细节。





```python
def cut(sentence,cut_all=False,HMM=True):  
	#整个分词的主函数，
	#输入sentence是需要分词的句子；
	#cut_all是分词模式，默认是精准模式；
	#HMM是隐马尔可夫链，默认开启。
    '''The main function that segments an entire sentence that contains 
    Chinese characters into seperated words. 
    Parameter:
        - sentence: The String to be segmented
        - cut_all: Model. True means full pattern, false means accurate pattern.   
        - HMM: Whether use Hidden Markov Model.
    '''
    if not isinstance(sentence, unicode):  #如果输入的句子不是统一编码形式的unicode,那么使用Python中的decode，将其解码称为unicode
		#注：unicode、gbk等都是中文编码的方式
        try:
            sentence = sentence.decode('utf-8')
        except UnicodeDecodeError:  #如果在decode的过程中出现了错误，那么就解码成gbk形式的。
            sentence = sentence.decode('gbk','ignore')
    '''
        \u4E00-\u9FA5a-zA-Z0-9+#&\._ : All non-space characters. Will be handled with re_han
        \r\n|\s : whitespace characters. Will not be Handled. 
    ''' 
	#下面使用到了Python中正则表达式模块re,正则表达式是用于处理字符串的强大工具，
	#拥有自己独特的语法以及一个独立的处理引擎，效率上可能不如str自带的方法，但功能十分强大。
	#其最擅长的、最被大家使用的就是字符串匹配功能
	#正则表达式的大致匹配过程是：依次拿出表达式和文本中的字符比较，如果每一个字符都能匹配，则匹配成功；
	#一旦有匹配不成功的字符则匹配失败。如果表达式中有量词或边界，这个过程会稍微有一些不同

	#re.compile 可以把正则表达式编译成一个正则表达式对象。
	#可以把那些经常使用的正则表达式编译成正则表达式对象，这样可以提高一定的效率
	#关于正则表达式指南：http://www.cnblogs.com/huxi/archive/2010/07/04/1771073.html
	#关于正则表达式常用的处理函数：http://www.cnblogs.com/sevenyuan/archive/2010/12/06/1898075.html

    re_han, re_skip = re.compile(ur"([\u4E00-\u9FA5a-zA-Z0-9+#&\._]+)", re.U), re.compile(ur"(\r\n|\s)", re.U)
	#将以下正则表达式编译成re_han对象：
	#\u4E00-\u9FA5是所有汉字的编码，a-zA-Z是所有大小写字母的编码，0-9是所有阿拉伯数字的编码，+#&\._其余字符
	#将以下正则表达式编译成re_skip对象：
	#\r是回车，\n是换行，|或， \s是空白字符（\r\n\t\f\v）
	#re.U是使得compile的结果取决于unicode定义的字符属性
    if cut_all:
        re_han, re_skip = re.compile(ur"([\u4E00-\u9FA5]+)", re.U), re.compile(ur"[^a-zA-Z0-9+#\n]", re.U)
		#全模式下，只handle中文字符部分[\u4E00-\u9FA5]，其余的字母、数字等等都要跳过。
    blocks = re_han.split(sentence)
	#这个split的作用和Python自带的split作用差不多，按照能够匹配的子串，即刚才compile进去的哪些字符，将sentence分割
	#而且是把sentence分成几个blocks，然后针对每一块进行分词
	'''
	例子：
	p = re.compile(r'\d+')
	print p.split('one1two2three3four4')
 
	### output ###
	# ['one', 'two', 'three', 'four', '']  #按照compile的表达式，把一个string分成几个blocks
	'''
    if HMM:  #默认情况使用__cut_DAG作为分词函数，为每个block分词
        cut_block = __cut_DAG
    else:
        cut_block = __cut_DAG_NO_HMM
    if cut_all:
        cut_block = __cut_all

    for blk in blocks:  #针对每一块进行分词
        if len(blk)==0:  #当blk为空时，继续下一个blk的分词
            continue
        if re_han.match(blk):  #如果re_han.match(blk)为true，说明当前blk是re_han范围内的文本
            for word in cut_block(blk):  #__cut_DAG作为分词函数为每个block分词，返回分词后word列表
                yield word  #yield作用也是返回，但是其不会消除local variables。
				#yield statement is what makes a function a generator function.
				#具体的yield和return在Python中的区别：http://blog.csdn.net/jiyanfeng1/article/details/8148561
        else:
            tmp = re_skip.split(blk)  #按照re_skip中的正则表达式，把blk划分成几块
            for x in tmp:
                if re_skip.match(x):
                    yield x  #返回匹配划分的每一部分
                elif not cut_all:  #如果是精准模式，则返回x中的每一项，即试图将句子最精确地切开
                    for xx in x:
                        yield xx
                else:  #如果是全模式，把句子中所有的可以成词的词语都扫描出来
                    yield x
```









