# Android RIL模块非启动界面联网实战（一） - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:05:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：968
原文地址::[http://yangyangzhao.blog.163.com/blog/static/175816366201011542451166/](http://yangyangzhao.blog.163.com/blog/static/175816366201011542451166/)
在上一篇博文中我讲了如何在Android中不启动界面连接wifi，在这篇博文中我将讲如何不启动界面连接手机网络。
Android的RIL（Radio Interface Layer）模块提供Android telephony服务和radio硬件之间的一个抽象层。RIL模块的架构如下图所示
![Android RIL模块非启动界面联网实战（一） - angelbear - angelbear的博客](http://www.netmite.com/android/mydroid/development/pdk/docs/telephony.gif)
最上面的是Android的应用程序，比如拨号、短信息等程序，这些程序调用Application Framework层的telephony API，即可以完成打电话、发短信等操作。
以上都是虚拟机以上层面的，rild和vender ril是linux层的用户程序（库），rild是一个daemon程序，在init.rc中可以看到rild程序的启动
service ril-daemon /system/bin/rild 
    socket rild stream 660 root radio 
    socket rild-debug stream 660 radio system 
    user root 
    group radio cache inet misc audio 
同时注意到rild在启动的时候，还开启了两个以名称做标识的socket，rild和rild-debug，rild socket就是上层的framework和rild程序通信的手段，而rild-debug socket是留给radiooptions等程序调试使用的，后来这个rild-debug的socket起到了很重要的作用。
这次我没有从java端的代码入手，而是直接先开始看rild的代码。rild的代码只有rild.c一个，本身并不长，下面是rild的main函数
- 
int main(int argc,char**argv)
- 
{
- 
constchar* rilLibPath = NULL;
- 
char**rilArgv;
- 
void*dlHandle;
- 
const RIL_RadioFunctions *(*rilInit)(conststruct RIL_Env *,int,char**);
- 
const RIL_RadioFunctions *funcs;
- 
char libPath[PROPERTY_VALUE_MAX];
- 
unsignedchar hasLibArgs =0;
- 
- 
int i;
- 
- 
for(i =1; i < argc ;){
- 
if(0== strcmp(argv[i],"-l")&&(argc - i >1)){
- 
            rilLibPath = argv[i +1];
- 
            i +=2;
- 
}elseif(0== strcmp(argv[i],"--")){
- 
            i++;
- 
            hasLibArgs =1;
- 
break;
- 
}else{
- 
            usage(argv[0]);
- 
}
- 
}
- 
- 
if(rilLibPath == NULL){
- 
if(0== property_get(LIB_PATH_PROPERTY, libPath, NULL)){
- 
// No lib sepcified on the command line, and nothing set in props.
- 
// Assume "no-ril" case.
- 
goto done;
- 
}else{
- 
            rilLibPath = libPath;
- 
}
- 
}
- 
- 
/* special override when in the emulator */
- 
#if 1
- 
{
- 
staticchar*  arg_overrides[3];
- 
staticchar   arg_device[32];
- 
int           done =0;
- 
- 
#define  REFERENCE_RIL_PATH  "/system/lib/libreference-ril.so"
- 
- 
/* first, read /proc/cmdline into memory */
- 
char          buffer[1024],*p,*q;
- 
int           len;
- 
int           fd = open("/proc/cmdline",O_RDONLY);
- 
- 
if(fd <0){
- 
            LOGD("could not open /proc/cmdline:%s", strerror(errno));
- 
goto OpenLib;
- 
}
- 
- 
do{
- 
            len = read(fd,buffer,sizeof(buffer));}
- 
while(len ==-1&& errno == EINTR);
- 
- 
if(len <0){
- 
            LOGD("could not read /proc/cmdline:%s", strerror(errno));
- 
            close(fd);
- 
goto OpenLib;
- 
}
- 
        close(fd);
- 
- 
if(strstr(buffer,"android.qemud=")!= NULL)
- 
{
- 
/* the qemud daemon is launched after rild, so
- 
  * give it some time to create its GSM socket
- 
  */
- 
int  tries =5;
- 
#define  QEMUD_SOCKET_NAME    "qemud"
- 
while(1){
- 
int  fd;
- 
- 
                sleep(1);
- 
- 
                fd = socket_local_client(
- 
                            QEMUD_SOCKET_NAME,
- 
                            ANDROID_SOCKET_NAMESPACE_RESERVED,
- 
                            SOCK_STREAM );
- 
- 
if(fd >=0){
- 
                    close(fd);
- 
                    snprintf( arg_device,sizeof(arg_device),"%s/%s",
- 
                                ANDROID_SOCKET_DIR, QEMUD_SOCKET_NAME );
- 
- 
                    arg_overrides[1]="-s";
- 
                    arg_overrides[2]= arg_device;
- 
                    done =1;
- 
break;
- 
}
- 
                LOGD("could not connect to %s socket: %s",
- 
                    QEMUD_SOCKET_NAME, strerror(errno));
- 
if(--tries ==0)
- 
break;
- 
}
- 
if(!done){
- 
                LOGE("could not connect to %s socket (giving up): %s",
- 
                    QEMUD_SOCKET_NAME, strerror(errno));
- 
while(1)
- 
                    sleep(0x00ffffff);
- 
}
- 
}
- 
- 
/* otherwise, try to see if we passed a device name from the kernel */
- 
if(!done)do{
- 
#define  KERNEL_OPTION  "android.ril="
- 
#define  DEV_PREFIX     "/dev/"
- 
- 
            p = strstr( buffer, KERNEL_OPTION );
- 
if(p == NULL)
- 
break;
- 
- 
            p +=sizeof(KERNEL_OPTION)-1;
- 
            q  = strpbrk( p," \t\n\r");
- 
if(q != NULL)
- 
*q =0;
- 
- 
            snprintf( arg_device,sizeof(arg_device), DEV_PREFIX "%s", p );
- 
            arg_device[sizeof(arg_device)-1]=0;
- 
            arg_overrides[1]="-d";
- 
            arg_overrides[2]= arg_device;
- 
            done =1;
- 
- 
}while(0);
- 
- 
if(done){
- 
            argv = arg_overrides;
- 
            argc =3;
- 
            i    =1;
- 
            hasLibArgs =1;
- 
            rilLibPath = REFERENCE_RIL_PATH;
- 
- 
            LOGD("overriding with %s %s", arg_overrides[1], arg_overrides[2]);
- 
}
- 
}
- 
OpenLib:
- 
#endif
- 
    switchUser();
- 
- 
    dlHandle = dlopen(rilLibPath, RTLD_NOW);
- 
- 
if(dlHandle == NULL){
- 
        fprintf(stderr,"dlopen failed: %s\n", dlerror());
- 
        exit(-1);
- 
- 
    RIL_startEventLoop();
- 
- 
    rilInit =(const RIL_RadioFunctions *(*)(conststruct RIL_Env *,int,char**))dlsym(dlHandle,"RIL_Init");
- 
- 
if(rilInit == NULL){
- 
        fprintf(stderr,"RIL_Init not defined or exported in %s\n", rilLibPath);
- 
        exit(-1);
- 
}
- 
- 
if(hasLibArgs){
- 
        rilArgv = argv + i -1;
- 
        argc = argc -i +1;
- 
}else{
- 
staticchar* newArgv[MAX_LIB_ARGS];
- 
staticchar args[PROPERTY_VALUE_MAX];
- 
        rilArgv = newArgv;
- 
        property_get(LIB_ARGS_PROPERTY, args,"");
- 
        argc = make_argv(args, rilArgv);
- 
}
- 
- 
// Make sure there's a reasonable argv[0]
- 
    rilArgv[0]= argv[0];
- 
- 
    funcs = rilInit(&s_rilEnv, argc, rilArgv);
- 
- 
    RIL_register(funcs);
- 
- 
done:
- 
- 
while(1){
- 
// sleep(UINT32_MAX) seems to return immediately on bionic
- 
        sleep(0x00ffffff);
- 
}
- 
}
RIL_开头的结构和函数都是libril中的，我们先不管这些，从上面的main函数看来，rild并不干活，它只是把ril-lib-path的so文件载入，然后调用一下其中的RIL_Init函数，传入一下参数，然后再将RIL_Init得到的结果调用libril中的RIL_register，就进入死循环了。
我们在去看一下RIL_Init和RIL_register都是干什么的呢？
- 
/*include/telephony/ril.h*/
- 
typedefstruct{
- 
int version;/* set to RIL_VERSION */
- 
    RIL_RequestFunc onRequest;
- 
    RIL_RadioStateRequest onStateRequest;
- 
    RIL_Supports supports;
- 
    RIL_Cancel onCancel;
- 
    RIL_GetVersion getVersion;
- 
} RIL_RadioFunctions;
- 
- 
/*reference-ril/reference-ril.c*/
- 
staticvoid onRequest (int request,void*data, size_t datalen, RIL_Token t);
- 
static RIL_RadioState currentState();
- 
staticint onSupports (int requestCode);
- 
staticvoid onCancel (RIL_Token t);
- 
staticconstchar*getVersion();
- 
- 
/*** Static Variables ***/
- 
staticconst RIL_RadioFunctions s_callbacks ={
- 
    RIL_VERSION,
- 
    onRequest,
- 
    currentState,
- 
    onSupports,
- 
    onCancel,
- 
    getVersion
- 
};
- 
- 
const RIL_RadioFunctions *RIL_Init(conststruct RIL_Env *env,int argc,char**argv)
- 
{
- 
//blabla...
- 
return&s_callbacks;
- 
}
- 
- 
/*libril/ril.cpp*/
- 
extern"C"void
- 
RIL_register (const RIL_RadioFunctions *callbacks){
- 
int ret;
- 
int flags;
- 
- 
if(callbacks == NULL ||((callbacks->version != RIL_VERSION)
- 
&&(callbacks->version !=2))){// Remove when partners upgrade to version 3
- 
        LOGE(
- 
"RIL_register: RIL_RadioFunctions * null or invalid version"
- 
" (expected %d)", RIL_VERSION);
- 
return;
- 
}
- 
if(callbacks->version <3){
- 
        LOGE ("RIL_register: upgrade RIL to version 3 current version=%d", callbacks->version);
- 
}
- 
- 
if(s_registerCalled >0){
- 
        LOGE("RIL_register has been called more than once. "
- 
"Subsequent call ignored");
- 
return;
- 
}
- 
- 
    memcpy(&s_callbacks, callbacks,sizeof(RIL_RadioFunctions));
- 
- 
    s_registerCalled =1;
- 
- 
// Little self-check
- 
- 
for(int i =0; i <(int)NUM_ELEMS(s_commands); i++){
- 
        assert(i == s_commands[i].requestNumber);
- 
}
- 
- 
for(int i =0; i <(int)NUM_ELEMS(s_unsolResponses); i++){
- 
        assert(i + RIL_UNSOL_RESPONSE_BASE
- 
== s_unsolResponses[i].requestNumber);
- 
}
- 
- 
// New rild impl calls RIL_startEventLoop() first
- 
// old standalone impl wants it here.
- 
- 
if(s_started ==0){
- 
        RIL_startEventLoop();
- 
}
- 
- 
// start listen socket
- 
- 
#if 0
- 
    ret = socket_local_server (SOCKET_NAME_RIL,
- 
            ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
- 
- 
if(ret <0){
- 
        LOGE("Unable to bind socket errno:%d", errno);
- 
        exit (-1);
- 
}
- 
    s_fdListen = ret;
- 
- 
#else
- 
    s_fdListen = android_get_control_socket(SOCKET_NAME_RIL);
- 
if(s_fdListen <0){
- 
        LOGE("Failed to get socket '" SOCKET_NAME_RIL "'");
- 
        exit(-1);
- 
}
- 
- 
    ret = listen(s_fdListen,4);
- 
- 
if(ret <0){
- 
        LOGE("Failed to listen on control socket '%d': %s",
- 
             s_fdListen, strerror(errno));
- 
        exit(-1);
- 
}
- 
#endif
- 
- 
- 
/* note: non-persistent so we can accept only one connection at a time */
- 
    ril_event_set (&s_listen_event, s_fdListen,false,
- 
                listenCallback, NULL);
- 
- 
    rilEventAddWakeup (&s_listen_event);
- 
- 
#if 1
- 
// start debug interface socket
- 
- 
    s_fdDebug = android_get_control_socket(SOCKET_NAME_RIL_DEBUG);
- 
if(s_fdDebug <0){
- 
        LOGE("Failed to get socket '" SOCKET_NAME_RIL_DEBUG "' errno:%d", errno);
- 
        exit(-1);
- 
}
- 
- 
    ret = listen(s_fdDebug,4);
- 
- 
if(ret <0){
- 
        LOGE("Failed to listen on ril debug socket '%d': %s",
- 
             s_fdDebug, strerror(errno));
- 
        exit(-1);
- 
}
- 
- 
    ril_event_set (&s_debug_event, s_fdDebug,true,
- 
                debugCallback, NULL);
- 
- 
    rilEventAddWakeup (&s_debug_event);
- 
#endif
- 
- 
}
看到上面三个文件的代码片段，相信大家有个大概的认识了把，vendor ril的RIL_Init函数返回的是真正干活的函数数组的指针，而libril中的RIL_register将这些函数指针存着供自己用，至于什么时候用？在RIL_register中可以看到其尝试获得rild以及rild-debug两个socket的控制socket，于是便可以得知，libril是监听这些socket的信息，然后调用传入的函数指针来处理这些信息，在ril.cpp中随处可见s_callbacks.onRequest的调用，s_callbacks就是callbacks的拷贝。
在ril.cpp的main函数的109行，我们看见这里注册了一个rild socket的listener，是函数listenCallback，这个函数就用来处理向rild发送的命令，
- 
staticvoid listenCallback (int fd,short flags,void*param){
- 
int ret;
- 
int err;
- 
int is_phone_socket;
- 
    RecordStream *p_rs;
- 
- 
struct sockaddr_un peeraddr;
- 
    socklen_t socklen =sizeof(peeraddr);
- 
- 
struct ucred creds;
- 
    socklen_t szCreds =sizeof(creds);
- 
- 
struct passwd *pwd = NULL;
- 
- 
    assert (s_fdCommand <0);
- 
    assert (fd == s_fdListen);
- 
- 
    s_fdCommand = accept(s_fdListen,(sockaddr *)&peeraddr,&socklen);
- 
- 
if(s_fdCommand <0){
- 
        LOGE("Error on accept() errno:%d", errno);
- 
/* start listening for new connections again */
- 
        rilEventAddWakeup(&s_listen_event);
- 
return;
- 
}
- 
- 
/* check the credential of the other side and only accept socket from
- 
  * phone process
- 
  */
- 
    errno =0;
- 
    is_phone_socket =0;
- 
- 
    err = getsockopt(s_fdCommand, SOL_SOCKET, SO_PEERCRED,&creds,&szCreds);
- 
- 
if(err ==0&& szCreds >0){
- 
        errno =0;
- 
        pwd = getpwuid(creds.uid);
- 
if(pwd != NULL){
- 
if(strcmp(pwd->pw_name, PHONE_PROCESS)==0){
- 
                is_phone_socket =1;
- 
}else{
- 
                LOGE("RILD can't accept socket from process %s", pwd->pw_name);
- 
}
- 
}else{
- 
            LOGE("Error on getpwuid() errno: %d", errno);
- 
}
- 
}else{
- 
        LOGD("Error on getsockopt() errno: %d", errno);
- 
}
- 
- 
if(!is_phone_socket ){
- 
      LOGE("RILD must accept socket from %s", PHONE_PROCESS);
- 
- 
      close(s_fdCommand);
- 
      s_fdCommand =-1;
- 
- 
      onCommandsSocketClosed();
- 
- 
/* start listening for new connections again */
- 
      rilEventAddWakeup(&s_listen_event);
- 
- 
return;
- 
}
- 
- 
    ret = fcntl(s_fdCommand, F_SETFL, O_NONBLOCK);
- 
- 
if(ret <0){
- 
        LOGE ("Error setting O_NONBLOCK errno:%d", errno);
- 
}
- 
- 
    LOGI("libril: new connection");
- 
- 
    p_rs = record_stream_new(s_fdCommand, MAX_COMMAND_BYTES);
- 
- 
    ril_event_set (&s_commands_event, s_fdCommand,1,
- 
        processCommandsCallback, p_rs);
- 
- 
    rilEventAddWakeup (&s_commands_event);
- 
- 
    onNewCommandConnect();
- 
}
- 
但是仔细看这个这个函数的35行到48行，发现rild的socket并不是谁都可以连的，必须是PHONE_PROCESS才可以连接，也就是说如果是其他process通过rild socket发命令libril是不会处理，直接报错的，其实这里应该可以直接注释掉，但是为了尽量不破坏原来的代码，我就没有走这条路。向下稍微看几行就发现了debugCallback函数，在RIL_register中通过ril_event_set (&s_debug_event,
 s_fdDebug, true,  debugCallback, NULL);将debug socket的处理函数设定成debugCallback，观察debugCallback中的几个处理函数，发下下面几个可以用来联网的函数。
- 
case5:
- 
            LOGI("Debug port: Radio On");
- 
            data =1;
- 
            issueLocalRequest(RIL_REQUEST_RADIO_POWER,&data,sizeof(int));
- 
            sleep(2);
- 
// Set network selection automatic.
- 
            issueLocalRequest(RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC, NULL,0);
- 
break;
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
case7:
- 
            LOGI("Debug port: Deactivate Data Call");
- 
            issueLocalRequest(RIL_REQUEST_DEACTIVATE_DATA_CALL,&deactData,
- 
sizeof(deactData));
- 
break;
- 
Radio on是将radio模块激活，Setup Data Call就是建立数据连接（gprs，td，wcdma之类的），有这两个功能的话就可以保证连上网了。
其实看到debugCallback的处理函数，我感到很眼熟，为什么呢？因为在rild的文件夹中有一个radiooptions.c的程序，这个程序的用法是这样的
- 
staticvoid print_usage(){
- 
    perror("Usage: radiooptions [option] [extra_socket_args]\n\
- 
           0 - RADIO_RESET, \n\
- 
           1 - RADIO_OFF, \n\
- 
           2 - UNSOL_NETWORK_STATE_CHANGE, \n\
- 
           3 - QXDM_ENABLE, \n\
- 
           4 - QXDM_DISABLE, \n\
- 
           5 - RADIO_ON, \n\
- 
           6 apn- SETUP_PDP apn, \n\
- 
           7 - DEACTIVE_PDP, \n\
- 
           8 number - DIAL_CALL number, \n\
- 
           9 - ANSWER_CALL, \n\
- 
           10 - END_CALL \n");
- 
}
- 
- 
这样就正好对应上了，radiooptions程序就是官方用来调试ril模块的一个小程序，debugCallback就是在libril中给这个小程序开的“后门”。
于是事情就好办了，前面的一切工作似乎都是白费的，只要调用几下radiooptions不久好了么？
