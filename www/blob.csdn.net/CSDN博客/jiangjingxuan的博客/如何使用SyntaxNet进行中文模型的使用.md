# 如何使用SyntaxNet进行中文模型的使用 - jiangjingxuan的博客 - CSDN博客





2017年02月07日 09:30:47[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2166









使用Chinese模型

1.在网上下载Chinese模型文件，网址：[http://download.tensorflow.org/models/parsey_universal/Chinese.zip](http://download.tensorflow.org/models/parsey_universal/Chinese.zip)

2.上传模型文件，路径为：


/xy/models/syntaxnet


/。解压文件

3.更改/xy/models/syntaxnet/syntaxnet/models/parsey_universal目录下的parse.sh  tokenize.sh文件，主要改里面的模型路径。

4.在 /xy/models/syntaxnet目录下运行以下命令


`MODEL_DIRECTORY=/xy/models/syntaxnet/Chinese`

echo '然而,這樣的處理也衍生了一些問題.' | \

syntaxnet/models/parsey_universal/tokenize_zh.sh $MODEL_DIRECTORY | \

`syntaxnet/models/parsey_universal/parse.sh $MODEL_DIRECTORY`





