# currval of sequence "follow_id_seq" is not yet defined in this session - =朝晖= - 博客园
# [currval of sequence "follow_id_seq" is not yet defined in this session](https://www.cnblogs.com/dhcn/p/7124628.html)
postgresql上使用
- select currval('follow_id_seq');  
报错：
- currval of sequence "follow_id_seq" is not yet defined in this session  
查阅参考资料，发现错误原因是要查当前值，必须先取一次next：执行以下语句，执行成功：
- dbname=# select nextval('follow_id_seq');  
-  nextval   
- ---------  
-     5347  
- (1 row)  
- 
- dbname=# select currval('follow_id_seq');  
-  currval   
- ---------  
-     5347  
- (1 row)  
http://sunboyyyl.blog.163.com/blog/static/224738120099135521859/

