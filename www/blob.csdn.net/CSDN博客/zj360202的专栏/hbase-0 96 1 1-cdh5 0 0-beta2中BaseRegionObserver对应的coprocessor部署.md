# hbase-0.96.1.1-cdh5.0.0-beta2中BaseRegionObserver对应的coprocessor部署 - zj360202的专栏 - CSDN博客





2014年03月14日 14:21:34[zj360202](https://me.csdn.net/zj360202)阅读数：1786








1.将开发好的jar包，放到hdfs路径下面hdfs://nameservice1/user/coprocessor/xg/
2.在hbase-site.xml中配置对应的类



```
<property>
    <name>hbase.coprocessor.region.classes</name>
    <value>org.cloud.xigua.coprocessor.Product,org.cloud.xigua.coprocessor.User,org.cloud.xigua.coprocessor.Subject</value>
  </property>
```



这些会在添加coprocessor的时候，通知Configuration读取开始部署

3.部署有两种方法：

     3.1常规部署:执行hbase shell脚本



```java
disable 'xg_product'
     alter 'xg_product', METHOD => 'table_att_unset', NAME => 'coprocessor$1'  #如果本来没有就不用这个，这个是用来删除已经有的coprocessor$1的
     alter 'xg_product',METHOD => 'table_att','coprocessor'=>'hdfs://nameservice1/user/coprocessor/xg/xg_coprocessor.jar|org.cloud.xigua.coprocessor.Product|1073741823|'
     enable 'xg_product'
```


     3.2热部署(java代码部署)

```java
byte[] HBASE_TABLE_BYTES = "xg_product".getBytes();
          
          try {
               HBaseAdmin admin = new HBaseAdmin(config);
               admin.disableTable(HBASE_TABLE_BYTES);
               
               HTableDescriptor htd = admin.getTableDescriptor(HBASE_TABLE_BYTES);
               htd.removeCoprocessor("org.cloud.xigua.coprocessor.Product");
               htd.addCoprocessor("org.cloud.xigua.coprocessor.Product",new Path("hdfs://nameservice1/user/coprocessor/xg/xg_coprocessor.jar"),Coprocessor.PRIORITY_USER,null);
//               htd.setValue("COPROCESSOR$1", "hdfs://nameservice1/user/coprocessor/xg/xg_coprocessor_0310.jar|org.cloud.xigua.coprocessor.imp.OrderScore|100|");
//               htd.setValue("COPROCESSOR$1", "hdfs://nameservice1/user/coprocessor/xg/xg_coprocessor.jar|org.cloud.xigua.coprocessor.Product|1073741823|");
               
               admin.modifyTable(HBASE_TABLE_BYTES, htd);
               
               admin.enableTable(HBASE_TABLE_BYTES);
//               System.out.println(htd.hasCoprocessor("org.cloud.xigua.coprocessor.Product"));
               
               admin.close();
          } catch (MasterNotRunningException e) {
               e.printStackTrace();
          } catch (ZooKeeperConnectionException e) {
               e.printStackTrace();
          } catch (IOException e) {
               e.printStackTrace();
          }
```







