# 用MapReduce实现大矩阵相乘 - Orisun - 博客园







# [用MapReduce实现大矩阵相乘](https://www.cnblogs.com/zhangchaoyang/articles/4646315.html)





### 方法一

第一阶段的Map,对每一数据项 $(i,j,v)$,若来自矩阵 A ,则输出 $(j,(A,i,v))$,若来自矩阵 B ,则输出$(i,(B,j,v))$,这 样 矩 阵 A 的 第j 列 和 矩 阵 B 的 第 i 行 会被同一个 reduce 节点处理;在 Reduce 端,将来自 A 和 B 的数据分别存储在数组 listA 和 listB ,对来自 A 中的数据 $(j,(A,i,v))$ ,令 listA[i] = v ,对来自 B 的数 据$(i,(B,j,v))$,令 listB[ j] = v 。将 listA 中的每个项 乘以 listB 中的每个项然后输出,对于 listA[i] 和 listB[ j] ,输出 $((i,j),listA[i]*listB[j])$。在第二阶段只 需要将第一阶段输出中有相同 key 的数据求和即可。 

MatrixMultiplication1.java

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1 package com.lagou.mining.hdfs;
  2 
  3 import java.io.IOException;
  4 import java.util.HashMap;
  5 import java.util.Iterator;
  6 import java.util.Map;
  7 import java.util.Map.Entry;
  8 
  9 import org.apache.hadoop.conf.Configuration;
 10 import org.apache.hadoop.fs.FileSystem;
 11 import org.apache.hadoop.fs.Path;
 12 import org.apache.hadoop.io.DoubleWritable;
 13 import org.apache.hadoop.io.IntWritable;
 14 import org.apache.hadoop.io.LongWritable;
 15 import org.apache.hadoop.io.Text;
 16 import org.apache.hadoop.mapreduce.Job;
 17 import org.apache.hadoop.mapreduce.Mapper;
 18 import org.apache.hadoop.mapreduce.Reducer;
 19 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
 20 import org.apache.hadoop.mapreduce.lib.input.FileSplit;
 21 import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
 22 
 23 
 24 /**
 25  * 矩阵相乘。
 26  * 
 27  * @Author:orisun
 28  * @Since:2015-6-26
 29  * @Version:1.0
 30  */
 31 public class MatrixMultiplication1 {
 32 
 33     private static final String MATRIXFILE1 = "A";
 34     private static final String MATRIXFILE2 = "B";
 35 
 36     public static class EleEmitMapper extends
 37             Mapper<IntWritable, Text, IntWritable, Text> {
 38 
 39         private String matrixFile1 = null;
 40         private String matrixFile2 = null;
 41 
 42         @Override
 43         protected void setup(Context context) {
 44             matrixFile1 = context.getConfiguration().get(MATRIXFILE1);
 45             matrixFile2 = context.getConfiguration().get(MATRIXFILE2);
 46         }
 47 
 48         @Override
 49         protected void map(IntWritable key, Text value, Context context)
 50                 throws IOException, InterruptedException {
 51             IntWritable i = key;
 52             String filename = ((FileSplit) context.getInputSplit()).getPath()
 53                     .getName();
 54             String[] arr = value.toString().split("\\s+");
 55             // 发出去N个互不相同的key
 56             if (filename.equals(matrixFile1)) {
 57                 for (int j = 0; j < arr.length; j++) {
 58                     double v = Double.parseDouble(arr[j]);
 59                     //元素为0时不发出，对于稀疏矩阵这样效率会比较高
 60                     if (v != 0) {
 61                         context.write(new IntWritable(j), new Text(MATRIXFILE1
 62                                 + "\t" + i.toString() + "\t" + v));
 63                     }
 64                 }
 65             } else if (filename.equals(matrixFile2)) {
 66                 for (int j = 0; j < arr.length; j++) {
 67                     double v = Double.parseDouble(arr[j]);
 68                     //元素为0时不发出，对于稀疏矩阵这样效率会比较高
 69                     if (v != 0) {
 70                         context.write(i, new Text(MATRIXFILE2 + "\t" + j + "\t"
 71                                 + v));
 72                     }
 73                 }
 74             }
 75         }
 76 
 77         @Override
 78         protected void cleanup(Context context) {
 79         }
 80     }
 81 
 82     public static class MultiplicationReducer extends
 83             Reducer<IntWritable, Text, Text, DoubleWritable> {
 84 
 85         @Override
 86         protected void reduce(IntWritable key, Iterable<Text> value,
 87                 Context context) throws IOException, InterruptedException {
 88             Map<Integer, Double> listA = new HashMap<Integer, Double>();
 89             Map<Integer, Double> listB = new HashMap<Integer, Double>();
 90             Iterator<Text> itr = value.iterator();
 91             while (itr.hasNext()) {
 92                 String[] arr = itr.next().toString().split("\\s+");
 93                 String matrixTag = arr[0];
 94                 int pos = Integer.parseInt(arr[1]);
 95                 double v = Double.parseDouble(arr[2]);
 96                 if (MATRIXFILE1.equals(matrixTag)) {
 97                     listA.put(pos, v);
 98                 } else if (MATRIXFILE2.equals(matrixTag)) {
 99                     listB.put(pos, v);
100                 }
101             }
102             // 在此需要进行N*N次的乘法
103             for (Entry<Integer, Double> entryA : listA.entrySet()) {
104                 int posA = entryA.getKey();
105                 double valA = entryA.getValue();
106                 for (Entry<Integer, Double> entryB : listB.entrySet()) {
107                     int posB = entryB.getKey();
108                     double valB = entryB.getValue();
109                     double production = valA * valB;
110                     context.write(new Text(posA + "\t" + posB),
111                             new DoubleWritable(production));
112                 }
113             }
114         }
115     }
116 
117     public static class SumMapper extends
118             Mapper<LongWritable, Text, Text, DoubleWritable> {
119 
120         @Override
121         protected void map(LongWritable key, Text value, Context context)
122                 throws IOException, InterruptedException {
123             String[] arr = value.toString().split("\\s+");
124             if (arr.length == 3) {
125                 context.write(new Text(arr[0] + "\t" + arr[1]),
126                         new DoubleWritable(Double.parseDouble(arr[2])));
127             }
128         }
129     }
130 
131     public static class SumCombiner extends
132             Reducer<Text, DoubleWritable, Text, DoubleWritable> {
133 
134         @Override
135         protected void reduce(Text key, Iterable<DoubleWritable> value,
136                 Context context) throws IOException, InterruptedException {
137             double sum = 0;
138             Iterator<DoubleWritable> itr = value.iterator();
139             while (itr.hasNext()) {
140                 sum += itr.next().get();
141             }
142             context.write(key, new DoubleWritable(sum));
143         }
144     }
145 
146     public static class SumReducer extends
147             Reducer<Text, DoubleWritable, Text, DoubleWritable> {
148 
149         @Override
150         protected void reduce(Text key, Iterable<DoubleWritable> value,
151                 Context context) throws IOException, InterruptedException {
152             double sum = 0;
153             Iterator<DoubleWritable> itr = value.iterator();
154             while (itr.hasNext()) {
155                 sum += itr.next().get();
156             }
157             context.write(key, new DoubleWritable(sum));
158         }
159     }
160 
161     /**
162      * matrix1 * matrix2 = product<br>
163      * matrixFile1：输入文件，m行q列。<br>
164      * matrixFile2：输入文件，q行n列。<br>
165      * productFile：输出文件，m行n列。<br>
166      * 各列用空白符分隔。
167      */
168     public static void main(String[] args) throws IOException,
169             ClassNotFoundException, InterruptedException {
170         if (args.length < 3) {
171             System.err
172                     .println("please input 3 cmd args: matrixFile1 matrixFile2 productFile");
173             System.exit(1);
174         }
175         String matrixFile1 = args[0];
176         String matrixFile2 = args[1];
177         String productFile = args[2];
178 
179         Configuration conf = new Configuration();
180         FileSystem fs = FileSystem.get(conf);
181         Path inFile1 = new Path(matrixFile1);
182         Path inFile2 = new Path(matrixFile2);
183         conf.set(MATRIXFILE1, inFile1.getName());
184         conf.set(MATRIXFILE2, inFile2.getName());
185         Path midFile = new Path(inFile1.getParent().toUri().getPath()
186                 + "/product_tmp");
187         Path outFile = new Path(productFile);
188         if (!fs.exists(inFile2) || !fs.exists(inFile1)) {
189             System.err.println("input matrix file does not exists");
190             System.exit(1);
191         }
192         if (fs.exists(midFile)) {
193             fs.delete(midFile, true);
194         }
195         if (fs.exists(outFile)) {
196             fs.delete(outFile, true);
197         }
198 
199         {
200             Job productionJob1 = Job.getInstance(conf);
201             productionJob1.setJobName("MatrixMultiplication1_step1");
202             productionJob1.setJarByClass(MatrixMultiplication1.class);
203 
204             FileInputFormat.addInputPath(productionJob1, inFile1);
205             FileInputFormat.addInputPath(productionJob1, inFile2);
206             productionJob1.setInputFormatClass(MatrixInputFormat.class);
207             productionJob1.setMapperClass(EleEmitMapper.class);
208             productionJob1.setMapOutputKeyClass(IntWritable.class);
209             productionJob1.setMapOutputValueClass(Text.class);
210 
211             FileOutputFormat.setOutputPath(productionJob1, midFile);
212             productionJob1.setReducerClass(MultiplicationReducer.class);
213             productionJob1.setNumReduceTasks(12);
214             productionJob1.setOutputKeyClass(Text.class);
215             productionJob1.setOutputValueClass(DoubleWritable.class);
216 
217             productionJob1.waitForCompletion(true);
218         }
219 
220         {
221             Job productionJob2 = Job.getInstance(conf);
222             productionJob2.setJobName("MatrixMultiplication1_step2");
223             productionJob2.setJarByClass(MatrixMultiplication1.class);
224 
225             FileInputFormat.setInputPaths(productionJob2, midFile);
226             productionJob2.setMapperClass(SumMapper.class);
227             productionJob2.setMapOutputKeyClass(Text.class);
228             productionJob2.setMapOutputValueClass(DoubleWritable.class);
229 
230             FileOutputFormat.setOutputPath(productionJob2, outFile);
231             productionJob2.setCombinerClass(SumCombiner.class);
232             productionJob2.setReducerClass(SumReducer.class);
233             productionJob2.setNumReduceTasks(1);
234             productionJob2.setOutputKeyClass(Text.class);
235             productionJob2.setOutputValueClass(DoubleWritable.class);
236 
237             productionJob2.waitForCompletion(true);
238             
239         }
240         fs.delete(midFile, true);
241         System.exit(0);
242     }
243 }


