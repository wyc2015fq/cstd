# hive指定多个字符作为列分隔符的问题说明 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月20日 16:11:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6108
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、问题：HDFS文件上列分隔符是##，hive建表时直接用##，发现输出的字段和文件不一致。

      建表语句如下：



```
ROW FORMAT DELIMITED 
  FIELDS TERMINATED BY '##' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.mapred.TextInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'
LOCATION
  'hdfs://nameservice-ha/pgw/gz'
```




2、原因：hive创建表指定分隔符时，不支持多个字符作为分隔符。

     上述就只能用#，简单解决办法就是写个MR程序将两个##改成一个#。




3、解决：Hive要支持多个字符作为分割符，需要自定义InputFormat.，重写next方法。

     代码如下：



```java
package com.hive;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.FileSplit;
import org.apache.hadoop.mapred.InputSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.JobConfigurable;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.mapred.TextInputFormat;

public class DefTextInputFormat extends TextInputFormat implements JobConfigurable {

	public RecordReader<LongWritable, Text> getRecordReader(InputSplit genericSplit, JobConf job, Reporter reporter) throws IOException {
		reporter.setStatus(genericSplit.toString());
		return new DefRecordReader((FileSplit)genericSplit, job);
	}
}
```






```java
package com.hive;

import java.io.IOException;
import java.io.InputStream;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.CompressionCodecFactory;
import org.apache.hadoop.mapred.FileSplit;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.util.LineReader;

public class DefRecordReader implements RecordReader<LongWritable, Text> {

    private CompressionCodecFactory compressionCodecs = null;
    private long start;
    private long pos;
    private long end;
    private LineReader lineReader;
    int maxLineLength;

    // 构造方法
    public DefRecordReader(FileSplit inputSplit, Configuration job) throws IOException {
        maxLineLength = job.getInt("mapred.mutilCharRecordReader.maxlength",
                Integer.MAX_VALUE);
        start = inputSplit.getStart();
        end = start + inputSplit.getLength();
        final Path file = inputSplit.getPath();
        // 创建压缩器
        compressionCodecs = new CompressionCodecFactory(job);
        final CompressionCodec codec = compressionCodecs.getCodec(file);
        // 打开文件系统
        FileSystem fs = file.getFileSystem(job);
        FSDataInputStream fileIn = fs.open(file);
        boolean skipFirstLine = false;

        if (codec != null) {
            lineReader = new LineReader(codec.createInputStream(fileIn), job);
            end = Long.MAX_VALUE;
        } else {
            if (start != 0) {
                skipFirstLine = true;
                --start;
                fileIn.seek(start);
            }
            lineReader = new LineReader(fileIn, job);
        }

        if (skipFirstLine) {
            start += lineReader.readLine(new Text(), 0,
                    (int) Math.min((long) Integer.MAX_VALUE, end - start));
        }
        this.pos = start;
    }

    public DefRecordReader(InputStream in, long offset, long endOffset, int maxLineLength) {
        this.maxLineLength = maxLineLength;
        this.start = offset;
        this.lineReader = new LineReader(in);
        this.pos = offset;
        this.end = endOffset;
    }

    public DefRecordReader(InputStream in, long offset, long endOffset,  Configuration job) throws IOException {
        this.maxLineLength = job.getInt(
                "mapred.mutilCharRecordReader.maxlength", Integer.MAX_VALUE);
        this.lineReader = new LineReader(in, job);
        this.start = offset;
        this.end = endOffset;
    }

    @Override
    public void close() throws IOException {
        if (lineReader != null)
            lineReader.close();
    }

    @Override
    public LongWritable createKey() {
        return new LongWritable();
    }

    @Override
    public Text createValue() {
        return new Text();
    }

    @Override
    public long getPos() throws IOException {
        return pos;
    }

    @Override
    public float getProgress() throws IOException {
        if (start == end) {
            return 0.0f;
        } else {
            return Math.min(1.0f, (pos - start) / (float) (end - start));
        }
    }

    @Override
    //重构next方法，处理行中字符，将多个列分割字符变成1个列分割字符
    public boolean next(LongWritable key, Text value) throws IOException {
        while (pos < end) {
            key.set(pos);
            int newSize = lineReader.readLine(value, maxLineLength,
                    Math.max((int) Math.min(Integer.MAX_VALUE, end - pos),
                            maxLineLength));
            // 把字符串中的"##"转变为"#"
            String strReplace = value.toString().replace("##", "#");
            Text txtReplace = new Text();
            txtReplace.set(strReplace);
            value.set(txtReplace.getBytes(), 0, txtReplace.getLength());
            if (newSize == 0)
                return false;
            pos += newSize;
            if (newSize < maxLineLength)
                return true;

        }
        return false;
    }
}
```


在建表时，指定com.hive.DefTextInputFormat类为INPUTFORMAT 。

当然要先将这两个类打包成jar部署到Hive的运行环境中，可参考http://blog.csdn.net/fjssharpsword/article/details/70271671



