# vaapi 硬解 demo - u013366022的专栏 - CSDN博客
2015年05月06日 10:04:53[slitaz](https://me.csdn.net/u013366022)阅读数：4380
### MID Multimedia frameworks
As an application vendor you may want to take advantage of this hardware video decode so that your application get the best out of the MID platform. You may not need to use the VA API in your code yourself. There exists several multimedia frameworks today that
 have been optimized to use this capability. Any application built on top of those frameworks can get the benefits of the platform without having VA API insights.
Two frameworks are available on the Moblin* platform: Helix* and Gstreamer*.
Helix framework is capable of using the video decode hardware acceleration. As a result every player built on top of the Helix framework benefits from this feature like the RealPlayer* for MID ([http://www.helixcommunity.org](http://www.helixcommunity.org/)).
Gstreamer is a very popular multimedia framework in the open source community. Many open source media players are based on this framework in the Linux world : Totem*, Rhythm*, Banshee*,...The company Fluendo* ([http://www.fluendo.com](http://www.fluendo.com/))
 is providing optimized codecs for the Gstreamer framework for the Intel® GMA 500 chipset. By using the Fluendo optimized codecs, all these applications can benefit seamlessly from the video hardware acceleration.
An implementation of the FFmpeg codecs using the VA API has been developed by Splitted-Desktop Systems* ([http://www.splitted-desktop.com](http://www.splitted-desktop.com/))
 , which resulted in dramatic performance improvements with video playbacks in MPlayer* on the current Intel® processor-based MIDs using the Intel® GMA 500 chipset. For reference, the sources are available at this location: [http://www.splitted-desktop.com/en/libva/](http://www.splitted-desktop.com/en/libva/)
### Typical code structure
The code implementing a video decoding with the VA API must follow a certain structure.
After an initialization phase, the client negotiates a mutually acceptable configuration with the server. It locks down profile, entry point, and other attributes that are not varying along the stream decoding. Once the configuration is set and accepted by
 the server, the client creates a decode context. This decode context can be seen as a virtualized hardware decode pipeline. The decode pipeline must be configured by passing a number of datasets.
The program is now ready to start decoding the stream. The client gets and fill decode buffers with slices and macroblock level data. The decode buffers are sent to the server until the server is able to decode and render the frame. The client then reiterate
 the operation with the decode buffers over and over to decode the bit stream. See below the typical flowchart of a decoder using the VA API. We will detail the different phases of the algorithm in the coming chapters.
![](https://software.intel.com/sites/default/files/m/d/4/1/d/8/mid-figure-4.jpg)
Initialization Phase
Setting display
|`1`|```x11_display = XOpenDisplay(NULL);```|
|`2`|```vaDisplay = vaGetDisplay(x11_display);```|
|`3`|```vaStatus = vaInitialize(vaDisplay, &major_ver, &minor_ver);```|
Negotiating and creating configuration
In order to determine the level of hardware acceleration supported on a particular platform, the client needs to make sure the hardware supports the desired video profile (format) and the entry points available for that profile. For this, the client queries
 the driver on its capabilities using the vaQueryConfigEntrypoints. Depending on the driver answer the client can take the appropriate action. Find here a code sample showing a configuration negotiation phase.
|`01`|```vaQueryConfigEntrypoints(vaDisplay, VAProfileMPEG2Main, entrypoints,```|
|`02`|`                             ``&num_entrypoints);`|
|`03`||
|`04`|`    ``for````(vld_entrypoint = 0; vld_entrypoint < num_entrypoints; vld_entrypoint++) {```|
|`05`|`        ``if````(entrypoints[vld_entrypoint] == VAEntrypointVLD)```|
|`06`|`            ``break``;`|
|`07`|`    ``}`|
|`08`|`    ``if````(vld_entrypoint == num_entrypoints) {```|
|`09`|`        ````/* not find VLD entry point */```|
|`10`|`        ``exit``(-1);`|
|`11`|`    ``}`|
|`12`||
|`13`|`    ````/* Assuming finding VLD, find out the format for the render target */```|
|`14`|`    ````attrib.type = VAConfigAttribRTFormat;```|
|`15`|`    ````vaGetConfigAttributes(vaDisplay, VAProfileMPEG2Main, VAEntrypointVLD,```|
|`16`|`                          ````&attrib, 1);```|
|`17`|`    ``if````((attrib.value & VA_RT_FORMAT_YUV420) == 0) {```|
|`18`|`        ````/* not find desired YUV420 RT format */```|
|`19`|`        ``exit``(-1);`|
|`20`|`    ``}`|
|`21`|`    `|
|`22`|`    ````vaStatus = vaCreateConfig(vaDisplay, VAProfileMPEG2Main, VAEntrypointVLD,```|
|`23`|`                              ````&attrib, 1,&config_id);```|
Decode context
Once a decode configuration has been created, the next step is to create a decode context which represents a virtual hardware decode pipeline. This virtual decode pipeline outputs decoded pixels to a render target called "Surface". The decoded frames are stored
 in Surfaces and can subsequently be rendered to X drawables defined in the first phase. 
The client creates two objects. It creates first a Surface object. This object gathers the parameters of the render target to be created by the driver like picture width, height and format. The second object is a "Context" object. The Context object is bound
 with a Surface object when it is created. Once a surface is bound to a given context, it can not be used to create another context. The association is removed when the context is destroyed. Both contexts and surfaces are identified by unique IDs and its implementation
 specific internals are kept opaque to the client. Any operation whether it is data transfer or frame decoding will be given this context ID as a parameter to determine which virtual decode pipeline is used. See below a code sample showing how to set the decode
 context.
|`01`|`/* `|
|`02`|`         ````* create surfaces for the current target as well as reference frames```|
|`03`|`        ````VASurfaceID vaSurface;```|
|`04`|`            ````vaStatus = vaCreateSurfaces(vaDisplay,surf_width,surf_height,```|
|`05`|`                                ````VA_RT_FORMAT_YUV420, 1, &vaSurface);```|
|`06`|`        ``/* `|
|`07`|`         ````* Create a context for this decode pipe```|
|`08`|`         ``*/`|
|`09`|`        ````VAContextID vaContext;```|
|`10`|`        `|
|`11`|`    ````vaStatus = vaCreateContext(vaDisplay, config_id,```|
|`12`|`                               ``CLIP_WIDTH,`|
|`13`|`                               ``((CLIP_HEIGHT+15)/16)*16,`|
|`14`|`                               ``VA_PROGRESSIVE,`|
|`15`|`                               ``&vaSurface,`|
|`16`|`                               ``1,`|
|`17`|`                               ``&vaContext);`|
Decoding frames
For decoding frames, we need to feed the virtual pipeline with parameter and bit stream data so that it can decode the compressed video frames. There are several types of data to send:
- Some configuration data like inverse quantization matrix buffer, picture parameter buffer, slice buffer parameter or other data structure required for the different formats supported. This data parameterize
 the virtual pipeline before sending the actual data stream for decode.
- The bitstream data. It needs to be sent in a structured way so that the driver can interpret it and decode it correctly.
There is a unique data transfer mechanism that allows the client to pass both types of data to the driver.
Creating Buffer
The way to send parameter and bit stream data to the driver is through "Buffers". The buffer data store is managed by the library while the client identifies each buffer with a unique Id assigned by the driver.
There are two methods to set the contents of the buffers that hold either parameters or bit stream data. The first one actually copies the data to the driver data store. To do this you in need to invoke vaCreateBuffer with a non null "data" parameter. In that
 case, a memory space is allocated in the data store on the server side and the data is copied from into this memory space. This is the way it is used in the sample code provided:
|`01`|`  ``static````VAPictureParameterBufferMPEG2 pic_param={```|
|`02`|`  ``horizontal_size:16,`|
|`03`|`  ``vertical_size:16,`|
|`04`|`  ``forward_reference_picture:0xffffffff,`|
|`05`|`  ``backward_reference_picture:0xffffffff,`|
|`06`|`  ``picture_coding_type:1,`|
|`07`|`  ``f_code:0xffff,`|
|`08`|`  ``{`|
|`09`|`      ``{`|
|`10`|`        ``intra_dc_precision:0,`|
|`11`|`        ``picture_structure:3,`|
|`12`|`        ``top_field_first:0,`|
|`13`|`        ``frame_pred_frame_dct:1,`|
|`14`|`        ``concealment_motion_vectors:0,`|
|`15`|`        ``q_scale_type:0,`|
|`16`|`        ``intra_vlc_format:0,`|
|`17`|`        ``alternate_scan:0,`|
|`18`|`        ``repeat_first_field:0,`|
|`19`|`        ````progressive_frame:1 ,```|
|`20`|`        ``is_first_field:1`|
|`21`|`      ``},`|
|`22`|`  ``}`|
|`23`|`};`|
|`24`||
|`25`|```vaStatus = vaCreateBuffer(vaDisplay, vaContext,```|
|`26`|`                              ``VAPictureParameterBufferType,`|
|`27`|`                              ``sizeof``(VAPictureParameterBufferMPEG2),`|
|`28`|`                              ````1, &pic_param,```|
|`29`|`                              ``&vaPicParamBuf);`|
If you call it with a null "data" parameter, the buffer object is created but the memory space is not assigned in the data store. By invoking vaMapBuffer(), the client get access to the buffer address space in the data store. This prevents doing memory copies
 of data from the client to the server address space. The client can then fill the buffer with data. After the buffer is filled with data and before it is actually transferred to the virtual pipeline, it must be unmapped calling vaUnmapBuffer(). Find here a
 code example:
|`1`|```/* Create a picture parameter buffer for this frame */```|
|`2`|`        ````VABufferID picture_buf;```|
|`3`|`        ````VAPictureParameterBufferMPEG2 *picture_param;```|
|`4`|`        ````vaCreateBuffer(dpy, context, VAPictureParameterBufferType,````sizeof````(VAPictureParameterBufferMPEG2), 1, NULL, &picture_buf);```|
|`5`|`        ````vaMapBuffer(dpy, picture_buf, &picture_param);```|
|`6`|`        ````picture_param->horizontal_size = 720;```|
|`7`|`        ````picture_param->vertical_size = 480;```|
|`8`|`        ````picture_param->picture_coding_type = 1;``````/* I-frame */```|
|`9`|`        ````vaUnmapBuffer(dpy, picture_buf);```|
Sending the parameters and bitstream for decode
For decoding frames we need to send stream parameters first: the inverse quantization matrix buffer, the picture parameter buffer, the slice buffer parameter or other data structures required for the given format. Then the data stream can be sent to the virtual
 pipeline. This data is passed using the data transfer mechanism described in the previous chapter. The transfer of data is invoked through vaRenderPicture call.
For each frame to render, you need to go through a vaBeginPicture/vaRenderPicture/vaEndPicture sequence. In this sequence, once the necessary parameters like the inverse quantize matrix or the picture parameter buffer or any other parameter needed depending
 on the format, are set, the data stream can be sent to the driver for decoding. The decode buffers are sent to the virtual pipeline owing to vaRenderPicture calls. When all the data related to the frame are sent, the vaEndPicture() call makes the end of rendering
 for the picture. This is a non blocking call so the client can start another vaBeginPicture/vaRenderPicture/vaEndPicture sequence while the hardware is decoding the current frame that has been submitted. The vaPutSurface call will send the decode output surface
 to the X drawable. It performs a de-interlacing (if needed) color space conversion and scaling to the destination rectangle. Find here a code sample describing the decode sequence.
|`01`|```vaBeginPicture(vaDisplay, vaContext, vaSurface);```|
|`02`|```vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaPicParamBuf, 1);```|
|`03`|```vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaIQMatrixBuf, 1);```|
|`04`|```vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaSliceParamBuf, 1);```|
|`05`|```vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaSliceDataBuf, 1);```|
|`06`|`vaEndPicture(vaDisplay,vaContext);`|
|`07`||
|`08`|```vaStatus = vaSyncSurface(vaDisplay, vaContext, vaSurface);```|
|`09`||
|`10`|`if````(putsurface) {```|
|`11`|`    ````win = XCreateSimpleWindow(x11_display, RootWindow(x11_display, 0), 0, 0,```|
|`12`|`                          ````win_width,win_height, 0, 0, WhitePixel(x11_display, 0));```|
|`13`|`    ````XMapWindow(x11_display, win);```|
|`14`|`    ````XSync(x11_display, True);```|
|`15`|`    `|
|`16`|`    ````vaStatus = vaPutSurface(vaDisplay, vaSurface, win,```|
|`17`|`                            ``0,0,surf_width,surf_height,`|
|`18`|`                            ``0,0,win_width,win_height,`|
|`19`|`                            ``NULL,0,0);`|
|`20`|`}`|
Additional capabilities
The VA API provides also other capabilities than just decoding acceleration. It provides functions for
- client and library synchronization
- subpicture blending in the decoded video stream
- host based post-processing by retrieving image data from decoded surfaces.
You can get more details on these capabilities in going through the VA API specifications. The API, which is currently in the version 0.29, will evolve overtime adding incremental functionalities supported by future version of chipsets.
### Performance
Let's compare the performance of video playback on the current MID platforms. The first test compares the playback performance with Totem player on a Compal Jax* 10 MID platform. The Intel® GMA 500 chipset used in this MID is the UL11L. The Intel® AtomTM processor
 is the Z500 at 800 MHZ. In this test we will limit ourselves to SD content as the UL11L chipset is not supporting HD content decode.
![](https://software.intel.com/sites/default/files/m/d/4/1/d/8/mid-figure-5.jpg)
The measurements taken below are measuring the CPU usage of a full video playback including audio decode. The first measurement shows the system cpu usage when doing a playback on Totem player with the software FFmpeg codecs (no hardware acceleration). The
 second one shows the system CPU usage when doing a playback on RealPlayer for MID with the hardware accelerated codecs.
|Video format|resolution|fps|Max CPU usage Totem + FFmpeg codecs|Max CPU usage RealPlayer for MID with hardware accelerated codecs|
|----|----|----|----|----|
|MPEG-2|720x480|30|72%|39%|
|MPEG-4|720x480|22|50%|31%|
|H.264|640x360|60|100%|27.5%|
|VC-1|720x480|25|100%|31%|
The usage of the VA API allows the CPU usage to drop significantly when the hardware video decode is used reducing significantly the power drain on the battery. Note that when the CPU reaches 100% the system is not capable anymore to match the targeted frame
 rate. Indeed, the frame rate drops to a few frames per second, giving a pretty degraded experience.
In the second test we will use a platform with an Intel® AtomTM processor Z530 at 1.6GHZ and a US15W GMA 500 chipset. Unlike in the previous test, this version of chipset is capable of decoding HD content. The playback is done with the regular FFmpeg codecs
 (without hardware acceleration) and the second one with the Fluendo codecs using hardware acceleration through the VA API. Note here that we are only measuring pure video decode. There is no audio decode happening.
|Video format|resolution|fps|Max CPU usage FFmpeg codecs|Max CPU usage Fluendo video codecs with hardware acceleration|
|----|----|----|----|----|
|MPEG-2|480x576|25|22.8%|18%|
|MPEG-4|640x272|24|22.4%|10%|
|H.264|1280x544|30|100%|13%|
|VC-1|1280x720|25|100%|15.5%|
The playback has been activated here using gst-launch-0.10 tool with the following command line : gst-launch-0.10 filesrc location=<media file> ! decodebin ! queue ! xvimagesink. The system had Intel® Hyper-threading Technology disabled. When reaching 100%
 usage, the playback experience is significantly degraded as the encoded fps cannot be delivered by the system. It drops to a few frames per second making the experience pretty poor.
### Summary
As MIDs are becoming more and more widespread, video playback on this devices is seen as one of the major usage model especially as mobile TV and Video on Demand are really becoming popular. To be able to experience video playback in optimal conditions and
 to extend the battery life of the device, it is essential that the video players are using the hardware video decode capability provided in the platform. 
Independent software vendors (ISV) have the choice to build their players on top of multimedia frameworks optimized for such platforms as Helix or Gstreamer, or to implement this decoding using the standard public API: VA API. It's a tremendous opportunity
 to get into this new growing segment and bring outstanding video support to the handheld world.
### Additional Resources
ISVs that are considering using hardware acceleration will benefit from the following resources:
The VA API specifications are published on the freedesktop.org site, For more information, please visit:[http://www.freedesktop.org/wiki/Software/vaapi](http://www.freedesktop.org/wiki/Software/vaapi).
If you look for information on Fluendo codecs, go to [http://www.fluendo.com](http://www.fluendo.com/)
Information on RealPlayer for MID can be found at[https://helixcommunity.org/licenses/realplayer_for_mid_faq.html](https://helixcommunity.org/licenses/realplayer_for_mid_faq.html)
The sources of Mplayer using VA API provided by Splitted-Desktop Systems are available there:[http://www.splitted-desktop.com/en/libva/](http://www.splitted-desktop.com/en/libva/)
For software development on the MIDs, Intel® Developer Zone offers technical resources at:[http://software.intel.com/en-us/appup/](https://software.intel.com/en-us/articles/atom/all/1)
### About the Author
Philippe Michelon has a long history of software optimization on numerous Intel® architectures. Philippe works as Application Engineer in the Intel® Software and Services Group in Grenoble in France, with ISV- and service-enabling for Intel's new mobile client
 platforms. Currently his focus is on MIDs.
Philippe holds a M.S in Computational and Mathematical Engineering and can be reached at[philippe.michelon@intel.com](mailto:philippe.michelon@intel.com)
Greetings
Special thanks to Jonathan Bian and Sengquan Yuan for their contribution to this paper.
### Sample code
Sample code decoding a hardcoded mpeg2 stream with VA API
|`001`|`/*`|
|`002`|` ````* Copyright (c) 2007-2008 Intel Corporation. All Rights Reserved.```|
|`003`|` ``*`|
|`004`|` ````* Permission is hereby granted, free of charge, to any person obtaining a```|
|`005`|` ````* copy of this software and associated documentation files (the```|
|`006`|` ````* "Software"), to deal in the Software without restriction, including```|
|`007`|` ````* without limitation the rights to use, copy, modify, merge, publish,```|
|`008`|` ````* distribute, sub license, and/or sell copies of the Software, and to```|
|`009`|` ````* permit persons to whom the Software is furnished to do so, subject to```|
|`010`|` ````* the following conditions:```|
|`011`|` ``* `|
|`012`|` ````* The above copyright notice and this permission notice (including the```|
|`013`|` ````* next paragraph) shall be included in all copies or substantial portions```|
|`014`|` ````* of the Software.```|
|`015`|` ``* `|
|`016`|` ````* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS```|
|`017`|` ````* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF```|
|`018`|` ````* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.```|
|`019`|` ````* IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR```|
|`020`|` ````* ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,```|
|`021`|` ````* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE```|
|`022`|` ````* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.```|
|`023`|` ``*/`|
|`024`|`/*`|
|`025`|` ````* This is a program to show how VAAPI work,```|
|`026`|` ````* It does VLD decode for a simple MPEG2 clip```|
|`027`|` ````* The clip and VA parameters are hardcoded into mpeg2vld-demo.c```|
|`028`|` ``*`|
|`029`|` ````* gcc -o  mpeg2vld-demo  mpeg2vld-demo.c -lva```|
|`030`|` ````* ./mpeg2vld-demo  : only do decode```|
|`031`|` ````* ./mpeg2vld-demo 1: decode+display```|
|`032`|` ``*`|
|`033`|` ``*/`|
|`034`|```#include <stdio.h>```|
|`035`|```#include <string.h>```|
|`036`|```#include <stdlib.h>```|
|`037`|```#include <getopt.h>```|
|`038`|```#include <X11/Xlib.h>```|
|`039`||
|`040`|```#include <unistd.h>```|
|`041`||
|`042`|```#include <sys/types.h>```|
|`043`|```#include <sys/stat.h>```|
|`044`|```#include <fcntl.h>```|
|`045`||
|`046`|```#include <assert.h>```|
|`047`||
|`048`|```#include "va.h"```|
|`049`|```#include "va_x11.h"```|
|`050`||
|`051`|```/* Data dump of a 16x16 MPEG2 video clip,it has one I frame```|
|`052`|` ``*/`|
|`053`|`static``unsigned ``char``mpeg2_clip[]={`|
|`054`|`    ``0x00,0x00,0x01,0xb3,0x01,0x00,0x10,0x13,0xff,0xff,0xe0,0x18,0x00,0x00,0x01,0xb5,`|
|`055`|`    ``0x14,0x8a,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0xb8,0x00,0x08,0x00,0x00,0x00,0x00,`|
|`056`|`    ``0x01,0x00,0x00,0x0f,0xff,0xf8,0x00,0x00,0x01,0xb5,0x8f,0xff,0xf3,0x41,0x80,0x00,`|
|`057`|`    ``0x00,0x01,0x01,0x13,0xe1,0x00,0x15,0x81,0x54,0xe0,0x2a,0x05,0x43,0x00,0x2d,0x60,`|
|`058`|`    ``0x18,0x01,0x4e,0x82,0xb9,0x58,0xb1,0x83,0x49,0xa4,0xa0,0x2e,0x05,0x80,0x4b,0x7a,`|
|`059`|`    ``0x00,0x01,0x38,0x20,0x80,0xe8,0x05,0xff,0x60,0x18,0xe0,0x1d,0x80,0x98,0x01,0xf8,`|
|`060`|`    ``0x06,0x00,0x54,0x02,0xc0,0x18,0x14,0x03,0xb2,0x92,0x80,0xc0,0x18,0x94,0x42,0x2c,`|
|`061`|`    ``0xb2,0x11,0x64,0xa0,0x12,0x5e,0x78,0x03,0x3c,0x01,0x80,0x0e,0x80,0x18,0x80,0x6b,`|
|`062`|`    ``0xca,0x4e,0x01,0x0f,0xe4,0x32,0xc9,0xbf,0x01,0x42,0x69,0x43,0x50,0x4b,0x01,0xc9,`|
|`063`|`    ``0x45,0x80,0x50,0x01,0x38,0x65,0xe8,0x01,0x03,0xf3,0xc0,0x76,0x00,0xe0,0x03,0x20,`|
|`064`|`    ``0x28,0x18,0x01,0xa9,0x34,0x04,0xc5,0xe0,0x0b,0x0b,0x04,0x20,0x06,0xc0,0x89,0xff,`|
|`065`|`    ``0x60,0x12,0x12,0x8a,0x2c,0x34,0x11,0xff,0xf6,0xe2,0x40,0xc0,0x30,0x1b,0x7a,0x01,`|
|`066`|`    ``0xa9,0x0d,0x00,0xac,0x64`|
|`067`|`};`|
|`068`||
|`069`|```/* hardcoded here without a bitstream parser helper```|
|`070`|` ````* please see picture mpeg2-I.jpg for bitstream details```|
|`071`|` ``*/`|
|`072`|`static````VAPictureParameterBufferMPEG2 pic_param={```|
|`073`|`  ``horizontal_size:16,`|
|`074`|`  ``vertical_size:16,`|
|`075`|`  ``forward_reference_picture:0xffffffff,`|
|`076`|`  ``backward_reference_picture:0xffffffff,`|
|`077`|`  ``picture_coding_type:1,`|
|`078`|`  ``f_code:0xffff,`|
|`079`|`  ``{`|
|`080`|`      ``{`|
|`081`|`        ``intra_dc_precision:0,`|
|`082`|`        ``picture_structure:3,`|
|`083`|`        ``top_field_first:0,`|
|`084`|`        ``frame_pred_frame_dct:1,`|
|`085`|`        ``concealment_motion_vectors:0,`|
|`086`|`        ``q_scale_type:0,`|
|`087`|`        ``intra_vlc_format:0,`|
|`088`|`        ``alternate_scan:0,`|
|`089`|`        ``repeat_first_field:0,`|
|`090`|`        ````progressive_frame:1 ,```|
|`091`|`        ``is_first_field:1`|
|`092`|`      ``},`|
|`093`|`  ``}`|
|`094`|`};`|
|`095`||
|`096`|```/* see MPEG2 spec65 for the defines of matrix */```|
|`097`|`static````VAIQMatrixBufferMPEG2 iq_matrix = {```|
|`098`|`  ``load_intra_quantiser_matrix:1,`|
|`099`|`  ``load_non_intra_quantiser_matrix:1,`|
|`100`|`  ``load_chroma_intra_quantiser_matrix:0,`|
|`101`|`  ``load_chroma_non_intra_quantiser_matrix:0,`|
|`102`|`  ``intra_quantiser_matrix:{`|
|`103`|`         ````8, 16, 16, 19, 16, 19, 22, 22,```|
|`104`|`        ````22, 22, 22, 22, 26, 24, 26, 27,```|
|`105`|`        ````27, 27, 26, 26, 26, 26, 27, 27,```|
|`106`|`        ````27, 29, 29, 29, 34, 34, 34, 29,```|
|`107`|`        ````29, 29, 27, 27, 29, 29, 32, 32,```|
|`108`|`        ````34, 34, 37, 38, 37, 35, 35, 34,```|
|`109`|`        ````35, 38, 38, 40, 40, 40, 48, 48,```|
|`110`|`        ````46, 46, 56, 56, 58, 69, 69, 83```|
|`111`|`    ``},`|
|`112`|`  ``non_intra_quantiser_matrix:{16},`|
|`113`|`  ``chroma_intra_quantiser_matrix:{0},`|
|`114`|`  ``chroma_non_intra_quantiser_matrix:{0}`|
|`115`|`};`|
|`116`||
|`117`|`static````VASliceParameterBufferMPEG2 slice_param={```|
|`118`|`  ``slice_data_size:150,`|
|`119`|`  ``slice_data_offset:0,`|
|`120`|`  ``slice_data_flag:0,`|
|`121`|`  ``macroblock_offset:38,````/* 4byte + 6bits=38bits */```|
|`122`|`  ``slice_vertical_position:0,`|
|`123`|`  ``quantiser_scale_code:2,`|
|`124`|`  ``intra_slice_flag:0`|
|`125`|`};`|
|`126`||
|`127`|```#define CLIP_WIDTH  16```|
|`128`|```#define CLIP_HEIGHT 16```|
|`129`||
|`130`|`int``surf_width=CLIP_WIDTH,surf_height=CLIP_HEIGHT;`|
|`131`|`int``win_width=CLIP_WIDTH<<1,win_height=CLIP_HEIGHT<<1;`|
|`132`||
|`133`|`int``main(``int``argc,``char``**argv)`|
|`134`|`{`|
|`135`|`    ````VAEntrypoint entrypoints[5];```|
|`136`|`    ``int``num_entrypoints,vld_entrypoint;`|
|`137`|`    ````VAConfigAttrib attrib;```|
|`138`|`    ````VAConfigID config_id;```|
|`139`|`    ````VASurfaceID vaSurface;```|
|`140`|`    ````VAContextID vaContext;```|
|`141`|`    ````VABufferID vaPicParamBuf,vaIQMatrixBuf,vaSliceParamBuf,vaSliceDataBuf;```|
|`142`|`    ``int````major_ver, minor_ver;```|
|`143`|`    ````Display *x11_display;```|
|`144`|`    ````VADisplay   vaDisplay;```|
|`145`|`    ````VAStatus vaStatus;```|
|`146`|`    ````Window win = 0;```|
|`147`|`    ``int``putsurface=0;`|
|`148`||
|`149`|`    ``if````(argc > 1)```|
|`150`|`        ``putsurface=1;`|
|`151`|` `|
|`152`|`    ````x11_display = XOpenDisplay(NULL);```|
|`153`|`    ````vaDisplay = vaGetDisplay(x11_display);```|
|`154`|`    ````vaStatus = vaInitialize(vaDisplay, &major_ver, &minor_ver);```|
|`155`||
|`156`|`    ````vaQueryConfigEntrypoints(vaDisplay, VAProfileMPEG2Main, entrypoints,```|
|`157`|`                             ``&num_entrypoints);`|
|`158`||
|`159`|`    ``for````(vld_entrypoint = 0; vld_entrypoint < num_entrypoints; vld_entrypoint++) {```|
|`160`|`        ``if````(entrypoints[vld_entrypoint] == VAEntrypointVLD)```|
|`161`|`            ``break``;`|
|`162`|`    ``}`|
|`163`|`    ``if````(vld_entrypoint == num_entrypoints) {```|
|`164`|`        ````/* not find VLD entry point */```|
|`165`|`        ``exit``(-1);`|
|`166`|`    ``}`|
|`167`||
|`168`|`    ````/* Assuming finding VLD, find out the format for the render target */```|
|`169`|`    ````attrib.type = VAConfigAttribRTFormat;```|
|`170`|`    ````vaGetConfigAttributes(vaDisplay, VAProfileMPEG2Main, VAEntrypointVLD,```|
|`171`|`                          ````&attrib, 1);```|
|`172`|`    ``if````((attrib.value & VA_RT_FORMAT_YUV420) == 0) {```|
|`173`|`        ````/* not find desired YUV420 RT format */```|
|`174`|`        ``exit``(-1);`|
|`175`|`    ``}`|
|`176`|`    `|
|`177`|`    ````vaStatus = vaCreateConfig(vaDisplay, VAProfileMPEG2Main, VAEntrypointVLD,```|
|`178`|`                              ````&attrib, 1,&config_id);```|
|`179`|`    `|
|`180`|`    ````vaStatus = vaCreateSurfaces(vaDisplay,surf_width,surf_height,```|
|`181`|`                                ````VA_RT_FORMAT_YUV420, 1, &vaSurface);```|
|`182`||
|`183`|`    ````/* Create a context for this decode pipe */```|
|`184`|`    ````vaStatus = vaCreateContext(vaDisplay, config_id,```|
|`185`|`                               ``CLIP_WIDTH,`|
|`186`|`                               ``((CLIP_HEIGHT+15)/16)*16,`|
|`187`|`                               ``VA_PROGRESSIVE,`|
|`188`|`                               ``&vaSurface,`|
|`189`|`                               ``1,`|
|`190`|`                               ``&vaContext);`|
|`191`|`    `|
|`192`|`    ````vaStatus = vaCreateBuffer(vaDisplay, vaContext,```|
|`193`|`                              ``VAPictureParameterBufferType,`|
|`194`|`                              ``sizeof``(VAPictureParameterBufferMPEG2),`|
|`195`|`                              ````1, &pic_param,```|
|`196`|`                              ``&vaPicParamBuf);`|
|`197`|`    ````vaStatus = vaCreateBuffer(vaDisplay, vaContext,```|
|`198`|`                              ``VAIQMatrixBufferType,`|
|`199`|`                              ``sizeof``(VAIQMatrixBufferMPEG2),`|
|`200`|`                              ````1, &iq_matrix,```|
|`201`|`                              ````&vaIQMatrixBuf );```|
|`202`|`                `|
|`203`|`    ````vaStatus = vaCreateBuffer(vaDisplay, vaContext,```|
|`204`|`                              ``VASliceParameterBufferType,`|
|`205`|`                              ``sizeof``(VASliceParameterBufferMPEG2),`|
|`206`|`                              ``1,`|
|`207`|`                              ````&slice_param, &vaSliceParamBuf);```|
|`208`||
|`209`|`    ````vaStatus = vaCreateBuffer(vaDisplay, vaContext,```|
|`210`|`                              ``VASliceDataBufferType,`|
|`211`|`                              ``0xc4-0x2f+1,`|
|`212`|`                              ``1,`|
|`213`|`                              ``mpeg2_clip+0x2f,`|
|`214`|`                              ``&vaSliceDataBuf);`|
|`215`||
|`216`|`    ````vaBeginPicture(vaDisplay, vaContext, vaSurface);```|
|`217`|`    ````vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaPicParamBuf, 1);```|
|`218`|`    ````vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaIQMatrixBuf, 1);```|
|`219`|`    ````vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaSliceParamBuf, 1);```|
|`220`|`    ````vaStatus = vaRenderPicture(vaDisplay,vaContext, &vaSliceDataBuf, 1);```|
|`221`|`    ``vaEndPicture(vaDisplay,vaContext);`|
|`222`||
|`223`|`    ````vaStatus = vaSyncSurface(vaDisplay, vaContext, vaSurface);```|
|`224`|`    `|
|`225`|`    ``if````(putsurface) {```|
|`226`|`        ````win = XCreateSimpleWindow(x11_display, RootWindow(x11_display, 0), 0, 0,```|
|`227`|`                              ````win_width,win_height, 0, 0, WhitePixel(x11_display, 0));```|
|`228`|`        ````XMapWindow(x11_display, win);```|
|`229`|`        ````XSync(x11_display, True);```|
|`230`|`        `|
|`231`|`        ````vaStatus = vaPutSurface(vaDisplay, vaSurface, win,```|
|`232`|`                                ``0,0,surf_width,surf_height,`|
|`233`|`                                ``0,0,win_width,win_height,`|
|`234`|`                                ``NULL,0,0);`|
|`235`|`    ``}`|
|`236`||
|`237`|`    ``printf``(````"press any key to exitn"````);`|
|`238`|`    ``getchar``();`|
|`239`||
|`240`|`    ``vaDestroySurfaces(vaDisplay,&vaSurface,1);`|
|`241`|`    ``vaDestroyConfig(vaDisplay,config_id);`|
|`242`|`    ``vaDestroyContext(vaDisplay,vaContext);`|
|`243`|`    ``vaTerminate(vaDisplay);`|
|`244`|`    `|
|`245`|`    ``return``0;`|
|`246`|`}`|