View Code
```

MatrixInputFormat.java

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 import java.io.IOException;
 2 
 3 import org.apache.hadoop.fs.Path;
 4 import org.apache.hadoop.io.IntWritable;
 5 import org.apache.hadoop.io.Text;
 6 import org.apache.hadoop.mapreduce.InputSplit;
 7 import org.apache.hadoop.mapreduce.JobContext;
 8 import org.apache.hadoop.mapreduce.RecordReader;
 9 import org.apache.hadoop.mapreduce.TaskAttemptContext;
10 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
11 
12 public class MatrixInputFormat extends FileInputFormat<IntWritable,Text>{
13 
14     @Override
15     public RecordReader<IntWritable, Text> createRecordReader(
16             InputSplit split, TaskAttemptContext context)
17             throws IOException, InterruptedException {
18         return new MatrixLineRecordReader();
19     }
20     
21     /*因为读入时要记录行号，所以要保证中有一个mapper，这样行号才是一致的*/
22     @Override
23     protected boolean isSplitable(JobContext context, Path filename){
24         return false;
25     }
26 
27 }


View Code
```

MatrixLineRecordReader.java

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 import java.io.IOException;
 2 
 3 import org.apache.hadoop.conf.Configuration;
 4 import org.apache.hadoop.fs.FSDataInputStream;
 5 import org.apache.hadoop.fs.FileSystem;
 6 import org.apache.hadoop.fs.Path;
 7 import org.apache.hadoop.io.IntWritable;
 8 import org.apache.hadoop.io.Text;
 9 import org.apache.hadoop.mapreduce.InputSplit;
