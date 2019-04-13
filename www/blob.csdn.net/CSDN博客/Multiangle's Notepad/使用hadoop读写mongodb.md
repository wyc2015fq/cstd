
# 使用hadoop读写mongodb - Multiangle's Notepad - CSDN博客


2016年11月07日 11:58:49[multiangle](https://me.csdn.net/u014595019)阅读数：3644



---由于我之前爬取的微博数据都放在mongodb内，所以使用hadoop来处理mongodb内的数据是很自然的一种选择。

---想要用hadoop读写mongodb内的数据，首先需要mongo-hadoop包。我是使用maven自动下载的，包名:

---org.mongodb.mongo-hadoop:mongo-hadoop-core

---或者你也可以去

---[http://search.maven.org/](http://search.maven.org/)

---或者其他网站自行下载。

---接下来，要使用MongoConfigUtil来设定一些参数，包括输入的数据库，输出的数据库，还可以指定输入哪些字段。例如

---Configuration conf =

---new

---Configuration() ;

---// 指定输入地址

---MongoConfigUtil.setInputURI(conf,

---"mongodb://username:password@ip:port/dbName.tableName"

---); 
BasicDBObject query =

---new

---BasicDBObject();
query.

---get

---(

---"source"

---) ;

---// 指定输入字段“source”

---MongoConfigUtil.setQuery(conf,query);

---// 指定输出地址

---MongoConfigUtil.setOutputURI(conf,

---"mongodb://username:password@ip:port/dbName.tableName"

---);

---而在写Mapper和Reducer类的时候，也要注意输入输出的格式。

---如果是Mapper的数据从Mongodb输入，则KEYIN=Object, VALUEIN=BSONObject ; 如果是Reducer的数据输出到Mongodb，则VALUEOUT=BSONWritable

---还有一点要注意，即

---job.setOutputKeyClass和job.setOutputValueClas在默认情况下是同时设置map阶段和reduce阶段的输出，也就是说只有map和reduce输出是一样的时候才不会出问题

---。当map和reduce输出是不一样的时候就需要通过job.setMapOutputKeyClass和job.setMapOutputValueClas来设置map阶段的输出。

---实际操作

---，我选用了之前爬取的微博数据，目的是统计发微博的各客户端数目。数据库中的单条数据如下所示：


![选区_040.png-16.6kB](http://static.zybuluo.com/multiangle/z377szo5ymn2enws2bscbb0u/%E9%80%89%E5%8C%BA_040.png)
---其中的source字段即为客户端的名字。那么要统计出各客户端的使用数目并输出到mongodb，其程序应该写成

---import

---com

---.mongodb

---.BasicDBObject

---;

---import

---com

---.mongodb

---.hadoop

---.MongoInputFormat

---;

---import

---com

---.mongodb

---.hadoop

---.MongoOutputFormat

---;

---import

---com

---.mongodb

---.hadoop

---.io

---.BSONWritable

---;

---import

---com

---.mongodb

---.hadoop

---.util

---.MongoConfigUtil

---;

---import org

---.apache

---.hadoop

---.conf

---.Configuration

---;

---import org

---.apache

---.hadoop

---.io

---.IntWritable

---;

---import org

---.apache

---.hadoop

---.io

---.Text

---;

---import org

---.apache

---.hadoop

---.mapreduce

---.Job

---;

---import org

---.apache

---.hadoop

---.mapreduce

---.Mapper

---;

---import org

---.apache

---.hadoop

---.mapreduce

---.Reducer

---;

---import org

---.bson

---.BSONObject

---;

---import org

---.bson

---.BasicBSONObject

---;

---import java

---.io

---.IOException

---;

---/**
 * Created by multiangle on 16-11-4.
 */

---public class mongo_connect_test {
    private static class MongoMapper extends Mapper<Object,BSONObject,Text,IntWritable>{
        @Override
        protected void map(Object key, BSONObject value, Context context)
                throws IOException, InterruptedException {
            Text source = new Text(value

---.get

---(

---"source"

---)

---.toString

---())

---;

---context

---.write

---(source,new IntWritable(

---1

---))

---;

---}
    }
    private static class MongoReducer extends Reducer<Text,IntWritable,Text,BSONWritable>{
        @Override
        protected void reduce(Text key, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {
            int sum =

---0

---;

---for (IntWritable value:values){
                sum += value

---.get

---()

---;

---}
            BasicBSONObject res = new BasicBSONObject()

---;

---res

---.put

---(

---"name"

---,key

---.toString

---())

---;

---res

---.put

---(

---"num"

---,sum)

---;

---context

---.write

---(key, new BSONWritable(res))

---;

---}
    }
    public static void main(String[] args) throws Exception{
        Configuration conf = new Configuration()

---;

---MongoConfigUtil

---.setInputURI

---(conf,

---"mongodb://localhost:27017/microblog_part.latest_history"

---)

---;

---BasicDBObject query = new BasicDBObject()

---;

---query

---.get

---(

---"source"

---)

---;

---MongoConfigUtil

---.setQuery

---(conf,query)

---;

---MongoConfigUtil

---.setOutputURI

---(conf,

---"mongodb://localhost:27017/microblog_part.hadoop_out"

---)

---;

---Job job = Job

---.getInstance

---(conf,

---"Mongo Connection"

---)

---;

---job

---.setJarByClass

---(mongo_connect_test

---.class

---)

---;

---job

---.setMapperClass

---(MongoMapper

---.class

---)

---;

---job

---.setReducerClass

---(MongoReducer

---.class

---)

---;

---job

---.setInputFormatClass

---(MongoInputFormat

---.class

---)

---;

---job

---.setMapOutputKeyClass

---(Text

---.class

---)

---;

---job

---.setMapOutputValueClass

---(IntWritable

---.class

---)

---;

---job

---.setOutputKeyClass

---(Text

---.class

---)

---;

---job

---.setOutputValueClass

---(BSONWritable

---.class

---)

---;

---job

---.setOutputFormatClass

---(MongoOutputFormat

---.class

---)

---;

---System

---.exit

---(job

---.waitForCompletion

---(true) ?

---0

---:

---1

---)

---;

---}
}


