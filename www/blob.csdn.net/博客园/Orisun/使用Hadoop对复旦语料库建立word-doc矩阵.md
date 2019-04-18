# 使用Hadoop对复旦语料库建立word-doc矩阵 - Orisun - 博客园







# [使用Hadoop对复旦语料库建立word-doc矩阵](https://www.cnblogs.com/zhangchaoyang/articles/2233323.html)





使用信息增益法选择特征项时需要建立word-doc矩阵。

由于预处理后的测试集有7196个文档，按照常规方法内存根本不够用，所以决定采用Hadoop的standalone模式。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


/**
 * Author: Orisun
 * Date: Sep 5, 2011
 * FileName: WordDocMatrix.java
 * Function: 建立word-doc矩阵
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class WordDocMatrix {
    static HashSet<String> stopwords = new HashSet<String>();// 停用词集合
    // 初始化停用词表
    public static void init_SW(String filePath) {
        stopwords.clear();
        File swFile = new File(filePath);
        if(!swFile.exists()){
            System.out.println("文件不存在，程序退出.");
            System.exit(2);
        }
        try {
            FileReader fr = new FileReader(swFile);
            BufferedReader br = new BufferedReader(fr);
            String word = null;
            while ((word = br.readLine()) != null) {
                stopwords.add(word);
            }
            br.close();
        } catch (IOException exp) {
            System.out.println("读取停用词时发生异常：" + exp.getMessage());
        }
    }
    static class WordDocMatrixMapper extends Mapper<LongWritable, Text, Text, Text> {
        public void map(LongWritable key, Text value, Context context)
                throws IOException, InterruptedException {
            // 经过Predeal后，一行就是一个文档的全部内容
            String line = value.toString();
            String[] words = line.split("\\s+");
            String global_rank = words[0];        //读取文档的全局编号
            for (int i = 1; i < words.length; i++) {
                String word = words[i];
                //不统计以字母数字和标点符号开头的词，以及除停用词
                if(word.length()>1 && word.matches("^[^\\w|\\pP].*") && !stopwords.contains(word))
                    context.write(new Text(word), new Text(global_rank));
            }
        }
    }

    static class WordDocMatrixReducer extends Reducer<Text, Text, Text, Text> {
        int doc_num=7196;    //训练集中包含的文档总数
        public void reduce(Text key, Iterable<Text> ranks,Context context) throws IOException, InterruptedException {
            ArrayList<Integer> al = new ArrayList<Integer>(doc_num);
            for (int i = 0; i < doc_num; i++)
                al.add(0);
            HashSet<Integer> set=new HashSet<Integer>();
            boolean flag=false;
            for (Text rank : ranks) {
                int index = Integer.parseInt(rank.toString());
                int ori = al.get(index);
                al.set(index, ori + 1);
                if(!flag){
                    set.add(index);
                    if(set.size()>0){
                        flag=true;
                    }
                }
            }        
            if(flag){
                StringBuffer count = new StringBuffer();
                for(int i=0;i<al.size();i++){
                    count.append(al.get(i)+" ");
                }
                context.write(key, new Text(count.toString()));
            }
        }
    }

    public static void main(String[] args) throws Exception {
        init_SW("/home/orisun/master/StopWord2");
        //不支持嵌套,但输入路径可以有多个
        Path inputPath1=new Path("/home/orisun/master/fudan_corpus/train_seg/C3-Art_ws");    
        Path inputPath2=new Path("/home/orisun/master/fudan_corpus/train_seg/C7-History_ws");
        Path inputPath3=new Path("/home/orisun/master/fudan_corpus/train_seg/C11-Space_ws");
        Path inputPath4=new Path("/home/orisun/master/fudan_corpus/train_seg/C19-Computer_ws");
        Path inputPath5=new Path("/home/orisun/master/fudan_corpus/train_seg/C31-Enviornment_ws");
        Path inputPath6=new Path("/home/orisun/master/fudan_corpus/train_seg/C32-Agriculture_ws");
        Path inputPath7=new Path("/home/orisun/master/fudan_corpus/train_seg/C34-Economy_ws");
        Path inputPath8=new Path("/home/orisun/master/fudan_corpus/train_seg/C38-Politics_ws");
        Path inputPath9=new Path("/home/orisun/master/fudan_corpus/train_seg/C39-Sports_ws");
        //输出路径在程序运行前不能存在
        Path outputPath=new Path("/home/orisun/matrix");    
        Job job = new Job();
        job.setJarByClass(WordDocMatrix.class);
        FileInputFormat.addInputPath(job, inputPath1);
        FileInputFormat.addInputPath(job, inputPath2);
        FileInputFormat.addInputPath(job, inputPath3);
        FileInputFormat.addInputPath(job, inputPath4);
        FileInputFormat.addInputPath(job, inputPath5);
        FileInputFormat.addInputPath(job, inputPath6);
        FileInputFormat.addInputPath(job, inputPath7);
        FileInputFormat.addInputPath(job, inputPath8);
        FileInputFormat.addInputPath(job, inputPath9);
        FileOutputFormat.setOutputPath(job, outputPath);
        job.setMapperClass(WordDocMatrixMapper.class);
        job.setReducerClass(WordDocMatrixReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

这里展示一下我的停用词表：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


期号
页号
关键词
出处
原刊
原文
正文
标题
Abstract
哎呀
哎哟
俺们
按照
吧哒
罢了
本着
比方
比如
鄙人
彼此
别的
别说
并且
不比
不成
不单
不但
不独
不管
不光
不过
不仅
不拘
不论
不怕
不然
不如
不特
不惟
不问
不只
朝着
趁着
除此之外
除非
除了
此间
此外
从而
但是
当着
的话
等等
叮咚
对于
多少
而况
而且
而是
而外
而言
而已
尔后
反过来
反过来说
反之
非但
非徒
否则
复印
嘎登
各个
各位
各种
各自
根据
故此
固然
关于
果然
果真
哈哈
何处
何况
何时
哼唷
呼哧
还是
还有
换句话说
换言之
或是
或者
极了
及其
及至
即便
即或
即令
即若
即使
几时
既然
既是
继而
加之
假如
假若
假使
鉴于
较之
接着
结果
紧接着
进而
尽管
经过
就是
就是说
具体地说
具体说来
开始
开外
可见
可是
可以
况且
来着
例如
连同
两者
另外
另一方面
慢说
漫说
每当
莫若
某个
某些
哪边
哪儿
哪个
哪里
哪年
哪怕
哪天
哪些
哪样
那边
那儿
那个
那会儿
那里
那么
那么些
那么样
那时
那些
那样
乃至
你们
宁可
宁肯
宁愿
啪达
旁人
凭借
其次
其二
其他
其它
其一
其余
其中
起见
起见
岂但
恰恰相反
前后
前者
然而
然后
然则
人家
任何
任凭
如此
如果
如何
如其
如若
如上所述
若非
若是
上下
尚且
设若
设使
甚而
甚么
甚至
省得
时候
什么
什么样
使得
是的
首先
谁知
顺着
似的
虽然
虽说
虽则
随着
所以
他们
他人
它们
她们
倘或
倘然
倘若
倘使
通过
同时
万一
为何
为了
为什么
为着
嗡嗡
我们
呜呼
乌乎
无论
无宁
毋宁
相对而言
向着
沿着
要不
要不然
要不是
要么
要是
也罢
也好
一般
一旦
一方面
一个
一来
一切
一样
一则
依照
以便
以及
以免
以至
以至于
以致
抑或
因此
因而
因为
由此可见
由于
有的
有关
有些
于是
于是乎
与此同时
与否
与其
越是
云云
再说
再者
在下
咱们
怎么
怎么办
怎么样
怎样
照着
这边
这儿
这个
这会儿
这就是说
这里
这么
这么点儿
这么些
这么样
这时
这些
这样
正如
之类
之所以
之一
只是
只限
只要
只有
至于
诸位
自从
自个儿
自各儿
自己
自家
自身
综上所述
总的来看
总的来说
总的说来
总而言之
总之
纵令
纵然
纵使
遵照
作为
```

hadoop不能直接在Eclipse里运行，运行方法是：

把生成的class文件打成jar包：jar  -cvf  WDM.jar  *.class

然后运行hadoop  jar WDM.jar  WordDocMatrix

在我的机子上运行6小时9分钟，生成的矩阵文件占了4.4G的磁盘空间，我用vim都打不开。使用wc -l part-r-00000试图窥测文件有多少行，等待了N久没有出结果。不过后来我我单独计算每个term的信息增益值存一个文件（9.8M），打开发现里面有325591个互异的term，这个数字也就是矩阵的行数。列数是全部文档的个数--7196。












