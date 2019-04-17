# linux中的crontab定时执行 - miner_zhu的博客 - CSDN博客





2018年09月13日 15:49:48[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：20标签：[linux																[定时](https://so.csdn.net/so/search/s.do?q=定时&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/miner_zhu/article/category/7903611)








# 环境

ubuntu

# 定时实现

cron不是自动运行的，所以可以

```python
sudo service cron start
```

查看cron是否运行 
`ps -ef | grep cron`
1：编辑crontab文件，用来存放你要执行的命令

sudo crontab -e

2：第一次用这个命令，会让你选择文本编辑器（输入数字选择）

我选的是vim 

3：添加执行命令

0 12 * * * /home/dell/report.py

sendeamil.py就是我需要自动运行的脚本，这句话的意思就是每天中午12点都会自动执行这个脚本！

4：重启cron来应用这个计划任务

sudo service cron restart



参考文章：[https://blog.csdn.net/katyusha1/article/details/78619549](https://blog.csdn.net/katyusha1/article/details/78619549)




