# centos磁盘空间满查询和移动命令小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月09日 11:40:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3225








1、df -h 

  发现挂在根目录/的逻辑卷100%使用，/data目录挂载的利用率只有1%，需将/文件移动到/data下。

 /dev/mapper/VolGroup-LogVol01 14G   14G     0 100% /

/dev/mapper/datavg-datalv 99G  214M   94G   1% /data


2、du / --max-depth=1 -h

      查看根目录下文件夹占用空间，发现/home和/usr下占用最多

     5.8G    /usr

    6.5G    /home



3、  du /home/bigdata --max-depth=1 -h 

      看到6.5G    /home/bigdata/software 文件夹占用



      find /home -size +100M

      查看哪些文件大于100M；移动这些文件到/data下




4、mv /home/bigdata/software  /data

      将文件夹software下文件都移动到/data




5、df -h

     /dev/mapper/VolGroup-LogVol01  14G  6.7G  6.3G  52% /

     空间利用率只有52%




