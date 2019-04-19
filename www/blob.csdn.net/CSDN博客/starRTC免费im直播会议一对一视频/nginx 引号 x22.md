# nginx 引号 x22 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月23日 16:54:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：4060
这个好像是nginx故意这样做的。
因为Nginx默认的log_format使用双引号作为间隔符，为了避免日志分析时候出现混乱，所以将双引号解析为x22了。
只能每天日志切割的时候，自己替换日志中的x22字符为双引号了
我的解决方法：
sed 's#\\x22#"#g' test.txt
可以加-i参数以让修改在文件中生效！
#!/bin/bash
#setting log path
log_files_path="/usr/local/nginx/logs/"
log_files_dir=${log_files_path}$(date -d "yesterday" +"%Y")/$(date -d "yesterday" +"%m")
#setting nginx
nginx_sbin="/usr/local/nginx/sbin/nginx"
if [ ! -d $log_files_dir ]; then
mkdir -p $log_files_dir
fi
cd $log_files_path
#setting log name
log_files_name=(www.abc.com)
log_files_num=${#log_files_name[@]}
for((i=0;i<$log_files_num;i++));do
mv ${log_files_path}${log_files_name[i]}.log ${log_files_dir}/${log_files_name[i]}.log
/bin/sed -e 's/x22/"/g' ${log_files_dir}/${log_files_name[i]}.log > ${log_files_dir}/${log_files_name[i]}_$(date -d "yesterday" +"%Y%m%d").log
/bin/rm ${log_files_dir}/${log_files_name[i]}.log
done
kill -USR1 `cat /usr/local/nginx/nginx.pid`
自己手动转
touch convert.log
sed -e 's/x22/"/g' api.starconnect.cn.log > convert.log
