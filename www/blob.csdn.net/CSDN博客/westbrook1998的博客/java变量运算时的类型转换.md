# java变量运算时的类型转换 - westbrook1998的博客 - CSDN博客





2018年03月21日 22:51:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42








![这里写图片描述](http://uploadfiles.nowcoder.net/images/20150917/415611_1442458661106_F4A62FDD254F710F39378C754ED65E61)
- 不同类型先转换成同一类型再计算
- 转换按长度**从小转到大**的方向进行，保证精度不丢失，如int * long，先把int转换成long
- **所有浮点数**运算都要以双精度类型计算，即时只含有float类型
- **char型和short型**参与运算时，必须先转换成int型
- 赋值时，如果等号右边的类型长度比左边长，自动转换后会丢失精度
- 补充：java浮点数默认为double，如果要定义float型浮点数要在后面添加f



