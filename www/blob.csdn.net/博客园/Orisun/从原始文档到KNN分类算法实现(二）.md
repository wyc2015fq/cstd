# 从原始文档到KNN分类算法实现(二） - Orisun - 博客园







# [从原始文档到KNN分类算法实现(二）](https://www.cnblogs.com/zhangchaoyang/articles/2165482.html)





1.复旦语料库train集中有6个类是多于1000个文档的，把它们取出来，分别删减至1000个文档，构成接下来我们要进行实验的训练集。
2.执行Predeal.java，它依次进行三项操作：把文件重命名为顺序的数字编号;把文件编号（即文件名）写入文档开头，用一个空格与正文内容隔开，去除正文每行末的换行符，把整个文档合并为一行，去除正文行间的空格;用ICTCLAS进行中文分词.由于一次运行对6000个文件进行分词程序会中断退出。所以只好一次只处理1000个文件。
这里去除停用词的方法我多说一句，程序中的代码是if (word.length()>=2 && (!Predeal.stopwords.contains(word)))，即我们不统计单个的数字、字母和汉字，也可避免把空格统计进去，所以用停用词表中全部是长度大于1的词语，并且根据语料库的情况多加了一些词，比如“期号、出处、原刊、原文、正文、标题”等。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Sep 2, 2011
  4  * FileName: Predeal.java
  5  * Function: 去除每行末的换行符，把整个文档合并为一行，去除行间的空格；用ICTCLAS进行中文分词；把文件重命名为顺序的数字编号，把该编号写入文档开头，用空格与正文内容隔开。
  6  */
  7 
  8 import java.io.BufferedReader;
  9 import java.io.BufferedWriter;
 10 import java.io.File;
 11 import java.io.FileReader;
 12 import java.io.FileWriter;
 13 import java.io.IOException;
 14 import java.util.HashSet;
 15 
 16 import ICTCLAS.I3S.AC.ICTCLAS50;
 17 
 18 public class Predeal {
 19     //文档顺序编号
 20     public static int seqnum=0;
 21     // 停用词集合
 22     public static HashSet<String> stopwords = new HashSet<String>();
 23 
 24     // 初始化停用词表
 25     public static void init_SW(String filePath) {
 26         File swFile = new File(filePath);
 27         if(!swFile.exists()){
 28             System.out.println("文件不存在，程序退出.");
 29             System.exit(2);
 30         }
 31         try {
 32             FileReader fr = new FileReader(swFile);
 33             BufferedReader br = new BufferedReader(fr);
 34             String word = null;
 35             while ((word = br.readLine()) != null) {
 36                 stopwords.add(word);
 37             }
 38             br.close();
 39         } catch (IOException exp) {
 40             System.out.println("读取停用词时发生异常：" + exp.getMessage());
 41         }
 42     }
 43     
 44     //把文件重命名为顺序的数字编号
 45     public void renameDoc(File srcFile){
 46         if(srcFile.isDirectory()){
 47             File[] childFiles=srcFile.listFiles();
 48             for(File child:childFiles){
 49                 renameDoc(child);
 50             }
 51         }
 52         else if(srcFile.isFile()){
 53             String newname=String.valueOf(seqnum);            
 54             if(!srcFile.renameTo(new File(srcFile.getParent(),newname))){
 55                 System.out.println("文件重命名失败，程序退出。");
 56                 System.exit(2);
 57             }
 58             seqnum++;
 59         }
 60     }
 61     //把文件编号（即文件名）写入文档开头，用一个空格与正文内容隔开。去除正文每行末的换行符，把整个文档合并为一行，去除正文行间的空格.
 62     public void delWrapAndSpace(File srcFile){
 63         if(srcFile.isDirectory()){
 64             File[] childFiles=srcFile.listFiles();
 65             for(File child:childFiles){
 66                 delWrapAndSpace(child);
 67             }
 68         }
 69         else if(srcFile.isFile()){
 70             String filename=srcFile.getName();
 71             //文档开头写入文件名加空格
 72             StringBuffer content=new StringBuffer(filename+" ");
 73             try{
 74                 FileReader fr=new FileReader(srcFile);
 75                 BufferedReader br=new BufferedReader(fr);
 76                 String line;
 77                 while((line=br.readLine())!=null){        //readLine()并不读取末尾的换行符
 78                     content.append(line.replaceAll("\\s+",""));        //StringBuffer会自动扩容.同时去除行间的空格
 79                 }
 80                 br.close();
 81                 FileWriter fw=new FileWriter(srcFile);
 82                 BufferedWriter bw=new BufferedWriter(fw);
 83                 bw.write(content.toString());
 84                 bw.flush();
 85                 bw.close();
 86             }catch (Exception e){
 87                 e.printStackTrace();
 88             }
 89         }
 90     }
 91 
 92     //用ICTCLAS进行中文分词.一次运行对6000个文件进行分词程序会中断退出。所以只好一次只处理1000个文件。
 93     public void wordSeg(File srcFile) {
 94         if (srcFile.isDirectory()) {
 95             File[] childFiles = srcFile.listFiles();
 96             for (File child : childFiles) {
 97                 wordSeg(child);
 98             }
 99         } else if (srcFile.isFile()) {
100             try {
101                 ICTCLAS50 ictc = new ICTCLAS50();
102                  // 指定Configure.xml和Data directory的存储位置
103                 String argu = "/home/orisun/master/ICTCLAS50_Linux_RHAS_32_JNI/API";
104                 if (ictc.ICTCLAS_Init(argu.getBytes("GB2312")) == false) { // UTF-8
105                     System.out.println("Init Fail!");
106                     return;
107                 }
108                 // 这里的参数“3”指定了待分词的文件是UTF-8编码
109                 if (ictc.ICTCLAS_FileProcess(srcFile.getAbsolutePath()
110                         .getBytes(), 3, 0, srcFile.getAbsolutePath().getBytes()) == false) {
111                     System.out.println(srcFile + "文件分词失败.");
112                     return;
113                 }
114                 ictc.ICTCLAS_Exit(); // 释放分词组件资源
115             } catch (Exception e) {
116                 e.printStackTrace();
117             }
118         }
119     }
120     
121     public static void main(String[] args){
122         Predeal predeal=new Predeal();
123         //训练文本集
124         File train=new File("/home/orisun/train");
125         if(!train.exists()){
126             System.out.println("文件不存在，程序退出.");
127             System.exit(2);
128         }
129         predeal.renameDoc(train);
130         predeal.delWrapAndSpace(train);
131         predeal.wordSeg(new File(train,"C19"));    
132         predeal.wordSeg(new File(train,"C31"));
133         predeal.wordSeg(new File(train,"C32"));
134         predeal.wordSeg(new File(train,"C33"));
135         predeal.wordSeg(new File(train,"C38"));
136         predeal.wordSeg(new File(train,"C39"));
137     }
138 }
```

3.WordDocMatrix.java使用Hadoop的MapReduce技术，生成word-doc矩阵。在map阶段进行一项预处理--去除停用词，并报告一条“单词--文件名"。在reduce阶段统计每个单词在各个文档中出现的次数，当单词在至少4个文档中出现时把它记入文件matrix/part-r-0000.
由于采用了hadoop技术不能在Eclipse里面直接运行，具体方法是：
（1）进行工程的bin文件夹
    cd ~/workspace/TextClassify/bin
（2）对所有class文件进行打包
    jar -cvf ~/WordDocMatrix.jar *.class
（3）把用户根目录下的matrix文件删除，把~/train下面的6个文件夹下面的所有文本文件移动到~/train下，删除6个分类文件夹。
（4）回到用户根目录，使用hadoop运行jar包
    cd ~
    hadoop jar WordDocMatrix.jar WordDocMatrix
由于是在单机上采用standalone模式运行hadoop，所以执行时间特别长，大约用了310分钟（1G内存），生成的part-r-0000文件有44222行，这也就是说在训练集中去除食用词后有44222个单词至少在4篇文本中出现过，接下来我们要计算这44222个单词的增益值，并对其进行排序。
当然不采用hadoop行动时间肯定会大大缩短，但在实际中由于word-doc矩阵非常大，程序运行一两分钟后就内存溢出了。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


 1 /**
 2  * Author: Orisun 
 3  * Date: Sep 2, 2011 
 4  * FileName: WordDocMatrix.java 
 5  * Function: 使用Hadoop的MapReduce技术，生成word-doc矩阵。在map阶段进行一项预处理--去除停用词，并报告一条“单词--文件名"。在reduce阶段统计每个单词在各个文档中出现的次数，当单词在至少4个文档中出现时把它记入文件。
 6  */
 7 import java.io.IOException;
 8 import java.util.ArrayList;
 9 import java.util.HashSet;
10 
11 import org.apache.hadoop.fs.Path;
12 import org.apache.hadoop.io.LongWritable;
13 import org.apache.hadoop.io.Text;
14 import org.apache.hadoop.mapreduce.Job;
15 import org.apache.hadoop.mapreduce.Mapper;
16 import org.apache.hadoop.mapreduce.Reducer;
17 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
18 import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
19 
20 public class WordDocMatrix {
21 
22     public static int docnum = 6000; // 训练文档总数
23     public static int clanum = 6; // 训练分类总数
24 
25     static class WordDocMatrixMapper extends
26             Mapper<LongWritable, Text, Text, Text> {
27         public void map(LongWritable key, Text value, Context context)
28                 throws IOException, InterruptedException {
29             // 经过Predeal后，一行就是一个文档的全部内容
30             String line = value.toString();
31             String[] words = line.split("\\s+");
32             String fname = words[0];
33             for (int i = 1; i < words.length; i++) {
34                 String word = words[i];
35                 //不统计单个的数字、字母和汉字，也可避免把空格统计进去
36                 if (word.length()>=2 && (!Predeal.stopwords.contains(word))) { // 去除停用词
37                     context.write(new Text(word), new Text(fname));
38                 }
39             }
40         }
41     }
42 
43     static class WordDocMatrixReducer extends Reducer<Text, Text, Text, Text> {
44         public void reduce(Text key, Iterable<Text> files,Context context) throws IOException, InterruptedException {
45             ArrayList<Integer> al = new ArrayList<Integer>(docnum);
46             for (int i = 0; i < docnum; i++)
47                 al.add(0);
48             HashSet<Integer> set=new HashSet<Integer>();
49             boolean flag=false;
50             for (Text file : files) {
51                 int index = Integer.parseInt(file.toString());
52                 int ori = al.get(index);
53                 al.set(index, ori + 1);
54                 if(!flag){
55                     set.add(index);
56                     if(set.size()>3){
57                         flag=true;
58                     }
59                 }
60             }        
61             if(flag){
62                 StringBuffer count = new StringBuffer();
63                 for(int i=0;i<al.size();i++){
64                     count.append(al.get(i)+" ");
65                 }
66                 context.write(key, new Text(count.toString()));
67             }
68         }
69     }
70 
71     public static void main(String[] args) throws Exception {
72         Predeal.init_SW("/home/orisun/master/StopWordTable_ZH");
73         Path inputPath=new Path("/home/orisun/train");
74         Path outputPath=new Path("/home/orisun/matrix");
75         Job job = new Job();
76         job.setJarByClass(WordDocMatrix.class);
77         FileInputFormat.addInputPath(job, inputPath);
78         FileOutputFormat.setOutputPath(job, outputPath);
79         job.setMapperClass(WordDocMatrixMapper.class);
80         job.setReducerClass(WordDocMatrixReducer.class);
81         job.setOutputKeyClass(Text.class);
82         job.setOutputValueClass(Text.class);
83         System.exit(job.waitForCompletion(true) ? 0 : 1);
84     }
85 }
```

4.运行FeatureSelect.java，计算每个单词的信息增益值，选择IG大的作为特征项。如果选用HashMap来存储单词及它对应的IG值，很快会内存溢出，所以我选择使用BerkelyDB来存储。
BerkelyDB是一个嵌入式数据库，它适合于管理海量的、简单的数据。Key/Value是BerkelyDB进行数据管理的基础。BerkelyDB底层实现采用B树，可以看成能够存储海量数据的HashMap。
程序计算了44222个单词的IG值，并把IG最大的300个特征项输出到文件，总共也只用了几分钟，确实挺快的！

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Sep 3, 2011
  4  * FileName: FeatureSelect.java
  5  * Function: 读取word-doc矩阵，计算每个词的信息增益值，排序。输出IG最大的前300个特征项。
  6  */
  7 
  8 import java.io.BufferedReader;
  9 import java.io.BufferedWriter;
 10 import java.io.File;
 11 import java.io.FileNotFoundException;
 12 import java.io.FileReader;
 13 import java.io.FileWriter;
 14 import java.util.ArrayList;
 15 import java.util.Collections;
 16 import java.util.Comparator;
 17 import java.util.Iterator;
 18 import java.util.Map;
 19 import java.util.Map.Entry;
 20 
 21 import com.sleepycat.bind.EntryBinding;
 22 import com.sleepycat.bind.serial.SerialBinding;
 23 import com.sleepycat.bind.serial.StoredClassCatalog;
 24 import com.sleepycat.collections.StoredMap;
 25 import com.sleepycat.je.Database;
 26 import com.sleepycat.je.DatabaseConfig;
 27 import com.sleepycat.je.DatabaseException;
 28 import com.sleepycat.je.Environment;
 29 import com.sleepycat.je.EnvironmentConfig;
 30 
 31 public class FeatureSelect {
 32     private Environment env;
 33     protected Database database; // 用来存放url队列的数据库
 34     protected Database catalogdatabase; // 用来创建StoredClassCatalog实例的数据库
 35     private static final String CLASS_CATALOG = "java_class_catalog"; // catalogdatabase的数据库名
 36     protected StoredClassCatalog javaCatalog; // StoredClassCatalog实例用来序列化对象
 37     StoredMap<String, Double> FeaDB = null;
 38 
 39     public FeatureSelect(String homeDirectory) throws DatabaseException,
 40             FileNotFoundException {
 41         EnvironmentConfig envConfig = new EnvironmentConfig(); // 环境配置
 42         envConfig.setTransactional(true); // 允许事务
 43         envConfig.setAllowCreate(true); // 当环境配置不存在时就创建
 44         env = new Environment(new File(homeDirectory), envConfig); // 创建环境
 45 
 46         DatabaseConfig dbConfig0 = new DatabaseConfig(); // 数据库配置
 47         dbConfig0.setTransactional(true); // 允许事务
 48         dbConfig0.setAllowCreate(true); // 当数据库不存在时就创建
 49         catalogdatabase = env.openDatabase(null, CLASS_CATALOG, dbConfig0);
 50         javaCatalog = new StoredClassCatalog(catalogdatabase);
 51         
 52         DatabaseConfig dbConfig = new DatabaseConfig(); // 数据库配置
 53         dbConfig.setTransactional(true); // 允许事务
 54         dbConfig.setAllowCreate(true); // 当数据库不存在时就创建
 55         database = env.openDatabase(null, "URL", dbConfig); // 打开数据库
 56         
 57         EntryBinding<String> keyBinding = new SerialBinding<String>(
 58                 javaCatalog, String.class);
 59         EntryBinding<Double> valueBinding = new SerialBinding<Double>(
 60                 javaCatalog, Double.class);
 61         FeaDB = new StoredMap<String, Double>(database, keyBinding,
 62                 valueBinding, true);
 63     }
 64 
 65     public void close() throws DatabaseException {
 66         database.close(); // 关闭存放url的数据库
 67         javaCatalog.close(); // 关闭用来序列化对象的javaCatalog类
 68         env.close(); // 关闭环境
 69     }
 70 
 71     public void calIG(File matrixFile) {
 72         if (!matrixFile.exists()) {
 73             System.out.println("Matrix文件不存在.程序退出.");
 74             System.exit(2);
 75         }
 76 
 77         double entropy = Math.log(WordDocMatrix.clanum);
 78         try {
 79             FileReader fr = new FileReader(matrixFile);
 80             BufferedReader br = new BufferedReader(fr);
 81             String line = null;
 82             while ((line = br.readLine()) != null) {
 83                 String[] content = line.split("\\s+");
 84                 String word = content[0];
 85                 ArrayList<Short> al = new ArrayList<Short>(WordDocMatrix.docnum);
 86                 for (int i = 0; i < WordDocMatrix.docnum; i++) {
 87                     short count = Short.parseShort(content[i + 1]);
 88                     al.add(count);
 89                 }
 90                 int category = WordDocMatrix.docnum / WordDocMatrix.clanum;
 91                 int wcount = 0; // 出现word的文档的文档数量
 92                 int[] wcount_class = new int[WordDocMatrix.clanum];// 每个类别中出现单词word的文档数
 93                 double pw = 0.0; // 出现word的文档占全部文档的比重
 94                 double[] pcw = new double[WordDocMatrix.clanum]; // 在单词word出现时各个类别所占的比重
 95                 double[] pcw_b = new double[WordDocMatrix.clanum]; // 在单词word不出现时各个类别所占的比重
 96                 for (int i = 0; i < WordDocMatrix.clanum; i++) {
 97                     for (int j = 0; j < category; j++) {
 98                         if (al.get(j + i * category) > 0) {
 99                             wcount_class[i]++;
100                         }
101                     }
102                     wcount += wcount_class[i];
103                 }
104                 pw = 1.0 * wcount / WordDocMatrix.docnum;
105                 for (int i = 0; i < WordDocMatrix.clanum; i++) {
106                     pcw[i] = 1.0 * wcount_class[i] / wcount;
107                     pcw_b[i] = 1.0 * (category - wcount_class[i])
108                             / (WordDocMatrix.docnum - wcount);
109                 }
110                 double d1 = 0.0;
111                 double d2 = 0.0;
112                 for (int i = 0; i < WordDocMatrix.clanum; i++) {
113                     d1 += pcw[i] * Math.log(pcw[i] + Double.MIN_VALUE);
114                     d2 += pcw_b[i] * Math.log(pcw_b[i] + Double.MIN_VALUE);
115                 }
116                 double ig = entropy + pw * d1 + (1.0 - pw) * d2;
117                 FeaDB.put(word, ig);
118             }
119         } catch (Exception e) {
120             e.printStackTrace();
121         }
122     }
123 
124     // Map按value进行排序(从大到小)
125     public ArrayList<Entry<String, Double>> sort() {
126         ArrayList<Entry<String, Double>> al = new ArrayList<Entry<String, Double>>();
127         //从数据库中读取数据
128         if(!FeaDB.isEmpty()){
129             Iterator<Entry<String,Double>> iter=FeaDB.entrySet().iterator();
130             while(iter.hasNext()){
131                 Entry<String,Double> entry=iter.next();
132                 al.add(entry);
133             }
134         }
135         Collections.sort(al, new Comparator<Map.Entry<String, Double>>() {
136             public int compare(Map.Entry<String, Double> o1,
137                     Map.Entry<String, Double> o2) {
138                 double res = o2.getValue() - o1.getValue();
139                 if (res < 0)
140                     return -1;
141                 else if (res > 0)
142                     return 1;
143                 else
144                     return 0;
145             }
146         });
147         return al;
148     }
149 
150     public static void main(String[] args) throws Exception{
151         FeatureSelect fs = new FeatureSelect("/home/orisun/develop/workspace");
152         fs.calIG(new File("/home/orisun/matrix/part-r-00000"));
153         ArrayList<Entry<String, Double>> al = fs.sort();
154         fs.close();
155         Iterator<Entry<String, Double>> iter = al.iterator();
156         int n = 0;
157         File file = new File("/home/orisun/features");
158         try {
159             file.createNewFile();
160             FileWriter fw = new FileWriter(file);
161             BufferedWriter bw = new BufferedWriter(fw);
162             while (iter.hasNext() && n++ < 300) {
163                 Entry<String, Double> entry = iter.next();
164                 bw.write(entry.getKey() + "\t");
165                 bw.write(String.valueOf(entry.getValue()));
166                 bw.newLine();
167             }
168             bw.flush();
169             bw.close();
170         } catch (Exception e) {
171             e.printStackTrace();
172         }
173     }
174 }
```

遇到内存溢出时我分别采取了两种解决方案：Hadoop和Berkely DB。由于Berkely DB出乎意料地快，而Hadoop独立模式又灰常地慢，所以可以考虑把WordDocMatrix.java采用Berkely DB来实现。



![](https://pic002.cnblogs.com/images/2011/103496/2011092018262252.png)















