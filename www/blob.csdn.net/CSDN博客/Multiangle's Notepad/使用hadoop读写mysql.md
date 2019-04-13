
# 使用hadoop读写mysql - Multiangle's Notepad - CSDN博客


2016年11月17日 18:04:30[multiangle](https://me.csdn.net/u014595019)阅读数：1751



---与mongodb相比，hadoop提供了内置的输入输出库DBInputFormat和DBOutputFormat,但是依然要使用JDBC驱动包com.mysql.jdbc.Driver。没有的可以去

---[http://www.mysql.com/products/connector/](http://www.mysql.com/products/connector/)

---下载。下下来后，只要把该包放在项目路径下即可，切莫在文件中import该包中内容，只要import java.sql.* 相关包内容即可。

---自定义类实现Writable和DBWritable接口

---将驱动导入之后，还需要自己写一个MySQL与MapReduce之间交互的类，需要实现Writable和DBWritable接口

---，Writable是为了与MapReduce进行对接，而DBWritable是为了与MySQL进行对接。这两个功能要实现的函数是比较类似的，都要实现write和readFields函数。作用顾名思义，前者用于将一条数据写入数据流或者数据库中，而后者则从数据流/库中读取数据。

---值得注意的是，如果你选择自己写初始化函数，那么这个类必须要有一个空白的初始化函数，不然会报错。

---如果我要输入的数据库如下面所示

![选区_042.png-12.2kB](http://static.zybuluo.com/multiangle/6jly82wyawygdbunw0zy3ake/%E9%80%89%E5%8C%BA_042.png)
---[ ](http://static.zybuluo.com/multiangle/6jly82wyawygdbunw0zy3ake/%E9%80%89%E5%8C%BA_042.png)

---那么我的相关自定义类应该这么写：

---[

](http://static.zybuluo.com/multiangle/6jly82wyawygdbunw0zy3ake/%E9%80%89%E5%8C%BA_042.png)

---private

---static

---class

---MysqlRecord

---implements

---DBWritable

---,

---Writable

---{

---protected

---int

---size ;

---protected

---Date time ;

---protected

---int

---input_speed ;

---protected

---int

---output_speed ;

---@Override

---public

---void

---write

---(PreparedStatement preparedStatement)

---throws

---SQLException {
        preparedStatement.setInt(

---1

---,

---this

---.size);
        preparedStatement.setDate(

---2

---,

---this

---.time);
        preparedStatement.setInt(

---3

---,

---this

---.input_speed);
        preparedStatement.setInt(

---4

---,

---this

---.output_speed);
    }

---@Override

---public

---void

---readFields

---(ResultSet resultSet)

---throws

---SQLException {

---this

---.size = resultSet.getInt(

---1

---) ;

---this

---.time = resultSet.getDate(

---2

---) ;

---this

---.input_speed = resultSet.getInt(

---3

---) ;

---this

---.output_speed = resultSet.getInt(

---4

---) ;
    }

---@Override

---public

---void

---write

---(DataOutput dataOutput)

---throws

---IOException {
        dataOutput.writeInt(

---this

---.size);
        Text.writeString(dataOutput,

---this

---.time.toString()) ;
        dataOutput.writeInt(

---this

---.input_speed);
        dataOutput.writeInt(

---this

---.output_speed);
    }

---@Override

---public

---void

---readFields

---(DataInput dataInput)

---throws

---IOException {

---this

---.size = dataInput.readInt() ;

---this

---.time = Date.valueOf(Text.readString(dataInput)) ;

---this

---.input_speed = dataInput.readInt() ;

---this

---.output_speed = dataInput.readInt() ;
    }
}

---[
](http://static.zybuluo.com/multiangle/6jly82wyawygdbunw0zy3ake/%E9%80%89%E5%8C%BA_042.png)

---如上面所示，我要读取的数据共有4个字段，Writable接口是对数据流进行操作的，所以输入输出是DataInput和DataOutput类对象； DBWritable负责对数据库进行操作，所以输入格式是ResultSet，输出格式是PreparedStatement; 这两个类的用法不再赘述，注意这两者的字段是

---从1开始计数

---的。

---[
](http://static.zybuluo.com/multiangle/6jly82wyawygdbunw0zy3ake/%E9%80%89%E5%8C%BA_042.png)

---实现Mapper和Reducer

---在定义好了接口类以后，下面就开始写Mapper类和Reducer类了。我的目标是统计同一个日期中的output_speed之和，那么根据MapReduce的流程，我要在Mapper类中生成许多Text-IntWritable键值对，分别存放date和output_speed，然后hadoop会自动通过shuffle过程将所有同一date的键值队放入一个递归器

---Iterator<IntWritable>

---里面，输入到Reducer类中。

---过程比较简单，不细说了，

---注意这里用的是新版api

---, 是org.apache.hadoop.mapreduce下的用法，而不是org.apache.hadoop.mapred中的用法，

---两者略有不同

---。

---private

---static

---class

---SQLMapper

---extends

---Mapper

---<

---LongWritable

---,

---MysqlRecord

---,

---Text

---,

---IntWritable

---> {

---@Override

---protected

---void

---map(LongWritable key, MysqlRecord value, Context context)
            throws IOException, InterruptedException {
        Date d = value.time ;

---int

---output = value.output_speed ;
        context.write(

---new

---Text(d.toString()),

---new

---IntWritable(output));
    }
}

---private

---static

---class

---SQLReducer

---extends

---Reducer

---<

---Text

---,

---IntWritable

---,

---MySQLStatistic

---,

---NullWritable

---> {

---@Override

---protected

---void

---reduce(Text key, Iterable<IntWritable> values, Context context)
            throws IOException, InterruptedException {

---int

---sum

---=

---0

---;

---for

---(IntWritable v:values){

---sum

---+= v.get() ;
        }
        MySQLStatistic res =

---new

---MySQLStatistic(key.toString(),

---sum

---) ;
        context.write(res, NullWritable.get());
    }
}

---其中MySQLStatistic也是实现Writable/DBWritable的类，只有两个字段，负责输出到MySQL。

---此外，

---如果是要输出数据到MySQL，Reducer类的KEYOUT必须是实现Writalbe和DBWritable的类，而VALUEOUT是NullWritable类。

---设置Job参数

---这部分是main函数中的一些参数设置。

---首先要用

---DBConfiguration.configureDB

---来设定连接MySQL所需要的一些认证信息。其函数的定义如下

---public

---static

---void configureDB(Configuration conf,

---String

---driverClass,

---String

---dbUrl,

---String

---userName,

---String

---passwd)

---这个没什么好说的，照着格式填就好了

---DBConfiguration

---.configureDB

---(
    conf,

---"com.mysql.jdbc.Driver"

---,

---"jdbc:mysql://localhost:3306/microblog_spider"

---,

---"root"

---,

---"admin"

---)

---;

---上面值得注意的是

---dbUrl部分，这个地址的格式一般为

---jdbc:mysql//hostname[

---:port

---]/dbname[

---?param1=value1

---][

---&param2=value2

---]

---使用[ ]括起来的部分是可选项。还有一点要注意的是，在数据中有中文存在的情况下，可能还需要指定连接的编码。此时就应该在参数中加上useUnicode和characterEncoding这两个选项。例如指定使用utf8字符集。

---jdbc:

---mysql:

---/

---/localhost:3306/microblog

---_spider?useUnicode=

---true

---&characterEncoding=utf8

---在写完MySQL的认证信息以后，再设置输出输出类

---job

---.setInputFormatClass

---(DBInputFormat

---.class

---)

---;

---job

---.setOutputFormatClass

---(DBOutputFormat

---.class

---)

---;

---然后，再设置表名，字段名等信息。

---String

---[] fields = {

---"size"

---,

---"time"

---,

---"input_speed"

---,

---"output_speed"

---} ; 
DBInputFormat.setInput(
        job,

---// job

---MysqlRecord.class,

---// input class

---"proxy_table"

---,

---// table name

---null

---,

---// condition

---"time"

---,

---// order by

---fields);

---// fields

---DBOutputFormat.setOutput(
        job,

---// job

---"hadoop_out"

---,

---// output table name

---"date"

---,

---"nums"

---// fields

---);

---下面是main函数的完整代码

---public

---static

---void

---main(String[] args) throws Exception{
    Configuration conf =

---new

---Configuration() ;
    DBConfiguration.configureDB(
            conf,

---"com.mysql.jdbc.Driver"

---,

---"jdbc:mysql://localhost:3306/microblog_spider"

---,

---"root"

---,

---"admin"

---);
    Job job = Job.getInstance(conf,

---"mysql test"

---) ;
    job.setJarByClass(mysql_test.

---class

---);
    job.setMapperClass(SQLMapper.

---class

---);
    job.setReducerClass(SQLReducer.

---class

---);
    job.setMapOutputKeyClass(Text.

---class

---);
    job.setMapOutputValueClass(IntWritable.

---class

---);
    job.setOutputKeyClass(MySQLStatistic.

---class

---);
    job.setOutputValueClass(NullWritable.

---class

---);
    job.setInputFormatClass(DBInputFormat.

---class

---);
    job.setOutputFormatClass(DBOutputFormat.

---class

---);
    String[] fields = {

---"size"

---,

---"time"

---,

---"input_speed"

---,

---"output_speed"

---} ; 
    DBInputFormat.setInput(
            job,

---// job

---MysqlRecord.

---class

---,

---// input class

---"proxy_table"

---,

---// table name

---null

---,

---// condition

---"time"

---,

---// order by

---fields);

---// fields

---DBOutputFormat.setOutput(
            job,

---// job

---"hadoop_out"

---,

---// output table name

---"date"

---,

---"nums"

---// fields

---);
    job.waitForCompletion(

---true

---) ;
}

---所有代码

---import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.db.DBConfiguration;
import org.apache.hadoop.mapreduce.lib.db.DBInputFormat;
import org.apache.hadoop.mapreduce.lib.db.DBOutputFormat;
import org.apache.hadoop.mapreduce.lib.db.DBWritable;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

---/**
 * Created by multiangle on 16-11-12.
 */

---public

---class

---mysql_test_demo

---{

---private

---static

---class

---MysqlRecord

---implements

---DBWritable

---,

---Writable

---{

---protected

---int

---size ;

---protected

---Date time ;

---protected

---int

---input_speed ;

---protected

---int

---output_speed ;
        @Override

---public

---void

---write(PreparedStatement preparedStatement) throws SQLException {
            preparedStatement.setInt(

---1

---,

---this

---.size);
            preparedStatement.setDate(

---2

---,

---this

---.time);
            preparedStatement.setInt(

---3

---,

---this

---.input_speed);
            preparedStatement.setInt(

---4

---,

---this

---.output_speed);
        }
        @Override

---public

---void

---readFields(ResultSet resultSet) throws SQLException {

---this

---.size = resultSet.getInt(

---1

---) ;

---this

---.time = resultSet.getDate(

---2

---) ;

---this

---.input_speed = resultSet.getInt(

---3

---) ;

---this

---.output_speed = resultSet.getInt(

---4

---) ;
        }
        @Override

---public

---void

---write(DataOutput dataOutput) throws IOException {
            dataOutput.writeInt(

---this

---.size);
            Text.writeString(dataOutput,

---this

---.time.toString()) ;
            dataOutput.writeInt(

---this

---.input_speed);
            dataOutput.writeInt(

---this

---.output_speed);
        }
        @Override

---public

---void

---readFields(DataInput dataInput) throws IOException {

---this

---.size = dataInput.readInt() ;

---this

---.time = Date.valueOf(Text.readString(dataInput)) ;

---this

---.input_speed = dataInput.readInt() ;

---this

---.output_speed = dataInput.readInt() ;
        }
        @Override

---public

---String toString() {

---return

---String.format(

---"%d\t%s\t%d\t%d"

---,size,time.toString(),input_speed,output_speed);
        }
    }

---private

---static

---class

---MySQLStatistic

---implements

---DBWritable

---,

---Writable

---{

---private

---Date

---date

---;

---private

---int

---nums ;
        MySQLStatistic(Date

---date

---,

---int

---nums){

---this

---.

---date

---=

---date

---;

---this

---.nums = nums ;
        }
        MySQLStatistic(String

---date

---,

---int

---nums){

---this

---.

---date

---= Date.valueOf(

---date

---) ;

---this

---.nums = nums ;
        }
        @Override

---public

---void

---write(DataOutput dataOutput) throws IOException {
            Text.writeString(dataOutput,

---date

---.toString()) ;
            dataOutput.writeInt(nums);
        }
        @Override

---public

---void

---readFields(DataInput dataInput) throws IOException {

---date

---= Date.valueOf(Text.readString(dataInput)) ;
            nums = dataInput.readInt() ;
        }
        @Override

---public

---void

---write(PreparedStatement preparedStatement) throws SQLException {
            preparedStatement.setDate(

---1

---,

---date

---);
            preparedStatement.setInt(

---2

---, nums);
        }
        @Override

---public

---void

---readFields(ResultSet resultSet) throws SQLException {

---date

---= resultSet.getDate(

---1

---) ;
            nums = resultSet.getInt(

---2

---) ;
        }
    }

---private

---static

---class

---SQLMapper

---extends

---Mapper

---<

---LongWritable

---,

---MysqlRecord

---,

---Text

---,

---IntWritable

---> {

---@Override

---protected

---void

---map(LongWritable key, MysqlRecord value, Context context)
                throws IOException, InterruptedException {
            Date d = value.time ;

---int

---output = value.output_speed ;
            context.write(

---new

---Text(d.toString()),

---new

---IntWritable(output));
        }
    }

---private

---static

---class

---SQLReducer

---extends

---Reducer

---<

---Text

---,

---IntWritable

---,

---MySQLStatistic

---,

---NullWritable

---> {

---@Override

---protected

---void

---reduce(Text key, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {

---int

---sum

---=

---0

---;

---for

---(IntWritable v:values){

---sum

---+= v.get() ;
            }
            MySQLStatistic res =

---new

---MySQLStatistic(key.toString(),

---sum

---) ;
            context.write(res, NullWritable.get());
        }
    }

---public

---static

---void

---main(String[] args) throws Exception{
        Configuration conf =

---new

---Configuration() ;
        DBConfiguration.configureDB(
                conf,

---"com.mysql.jdbc.Driver"

---,

---"jdbc:mysql://localhost:3306/microblog_spider"

---,

---"root"

---,

---"admin"

---);

        Job job = Job.getInstance(conf,

---"mysql test"

---) ;
        job.setJarByClass(mysql_test.class);
        job.setMapperClass(SQLMapper.class);
        job.setReducerClass(SQLReducer.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);
        job.setOutputKeyClass(MySQLStatistic.class);
        job.setOutputValueClass(NullWritable.class);
        job.setInputFormatClass(DBInputFormat.class);
        job.setOutputFormatClass(DBOutputFormat.class);
        String[] fields = {

---"size"

---,

---"time"

---,

---"input_speed"

---,

---"output_speed"

---} ; 
        DBInputFormat.setInput(
                job,

---// job

---MysqlRecord.class,

---// input class

---"proxy_table"

---,

---// table name

---null

---,

---// condition

---"time"

---,

---// order by

---fields);

---// fields

---DBOutputFormat.setOutput(
                job,

---// job

---"hadoop_out"

---,

---// output table name

---"date"

---,

---"nums"

---// fields

---);
        job.waitForCompletion(

---true

---) ;
    }
}


