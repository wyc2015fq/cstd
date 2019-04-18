# EasyDarwin实现说明之添加模块 - qq_42302962的博客 - CSDN博客
2018年05月29日 11:13:27[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：311
在Darwin的架构中一个很重要的概念就是模块（Module），我们在原生Darwin系统上加入了两个模块（Module），是MyBCInteractModule和MyFileUploadModule，每个模块都要提供两个函数，Main和dispatch函数，服务器启动时（QTSServer）会调用Main函数完成对该模块的初始化；Dispatch是注册给服务器的一个回调函数，服务器通过该函数分发该模块订阅的各种角色，这些角色定义如下：
```
/********************************************************************/
// QTSS API ROLES
//
// Each role represents a unique situation in which a module may be
// invoked. Modules must specify which roles they want to be invoked for. 
enum
{
    //Global
    QTSS_Register_Role =             FOUR_CHARS_TO_INT('r', 'e', 'g', ' '), //reg  //All modules get this once at startup
    QTSS_Initialize_Role =           FOUR_CHARS_TO_INT('i', 'n', 'i', 't'), //init //Gets called once, later on in the startup process
    QTSS_Shutdown_Role =             FOUR_CHARS_TO_INT('s', 'h', 'u', 't'), //shut //Gets called once at shutdown
    
    QTSS_ErrorLog_Role =             FOUR_CHARS_TO_INT('e', 'l', 'o', 'g'), //elog //This gets called when the server wants to log an error.
    QTSS_RereadPrefs_Role =          FOUR_CHARS_TO_INT('p', 'r', 'e', 'f'), //pref //This gets called when the server rereads preferences.
    QTSS_StateChange_Role =          FOUR_CHARS_TO_INT('s', 't', 'a', 't'), //stat //This gets called whenever the server changes state.
    
    QTSS_Interval_Role =             FOUR_CHARS_TO_INT('t', 'i', 'm', 'r'), //timr //This gets called whenever the module's interval timer times out calls.
    
    //RTSP-specific
    QTSS_RTSPFilter_Role =           FOUR_CHARS_TO_INT('f', 'i', 'l', 't'), //filt //Filter all RTSP requests before the server parses them
    QTSS_RTSPRoute_Role =            FOUR_CHARS_TO_INT('r', 'o', 'u', 't'), //rout //Route all RTSP requests to the correct root folder.
    QTSS_RTSPAuthenticate_Role =     FOUR_CHARS_TO_INT('a', 't', 'h', 'n'), //athn //Authenticate the RTSP request username.
    QTSS_RTSPAuthorize_Role =        FOUR_CHARS_TO_INT('a', 'u', 't', 'h'), //auth //Authorize RTSP requests to proceed
    QTSS_RTSPPreProcessor_Role =     FOUR_CHARS_TO_INT('p', 'r', 'e', 'p'), //prep //Pre-process all RTSP requests before the server responds.
                                        //Modules may opt to "steal" the request and return a client response.
    QTSS_RTSPRequest_Role =          FOUR_CHARS_TO_INT('r', 'e', 'q', 'u'), //requ //Process an RTSP request & send client response
    QTSS_RTSPPostProcessor_Role =    FOUR_CHARS_TO_INT('p', 'o', 's', 't'), //post //Post-process all RTSP requests
    QTSS_RTSPSessionClosing_Role =   FOUR_CHARS_TO_INT('s', 'e', 's', 'c'), //sesc //RTSP session is going away
    QTSS_RTSPIncomingData_Role =     FOUR_CHARS_TO_INT('i', 'c', 'm', 'd'), //icmd //Incoming interleaved RTP data on this RTSP connection
    //RTP-specific
    QTSS_RTPSendPackets_Role =          FOUR_CHARS_TO_INT('s', 'e', 'n', 'd'), //send //Send RTP packets to the client
    QTSS_ClientSessionClosing_Role =    FOUR_CHARS_TO_INT('d', 'e', 's', 's'), //dess //Client session is going away
    
    //RTCP-specific
    QTSS_RTCPProcess_Role =             FOUR_CHARS_TO_INT('r', 't', 'c', 'p'), //rtcp //Process all RTCP packets sent to the server
    //File system roles
    QTSS_OpenFilePreProcess_Role =      FOUR_CHARS_TO_INT('o', 'p', 'p', 'r'),  //oppr
    QTSS_OpenFile_Role =                FOUR_CHARS_TO_INT('o', 'p', 'f', 'l'),  //opfl
    QTSS_AdviseFile_Role =              FOUR_CHARS_TO_INT('a', 'd', 'f', 'l'),  //adfl
    QTSS_ReadFile_Role =                FOUR_CHARS_TO_INT('r', 'd', 'f', 'l'),  //rdfl
    QTSS_CloseFile_Role =               FOUR_CHARS_TO_INT('c', 'l', 'f', 'l'),  //clfl
    QTSS_RequestEventFile_Role =        FOUR_CHARS_TO_INT('r', 'e', 'f', 'l'),  //refl
    //HLS Session
    Easy_HLSOpen_Role   =               FOUR_CHARS_TO_INT('h', 'l', 's', 'o'),  //hlso
    Easy_HLSClose_Role  =               FOUR_CHARS_TO_INT('h', 'l', 's', 'c'),  //hlsc
    
};
typedef UInt32 QTSS_Role;
```
每个模块的main可以如下：
```cpp
```cpp
QTSS_Error MyBCInteractModule_Main(void* inPrivateArgs)
{
    return _stublibrary_main(inPrivateArgs, MyBCInteractModuleDispatch);
}
```
```
其中MyBCInteractModuleDispatch是Dispatch函数，该函数负责对各种角色的处理，如：
```php
```php
QTSS_Error  MyBCInteractModuleDispatch(QTSS_Role inRole, QTSS_RoleParamPtr inParams)
{
    switch (inRole)
    {
        case QTSS_Register_Role:
            return Register(&inParams->regParams);
        break;
        
        case QTSS_Initialize_Role:
            return Initialize(&inParams->initParams);
        break;
        
        case QTSS_RereadPrefs_Role:
            return RereadPrefs();
        break;
        case QTSS_RTSPAuthenticate_Role:
                return AuthenticateRTSPRequest(&inParams->rtspAthnParams);
        break;
        
        case QTSS_RTSPAuthorize_Role:
                return AccessAuthorizeRTSPRequest(&inParams->rtspRequestParams);
        break;
            
        case QTSS_Shutdown_Role:
            return Shutdown();
        break;
    }
    
    return QTSS_NoErr;
}
```
```
那么，MyBCInteractModule和!MyFileUploadModule是在什么时候注册到Darwin系统中的呢？答案是QTSServer::LoadCompiledInModules，在调用!SetupModule时传人了模块的Main函数，SetupModule时会调用该main函数，也就获取到了该模块的dispatch函数，在AddModule时，调用dispatch函数分发QTSS_Register_Role角色，此时MyBCInteractModule和MyFileUploadModule会去添加自己感兴趣的角色，最后在AddModule里把MyBCInteractModule和MyFileUploadModule添加到队列里。到此，添加模块算是完成了。
```cpp
```cpp
void    QTSServer::LoadCompiledInModules()
{
#ifndef DSS_DYNAMIC_MODULES_ONLY
    //
    // The following modules are all compiled into the server. 
    
    QTSSModule* theFileModule = new QTSSModule("QTSSFileModule");
    (void)theFileModule->SetupModule(&sCallbacks, &QTSSFileModule_Main);
    (void)AddModule(theFileModule);
    //与业务服务器通信
    QTSSModule* theBCInteractModule = new QTSSModule("MyBCInteractModule");
    (void)theBCInteractModule->SetupModule(&sCallbacks, &MyBCInteractModule_Main);
    (void)AddModule(theBCInteractModule);
    
    //文件上传模块  
    QTSSModule* theFileUploadModule = new QTSSModule("MyFileUploadModule");
    (void)theFileUploadModule->SetupModule(&sCallbacks, &MyFileUploadModule_Main);
    (void)AddModule(theFileUploadModule);
    xxxxxx
}
```
```
技术交流可以加QQ【1915869449】
