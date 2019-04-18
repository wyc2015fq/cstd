# Oracle数据库建示例 - wsp_1138886114的博客 - CSDN博客





2019年03月26日 13:46:05[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：51











### 文章目录
- [一、 简单表格](#__1)
- [二、自增序列的表格](#_41)
- [三、涉及表空间的表格创建](#_112)




### 一、 简单表格

这个是最简单的表格，不涉及各种约束。字段的 “int” 类型不能设置字符长度,否则报错。

```
-- Create table
create table Invoice_data_Table
(
  billid                  varchar2(30),
  total                   varchar2(30),
  invoice_no              varchar2(30),
  invoice_code            varchar2(30),
  create_date             varchar2(30),
  check_code              varchar2(30),
  is_invoice              int,
  return_code             int,
  description             varchar2(30)
)
;
-- Add comments to the table 
comment on table Invoice_data_Table
  is '识别结果表';
-- Add comments to the columns 
comment on column Invoice_data_Table.billid
  is '参考主键';
comment on column Invoice_data_Table.total
  is '发票金额';
comment on column Invoice_data_Table.invoice_no
  is '发票号码';
comment on column Invoice_data_Table.invoice_code
  is '发票代码';
comment on column Invoice_data_Table.create_date
  is '开票日期';
comment on column Invoice_data_Table.check_code
  is '校验码';
comment on column Invoice_data_Table.is_invoice
  is '是否增值税发票';
comment on column Invoice_data_Table.return_code
  is '返回码';
comment on column Invoice_data_Table.description
  is '返回详情';
```

### 二、自增序列的表格

这次添加的自增序列 ‘id’ 和触发器；相当于主键。（MySQL中有自增主键，Oracle中没有，所有要用到触发器）

```
-- Create table
create table Invoice_data_Table
(
  id                      varchar2(100),
  billid                  varchar2(100),
  total                   varchar2(100),
  invoice_no              varchar2(100),
  invoice_code            varchar2(100),
  create_date             varchar2(100),
  check_code              varchar2(100),
  start_time              timestamp,
  end_time                timestamp,
  is_invoice              int,
  return_code             int,
  description             varchar2(100)
)
;
-- Add comments to the table 
comment on table Invoice_data_Table
  is '识别结果表';
-- Add comments to the columns 
comment on column Invoice_data_Table.billid
  is '参考主键';
comment on column Invoice_data_Table.total
  is '发票金额';
comment on column Invoice_data_Table.invoice_no
  is '发票号码';
comment on column Invoice_data_Table.invoice_code
  is '发票代码';
comment on column Invoice_data_Table.create_date
  is '开票日期';
comment on column Invoice_data_Table.check_code
  is '校验码';
comment on column Invoice_data_Table.start_time
  is '开始时间';
comment on column Invoice_data_Table.end_time
  is '结束时间';
comment on column Invoice_data_Table.is_invoice
  is '是否增值税发票';
comment on column Invoice_data_Table.return_code
  is '返回码';
comment on column Invoice_data_Table.description
  is '返回详情';

-- 创建序列
create sequence ID_sequence
minvalue 1 
maxvalue 99999999 
start with 1 
increment by 1 
NOCYCLE
nocache;

-- 创建触发器
create or replace trigger ID_trigger
BEFORE insert ON Invoice_data_Table 
FOR EACH ROW
begin 
  select ID_sequence.nextval into:new.id from dual;
end;

--================== 分割线 ========================
--查询序列
select ID_sequence.nextval from dual;

--查看触发器
select * from ID_trigger;
```

### 三、涉及表空间的表格创建

```
-- Create table
create table Invoice_data_Table
(
  id                      varchar2(100),
  billid                  varchar2(100),
  total                   varchar2(100),
  invoice_no              varchar2(100),
  invoice_code            varchar2(100),
  create_date             varchar2(100),
  check_code              varchar2(100),
  start_time              timestamp,
  end_time                timestamp,
  is_invoice              int,
  return_code             int,
  description             varchar2(100)
)
tablespace invoice_data
  pctfree 10
  initrans 1
  maxtrans 255
  storage
  (
    initrans 1M
    next 1M
    minextents 1
    maxextents unlimited
  );
;
-- Add comments to the table 
comment on table Invoice_data_Table
  is '识别结果表';
-- Add comments to the columns 
comment on column Invoice_data_Table.billid
  is '参考主键';
comment on column Invoice_data_Table.total
  is '发票金额';
comment on column Invoice_data_Table.invoice_no
  is '发票号码';
comment on column Invoice_data_Table.invoice_code
  is '发票代码';
comment on column Invoice_data_Table.create_date
  is '开票日期';
comment on column Invoice_data_Table.check_code
  is '校验码';
comment on column Invoice_data_Table.start_time
  is '开始时间';
comment on column Invoice_data_Table.end_time
  is '结束时间';
comment on column Invoice_data_Table.is_invoice
  is '是否增值税发票';
comment on column Invoice_data_Table.return_code
  is '返回码';
comment on column Invoice_data_Table.description
  is '返回详情';

-- 创建序列
create sequence ID_sequence
minvalue 1 
maxvalue 99999999 
start with 1 
increment by 1 
NOCYCLE
nocache;

-- 创建触发器
create or replace trigger ID_trigger
BEFORE insert ON Invoice_data_Table 
FOR EACH ROW
begin 
  select ID_sequence.nextval into:new.id from dual;
end;
--================== 分割线 ========================
--查询序列
select ID_sequence.nextval from dual;

--查看触发器
select * from ID_trigger;
```



