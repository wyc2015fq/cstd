# 如何得到GPU的运行状态，这个找了好久 - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 09:50:11[vbskj](https://me.csdn.net/vbskj)阅读数：11452










现在主流的显卡有两种，一类是Nvidia的GPU，一种是AMD的GPU。

很多人都在做GPU的时候，都想知道GPU的特性，想知道使用时候的状态，有问题，频率，内存使用情况，占有率，很多人问道了，是否可以使用系统底层的API得到？

其实系统给的API并不完善，经常得不到完全的信息。

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


对于NVidia的GPU，推荐使用NVAPI来得到这些参数：

[http://developer.nvidia.com/nvapi](http://developer.nvidia.com/nvapi)


这个链接是NVAPI的具体的信息：



# NVAPI





## WHAT IS NVAPI, AND WHY DO YOU NEED IT?


NVAPI is NVIDIA's core software development kit that allows direct access to NVIDIA GPUs and drivers on all windows platforms. NVAPI provides support for [categories
 of operations](http://developer.nvidia.com/nvapi#categories) that range beyond the scope of those found in familiar graphics APIs such as DirectX and OpenGL.


Initially exposed only to OEMs and game developer tools, NVAPI is now available for download to all developers interested in building Windows applications on NVIDIA GPUs.

## DOWNLOADS
- [NVAPI
 (2012-Apr)](http://developer.nvidia.com/sites/default/files/akamai/tools/files/NVAPI-Apr2012.zip) (1MB ZIP)
- [NVIDIA
 Driver Settings Programming Guide](http://developer.nvidia.com/sites/default/files/akamai/tools/docs/PG-5116-001_v02_public.pdf) (520k PDF)

## APPLICATION TYPES


NVAPI allows full access to NVIDIA GPUs and drivers in any UI and non-UI application. Under Windows Vista, the application context using NVAPI must be launched in Session 1 or higher.

## NVAPI FEATURE CATEGORIES
- Driver
 Management

Initialization and driver version controls.- GPU
 Management

Enumeration of physical and logical GPUs. Thermal and Cooling controls.- Display
 Management

Enumeration of NVDIA displays, display postion and timings controls.- GPU
 Topology

Ability to enable SLI and Hybrid GPU topologies.[*](http://developer.nvidia.com/nvapi#NDA)- Frame
 Rendering

Ability to control Video and DX rendering not available in DX runtime.[*](http://developer.nvidia.com/nvapi#NDA)- System
 Management

Ability to query chipset and system specific information.- HDTV
 Controls

HDTV format and overscan controls.[*](http://developer.nvidia.com/nvapi#NDA)- Video
 Controls

Extended video engine controls.[*](http://developer.nvidia.com/nvapi#NDA)- Connecting
 and Configuring Monitors

Ability to set views on multiple target monitors.- GPU
 Overclocking

GPU overclocking APIs allows apps to run apps at maximum possible clocks. [*](http://developer.nvidia.com/nvapi#NDA)
* Use
 NDA edition for full control of this feature.

## NVAPI COMPONENTS & DOCUMENTATION


NVAPI components includes the header, lib files for each platform - x86 and x64 respectively.


Documentation is provided as a compiled HTML help (.chm) file in each download.

## NVAPI SUPPORT


NVAPI is constantly and rigorously tested not only for end-users, but for internal use at NVIDIA and our closest partners. To discuss common issues regarding NVAPI with NVIDIA and other developers, [click
 here](http://developer.nvidia.com/forums/index.php?showforum=10) for a direct link to NVIDIA's developer forums.

## NVAPI NDA CUSTOMERS


NVAPI comes in two "flavors": the public version, available below, and a more extensive version available to registered developers under NDA. Registered developers can [log
 in here,](https://nvdeveloper.nvidia.com/) or contact your NVIDIA account manager to make sure you have the latest NDA edition of NVAPI.

Not
 a registered NVIDIA developer? [Sign
 up here.](http://developer.nvidia.com/page/registered_developer_program.html)





--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------







对于AMD的GPU：

可以使用 GPUPerfAPI：

[http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)


这个链接上可以找到信息：



|GPUPerfAPI![Skip Navigation Links](http://developer.amd.com/WebResource.axd?d=_ttO-kr3w2EUGBya0gM1ASEg_X6pTaDytE3JemYXvdq53zVBexPEKAfceqcvSaVxh9FAt0SdzKC5k40nyEjRv8zwUek1&t=634210184576316535)[Home](http://developer.amd.com/pages/default.aspx) > [Tools](http://developer.amd.com/tools/Pages/default.aspx) > GPUPerfAPI| | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|![GpuPerAPI Banner](http://developer.amd.com/tools/GPUPerfAPI/PublishingImages/GPUPerfAPI-longbnrV1.jpg)### OverviewGPUPerfAPI is AMD's library for accessing GPU performance counters on ATI Radeon graphics cards. It is used by GPU PerfStudio 2 and the AMD APP Profiler and is now available to third party developers who wish to incorporate it within their own applications. GPUPerfAPI supports DirectX10, DirectX11, OpenGL, and OpenCL applications.### Release Notes**Version 2.9 (1/18/12)**- Add support for AMD Radeon 7000 Series hardware.- Add support for more FirePro, Mobility, and APU devices.- Improved memory consumption and performance during profiling.- Reduce memory footprint of the GPUPerfAPI DLLs.- Correct counters are now exposed on systems with dual GPUs of different hardware generations.- DirectX 10:- Fixed shader related counters on AMD Radeon HD 2000-5000 Series hardware.- Fixed PrimitivesIn counter on AMD Radeon HD 2000 Series hardware.**Version 2.8 (4/22/11)**- DirectX 10:- Fixed GPUTime support on AMD Radeon HD 2000 and 3000 Series hardware.- DirectX 11:- Fixed counter accessibility on AMD Radeon HD 2000 and 3000 Series hardware.**Version 2.7 (4/6/11)**- New entrypoint for registering logging callback function for improved troubleshooting.- Adds support for AMD Radeon HD 6000 Series hardware.- New DepthAndStencil counters give more detailed understanding of HiZ behavior.- OpenGL:- Improved accuracy of depth, texture, busy, and stalled counters.- OpenCL:- FetchSize counter now reports the correct value.- DirectX 10/11:- Fixed support for AMD Radeon HD 4000 Series hardware.- Fixed support for Catalyst 11.2 drivers on AMD Radeon HD 5000 Series hardware.**Version 2.5 (11/17/10)**- Adds Linux support for OpenGL and OpenCL.- More consistent naming between OpenCL and DX Compute Shader counters.- Improved methods for identifying existing hardware.- DirectX 10/11:- Improved accuracy of DepthAndStencil counters.- Improved accuracy of ColorBuffer counters.- OpenCL:- New counters: FetchSize, CacheHit, LDSFetchInsts, LDSWriteInsts, FastPath, CompletePath, PathUtilization.- OpenGL:- Fixed an issue that caused counters to not be available if the application is using queries.- Adds tessellation related counters.**Version 2.3 (6/4/10)**- Supports DirectX10, DirectX11, OpenGL on ATI Radeon 2000, 3000, 4000, and 5000 series.- Supports OpenCL on ATI Radeon 4000 and 5000 series.- Provides derived counters based on raw HW performance counters.- Manages memory automatically - no allocations required.- Requires ATI Catalyst driver 10.1 or later.### Documentation- [GPUPerfAPI User Guide (pdf)](http://developer.amd.com/tools/GPUPerfAPI/assets/GPUPerfAPI-UserGuide.pdf)### Support- [GPU Developer Tools Forum](http://forums.amd.com/devforum/categories.cfm?catid=347)- [GPU Developer Tools Help Desk](http://developer.amd.com/support/KnowledgeBase/pages/HelpdeskTicketForm.aspx?Category=7&SubCategory=55)### Download|[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)| | | | | | ||----|----|----|----|----|----|----|----|----|----|----||[Reset My View](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.|[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.|| | | | | | | | | | | ||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.| | | | | | ||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.| | | | | | |||[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----|| | | | | ||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[Reset My View](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.|[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.|
||[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----|| | | | | ||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[Reset My View](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.|[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.| |
|[Reset My View](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[File Name](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Launch Date](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[OS](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Bitness](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)|[Description](http://developer.amd.com/tools/GPUPerfAPI/Pages/default.aspx)||----|----|----|----|----||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.||[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.||[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.|[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.| | |
| | | | | | | | | | | | | |
|[GPUPerfAPI.2.9.314-lnx.gz](http://developer.amd.com/Downloads/GPUPerfAPI.2.9.314-lnx.gz)|01/18/2012|Linux®|32-bit/64-bit|Linux libraries for accessing OpenGL and OpenCL GPU performance counters.| | | | | | | | |
|[GPUPerfAPI-2.9.325.0.zip(1.93 KB)](http://developer.amd.com/Downloads/GPUPerfAPI-2.9.325.0.zip)|01/18/2011|Windows®|32-bit/64-bit|AMD's library for accessing GPU performance counters.| | | | | | | | |




========================================================================================================================================================================

GPU的参数特性，通过里面的sample是可以找到简单地例子，可以快速的入门的。

对于CPU的参数特性的拾取，下次再讲：）





From OpenHero  [http://blog.csdn.net/openhero/article/details/7702028#comments](http://blog.csdn.net/openhero/article/details/7702028#comments)







