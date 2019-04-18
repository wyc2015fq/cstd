# 从原始文档到KNN分类算法实现（一） - Orisun - 博客园







# [从原始文档到KNN分类算法实现（一）](https://www.cnblogs.com/zhangchaoyang/articles/2162393.html)




- 
建立原始文档
~/corpus/C1下建三个文件：0,1,2。内容分别为：
|0|眼睛 明亮 健康 身体 发达|
|----|----|
|1|高大 身材 胳膊 勇猛 四肢|
|2|胳膊 健康 身体 高大 健康|

~/corpus/C2下建三个文件：3,4,5。内容分别为：
|3|美国 很 发达|
|----|----|
|4|美国 经济 富强|
|5|日本 富强 科技 发达|

- 
新建文件夹~/frequency，运行程序WordCount.java。该程序的输入文件是~/corpus，运行结束后在~/frequency下产生了6个新文件：0—5,分别记录了原始文档中每个词出现的次数。

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


 1 /**
 2  * Author: Orisun
 3  * Date: Aug 29, 2011
 4  * FileName: WordCount.java
 5  * Function: calculate word frequency in each document.
 6  */
 7 package featureselect;
 8 
 9 import java.io.BufferedReader;
10 import java.io.BufferedWriter;
11 import java.io.File;
12 import java.io.FileReader;
13 import java.io.FileWriter;
14 import java.util.HashMap;
15 import java.util.Iterator;
16 import java.util.Map.Entry;
17 
18 public class WordCount {
19 
20     //参数文件是（经过预处理的）原始训练文档集所在的文件夹
21     public void wordCount(File srcFile) {
22         if (srcFile.isDirectory()) {
23             File[] children = srcFile.listFiles();
24             for (File child : children) {
25                 wordCount(child);
26             }
27         } else if (srcFile.isFile()) {
28             HashMap<String,Integer> wordfreq=new HashMap<String,Integer>();        //存储单词出现的次数
29             try {
30                 FileReader fr = new FileReader(srcFile);
31                 BufferedReader br = new BufferedReader(fr);
32                 String line=null;
33                 while((line=br.readLine())!=null){
34                     String[] words=line.split("\\s+");
35                     for(String word:words){
36                         if(!wordfreq.containsKey(word)){
37                             wordfreq.put(word, new Integer(1));
38                         }else{
39                             int n=wordfreq.get(word);
40                             wordfreq.put(word, new Integer(n+1));
41                         }
42                     }
43                 }
44                 br.close();
45                 
46                 //把词频写入新文件
47                 File newFile=new File("/home/orisun/frequency",srcFile.getName());
48                 newFile.createNewFile();
49                 FileWriter fw=new FileWriter(newFile);
50                 BufferedWriter bw=new BufferedWriter(fw);
51                 Iterator<Entry<String, Integer>> iter=wordfreq.entrySet().iterator();
52                 while(iter.hasNext()){
53                     Entry<String,Integer> entry=iter.next();
54                     bw.write(entry.getKey());
55                     bw.write("\t");
56                     bw.write(String.valueOf(entry.getValue()));
57                     bw.newLine();
58                 }
59                 bw.flush();
60                 bw.close();
61             } catch (Exception e) {
62                 e.printStackTrace();
63             }
64         }
65     }
66     
67     public static void main(String[] args){
68         WordCount inst=new WordCount();
69         File file=new File("/home/orisun/corpus");
70         if(!file.exists()){
71             System.out.println("文件不存在，程序退出.");
72             System.exit(2);
73         }
74         inst.wordCount(file);
75     }
76 }
```

- 下一步是运行WordDocMatrix.java。运行时它的输入是第2步产生的~/frequency下的六个文件，输出是一个文件~/matrix，该文件记录了每个单词在各个文档中出现的次数。
||0|1|2|3|4|5|
|----|----|----|----|----|----|----|
|发达|1|0|0|1|0|1|
|健康|1|0|2|0|0|0|
|富强|0|0|0|0|1|1|
|日本|0|0|0|0|0|1|
|四肢|0|1|0|0|0|0|
|眼睛|1|0|0|0|0|0|
|高大|0|1|1|0|0|0|
|很|0|0|0|1|0|0|
|身体|1|0|1|0|0|0|
|身材|0|1|0|0|0|0|
|明亮|1|0|0|0|0|0|
|胳膊|0|1|1|0|0|0|
|经济|0|0|0|0|1|0|
|勇猛|0|1|0|0|0|0|
|科技|0|0|0|0|0|1|
|美国|0|0|0|1|1|0|


```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Aug 29, 2011
  4  * FileName: WordDocMatrix.java
  5  * Function: word-document matrix denote each word frequency in each document.
  6  */
  7 package featureselect;
  8 
  9 import java.io.BufferedReader;
 10 import java.io.File;
 11 import java.io.FileReader;
 12 import java.io.PrintStream;
 13 import java.util.ArrayList;
 14 import java.util.HashMap;
 15 import java.util.Iterator;
 16 import java.util.Map.Entry;
 17 
 18 public class WordDocMatrix {
 19 
 20     // 训练集分为3类，每类各1000个文档
 21     public static final int docnumber = 3000; 
 22     public static final int classnumber = 3;
 23     //matrix记录各个词在各个文档中出现过几次
 24     HashMap<String, ArrayList<Short>> matrix = new HashMap<String, ArrayList<Short>>();
 25 
 26     //参数文件是文件夹，其中的文件分别存储了文档中各个单词出现的次数
 27     public void buildMatrix(File srcFile) {
 28         if (srcFile.isDirectory()) {
 29             File[] children = srcFile.listFiles();
 30             for (File child : children) {
 31                 buildMatrix(child);
 32             }
 33         } else if (srcFile.isFile()) {
 34             int filename=Integer.parseInt(srcFile.getName());
 35             try {
 36                 FileReader fr = new FileReader(srcFile);
 37                 BufferedReader br = new BufferedReader(fr);
 38                 String line=null;
 39                 while((line=br.readLine())!=null){
 40                     String[] pair=line.split("\\s+");
 41                     if(!matrix.containsKey(pair[0])){
 42                         ArrayList<Short> al=new ArrayList<Short>(docnumber);
 43                         for(int i=0;i<docnumber;i++)
 44                             al.add((short)0);
 45                         al.set(filename, Short.parseShort(pair[1]));
 46                         matrix.put(pair[0], al);
 47                     }else{
 48                         ArrayList<Short> al=matrix.get(pair[0]);
 49                         short orig=al.get(filename);
 50                         al.set(filename, (short)(orig+Short.parseShort(pair[1])));
 51                     }
 52                 }
 53                 br.close();
 54             } catch (Exception e) {
 55                 e.printStackTrace();
 56             }
 57         }
 58     }
 59     
 60     //打印矩阵的前几行，输出到文件，以作验证（如果全部打印文件会因太大而加载过慢，甚至可能打不开）
 61     public static void main(String[] args){
 62         WordDocMatrix inst=new WordDocMatrix();
 63         try{
 64             File Mfile=new File("/home/orisun/freq");
 65             if (!Mfile.exists()) {
 66                 System.out.println("文件不存在，程序退出.");
 67                 System.exit(2);
 68             }
 69             inst.buildMatrix(Mfile);
 70             File file=new File("/home/orisun/matrix");
 71             file.createNewFile();
 72             PrintStream ps=new PrintStream(file);
 73             inst.printMatrix(ps,inst.matrix);
 74             //inst.printMatrix(System.out,inst.matrix);
 75         } catch (Exception e) {
 76             e.printStackTrace();
 77         }
 78     }
 79     
 80     //输出matrix
 81     public void printMatrix(PrintStream out,HashMap<String, ArrayList<Short>> matrix){
 82         Iterator<Entry<String, ArrayList<Short>>> iter=matrix.entrySet().iterator();
 83         try{
 84         while(iter.hasNext()){
 85             Entry<String, ArrayList<Short>> entry=iter.next();
 86             out.print(entry.getKey());
 87             out.print("\t");
 88             for(int i=0;i<docnumber;i++){
 89                 out.print(String.valueOf(entry.getValue().get(i)));
 90                 out.print("\t");
 91             }
 92             out.println();
 93         }
 94         out.flush();
 95         out.close();
 96         }catch(Exception e){
 97             e.printStackTrace();
 98         }
 99     }
100 }
```

4．IG.java在整个算法过程中也不必单独运行，它只是为后面的特征词选择提供函数调用。运行时它的输入是~/frequency下的六个文件，通过调从文件读入word-doc矩阵，最后计算并输出各个词的信息增益值IG。
|发达|0.0566330123|
|----|----|
|健康|0.3182570841|
|富强|0.3182570841|
|日本|0.1323041247|
|四肢|0.1323041247|
|眼睛|0.1323041247|
|高大|0.3182570841|
|很|0.1323041247|
|身体|0.3182570841|
|身材|0.1323041247|
|明亮|0.1323041247|
|胳膊|0.3182570841|
|经济|0.1323041247|
|勇猛|0.1323041247|
|科技|0.1323041247|
|美国|0.3182570841|

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Aug 29, 2011
  4  * FileName: IG.java
  5  * Function: calculate the information gain of each word in documents
  6  */
  7 package featureselect;
  8 
  9 import java.io.BufferedReader;
 10 import java.io.File;
 11 import java.io.FileReader;
 12 import java.io.IOException;
 13 import java.util.ArrayList;
 14 import java.util.HashMap;
 15 import java.util.Iterator;
 16 import java.util.Map.Entry;
 17 
 18 public class IG {
 19 
 20     // matrix记录各个词在各个文档中出现过几次
 21     public static HashMap<String, ArrayList<Short>> matrix = new HashMap<String, ArrayList<Short>>();
 22     HashMap<String, Double> map = new HashMap<String, Double>(); // 存储每个单词的信息增益值
 23 
 24     //参数文件存储word-doc矩阵
 25     public static void initMatrix(File srcFile) {
 26         if (!(srcFile.exists() && srcFile.isFile())) {
 27             System.out.println("Matrix文件不存在.程序退出.");
 28             System.exit(2);
 29         }
 30         try {
 31             FileReader fr = new FileReader(srcFile);
 32             BufferedReader br=new BufferedReader(fr);                    
 33             String line=null;
 34             while((line=br.readLine())!=null){        
 35                 //System.out.println(line);
 36                 String[] content=line.split("\\s+");
 37                 String word=content[0];
 38                 //System.out.print(word+"\t");
 39                 ArrayList<Short> al=new ArrayList<Short>(WordDocMatrix.docnumber);
 40                 for(int i=0;i<WordDocMatrix.docnumber;i++){
 41                     short count=Short.parseShort(content[i+1]);
 42                     //System.out.print(count+"\t");
 43                     al.add(count);
 44                 }
 45                 //System.out.println();
 46                 matrix.put(word, al);
 47             }
 48             //System.out.println("word-doc矩阵的行数："+matrix.size());
 49             br.close();
 50         } catch (IOException e) {
 51             e.printStackTrace();
 52         }
 53     }
 54 
 55     //参数文件是文件夹，其中的文件分别存储了文档中各个单词出现的次数
 56     public void calIG(File srcFile) {
 57         initMatrix(new File("/home/orisun/matrix"));
 58         Iterator<Entry<String, ArrayList<Short>>> iter = matrix.entrySet().iterator();
 59         double entropy = Math.log(WordDocMatrix.classnumber);
 60         while (iter.hasNext()) {
 61             Entry<String, ArrayList<Short>> entry = iter.next();
 62             String word = entry.getKey();
 63             ArrayList<Short> al = entry.getValue();
 64             int category = WordDocMatrix.docnumber / WordDocMatrix.classnumber;
 65             int wcount = 0; // 出现word的文档的文档数量
 66             int[] wcount_class = new int[WordDocMatrix.classnumber];// 每个类别中出现单词word的文档数
 67             double pw = 0.0; // 出现word的文档占全部文档的比重
 68             double[] pcw = new double[WordDocMatrix.classnumber]; // 在单词word出现时各个类别所占的比重
 69             double[] pcw_b = new double[WordDocMatrix.classnumber]; // 在单词word不出现时各个类别所占的比重
 70             for (int i = 0; i < WordDocMatrix.classnumber; i++) {
 71                 for (int j = 0; j < category; j++) {
 72                     if (al.get(j + i * category) > 0) {
 73                         wcount_class[i]++;
 74                     }
 75                 }
 76                 wcount += wcount_class[i];
 77             }
 78             pw = 1.0 * wcount / WordDocMatrix.docnumber;
 79             for (int i = 0; i < WordDocMatrix.classnumber; i++) {
 80                 pcw[i] = 1.0 * wcount_class[i] / wcount;
 81                 pcw_b[i] = 1.0 * (category - wcount_class[i])
 82                         / (WordDocMatrix.docnumber - wcount);
 83             }
 84             double d1 = 0.0;
 85             double d2 = 0.0;
 86             for (int i = 0; i < WordDocMatrix.classnumber; i++) {
 87                 d1 += pcw[i] * Math.log(pcw[i] + Double.MIN_VALUE);
 88                 d2 += pcw_b[i] * Math.log(pcw_b[i] + Double.MIN_VALUE);
 89             }
 90             double ig = entropy + pw * d1 + (1.0 - pw) * d2;
 91             map.put(word, ig);
 92             //System.out.println(word+"\t"+ig);
 93         }
 94     }
 95     
 96     public static void main(String[] args){
 97         IG inst=new IG();
 98         File matrixFile=new File("/home/orisun/matrix");
 99         initMatrix(matrixFile);
100         //new featureselect.WordDocMatrix().printMatrix(System.out, matrix);
101         File freqFile=new File("/home/orisun/frequency");
102         inst.calIG(freqFile);
103     }
104 }
```

