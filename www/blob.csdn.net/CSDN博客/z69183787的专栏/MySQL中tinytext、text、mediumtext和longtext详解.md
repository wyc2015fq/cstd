# MySQL中tinytext、text、mediumtext和longtext详解 - z69183787的专栏 - CSDN博客
2018年07月27日 15:15:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：766
[https://www.cnblogs.com/pureEve/p/6015000.html](https://www.cnblogs.com/pureEve/p/6015000.html)
一、数字类型
|类型|范围|说明||
|----|----|----|----|
|Char(N) [binary]|N=1~255 个字元			binary ：分辨大小写|固定长度|std_name cahr(32) not null|
|VarChar(N) [binary]|N=1~255 个字元			binary ：分辨大小写|可变长度|std_address varchar(256)|
|TinyBlob|最大长度255个字元(2^8-1)|Blob (Binary large objects)储存二进位资料，且有分大小写|memo text not null|
|TinyText|最大长度255个字元(2^8-1)| | |
|Blob|最大长度65535个字元(2^16-1)| | |
|Text|最大长度65535个字元(2^16-1)| | |
|MediumBlob|最大长度 16777215 个字元(2^24-1)| | |
|MediumText|最大长度 16777215 个字元(2^24-1)| | |
|LongBlob|最大长度4294967295个字元 (2^32-1)| | |
|LongText|最大长度4294967295个字元 (2^32-1)| | |
|Enum|集合最大数目为65535|列举(Enumeration)，Enum单选、Set复选|sex enum(1,0)			habby set(‘玩电玩’,'睡觉’,'看电影’,'听音乐’)|
|Set|集合最大数目为64| | |
辨別Null与Not Null ：Null为允许储存空值(Null)
二、数值
|类型|范围|说明|例如|
|----|----|----|----|
|TinyInt[M] [UNSIGNED]|-128~127			UNSIGNED ： 0~255||num tinyint unsigned|
|SmallInt[M] [UNSIGNED]|-32768~32767			UNSIGNED ：0~ 65535|||
|MediumInt[M] [UNSIGNED]|-8388608~8388607			UNSIGNED ：0~16777215|||
|Int[M] [UNSIGNED]|-2^31~2^31-1			UNSIGNED ： 0~2^32|||
|BigInt[M] [UNSIGNED]|-2^63~2^63-1			UNSIGNED ： 0~2^64|||
|Float [(M,D)]|-3.4E+38~3.4E+38( 约 )|注： M 为长度， D 为小数,Float 4 bytes,Double 8 bytes||
|Double [(M,D)]|-1.79E+308~1.79E+308( 约 )| | |
|Decimal [(M,D)]|| | |
辨別ZeroFill：当宣告关键字ZeroFill为自动填满0，如 000021
三、日期时间
|类型|范围|说明|
|----|----|----|
|Date|日期(yyyy-mm-dd)||
|Time|时间(hh:mm:ss)||
|DateTime|日期与时间組合(yyyy-mm-dd hh:mm:ss)||
|TimeStamp|yyyymmddhhmmss||
|Year|年份yyyy||