10 import org.apache.hadoop.mapreduce.RecordReader;
11 import org.apache.hadoop.mapreduce.TaskAttemptContext;
12 import org.apache.hadoop.mapreduce.lib.input.FileSplit;
13 import org.apache.hadoop.util.LineReader;
14 
15 public class MatrixLineRecordReader extends RecordReader<IntWritable, Text> {
16     private LineReader in;
17     private int lineno = 0; // 行编号从0开始
18     private boolean more = true;
19     private IntWritable key = null;
20     private Text value = null;
21 
22     @Override
23     public void initialize(InputSplit inputSplit, TaskAttemptContext context)
24             throws IOException, InterruptedException {
25         FileSplit split = (FileSplit) inputSplit;
26         Configuration conf = context.getConfiguration();
27         Path file = split.getPath();
28         FileSystem fs = file.getFileSystem(conf);
29         FSDataInputStream fileIn = fs.open(file);
30         in = new LineReader(fileIn, conf);
31     }
32 
33     @Override
34     public boolean nextKeyValue() throws IOException, InterruptedException {
35         if (key == null)
36             key = new IntWritable();
37         if (value == null)
38             value = new Text();
39         int readsize = in.readLine(value);
40         if (readsize == 0) {
41             more = false;
42             return false;
43         }
44         key.set(lineno);
45         lineno++;
46         return true;
47     }
48 
49     @Override
50     public IntWritable getCurrentKey() throws IOException, InterruptedException {
51         return key;
52     }
53 
54     @Override
55     public Text getCurrentValue() throws IOException, InterruptedException {
56         return value;
57     }
58 
59     @Override
60     public float getProgress() throws IOException, InterruptedException {
61         if (more)
62             return 0.0f;
63         else
64             return 100f;
65     }
66 
67     @Override
68     public void close() throws IOException {
69         in.close();
70     }
71 
72 }


