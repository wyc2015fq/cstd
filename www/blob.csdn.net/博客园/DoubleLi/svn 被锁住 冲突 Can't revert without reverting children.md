# svn 被锁住 冲突 Can't revert without reverting children - DoubleLi - 博客园






# 解决

执行以下命令 

D:\development\work>svn rm –-keep-local D:\development\work\ohho\common\logic\backstage\operation 

结果 

D         ohho\common\logic\backstage\operation 

D         ohho\common\logic\backstage\operation__init__.py 

D         ohho\common\logic\backstage\operation\logic_fail_match_log.py 

D         ohho\common\logic\backstage\operation\logic_match_meet_count 

D         ohho\common\logic\backstage\operation\logic_match_meet_count.py 

D         ohho\common\logic\backstage\operation\logic_match_meet_count__init__.py 

D         ohho\common\logic\backstage\operation\logic_user_information.py

然后在上级目录 执行revert

在这个目录上面执行 clean up

revert









