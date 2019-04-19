# 批量删除es索引(索引格式为索引前缀-YYYY-mm-dd) - sxf_123456的博客 - CSDN博客
2017年12月19日 17:02:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：567
cre_index.sh
curl -XPUT 0.0.0.0:9200/gather-027-20170228
curl -XPUT 0.0.0.0:9200/gather-027-20170227
curl -XPUT 0.0.0.0:9200/gather-027-20170226
curl -XPUT 0.0.0.0:9200/gather-027-20170225
curl -XPUT 0.0.0.0:9200/gather-027-20170224
curl -XPUT 0.0.0.0:9200/gather-027-20170223
curl -XPUT 0.0.0.0:9200/gather-027-20170222
curl -XPUT 0.0.0.0:9200/gather-027-20170221
curl -XPUT 0.0.0.0:9200/gather-027-20170220
curl -XPUT 0.0.0.0:9200/gather-027-20170219
curl -XPUT 0.0.0.0:9200/gather-027-20170218
shell.py
import os
class Shell(object):
    def __init__(self,sh_path,host,port,index_pre,index_date,time_size,keep_count):
        self.shell = str(sh_path)
        self.host  = str(host)
        self.port  = str(port)
        self.index_pre = str(index_pre)
        self.index_date = str(index_date)
        self.time_size = str(time_size)
        self.keep_count = str(keep_count)
    def run_background(self):
        cmd_list=[]
        cmd_list.append(self.shell)
        cmd_list.append(self.host)
        cmd_list.append(self.port)
        cmd_list.append(self.index_pre)
        cmd_list.append(self.index_date)
        cmd_list.append(self.time_size)
        cmd_list.append(self.keep_count)
        pos = self.shell.rfind('/')+1
        #print(pos)
        #print(len(self.shell))
        sh_file = self.shell[pos:]
        exe = 'chmod +x '+sh_file
        #print(exe)
        os.system(exe)
        cmd = ' '.join(cmd_list)
        #print("cmd="+cmd)
        os.system(cmd)
if __name__=='__main__':
    # cmd="del_Index.sh 10.0.0.20 9200 test-0 2017-12-05 30 2"
    sh_path = "/root/sxf_test/del_Index.sh"
    host = "0.0.0.0"
    port = 9200
    index_pre = "gather-027"
    index_date = "20170228"
    time_size = 1
    keep_count = 0
    sh = Shell(sh_path,host,port,index_pre,index_date,time_size,keep_count)
    sh.run_background()
