# MapReduce Join - Soul Joy Hub - CSDN博客

2016年08月08日 13:58:22[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：347标签：[mapreduce																[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


# Map Side Join

```java
package MapJoin;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.net.URI;
import java.util.HashMap;
import java.util.Map;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

/*
 * 
 Table1
 011990-99999    SIHCCAJAVRI
 012650-99999    TYNSET-HANSMOEN

 Table2
 012650-99999    194903241200    111
 012650-99999    194903241800    78
 011990-99999    195005150700    0
 011990-99999    195005151200    22
 011990-99999    195005151800    -11
 * */

public class MapJoin {
    static class mapper extends Mapper<LongWritable, Text, Text, Text> {
        private Map<String, String> Table1Map = new HashMap<String, String>();

        // 将小表读到内存HashMap中
        protected void setup(Context context) throws IOException {
            URI[] paths = context.getCacheFiles();

            Configuration conf = new Configuration();
            FileSystem fs = FileSystem.get(conf);
            FSDataInputStream fsr = fs.open(new Path(paths[0].toString()));
            // BufferedReader br = new BufferedReader(new FileReader(
            // paths[0].toString()));
            String line = null;
            try {
                while ((line = fsr.readLine().toString()) != null) {
                    String[] vals = line.split("\\t");
                    if (vals.length == 2) {
                        Table1Map.put(vals[0], vals[1]);
                    }
                }
            } catch (Exception e) {
                // TODO: handle exception
                e.printStackTrace();
            } finally {
                fsr.close();
            }
        }

        // 对大表进行Map扫描
        protected void map(LongWritable key, Text val, Context context)
                throws IOException, InterruptedException {
            String[] vals = val.toString().split("\\t");
            if (vals.length == 3) {
                // 每条记录都用外键对HashMap get
                String Table1Vals = Table1Map.get(vals[0]);
                Table1Vals = (Table1Vals == null) ? "" : Table1Vals;
                context.write(new Text(vals[0]), new Text(Table1Vals + "\t"
                        + vals[1] + "\t" + vals[2]));
            }
        }
    }

    public static void main(String[] args) throws IOException,
            ClassNotFoundException, InterruptedException {
        Configuration conf = new Configuration();

        String[] otherArgs = new GenericOptionsParser(conf, args)
                .getRemainingArgs();
        if (otherArgs.length != 3) {
            System.err
                    .println("Parameter number is wrong, please enter three parameters：<big table hdfs input> <small table local input> <hdfs output>");
            System.exit(-1);
        }

        Job job = new Job(conf, "MapJoin");

        job.setJarByClass(MapJoin.class);
        job.setMapperClass(mapper.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        job.addCacheFile((new Path(args[1]).toUri()));
        FileOutputFormat.setOutputPath(job, new Path(args[2]));

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

# Reduce Side Join

```java
package ReduceJoin;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

/*user.csv文件：

"ID","NAME","SEX"
"1","user1","0"
"2","user2","0"
"3","user3","0"
"4","user4","1"
"5","user5","0"
"6","user6","0"
"7","user7","1"
"8","user8","0"
"9","user9","0"

order.csv文件：

"USER_ID","NAME"
"1","order1"
"2","order2"
"3","order3"
"4","order4"
"7","order7"
"8","order8"
"9","order9"
*/

public class ReduceJoin {

    public static class MapClass extends   
    Mapper<LongWritable, Text, Text, Text>  
{  

    //最好在map方法外定义变量，以减少map计算时创建对象的个数  
    private Text key = new Text();  
    private Text value = new Text();  
    private String[] keyValue = null;  

    @Override  
    protected void map(LongWritable key, Text value, Context context)  
        throws IOException, InterruptedException  
    {  
        //value是每一行的内容,Text类型，所有我们要把key从value中解析出来  
        keyValue = value.toString().split(",", 2);  
        this.key.set(keyValue[0]);  //把外键设为MapReduce key
        this.value.set(keyValue[1]);  
        context.write(this.key, this.value);  
    }  

}  

public static class Reduce extends Reducer<Text, Text, Text, Text>  
{  

    private Text value = new Text();  

    @Override  
    protected void reduce(Text key, Iterable<Text> values, Context context)  
            throws IOException, InterruptedException  
    {  
        StringBuilder valueStr = new StringBuilder();  

        //values中的每一个值是不同数据文件中的具有相同key的值  
        //即是map中输出的多个文件相同key的value值集合  
        for(Text val : values)  
        {  
            valueStr.append(val);  
            valueStr.append(",");  
        }  

        this.value.set(valueStr.deleteCharAt(valueStr.length()-1).toString());  
        context.write(key, this.value);  
    }  

}  

    public static void main(String[] args) throws IllegalArgumentException, IOException, ClassNotFoundException, InterruptedException {
        // TODO Auto-generated method stub
        Configuration conf = new Configuration();  
        Job job = new Job(conf, "MyJoin");  

        job.setJarByClass(ReduceJoin.class);  
        job.setMapperClass(MapClass.class);  
        job.setReducerClass(Reduce.class);  
        //job.setCombinerClass(Reduce.class);  

        job.setOutputKeyClass(Text.class);  
        job.setOutputValueClass(Text.class);  

        job.setInputFormatClass(TextInputFormat.class);  
        job.setOutputFormatClass(TextOutputFormat.class);  

        FileInputFormat.addInputPath(job, new Path(args[0]));  
        FileOutputFormat.setOutputPath(job, new Path(args[1]));  

        System.exit(job.waitForCompletion(true) ? 0 : 1);  
    }

}
```

