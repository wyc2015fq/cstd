# MapReduce：随机生成100个小数并求最大值 - Soul Joy Hub - CSDN博客

2016年08月02日 20:21:02[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：685


# 自定义类

在编写MapReduce的时候，自带的输入格式有时候满足不了我们的需求，这就需要自己定义InputFormat，InputSplit和RecordReader。

## FindMaxValueInputSplit

```java
package FindMaxValue;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapreduce.InputSplit;

public class FindMaxValueInputSplit extends InputSplit implements Writable{

    private int m_StartIndex;
    private int  m_EndIndex;
    private ArrayWritable m_FloatArray = new ArrayWritable(FloatWritable.class);
    public FindMaxValueInputSplit() {
        // TODO Auto-generated constructor stub
    }
    public FindMaxValueInputSplit(int start,int end){
        m_StartIndex = start;
        m_EndIndex = end;
        int len = m_EndIndex - m_StartIndex + 1;
        int index = m_StartIndex;
        FloatWritable[] result = new FloatWritable[len];
        for(int i = 0 ; i < len ;++i)
        {
            float f = FindMaxValueInputFormat.floatvalues[index];
            FloatWritable fw = new FloatWritable(f);
            result[i] = fw;
            ++index;
        }
        m_FloatArray.set(result);
    }
    @Override
    public void readFields(DataInput arg0) throws IOException {
        // TODO Auto-generated method stub
        this.m_StartIndex = arg0.readInt();
        this.m_EndIndex = arg0.readInt();
        this.m_FloatArray .readFields(arg0);
    }

    @Override
    public void write(DataOutput arg0) throws IOException {
        // TODO Auto-generated method stub
        arg0.writeInt(this.m_StartIndex);
        arg0.writeInt(this.m_EndIndex);
        this.m_FloatArray.write(arg0);
    }

    @Override
    public long getLength() throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        return (this.m_EndIndex - this.m_StartIndex +1);
    }

    @Override
    public String[] getLocations() throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        return new String[]  {"localhost","localhost"};
    }

    public int get_m_Start(){
        return m_StartIndex;
    }

    public int get_m_End(){
        return m_EndIndex;
    }

    public ArrayWritable get_floatArray(){
        return m_FloatArray;
    }

}
```

## FindMaxValueRecordReader

```java
package FindMaxValue;

import java.io.IOException;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;

public class FindMaxValueRecordReader  extends RecordReader<IntWritable, ArrayWritable>{
    private int  m_Start;
    private int m_End;
    private int m_index;
    private IntWritable key = null;
    private ArrayWritable value = null;
    private FindMaxValueInputSplit fmvis = null;
    @Override
    public void close() throws IOException {
        // TODO Auto-generated method stub

    }

    @Override
    public IntWritable getCurrentKey() throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        return key;
    }

    @Override
    public ArrayWritable getCurrentValue() throws IOException,
            InterruptedException {
        // TODO Auto-generated method stub
        return value;
    }

    @Override
    public float getProgress() throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        if(m_Start == m_End){
            return 0;
        } else{
            return Math.min(1, (this.m_index - this.m_Start) / (float)(this.m_End - this.m_Start));
        }
    }

    @Override
    public void initialize(InputSplit arg0, TaskAttemptContext arg1)
            throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        fmvis = (FindMaxValueInputSplit)arg0;
        this.m_Start = fmvis.get_m_Start();
        this.m_End = fmvis.get_m_End();
        this.m_index = this.m_Start;
    }

    @Override
    public boolean nextKeyValue() throws IOException, InterruptedException {
        // TODO Auto-generated method stub
        if(key == null){
            key = new IntWritable();
        }
        if(value == null){
            value = new ArrayWritable(FloatWritable.class);
        }
        if(m_index  <= m_End){
            key.set(m_index);
            value = fmvis.get_floatArray();
            m_index = m_End+1;
            return true;
        } else{
            return false;
        }
    }

}
```

# FindMaxValueInputFormat

```java
package FindMaxValue;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.InputFormat;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.JobContext;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;

public class FindMaxValueInputFormat  extends InputFormat<IntWritable, ArrayWritable>{

    public static float [] floatvalues;
    @Override
    public RecordReader<IntWritable, ArrayWritable> createRecordReader(
            InputSplit split, TaskAttemptContext context) throws IOException,
            InterruptedException {
        // TODO Auto-generated method stub
        return new FindMaxValueRecordReader();
    }

    @Override
    public List<InputSplit> getSplits(JobContext context) throws IOException,
            InterruptedException {
        // TODO Auto-generated method stub
        int NumofValues = context.getConfiguration().getInt("NumOfValues", 100);
        floatvalues = new float[NumofValues];
        Random random = new Random();
        for(int i = 0 ;i < NumofValues;++i){
            floatvalues[i] = random.nextFloat();
        }
        int NumofSplit = context.getConfiguration().getInt("mapred.map.tasks", 2);
        int beg = 0;

        int length = (int)Math.floor(NumofValues/NumofSplit);//尽量让每个task分配相同数据量的split

        ArrayList<InputSplit> splits = new ArrayList<InputSplit>();
        int end = length -1;
        for(int i = 0;i < NumofSplit;++i){
            FindMaxValueInputSplit split = new FindMaxValueInputSplit(beg, end);
            splits.add(split);

            beg = end+1;
            end = beg + length -1;
        }

        FindMaxValueInputSplit split = new FindMaxValueInputSplit(beg,NumofValues-1);//把剩下的数据全都放进最后一个split里面
        splits.add(split);
        return splits;
    }

}
```

# MapReduce

## Mapper

```java
package FindMaxValue;

import java.io.IOException;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class FindMaxValueMapper extends Mapper<IntWritable, ArrayWritable, IntWritable, FloatWritable>{
        public void map(IntWritable key,ArrayWritable value,Context context) throws IOException, InterruptedException{
        FloatWritable[] floatWritables = (FloatWritable[])value.toArray();
        float maxfloat = floatWritables[0].get();
        float tmp;
        //找最大的
        for(int i = 1;i < floatWritables.length;++i){
            tmp = floatWritables[i].get();
            if(tmp > maxfloat){
                maxfloat = tmp;
            }
        }
        context.write(new IntWritable(1), new FloatWritable(maxfloat));
    }
}
```

## Reducer

```java
package FindMaxValue;

import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class FindMaxValueReducer extends Reducer<IntWritable, FloatWritable, Text, FloatWritable>{
    public void reduce(IntWritable key,Iterable<FloatWritable> values,Context context) throws IOException, InterruptedException{
        Iterator<FloatWritable> it = values.iterator();
        float maxfloat = 0 , tmp;
        if(it.hasNext())
        {
            maxfloat = ((FloatWritable)(it.next())).get();
        }else
        {
            context.write(new Text("MAx is"),null);
            return ;
        }
        //找最大的
        while(it.hasNext())
        {
            tmp = ((FloatWritable)(it.next())).get();
            if(tmp > maxfloat)
            {
                maxfloat = tmp;
            }
        }       
        context.write(new Text("Max is"), new FloatWritable(maxfloat));
    }
}
```

## Main

这里不能加“job.setCombinerClass(FindMaxValueReducer.class);”，否则会报 [wrong key class](http://blog.csdn.net/u011239443/article/details/51823553) 异常。

```java
package FindMaxValue;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class MaxValueDriver {
    public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
        Configuration conf = new Configuration();
        @SuppressWarnings("deprecation")
        Job job = new Job(conf,"FindMaxValue");

        job.setJarByClass(MaxValueDriver.class);
        job.setMapperClass(FindMaxValueMapper.class);
        //job.setCombinerClass(FindMaxValueReducer.class);
        job.setReducerClass(FindMaxValueReducer.class);
        job.setInputFormatClass(FindMaxValueInputFormat.class);
        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(FloatWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(FloatWritable.class);
        FileOutputFormat.setOutputPath(job, new Path(args[0]));
        System.out.println(conf.get("mapred.job.tracker"));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

