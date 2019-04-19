# WINCE6.0下RIL中多路虚拟串口的实现解读之二 - xqhrs232的专栏 - CSDN博客
2010年02月04日 11:21:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2108
rilgsm.dll驱动程序的形成
1。先看C:/WINCE600/PLATFORM/DEVICEEMULATOR/FILES/platform.bib文件
; @CESYSGEN IF CELLCORE_MODULES_RIL
IF IMGNOTPCLOW !
IF IMGNORILTSP !
IF IMGFAKERIL
    fakeril.dll                 $(_FLATRELEASEDIR)/fakeril.dll              NK SHK
IF IMGFAKERILNETWORK
    fakeril_intermediate.dll    $(_FLATRELEASEDIR)/fakeril_intermediate.dll               NK  SHK
ENDIF ; IMGFAKERILNETWORK
ELSE
IF IMGTTPCOM
    rilgsm.dll          $(_FLATRELEASEDIR)/rilttpcom.dll        NK SHK
    ttpmux.dll          $(_FLATRELEASEDIR)/ttpmux.dll           NK SHK
IF IMGTTPCOM_TTPGENIE
    ttpgenie.dll        $(_FLATRELEASEDIR)/ttpgenie.dll         NK SHK
ENDIF ; IMGTTPCOM_TTPGENIE
ELSE
IF IMGENFORA
    rilgsm.dll          $(_FLATRELEASEDIR)/rilenfora.dll          NK  SHK
ENDIF IMGENFORA
ENDIF ; IMGTTPCOM
ENDIF ; IMGFAKERIL
ENDIF ; IMGNORILTSP !
ENDIF ; IMGNOTPCLOW !
; @CESYSGEN ENDIF CELLCORE_MODULES_RIL
其中有两个地方----针对不同的环境变量，对应把那个厂家的MODEMN驱动包成rilgsm.dll进NK
 rilgsm.dll          $(_FLATRELEASEDIR)/rilttpcom.dll        NK SHK
 rilgsm.dll          $(_FLATRELEASEDIR)/rilenfora.dll          NK  SHK
2。再看rilenfora.dll的形成过程
    AAAA-----下面的内容来自C:/WINCE600/PLATFORM/DEVICEEMULATOR/SRC/DRIVERS/RILPDD/ENFORA/下的SOURCES文件
TARGETNAME=rilenfora
!include $(_PUBLICROOT)/cellcore/ril/driver/mdd/sources_common.inc----------用到的WINCE里面的文件
!include $(_PUBLICROOT)/cellcore/ril/driver/mdd/sources_dll.inc----------用到的WINCE里面的文件
CDEFINES=$(CDEFINES) -DALPHA_IN_CPI
SOURCES=enfora.cpp
   BBBB------(_PUBLICROOT)/cellcore/ril/driver/mdd/sources_common.inc文件内容--------无实际作用，不引用
   CCCC-----(_PUBLICROOT)/cellcore/ril/driver/mdd/sources_dll.inc文件内容
!IF "$(IMGNOOSSVCSRIL)" == "1"
SKIPBUILD=1
!ENDIF
!IF "$(BSP_RILGSM_MSFT)" == "1"
SKIPBUILD=1
!ENDIF
!IF "$(BSP_NORILGSM)" == "1"
SKIPBUILD=1
!ENDIF
RELEASETYPE=PLATFORM
TARGETTYPE=DYNLINK
DEFFILE=$(_PUBLICROOT)/cellcore/ril/driver/mdd/rilgsm.def
DLLENTRY=_DllMainCRTStartup
SOURCELIBS=$(_PUBLICROOT)/cellcore/oak/lib/$(_CPUINDPATH)/rilmdd.lib
TARGETLIBS= $(TARGETLIBS) /
    $(_PROJECTROOT)/cesysgen/sdk/lib/$(_CPUINDPATH)/coredll.lib /
    $(_PUBLICROOT)/cellcore/oak/lib/$(_CPUINDPATH)/cellcore.lib /
    $(_PUBLICROOT)/cellcore/oak/lib/$(_CPUINDPATH)/ccoreutl.lib /
    $(_PUBLICROOT)/cellcore/oak/lib/$(_CPUINDPATH)/toolbox.lib
//-----------
通过上面的文件内容可见，PDD+MDD层合成一个合法的产生一个DLL的SOURCES文件----PDD层指定DLL的名称，MDD层指定DLL要用的rilgsm.def文件和要连接到的LIB库文件
这里的LIB文件就是MDD层的代码对应编译出来的，所以rilgsm.dll是一个MDD+PDD层代码合到一块的产物。
