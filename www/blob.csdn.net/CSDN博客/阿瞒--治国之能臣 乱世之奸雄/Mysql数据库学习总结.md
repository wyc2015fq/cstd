# Mysql数据库学习总结 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年11月17日 14:22:34[baofenny](https://me.csdn.net/jiaminbao)阅读数：1502
mysql可以分为四大类：
 一个数据库、一个表代表一个数据对象；
 DDL(数据定义)
 DML(数据操作)
 DQL(数据查询)
 DCL(数据控制) 
数据值和列类型
 细分都是按空间大小来区分的
 可以存下就可以
 1.数值型
  整型（整数）
  TINYINT 非常小的整型 1byte -128---127 2的8次方 0---255（无符号型UNSIGNED）
  SIMALLINT较小的整型 2byte -32768--32767 2的16次方 0---65535
  MEDIUHINT中等大小的整形 3byte   0---1677215
  INT 标准的整数型 4byte
  BIGINT 大整数型 8byte
  浮点型（小数）
  float(m,d)  4byte
  double(m,d)  8byte
  定点数 decinal  (m+2)byte
 2.字符型
  char(m)  255 固定长度
  varchar(m) 255 可变长度
  两者检索方式不同，char速度快，但浪费空间用于固定字段如：sex;
  varchar用于变化较大的字段，如文章标题；
  char(4)  varchar(4)
  "a" 4byte "a" 1byte
  "abcdefg" 4byte "abcdefg" 5byte
  text 文本数据（文章） 2的16次方-1
    mediumtext
    longtext
  blob 用于保存二进制数据（相片） 2的16次方-1
    mediumblob
    longblob
  enum 枚举 1---2byte  65535 一次只能有一个值
   enum(a,b,c,d,g)
  set 集合 1 2 3 4 8 BYTE  64 一次可以用多个集合中的值，中间使用“，”隔开；
 3.日期型
  DATE  YYYY-MM-DD
  TIME  hh:mm:ss
  DATETIME YYYY-MM-DD hh:mm:ss
  TIMESIAMP YYYYMMDDhhmmss
  YEAR  YYYY
 NULL
创建索引
 主键索引
  主要作用是确定数据库表里一条特定数据记录的位置
  最好为每一张数据表定义一个主键
  一个表只能指定一个主键
  主键的值不能为空
 唯一索引
  都是可以用防止创建重复的值
  每个表都可以有多个唯一索引
  unique
  主键是标示一个记录不能重复，唯一是标示一个字段的; 
 常规索引
 最重要的技术
 提升数据库的性能
 索引顺序，
 可以提高查找 的速度，减慢删除、插入、修改
 和表一样是独立的数据对象
 可以单独使用，也可以在创建表时创建
 index和key是同义词
全文索引
 fulltext类型索引，myisom表类型使用，只有在varchar char text 文本字符串上使用
 也可以用多个数据列使用
