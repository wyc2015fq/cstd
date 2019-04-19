# MySQL函数总结 - 零度的博客专栏 - CSDN博客
2016年09月29日 16:01:09[零度anngle](https://me.csdn.net/zmx729618)阅读数：306
### 一、数学函数
求指数函数power()
select fweight,power(fweight,-0.5),power(fweight,2),power(fweight,3),power(fweight,4) from t_person;
求平方根函数sqrt()
select fweight,sqrt(fweight) from t_person
求随机数函数rand()
select rand();
舍入到最大整数函数ceiling()
select fname,fweight,ceiling(fweight),ceiling(fweight*-1) from t_person;
舍入到最小整数函数floor()
select fname,fweight,floor(fweight) from t_person;
四舍五入函数round()
select fname,fweight,round(fweight,1),round(fweight*-1,0),round(fweight,1) from t_person;
其中第一个参数是要被操作的数，第二个参数是要舍入的位数。1是舍入小数点后最后一位，-1是小数点前一位 比如32.15 如果第二个参数是1 则为32.2 为-1则是30，如果round()只传一个参数的话就四舍五入到小数点。
正弦函数sin()余弦函数cos()正切函数tan()
反正弦函数asin(1/var)反余弦函数acos(1/var)反正切函数atan(var)
其中var 是变量 即要操作的数
select fname,fweight,sin(fweight),cos(fweight),tan(fweight) from t_person;
select fname,fweight,asin(1/fweight),acos(1/fweight),atan(fweight) from t_person;
两个变量的反正切atan2(var,var)余切cot()
求圆周率函数PI(),acos(-1)
弧度制转换为角度制degrees(var);
角度值转换为弧度制radians(var);
求符号sign()如果大于0返回1，如果小于0返回-1，等于0返回0
求整数余数mod(var,var)
求自然对数log(var) oracle中是ln();
求以10为底的对数log10(var);
求幂函数power(x,y)
### 二、字符串函数
计算长度length()
字符串转换为小写lower();
select fname,lower(fname) from t_person;
字符串转换为大写upper();
select fname,upper(fname) from t_person;
去掉字符串左边空格ltrim()
select fname,ltrim(fname),ltrim(' abc ') from t_person;
截去字符串右侧空格rtrim(string),去掉字符串两侧空格trim(string)
select fname,trim(fname),rtrim(' aaa ') from t_person;
select fname,ltrim(fname),ltrim(rtrim(' asd ')) from t_person;
取子字符串substring(string,start_position,length)
select substring('asdfghjll',2,3);
oracle中是substr（string,start_position,length）
计算子字符串的位置instr(string,substring)
[MySQL](http://lib.csdn.net/base/14)>
 select fname,instr(fname,'m'),instr(fname,'ly') from t_person;
+-----------+--------------------+----------------------+
| fname     | instr(fname,'m') | instr(fname,'ly')   |
+-----------+--------------------+----------------------+
| Tom       |                  3      |                          0
 |
| Jim        |                   3     |                          0
 |
| Lily        |                   0      |                          3
 |
| Kelly      |                   0     |                          4
 |
| Sam        |                   3       |           
