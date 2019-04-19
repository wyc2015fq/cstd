# Overlay Z-Orders (Windows CE 5.0) - xqhrs232的专栏 - CSDN博客
2012年11月21日 21:20:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：746
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://msdn.microsoft.com/en-us/library/ms924178.aspx](http://msdn.microsoft.com/en-us/library/ms924178.aspx)
相关网帖
1.Windows CE, directdraw display driver - overlay z order----[http://www.mofeel.net/1020-microsoft-public-windowsce-embedded/7762.aspx](http://www.mofeel.net/1020-microsoft-public-windowsce-embedded/7762.aspx)
2.development, DirectDraw Blt and AlphaBlt are really slow -----[http://www.mofeel.net/1019-microsoft-public-windowsce-app-development/5851.aspx](http://www.mofeel.net/1019-microsoft-public-windowsce-app-development/5851.aspx)
3.Windows CE, How to handle with IDirectDrawSurface::UpdateOverlayZOrder in----[http://www.mofeel.net/1020-microsoft-public-windowsce-embedded/640.aspx](http://www.mofeel.net/1020-microsoft-public-windowsce-embedded/640.aspx)
4.DDRAWI_DIRECTDRAW_GBL (Windows CE 5.0)----[http://msdn.microsoft.com/en-us/library/ms898266.aspx](http://msdn.microsoft.com/en-us/library/ms898266.aspx)
5.RE: UpdateOverlayZOrder failure----[http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.embedded/2008-12/msg00069.html](http://www.tech-archive.net/Archive/WindowsCE/microsoft.public.windowsce.embedded/2008-12/msg00069.html)

**Windows CE 5.0**
This topic has not yet been rated - [Rate
 this topic](http://msdn.microsoft.com/en-us/library/ms924178.aspx#feedback)
[Send
 Feedback](mailto:medfdbk@microsoft.com?subject=Product:%20Windows%20CE%20%7C%20Version:%205.0%20FP%20NMD%20%7C%20Release:%20Platform%20Builder%20%7C%20Title:%20Overlay%20Z-Orders%20%7C%20Filename:%20wce50conoverlayzorders.htm%20%7C%20Built%20on:%20Thursday,%20February%2002,%202006&body=We%20cannot%20answer%20technical%20support%20questions,%20but%20we%20welcome%20your%20suggestions%20and%20comments%20about%20the%20documentation.%20For%20technical%20support,%20see%20http://go.microsoft.com/fwlink/?linkid=3939.%20All%20feedback%20is%20monitored%20regularly%20by%20the%20Windows%20CE%20documentation%20team%20and%20typically%20answered%20within%20two%20business%20days.%20%20If%20you%20have%20any%20questions%20about%20how%20the%20feedback%20process%20works,%20please%20see%20the%20topic%20Questions%20About%20Submitting%20Feedback.)
Overlay surfaces are assumed to be on top of all other screen elements, but when you display multiple overlay surfaces, you need some way to visually organize them.
DirectDraw supports overlay z-ordering to manage the order in which overlays clip each other. Z-order values represent conceptual distances from the primary surface toward the viewer.
They range from 0, which is just on top of the primary surface, to 4 billion, which is as close to the viewer as possible, and no two overlays can share the same z-order.
You set z-order values by calling the [IDirectDrawSurface5::UpdateOverlayZOrder](http://msdn.microsoft.com/en-us/library/aa451768.aspx) method.
Destination color keys are affected only by the bits on the primary surface, not by overlays occluded by other overlays. Source color keys work on an overlay whether or not a z-order was specified for the overlay.
Overlays without a specified z-order are assumed to have a z-order of 0. Overlays that do not have a specified z-order behave in unpredictable ways when overlaying the same area on the primary surface.
A DirectDraw object does not track the z-orders of overlays displayed by other applications.
> **Note   **You can ensure proper clipping of multiple overlay surfaces by calling **UpdateOverlayZOrder** in response to WM_KILLFOCUS messages.
> When you receive this message, set your overlay surface to the rearmost z-order position by calling the **UpdateOverlayZOrder**method with the dwFlags parameter set to DDOVERZ_SENDTOBACK.
> 
> 
> //=============================================
> 备注::
> 1>今天也遇到这样的问题，多个OVERLAY程序，有的OVERLAY程序老是会被盖住的情况，不知道这篇文章中讲的方法是不是可行，明天试一下看看，希望可性！！！
> 
> 
