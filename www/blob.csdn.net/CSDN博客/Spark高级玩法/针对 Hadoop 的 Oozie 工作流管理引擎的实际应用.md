# 针对 Hadoop 的 Oozie 工作流管理引擎的实际应用 - Spark高级玩法 - CSDN博客
2018年10月18日 00:06:06[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：49
本文操作性强，建议先收藏再仔细阅读，文章源码可以公众号后台输入1026获取。
**简介**
Apache Oozie 是用于 Hadoop 平台的一种工作流调度引擎。该框架（如图 1 所示）使用 Oozie协调器促进了相互依赖的重复工作之间的协调，您可以使用预定的时间或数据可用性来触发 Apache Oozie。您可以使用 Oozie bundle系统提交或维护一组协调应用程序。作为本练习的一部分，Oozie 运行了一个 Apache Sqoop 作业，以便在 MySQL数据库中的数据上执行导入操作，并将数据传输到 Hadoop 分布式文件系统 (HDFS) 中。可以利用导入的数据集执行 Sqoop合并操作，从而更新较旧的数据集。通过利用 UNIX shell 操作，可从 MySQL 数据库中提取用来执行 Sqoop 作业的元数据。同理，可执行 Java操作来更新 Sqoop 作业所需的 MySQL 数据库中的元数据。
##### 图 1. Oozie 编排架构
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV5PmE5B4qWuS8ZsxcVGUGGRXvMRBDXMt0IwmSfichjs699be2EuGntWpibHzZXf8F3PCQXgJiaT1Dxg/640)
## **Oozie 工作流**
Oozie 工作流是控制依赖有向非循环图 (DAG) 中安排的 Oozie 操作的集合。控制依赖（Control  dependency）可确保以下操作在前面的操作已成功完成后才会启动。本文首先会简要概述工作流控制节点，然后重点介绍以下工作流操作节点：
- 
Oozie shell 操作
- 
Oozie Java 操作
- 
Oozie Sqoop 操作
- 
Oozie Hive 操作
- 
Oozie 电子邮件操作
## **工作流控制节点**
*启动控制节点*（如清单 1 所示）是工作流作业的入口点。在工作流启动时，它会自动过渡到启动过程中指定的节点。
##### 清单 1. 启动控制节点
```
<workflow-app xmlns="uri:oozie:workflow:0.2" name="ooziedemo-wf">
    <start to="timeCheck"/>
</workflow-app>
```
*末端控制节点*（如清单 2 所示）是结束工作流作业时所用的节点。它表示工作流操作已经成功完成。一个工作流定义必须有一个末端节点。
##### 清单 2. 末端控制节点
```
<workflow-app xmlns="uri:oozie:workflow:0.2" name="ooziedemo-wf">
    <end name="end"/>
</workflow-app>
```
*停止控制节点*（如清单 3 所示）可使工作流作业自行停止。在到达停止节点（kill node）时，如果工作流作业启动的一个或更多操作正在运行，那么当前运行的所有操作都将停止。工作流定义可以包含零个或更多停止节点。
##### 清单 3. 停止控制节点
```
<workflow-app xmlns="uri:oozie:workflow:0.2" name="ooziedemo-wf">
    <kill name="fail">
        <message>Sqoop failed, error message[${wf:errorMessage(wf:lastErrorNode())}]</message>
   </kill>
</workflow-app>
```
*决策控制节点*（如清单 4 所示）可使工作流确定要采用的执行路径。决策节点的工作原理类似于拥有一组谓词转换对（predicates-transition  pair）和一个默认转换的 switch-case 块。谓词是按顺序进行评估的，直至其中一个评估为 ture 为止，同时还会进行相应的转换。如果没有一个谓词被评估为true，则会采用默认转换。
##### 清单 4. 决策控制节点
```
<workflow-app xmlns="uri:oozie:workflow:0.2" name="ooziedemo-wf">
    <decision name="master-decision">
       <switch>
         <case to="sqoopMerge1">
                 ${wf:actionData('hiveSwitch')['paramNum'] eq 1}
         </case>
         <default to="sqoopMerge2"/>
       </switch>
   </decision>
</workflow-app>
```
*分支节点* 将一个执行路径分为多个并发路径。*联接节点*一直等待，直到前面的分支节点的所有并发执行路径都到达联接节点为止。您必须成对使用分叉节点和联接节点，如清单 5 所示。
##### 清单 5. 分支-联接控制节点
```
<workflow-app xmlns="uri:oozie:workflow:0.2" name="ooziedemo-wf">
    <fork name="forking">
        <path start="sqoopMerge1"/>
        <path start="sqoopMerge2"/>
    </fork>
    <join name="joining" to="hiveSwitch"/>
</workflow-app>
```
**Oozie shell 操作**
您可以将 Oozie shell 操作作为工作流的一部分进行配置，从而运行某个文件中的一组 shell 脚本。您可以利用包含必要参数的`job-tracker`、`name-node` 和 `exec` 元素来配置 Oozie  shell 操作，从而执行相关的任务，如清单 6 所示。您可以将具有配置参数的一个配置 shell 操作来创建或删除 HDFS 上的文件和目录，然后启动 shell  作业。您可以使用与配置元素内联的 `job-xml` 元素，利用配置参数将一个 XML 文件传递给 shell 作业。您可以配置其他文件或归档文件，让它们可用于 shell 作业。在 shell 作业结束后，您可以让 shell 作业的输出可供`workflow` 作业使用，但它需要满足以下条件：
- 
输出的格式必须是一个有效的 Java 属性文件。
- 
输出的大小必须小于 2KB。
##### 清单 6. Shell 脚本
```
host="XXX.XX.XX.XXX"
port="3306"
username="root"
password=""
database="zzz"
tableName="$1"
####################################
echo "Host: $host"
echo "Database: $database"
echo "Table: $tableName"
####################################
sqoopLstUpd=`mysql --host=$host --port=$port --user=$username --password=$password 
-N -e 'SELECT PARM_DATE_VAL from T_CONTROL_PARM where PARM_NM="SQOOP_INCR_LST_UPD"
 and PARM_GROUP_NM="'$tableName'"' $database`
