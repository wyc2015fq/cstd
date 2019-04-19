# postgresql 存储过程 - Ms.Fox的专栏 - CSDN博客
2019年02月20日 17:18:35[lulu-lu](https://me.csdn.net/smbluesky)阅读数：17
1 查询存储过程
select  pg_proc.proname, prosrc  from pg_proc  where proname = 'radacct_update_deviceid'; 
'radacct_update_deviceid 是存储过程的名称
![](https://img-blog.csdnimg.cn/20190220171810330.png)
2、创建存储过程
这个是无返回值的存储过程
CREATE OR REPLACE FUNCTION function_dev () 
RETURNS **void **AS $$
BEGIN  
  UPDATE t_wifi SET dev='17fe';
END; 
$$
LANGUAGE plpgsql ;
当创建好以后，在触发器中准备使用时，会报ERROR:  function insert_cdr must return type "trigger"这个错误。
先删除这个存储过程
DROP FUNCTION function_dev () 
然后再重新创建
DROP FUNCTION rzhflows_update_devmac() 
CREATE OR REPLACE FUNCTION rzhflows_update_devmac () 
RETURNS **trigger **AS $$
BEGIN  
  UPDATE t_wifi_rzhflows SET devmac='17:43:23:6a:d7:fe';
END; 
$$
LANGUAGE plpgsql ;
3 行触发器和语句触发器的区别
**语句级触发器执行每个SQL时，只执行一次 ；行级触发器每行都会执行一次**
