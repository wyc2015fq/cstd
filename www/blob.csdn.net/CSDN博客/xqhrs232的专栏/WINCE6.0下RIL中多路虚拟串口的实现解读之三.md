# WINCE6.0下RIL中多路虚拟串口的实现解读之三 - xqhrs232的专栏 - CSDN博客
2010年02月04日 11:45:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1709
存在的几个疑问
1。Mux07_10.dll++++rilgsm.dll这两个驱动程序加载的先后次序
    rilgsm.dll要用到Mux07_10.dll虚拟出来的两路串口中的一个，那么Mux07_10.dll应该在rilgsm.dll之前被加载，但看注册表项没看出这个先后次序。
2。RIL Proxy是RIL的一个代理，所有高层的API都是跟这个打交道的。那TAPI/SMS/SIM这些高层的API是怎么跟RIL Proxy这个代理层打交道的？
    TAPI.DLL++++SMSDRV.DLL++++SIMDRV.DLL这些驱动的流程是怎么样的,还没细看。
3。RIL Proxy把来自高层的API命令一个个转换成RIL---XXX( )命令，每个RIL---XXX( )命令对应RIL_IOControl函数里面的一个分支
举例如下----RIL_Dial()命令对应RILDrv_Dial， RILDrv----XXX( )系列命令是真正的最底层实现功能的函数。
      // RIL_Dial()
            case IOCTL_RIL_DIAL:
                if ( BufInDuplicate.ptr() && sizeof(DIAL_PARAMS) == BufInDuplicate.size())
                {
                    LPDIAL_PARAMS pDP = (LPDIAL_PARAMS)BufInDuplicate.ptr();
                    pDP->szAddress[ARRAY_LENGTH(pDP->szAddress)-1] = 0;
                }
                CALLDRVAPI_PARAMS(DIAL_PARAMS, RILDrv_Dial(dwOpenData, pParams->szAddress, pParams->dwType, /
                                                                        pParams->dwOptions));
                break;
RIL---XXX( )命令是RIL Proxy代理层对底层RIL的一个简单封装，不建议直接所有RIL Proxy代理层命令去调RIL的底层命令。一般使用TAPI/SMS/SIM这些高层的API。
当然简单的对RIL进行测试也是可以直接去调RIL---XXX( )命令的。RIL Proxy代理层被引用的次数是没有限制的，但要用到一个ril.lib文件，但WINCE60下的ril.lib文件不能直接使用。
直接在APP里面去调RIL---XXX( )命令相当于自己的APP空间里面有了一个RIL Proxy代理层的实体存在。
RIL层只有一个，因为底层的串口只有一路，RIL Proxy代理层应该有无数个，TAPI/SMS/SIM这些高层的API的驱动程序里面都应该有一个RIL Proxy代理层的实体存在，但还没看出这个玄机所在。
//------------------下面的OPEN代码应该可以说明这样的问题，每次都pHandle = new CRilInstanceHandle这样的对象，并把其加到列表中pDevice->AddToList(pHandle)，函数返回pHandle对象
//
// Open a driver instance
//
#ifdef __cplusplus
extern "C" DWORD RIL_Open(DWORD dwData, DWORD dwAccess, DWORD dwShareMode)
#else
DWORD RIL_Open(DWORD dwData, DWORD dwAccess, DWORD dwShareMode)
#endif
{
    FUNCTION_TRACE(RIL_Open);
    DEBUGCHK(dwData != 0);
    CRilInstanceHandle* pHandle = NULL;
    CRilHandle* pDevice = RILHANDLE_FROM_DRIVERCOOKIE(dwData);
#ifdef RIL_ALLOW_DRIVER_REMOVAL_TESTING
    if (!g_pDeviceAllowDriverRemovalTesting)
    {
        return 0x4321;
    }
#endif
    DWORD dwRet = 0;
    // Wait until the COM port is initialized
    if (!pDevice /* || !pDevice->WaitForInit(MAX_DRIVERINIT_TIME) */ )
    {
        goto Error;
    }
    pHandle = new CRilInstanceHandle;
    if (!pHandle || !pHandle->Init(pDevice))
    {
        goto Error;
    }
    // Insert new handle into the handle list
    if (!pDevice->AddToList(pHandle))
    {
        goto Error;
    }
    dwRet = (DWORD)pHandle;
    Error:
    if (!dwRet)
    {
        delete pHandle;
    }
    return dwRet;
}
4。RIL Proxy Log用于调试的日志文件产生，但不知道怎么使用，关于RIL的调试手段还不清楚。
 5。Mux07_10.dll是怎么形成的，看其SOURCES文件只是形成一个LIB文件啊！
   Mux07_10.dll是一个虚拟串口的驱动能不能直接用到一般的场合-----也就是简单的把一个物理串口虚拟成2-3个虚拟串口,不去跟RIL挂接。
   还是必须用到GSM07.10这样的协议才能实现虚拟串口的功能.
  setup710.txt这个文件里面的说明主要是起什么作用的？还真没看太明白！！！
