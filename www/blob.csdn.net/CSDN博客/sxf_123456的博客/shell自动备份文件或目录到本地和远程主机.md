# shell自动备份文件或目录到本地和远程主机 - sxf_123456的博客 - CSDN博客
2017年04月28日 09:03:52[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：356
#!/bin/bash
#Function:Automatically backup a given list of directories 
#or files, and can retain the N day backup files.
#Can be back to the directory specified by the remote host,
# but the need for free password login to the remote host
#This script contains two files:Executable file "backup.sh" and
# backup project file "bakfile.lst" ,two files must be in the same
 directory
#Author:JianJie
#Date:2016/8/26
#Usage: sh backup.sh &
#Version:v2
###############################################################
LOG_FILE=/backup/local/log/`date +"%Y-%m"`.log #Log file
LOG_DIR=/backup/local/log
DATE_FORMAT=`date +"%Y-%m-%d"` 
BAK_FILE_LST=bakfile.lst #Need to backup the file or directory to
 this file
BAK_DIR=/backup/local #Backup directory
KEEP_DAYS=5 #File retention days
REMOTE_HOST="" #Split between multiple hosts with ':'
REMOTE_DIR=/backup/$HOSTNAME #Remote host backup directory
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" #The
 directory where the current script is running
FORMAT="tar.gz" #The compression format can be:tar.xz,tar.gz,tar.bz2
###############################################################
if [ "$REMOTE_HOST" != "" ];then
#Determine whether there is a backup directory on the remote host
ssh -t -q -n "$REMOTE_HOST" "[ ! -d "$REMOTE_DIR" ] && mkdir -p "$REMOTE_DIR""
fi
#Determine whether log directory and log file exists
if [ ! -d $LOG_DIR ];then
mkdir -p $LOG_DIR
touch $LOG_FILE
else
if [ ! -e $LOG_FILE ];then
touch $LOG_FILE
fi
fi
#LogStart
echo "Backup start at $(date +"%Y-%m-%d %H:%M:%S")" >> $LOG_FILE
echo "--------------------------------------------------------" >>
 $LOG_FILE
echo " " >> $LOG_FILE
#Determine whether the file list 'bakfile.lst' exists
if [ ! -e $CURRENT_DIR/$BAK_FILE_LST ];then
echo "Error:Important file [$CURRENT_DIR/$BAK_FILE_LST] has been lost,now
 creating..." >> $LOG_FILE
touch $CURRENT_DIR/$BAK_FILE_LST
echo "The file [$CURRENT_DIR/$BAK_FILE_LST] has been created,but you
 still need to write data in it!" >> $LOG_FILE
echo " " >> $LOG_FILE
echo "--------------------------------------------------------" >>
 $LOG_FILE
echo "Backup end at $(date +"%Y-%m-%d %H:%M:%S")" >> $LOG_FILE
echo " " >> $LOG_FILE
exit 3
fi
for BACK_FILE in $(cat $CURRENT_DIR/$BAK_FILE_LST)
do 
BASE_NAME=`echo "$BACK_FILE" | xargs basename`
OLD_FILE=$BAK_DIR/$BASE_NAME.$(date --date="$KEEP_DAYS days ago" +"%Y-%m-%d").$FORMAT
if [ -e "$BACK_FILE" ];then
cd $(dirname $BACK_FILE)
#If a backup file already exists, it will no longer be backed up
if [ -f $BAK_DIR/$BASE_NAME.$DATE_FORMAT.$FORMAT ];then
echo "Backup file [$BAK_DIR/$BASE_NAME.$DATE_FORMAT.$FORMAT] already
 exists!" >> $LOG_FILE
else
cp -RHpf $BASE_NAME $BAK_DIR
cd $BAK_DIR
case $FORMAT in
tar.xz)
tar -cpJf $BASE_NAME.$DATE_FORMAT.$FORMAT $BASE_NAME ;;
tar.gz)
tar -cpzf $BASE_NAME.$DATE_FORMAT.$FORMAT $BASE_NAME ;;
tar.bz2)
tar -cpjf $BASE_NAME.$DATE_FORMAT.$FORMAT $BASE_NAME ;;
*)
echo "Compress FORMAT error,default compression FORMAT:tar.gz" >>
 $LOG_FILE
tar -cpzf $BASE_NAME.$DATE_FORMAT.tar.gz $BASE_NAME ;;
esac
#Backup files back to the remote host via scp
if [ "$REMOTE_HOST" != "" ];then
for host in $(echo "$REMOTE_HOST" | sed 's/:/ /g')
do
scp -o ConnectTimeout=10 $BASE_NAME.$DATE_FORMAT.$FORMAT $host:$REMOTE_DIR
 >>$LOG_FILE 2>/dev/null && \
echo "Successful backup [$BACK_FILE] to the remote host:[$host:$REMOTE_DIR]"
 >>$LOG_FILE || \
echo "Backup [$BACK_FILE] to the remote host:[$host:$REMOTE_DIR] failed,please
 check it" >>$LOG_FILE
done
fi
#Backup complete, delete the copy of the source file
rm -rf $BASE_NAME
echo "Backup [$BACK_FILE] done into [$BAK_DIR] successfuly!" >> $LOG_FILE
#Existing N days old file, delete it
if [ -f $OLD_FILE ];then
rm -rf $OLD_FILE
echo "[$OLD_FILE],delete old file successfuly!" >> $LOG_FILE
fi
fi
else
echo "Wrong directory or file:[$BACK_FILE],please check it" >>$LOG_FILE
fi
done
echo " " >> $LOG_FILE
echo "--------------------------------------------------------" >>
 $LOG_FILE
echo "Backup end at $(date +"%Y-%m-%d %H:%M:%S")" >> $LOG_FILE
#LogEnd
echo " " >> $LOG_FILE
exit 0
