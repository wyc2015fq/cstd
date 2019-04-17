# 【Hadoop & Hbase】大数据存储系统编程_读出写入 - CD's Coding - CSDN博客





2017年05月14日 14:48:16[糖果天王](https://me.csdn.net/okcd00)阅读数：1137








### 0x01 编译环境
- jdk 1.7
- hbase 0.98
- hadoop 2.6
- Ubuntu Linux 14.04.2

### 0x02 目标

![Quest](https://img-blog.csdn.net/20170514143133907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中我的任务编号为`5`，即`Sort-based Distinct`

> 
**Sort-based Distinct**

  根据group by key 排序 

  然后同一个group的都会在一起 

  统计输出
任务来源： 陈世敏老师 2017春季课程 《大数据系统与大规模数据分析》 
![Quest](https://img-blog.csdn.net/20170514143322596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 0x03 实现需要的知识

#### hdfs工具

`$ hdfs dfs –help`

打印出所有命令的usage信息

`-copyFromLocal [-f] [-p] [-l] <localsrc> ... <dst> :`

Identical to the -put command.

`-copyToLocal [-p] [-ignoreCrc] [-crc] <src> ... <localdst> :`

Identical to the -get command.

`-cat [-ignoreCrc] <src> ... :`

Fetch all files that match the file pattern  and display 

their content on stdout.
`-ls [-d] [-h] [-R] [<path> ...] :`

list contents

#### 举例：读一个HDFS文件

```
public class HDFSTest {
    public static void main(String[] args) throws IOException, URISyntaxException{
        String file= “hdfs://localhost:9000/文件路径";
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(file), conf);
        Path path = new Path(file);
        FSDataInputStream in_stream = fs.open(path);
        BufferedReader in = new BufferedReader(new InputStreamReader(in_stream));
        String s;
        while ((s=in.readLine())!=null) {
            System.out.println(s);
        }
        in.close();
        fs.close();
    }
}
```

#### HBase数据模型
- Key包括row key与column两个部分
- 所有row key是按顺序存储的
- 其中column又有column family前缀 
- Column family是需要事先声明的，种类有限（例如~10或~100）
- 而column key可以有很多

- 具体存储时，每个column family将分开存储（类似列式数据库）

#### hbase shell

`create 'mytable', 'mycf’`

创建表，column family

```
put 'mytable', 'abc', 'mycf:a', '123'
0 row(s) in 0.0580 seconds
put 'mytable', 'def', 'mycf:b', '456'
0 row(s) in 0.0060 seconds
scan 'mytable'
ROW COLUMN+CELL
abc column=mycf:a, timestamp=1427731972925, value=123
def column=mycf:b, timestamp=1427731990058, value=456
2 row(s) in 0.0300 seconds
```

#### 举例：HBase create table & Put

```
public class HBaseTest {
    public static void main(String[] args) throws MasterNotRunningException,
ZooKeeperConnectionException, IOException {
        // create table descriptor
        String tableName= "mytable";
        HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));
        // create column descriptor
        HColumnDescriptor cf = new HColumnDescriptor("mycf");
        htd.addFamily(cf);
        // configure HBase
        Configuration configuration = HBaseConfiguration.create();
        HBaseAdmin hAdmin = new HBaseAdmin(configuration);
        hAdmin.createTable(htd);
        hAdmin.close();
        // put "mytable","abc","mycf:a","789"
        HTable table = new HTable(configuration,tableName);
        Put put = new Put("abc".getBytes());
        put.add("mycf".getBytes(),"a".getBytes(),"789".getBytes());
        table.put(put);
        table.close();
        System.out.println("put successfully");
    }
}
```

### 0x04 实现源码

```java
/* # coding=utf8
# ========================================================
#   Copyright (C) 2017 All rights reserved.
#   
#   filename : Hw1Grp5.java
#   author   : okcd00@qq.com / Chen Dian
#   date     : 2017-03-26
#   desc     : Homework1 Sort_Based_Distinct
# ======================================================== */

import java.net.URI;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Collections;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.apache.hadoop.conf.Configuration;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;

import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;

public class Hw1Grp5 {
    private static String tableName = "Result";
    private static byte[] colFam = "res".getBytes();

    /**
     * Create Hbase table
     * @param tableName
     * @param colFam
     * @throws IOException 
     * @throws ZooKeeperConnectionException 
     */
    public static void CreateHbaseTable(String tableName, byte[] colFam) throws ZooKeeperConnectionException, IOException{
        // System.out.println("Creating table " + tableName + " ......");
        @SuppressWarnings("resource")
        HBaseAdmin hAdmin = new HBaseAdmin(HBaseConfiguration.create());
        TableName tn = TableName.valueOf(tableName);
        if (hAdmin.tableExists(tn)) {
            hAdmin.disableTable(tn);  
            hAdmin.deleteTable(tn);  
            // System.out.println(tableName + " existed, now deleting....");  
        }  
        HTableDescriptor tableDescriptor = new HTableDescriptor(tn);  
        tableDescriptor.addFamily(new HColumnDescriptor(colFam)); 
        hAdmin.createTable(tableDescriptor);  
        // System.out.println("Table " + tableName + " created."); 
    }

    /**
     * Judge if one satisfied the demand
     * @param filterCon
     * @param colVal
     * @param filterVal
     * @return result
     */
    private static boolean InfoJudge(String filterCon, double colVal, double filterVal)
    {
        boolean result = (
            filterCon.equals("eq") && colVal == filterVal ||
            filterCon.equals("gt") && colVal >  filterVal ||
            filterCon.equals("ge") && colVal >= filterVal ||            
            filterCon.equals("le") && colVal <= filterVal ||
            filterCon.equals("lt") && colVal <  filterVal ||
            filterCon.equals("ne") && colVal != filterVal 
        );
        return result;
    }

    /**
     * Load & Filter data
     * @param filepath
     * @param filter
     * @param columns
     * @return alist
     * @throws IOException
     */
    private static List<String> LoadAndFilter(String filepath, String filter, String columns) throws IOException {
        // Deal with parameters
        String filterItems[] = filter.split(",");
        String fi0 = filterItems[0].substring(1);
        String fi1 = filterItems[1];
        String fi2 = filterItems[2];

        int    filterCol = Integer.valueOf(fi0);
        String filterCon = fi1;
        double filterVal = Double.valueOf(fi2);

        String[] tmpCols = columns.split(",");
        int[] cols = new int[tmpCols.length];
        for(int i=0; i<tmpCols.length; i++)
            cols[i] = Integer.valueOf(tmpCols[i].substring(1));

        // Loading prepare
        FileSystem        fs = FileSystem.get(URI.create(filepath), new Configuration());
        FSDataInputStream is = fs.open(new Path(filepath));
        BufferedReader    br = new BufferedReader(new InputStreamReader(is));

        // Filter
        String line = null;
        List<String> Llist = new LinkedList<String>();
        while( ( line=br.readLine() )!=null ){
            String[] vals = line.split("\\|");
            double colVal = Double.valueOf(vals[filterCol]);
            if( InfoJudge(filterCon, colVal, filterVal) ){
                String key = "";
                for(int i:cols) key += vals[i] + "|";
                Llist.add(key);
            }
        }
        br.close();
        fs.close();
        return Llist;
    }

    /**
     * Insert data into hbase table
     * @param answer
     * @param columns
     * @param tableName
     * @param colFam
     * @throws IOException
     */
    private static void InsertIntoHbaseTable(List<String> answer, String columns, String tableName, byte[] colFam) throws IOException {
        //get parameters
        String[] cols = columns.split(",");

        //insert into hbase table from uniqueList
        HTable table = new HTable(HBaseConfiguration.create(), TableName.valueOf(tableName));
        int key = 0;
        for(String cur : answer){
            Put put = new Put(("" + key++).getBytes());
            String[] tmp = cur.split("\\|");
            for(int j=0; j<cols.length; j++)
                put.add(colFam, cols[j].getBytes(), tmp[j].getBytes());
            table.put(put);
        }
        table.close();
    }

    /**
     * Unique for distinct
     * @param alist
     * @return retlist
     */
    private static ArrayList<String> Unique(List<String> alist) {
        String lastWord = "";
        ArrayList<String> uniqList = new ArrayList<String>(alist);
        Collections.sort(uniqList);
        ArrayList<String> retlist  = new ArrayList<String>();
        for(String cur : uniqList){
            // System.out.println("LOG: "+cur+" & "+lastWord);
            // cur == lastWord NOT WORK! should use `equals()`
            if(!cur.equals(lastWord)) retlist.add(cur);
            lastWord = new String(cur);
        }
        return retlist;
    }

    /**
     * Main interface
     * @param args
     * @throws IOException
     * @throws MasterNotRunningException
     * @throws ZooKeeperConnectionException
     */
    public static void main(String[] args) throws MasterNotRunningException, ZooKeeperConnectionException, IOException {
        if(args.length != 3){
                System.out.println("Invalid Input, Please type in commands like this:");
                System.out.println("java Hw1Grp5 R=/hw1/lineitem.tbl select:R1,gt,5.1 distinct:R2,R3,R5...");
                System.exit(0);
            }
        /*
            $ java Hw1Grp5 R=/hw1/lineitem.tbl select:R1,gt,5.1 distinct:R2,R3,R5...
              means:
                Args1 = "R=/hw1/lineitem.tbl";
                Args2 = "select:R1,gt,5.1";
                Args3 = "distinct:R2,R3,R5";
        */
        String Args1 = args[0], Args2 = args[1], Args3 = args[2];

        /* filepath, filter, columns */
        Args1 = Args1.substring(Args1.indexOf('=')+1).trim();
        Args2 = Args2.substring(Args2.indexOf(':')+1).trim();
        Args3 = Args3.substring(Args3.indexOf(':')+1).trim();

        CreateHbaseTable(tableName, colFam);
        List<String> Llist = LoadAndFilter(Args1, Args2, Args3);
        List<String> answer = Unique(Llist);
        InsertIntoHbaseTable(answer, Args3, tableName, colFam);

    }
}
```







