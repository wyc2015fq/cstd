# SIM300模块AT命令参考 - xqhrs232的专栏 - CSDN博客
2013年01月26日 23:11:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1091
原文地址::[http://www.cnblogs.com/feisky/archive/2010/05/24/1743073.html](http://www.cnblogs.com/feisky/archive/2010/05/24/1743073.html)
**开机命令**
AT+CFUN=1,1          //此命令可以开启simcom模块的大部分功能，一般在初始化模块的时候都要写上；
AT&F                       //设置缺省制造商当前所有参数
**测试网络情况**
AT+CSQ         //+CSQ: 28,0         返回信号强度，10以上就可以
AT+CREG？         //+CREG: 0,1         测试网络注册情况，第二个参数为1或5就可以
AT+COPS？          //+COPS: 0,0,"CHINA MOBILE"     测试运营商
**电话**
AT+CHFA=1     //0/1此命令用来切换两个音频通道
AT+CLIP=1           //来电显示
ATH                      //断开现有连接/挂断电话
ATA                      //接听电话
AT+VTS="dt"        //开启dtmf音
AT+CRSL=<n>      //响铃音量大小，范围是0~100，数值越小，则音量越轻；
AT+CLVL=<n>      //通话音量大小，范围是0~100，数值越小，则音量越轻；
AT+CMUT            //静音控制
ATD 10086；         //向外拨打电话
ATDL                   //重拨上次拨打的电话号码
**短信**
AT+CMGF=<n>            //选择短信格式  n=0 PDU模式  1－文本格式。
AT+CNMI=2,1,0,0,0    //表示接收到信息保存到卡片上。
//接收到信息时提示：**+CMTI: "SM",3**，需要使用 **AT+CMGR=3**读。
AT+CNMI=2,2,0,0,0   //直接从模块读取短信息而不是利用模块从SIM //卡读取短信息
//接收到信息时提示：
**+CMT: "+8613333333333",,"10/05/24,21:47:27+32"**
**fefeifei sld**
**at+cnmi=2,2,0,0,0**
**OK**
**(从结果上来看，接收到的短信内容处于前两个回车之间)**
AT+CMGR=3               //读取第几条短信
**其他**
AT+CBAND=PGSM_MODE(900)/DCS_MODE(1800)/PCS_MODE(1900)/EGSM_DCS_MODE(900扩展)/GSM850_PCS_MODE(850) 出厂默认设置900/1800，自动切换。
ATE                             //设置回声模式
AT+ECHO                   //回音消除技术控制(ECHO CANCELLATION CONTROL)
**GPRS**
**命令模式下：**
AT+CIPMODE=0；                                  //必须是连接中断的时候进行设置
AT+CGDCONT=1,"IP","CMNET"              //Define PDP context, 定义PDP 上下文
OK
AT+CIPSTART="TCP","60.29.18.195","2020"
OK
CONNECT OK
AT+CIPSEND=5
> 12345
SEND OK
AT+CIPCLOSE                    //关闭连接
CLOSE OK
AT+CIPSHUT
SHUT OK
AT+CGDCONT=1,"IP","CMNET"
OK
AT+CIPSTART="TCP","60.29.18.195","2020"
OK
CONNECT OK
AT+CIPSEND=6
> 123456
SEND OK
**数据模式下**
AT+CIPMODE=1
OK
AT+CIPCCFG=3,2,256,1
OK
AT&D1
OK
AT+CIPSTART="TCP","60.29.18.195","2020"
OK
CONNECT
……..
……..
+++               //切换时前后必须要有0.5S时间间隔无数据，每个+之间不超过20ms间隔，否则当作发送的数据处理
OK
ATO
注意事项：
1、  数据量大的时候，比如传输图片等，采用透明传输模式，要进行流量控制的话，可以传输1024字节/包，保证可以有时间进行切换到命令模式。
2、  命令模式传输数据最大1024字节/每包，建议最大为300-400字节/包。如果网络阻塞出现，最好是断开重新建立连接。可以设置为20S无响应，即重新连接。
