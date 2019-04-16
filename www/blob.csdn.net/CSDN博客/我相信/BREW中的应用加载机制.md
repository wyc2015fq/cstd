# BREW中的应用加载机制 - 我相信...... - CSDN博客





2011年03月18日 20:31:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1042标签：[brew																[module																[null																[applet																[数据结构																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)




在BREW中，module是基本的执行单位，在手机上体现为.mod文件，在仿真器中体现为.dll文件。一个module可以包含一个或多个applet，或一个或多个扩展类（关于扩展类有单独的章节进行阐述）。凡是在手机上与其他代码一同编译完成的叫做静态模块，凡是通过下载方式（无线下载或者数据线下载）存于文件系统中的叫动态模块。

每个module都需要有标识自身的MIF文件，从BREW 3.1开始即使是静态module也需要有相应的MIF。而在BREW3.1之前，对于静态module是没有单独的MIF文件的，需要通过AEEAppInfo的结构体来表示module的信息，里面主要包括clsid，应用类型，图标等信息，每个静态 module都需要有一个实例化的AEEAppInfo结构体，AEE从此数据结构中获得必要的module信息。在环境初始化（AEE_init）的时候AEE通过枚举每个MIF文件来获得各个module的必要信息，比如clsid等。

针对BREW3.1以前的版本，由于静态module不存在MIF文件，所以过程有所不同。每个静态module的实现必须提供一个XXX_getmodinfo()，在该函数中返回特定于该module的Mod_Load（）函数指针，通常形式为 XXXMod_Load,同时返回特定于该module的AEEAppInfo结构数据。所有的这些XXX_getmodinfo函数指针构成了一个staticmodinfo的数组。初始化时AEE通过检索该数组（猜测执行其中的每一个函数）来获得每个static module的相关模块信息（比如clsid）以及加载函数。

Module的加载是在运行时才进行的。对于动态应用，加载是通过通用函数AEEMod_Load实现的，而AEEMod_Load实际是调用AEEStaticMod_New，在AeeModGen.c中可以看到该函数的声明。

int AEEStaticMod_New(int16 nSize, IShell *pIShell, void *ph, IModule **ppMod,

                   PFNMODCREATEINST pfnMC,PFNFREEMODDATA pfnMF)

{

 AEEMod *pMe = NULL;

 VTBL(IModule) *modFuncs;

if (!ppMod || !pIShell) {

    return EFAILED;

 }

 *ppMod = NULL; 

#ifdef AEE_SIMULATOR

if (!ph) {

    return EFAILED;

 } else {

    g_pvtAEEStdLibEntry = (AEEHelperFuncs *)ph;

 }

#endif

 //Allocate memory for the AEEMod object

 if (nSize < sizeof(AEEMod)) {

    nSize += sizeof(AEEMod);

 }

if (NULL == (pMe = (AEEMod *)MALLOC(nSize + sizeof(IModuleVtbl)))) {

    return ENOMEMORY;

} 

 modFuncs = (IModuleVtbl *)((byte *)pMe + nSize);



 // Initialize individual entries in the VTBL

 modFuncs->AddRef         = AEEMod_AddRef;

 modFuncs->Release        = AEEMod_Release;

 modFuncs->CreateInstance = AEEMod_CreateInstance;

modFuncs->FreeResources  = AEEMod_FreeResources;



 // initialize the vtable

 INIT_VTBL(pMe, IModule, *modFuncs);



 // initialize the data members



 // Store address of Module's CreateInstance function

 pMe->pfnModCrInst = pfnMC;



 // Store Address of Module's FreeData function

 pMe->pfnModFreeData = pfnMF;

 pMe->m_nRefs = 1;

 pMe->m_pIShell = pIShell;



// Set the pointer in the parameter

 *ppMod = (IModule*)pMe;

 return SUCCESS;

}

上述代码在sdk中的AeeModGen.c可以找到，概括起来，就是在为module分配内存，并且实例化vtbl表，其中有两行代码值得注意：

modFuncs->CreateInstance = AEEMod_CreateInstance;

pMe->pfnModCrInst = pfnMC;

第一行是指定module的创建函数为AEEMod_CreateInstance，而第二行是指定该module具有自身特殊的创建函数，该函数即为参数pfnMC指定的函数。而在AEEMod_Load中调用AEEStaticMod_New时该参数为NULL，即所有动态 module采用通用的createinstance 函数（该函数实际上即为AEEClsCreateInstance）, 对于静态module，其实其自身的XXXMod_Load加载函数和通用的AEEMod_Load具体实现几乎一样，最主要的区别在于其调用AEEStaticMod_New时指定了pfnMC参数，即静态 module需要指定自身的创建函数。这是通过在AEEStaticMod_New中代码modFuncs->CreateInstance = AEEMod_CreateInstance 来指定，由AEEMod_CreateInstance函数来创建的。

static int AEEMod_CreateInstance(IModule *pIModule,IShell *pIShell, AEECLSID ClsId, void **ppObj) {

 AEEMod    *pme = (AEEMod *)pIModule;

 int        nErr = 0;

 // For a dynamic module, they must supply the AEEClsCreateInstance()

 //   function. Hence, invoke it. For a static app, they will have

if (pme->pfnModCrInst) {

    nErr = pme->pfnModCrInst(ClsId, pIShell, pIModule, ppObj);

#if !defined(AEE_STATIC)

 } else {

    nErr = AEEClsCreateInstance(ClsId, pIShell, pIModule, ppObj);

#endif

 return nErr;

}

对于动态 module，由于pme->pfnModCrInst为NULL，所以调用通用的创建函数AEEClsCreateInstance(ClsId, pIShell, pIModule, ppObj)来进行创建。AEEClsCreatelnstance通过调用AEEAppGen.c的AEEApplet_New()方法来完成空间分配、虚函数表构造、初始化等一系列工作。而对于静态module，因为指定了自身的创建函数，所以pme->pfnModCrInst不为NULL，从而执行特定于该module自身的创建函数



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914184.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914184.aspx)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=applet&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




