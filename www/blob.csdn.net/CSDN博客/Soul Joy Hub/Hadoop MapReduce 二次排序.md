# Hadoop MapReduce 二次排序 - Soul Joy Hub - CSDN博客

2016年08月05日 15:27:37[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：418标签：[mapreduce																[hadoop																[hadoop mapreduce](https://so.csdn.net/so/search/s.do?q=hadoop mapreduce&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


```java
package SecondarySort;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Set;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.RawComparator;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class SecondarySort {

    public static class IntPair implements WritableComparable<IntPair> {
        int first, second;

        public void set(int left, int right) {
            first = left;
            second = right;
        }

        public int getFirst() {
            return first;
        }

        public int getSecond() {
            return second;
        }

        @Override
        public void readFields(DataInput arg0) throws IOException {
            // TODO Auto-generated method stub
            first = arg0.readInt();
            second = arg0.readInt();
        }

        @Override
        public void write(DataOutput arg0) throws IOException {
            // TODO Auto-generated method stub
            arg0.writeInt(first);
            arg0.writeInt(second);
        }

        // 关键：自定义类型的比较方法
        @Override
        public int compareTo(IntPair arg0) {
            // TODO Auto-generated method stub
            if (first != arg0.first) {
                return first < arg0.first ? -1 : 1;
            } else if (second != arg0.second) {
                return second < arg0.second ? -1 : 1;
            } else {
                return 0;
            }
        }

        public int hashCode() {
            return first * 157 + second;
        }

        public boolean equals(Object right) {
            if (right == null)
                return false;
            if (this == right)
                return true;
            if (right instanceof IntPair) {
                IntPair r = (IntPair) right;
                return r.first == first && r.second == second;
            } else {
                return false;
            }
        }
    }

    public static class FirstPartitioner extends
            Partitioner<IntPair, IntWritable> { // 类型要和Mapper输出的一样

        @Override
        public int getPartition(IntPair arg0, IntWritable arg1, int arg2) {
            // TODO Auto-generated method stub
            return Math.abs((arg0.getFirst() * 127) % arg2);
        }

    }

    /*
     * 第一种方法，实现接口RawComparator 数据类型的比较在MapReduce中式及其重要的,
     * Mapreduce中有一个排序阶段，key和其他的key相比较。 针对此，Hadoop 提供的一个优化是 RawComparator
     * 
     * public static class GroupingComparator implements RawComparator<IntPair>{
     * 
     * @Override public int compare(IntPair arg0, IntPair arg1) { // TODO
     * Auto-generated method stub int l = arg0.getFirst(); int r =
     * arg0.getFirst(); return l == r ? 0 : (l < r ? -1 : 1); }
     * 
     * @Override public int compare(byte[] b1, int s1, int l1, byte[] b2, int
     * s2, int l2) { return WritableComparator.compareBytes(b1, s1,
     * Integer.SIZE/8, b2, s2, Integer.SIZE/8); } }
     */

    // 方法二
    public static class GroupingComparator extends WritableComparator {
        protected GroupingComparator() {
            super(IntPair.class, true);// 调用父类的构造函数
        }

        public int compare(WritableComparable w1, WritableComparable w2) {
            IntPair i1 = (IntPair) w1;
            IntPair i2 = (IntPair) w2;
            int l = i1.getFirst();
            int r = i2.getFirst();
            return l == r ? 0 : (l < r ? -1 : 1);
        }
    }

    public static class Map extends
            Mapper<LongWritable, Text, IntPair, IntWritable> {
        private final IntPair ip = new IntPair();
        private final IntWritable iw = new IntWritable();

        public void map(LongWritable key, Text value, Context context)
                throws IOException, InterruptedException {
            String line = value.toString();
            StringTokenizer st = new StringTokenizer(line);
            int left = 0;
            int right = 0;
            if (st.hasMoreTokens()) {
                left = Integer.parseInt(st.nextToken());
                if (st.hasMoreTokens()) {
                    right = Integer.parseInt(st.nextToken());
                }
                ip.set(left, right);
                iw.set(right);
                context.write(ip, iw);
            }
        }
    }

    public static class Reduce extends
            Reducer<IntPair, IntWritable, Text, IntWritable> {
        private final Text left = new Text();
        private static final Text SEPARATOR = new Text(
                "======================================");

        public void reduce(IntPair key, Iterable<IntWritable> values,
                Context context) throws IOException, InterruptedException {
            context.write(SEPARATOR, null);
            left.set(Integer.toString(key.getFirst()));
            for (IntWritable val : values) {
                context.write(left, val);
            }
        }
    }

    public static void main(String[] args) throws IllegalArgumentException,
            IOException, ClassNotFoundException, InterruptedException {
        // TODO Auto-generated method stub
        Configuration conf = new Configuration();
        @SuppressWarnings("deprecation")
        Job job = new Job(conf, "mysecondarysort");
        job.setJarByClass(SecondarySort.class);
        job.setMapperClass(Map.class);
        job.setReducerClass(Reduce.class);
        job.setPartitionerClass(FirstPartitioner.class);

        job.setGroupingComparatorClass(GroupingComparator.class);

        job.setMapOutputKeyClass(IntPair.class);
        job.setMapOutputValueClass(IntWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        job.setInputFormatClass(TextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        FileInputFormat.setInputPaths(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
```

