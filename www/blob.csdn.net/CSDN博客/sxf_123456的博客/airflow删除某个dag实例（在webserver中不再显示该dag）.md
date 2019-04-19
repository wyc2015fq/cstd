# airflow删除某个dag实例（在webserver中不再显示该dag） - sxf_123456的博客 - CSDN博客
2018年03月10日 11:32:49[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1542
删除webserver中的dag实例
一、airflow的版本为1.8
以dags目录下的dag_id为test为例，程序名为test_tt.py
1、使用airflow resetdb命令清除数据库
2、删除dag文件
3、关闭webserver：
ps -ef|grep -Ei '(airflow-webserver)'| grep master | awk '{print $2}'|xargs -i kill {}
4、重启webserver
airflow webserver -p 8080 & 或者 airflow webserver -D
5、打开浏览器并刷新，查看dag是否删除
二、airflow的版本为1.9
1、删除dag文件
2、使用airflow resetdb命令清除数据库
