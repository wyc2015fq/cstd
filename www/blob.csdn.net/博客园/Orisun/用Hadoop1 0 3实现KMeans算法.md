# 用Hadoop1.0.3实现KMeans算法 - Orisun - 博客园







# [用Hadoop1.0.3实现KMeans算法](https://www.cnblogs.com/zhangchaoyang/articles/2634365.html)





从理论上来讲用MapReduce技术实现KMeans算法是很Natural的想法：在Mapper中逐个计算样本点离哪个中心最近，然后Emit(样本点所属的簇编号，样本点)；在Reducer中属于同一个质心的样本点在一个链表中，方便我们计算新的中心，然后Emit(质心编号，质心)。但是技术上的事并没有理论层面那么简单。

Mapper和Reducer都要用到K个中心（我习惯称之为质心），Mapper要读这些质心，Reducer要写这些质心。另外Mapper还要读存储样本点的数据文件。我先后尝试以下3种方法，只有第3种是可行的，如果你不想被我误导，请直接跳过前两种。

### 一、用一个共享变量在存储K个质心

由于K很小，所以我们认为用一个Vector<Sample>来存储K个质心是没有问题的。以下代码是错误的：

class MyJob extends Tool{
　　static Vector<Sample> centers=new Vector<Sample>(K);
　　static class MyMapper extends Mapper{
　　　　//read centers
　　}　
　　static class MyMapper extends Reducer{
　　　　//update centers
　　}
　　void run(){
　　　　until ( convergence ){
　　　　　　map();
　　　　　　reduce();
　　　　}
}


发生这种错误是因为对hadoop执行流程不清楚，对数据流不清楚。简单地说Mapper和Reducer作为MyJob的内部静态类，它们应该是独立的--它们不应该与MyJob有任何交互，因为Mapper和Reducer分别在Task Tracker的不同JVM中运行，而MyJob以及MyJob的内部其他类都在客户端上运行，自然不能在不同的JVM中共享一个变量。

详细的流程是这样的：

首先在客户端上，JVM加载MyJob时先初始化静态变量，执行static块。然后提交作业到Job Tracker。

在Job Tracker上，分配Mapper和Reducer到不同的Task Tracker上。Mapper和Reducer线程获得了MyJob类静态变量的初始拷贝（这份拷贝是指MyJob执行完静态块之后静态变量的模样）。

在Task Tracker上，Mapper和Reducer分别地读写MyJob的静态变量的本地拷贝，但是并不影响原始的MyJob中的静态变量的值。

### 二、用分布式缓存文件存储K个质心

既然不能通过共享外部类变量的方式，那我们通过文件在map和reduce之间传递数据总可以吧，Mapper从文件中读取质心，Reducer把更新后的质心再写入这个文件。这里的问题是：如果确定要把质心放在文件中，那Mapper就需要从2个文件中读取数据--质心文件和样本数据文件。虽然有MutipleInputs可以指定map()的输入文件有多个，并可以为每个输入文件分别指定解析方式，但是MutipleInputs不能保证每条记录从不同文件中传给map()的顺序。在我们的KMeans中，我们希望质心文件全部被读入后再逐条读入样本数据。

于是乎就想到了DistributedCache，它主要用于Mapper和Reducer之间共享数据。DistributedCacheFile是缓存在本地文件，在Mapper和Reducer中都可使用本地Java I/O的方式读取它。于是我又有了一个错误的思路：

class MyMaper{
	Vector<Sample> centers=new Vector<Sample>(K);
	void setup(){
		//读取cacheFile，给centers赋值
	}
	void map(){
		//计算样本离哪个质心最近
	}
}
class MyReducer{
	Vector<Sample> centers=new Vector<Sample>(K);
	void reduce(){
		//更新centers
	}
	void cleanup(){
		//把centers写回cacheFile
	}
}


错因：DistributedCacheFile是只读的，在任务运行前，TaskTracker从JobTracker文件系统复制文件到本地磁盘作为缓存，这是单向的复制，是不能写回的。试想在分布式环境下，如果不同的mapper和reducer可以把缓存文件写回的话，那岂不又需要一套复杂的文件共享机制，严重地影响hadoop执行效率。

### 三、用分布式缓存文件存储样本数据

其实DistributedCache还有一个特点，它更适合于“大文件”（各节点内存容不下）缓存在本地。仅存储了K个质心的文件显然是小文件，与之相比样本数据文件才是大文件。

此时我们需要2个质心文件：一个存放上一次的质心prevCenterFile，一个存放reducer更新后的质心currCenterFile。Mapper从prevCenterFile中读取质心，Reducer把更新后有质心写入currCenterFile。在Driver中读入prevCenterFile和currCenterFile，比较前后两次的质心是否相同（或足够地接近），如果相同则停止迭代，否则就用currCenterFile覆盖prevCenterFile（使用fs.rename），进入下一次的迭代。

这时候Mapper就是这样的：

class MyMaper{
	Vector<Sample> centers=new Vector<Sample>(K);
	void map(){
		//逐条读取质心，给centers赋值
	}
	void cleanup(){
		//逐行读取cacheFile,计算每个样本点离哪个质心最近
		//然后Emit(样本点所属的簇编号，样本点)
	}
}


### 源代码

试验数据是在Mahout项目中作为example提供的，600个样本点，每个样本是一个60维的浮点向量。[点击下载](http://archive.ics.uci.edu/ml/databases/synthetic_control/synthetic_control.data)

为样本数据建立一个类Sample.java。

package kmeans;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.io.Writable;

public class Sample implements Writable{
	private static final Log log=LogFactory.getLog(Sample.class);
	public static final int DIMENTION=60;
	public double arr[];
	
	public Sample(){
		arr=new double[DIMENTION];
	}
	
	public static double getEulerDist(Sample vec1,Sample vec2){
		if(!(vec1.arr.length==DIMENTION && vec2.arr.length==DIMENTION)){
			log.error("vector's dimention is not "+DIMENTION);
			System.exit(1);
		}
		double dist=0.0;
		for(int i=0;i<DIMENTION;++i){
			dist+=(vec1.arr[i]-vec2.arr[i])*(vec1.arr[i]-vec2.arr[i]);
		}
		return Math.sqrt(dist);
	}
	
	public void clear(){
		for(int i=0;i<arr.length;i++)
			arr[i]=0.0;
	}
	
	@Override
	public String toString(){
		String rect=String.valueOf(arr[0]);
		for(int i=1;i<DIMENTION;i++)
			rect+="\t"+String.valueOf(arr[i]);
		return rect;
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		String str[]=in.readUTF().split("\\s+");
		for(int i=0;i<DIMENTION;++i)
			arr[i]=Double.parseDouble(str[i]);
	}

	@Override
	public void write(DataOutput out) throws IOException {
		out.writeUTF(this.toString());
	}
}


KMeans.java

package kmeans;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Vector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class KMeans extends Configured implements Tool{
	private static final Log log = LogFactory.getLog(KMeans2.class);

	private static final int K = 10;
	private static final int MAXITERATIONS = 300;
	private static final double THRESHOLD = 0.01;
	
	public static boolean stopIteration(Configuration conf) throws IOException{
		FileSystem fs=FileSystem.get(conf);
		Path pervCenterFile=new Path("/user/orisun/input/centers");
		Path currentCenterFile=new Path("/user/orisun/output/part-r-00000");
		if(!(fs.exists(pervCenterFile) && fs.exists(currentCenterFile))){
			log.info("两个质心文件需要同时存在");
			System.exit(1);
		}
		//比较前后两次质心的变化是否小于阈值，决定迭代是否继续
		boolean stop=true;
		String line1,line2;
		FSDataInputStream in1=fs.open(pervCenterFile);
		FSDataInputStream in2=fs.open(currentCenterFile);
		InputStreamReader isr1=new InputStreamReader(in1);
		InputStreamReader isr2=new InputStreamReader(in2);
		BufferedReader br1=new BufferedReader(isr1);
		BufferedReader br2=new BufferedReader(isr2);
		Sample prevCenter,currCenter;
		while((line1=br1.readLine())!=null && (line2=br2.readLine())!=null){
			prevCenter=new Sample();
			currCenter=new Sample();
			String []str1=line1.split("\\s+");
			String []str2=line2.split("\\s+");
			assert(str1[0].equals(str2[0]));
			for(int i=1;i<=Sample.DIMENTION;i++){
				prevCenter.arr[i-1]=Double.parseDouble(str1[i]);
				currCenter.arr[i-1]=Double.parseDouble(str2[i]);
			}
			if(Sample.getEulerDist(prevCenter, currCenter)>THRESHOLD){
				stop=false;
				break;
			}
		}
		//如果还要进行下一次迭代，就用当前质心替代上一次的质心
		if(stop==false){
			fs.delete(pervCenterFile,true);
			if(fs.rename(currentCenterFile, pervCenterFile)==false){
				log.error("质心文件替换失败");
				System.exit(1);
			}
		}
		return stop;
	}
	
	public static class ClusterMapper extends Mapper<LongWritable, Text, IntWritable, Sample> {
		Vector<Sample> centers = new Vector<Sample>();
		@Override
		//清空centers
		public void setup(Context context){
			for (int i = 0; i < K; i++) {
				centers.add(new Sample());
			}
		}
		@Override
		//从输入文件读入centers
		public void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {
			String []str=value.toString().split("\\s+");
			if(str.length!=Sample.DIMENTION+1){
				log.error("读入centers时维度不对");
				System.exit(1);
			}
			int index=Integer.parseInt(str[0]);
			for(int i=1;i<str.length;i++)
				centers.get(index).arr[i-1]=Double.parseDouble(str[i]);
		}
		@Override
		//找到每个数据点离哪个质心最近
		public void cleanup(Context context) throws IOException,InterruptedException {
			Path []caches=DistributedCache.getLocalCacheFiles(context.getConfiguration());
			if(caches==null || caches.length<=0){
				log.error("data文件不存在");
				System.exit(1);
			}
			BufferedReader br=new BufferedReader(new FileReader(caches[0].toString()));
			Sample sample;
			String line;
			while((line=br.readLine())!=null){
				sample=new Sample();
				String []str=line.split("\\s+");
				for(int i=0;i<Sample.DIMENTION;i++)
					sample.arr[i]=Double.parseDouble(str[i]);
				
				int index=-1;
				double minDist=Double.MAX_VALUE;
				for(int i=0;i<K;i++){
					double dist=Sample.getEulerDist(sample, centers.get(i));
					if(dist<minDist){
						minDist=dist;
						index=i;
					}
				}
				context.write(new IntWritable(index), sample);
			}
		}
	}
	
	public static class UpdateCenterReducer extends Reducer<IntWritable, Sample, IntWritable, Sample> {
		int prev=-1;
		Sample center=new Sample();;
		int count=0;
		@Override
		//更新每个质心（除最后一个）
		public void reduce(IntWritable key,Iterable<Sample> values,Context context) throws IOException,InterruptedException{
			while(values.iterator().hasNext()){
				Sample value=values.iterator().next();
				if(key.get()!=prev){
					if(prev!=-1){
						for(int i=0;i<center.arr.length;i++)
							center.arr[i]/=count;		
						context.write(new IntWritable(prev), center);
					}
					center.clear();
					prev=key.get();
					count=0;
				}
				for(int i=0;i<Sample.DIMENTION;i++)
					center.arr[i]+=value.arr[i];
				count++;
			}
		}
		@Override
		//更新最后一个质心
		public void cleanup(Context context) throws IOException,InterruptedException{
			for(int i=0;i<center.arr.length;i++)
				center.arr[i]/=count;
			context.write(new IntWritable(prev), center);
		}
	}

	@Override
	public int run(String[] args) throws Exception {
		Configuration conf=getConf();
		FileSystem fs=FileSystem.get(conf);
		Job job=new Job(conf);
		job.setJarByClass(KMeans.class);
		
		//质心文件每行的第一个数字是索引
		FileInputFormat.setInputPaths(job, "/user/orisun/input/centers");
		Path outDir=new Path("/user/orisun/output");
		fs.delete(outDir,true);
		FileOutputFormat.setOutputPath(job, outDir);
		
		job.setInputFormatClass(TextInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		job.setMapperClass(ClusterMapper.class);
		job.setReducerClass(UpdateCenterReducer.class);
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Sample.class);
		
		return job.waitForCompletion(true)?0:1;
	}
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		FileSystem fs=FileSystem.get(conf);
		
		//样本数据文件中每个样本不需要标记索引
		Path dataFile=new Path("/user/orisun/input/data");
		DistributedCache.addCacheFile(dataFile.toUri(), conf);

		int iteration = 0;
		int success = 1;
		do {
			success ^= ToolRunner.run(conf, new KMeans(), args);
			log.info("iteration "+iteration+" end");
		} while (success == 1 && iteration++ < MAXITERATIONS
				&& (!stopIteration(conf)));
		log.info("Success.Iteration=" + iteration);
		
		//迭代完成后再执行一次mapper，输出每个样本点所属的分类--在/user/orisun/output2/part-m-00000中
		//质心文件保存在/user/orisun/input/centers中
		Job job=new Job(conf);
		job.setJarByClass(KMeans.class);
		
		FileInputFormat.setInputPaths(job, "/user/orisun/input/centers");
		Path outDir=new Path("/user/orisun/output2");
		fs.delete(outDir,true);
		FileOutputFormat.setOutputPath(job, outDir);
		
		job.setInputFormatClass(TextInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		job.setMapperClass(ClusterMapper.class);
		job.setNumReduceTasks(0);
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Sample.class);
		
		job.waitForCompletion(true);
	}
}


注意在Driver中创建Job实例时一定要把Configuration类型的参数传递进去，否则在Mapper或Reducer中调用DistributedCache.getLocalCacheFiles(context.getConfiguration());返回值就为null。因为空构造函数的Job采用的Configuration是从hadoop的配置文件中读出来的（使用new Configuration()创建的Configuration就是从hadoop的配置文件中读出来的），请注意在main()函数中有一句：DistributedCache.addCacheFile(dataFile.toUri(), conf);即此时的Configuration中多了一个DistributedCacheFile，所以你需要把这个Configuration传递给Job构造函数，如果传递默认的Configuration，那在Job中当然不知道DistributedCacheFile的存在了。

### Further

方案三还是不如人意，质心文件是很小的（因为质心总共就没几个），用map()函数仅仅是来读一个质心文件根本就没有发挥并行的作用，而且在map()中也没有调用context.write()，所以Mapper中做的事情可以放在Reducer的setup()中来完成，这样就不需要Mapper了，或者说上面设计的就不是MapReduce程序，跟平常的单线程串行程序是一样的。sigh












