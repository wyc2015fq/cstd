# Hadoop中WordCount - LC900730的博客 - CSDN博客
2017年12月06日 10:10:46[lc900730](https://me.csdn.net/LC900730)阅读数：175
## map()
```
/***
 * KEYIN:默认情况下，是mr框架所读到的一行文本的起始偏移量，Long,hadoop中有自己的更精简的序列化接口，不用Long，而用LongWritable
 * VALUEIN:默认情况下，是mr框架所读到的文本内容，    String Text
 * KEYOUT:用户自定义逻辑处理完成之后输出数据中的key，单词   String
 * VALUEOUT:用户自定义逻辑完成之后输出数据中的value，在此处是单词次数，Integer IntWritable
*/
public class WordcountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
    /**
     * map阶段的业务逻辑处理就写在map()方法中
     * maptask会对每一行输入数据调用一次我们自定义的map()方法
     * @throws InterruptedException 
     */
    @Override
    protected void map(LongWritable key,Text value,Context context) throws IOException, InterruptedException{
        //将maptask传递给我们的文本内容先转换成string
        String line=value.toString();
        //按照空格行切割单词
        String[] words=line.split(" ");
        //将单词输出为<单词，1>
        for(String word:words) {
            //将单词作为key，将次数1作为Value，以便于后续的数据分发，可以根据单词分发，以便于相同单词会到相同的reduce task
            context.write(new Text(word),new IntWritable(1));
        }
    }
}
```
在hadoop中有更精简的序列化接口，不用Long而用LongWritable
## reduce()
```
package cn.itcast.bigdata.mr.wcdemo;
import java.io.IOException;
import java.util.Iterator;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Mapper.Context;
/**
 * KEYIN,VALUE对应mapper输出的KEYOUT，VALUEOUT
 * KEYOUT,VALUEOUT是自定义的reduce逻辑处理结果的输出数据类型
 * @author 
 */
public class WordcountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
    /**
     * <angel,1> <angel,1> <angel,1> <angel,1> <angel,1>
     * <hello,1> <hello,1> <hello,1> <hello,1> <hello,1> <hello,1>
     * <banana,1> <banana,1> <banana,1> <banana,1> <banana,1> <banana,1>
     * 入参key：是一组单词的kv对应的key,将相同单词的一组传递，如此时key是hello，那么参数二是一个迭代器，一组数
     * @throws InterruptedException 
     * @throws IOException 
     */
    @Override
    protected void reduce(Text key,Iterable<IntWritable> values ,Context context) throws IOException, InterruptedException {
        int count=0;
/**
        Iterator<IntWritable> iterator=values.iterator();
        while(iterator.hasNext()) {
            count+=iterator.next().get();
        }
*/
        for(IntWritable value:values) {
            count+=value.get();
        }
        context.write(key, new IntWritable(count));
    }    
}
```
## 封装job对象
```
package cn.itcast.bigdata.mr.wcdemo;
import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
/**
 * 相当于一个yarn集群的客户端
 * 需要在此封装我们mr程序的相关运行参数，指定jar包，最后交给yarn
 * @author liuchao
 */
public class WordcountDriver {
    public static void main(String[] args) throws IllegalArgumentException, IOException, ClassNotFoundException, InterruptedException {
        Configuration conf=new Configuration();
        /*
         * 集群中节点都有配置文件
        conf.set("mapreduce.framework.name.", "yarn");
        conf.set("yarn.resourcemanager.hostname", "mini1");
        */
        Job job=Job.getInstance(conf);
        //jar包在哪里,现在在客户端，传递参数
        //任意运行，类加载器知道这个类的路径，就可以知道jar包所在的本地路径
        job.setJarByClass(WordcountDriver.class);
        //指定本业务job要使用的mapper/Reducer业务类
        job.setMapperClass(WordcountMapper.class);
        job.setReducerClass(WordcountReducer.class);
        //指定mapper输出数据的kv类型
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);
        //指定最终输出的数据kv类型
        job.setOutputKeyClass(Text.class);
        job.setOutputKeyClass(IntWritable.class);
        //指定job的输入原始文件所在目录
        FileInputFormat.setInputPaths(job, new Path(args[0]));
        //指定job的输出结果所在目录
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        //将job中配置的相关参数及job所用的java类在的jar包，提交给yarn去运行
        //提交之后，此时客户端代码就执行完毕，退出
        //job.submit();
        //等集群返回结果在退出
        boolean res=job.waitForCompletion(true);
        System.exit(res?0:1);
        //类似于shell中的$？
    }
}
```