del_Index.sh
#!/bin/bash
#############################
# @author: sxf
# @file: del_Index.sh
# @time: 2017-12-05 10:32
# @program: shell
#############################
# $1 esip 如:10.0.0.20
# $2 esport 如:9200
# $3 索引前缀 如:gather-010
# $4 索引日期 如:2017-12-05
# $5 索引粒度 如:1天
# $6 索引保留数量 如:3天则保留
#   gather-010-2017-12-05,
#   gather-010-2017-12-04,
#   gather-010-2017-12-03这三个索引，其余索引删掉
if [ $# -ge 0 ] && [ $# -lt 6 ]
then
    echo "Cmd line have four parameter after execut_file"
    echo "Usage:arvg[1] argv[2] argv[3] argv[4] argv[5] argv[6]"
    echo "argv[1] esip"
    echo "argv[2] esport"
    echo "argv[3] es索引前缀"
    echo "argv[4] es索引日期(yyyyMMdd)"
    echo "argv[5] es索引粒度"
    echo "argv[6] es索引保留数量"
    exit 1
else
    esip=$1
    esport=$2
    index_pre=$3
    index_date=$4
    time_size=$5
    keep_count=$6
fi
esUrl=${esip}:${esport}
index_new=$index_pre-*
indexfile=${index_pre}-${index_date}-index.txt
ch='-'
flag=0
if echo $index_date | grep -q "-" && date -d $index_date +%Y%m%d >> /dev/null
then
    echo "es索引日期格式为yyyyMMdd,不是格式yyyy-MM-dd"
    exit 1
else
    data=$(echo ${#index_date})
    if [ $data != 8 ]
    then
        echo "es索引日期格式为yyyyMMdd"
        exit 1
    fi
    #year=${index_date:0:4}
    month=${index_date:4:2}
    if [ $month -gt 12 ] || [ $month -lt 01 ]
    then
echo "es索引日期月份${month}错误"
        exit 1
    fi
    if echo $month | grep -q '0' >> /dev/null
    then
        new_month=${month:1:1}  #10月
        if [ $new_month == 0 ]
        then
            new_month=${month}
        fi
    else
        new_month=${month}
    fi
    #查询符合条件的索引，写入文件
    curl $esUrl/_cat/indices/$index_new | awk 'NR>=1' | awk '{print $3}' | sort > $indexfile 
    if [ $time_size == 1 ] && [ $keep_count != 0 ]
    then
        keep=`expr $keep_count - 1`
        #设置索引保留的开始日期
        indexdate=$(date -d"${keep} day ago ${index_date}" +%Y%m%d)
    elif [ $time_size == 7 ] && [ $keep_count != 0 ]
    then
        keep=`expr $keep_count \* $time_size - 1`
        indexdate=$(date -d"${keep} day ago ${index_date}" +%Y%m%d)
    elif [ $time_size == 30 ] && [ $keep_count != 0 ]
    then
        #echo "new_month="$new_month
        #echo "keep_count="$keep_count
        start=`expr $new_month - $keep_count + 1`
        len_start=$(echo ${#start}) #取出开始月的长度
        #echo "sta="$start
        #echo "len_start="$len_start
        if [ $len_start -lt 2 ]
        then
            start='0'${start}
        else
            start=$start
        fi
        #echo $start
    fi
    #echo "keep="$keep
    #echo "keep_count!=0,start="$start               
    #exit 1
    if [ $keep_count == 0 ] #保留天数为0
    then
   indexdate=$index_date
    fi
    #echo $indexdate
    if [ $time_size == 30 ] && [ $keep_count == 0 ] #间隔为月，保留天数为0
    then
        start=$new_month
        len_start=$(echo ${#start})
        if [ $len_start -lt 2 ]
        then
            start='0'${start}
        fi
    fi
    #echo "keep_count==0,start="$start   
    if [ $time_size == 7 ] || [ $time_size == 1 ] #间隔为周，或间隔为日
    then 
        for item in `cat ${indexfile}`  
        do    
            parameter=${esUrl}/${item}
            let pos=`echo "$item" | awk -F ''$ch'' '{printf "%d", length($0)-length($NF)}'`
            in_date=${item:${pos}:17}
            #echo $in_date
            if [ $keep_count == 0 ] #保留0天
            then        
                if [ $indexdate -ge  $in_date ]  
                then
                    var=$(curl -XDELETE ${parameter})
                    echo "Del--"${parameter}--$var
                fi
            else
                if [ $indexdate -gt $in_date ]
                then
                     var=$(curl -XDELETE ${parameter})
                     echo "Del--"${parameter}--$var 
                fi
            fi
        done 
    fi
    #echo "start="$start
    if  [ $time_size == 30 ]  #间隔为月
    then
        for item in `cat ${indexfile}`  
        do    
            parameter=${esUrl}/${item}
            let pos=`echo "$item" | awk -F ''$ch'' '{printf "%d", length($0)-length($NF)}'`
            in_date=${item:${pos}:17}
            mm=${in_date:4:2}
            #echo "mm="$mm  
            if [ $keep_count == 0 ] #保留天数为0
            then        
                if [ $start -ge  $mm ]  
                then
                    var=$(curl -XDELETE ${parameter})
                    echo "Del--"${parameter}--$var
                fi
            else
                if [ $start -gt $mm ]
                then
                     var=$(curl -XDELETE ${parameter})
                     echo "Del--"${parameter}--$var 
                fi
            fi
        done 
    fi
fi
