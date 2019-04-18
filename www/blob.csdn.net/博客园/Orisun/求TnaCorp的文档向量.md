# 求TnaCorp的文档向量 - Orisun - 博客园







# [求TnaCorp的文档向量](https://www.cnblogs.com/zhangchaoyang/articles/2355397.html)





我下载的是[TanCorp-12预处理格式](http://www.searchforum.org.cn/tansongbo/corpus/TanCorp-12-Txt.rar)

step1.编码转换

刚下载下来的语料库编码是cp936即gb2312，所以下先转换成utf-8编码。转码工作在windows下进行，用到一个工具[iconv.exe](http://www.w3cool.com/2007/12/18/iconv_for_windows.html).

iconv *.txt -p E:\\TanCorp-12-Txt -f gb2312 -t utf-8 -v -s

step2.去掉文件的前3个字节

utf-8格式一个汉字是3个字节。Windows下记事本在保存utf-8文件时会在文件头加入3个字节：efbbbf--在上一步使用了iconv.exe后文开头也多出了这3个字节。在Linux下你可以用hexdump -C file查看文件的十六进制编码，或在vim中用:%!xxd也可以。

去掉文件的前3个字节方法是：先获取文件长度size，然后tail -c size-3 file > newfile即可。

#!/bin/bash
OPATH="TanCorp-12-Txt"
DPATH="/tmp"

find $OPATH -type d -exec mkdir -pv ${DPATH}/{} \;
find $OPATH -type f > tmpf
while read FILE
do
    newf=${DPATH}/$FILE
    touch $newf
    size=$(wc -c $FILE|sed 's/ .*//')
    let need=size-3 
	tail -c $need $FILE > $newf
done < tmpf
rm tmpf
echo "all done"


获取shell输出到标准输出上的值可以有两种方法--请注意第11行代码，这实际上也是sed向shell传值的方法：

1.size=$(wc -c $FILE|sed 's/ .*//')

2.size=`wc -c $FILE|sed 's/ .*//'`

step3.让文件名中包含它所属的类别

#!/bin/bash
DIR="TanCorp-12-Txt"

function process
{
	if [ $1 = $DIR ]
	then
		return
	fi
    count=0
    pn=$1
    class1=${pn#*/}			#先把第一个slash及其左边的内容去掉
    class=${class1%%-*}		#再把第一个-及其右边的内容去掉
	find $1 -type f > truef
	while read TF
	do
		mv $TF ${pn}/${class}-$((count=count+1))
	done < truef
	rm truef
}

find $DIR -type d > tmpf
while read FILE
do
    process $FILE
done < tmpf
rm tmpf
echo "all done"


step4.把单个文件合并为一行.

由于语料库每行末尾已经有了一个空格，所我我们只需要把换行符去掉即可。注意在TanCorp中换行用的是"\r\n"

#!/bin/bash
SPATH="/home/orisun"
DIR="TanCorp-12-Txt"
DPATH="/tmp"

find ${DIR} -type d -exec mkdir -pv ${DPATH}/{} \;
find ${DIR} -type f > tmp
while read FILE
do
	cat $FILE | tr -d "\r\n" > ${DPATH}/${FILE}
#	awk '{printf "%s",$0 >>bakfile}' bakfile=${DPATH}/${FILE} $FILE	#不用tr用awk也可以实现
done < tmp
rm tmp
echo "all done"


step5.统计文档中词汇出现的频数

#!/bin/bash
OPATH="TanCorp-12-Txt"
DPATH="/tmp"

find $OPATH -type d -exec mkdir -pv ${DPATH}/{} \;
find $OPATH -type f > tmpf
while read FILE
do
    dest=${DPATH}/${FILE}
    bname=${dest##*/}		#拿掉最后一个／及其左边的内容,即获取basename
    touch $dest
    awk 'BEGIN{RS=" ";}
   		{
			if(NR==1)
			{
				previous=fn;
			}
			if ($1!=previous)
			{
				printf "%s %d\n",previous,count>>newfile;
				previous=$1;
				count=1;
			}
			else
			{
				count++;
			}
    	}
    	END{printf "%s %d\n",previous,count>>newfile;}' newfile=$dest fn=$bname $FILE
done < tmpf
rm tmpf
echo "all done"


step6.去掉不成词的单个汉字

#!/bin/bash
DIR="TanCorp-12-Txt-3"
DPATH="/tmp"

find $DIR -type d -exec mkdir -pv ${DPATH}/{} \;
find $DIR -type f > tmpf
while read FILE
do
    sed '/^. /'d $FILE > ${DPATH}/$FILE
done < tmpf
rm tmpf
echo "all done"


step7.统计每个词的文档频率

为避免内存溢出的问题，这一步采用hadoop来完成。先对step6得到的结果进行以下处理，但是step6得到的结果还要保存下来，以后还要用。下面的操作是把文档合并成一行，去掉频数词。

#!/bin/bash
DIR="TanCorp-12-Txt-5"
DPATH="/tmp"

find $DIR -type d -exec mkdir -pv ${DPATH}/{} \;
find $DIR -type f > tmpf
while read FILE
do
   dest=${DPATH}/$FILE
   awk 'BEGIN{ORS=" ";} {print $1 >> fn;}' fn=$dest $FILE
done < tmpf
rm tmpf
echo "all done"


这样文档中的第一个词是文档名，每个词在文档中出现一次。在map阶段遇到一个词先判断在不在停用词表中，在就忽略，否则就抛出(单词，文档名)；在reduce阶段统计一个词在多少个文档中出现过（专业术语叫做文档频率），如何文档频率小于4则忽略，否则把(单词，文档频率，包含单词的文档列表)写入文件。

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

public class DF {
	static HashSet<String> stopwords = new HashSet<String>();// 停用词集合
	// 初始化停用词表
	public static void init_SW(String filePath) {
		stopwords.clear();
		File swFile = new File(filePath);
		if (!swFile.exists()) {
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

	static class DFMapper extends Mapper<LongWritable, Text, Text, Text> {
		public void map(LongWritable key, Text value, Context context)
				throws IOException, InterruptedException {
			String line = value.toString();
			String[] words = line.split("\\s+");
			String filename = words[0];
			for (int i = 1; i < words.length; i++) {
				if (!stopwords.contains(words[i])) {		//不能是停用词
					context.write(new Text(words[i]), new Text(filename));
				}
			}
		}
	}

	static class DFReducer extends Reducer<Text, Text, Text, Text> {
		public void reduce(Text key, Iterable<Text> fns, Context context)
				throws IOException, InterruptedException {
			ArrayList<String> al = new ArrayList<String>();
			StringBuffer buf = new StringBuffer();
			for (Text fn : fns) {
				al.add(fn.toString());
				buf.append(fn.toString()+" ");
			}
			int count=al.size();
			if(count<4)			//如果文档频率小于4,则认为不可能成为特征词
				return;
			context.write(key, new Text(String.valueOf(count)+" "+buf.toString()));
		}
	}

	public static void main(String[] args) throws Exception {
		init_SW("/home/orisun/stopword");
		Path inputPath1 = new Path("/home/orisun/TanCorp-12-Txt-6/c1-education");
		Path inputPath2 = new Path("/home/orisun/TanCorp-12-Txt-6/c2-entertainment");
		Path inputPath3 = new Path("/home/orisun/TanCorp-12-Txt-6/c3-health");
		Path inputPath4 = new Path("/home/orisun/TanCorp-12-Txt-6/c4-house");
		Path inputPath5 = new Path("/home/orisun/TanCorp-12-Txt-6/c5-region");
		Path inputPath6 = new Path("/home/orisun/TanCorp-12-Txt-6/c6-sports");
		Path inputPath7 = new Path("/home/orisun/TanCorp-12-Txt-6/c7-talents");
		// 输出路径在程序运行前不能存在
		Path outputPath = new Path("/home/orisun/matrix");
		Job job = new Job();
		job.setJarByClass(DF.class);
		FileInputFormat.addInputPath(job, inputPath1);
		FileInputFormat.addInputPath(job, inputPath2);
		FileInputFormat.addInputPath(job, inputPath3);
		FileInputFormat.addInputPath(job, inputPath4);
		FileInputFormat.addInputPath(job, inputPath5);
		FileInputFormat.addInputPath(job, inputPath6);
		FileInputFormat.addInputPath(job, inputPath7);
		FileOutputFormat.setOutputPath(job, outputPath);
		job.setMapperClass(DFMapper.class);
		job.setReducerClass(DFReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}


step8.计算信息增益，选取特征项

计算以下工作之前，你应该把TanCorp分成训练集和测试集，step8的工作是在训练集上进行的。

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

public class CalIG {

	public void calIG(File matrixFile, File IGFile) {
		if (!matrixFile.exists()) {
			System.out.println("Matrix文件不存在.程序退出.");
			System.exit(2);
		}

		int category_num = 7; // 一共有7大分类
		int[] category_count = { 115,750,703,468,75,1403,304}; // 每个分类包含的文档数
		int doc_num = 0; // 全部文档数量
		for (int i = 0; i < category_num; i++)
			doc_num += category_count[i];

		double HC = getEntropy(category_count);

		try {
			FileReader fr = new FileReader(matrixFile);
			BufferedReader br = new BufferedReader(fr);
			PrintWriter pw = new PrintWriter(new FileOutputStream(IGFile));
			String line = null;
			while ((line = br.readLine()) != null) {
				String[] content = line.split("\\s+");
				int len = content.length;
				String term = content[0];
				int term_count = Integer.parseInt(content[1]); // 出现term的文档数量
				int[] term_class_count = new int[category_num];// 每个类别中出现term的文档数量
				int[] term_b_class_count = new int[category_num];// 每个类别中不出现term的文档数量
				for (int i = 2; i < len; i++) {
					String catstr = content[i].split("-")[0];
					int catint = Integer.parseInt(catstr.substring(1));
					term_class_count[catint - 1]++;
				}
				for (int i = 0; i < category_num; i++) {
					term_b_class_count[i] = category_count[i]
							- term_class_count[i];
				}

				double HCT = 1.0 * term_count / doc_num
						* getEntropy(term_class_count) + 1.0
						* (doc_num - term_count) / doc_num
						* getEntropy(term_b_class_count);
				double IG = HC - HCT;

				pw.println(term + "\t" + String.valueOf(term_count) + "\t"
						+ String.valueOf(IG));
				pw.flush();
			}
			br.close();
			pw.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public double getEntropy(int[] arr) {
		int sum = 0;
		double entropy = 0.0;
		for (int i = 0; i < arr.length; i++) {
			sum += arr[i];
			entropy += arr[i] * Math.log(arr[i] + Double.MIN_VALUE)
					/ Math.log(2);
		}
		entropy /= sum;
		entropy -= Math.log(sum) / Math.log(2);
		return 0 - entropy;
	}

	public static void main(String[] args) throws Exception {
		Date currentTime = new Date();
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		System.out.println("Begin Time: " + formatter.format(currentTime));

		CalIG inst = new CalIG();
		File in = new File("/home/orisun/matrix/part-r-00000");
		File out = new File("/home/orisun/frequency1");
		inst.calIG(in, out);

		currentTime = new Date();
		System.out.println("End Time: " + formatter.format(currentTime));
	}
}


此时保存在/home/orisun/frequency1中的是：（单词  文档频率  信息增益值），我们要按照第3列对文件进行逆序（从大到小）排序：

sort -n -r -k3 frequency1 > frequency

注意sort命令按数字进行排序时不能识别科学计数法，即它认为9.9E-4比0.1大。

我们保留IG值最大的2000个词，其他的可以从frequency文件中删掉。

step9.按照TF-IDF计算文档向量

对于训练集

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.HashMap;

public class Train_Vector {
	final static int N=3818;		//训练集文档总数目
	static int feanum=700;		//700个特征项
	static HashMap<String,Integer> feamap=new HashMap<String,Integer>();
	static int[] feafd=new int[feanum];
	
	static void initFea(File ff){
		if (!ff.exists()) {
			System.out.println("Matrix文件不存在.程序退出.");
			System.exit(2);
		}
		try{
			FileReader fr = new FileReader(ff);
			BufferedReader br = new BufferedReader(fr);
			String line=null;
			for(int i=0;i<feanum;i++){
				line=br.readLine();
				String[] conts=line.split("\\s+");
				feamap.put(conts[0], i);
				feafd[i]=Integer.parseInt(conts[1]);
			}
			br.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	static void calvec(File srcFile){
		if (!srcFile.exists()) {
			System.out.println("Matrix文件不存在.程序退出.");
			System.exit(2);
		}
		if(srcFile.isDirectory()){
			File[] childFiles=srcFile.listFiles();
			for(File child:childFiles){
				calvec(child);
			}
		}
		else if(srcFile.isFile()){
			double[] arr=new double[feanum];
			int df,tf,index;
			try{
				FileReader fr=new FileReader(srcFile);
				BufferedReader br=new BufferedReader(fr);
				String line=br.readLine();		//跳过第一行，因为第一行存储的是文件名
				while((line=br.readLine())!=null){
					String[] conts=line.split("\\s+");
					if(feamap.containsKey(conts[0])){
						tf=Integer.parseInt(conts[1]);
						index=feamap.get(conts[0]);
						df=feafd[index];
						arr[index]=Math.log(tf+1.0)*Math.log(N/df)/(Math.log(2.0)*Math.log(2.0));
					}
				}
				br.close();
			}catch (Exception e){
				e.printStackTrace();
			}
			double fenmu=0.0;
			for(int i=0;i<feanum;i++)
				fenmu+=Math.pow(arr[i], 2.0);
			fenmu=Math.pow(fenmu, 0.5);
			try{
				File out=new File(srcFile.getAbsoluteFile()+".vec");
				PrintWriter pw = new PrintWriter(new FileOutputStream(out));
				for(int i=0;i<feanum;i++)
					pw.printf("%f\t", arr[i]/fenmu);
				pw.flush();
				pw.close();
			}catch (Exception e){
				e.printStackTrace();
			}
			
		}
	}
	public static void main(String[]args)throws Exception {
		initFea(new File("/home/orisun/feature"));
		calvec(new File("/home/orisun/TanCorp-12-Txt-5"));
	}
}


对于测试集，特征项的权重则直接用w=tf*IG，对以上代码作轻微发动即可。

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.HashMap;

public class TestVector {
	static int feanum=700;		//700个特征项
	static HashMap<String,Integer> feamap=new HashMap<String,Integer>();
	static double[] feaig=new double[feanum];
	
	static void initFea(File ff){
		if (!ff.exists()) {
			System.out.println("Matrix文件不存在.程序退出.");
			System.exit(2);
		}
		try{
			FileReader fr = new FileReader(ff);
			BufferedReader br = new BufferedReader(fr);
			String line=null;
			for(int i=0;i<feanum;i++){
				line=br.readLine();
				String[] conts=line.split("\\s+");
				feamap.put(conts[0], i);
				feaig[i]=Double.parseDouble(conts[2]);
			}
			br.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	static void calvec(File srcFile){
		if (!srcFile.exists()) {
			System.out.println("Matrix文件不存在.程序退出.");
			System.exit(2);
		}
		if(srcFile.isDirectory()){
			File[] childFiles=srcFile.listFiles();
			for(File child:childFiles){
				calvec(child);
			}
		}
		else if(srcFile.isFile()){
			double[] arr=new double[feanum];
			int tf,index;
			double ig;
			try{
				FileReader fr=new FileReader(srcFile);
				BufferedReader br=new BufferedReader(fr);
				String line=br.readLine();		//跳过第一行，因为第一行存储的是文件名
				while((line=br.readLine())!=null){
					String[] conts=line.split("\\s+");
					if(feamap.containsKey(conts[0])){
						tf=Integer.parseInt(conts[1]);
						index=feamap.get(conts[0]);
						ig=feaig[index];
						arr[index]=tf*ig;
					}
				}
				br.close();
			}catch (Exception e){
				e.printStackTrace();
			}
			try{
				File out=new File(srcFile.getAbsoluteFile()+".vec");
				PrintWriter pw = new PrintWriter(new FileOutputStream(out));
				for(int i=0;i<feanum;i++)
					pw.printf("%f\t", arr[i]);
				pw.flush();
				pw.close();
			}catch (Exception e){
				e.printStackTrace();
			}
			
		}
	}
	public static void main(String[]args)throws Exception {
		initFea(new File("/home/orisun/feature"));
		calvec(new File("/home/orisun/TanCorp-5-Test"));
	}
}


我们把向量文档挪到一个单独的文件夹下面去：

#!/bin/bash
DIR="TanCorp-12-Txt-5"
DPATH="/tmp"

find $DIR -type d -exec mkdir -pv /tmp/{} \;
find $DIR -type f -name *.vec -exec mv {} ${DPATH}/{} \;


step10.文本分类或聚类

测试文档和训练文档都已经转换成向量的形式了，至于用什么算法进行文件分类或聚类就看你的了。