echo "sqoopLstUpd=$sqoopLstUpd"
echo "tableName=$tableName"
```
清单 7. Oozie shell 操作清单 7 展示了 workflow.xml 文件中的 shell 操作配置。
```
<action name="timeCheck">
    <shell xmlns="uri:oozie:shell-action:0.1">
       <job-tracker>${jobTracker}</job-tracker>
       <name-node>${nameNode}</name-node>
       <configuration>
           <property>
               <name>mapred.job.queue.name</name>
               <value>${queueName}</value>
           </property>
       </configuration>
       <exec>${sqoopUpdTrack}</exec>
       <argument>${tableName}</argument>
       <file>${sqoopUpdTrackPath}#${sqoopUpdTrack}</file>
       <capture-output/>
    </shell>
    <ok to="sqoopIncrImport"/>
    <error to="fail"/>
</action>
```
要想访问 shell 输出，可以使用清单 8 中所示的 Sqoop 增量作业。
##### 清单 8. 用来实现增量导入的 Oozie Sqoop 操作
```
<action name="sqoopIncrImport">
        <sqoop xmlns="uri:oozie:sqoop-action:0.2">
           <job-tracker>${jobTracker}</job-tracker>
           <name-node>${nameNode}</name-node>
           <prepare>
               <delete path="${s3BucketLoc}/${tableName}/incr"/>
               <mkdir path="${s3BucketLoc}/${tableName}"/>
           </prepare>
           <configuration>
               <property>
                   <name>mapred.job.queue.name</name>
                   <value>${queueName}</value>
               </property>
           </configuration>
           <arg>import</arg>
           <arg>--connect</arg>
           <arg>${dbURL}</arg>
           <arg>--driver</arg>
           <arg>${mySqlDriver}</arg>
           <arg>--username</arg>
           <arg>${user}</arg>
           <arg>--table</arg>
           <arg>${wf:actionData('timeCheck')['tableName']}</arg>
           <arg>--target-dir</arg>
           <arg>${s3BucketLoc}/${tableName}/incr</arg>
           <arg>--check-column</arg>
           <arg>LAST_UPD</arg>
           <arg>--incremental</arg>
           <arg>lastmodified</arg>
           <arg>--last-value</arg>
           <arg>${wf:actionData('timeCheck')['sqoopLstUpd']}</arg>
           <arg>--m</arg>
           <arg>1</arg>
       </sqoop>
       <ok to="sqoopMetaUpdate"/>
       <error to="fail"/>
   </action>
