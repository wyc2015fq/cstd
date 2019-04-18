# PL/SQL语法 - 游标 - weixin_33985507的博客 - CSDN博客
2019年01月08日 04:52:25[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
Oracle中的游标分为显式游标和隐式游标。
隐式游标是系统自动为你创建的。
显式游标是用户通过创建CURSOR的方式来创建。
在Oracle中有三种类型的游标：
**1）不带参数的游标**      eg:
cursor customer_cur is
            select * from customers;      
**2）带参数的游标**      eg:
cursor customer_cur(customer_id in number) is
            select * from customers
            where customerid = customer_id;
**3）有返回结果的游标**eg:
cursor customer_cur return customers%rowtype is
            select * from customers;
游标的五大属性：
1）%FOUND              - 记录找到
2)  %NOTFOUND        - 记录没找到
3)  %ROWCOUNT       - 游标当前记录数
                                    （每取回一条记录，ROWCOUNT将增加1）
4)  %ISOPEN              - 游标是否被打开
5)  %ROWTYPE           - 代表游标取回的某一行记录
游标使用四大步骤：
**1) 声明游标**      cursor is select语句
**2) 打开游标**
      open 游标名
**3) 从游标中取回数据**      fetch 游标名 into 行记录变量
**4) 关闭游标**      close 游标名
下面我们就来举个例子：
```
declare 
  -- 声明游标
  cursor customer_cur(city_in char) is
    select * from customers
    where city = city_in;
  customer_rec customer_cur%ROWTYPE;
begin
  -- 打开游标
  if not customer_cur%ISOPEN then
    open customer_cur('Madrid');
  end if;
  
  -- 从游标中取回数据
  fetch customer_cur into customer_rec;
  
  /*
    使用游标
  */
  while customer_cur%FOUND
  loop
    dbms_output.put_line(TO_CHAR(customer_cur%ROWCOUNT) || ':' || customer_rec.customerid  
      || ', ' || customer_rec.city);
    fetch customer_cur into customer_rec;
  end loop;
  
  -- 关闭游标
  close customer_cur;
  
end;
```