５．FS.java需要运行。它的输入是~/frequency下的六个文件，调用IG.java中的calIG方法，并对各单词的信息增益进行从大到小排序，最后把IG值最大的N个特征项输出到文件~/features中，~/features中FS.java运行时创建的。这里我们选择6个特征词，IG值从大到小排


- 
健康 富强 高大 身体 胳膊 美国

这也是在FS.features中的存储顺序（FS.features是ArrayList类型）。

- 
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


 1 /**
 2  * Author: Orisun
 3  * Date: Aug 29, 2011
 4  * FileName: FS.java
 5  * Function: feature select with information gain
 6  */
 7 package featureselect;
 8 
 9 import java.io.BufferedWriter;
10 import java.io.File;
11 import java.io.FileWriter;
12 import java.util.ArrayList;
13 import java.util.Collections;
14 import java.util.HashMap;
15 import java.util.Iterator;
16 import java.util.Map.Entry;
17 import java.util.Map;
18 import java.util.Comparator;
19 
20 public class FS {
21 
22     public static ArrayList<String> features=new ArrayList<String>();//存放最终选择的特征词
23     
24     //选择信息增益值最大的n个单词作为特征项
25     public void featureSelect(int n){
26         IG inst=new IG();
27         inst.calIG(new File("/home/orisun/frequency"));
28         ArrayList<Entry<String,Double>> list=sort(inst.map);
29         Iterator<Entry<String,Double>> iter=list.iterator();
30         int index=0;
31         while(index++<n && iter.hasNext()){
32             Entry<String,Double> entry=iter.next();
33             //System.out.println(entry.getKey()+"  "+entry.getValue());
34             features.add(entry.getKey());
35         }
36     }
37     
38     //Map按value进行排序
39     public ArrayList<Entry<String,Double>> sort(HashMap<String,Double> arg){
40         ArrayList<Entry<String,Double>> al=new ArrayList<Entry<String,Double>>(arg.entrySet());
41         Collections.sort(al, new Comparator<Map.Entry<String,Double>>(){
42             public int compare(Map.Entry<String, Double> o1,Map.Entry<String,Double> o2){
43                 double res=o2.getValue()-o1.getValue();
44                 if(res<0)
45                     return -1;
46                 else if(res>0)
47                     return 1;
48                 else
49                     return 0;
50         }
51         });
52         return al;
53     }
54     
55     //把最终选择的特征词存入文件
56     public static void main(String[] args){
57         FS inst=new FS();
58         inst.featureSelect(6);
59         try{    
60             File file=new File("/home/orisun/features");
61             file.createNewFile();
62             FileWriter fw=new FileWriter(file);
63             BufferedWriter bw=new BufferedWriter(fw);
64             Iterator<String> iter=FS.features.iterator();
65             while(iter.hasNext()){
66                 String feature=iter.next();
67                 bw.write(feature);
68                 bw.newLine();
69             }
70             bw.flush();
71             bw.close();
72         } catch (Exception e) {
73             e.printStackTrace();
74         }
75     }
76 }
```


6．建立文档向量模型，TDVM.java需要运行。它首先从文件~/features中读取特征项存入TDVM.features中（DVM.features是HashSet类型），此时特征项在TDVM.features中的顺序为：- 
健康富强胳膊高大美国身体

然后调用IG.initMatrix，从文件中读入word-doc矩阵，计算出特征项在文档中的权重，最终得出文档向量模型。
归一化之前6个文档向量分别为：
|1.09861228860.0 0.0 0.0 0.0 1.0986122886|0.0 0.0 1.09861228861.09861228860.0 0.0|2.19722457730.0 1.09861228861.09861228860.0 1.0986122886|0.0 0.0 0.0 0.0 1.09861228860.0|0.0 1.09861228860.0 0.0 1.09861228860.0|0.0 1.09861228860.0 0.0 0.0 0.0|
|----|----|----|----|----|----|

归一化之后6个文档向量分别为：

|0.44721359540.0 0.0 0.0 0.0 0.70710678118|0.0 0.0 0.707106781180.707106781180.0 0.0|0.89442719090.0 0.707106781180.707106781180.0 0.70710678118|0.0 0.0 0.0 0.0 0.707106781180.0|0.0 0.707106781180.0 0.0 0.707106781180.0|0.0 0.707106781180.0 0.0 0.0 0.0|
|----|----|----|----|----|----|

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Aug 29, 2011
  4  * FileName: DVM.java
  5  * Function: build document vector model for documents in train
  6  */
  7 package DVM;
  8 
  9 import java.io.BufferedReader;
 10 import java.io.BufferedWriter;
 11 import java.io.File;
 12 import java.io.FileReader;
 13 import java.io.FileWriter;
 14 import java.util.ArrayList;
 15 import java.util.HashMap;
 16 import java.util.HashSet;
 17 import java.util.Iterator;
 18 import java.util.Map.Entry;
 19 import java.util.Vector;
 20 
 21 public class TVSM {
 22 
 23     HashSet<String> features = new HashSet<String>();// 存放最终选择的特征词
 24     int len; // 特征项的个数,亦即文档向量的长度
 25     File path = new File("/home/orisun/dvm"); // 存放文档向量的路径
 26     HashMap<String, Vector<Double>> dv = new HashMap<String, Vector<Double>>(); // key是文档名，value是文档对应的规一化之前的向量
 27     double[] sqrt;        //存储向量数组每个位置上的平方和
 28 
 29     public void printFeature() {
 30         Iterator<String> iter = features.iterator();
 31         while (iter.hasNext()) {
 32             System.out.print(iter.next() + "\t");
 33         }
 34         System.out.println();
 35     }
 36 
 37     // 从文件中读入特征项。参数文件存储经过特征选择后剩下的特征项。
 38     public void initFeatures(File file) {
 39         try {
 40             FileReader fr = new FileReader(file);
 41             BufferedReader br = new BufferedReader(fr);
 42             String line = null;
 43             while ((line = br.readLine()) != null) {
 44                 features.add(line.split("\\s+")[0]);
 45             }
 46         } catch (Exception e) {
 47             e.printStackTrace();
 48         }
 49         len = features.size();
 50         sqrt = new double[len];
 51     }
 52 
 53     public void buildDVM(File srcFile) {
 54         if (srcFile.isDirectory()) {
 55             File[] children = srcFile.listFiles();
 56             for (File child : children) {
 57                 buildDVM(child);
 58             }
 59         } else if (srcFile.isFile()) {
 60             featureselect.IG.initMatrix(new File("/home/orisun/matrix"));
 61             // new featureselect.WordDocMatrix().printMatrix(System.out,featureselect.IG.matrix);
 62             Object[] feature_array = features.toArray();
 63             HashMap<String, Double> fea_wei = new HashMap<String, Double>();
 64 
 65             int filename = Integer.parseInt(srcFile.getName());
 66             Vector<Double> v = new Vector<Double>(len);
 67             for (int i = 0; i < len; i++)
 68                 v.add(0.0);
 69             try {
 70                 FileReader fr = new FileReader(srcFile);
 71                 BufferedReader br = new BufferedReader(fr);
 72                 String line = null;
 73                 while ((line = br.readLine()) != null) { // 逐个读取文档中的词语
 74                     String word = line.split("\\s+")[0];
 75                     if (!features.contains(word))
 76                         continue;
 77                     int tf = 0; // 特征项在本文档中出现的频率
 78                     int Ni = 0; // 出现特征项的文档数目
 79                     int N = featureselect.WordDocMatrix.docnumber; // 全部文档数目
 80                     ArrayList<Short> al = featureselect.IG.matrix.get(word);
 81                     tf = al.get(filename);
 82                     for (int i = 0; i < N; i++) {
 83                         if (al.get(i) > 0)
 84                             Ni++;
 85                     }
 86                     // System.out.println("word="+word+"\tfilenmae="+filename+"\ttf="+tf+"\tNi="+Ni);
 87                     double weight = -1.0 * tf
 88                             * Math.log(1.0 * Ni / N + Double.MIN_VALUE);
 89                     fea_wei.put(word, weight);
 90                 }
 91                 for (int i = 0; i < feature_array.length; i++) {
 92                     String feat = feature_array[i].toString();
 93                     double w = 0.0;
 94                     if (fea_wei.containsKey(feat))
 95                         w = fea_wei.get(feat);
 96                     v.set(i, w);
 97                     sqrt[i] += Math.pow(w, 2);
 98                 }
 99                 dv.put(String.valueOf(filename), v);
100 
101             } catch (Exception e) {
102                 e.printStackTrace();
103             }
104 
105         }
106     }
107 
108     //文档向量归一化
109     public void unionVector() {
110         Iterator<Entry<String, Vector<Double>>> iter = dv.entrySet().iterator();
111         while (iter.hasNext()) {
112             Entry<String, Vector<Double>> entry = iter.next();
113             String fname = entry.getKey();
114             Vector<Double> v = entry.getValue();
115             Iterator<Double> it=v.iterator();
116             
117             File newFile = new File(path, fname);
118             try {
119                 newFile.createNewFile();
120                 FileWriter fw=new FileWriter(newFile);
121                 BufferedWriter bw=new BufferedWriter(fw);
122                 
123                 int index=0;
124                 while(it.hasNext()){
125                     double d=it.next();
126                     d/=Math.sqrt(sqrt[index]);
127                     //归一化后写入文件
128                     bw.write(String.valueOf(d));
129                     bw.newLine();
130                     index++;
131                 }
132                 bw.flush();
133                 bw.close();
134             } catch (Exception e) {
135                 e.printStackTrace();
136             }
137 
138         }
139     }
140 
141     public static void main(String[] args) {
142         TVSM inst = new TVSM();
143         File feaFile = new File("/home/orisun/features");
144         inst.initFeatures(feaFile);
145         // inst.printFeature();
146         File freqFile = new File("/home/orisun/corpus");
147         
148         if (!freqFile.exists()) {
149             System.out.println("文件不存在，程序退出.");
150             System.exit(2);
151         }
152         inst.buildDVM(freqFile);
153         inst.unionVector();
154     }
155 }
```