```
**Oozie Java 操作**
Java 操作运行了指定的主要 Java 类的 public static void main (`String [] args`) 方法。Java  应用程序作为具有单个 mapper 任务的 MapReduce 作业运行Hadoop 集群上。工作流作业一直要等到 Java操作结束运行之后才能继续执行下一个操作。Java 操作可使用 `job-tracker`、`name-node`、Java主类、JVM 选项和输入参数进行配置，如清单 9 所示。您可以使用 Expression Language (EL) 表达式将参数分配给内联属性值。您必须以 Java属性文件的格式写入所有输出参数。
您可以配置 Java 操作来清理 HDFS 文件和目录，或者建立 Apache HCatalog 分区，然后再启动 Java 应用程序。这使得 Oozie能够在出现暂时性或非暂时性故障时重试 Java 操作。
##### 清单 9. Oozie Java 操作
```
<action name="sqoopMetaUpdate">
          <java>
               <job-tracker>${jobTracker}</job-tracker>
               <name-node>${nameNode}</name-node>
               <configuration>
                   <property>
                      <name>mapred.job.queue.name</name>
                      <value>${queueName}</value>
                   </property>
               </configuration>
               <main-class>SqoopMetaUtil</main-class>
               <java-opts></java-opts>
               <arg>${tableName}</arg>
               <archive>${mySqlDriverPath}</archive>
          </java>
          <ok to="hiveSwitch"/>
          <error to="fail"/>
</action>
```
您可以通过使用 `capture-output` 来配置 Java 操作，从而将值传递给下一操作。您可以通过使用 Hadoop EL函数来访问这些值。您能够以 Java 属性文件的格式在 Java 类中写入值，如清单 10 所示。
##### 清单 10. 用于实现值传递的 Java 代码片段
```
String OOZIE_ACTION_OUTPUT_PROPERTIES = "oozie.action.output.properties";
String oozieProp = System.getProperty(OOZIE_ACTION_OUTPUT_PROPERTIES);
       OutputStream os = null;
       if(oozieProp != null){
          File propFile = new File(oozieProp);
          Properties p = new Properties();
          p.setProperty("name", "Autodesk");
          p.setProperty("address", "Sun Rafael");
          try {
               os = new FileOutputStream(propFile);
               p.store(os, "");
          } catch (FileNotFoundException e) {
               System.err.println("<<< FileNotFoundException >>>"+e.getMessage());
          } catch (IOException e) {
               System.err.println("<<< IOException >>>"+e.getMessage());
          }
          finally{
               if(os != null)
               try {
                    os.close();
               } catch (IOException e) {
                    System.err.println("<<< IOException >>>"+e.getMessage());
               }
          }
       }
       else{
            throw new RuntimeException(OOZIE_ACTION_OUTPUT_PROPERTIES
                    + " System property not defined");
    }
```
您可以在 workflow.xml 文件中配置操作来访问属性文件中的相应值设置，如清单 11 所示。
##### 清单 11. 用于实现值传递的 Oozie Java 操作
```
<action name="jProperties">
      <java>
           <job-tracker>${jobTracker}</job-tracker>
           <name-node>${nameNode}</name-node>
           <configuration>
               <property>
                  <name>mapred.job.queue.name</name>
                  <value>${queueName}</value>
               </property>
           </configuration>
           <main-class>PropertyExplorer</main-class>
           <java-opts></java-opts>
           <capture-output/>
      </java>
      <ok to="email"/>
      <error to="fail"/>
   </action>
   <action name="email">
         <email xmlns="uri:oozie:email-action:0.1">
            <to>surajit.paul@autodesk.com</to>
            <subject>Oozie workflow finished successfully!</subject>
            <body>${wf:actionData('jProperties')['name']} | 
            ${wf:actionData('jProperties')['address']}</body>
         </email>
         <ok to="end"/>
         <error to="fail"/>
   </action>
