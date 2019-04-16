# Hbase实战二：编程实战 - youbingchen的博客 - CSDN博客





2016年06月22日 21:45:51[youbingchen](https://me.csdn.net/youbingchen)阅读数：395








在安装后Hbase[Hbase实战一：安装配置](http://blog.csdn.net/youbingchen/article/details/51728486)，大家快乐的编程实践把

# 1.利用Shell编程

### 在HBase用create命令创建表
` create 'student','Sname','Ssex','Sage','Sdept','course'`
> 
此时，即创建了一个“student”表，属性有：Sname,Ssex,Sage,Sdept,course。因为HBase的表中会有一个系统默认的属性作为主键，主键无需自行创建，默认为put命令操作中表名后第一个数据。创建完“student”表后，可通过describe命令查看“student”表的基本信息。


```bash
describe 'student '
```

> 
删除表


```
disable 'student'
drop 'student'
```

### HBase数据库基本操作

#### 插入数据

Hbase用put命令添加数据，一次只能为一个表的一行数据的一个列添加一个数据。当运行命令：put ‘student’,’95001’,’Sname’,’LiYing’时，即为student表添加了学号为95001，名字为LiYing的一行数据，其主键为95001（默认为第一列）。

```
put 'student','95001','Sname','LiYing'
put 'student','95001','Sname:firstname','LiYing'  # Sname表示列族，可以不用
```

#### 删除数据

> 
在HBase中用delete以及deleteall命令进行删除数据操作，它们的区别是：1. delete用于删除一个数据，是put的反向操作；2. deleteall操作用于删除一行数据。


```java
delete 'student','95001','Ssex'
```

> 
即删除了student表中95001行下的Ssex列的所有数据。 

  deleteall命令


```bash
deleteall 'student','95001'
```

> 
删除了student表中的95001行的全部数据


#### 查询命令

##### get 命令
`get 'student','95001'`
> 
返回的是‘student’表‘95001’行的数据


##### scan命令

```bash
scan 'student'
```

> 
返回的是‘student’表的全部数据


#### 退出

```bash
exit
```

# 2.Java编程

```java
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.*;
import org.apache.hadoop.hbase.client.*;
import java.io.IOException;

public class ExampleForHbase{
    public static Configuration configuration;
    public static Connection connection;
    public static Admin admin;

    public static void main(String[] args)throws IOException{
        createTable("t2",new String[]{"cf1","cf2"});
        insterRow("t2", "rw1", "cf1", "q1", "val1");
        getData("t2", "rw1", "cf1", "q1");
        deleteTable("t2");
    }


    //建立连接
    public static void init(){
        configuration  = HBaseConfiguration.create();
        configuration.set("hbase.rootdir","hdfs://localhost:9000/hbase");
        try{
            connection = ConnectionFactory.createConnection(configuration);
            admin = connection.getAdmin();
        }catch (IOException e){
            e.printStackTrace();
        }
    }
    //关闭连接
    public static void close(){
        try{
            if(admin != null){
                admin.close();
            }
            if(null != connection){
                connection.close();
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }
    //建表
    public static void createTable(String myTableName,String[] colFamily) throws IOException {

        init();
        TableName tableName = TableName.valueOf(myTableName);

        if(admin.tableExists(tableName)){
            System.out.println("talbe is exists!");
        }else {
            HTableDescriptor hTableDescriptor = new HTableDescriptor(tableName);
            for(String str:colFamily){
                HColumnDescriptor hColumnDescriptor = new HColumnDescriptor(str);
                hTableDescriptor.addFamily(hColumnDescriptor);
            }
            admin.createTable(hTableDescriptor);
        }
        close();
    }
    //删表
    public static void deleteTable(String tableName) throws IOException {
        init();
        TableName tn = TableName.valueOf(tableName);
        if (admin.tableExists(tn)) {
            admin.disableTable(tn);
            admin.deleteTable(tn);
        }
        close();
    }

    //查看已有表
    public static void listTables() throws IOException {
        init();
        HTableDescriptor hTableDescriptors[] = admin.listTables();
        for(HTableDescriptor hTableDescriptor :hTableDescriptors){
            System.out.println(hTableDescriptor.getNameAsString());
        }
        close();
    }
    public static void insterRow(String tableName,String rowKey,String colFamily,String col,String val) throws IOException {
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Put put = new Put(rowKey.getBytes());
        put.addColumn(colFamily.getBytes(), col.getBytes(), val.getBytes());
        table.put(put);
        table.close();
        close();
    }
    //删除数据
    public static void deleRow(String tableName,String rowKey,String colFamily,String col) throws IOException {
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Delete delete = new Delete(rowKey.getBytes());
        //删除指定列族
        //delete.addFamily(Bytes.toBytes(colFamily));
        //删除指定列
        //delete.addColumn(Bytes.toBytes(colFamily),Bytes.toBytes(col));
        table.delete(delete);
        table.close();
        close();
    }
    //根据rowkey查找数据
    public static void getData(String tableName,String rowKey,String colFamily,String col)throws  IOException{
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Get get = new Get(rowKey.getBytes());
        get.addColumn(colFamily.getBytes(),col.getBytes());
        Result result = table.get(get);
        showCell(result);
        table.close();
        close();
    }
    //格式化输出
    public static void showCell(Result result){
        Cell[] cells = result.rawCells();
        for(Cell cell:cells){
            System.out.println("RowName:"+new String(CellUtil.cloneRow(cell))+" ");
            System.out.println("Timetamp:"+cell.getTimestamp()+" ");
            System.out.println("column Family:"+new String(CellUtil.cloneFamily(cell))+" ");
            System.out.println("row Name:"+new String(CellUtil.cloneQualifier(cell))+" ");
            System.out.println("value:"+new String(CellUtil.cloneValue(cell))+" ");
        }
    }
}
```



