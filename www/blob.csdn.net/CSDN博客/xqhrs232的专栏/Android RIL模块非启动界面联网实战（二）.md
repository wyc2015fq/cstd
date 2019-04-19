# Android RIL模块非启动界面联网实战（二） - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:07:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1724
原文地址::[http://yangyangzhao.blog.163.com/blog/static/17581636620101163758306/](http://yangyangzhao.blog.163.com/blog/static/17581636620101163758306/)
在[Android RIL模块非启动界面联网实战（一）](http://yangyangzhao.blog.163.com/blog/static/175816366201011542451166/)中，最后发现只需要调用radiooptions这个程序就可以达到联网的目的了，一切似乎都很简单。
事实上如何呢？在android的shell下调用radiooptions，得到它的usage
bash-4.1# radiooptions 
Usage: radiooptions [option] [extra_socket_args] 
           0 - RADIO_RESET, 
           1 - RADIO_OFF, 
           2 - UNSOL_NETWORK_STATE_CHANGE, 
           3 - QXDM_ENABLE, 
           4 - QXDM_DISABLE, 
           5 - RADIO_ON, 
           6 apn- SETUP_PDP apn, 
           7 - DEACTIVE_PDP, 
           8 number - DIAL_CALL number, 
           9 - ANSWER_CALL, 
           10 - END_CALL 
: Unknown error: 0
然后首先调用一下radio on（其实我是在已经启动了界面的情况下实验的，不用特意调用），发现log如下
bash-4.1# radiooptions 5 
I/RILC    (  110): Debug port: Radio On 
D/RILC    (  110): C[locl]> RADIO_POWER 
bash-4.1# D/RILC    (  110): C[locl]< RADIO_POWER 
D/RILC    (  110): C[locl]> SET_NETWORK_SELECTION_AUTOMATIC 
D/RILJ    ( 1673): [UNSL]< UNSOL_RESPONSE_NETWORK_STATE_CHANGED 
D/RILJ    ( 1673): [0233]> OPERATOR 
D/RILC    (  110): C[locl]< SET_NETWORK_SELECTION_AUTOMATIC
看来libril在接受radio on的消息之后会尝试去寻找网络运营商，之后再去尝试连接setup data call，我用的是移动的卡，因此apn的名字是cmnet，于是我们尝试传入参数cmnet
bash-4.1# radiooptions 6 cmnet 
I/RILC    (  110): Debug port: Setup Data Call, Apn :cmnet 
D/RILC    (  110): C[locl]> SETUP_DATA_CALL 
bash-4.1# E/RILJ    (  190): Hit EOS reading message length 
I/RILJ    (  190): Disconnected from 'rild' socket 
D/RILB    (  190): Notifying: radio not available 
D/RILB    (  190): Notifying: radio off or not available 
D/GSM     (  190): Poll ServiceState done:  oldSS=[0 home 涓浗绉诲姩 (N/A) 46000  EDGE CSS not supported -1 -1RoamInd: -1DefRoamInd: -1] newSS=[1 home null null null  Unknown CSS not supported -1 -1RoamInd: -1DefRoamInd: -1] oldGprs=0 newGprs=0 oldType=EDGE newType=EDGE
D/RILJ    (  190): [0000]> GET_CURRENT_CALLS 
D/RILJ    (  190): [0000]< GET_CURRENT_CALLS error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE
D/GSM     (  190): GSMDataConnTrack handleMessage { what=12 when=547391 obj=android.os.AsyncResult@45ce59c8 }
D/GSM     (  190): [GsmDataConnectionTracker] Radio is off and clean up all connection
D/GSM     (  190): [GsmDataConnectionTracker] Clean up connection due to radioTurnedOff
D/GSM     (  190): [GsmDataConnectionTracker] setState: DISCONNECTING 
D/GSM     (  190): [PdpConnection] DataConnection.clearSettings() 
D/GSM     (  190): [DataConnection] Stop poll NetStat 
D/GSM     (  190): [GsmDataConnectionTracker] setState: IDLE 
D/GSM     (  190): [IccCard] Broadcasting intent ACTION_SIM_STATE_CHANGED NOT_READY reason null
D/RILJ    (  190): [0001]> REQUEST_GET_NEIGHBORING_CELL_IDS 
D/RILJ    (  190): [0001]< REQUEST_GET_NEIGHBORING_CELL_IDS error: com.android.internal.telephony.CommandException: RADIO_NOT_AVAILABLE
I/RILJ    (  190): Connected to 'rild' socket 
I/RILC    (  468): libril: new connection 
I/RILC    (  468): RIL Daemon version: HTC-RIL 1.6.1053G (Feb  2 2010,21:48:07) 
D/RILJ    (  190): [UNSL]< UNSOL_RESPONSE_RADIO_STATE_CHANGED RADIO_UNAVAILABLE
在打出来上面的log之后，手机信号没有了。。。。。。。。。。。 –_–|||  重启手机后终于能搜到信号了。
看来直接使用radiooptions来setup data call是不行的，那我们看看界面上连接gprs是怎么做的把，我打开手机网络的选项，发现它是这么连接网络的
D/RILJ    (  190): [0079]> SETUP_DATA_CALL 1 0 cmnet none none 3 
D/GSM     (  190): [GsmDataConnectionTracker] setState: INITING 
D/RILJ    (  190): [UNSL]< UNSOL_DATA_CALL_LIST_CHANGED [DataCallState: { cid: 1, active: 2, type: IP, apn: cmnet, address: 10.129.168.228 }, DataCallState: { cid: -1, active: 0, type: , apn: , address:  }, DataCallState: { cid: -1, active: 0, type: , apn:
 , address:  }] 
D/GSM     (  190): GSMDataConnTrack handleMessage { what=6 when=466064 obj=android.os.AsyncResult@45c701b8 }
D/RILJ    (  190): [0079]< SETUP_DATA_CALL {1, rmnet0, 10.129.168.228} 
看看和我用radiooptions连接网络的时候，差别在哪里？参数多了很多，SETUP_DATA_CALL 1 0 cmnet none none 3，这里有6个参数，而radiooptions只有一个参数，当然就不行了。于是去看libril和refrence-ril的代码
- 
staticvoid requestSetupDataCall(void*data, size_t datalen, RIL_Token t) //refrence-ril
- 
{
- 
constchar*apn;
- 
char*cmd;
- 
int err;
- 
    ATResponse *p_response = NULL;
- 
char*response[2]={"1", PPP_TTY_PATH };
- 
- 
    apn =((constchar**)data)[2];
- 
- 
staticvoid debugCallback (int fd,short flags,void*param){ //libril
- 
int acceptFD, option;
- 
struct sockaddr_un peeraddr;
- 
    socklen_t socklen =sizeof(peeraddr);
- 
int data;
- 
unsignedint qxdm_data[6];
- 
constchar*deactData[1]={"1"};
- 
char*actData[1];
- 
    RIL_Dial dialData;
- 
int hangupData[1]={1};
- 
int number;
- 
char**args;
- 
- 
//blabla 
- 
case6:
- 
            LOGI("Debug port: Setup Data Call, Apn :%s\n", args[1]);
- 
            actData[0]= args[1];
- 
            issueLocalRequest(RIL_REQUEST_SETUP_DATA_CALL,&actData,
- 
sizeof(actData));
- 
break;
- 
注意到libril中传且只传一个参数，而reference-ril中的apn参数是从第二个取出来的，这样强制转换的话reference-ril就肯定会crash，难怪我那样调用之后手机就没有信号了。。。
查出错误之后改起来就容易多了，首先要将libril里面传给reference-ril的参数更改，这里我直接改成了
- 
            issueLocalRequest(RIL_REQUEST_SETUP_DATA_CALL,/*&actData,
- 
  sizeof(actData));*/
- 
&args[1],6*sizeof(args[1]));
- 
然后，将radiooptions里面setup data call的参数限制解除。
于是，这样就彻底解决了这个问题。不知道这算不是ril里面的一个bug呢？
