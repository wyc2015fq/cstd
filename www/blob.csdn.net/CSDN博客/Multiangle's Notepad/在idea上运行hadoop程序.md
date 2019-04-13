
# 在idea上运行hadoop程序 - Multiangle's Notepad - CSDN博客


2016年11月03日 15:17:42[multiangle](https://me.csdn.net/u014595019)阅读数：2819



---本文的内容是如何让idea读取hdfs中的文件。

---在开始前，我默认各位看官已经装好了hadoop。如果还没有安装hadoop，可以看这篇文章：

---[linux下命令行安装hadoop2.7.2过程](http://blog.csdn.net/u014595019/article/details/50804625)

---首先要将文本文件从本地移到hdfs中

---$hadoop

---fs

----mkdir

---/tmp

---$hadoop

---fs

----copyFromLocal

---/home/multiangle/download/

---280.

---txt /tmp/

---280.

---txt

---其中

---/home/multiangle/download/280.txt

---为本地文件地址，内容是《斗破苍穹》(笑)。

---/tmp/280.txt

---是hdfs中的地址。指令执行结果如下

![选区_037.png-21.1kB](http://static.zybuluo.com/multiangle/gf1kkvf1fgxnm046151lisz7/%E9%80%89%E5%8C%BA_037.png)
---我这边因为之前已经拷过一次，所以显示文件已经存在了。

---建立工程，写代码

---建立一个hadoop_test工程，新建一个file_sys_test文件，往其中拷入

---import

---org.apache.hadoop.conf.Configuration;

---import

---org.apache.hadoop.fs.FileSystem;

---import

---org.apache.hadoop.fs.Path;

---import

---org.apache.hadoop.io.IOUtils;

---import

---java.io.BufferedReader;

---import

---java.io.IOException;

---import

---java.io.InputStream;

---import

---java.io.InputStreamReader;

---import

---java.net.URI;

---/**
 * Created by multiangle on 16-10-31.
 */

---public

---class

---file_sys_test

---{

---public

---static

---void

---main(String[] args)throws IOException{
        InputStream

---is

---=

---null

---;
        String addr =

---"hdfs://localhost:9000/tmp/280.txt"

---;
        Configuration conf =

---new

---Configuration() ;
        FileSystem fs = FileSystem.

---get

---(URI.create(addr),conf) ;

---try

---{

---is

---= fs.open(

---new

---Path(addr)) ;
            InputStreamReader isr =

---new

---InputStreamReader(

---is

---,

---"gbk"

---) ;
            BufferedReader br =

---new

---BufferedReader(isr) ;
            String line = br.readLine() ;

---while

---(line!=

---null

---){
                System.out.println(line);
                line = br.readLine() ;
            }
        }

---finally

---{
            IOUtils.closeStream(

---is

---) ;
        }
    }
}

---设置各项参数

---首先点击File->Project Structure, 在左侧找到modules,点击右边的绿色小十字->Jar or directories，

---将$HADOOP_INSTALL/share/hadoop下的若干目录添加进来，如下图所示

![选区_031.png-68.5kB](http://static.zybuluo.com/multiangle/muht1smgdsvtxfpkwr1dwji5/%E9%80%89%E5%8C%BA_031.png)
---再在Project Setting下找到 Artifacts选项，将Build on make选项勾上，再点中间的绿色小十字->module output

![选区_035.png-71.6kB](http://static.zybuluo.com/multiangle/9l64svecj7dwk8q6go4r6jt7/%E9%80%89%E5%8C%BA_035.png)
---然后再菜单栏中找到Run->Edit Configuration, 再左边找到绿色小十字->Application, 在Main class中输入org.apache.hadoop.util.RunJar, 在program arguments中填入之前module output的jar包地址，还有包名.类名。如果没有包，则直接填类名。如下图所示

![选区_036.png-74.4kB](http://static.zybuluo.com/multiangle/wq1fkej9gtlctk78mdu5kct1/%E9%80%89%E5%8C%BA_036.png)
---现在就能运行起来了

![选区_038.png-181.7kB](http://static.zybuluo.com/multiangle/w542qq6a3ckw8lco0tzb5jdq/%E9%80%89%E5%8C%BA_038.png)

