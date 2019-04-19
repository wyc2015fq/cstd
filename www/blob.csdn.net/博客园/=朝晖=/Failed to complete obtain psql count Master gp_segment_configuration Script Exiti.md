# Failed to complete obtain psql count Master gp_segment_configuration Script Exiti - =朝晖= - 博客园
# [Failed to complete obtain psql count Master gp_segment_configuration Script Exiti](https://www.cnblogs.com/dhcn/p/8446209.html)
问题： 在初始化过程中，如到以下问题：
gpadmin-[FATAL]:-Failed to complete obtain psql count Master gp_segment_configuration  Script Exiting!
Script has left Greenplum Database in an incomplete state
解决方法：
echo "RemoveIPC=no" >> /etc/systemd/logind.conf
/bin/systemctl restart systemd-logind.service
这个解决方案在Ubuntu greenplum-db-oss 5.4.1 场景下一样可用 
参考网址：https://github.com/greenplum-db/gpdb/issues/197

