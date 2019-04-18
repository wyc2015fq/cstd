# 开发MapReduce程序 - Orisun - 博客园







# [开发MapReduce程序](https://www.cnblogs.com/zhangchaoyang/articles/2648748.html)





### 配置Configuration

一条configuration的名称可以是任意字符串，值可以是任意数据类型。

```
conf.set("name", "orisun");
conf.setInt("age",24);
```

在代码中设置只对本次代码运行有效，而在配置文件中设置则长久有效。

configuration-1.xml

```
<?xml version="1.0"?>
<configuration>
<property>
<name>color</name>
<value>yellow</value>
<description>Color</description>
</property>

<property>
<name>size</name>
<value>10</value>
<description>Size</description>
</property>

<property>
<name>weight</name>
<value>heavy</value>
<final>true</final>
<description>Weight</description>
</property>

<property>
<name>size-weight</name>
<value>${size},${weight}</value>
<description>Size and weight</description>
</property>
</configuration>
```

注意上面的weight属性被标记为<final>true</final>，表示只读，不可被更改或覆盖。属性中可以引用其他属性，比如size-weight的属性值就是${size},${weight}

```
assertThat(conf.get("size-weight"), is("14,heavy"));
```

属性还可以以这种代码形式进行设置：

```
System.setProperty("length", "2");
```

也可以通过指定JVM参数的形式来设定：-D property=value 

在代码中添加配置文件后就可以获取这些属性值了。hadoop使用的默认配置文件是core-site.xml。

```
Configuration conf = new Configuration();
conf.addResource("configuration-1.xml");
String color=conf.get("color");
int size=conf.getInt("size", 0);
```

当然你可不断调用conf.addResource()添加任意多个配置文件，如果出现属性名称相同，后来的会覆盖先前的（只读属性除外）。

也可以通过hadoop命令选项指定configuration文件。

% hadoop fs -conf conf/hadoop-localhost.xml -ls .

下面的ConfigurationPrinter类负责打印配置文件中的每一条属性的健值对。

```
1 package basic;
 2 
 3 import java.util.Map.Entry;
 4 
 5 import org.apache.hadoop.conf.Configuration;
 6 import org.apache.hadoop.conf.Configured;
 7 import org.apache.hadoop.util.Tool;
 8 import org.apache.hadoop.util.ToolRunner;
 9 
10 public class ConfigurationPrinter extends Configured implements Tool {
11     static {
12         Configuration.addDefaultResource("hdfs-default.xml");
13         Configuration.addDefaultResource("hdfs-site.xml");
14         Configuration.addDefaultResource("mapred-default.xml");
15         Configuration.addDefaultResource("mapred-site.xml");
16     }
17 
18     @Override
19     public int run(String[] args) throws Exception {
20         Configuration conf = getConf();
21         for (Entry<String, String> entry : conf) {
22             System.out.printf("%s=%s\n", entry.getKey(), entry.getValue());
23         }
24         return 0;
25     }
26 
27     public static void main(String[] args) throws Exception {
28         int exitCode = ToolRunner.run(new ConfigurationPrinter(), args);
29         System.exit(exitCode);
30     }
31 
32 }
```

有几个跟Configuration相关的类需要解释一下。

```
public interface Configurable{
        Configuration getConf();
        void setConf(Configuration conf);
}
```

```
public class Configured implements Configurable{
        public Configured(Configuration conf);
}
```

Tool接口负责处理通用命令行选项，这里的选项args是由ToolRuuner.run(Tool,String[])带进来的。

```
public interface Tool extends Configurable{
        int run(String[] args);
}
```

ToolRunner用来运行实现了Tool接口的类。它和GenericOptionParser一起工作来解释命令行选项并修改Tool的Configuration。

```
public class ToolRunner{
    static void  printGenericCommandUsage(PrintStream out) 
    static int    run(Configuration conf, Tool tool, String[] args) 
    static int    run(Tool tool, String[] args) 
}
```

事实上负责解释hadoop命令行选项的类正是GenericOptionParser。

GenericOptionParser和ToolRunner的一些选项：

-D property=value

-conf filename

-fs uri　　　　等价于 -D fs.default.name=uri

-jt host:port　　　　指定jobtracker工作的主机及端口，等价于-D mapred.job.tracker=host:port

-files file1,file2,……　　　　把文件得到到所的任务节点上去，使之可以从本地获取

-archives archive1,archive2,……　　把archive文件得到到所的任务节点上去，使之可以像本地文件一样unarchive和读取

-libjars jar1,jar2,……　　　　放到所有任务节点的classpath中，使MapReduce程序运行时可以获得到它们

###  远程调试

首先把属性keep.failed.task.files设为true，这样tasktracker就会保留足够的信息以便可以在同一个inputsplit上再次运行。然后你需要运行一个IsolationRunner。

当用hadoop解决复杂的问题时，我们更倾向于设计更多的map-reduce而不是把单个map或reduce设计得更加复杂。事实上对于复杂的问题，我们可以使用比MapReduce更高级的语言来解决：Pig,Hive,Cascading。












