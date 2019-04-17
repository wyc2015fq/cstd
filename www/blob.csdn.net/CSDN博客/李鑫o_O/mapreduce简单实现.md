# mapreduce简单实现 - 李鑫o_O - CSDN博客





2016年03月10日 20:56:57[hustlx](https://me.csdn.net/HUSTLX)阅读数：859












第一步：登陆服务器：

ssh 2014210***@thumedia.org -p 6349

2014210***@thumedia.org's password:

输入密码之后可见：

Welcome to Ubuntu 12.04.4 LTS (GNU/Linux 3.2.0-61-generic x86_64)

* Documentation:https://help.ubuntu.com/

New release '14.04.1 LTS' available.

Run 'do-release-upgrade' to upgrade to it.

Last login: Sun Dec 14 21:01:46 2014 from cluster0

2014210***@cluster-3-1:~$

第二步，在/home/2014210***文件夹下编写mapper.py
 /reducer.py。将数据集hw2复制到/user/2014210***文件夹下。

其中mapper.py如下：



```python
#!/usr/bin/env python
import sys
import re
for line in sys.stdin:
    line = line.strip()
    words = line.split()
    for word in words:
        mi=re.search('2014(.*)',word)
        if mi:
                print "%s\t%s" % (mi.group(), 1)
```




其中reducer.py如下：



```python
#!/usr/bin/env python
import sys
word2count={}
for line in sys.stdin:
    word,count=line.split('\t',1)
    try:
        word=int(word)
        count=int(count)
    except ValueError:
        continue
 
    try:
        word2count[word] = word2count[word]+count
    except:
        word2count[word]=count
for word in word2count.keys():
    print '%s\t%s'%(word,word2count[word])
```




第三步，测试mapper.py：

2014210***@cluster-3-1:~$ echo "foo 2014210199 foo 2014210*** quux 2014210*** labs foo bar quux" | ./mapper.py

20142101991

2014210***1

2014210***1

2014210***@cluster-3-1:~$

测试reducer.py：

2014210***@cluster-3-1:~$ echo "foo 2014210199 foo 2014210*** quux 2014210*** labs foo bar quux" | ./mapper.py | sort -k1,1 | ./reducer.py

2014210***2

20142101991

2014210***@cluster-3-1:~$

可见map和reduce均正确。

第四步：运行：

2014210***@cluster-3-1:~$ hadoop jar /opt/cloudera/parcels/CDH/jars/hadoop-streaming-2.5.0-mr1-cdh5.2.0.jar -input /user/2014210***/hw2
 -output /user/2014210***/output -mapper ./mapper.py -file ./mapper.py -reducer ./reducer.py -file ./reducer.py

14/12/14 21:17:22 WARN streaming.StreamJob: -file option is deprecated, please use generic option -files instead.

packageJobJar: [./mapper.py, ./reducer.py] [/opt/cloudera/parcels/CDH-5.2.0-1.cdh5.2.0.p0.36/jars/hadoop-streaming-2.5.0-cdh5.2.0.jar]
 /tmp/streamjob6018778792983891193.jar tmpDir=null

14/12/14 21:17:24 INFO client.RMProxy: Connecting to ResourceManager at cluster-4-0/192.168.5.20:8032

14/12/14 21:17:25 INFO client.RMProxy: Connecting to ResourceManager at cluster-4-0/192.168.5.20:8032

14/12/14 21:17:26 INFO mapred.FileInputFormat: Total input paths to process : 19

14/12/14 21:17:26 INFO mapreduce.JobSubmitter: number of splits:19

14/12/14 21:17:26 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1417174627956_0387

14/12/14 21:17:27 INFO impl.YarnClientImpl: Submitted application application_1417174627956_0387

14/12/14 21:17:27 INFO mapreduce.Job: The url to track the job: http://cluster-4-0:8088/proxy/application_1417174627956_0387/

14/12/14 21:17:27 INFO mapreduce.Job: Running job: job_1417174627956_0387

14/12/14 21:17:37 INFO mapreduce.Job: Job job_1417174627956_0387 running in uber mode : false

14/12/14 21:17:37 INFO mapreduce.Job:map 0% reduce 0%

14/12/14 21:17:50 INFO mapreduce.Job:map 6% reduce 0%

14/12/14 21:17:51 INFO mapreduce.Job:map 10% reduce 0%

14/12/14 21:17:52 INFO mapreduce.Job:map 11% reduce 0%

14/12/14 21:17:53 INFO mapreduce.Job:map 15% reduce 0%

14/12/14 21:17:54 INFO mapreduce.Job:map 18% reduce 0%

14/12/14 21:17:55 INFO mapreduce.Job:map 19% reduce 0%

14/12/14 21:17:56 INFO mapreduce.Job:map 24% reduce 0%

14/12/14 21:17:57 INFO mapreduce.Job:map 27% reduce 0%

14/12/14 21:17:58 INFO mapreduce.Job:map 28% reduce 0%

14/12/14 21:17:59 INFO mapreduce.Job:map 33% reduce 0%

14/12/14 21:18:00 INFO mapreduce.Job:map 35% reduce 0%

14/12/14 21:18:01 INFO mapreduce.Job:map 37% reduce 0%

14/12/14 21:18:02 INFO mapreduce.Job:map 41% reduce 0%

14/12/14 21:18:03 INFO mapreduce.Job:map 43% reduce 0%

14/12/14 21:18:04 INFO mapreduce.Job:map 45% reduce 0%

14/12/14 21:18:05 INFO mapreduce.Job:map 49% reduce 0%

14/12/14 21:18:06 INFO mapreduce.Job:map 52% reduce 0%

14/12/14 21:18:07 INFO mapreduce.Job:map 53% reduce 0%

14/12/14 21:18:08 INFO mapreduce.Job:map 59% reduce 0%

14/12/14 21:18:09 INFO mapreduce.Job:map 66% reduce 0%

14/12/14 21:18:10 INFO mapreduce.Job:map 68% reduce 0%

14/12/14 21:18:11 INFO mapreduce.Job:map 81% reduce 0%

14/12/14 21:18:12 INFO mapreduce.Job:map 91% reduce 0%

14/12/14 21:18:13 INFO mapreduce.Job:map 92% reduce 0%

14/12/14 21:18:14 INFO mapreduce.Job:map 94% reduce 0%

14/12/14 21:18:15 INFO mapreduce.Job:map 96% reduce 0%

14/12/14 21:18:16 INFO mapreduce.Job:map 100% reduce 0%

14/12/14 21:18:21 INFO mapreduce.Job:map 100% reduce 1%

14/12/14 21:18:22 INFO mapreduce.Job:map 100% reduce 7%

14/12/14 21:18:23 INFO mapreduce.Job:map 100% reduce 14%

14/12/14 21:18:24 INFO mapreduce.Job:map 100% reduce 21%

14/12/14 21:18:25 INFO mapreduce.Job:map 100% reduce 22%

14/12/14 21:18:26 INFO mapreduce.Job:map 100% reduce 24%

14/12/14 21:18:27 INFO mapreduce.Job:map 100% reduce 25%

14/12/14 21:18:28 INFO mapreduce.Job:map 100% reduce 29%

14/12/14 21:18:29 INFO mapreduce.Job:map 100% reduce 36%

14/12/14 21:18:30 INFO mapreduce.Job:map 100% reduce 38%

14/12/14 21:18:31 INFO mapreduce.Job:map 100% reduce 42%

14/12/14 21:18:33 INFO mapreduce.Job:map 100% reduce 46%

14/12/14 21:18:35 INFO mapreduce.Job:map 100% reduce 49%

14/12/14 21:18:36 INFO mapreduce.Job:map 100% reduce 56%

14/12/14 21:18:37 INFO mapreduce.Job:map 100% reduce 60%

14/12/14 21:18:38 INFO mapreduce.Job:map 100% reduce 61%

14/12/14 21:18:39 INFO mapreduce.Job:map 100% reduce 64%

14/12/14 21:18:40 INFO mapreduce.Job:map 100% reduce 67%

14/12/14 21:18:41 INFO mapreduce.Job:map 100% reduce 68%

14/12/14 21:18:42 INFO mapreduce.Job:map 100% reduce 72%

14/12/14 21:18:43 INFO mapreduce.Job:map 100% reduce 76%

14/12/14 21:18:44 INFO mapreduce.Job:map 100% reduce 83%

14/12/14 21:18:46 INFO mapreduce.Job:map 100% reduce 89%

14/12/14 21:18:47 INFO mapreduce.Job:map 100% reduce 90%

14/12/14 21:18:48 INFO mapreduce.Job:map 100% reduce 93%

14/12/14 21:18:49 INFO mapreduce.Job:map 100% reduce 94%

14/12/14 21:18:51 INFO mapreduce.Job:map 100% reduce 96%

14/12/14 21:18:52 INFO mapreduce.Job:map 100% reduce 100%

14/12/14 21:18:56 INFO mapreduce.Job: Job job_1417174627956_0387 completed successfully

14/12/14 21:18:56 INFO mapreduce.Job: Counters: 49

File System Counters

FILE: Number of bytes read=1275232

FILE: Number of bytes written=12371381

FILE: Number of read operations=0

FILE: Number of large read operations=0

FILE: Number of write operations=0

HDFS: Number of bytes read=2088993303

HDFS: Number of bytes written=544

HDFS: Number of read operations=273

HDFS: Number of large read operations=0

HDFS: Number of write operations=144

Job Counters

Launched map tasks=19

Launched reduce tasks=72

Data-local map tasks=19

Total time spent by all maps in occupied slots (ms)=2391716

Total time spent by all reduces in occupied slots (ms)=4403300

Total time spent by all map tasks (ms)=597929

Total time spent by all reduce tasks (ms)=440330

Total vcore-seconds taken by all map tasks=597929

Total vcore-seconds taken by all reduce tasks=440330

Total megabyte-seconds taken by all map tasks=2449117184

Total megabyte-seconds taken by all reduce tasks=4508979200

Map-Reduce Framework

Map input records=190000000

Map output records=1790077

Map output bytes=23271001

Map output materialized bytes=1300723

Input split bytes=2005

Combine input records=0

Combine output records=0

Reduce input groups=32

Reduce shuffle bytes=1300723

Reduce input records=1790077

Reduce output records=32

Spilled Records=3580154

Shuffled Maps =1368

Failed Shuffles=0

Merged Map outputs=1368

GC time elapsed (ms)=1157

CPU time spent (ms)=819760

Physical memory (bytes) snapshot=37610864640

Virtual memory (bytes) snapshot=822321184768

Total committed heap usage (bytes)=167987642368

Shuffle Errors

BAD_ID=0

CONNECTION=0

IO_ERROR=0

WRONG_LENGTH=0

WRONG_MAP=0

WRONG_REDUCE=0

File Input Format Counters

Bytes Read=2088991298

File Output Format Counters

Bytes Written=544

14/12/14 21:18:56 INFO streaming.StreamJob: Output directory: /user/2014210***/output

2014210***@cluster-3-1:~$

第五步：将output复制到/home/2014210***文件夹中

2014210***@cluster-3-1:~$ hadoop fs -copyToLocal /user/2014210***/output /home/2014210***

2014210***@cluster-3-1:~$ ls

hw2mapper.pyoutputreducer.py

2014210***@cluster-3-1:~$ cd output/

2014210***@cluster-3-1:~/output$ ls

part-00000part-00013part-00026part-00039part-00052part-00065

part-00001part-00014part-00027part-00040part-00053part-00066

part-00002part-00015part-00028part-00041part-00054part-00067

part-00003part-00016part-00029part-00042part-00055part-00068

part-00004part-00017part-00030part-00043part-00056part-00069

part-00005part-00018part-00031part-00044part-00057part-00070

part-00006part-00019part-00032part-00045part-00058part-00071

part-00007part-00020part-00033part-00046part-00059_SUCCESS

part-00008part-00021part-00034part-00047part-00060

part-00009part-00022part-00035part-00048part-00061

part-00010part-00023part-00036part-00049part-00062

part-00011part-00024part-00037part-00050part-00063

part-00012part-00025part-00038part-00051part-00064

用more命令查看结果：

2014210***@cluster-3-1:~/output$ more part*

::::::::::::::

part-00000

::::::::::::::

::::::::::::::

part-00001

::::::::::::::

201421098756009

201421109256231

201421082255916

::::::::::::::

part-00002

::::::::::::::

::::::::::::::

part-00003

::::::::::::::

201421109456024

::::::::::::::

part-00004

::::::::::::::

::::::::::::::

part-00005

::::::::::::::

::::::::::::::

part-00006

::::::::::::::

201421082755762

::::::::::::::

part-00007

::::::::::::::

::::::::::::::

part-00008

::::::::::::::

201421109956058

::::::::::::::

part-00009

::::::::::::::

::::::::::::::

part-00010

::::::::::::::

::::::::::::::

part-00011

::::::::::::::

::::::::::::::

part-00012

::::::::::::::

::::::::::::::

part-00013

::::::::::::::

201421100556067

::::::::::::::

part-00014

::::::::::::::

::::::::::::::

part-00015

::::::::::::::

201421080656051

::::::::::::::

part-00016

::::::::::::::

::::::::::::::

part-00017

::::::::::::::

::::::::::::::

part-00018

::::::::::::::

::::::::::::::

part-00019

::::::::::::::

::::::::::::::

part-00020

::::::::::::::

::::::::::::::

part-00021

::::::::::::::

::::::::::::::

part-00022

::::::::::::::

::::::::::::::

part-00023

::::::::::::::

::::::::::::::

part-00024

::::::::::::::

::::::::::::::

part-00025

::::::::::::::

201421099056303

::::::::::::::

part-00026

::::::::::::::

::::::::::::::

part-00027

::::::::::::::

::::::::::::::

part-00028

::::::::::::::

::::::::::::::

part-00029

::::::::::::::

::::::::::::::

part-00030

::::::::::::::

201421083055630

::::::::::::::

part-00031

::::::::::::::

201421099655980

::::::::::::::

part-00032

::::::::::::::

::::::::::::::

part-00033

::::::::::::::

201421110055803

::::::::::::::

part-00034

::::::::::::::

201421083456005

::::::::::::::

part-00035

::::::::::::::

201421097056025

201421110255520

::::::::::::::

part-00036

::::::::::::::

::::::::::::::

part-00037

::::::::::::::

201421097256566

::::::::::::::

part-00038

::::::::::::::

201421083855563

::::::::::::::

part-00039

::::::::::::::

::::::::::::::

part-00040

::::::::::::::

201421081056113

::::::::::::::

part-00041

::::::::::::::

201421081155899

::::::::::::::

part-00042

::::::::::::::

201421081256001

::::::::::::::

part-00043

::::::::::::::

201421108355875

201421081356485

::::::::::::::

part-00044

::::::::::::::

201421108455704

::::::::::::::

part-00045

::::::::::::::

201421108555587

::::::::::::::

part-00046

::::::::::::::

::::::::::::::

part-00047

::::::::::::::

201421108755539

::::::::::::::

part-00048

::::::::::::::

201421108855773

201421081855954

::::::::::::::

part-00049

::::::::::::::

201421081955811

::::::::::::::

part-00050

::::::::::::::

::::::::::::::

part-00051

::::::::::::::

::::::::::::::

part-00052

::::::::::::::

::::::::::::::

part-00053

::::::::::::::

::::::::::::::

part-00054

::::::::::::::

::::::::::::::

part-00055

::::::::::::::

::::::::::::::

part-00056

::::::::::::::

::::::::::::::

part-00057

::::::::::::::

::::::::::::::

part-00058

::::::::::::::

::::::::::::::

part-00059

::::::::::::::

::::::::::::::

part-00060

::::::::::::::

::::::::::::::

part-00061

::::::::::::::

::::::::::::::

part-00062

::::::::::::::

::::::::::::::

part-00063

::::::::::::::

::::::::::::::

part-00064

::::::::::::::

::::::::::::::

part-00065

::::::::::::::

::::::::::::::

part-00066

::::::::::::::

::::::::::::::

part-00067

::::::::::::::

201421098156055

201421075056156

::::::::::::::

part-00068

::::::::::::::

201421098255879

::::::::::::::

part-00069

::::::::::::::

::::::::::::::

part-00070

::::::::::::::

::::::::::::::

part-00071

::::::::::::::

2014210***55733

2014210***@cluster-3-1:~/output$





