View Code
```

方法一的优点是：再大的矩阵也可以处理。缺点是：网络IO太大，速度慢。如果两个矩阵中有一个不是太大，可以采用方法二。

### 方法二

对于$A\times{B}$，如果B不是很大，可以把B放到分布式缓存上，把A按行切分发送给多个Mapper Task，各个Mapper Task把B完全放入内存中。

MatrixMultiplication2.java

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1 import java.io.BufferedReader;
  2 import java.io.FileReader;
  3 import java.io.IOException;
  4 import java.net.URISyntaxException;
  5 
  6 import org.apache.hadoop.conf.Configuration;
  7 import org.apache.hadoop.fs.FileSystem;
  8 import org.apache.hadoop.fs.Path;
  9 import org.apache.hadoop.io.DoubleWritable;
 10 import org.apache.hadoop.io.IntWritable;
 11 import org.apache.hadoop.io.Text;
 12 import org.apache.hadoop.mapreduce.Job;
 13 import org.apache.hadoop.mapreduce.Mapper;
 14 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
 15 import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
 16 
 17 
 18 /**
 19  * 计算C=A*B。A的规模是M*R，B的规模是R*N，则C的规模是M*N。<br>
 20  * A矩阵按行切分，发送给不同的Mapper，B作为分布式缓存，全量加载到各个Mapper的内存当中。
 21  * 
 22  * @Author:orisun
 23  * @Since:2015-7-14
 24  * @Version:1.0
 25  */
 26 public class MatrixMultiplication2 {
 27 
 28 //    private static final Log LOG = LogFactory.getLog(FileInputFormat.class);
 29 
 30     private static final String ROW1 = "M";
 31     private static final String COL1 = "R";
 32     private static final String COL2 = "N";
 33 
 34     public static class MultipleMapper extends
 35             Mapper<IntWritable, Text, Text, DoubleWritable> {
 36 
 37         private double[][] B;
 38         private int M;
 39         private int R;
 40         private int N;
 41 
 42         @Override
 43         protected void setup(Context context) throws IOException {
 44             M = Integer.parseInt(context.getConfiguration().get(ROW1));
 45             R = Integer.parseInt(context.getConfiguration().get(COL1));
 46             N = Integer.parseInt(context.getConfiguration().get(COL2));
 47             B = new double[R][];
 48             // URI[] localCacheFiles =
 49             // context.getCacheFiles();//这个方法是新版推荐的，但是不work
 50             @SuppressWarnings("deprecation")
 51             Path[] localFiles = context.getLocalCacheFiles();// 这个方法虽然是过期的，但是work
 52             BufferedReader br = new BufferedReader(new FileReader(
 53                     localFiles[0].toString()));
 54             for (int i = 0; i < R; i++) {
 55                 String line = br.readLine();
 56                 assert line != null;
 57                 B[i] = new double[N];
 58                 String[] arr = line.split("\\s+");
 59                 assert arr.length == N;
 60                 for (int j = 0; j < N; j++) {
 61                     B[i][j] = Double.parseDouble(arr[j]);
 62                 }
 63             }
 64             br.close();
 65 //            LOG.warn("B scale:" + B.length + "," + B[B.length - 1].length);
 66             System.out.print("B scale:" + B.length + "," + B[B.length - 1].length);
 67         }
 68 
 69         @Override
 70         protected void map(IntWritable key, Text value, Context context)
 71                 throws IOException, InterruptedException {
 72             int lineno = key.get();// 第一个矩阵的行号
 73             assert lineno < M;
 74             String[] arr = value.toString().split("\\s+");
 75             assert arr.length == R;
 76             for (int j = 0; j < N; j++) {
 77                 double sum = 0.0;
 78                 for (int i = 0; i < R; i++) {
 79                     sum += B[i][j] * Double.parseDouble(arr[i]);
 80                 }
 81                 context.write(new Text(lineno + "," + j), new DoubleWritable(
 82                         sum));
 83             }
 84         }
 85     }
 86 
 87     public static void main(String[] args) throws IOException,
 88             URISyntaxException, ClassNotFoundException, InterruptedException {
 89         if (args.length < 6) {
 90             System.err
 91                     .println("please input 6 cmd args: matrixFile1 matrixFile2 row_of_m1 col_of_m1 col_of_m2 productFile");
 92             System.exit(1);
 93         }
 94         String matrixFile1 = args[0];
 95         String matrixFile2 = args[1];
 96         String productFile = args[5];
 97 
 98         Configuration conf = new Configuration();
 99         FileSystem fs = FileSystem.get(conf);
100         Path inFile1 = new Path(matrixFile1);
101         Path inFile2 = new Path(matrixFile2);
102         conf.set(ROW1, args[2]);
103         conf.set(COL1, args[3]);
104         conf.set(COL2, args[4]);
105 
106         Path outFile = new Path(productFile);
107         if (!fs.exists(inFile1) || !fs.exists(inFile2)) {
108             System.err.println("input matrix file does not exists");
109             System.exit(1);
110         }
111         if (fs.exists(outFile)) {
112             fs.delete(outFile, true);
113         }
114 
115         Job job = Job.getInstance(conf);
116         job.setJobName("MatrixMultiplication2");
117         job.setJarByClass(MatrixMultiplication2.class);
118         job.addCacheFile(inFile2.toUri());// 分发缓存文件
119 
120         FileInputFormat.setInputPaths(job, inFile1);
121         FileOutputFormat.setOutputPath(job, outFile);
122         job.setInputFormatClass(LineInputFormat.class);
123         job.setMapperClass(MultipleMapper.class);
124         job.setMapOutputKeyClass(Text.class);
125         job.setMapOutputValueClass(DoubleWritable.class);
126 
127         job.setNumReduceTasks(0);
128         int exitcode = job.waitForCompletion(true) ? 0 : 1;
129         System.exit(exitcode);
130     }
131 }


View Code
```

