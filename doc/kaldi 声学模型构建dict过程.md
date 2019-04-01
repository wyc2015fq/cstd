# kaldi 声学模型构建dict过程

2017年02月09日 16:40:15 [quheDiegooo](https://me.csdn.net/quheDiegooo) 阅读数：1779



kaldi声学模型训练，首先需要数据准备，

在数据准备阶段需要准备标注数据的发音，处理步骤如下：

1.首先是把训练数据和测试数据对应的标注（text）合成一个文件到text_all，同时去掉其他非字母和数字的字符串

  cat $traindatadir/text$testdatadir/text| perl -pe"s/^[\w\-]+\s+//">$dictdir/text_all

2.然后就是根据准备好的发音字典（sum_dict.processed.lex）对第一步的标注数据进行处理，得到sum_dict.lex 带声调

  local/update_lex.pl -i ./conf/sum_dict.processed.lex-t$dictdir/text_all-o$dictdir/sum_dict.lex

3.最后，根据第二步骤生成的文件，来生成字典，音素等文件，放到字典文件夹下（$dictdir）

​    local/prepare_dict.sh $dictdir/sum_dict.lex$dictdir1

生成之后查看字典文件夹下面的文件，有如下文件：

1) words.txt   为训练数据和测试数据分词之后，和起来的所有去重之后的词

2) lexicon.txt  根据sum_dict.lex，生成word对应的发音字典文件，对应格式为 word phone phone phone...

3) silence_phones.txt  静音音素

4)optional_silence.txt  一个单独的音素，用来作为字典中默认的静音音素

5)phones.txt  根据lexicon生成的所有的音素集合

6)nonsilence_phones.txt  非静音音素

7)extra_questions.txt   用于构建决策树的问题集，由静音音素和非静音音素生成

还有sum_dict.lex  text_all  text_all_word 

和lexiconp.txt 带概率的发音字典文件，格式为word prob phone phone phone...