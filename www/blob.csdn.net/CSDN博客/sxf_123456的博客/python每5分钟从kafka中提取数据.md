# python每5分钟从kafka中提取数据 - sxf_123456的博客 - CSDN博客
2018年05月13日 11:42:32[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1119
import sys
sys.path.append("..")
from datetime import datetime
from utils.kafka2file import KafkaDownloader
import  os
"""
实现取kafka数据，文件按照取数据的间隔命名
如每5分钟从kafka取数据写入文件中，文件名为当前时间加5
"""
TOPIC = "rtz_queue"
HOSTS = "ip:9092,ip:9092"
GROUP = "2001"
def get_end_time(hour,minute,time_step):
    if (minute+time_step)%60<60:
        if (minute+time_step)%60<10:
            return str(hour+int((minute+time_step)/60))+":"+"0"+str((minute+time_step)%60)
        else:
            return str(hour+int((minute+time_step)/60))+":"+str((minute+time_step)%60)
    else:
        pass
def kafkawritefile(time_step,time_num):
    start = datetime.now()
    downloader = KafkaDownloader(HOSTS, TOPIC, GROUP)
    i = 1
while(i<=time_num):
        end_time = get_end_time(start.hour, start.minute,i*time_step)
        end_time_file = end_time.replace(':', '_')
        outfile_path = "/data/tmp/" + end_time_file + ".csv"
if os.path.exists(outfile_path):
            os.remove(outfile_path)
        writefile = open(outfile_path, 'a+', encoding='utf-8')
       
        for msg in downloader.message():
            curr_time = datetime.now()
            curr_time = str(curr_time)
            split_curr_time = curr_time.split(' ')
            curr_time_str = split_curr_time[1][0:5]
        
            if curr_time_str >= str(end_time):        
                break
i += 1
if __name__=='__main__':
    time_step = 15
time_num = 1
kafkawritefile(time_step,time_num)