```
**Oozie Sqoop 操作**
Oozie 工作流触发了一个 Sqoop 脚本，该脚本在 Hadoop 集群上启动了一个 Sqoop 作业。Sqoop 作业通过在 Hadoop 集群上启动MapReduce 作业来完成任务。Sqoop 脚本启动的 MapReduce 作业会将数据从 RDBMS 传输到 HDFS。您可以配置一个 Sqoop 操作（如清单12 所示）来删除 HDFS 上的文件和目录，然后再启动 Sqoop 作业。与其他 Oozie 操作类似，您可以通过使用 `job-xml`元素，利用其他的属性来配置 Sqoop 操作。*configuration* 元素中指定的属性值将会覆盖 *job-xml*  元素中指定的属性。可以将其他文件和归档文件提供给 Sqoop 作业。
##### 清单 12. 用于合并的 Oozie Sqoop 操作
```
<action name="sqoopMerge1">
         <sqoop xmlns="uri:oozie:sqoop-action:0.2">
             <job-tracker>${jobTracker}</job-tracker>
             <name-node>${nameNode}</name-node>
             <prepare>
                 <delete path="${s3BucketLoc}/${tableName}/master1"/>
                 <mkdir path="${s3BucketLoc}/${tableName}"/>
             </prepare>
             <configuration>
                 <property>
                     <name>mapred.job.queue.name</name>
                     <value>${queueName}</value>
                 </property>
             </configuration>
             <arg>merge</arg>
             <arg>--new-data</arg>
             <arg>${s3incr}</arg>
             <arg>--onto</arg>
             <arg>${s3BucketLoc}/${tableName}/master2</arg>
             <arg>--target-dir</arg>
             <arg>${s3BucketLoc}/${tableName}/master1</arg>
             <arg>--jar-file</arg>
             <arg>${tableJarLoc}/${tableName}.jar</arg>
             <arg>--class-name</arg>
             <arg>${tableName}</arg>
             <arg>--merge-key</arg>
             <arg>ROW_ID</arg>
         </sqoop>
         <ok to="hive-master1"/>
         <error to="fail"/>
   </action>
```
**Oozie Hive 操作**
您可以配置 Hive 操作（如清单 13 所示）来执行 HDFS 上的文件和目录中的任何 Hive 脚本。该操作启动了一个 MapReduce作业来完成这些任务。您需要在 Oozie 上配置 Hive 操作，使用 Hive 配置文件 hive-default.xml 或 hive-site.xml 作为*job-xml* 元素。对于支持 Hive 访问 Oozie 环境而言，这一点是必需的。您可以配置 Hive 操作来创建或删除 HDFS文件和目录，然后再启动 Hive 作业。*configuration* 元素中指定的属性值将会覆盖 *job-xml*文件中指定的值。您可以添加其他的文件和归档文件，让它们可用于 Hive 作业。Oozie 执行了由脚本元素中的路径指定的 Hive 脚本。您可以通过 Oozie 工作流，将参数作为输入参数分配给 Hive 脚本。
##### 清单 13. Oozie Hive 操作
```
<action name="hiveSwitch">
     <shell xmlns="uri:oozie:shell-action:0.1">
         <job-tracker>${jobTracker}</job-tracker>
         <name-node>${nameNode}</name-node>
          <configuration>
          <property>
              <name>mapred.job.queue.name</name>
              <value>${queueName}</value>
          </property>
          </configuration>
          <exec>${hiveSwitchScript}</exec>
          <argument>${tableName}</argument>
          <file>${hiveSwitchScriptPath}#${hiveSwitchScript}</file>
       <capture-output/>
       </shell>
       <ok to="master-decision"/>
       <error to="fail"/>
   </action>
```
**Oozie 电子邮件操作**
Oozie 电子邮件操作（如清单 14 所示）可以从工作流应用程序发送电子邮件。除了主题和消息正文之外，电子邮件操作还必须拥有 to 和cc（可选）地址。您可以使用逗号分割的电子邮件地址向多个收件人发送电子邮件。电子邮件操作可同步运行，而工作流作业一直要等到发送电子邮件之后才会触发下一个操作。您可以使用Hadoop EL 表达式将参数分配给电子邮件操作。
##### 清单 14. Oozie 电子邮件操作
```
<action name="email">
    <email xmlns="uri:oozie:email-action:0.1">
            <to>surajit.paul@autodesk.com</to>
            <subject>Oozie workflow finished successfully!</subject>
            <body>${wf:actionData('jProperties')['name']} | 
            ${wf:actionData('jProperties')['address']}</body>
        </email>
        <ok to="end"/>
        <error to="fail"/>
   </action>
```
**结束语**
当多个相互依赖的作业与数据流捆绑在一起的时候，Oozie 工作流就会变成一个数据管道应用程序。Apache Oozie工作流促进了数据逻辑流程的设计、错误处理、故障转移机制等。您可以配置 Oozie协调器或捆绑的应用程序来有效地管理工作流，不过，关于这些主题的讨论已超出了本文的讨论范围。一些等效的 Hadoop 工作流引擎包括 Amazon DataPipeline、Simple Workflow Engine、Azkaban、Cascading 和 Hamake。虽然 Hamake 和 Oozie 是基于 XML的配置，但 Azkaban 是使用包含键值对的文本文件进行配置的，而 Cascading 是使用 Java API 进行配置的。
原文地址：https://www.ibm.com/developerworks/cn/data/library/bd-hadoopoozie/index.html
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