LineInputFormat.java

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 import java.io.IOException;
 2 import java.util.ArrayList;
 3 import java.util.List;
 4 
 5 import org.apache.commons.logging.Log;
 6 import org.apache.commons.logging.LogFactory;
 7 import org.apache.hadoop.fs.BlockLocation;
 8 import org.apache.hadoop.fs.FileStatus;
 9 import org.apache.hadoop.fs.FileSystem;
10 import org.apache.hadoop.fs.Path;
11 import org.apache.hadoop.io.IntWritable;
12 import org.apache.hadoop.io.Text;
13 import org.apache.hadoop.mapreduce.InputSplit;
14 import org.apache.hadoop.mapreduce.JobContext;
15 import org.apache.hadoop.mapreduce.RecordReader;
16 import org.apache.hadoop.mapreduce.TaskAttemptContext;
17 import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
18 import org.apache.hadoop.mapreduce.lib.input.FileSplit;
19 
20 public class LineInputFormat extends FileInputFormat<IntWritable, Text> {
21     public long mySplitSize = 5 * 1024 * 1024; // 一个split的大小（单位B）。注意：对于每个inputsplit都会产生一个mapper
22     // task来处理它，所以要控制inputsplit的数量
23     private static final Log LOG = LogFactory.getLog(FileInputFormat.class);
24     private static final double SPLIT_SLOP = 1.1; // 10% slop
25 
26     @Override
27     public RecordReader<IntWritable, Text> createRecordReader(InputSplit split,
28             TaskAttemptContext context) {
29         return new MatrixLineRecordReader();
30     }
31 
32     @Override
33     protected boolean isSplitable(JobContext context, Path file) {
34         return true;// 要求分片
35     }
36 
37     @Override
38     public List<InputSplit> getSplits(JobContext job) throws IOException {
39         // generate splits
40         List<InputSplit> splits = new ArrayList<InputSplit>(); // 用以存放生成的split的
41         for (FileStatus file : listStatus(job)) {// filestatues是文件对应的信息，具体看对应的类
42             Path path = file.getPath();
43             FileSystem fs = path.getFileSystem(job.getConfiguration());
44             long length = file.getLen(); // 得到文本的长度
45             BlockLocation[] blkLocations = fs.getFileBlockLocations(file, 0,
46                     length); // 取得文件所在块的位置
47             if ((length != 0) && isSplitable(job, path)) { // 如果文件不为空，并且可以分片的话就进行下列操作,
48                 // 可以通过调整splitSize的大小来控制对应的文件块的大小，比如设置splitSize=100，那么就可以控制成每个split的大小
49                 // 但是问题是，我是要求按行进行处理的，虽然这样应该也可以按行进行切分了，不过却不能保证每个split对应的行数都是相等的
50                 // 一般情况是如果文件大于64M（32M）就会使用块大小来作为split
51                 long splitSize = mySplitSize;
52                 long bytesRemaining = length; // 文本的长度
53                 while (((double) bytesRemaining) / splitSize > SPLIT_SLOP) {// 剩下的文本长度大于split大小的SPLIT_SLOP倍数
54                     int blkIndex = getBlockIndex(blkLocations, length
55                             - bytesRemaining);// 找到对应block块中对应的第0个字符开始，
56                     splits.add(new FileSplit(path, length - bytesRemaining,
57                             splitSize, blkLocations[blkIndex].getHosts()));
58                     // 这个是形成split的代码FileSplit(文件路径，0，split大小，host)
59                     // 原始函数为 FileSplit(Path file, long start, long length,
60                     // String[] hosts) {
61                     // 但是应该可以通过重写FileSplit来实现对应的要求
62                     bytesRemaining -= splitSize;
63                 }
64                 if (bytesRemaining != 0) {
65                     splits.add(new FileSplit(path, length - bytesRemaining,
66                             bytesRemaining,
67                             blkLocations[blkLocations.length - 1].getHosts()));
68                 }
69             } else if (length != 0) {
70                 splits.add(new FileSplit(path, 0, length, blkLocations[0]
71                         .getHosts()));
72             } else {
73                 splits.add(new FileSplit(path, 0, length, new String[0]));
74             }
75         }
76         LOG.info("NUMBER OF SPLITS: " + splits.size());
77         return splits;
78     }
79 }


View Code
```














