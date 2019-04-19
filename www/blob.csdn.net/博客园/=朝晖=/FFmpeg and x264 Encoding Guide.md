# FFmpeg and x264 Encoding Guide - =朝晖= - 博客园
# [FFmpeg and x264 Encoding Guide](https://www.cnblogs.com/dhcn/p/7130474.html)
https://trac.ffmpeg.org/wiki/Encode/H.264
# FFmpeg and H.264 Encoding Guide
#### Contents
- [Constant Rate Factor (CRF)](https://trac.ffmpeg.org/wiki/Encode/H.264#crf)
- [Two-Pass](https://trac.ffmpeg.org/wiki/Encode/H.264#twopass)
- [Lossless H.264](https://trac.ffmpeg.org/wiki/Encode/H.264#LosslessH.264)
- [Overwriting default preset settings](https://trac.ffmpeg.org/wiki/Encode/H.264#Overwritingdefaultpresetsettings)
- [Additional Information & Tips](https://trac.ffmpeg.org/wiki/Encode/H.264#AdditionalInformationTips)
- [FAQ](https://trac.ffmpeg.org/wiki/Encode/H.264#FAQ)
- [Additional Resources](https://trac.ffmpeg.org/wiki/Encode/H.264#AdditionalResources)
The goal of this guide is to inform new users how to create a high-quality H.264 video using the encoder x264.
There are two rate control modes that are usually suggested for general use: [Constant Rate Factor (CRF)](https://trac.ffmpeg.org/wiki/Encode/H.264#crf) or [Two-Pass ABR](https://trac.ffmpeg.org/wiki/Encode/H.264#twopass). The rate control is a method that will decide how many bits will be used for each frame. This will determine the file size and also how quality is distributed.
If you need help compiling and installing libx264 see one of our [FFmpeg and x264 compiling guides](https://trac.ffmpeg.org/wiki/CompilationGuide).
## Constant Rate Factor (CRF)
This method allows the encoder to attempt to achieve a certain output quality for the whole file when output file size is of less importance. This provides maximum compression efficiency with a single pass. Each frame gets the bitrate it needs to keep the requested quality level. The downside is that you can't tell it to get a specific filesize or not [Go](http://lib.csdn.net/base/go) over a specific size or bitrate.
### 1. Choose a CRF value
The range of the quantizer scale is 0-51: where 0 is lossless, 23 is default, and 51 is worst possible. A lower value is a higher quality and a subjectively sane range is 18-28. Consider 18 to be visually lossless or nearly so: it should look the same or nearly the same as the input but it isn't technically lossless.
The range is exponential, so increasing the CRF value +6 is roughly half the bitrate while -6 is roughly twice the bitrate. General usage is to choose the highest CRF value that still provides an acceptable quality. If the output looks good, then try a higher value and if it looks bad then choose a lower value.
**Note:** The CRF quantizer scale mentioned on this page only applies to 8-bit x264 (10-bit x264 quantizer scale is 0-63). You can see what you are using by referring to the ffmpeg console output during encoding (yuv420p or similar for 8-bit, and yuv420p10le or similar for 10-bit). 8-bit is more common among distributors.
### 2. Choose a preset
A preset is a collection of options that will provide a certain encoding speed to compression ratio. A slower preset will provide better compression (compression is quality per filesize). This means that, for example, if you target a certain file size or constant bit rate, you will achieve better quality with a slower preset. Similarly, for constant quality encoding, you will simply save bitrate by choosing a slower preset.
The general guideline is to use the slowest preset that you have patience for. Current presets in descending order of speed are: ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo. The default preset is medium. Ignore placebo as it is not useful (see [FAQ](https://trac.ffmpeg.org/wiki/Encode/H.264#FAQ)). You can see a list of current presets with -preset help (see example below), and what settings they apply with x264 --fullhelp.
You can optionally use -tune to change settings based upon the specifics of your input. Current tunings include: film, animation, grain, stillimage, psnr, ssim, fastdecode, zerolatency. For example, if your input is animation then use the animation tuning, or if you want to preserve grain then use the grain tuning. If you are unsure of what to use or your input does not match any of tunings then omit the -tune option. You can see a list of current tunings with -tune help, and what settings they apply with x264 --fullhelp.
Another optional setting is -profile:v which will limit the output to a specific H.264 profile. This can generally be omitted unless the target device only supports a certain profile (see [Compatibility](https://trac.ffmpeg.org/wiki/Encode/H.264#Compatibility)). Current profiles include: baseline, main, high, high10, high422, high444. Note that usage of -profile:v is incompatible with lossless encoding.
To list all possible internal preset and tunes:
ffmpeg -f lavfi -i nullsrc -c:v libx264 -preset help -f mp4 -
**Note:** Windows users may need to use NUL instead of - as the output.
### 3. Use your settings
Once you've chosen your settings apply them for the rest of your videos if you are encoding more. This will ensure that they will all have similar quality.
### CRF Example
ffmpeg -i input -c:v libx264 -preset slow -crf 22 -c:a copy output.mkv
Note that in this example the audio stream of the input file is simply [​stream copied](http://ffmpeg.org/ffmpeg.html#Stream-copy) over to the output and not re-encoded.
## Two-Pass
This method is generally used if you are targeting a specific output file size and output quality from frame to frame is of less importance. This is best explained with an example. Your video is 10 minutes (600 seconds) long and an output of 50 MB is desired. Since bitrate = file size / duration:
(50 MB * 8192 [converts MB to kilobits]) / 600 seconds = ~683 kilobits/s total bitrate
683k - 128k (desired audio bitrate) = 555k video bitrate
### Two-Pass Example
ffmpeg -y -i input -c:v libx264 -preset medium -b:v 555k -pass 1 -c:a aac -b:a 128k -f mp4 /dev/null && \
ffmpeg -i input -c:v libx264 -preset medium -b:v 555k -pass 2 -c:a aac -b:a 128k output.mp4
**Note:** Windows users should use NUL instead of /dev/null.
As with CRF, choose the slowest preset you can tolerate.
In pass 1 specify a output format with -f that matches the output format in pass 2. Also in pass 1, specify the audio codec used in pass 2; in many cases -an in pass 1 will not work.
See [​Making a high quality MPEG-4 ("DivX") rip of a DVD movie](http://www.mplayerhq.hu/DOCS/HTML/en/menc-feat-dvd-mpeg4.html). It is an MEncoder guide, but it will give you an insight about how important it is to use two-pass when you want to efficiently use every bit when you're constrained with storage space.
## Lossless H.264
You can use -crf 0 to encode a lossless output. Two useful presets for this are ultrafast or veryslow since either a fast encoding speed or best compression are usually the most important factors. Most non-FFmpeg based players will not be able to decode lossless (but YouTube can), so if compatibility is an issue you should not use lossless.
Note that lossless output files will likely be huge.
### Lossless Example (fastest encoding)
ffmpeg -i input -c:v libx264 -preset ultrafast -crf 0 output.mkv
### Lossless Example (best compression)
ffmpeg -i input -c:v libx264 -preset veryslow -crf 0 output.mkv
## Overwriting default preset settings
You can overwrite default preset settings with the x264opts option, the x264-params option, or by using the libx264 private options (see ffmpeg -h encoder=libx264). This is not recommended unless you know what you are doing. The presets were created by the x264 developers and tweaking values to get a better output is usually a waste of time.
Example:
ffmpeg -i input -c:v libx264 -preset slow -crf 22 -x264opts keyint=123:min-keyint=20 -c:a copy output.mkv
## Additional Information & Tips
### CBR (Constant Bit Rate)
There is no native CBR mode, but you can "simulate" a constant bit rate setting by tuning the parameters of ABR:
ffmpeg -i input -c:v libx264 -b:v 4000k -minrate 4000k -maxrate 4000k -bufsize 1835k out.m2v
In the above example, -bufsize is the "rate control buffer" so it will enforce your requested "average" (4000k in this case) across each 1835k worth of video. So basically it is assumed that the receiver/end player will buffer that much data so it's ok to fluctuate within that much.
Of course, if it's all just empty/black frames then it will still serve less than that many bits/s but it will raise the quality level as much as it can, up to the crf level.
### CRF with maximum bit rate
You can also also use -crf with a maximum bit rate by specifying both -crf and -maxrate:
ffmpeg -i input -c:v libx264 -crf 20 -maxrate 400k -bufsize 1835k output.mp4
This will effectively "target" -crf 20, but if the output exceeds 400kb/s, it will degrade to something less than -crf 20 in that case.
### Low Latency
libx264 offers a -tune zerolatency option. See the [StreamingGuide](https://trac.ffmpeg.org/wiki/StreamingGuide).
### Compatibility
#### All devices
If you want your videos to have highest compatibility with older devices:
-profile:v baseline -level 3.0
This disables some advanced features but provides for better compatibility. Typically you may not need this setting (and therefore avoid using -profile:v and -level), but if you do use this setting it may increase the bit rate compared to what is needed to achieve the same quality in higher profiles.
#### iOS
High4.2iPad Air and later, iPhone 5s and later-profile:v high -level 4.2
**Note:** This table does not include any additional restrictions which may be required by your device.
#### QuickTime
QuickTime only supports YUV planar color space with 4:2:0 chroma subsampling (use -vf format=yuv420p or -pix_fmt yuv420p) for H.264 video.
#### Blu-ray
See [​Authoring a professional Blu-ray Disc with x264](http://www.x264bluray.com/).
### Pre-testing your settings
Encode a random section instead of the whole video with the -ss and -t/-to options to quickly get a general idea of what the output will look like.
- -ss: Offset time from beginning. Value can be in seconds or HH:MM:SS format.
- -t: Duration. Value can be in seconds or HH:MM:SS format.
- -to: Stop writing the output at specified position. Value can be in seconds or HH:MM:SS format.
### faststart for web video
You can add -movflags +faststart as an output option if your videos are going to be viewed in a browser. This will move some information to the beginning of your file and allow the video to begin playing before it is completely downloaded by the viewer. It is not required if you are going to use a video service such as YouTube.
### Custom preset file
Refer to the -vpre output option in the [documentation](https://ffmpeg.org/ffmpeg.html#Preset-files).
## FAQ
### Will two-pass provide a better quality than CRF?[¶](https://trac.ffmpeg.org/wiki/Encode/H.264#Willtwo-passprovideabetterqualitythanCRF)
[​No](http://web.archive.org/web/20140206073211/http://doom10.org/index.php?PHPSESSID=okj08qe73ictdtv532augv8nu7&topic=267.msg2071#msg2071), though it does allow you to target a file size more accurately.
### Why is placebo a waste of time?
It helps at most ~1% compared to the veryslow preset at the cost of a much higher encoding time. It's diminishing returns: veryslow helps about 3% compared to the slowerpreset, slower helps about 5% compared to the slow preset, and slow helps about 5-10% compared to the medium preset.
### Why doesn't my lossless output look lossless?
Blame the RGB to YUV color space conversion. If you convert to yuv444 it should still be lossless (which is the default now).
### Will a graphics card make x264 encode faster?
For x264 specifically, probably not. [​x264 supports OpenCL](http://git.videolan.org/?p=x264.git;a=commit;h=3a5f6c0aeacfcb21e7853ab4879f23ec8ae5e042) for some lookahead operations.
See [HWAccelIntro](https://trac.ffmpeg.org/wiki/HWAccelIntro) for information on supported hardware based H.264 encoders and decoders.
### Encoding for dumb players
You may need to use -vf format=yuv420p (or the alias -pix_fmt yuv420p) for your output to work in QuickTime and most other players. These players only supports the YUV planar color space with 4:2:0 chroma subsampling for H.264 video. Otherwise, depending on your source, ffmpeg may output to a pixel format that may be incompatible with these players.

