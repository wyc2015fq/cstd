# oracle 游标 - 左直拳的马桶_日用桶 - CSDN博客
2017年07月09日 12:08:44[左直拳](https://me.csdn.net/leftfist)阅读数：350标签：[oracle																[存储过程																[游标																[cursor](https://so.csdn.net/so/search/s.do?q=cursor&t=blog)](https://so.csdn.net/so/search/s.do?q=游标&t=blog)](https://so.csdn.net/so/search/s.do?q=存储过程&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
游标最符合我们思维了，平时用惯了高级语言，一言不合就循环。根据我有限的写存储过程的心得，能不用游标就不用，数据库的强项在于表关联，过滤，sql写得好，也许根本不用游标进行循环，而速度则远远超过后者。
但有时候，好像还是要用游标。感觉oracle的游标与SQL SERVER大同小异。无非就是声明、开启、遍历、关闭，这几个步骤。套路如下：
```
create or replace procedure pro_test_cursor is
begin
--声明一个游标
  DECLARE
  CURSOR c_id
  IS select id from(
  SELECT 1 id from dual 
  union all
  SELECT 2 id from dual
  union all
  SELECT 3 id from dual) 
  order by id;
  v_id number(1);
  begin
  --以下是两种遍历方式
    --1、判断是否已经遍历完毕在前
    open c_id;--开启游标
    LOOP
      FETCH c_id INTO v_id;
      EXIT WHEN c_id%NOTFOUND;
      insert into tmp_cursor(id) values(v_id);
    END LOOP;  
    CLOSE c_id;--关闭游标
    --2、判断是否已经遍历完毕在后
    open c_id;
    FETCH c_id INTO v_id;
    WHILE c_id%FOUND LOOP
      insert into tmp_cursor(id) values(v_id);
      FETCH c_id INTO v_id;
    END LOOP;  
    CLOSE c_id;
  end;
  commit;
end pro_test_cursor;
/
```
这个边界，我总是搞不清楚，这里总结一下： 
游标猜测是有个类似结尾符合的东西，指针来到这里，就是NOTFOUND了。而在此之前，即使是最后一条记录，指针的状态都是FOUND的。
OPEN游标，指针应该是在开启位置，没有啥FOUND或NOT FOUND的状态；FETCH开始后，指针才指向第一条记录，指针状态为FOUND，所以**这个FOUND状态，不是指下一条记录可以FOUND，而是指当前指针为FOUND**。继续遍历，到了结尾符合，就NOTFOUND了。