７．新建一个文件~/unknown，文件内容为：
- 
美国 比 日本 富强

接下来我们要判断这个文档属于C1类还是C2类。




８．利用WordCount.java程序（当然参数文件要作修改）我们统计出unknown文件中各个词语出现的次数，写入原文件中。

９．运行ADVM.java建立unknown文件的文档向量模型，向量写入文件~/dvm2/unknown中。其文档向量为（不需要进行归一化）：
- 
0.0 

1.0986122886681098 

0.0 

0.0 

1.0986122886681098 

0.0

- 
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Aug 31, 2011
  4  * FileName: ADVM.java
  5  * Function: build document vector model for a new come document
  6  */
  7 package DVM;
  8 
  9 import java.io.BufferedReader;
 10 import java.io.BufferedWriter;
 11 import java.io.File;
 12 import java.io.FileReader;
 13 import java.io.FileWriter;
 14 import java.util.ArrayList;
 15 import java.util.HashMap;
 16 import java.util.HashSet;
 17 import java.util.Iterator;
 18 import java.util.Vector;
 19 
 20 public class AVSM {
 21 
 22     HashSet<String> features = new HashSet<String>();// 存放最终选择的特征词
 23     int len; // 特征项的个数,亦即文档向量的长度
 24     File path = new File("/home/orisun/dvm2"); // 存放文档向量的路径
 25 
 26     // 从文件中读入特征项。参数文件存储经过特征选择后剩下的特征项。
 27     public void initFeatures(File file) {
 28         try {
 29             FileReader fr = new FileReader(file);
 30             BufferedReader br = new BufferedReader(fr);
 31             String line = null;
 32             while ((line = br.readLine()) != null) {
 33                 features.add(line.split("\\s+")[0]);
 34             }
 35         } catch (Exception e) {
 36             e.printStackTrace();
 37         }
 38         len = features.size();
 39     }
 40     
 41     //参数文件存放文档中单词的频数
 42     public void buildDVM(File srcFile) {
 43         if (srcFile.isDirectory()) {
 44             File[] children = srcFile.listFiles();
 45             for (File child : children) {
 46                 buildDVM(child);
 47             }
 48         } else if (srcFile.isFile()) {
 49             featureselect.IG.initMatrix(new File("/home/orisun/matrix"));
 50             Object[] feature_array = features.toArray();
 51             HashMap<String, Double> fea_wei = new HashMap<String, Double>();
 52 
 53             String filename = srcFile.getName();
 54             File newFile = new File(path, filename);
 55             Vector<Double> v = new Vector<Double>(len);
 56             for (int i = 0; i < len; i++)
 57                 v.add(0.0);
 58             try {
 59                 newFile.createNewFile();
 60                 FileReader fr = new FileReader(srcFile);
 61                 BufferedReader br = new BufferedReader(fr);
 62                 String line = null;
 63                 while ((line = br.readLine()) != null) { // 逐个读取文档中的词语
 64                     String[] content=line.split("\\s+");
 65                     String word = content[0];
 66                     int tf=Integer.parseInt(content[1]);// 特征项在本文档中出现的频率
 67                     if (!features.contains(word))
 68                         continue;
 69                     int Ni = 0; // 出现特征项的文档数目
 70                     int N = featureselect.WordDocMatrix.docnumber; // 全部文档数目
 71                     ArrayList<Short> al = featureselect.IG.matrix.get(word);
 72                     for (int i = 0; i < N; i++) {
 73                         if (al.get(i) > 0)
 74                             Ni++;
 75                     }
 76                     //System.out.println("word="+word+"\tfilenmae="+filename+"\ttf="+tf+"\tNi="+Ni);
 77                     double weight = -1.0 * tf
 78                             * Math.log(1.0 * Ni / N + Double.MIN_VALUE);
 79                     fea_wei.put(word, weight);
 80                 }
 81                 for(int i=0;i<feature_array.length;i++){
 82                     String feat=feature_array[i].toString();
 83                     double w=0.0;
 84                     if(fea_wei.containsKey(feat))
 85                         w=fea_wei.get(feat);
 86                     v.set(i, w);
 87                 }
 88                 
 89                 //把文档向量写入dvm路径下的文件
 90                 FileWriter fw=new FileWriter(newFile);
 91                 BufferedWriter bw=new BufferedWriter(fw);
 92                 Iterator<Double> iter=v.iterator();
 93                 while(iter.hasNext()){
 94                     bw.write(String.valueOf(iter.next()));
 95                     bw.newLine();
 96                 }
 97                 bw.flush();
 98                 bw.close();
 99             } catch (Exception e) {
100                 e.printStackTrace();
101             }
102 
103         }
104     }
105     public static void main(String[] args){
106         AVSM inst=new AVSM();
107         File feaFile=new File("/home/orisun/features"); 
108         inst.initFeatures(feaFile);
109         //inst.printFeature();
110         File freqFile=new File("/home/orisun/unknown");
111         inst.buildDVM(freqFile);
112         if(!freqFile.exists()){
113             System.out.println("文件不存在，程序退出.");
114             System.exit(2);
115         }
116         
117     }
118 }
```

１０．运行KNN.java。计算出unknown向量和6个训练向量的夹角斜纹余弦为：

||0|0||----|----||1|0||2|0||3|0.7071||4|1||5|0.7071||0|0|1|0|2|0|3|0.7071|4|1|5|0.7071|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|0|0| | | | | | | | | | | |
|1|0| | | | | | | | | | | |
|2|0| | | | | | | | | | | |
|3|0.7071| | | | | | | | | | | |
|4|1| | | | | | | | | | | |
|5|0.7071| | | | | | | | | | | |



余弦值越大说明夹角越小，说明向量越相似。显然unknown文件与文档3、4、5很相似，所以它因该属于第2类！

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


  1 /**
  2  * Author: Orisun
  3  * Date: Aug 30, 2011
  4  * FileName: KNN.java
  5  * Function: K-Nearest Neighbour Algorithm
  6  */
  7 package classification;
  8 
  9 import java.io.BufferedReader;
 10 import java.io.File;
 11 import java.io.FileReader;
 12 import java.io.IOException;
 13 import java.util.ArrayList;
 14 import java.util.HashMap;
 15 import java.util.Vector;
 16 import java.util.Map.Entry;
 17 
 18 import featureselect.FS;
 19 
 20 public class KNN {
 21     //训练集文档的向量表示
 22     HashMap<String, Double> dist = new HashMap<String, Double>(); // 等分类文档互每个训练集文档的距离
 23     Vector<Double> uv = new Vector<Double>(); // 待分类文档的向量表示
 24 
 25     // srcFile存放了待分类文档的向量表示
 26     public void initUV(File srcFile) {
 27         if (!srcFile.exists()) {
 28             System.err.println("File not found.Program exit with failure.");
 29             System.exit(2);
 30         }
 31         try {
 32             FileReader fr = new FileReader(srcFile);
 33             BufferedReader br = new BufferedReader(fr);
 34             String line = null;
 35             while ((line = br.readLine()) != null) {
 36                 uv.add(Double.valueOf(line.trim()));
 37             }
 38             br.close();
 39         } catch (IOException e) {
 40             e.printStackTrace();
 41         }
 42     }
 43 
 44     // srcFile是训练集的文档向量所在的文件夹
 45     public void calDist(File srcFile) {
 46         File[] children = srcFile.listFiles();
 47         
 48         for (File child : children) {
 49             String filename = child.getName();
 50             Vector<Double> v = new Vector<Double>();
 51             try {
 52                 FileReader fr = new FileReader(child);
 53                 BufferedReader br = new BufferedReader(fr);
 54                 String line = null;
 55                 while ((line = br.readLine()) != null) {
 56                     v.add(Double.valueOf(line.trim()));
 57                 }
 58                 br.close();
 59             } catch (IOException e) {
 60                 e.printStackTrace();
 61             }
 62             int len = v.size();
 63             double d = cos(v, uv, len);
 64             dist.put(filename, d);
 65             System.out.println("到"+filename+"的距离是"+d);
 66         }
 67     }
 68 
 69     // 计算两个向量的夹角的余弦。如果此值的绝对值越大，说明夹角越小，越相似，距离越近。
 70     public double cos(Vector<Double> v1, Vector<Double> v2, int len) {
 71         double res = 0.0;
 72         double mul = 0.0;
 73         double p1 = 0.0, p2 = 0.0;
 74         for (int i = 0; i < len; i++) {
 75             double one = v1.get(i);
 76             double two = v2.get(i);
 77             mul += one * two;
 78             p1 += Math.pow(one, 2);
 79             p2 += Math.pow(two, 2);
 80         }
 81         res = Math.abs(mul) / Math.sqrt(p1 * p2);
 82         return res;
 83     }
 84 
 85     public void knn(int k){
 86         //对新向量到所有训练向量的距离按从大到小排序
 87         FS fs=new FS();
 88         ArrayList<Entry<String,Double>> dist_list=fs.sort(dist);
 89         int c1=0,c2=0;
 90         for(int i=0;i<k;i++){
 91             Entry<String,Double> entry=dist_list.get(i);
 92             int fileNum=Integer.parseInt(entry.getKey());
 93             if(fileNum>=0 && fileNum<3)
 94                 c1++;
 95             else if(fileNum>=3 && fileNum<6)
 96                 c2++;
 97         }
 98         if(c1>c2)
 99             System.out.println("属于第1类！");
100         else if(c2>1)
101             System.out.println("属于第2类！");
102         else
103             System.out.println("属于两类的可能性一样大！");
104     }
105     
106     public static void main(String[] args){
107         KNN inst=new KNN();
108         File uvFile=new File("/home/orisun/dvm2/unknown");
109         inst.initUV(uvFile);
110         File tvFiles=new File("/home/orisun/dvm");
111         inst.calDist(tvFiles);
112         inst.knn(3);
113     }
114 }
```



以上只是简单的模拟，在实际文本分类中训练集很大时，word-doc matrix会很大，必然造成内存溢出。解决方法参见我的从原始文档到KNN分类算法实现（二）
















