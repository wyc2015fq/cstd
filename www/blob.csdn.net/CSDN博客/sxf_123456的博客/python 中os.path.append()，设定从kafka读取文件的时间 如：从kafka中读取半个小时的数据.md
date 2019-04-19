# python 中os.path.append()，设定从kafka读取文件的时间 如：从kafka中读取半个小时的数据 - sxf_123456的博客 - CSDN博客
2017年09月05日 21:42:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：987
                
os.path.append()
对于模块和自己写的程序不在同一目录下，可以把模块的路径通过sys.path.append(路径)添加到程序中。
需要在程序开头加上：
import sys
sys.path.append('引用模块的地址')
例子:
#C:\Users\Administrator\Desktop\Hello.py  #Hello.py程序存放路径
#Hello.py
def hello():
    print("hello,world")
import sys
sys.path.append('C:\\Users\\Administrator\\Desktop')
import Hello
Hello.hello()
设定从kafka读取文件的时间
如：从kafka中读取半个小时的数据
以下代码实现：
import sys
sys.path.append("..")
from datetime import datetime
from utils.kafka2file import KafkaDownloader  #引入KafkaDownloader模块
import  os
TOPIC = "rtz_queue"
HOSTS = "1.1.1.1:9092,1.1.1.2:9092"
GROUP = "2001"
def get_end_time(hour,minute,time_step):
    if (minute+time_step)%60<60:
        if (minute+time_step)%60<10: #小于10分钟的转化为分钟数前添加0
            return str(hour+int((minute+time_step)/60))+":"+"0"+str((minute+time_step)%60)
        else:
            return str(hour+int((minute+time_step)/60))+":"+str((minute+time_step)%60)
    else:
        pass
#@time_step:时间间隔
#@tme_num:时间间隔次数
def kafkawritefile(time_step,time_num):
    start = datetime.now()
    downloader = KafkaDownloader(HOSTS, TOPIC, GROUP)
    i = 1
    while(i<=time_num):
        end_time = get_end_time(start.hour, start.minute,i*time_step) #读取数据，每次结束时间
        end_time_file = end_time.replace(':', '_')  #文件名中不能用:，用-替换掉
        outfile_path = "/data/tmp/" + end_time_file + ".csv"
        if os.path.exists(outfile_path):
            os.remove(outfile_path)
        writefile = open(outfile_path, 'a+')
        for msg in downloader.message():
            msg1 = str(msg)
            msg2 = msg1[2:len(msg1)-1]
            curr_time = datetime.now()
            curr_time = str(curr_time)
            split_curr_time = curr_time.split(' ')
            curr_time_str = split_curr_time[1][0:5]
            if curr_time_str<str(end_time):
                writefile.write(str(msg2))
                writefile.write('\n')
            else:
                writefile.write(str(msg2))
                writefile.write('\n')
                writefile.close()
                break
        i += 1
if __name__=='__main__':
    time_step = 5
    time_num = 6
    kafkawritefile(time_step,time_num)
