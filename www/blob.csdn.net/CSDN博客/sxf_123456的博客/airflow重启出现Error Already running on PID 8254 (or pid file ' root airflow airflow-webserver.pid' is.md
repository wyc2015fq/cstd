# airflow重启出现Error: Already running on PID 8254 (or pid file '/root/airflow/airflow-webserver.pid' is - sxf_123456的博客 - CSDN博客
2018年04月18日 10:24:55[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：569
                重启airflow出现以下错误。
[root@localhost example]# airflow webserver -p 8080  &
Error: Already running on PID 8254 (or pid file '/root/airflow/airflow-webserver.pid' is stale)
解决办法：找到该进行号的进程，找到进程号后第二列，再查找第二列为进程号的进程
[root@localhost example]# 
[root@localhost example]# ps -ef | grep 8254
root      8254  8252  0 09:58 pts/1    00:00:01 [gunicorn: maste] <defunct>
root      8319  7986  0 10:02 pts/0    00:00:00 grep --color=auto 8254
[root@localhost example]# ps -ef | grep 8252
root      8252  7890 10 09:58 pts/1    00:00:25 /root/anaconda3/bin/python /root/anaconda3/bin/airflow webserver -p 8080
root      8254  8252  0 09:58 pts/1    00:00:01 [gunicorn: maste] <defunct>
root      8323  7986  0 10:03 pts/0    00:00:00 grep --color=auto 8252
[root@localhost example]# kill 8252
重新重启airflow webserver
[root@localhost example]# airflow webserver -p 8080  &            
