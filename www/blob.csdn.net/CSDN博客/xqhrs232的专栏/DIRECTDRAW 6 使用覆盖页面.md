# DIRECTDRAW 6: 使用覆盖页面 - xqhrs232的专栏 - CSDN博客
2012年11月22日 14:29:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：745
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.sina.com.cn/s/blog_5be1061c0100bgka.html](http://blog.sina.com.cn/s/blog_5be1061c0100bgka.html)
相关网帖
1.[覆盖表面（Overlay Surfaces）](http://blog.csdn.net/xiaolei05/article/details/7384192)----[http://blog.csdn.net/xiaolei05/article/details/7384192](http://blog.csdn.net/xiaolei05/article/details/7384192)
 这个DIRECTDRAW 将逐步的向你演示如何在程序中使用 DirectDraw 和硬件支持的覆盖页面.DIRECTDRAW 围绕着 SDK 所带的蚊子(Mosquito)的例程而写.蚊子是一个使用了覆盖页面的换页链在桌面上显示位图动画的简单程序,它不对主页面执行位块传送操作.例程调整了覆盖页面的特性以适应硬件的限制.
蚊子例程分以下几步执行(复杂任务又被分为更小的子步骤):
• 步骤
 1:创建一个主页面
• 步骤
 2:测试硬件覆盖支持
• 步骤
 3:创建一个覆盖页面
• 步骤
 4:显示覆盖页面
• 步骤
 5:更新覆盖页面的显示位置
• 步骤
 6:隐藏覆盖页面
步骤 1:创建一个主页面
要准备使用覆盖页面,你必须先初始化 DirectDraw 并创建一个将被用于覆盖页面显示其上的主页面.蚊子例程使用如下代码创建主页面:
 //
 对结构清零和设置 dwSize 成员.
 ZeroMemory(&ddsd,
 sizeof(ddsd));
 ddsd.dwSize
 = sizeof(ddsd);
 //
 设置标志并创建一个主页面.
 ddsd.dwFlags
 = DDSD_CAPS;
 ddsd.ddsCaps.dwCaps
 = DDSCAPS_PRIMARYSURFACE;
 ddrval
 = g_lpdd->CreateSurface(&ddsd, &g_lpddsPrimary, NULL );
上例一开始初始化后面将用到的 DDSURFACEDESC 结构.然后设置合适的标志来创建主页面并调用 IDirectDraw2::CreateSurface 函数创建之.对于这个调用,第一个参数是一个指向 DDSURFACEDESC 结构的指针,它描述了要创建的页面.第二个参数是一个指向调用成功后获得
 IDirectDrawSurface 接口的变量的指针.最后一个参数设为 NULL 表示没有使用 COM 集合.
步骤 2:测试硬件覆盖支持
初始化 DirectDraw 之后,你需要校验设备是否支持覆盖页面.因为 DirectDraw 不仿真覆盖,如果硬件设备不支持你就不能继续.你可以通过调用 IDirectDraw2::GetCaps 函数获得硬件性能来测试是否支持覆盖.调用该函数之后,查看相关的 DDCAPS
 结构的 dwFlags 成员是否有 DDCAPS_OVERLAY 标志.如果标志存在,表示显示硬件支持覆盖;反之,你不能在这个设备上使用覆盖页面.
以下从蚊子例程截取的例子演示了如何测试硬件覆盖支持:
BOOL AreOverlaysSupported()
{
 DDCAPS  capsDrv;
 HRESULT
 ddrval;
 //
 获得驱动性能来确定是否支持覆盖.
 ZeroMemory(&capsDrv,
 sizeof(capsDrv));
 capsDrv.dwSize
 = sizeof(capsDrv);
 ddrval
 = g_lpdd->GetCaps(&capsDrv, NULL);
 if
 (FAILED(ddrval))
 return
 FALSE;
 //
 当前模式是否支持覆盖?
 //
 (现在 DirectDraw 仿真层还不支持覆盖.
 //
 如果硬件不支持与覆盖相关的 API 将失败). 
 if
 (!(capsDrv.dwCaps & DDCAPS_OVERLAY))
 return
 FALSE;
 return
 TRUE;
}
上例调用 IDirectDraw2::GetCaps 函数来获得硬件性能.第一个参数是被用来填充描述设备驱动性能的信息的 DDCAPS 结构的地址.因为程序不需要仿真性能的信息,第二个参数设为 NULL.
获得驱动性能之后,例程使用逻辑 AND 操作来检查 dwCaps 成员是否有 DDCAPS_OVERLAY 标志.如果没有,例程返回 FALSE 以示失败.否则,返回 TRUE 表示驱动设备支持覆盖页面.
在你的代码里,这也是一个好的时机来检查 DDCAPS 结构的 dwMaxVisibleOverlays 和 dwCurrentVisibleOverlays 成员以确定没有其它程序使用覆盖页面.
步骤 3:创建一个覆盖页面
在你知道驱动设备支持覆盖页面后就可以试着创建一个了.因为没有一个关于设备必须如何支持覆盖页面的标准的规范,你不能指望能够创建一个任意尺寸或像素格式的覆盖页面.另外，你也不要期望创建覆盖页面一次成功.因此,准备好多次尝试创建从最好的到最差的配置直到有一个可用.
注意  你可以调用
 IDirectDraw2::GetFourCCCodes 函数来获得一个描述驱动设备对覆盖页面支持的非RGB像素格式的 FOURCC 代码列表.然而,如果你要使用 RGB 覆盖页面,推荐你尝试用多种普通的 RGB 格式创建覆盖页面,一种不行就试试差一点的一种.
蚊子例程就是在这种"最好的情况到最差的情况"的思想的指导下创建覆盖页面的.例程首先试着创建一个三缓冲换页复杂覆盖页面.如果不行,例程就试着用另一种普通的像素格式配置.如下的代码片断演示了是如何做的:
 ZeroMemory(&ddsdOverlay,
 sizeof(ddsdOverlay));
 ddsdOverlay.dwSize
 = sizeof(ddsdOverlay);
 ddsdOverlay.dwFlags=
 DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |
 DDSD_BACKBUFFERCOUNT|
 DDSD_PIXELFORMAT;
 ddsdOverlay.ddsCaps.dwCaps
 = DDSCAPS_OVERLAY | DDSCAPS_FLIP |
 DDSCAPS_COMPLEX
 | DDSCAPS_VIDEOMEMORY;
 ddsdOverlay.dwWidth  =320;
 ddsdOverlay.dwHeight
 =240;
 ddsdOverlay.dwBackBufferCount=2;
 //试着用我们的列表里的一种像素格式创建覆盖页面.
 i=0;
 do{
 ddsdOverlay.ddpfPixelFormat=g_ddpfOverlayFormats[i];
 //
 尝试创建覆盖页面
 ddrval
 = g_lpdd->CreateSurface(&ddsdOverlay, &g_lpddsOverlay, NULL);
 }
 while( FAILED(ddrval) && (++i < NUM_OVERLAY_FORMATS) );
上例通过在 DDSURFACEDESC 结构中设置相应的标志和值来反映要创建的是三缓冲换页复杂覆盖页面.然后例程执行了一个循环尝试在各种普通的像素格式之间创建页面,从最好得到最差的.如果尝试成功,跳出循环.如果所有的尝试都失败,可能是没有足够的内存来执行三缓冲任务或不支持换页覆盖页面.在这种情况下,例程尝试使用单个非换页覆盖页面的配置,如下所示:
 //如果创建三缓冲复杂覆盖页面失败,尝试单个非换页换存
 if(FAILED(ddrval))
 {
 ddsdOverlay.dwBackBufferCount=0;
 ddsdOverlay.ddsCaps.dwCaps=DDSCAPS_OVERLAY
 | DDSCAPS_VIDEOMEMORY;
 ddsdOverlay.dwFlags=
 DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
 //
 尝试创建覆盖页面
 ddrval
 = g_lpdd->CreateSurface(&ddsdOverlay, &g_lpddsOverlay, NULL);
 i=0;
 do{
 ddsdOverlay.ddpfPixelFormat=g_ddpfOverlayFormats[i];
 ddrval
 = g_lpdd->CreateSurface(&ddsdOverlay, &g_lpddsOverlay, NULL);
 }
 while( FAILED(ddrval) && (++i < NUM_OVERLAY_FORMATS) );
 //
 不能创建覆盖页面.退出,返回失败.
 if
 (FAILED(ddrval))
 return
 FALSE;
 }
前面的代码重新设置了 DDSURFACEDESC 结构的标志和值以表明创建单个非换页覆盖页面.例程再次使用循环尝试各种像素格式来创建页面,如果尝试成功就停止循环.如果仍不行,例程返回 FALSE 以示失败.
在你成功地创建了覆盖页面之后,你可以把位图装载其上以备显示.
步骤 4:显示覆盖页面
在创建了覆盖页面之后,你就可以显示它了.通常,显示硬件要对覆盖页面的矩形的位置和像素进行对齐约束.另外,为了成功地显示覆盖页面你常常需要通过调整目标矩形的宽度来解决最少所需的拉伸因数.蚊子例程通过以下步骤准备显示覆盖页面:
• 步骤
 4.1:确定最少显示需求
• 步骤
 4.2:设置源和目标矩形
• 步骤
 4.3:显示覆盖页面
Step 4.1: Determining the Minimum Display Requirements
大部分的显示硬件在显示覆盖时都会有一些强制性的约束.为了成功的显示覆盖页面你必须小心的满足这些约束.你可以通过调用 IDirectDraw2::GetCaps 函数获得关于这些约束的信息.该函数填充的 DDCAPS 结构包含了关于覆盖性能和使用约束的信息.不同的硬件有不同的约束,因此一定要查看
 dwFlags 成员的约束来决定用于你的是哪种约束.
蚊子例程开始先获得硬件的性能,然后采取基于最小拉伸因子的行动,如以下例程片断所示:
 //
 获得驱动性能
 ddrval
 = g_lpdd->GetCaps(&capsDrv, NULL);
 if
 (FAILED(ddrval))
 return
 FALSE;
 //
 确定最小拉伸因子并按此设置局部变量.
 if(capsDrv.dwCaps
 & DDCAPS_OVERLAYSTRETCH)
 uStretchFactor1000
 = (capsDrv.dwMinOverlayStretch>1000) ? capsDrv.dwMinOverlayStretch : 1000;
 else
 uStretchFactor1000
 = 1000;
上面的代码调用 GetCaps 仅获得硬件的性能.第一个参数是将被填充设备驱动信息的 DDCAPS 结构的指针, 第二个参数设为 NULL 表明不提取仿真的信息.
程序在一个临时变量中保留了最小拉伸因子以备后用(注意最小拉伸因子被报告为乘以 1000,因此 1300 实际上是 1.3). 如果驱动报告的值大于 1000,这表示所有的目标矩形必须沿 x 轴按此比例拉伸.例如,如果驱动报告的拉伸因子为 1.3 而源矩形为 320 像素宽,目标矩形至少
 416 像素宽.如果驱动报告的拉伸因子小于 1000,这表示驱动可以显示覆盖比源矩形小,但也可以按需要拉伸.
接着,例程检查了描述驱动尺寸对齐约束的值,如下所示:
 //
 Grab any alignment restrictions and set the local variables acordingly.
 uSrcSizeAlign
 = (capsDrv.dwCaps & DDCAPS_ALIGNSIZESRC)?capsDrv.dwAlignSizeSrc:0;
 uDestSizeAlign=
 (capsDrv.dwCaps & DDCAPS_ALIGNSIZESRC)?capsDrv.dwAlignSizeDest:0;
例程使用了更多的临时变量来保存 dwAlignSizeSrc 和 dwAlignSizeDest 成员报告的尺寸对齐约束.这些值提供了关于像素宽度对齐约束的信息将在后面设置源和目标矩形的尺寸时需要.源和目标矩形的像素宽度必须使这些成员值的倍数.
最后,例程检查了描述目标矩形边界对齐的值:
 //
 设置全局的"目标位置对齐"我们就不需要每次移动覆盖页面时都调用 GetCaps().
 if
 (capsDrv.dwCaps & DDCAPS_ALIGNBOUNDARYDEST)
 g_dwOverlayXPositionAlignment
 = capsDrv.dwAlignBoundaryDest;
 else
 g_dwOverlayXPositionAlignment
 = 0;
前面的代码使用了一个全局变量来保存目标矩形的边界对齐的值,它从 dwAlignBoundaryDest 成员获得.这个值将在程序后面重新放置覆盖时用到(细节请参阅步骤 5:更新覆盖页面的显示位置).你必须设置目标矩形的左上角的X坐标像素值与之对齐.亦即,如果值为 4,你只能指定目标矩形的左上角的X坐标像素值为
 0, 4, 8, 12, 等等.蚊子例程在 0,0 处初始化和显示覆盖,因此可以假定为对齐的同时例程也不需要获取约束信息直到第一次显示覆盖以后. 你可能要做的与之不同,因此你可能需要在显示覆盖以前检查这一信息并调整目标矩形.
步骤 4.2:设置源和目标矩形
在获得了驱动的覆盖约束之后你应该按此设置源矩形和目标矩形的值,以确保能够正确显示覆盖。蚊子例程下面的代码设定了源矩形的特性:
 //
 设置源 RECT 的初始值.
 rs.left=0;
 rs.top=0;
 rs.right
 = 320;
 rs.bottom
 = 240;
 //
 如果必要,使用尺寸对齐约束.
 if
 (capsDrv.dwCaps & DDCAPS_ALIGNSIZESRC && uSrcSizeAlign)
 rs.right
 -= rs.right % uSrcSizeAlign;
上面的代码为页面设置了初始值包含整个页面的尺寸.如果设备驱动需要源矩形尺寸对齐,例程就调整源矩形来保证.例程调整了源矩形的宽度比原始值小是因为如果不完全重建页面就不能扩展宽度.然而,你的代码可以简单第以较小的矩形开始然后按照驱动约束拓宽.
在设定了源矩形的大小和适应了硬件约束后,需要设置和调整目标矩形的大小.这一过程需要稍多的工作因为目标矩形可能需要先拉伸再调整以符合尺寸对齐约束.下面的代码根据最小拉伸因子来完成这一任务:
 //
 设置目标 RECT,以源 RECT 的值开始.
 //
 我们使用源 RECT 的尺寸而不用页面的尺寸因为它们是不同的.
 rd.left=0;
 rd.top=0;
 rd.right  =
 (rs.right*uStretchFactor1000+999)/1000;
 //
 (Adding 999 avoids integer truncation problems.)
 //
 (这不是 DDraw 所必需的,但我们拉伸了高度也要维持屏幕高宽比).
 rd.bottom
 = rs.bottom*uStretchFactor1000/1000;
前面的代码先设置目标矩形的左上角为屏幕的左上角,再根据最小拉伸因子设置其宽度.根据拉伸因子调整时,注意例程在宽度和拉伸因子的乘积上又加了999.这是为了避免出现整数截断,那会导致矩形和最小拉伸因子要求的宽度不一致.要得到更多的信息,请参阅最小和最大拉伸因子.例程在拉伸宽度后也拉伸了高度.拉伸高度并不是必须的,这里只是为了保持位图的长宽比以免显示扭曲.
拉伸目标矩形后,例程继续对其进行调整以保证和尺寸对齐约束一致,如以下代码所示:
 //
 调整目标 RECT 的宽度和对其约束一致.
 if
 (capsDrv.dwCaps & DDCAPS_ALIGNSIZEDEST && uDestSizeAlign)
 rd.right
 = (int)((rd.right+uDestSizeAlign-1)/uDestSizeAlign)*uDestSizeAlign;
例程检测性能标志来查看驱动是否强制目标尺寸对齐约束.如果是,就增加目标矩形的宽度到足够的像素来满足对齐约束.注意对矩形的调整是扩展其宽度而不能减少其宽度.因为减少宽度可能会导致目标矩形比最小拉伸因子要求的要小,从而引起显示覆盖表面失败.
步骤 4.3:显示覆盖页面
在设置了源矩形和目标矩形后，就可以第一次显示覆盖了。如果你正确地准备了，这会很简单。蚊子例程用如下的代码来显示覆盖：
 //
 设置要传送给 UpdateOverlay 的标志
 dwUpdateFlags
 = DDOVER_SHOW | DDOVER_DDFX;
 //
 硬件覆盖是否支持源关键色?
 //
 如果支持，我们就可以隐藏图形的黑色背景.
 //
 这可能不能在 YUV 格式下工作
 if
 (capsDrv.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
 dwUpdateFlags
 |= DDOVER_KEYSRCOVERRIDE;
 //
 创建一个覆盖 FX 结构这样我们就可以指定一个源关键色.
 //
 如果不设置 DDOVER_SRCKEYOVERRIDE 标志着一信息将被忽略.
 ZeroMemory(&ovfx,
 sizeof(ovfx));
 ovfx.dwSize
 = sizeof(ovfx);
 ovfx.dckSrcColorkey.dwColorSpaceLowValue=0;
 // 指定黑色为源关键色
 ovfx.dckSrcColorkey.dwColorSpaceHighValue=0;
 //
 调用 UpdateOverlay() 显示覆盖到屏幕.
 ddrval
 = g_lpddsOverlay->UpdateOverlay(&rs, g_lpddsPrimary, &rd, dwUpdateFlags, &ovfx);
 if(FAILED(ddrval))
 return
 FALSE;
前面的例程开始在临时变量 dwUpdateFlags 中设定了 DDOVER_SHOW 和 DDOVER_DDFX 标志，指明该覆盖是第一次显示，以及硬件应该使用包含在相关的 DDOVERLAYFX 结构中的效果信息完成这一工作。然后，例程检查已存在的 DDCAPS 结构确定覆盖是否支持源
 关键色(color keying) 。如果是， DDOVER_KEYSRCOVERRIDE 标志就包含在 dwUpdateFlags 变量中利用源关键色，例程也据此设置关键色。
准备完之后，例程调用 IDirectDrawSurface3::UpdateOverlay 函数来显示覆盖。对于该调用，第一个和第三个参数是已调整的源矩形和目标矩形的地址。第二个参数是覆盖将显示在其上的主表面的地址。第四个参数包括了放置于此前准备的 dwUpdateFlags
 变量中的标志，第五个参数是 DDOVERLAYFX 结构的地址，其成员将被设为同那些标志相匹配。
如果硬件只支持一个覆盖页面而且该表面正在使用， UpdateOverlay 函数就会失败，并返回 DDERR_OUTOFCAPS。 另外，如果 UpdateOverlay 失败，你就需要尝试减少目标矩形的宽度来应付硬件错误报告出的最小拉伸因子过小的可能性。不过，这种情况很少发生，在蚊子例程中如果
 UpdateOverlay 调用失败也只是简单地返回一个错误信息。
步骤 5:更新覆盖页面的显示位置
显示覆盖页面之后，有可能就不需要做其它操作了。然而，有些软件还需要重新放置覆盖页面。蚊子例程就使用IDirectDrawSurface3::SetOverlayPosition 函数重新放置覆盖，代码如下例程所示：
 //
 设置 x轴 和 y轴 坐标
 .
 .
 .
 //
 我们需要检查 x 轴的任何对齐约束，如果必要就对齐。
 if
 (g_dwOverlayXPositionAlignment)
 dwXAligned
 = g_nOverlayXPos - g_nOverlayXPos % g_dwOverlayXPositionAlignment;
 else
 dwXAligned
 = g_nOverlayXPos;
 //
 设置覆盖到新的位置。
 ddrval
 = g_lpddsOverlay->SetOverlayPosition(dwXAligned, g_nOverlayYPos);
 if
 (ddrval == DDERR_SURFACELOST)
 {
 if
 (!RestoreAllSurfaces())
 return;
 }
以上例程开始对齐矩形以满足可能存在的任何目标矩形边界对齐约束。当程序此前调用 IDirectDraw2::GetCaps 函数时，以将全局变量 g_dwOverlayXPositionAlignment
 已经设定为同 DDCAPS 结构中 dwAlignBoundaryDest 成员所报告出的值相等（详情请参阅步骤 4.1:确定最少显示需求）。如果存在目标对齐约束，程序就据此调整新的X坐标为像素对齐。若不满足要求，覆盖页面就不能显示。
在完成了对新的X坐标所需的调整之后，例程调用 IDirectDrawSurface3::SetOverlayPosition 函数重新放置覆盖。对于这一调用，第一个参数是对齐的X坐标，第二个参数是新的Y坐标。这些值表明了覆盖左上角新的位置。这里并不接受也不需要宽度和高度信息，因为
 DirectDraw 在开始用 IDirectDrawSurface3::UpdateOverlay 函数初始化显示覆盖时就已经获得了页面的大小。如果因为一个或多个页面丢失而引起的重新放置覆盖表面的失败，例程就调用一个程序中定义的函数来恢复它们并重新装入它们的位图。
注意  小心不要使用太靠近目标页面的右、下边界的坐标。
 IDirectDraw2::SetOverlayPosition 函数并不执行裁剪功能，所以使用那些可能导致覆盖超出目标页面边界的坐标会引起调用的失败，并返回 DDERR_INVALIDPOSITION。
步骤 6:隐藏覆盖页面
当你不再需要覆盖页面或只想不让覆盖可见，你可以设定适当的标志调用 IDirectDrawSurface3::UpdateOverlay 函数来隐藏该覆盖表面。蚊子例程在准备关闭应用程序时用以下代码隐藏覆盖表面：
void DestroyOverlay()
{
 if
 (g_lpddsOverlay){
 //
 带 DDOVER_HIDE 标志使用 UpdateOverlay() 来从显示中移除覆盖。
 g_lpddsOverlay->UpdateOverlay(NULL,
 g_lpddsPrimary, NULL, DDOVER_HIDE, NULL);
 g_lpddsOverlay->Release();
 g_lpddsOverlay=NULL;
 }
}
以上例程在调用 IDirectDrawSurface3::UpdateOverlay 时，对源矩形和目标矩形指定了 NULL，因为在隐藏覆盖的过程中与它们无关。例程简单地使用第五个参数为 NULL 因为不使用覆盖效果。第二个参数是目标页面的指针。最后，例程在第四个参数使用
 DDOVER_HIDE 标志表明该覆盖将从显示中移除。
例程在隐藏覆盖之后，释放了它的 IDirectDrawSurface3 接口，并将其全局变量设为 NULL 使之无效。对于蚊子例程的目的，覆盖就不再需要了。如果在应用程序中以后还需要使用该覆盖页面，就只需简单地隐藏覆盖而不释放它，然后在需要的时候再重新显示它。
