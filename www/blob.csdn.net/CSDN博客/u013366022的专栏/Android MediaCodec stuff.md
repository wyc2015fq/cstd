# Android MediaCodec stuff - u013366022的专栏 - CSDN博客
2014年05月14日 15:28:17[slitaz](https://me.csdn.net/u013366022)阅读数：3708
# Android MediaCodec stuffLast updated: 2014-04-12
- [Overview](http://bigflake.com/mediacodec/#overview)
- [Samples](http://bigflake.com/mediacodec/#samples)
- [FAQ](http://bigflake.com/mediacodec/#faq)
This page is about the Android [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html) class,
 which can be used to encode and decode audio and video data. It includes a collection of sample code and answers to frequently-asked questions.
## Overview
The `MediaCodec` class first became available in Android 4.1 (API 16). It was added to allow direct access to the media codecs on the device.
 As such, it provides a rather "raw" interface. While the`MediaCodec` class exists in both Java and C++ sources, only the former is public.
In Android 4.3 (API 18), `MediaCodec` was expanded to include a way to provide input through a `Surface` (via the `createInputSurface` method). This allows input to come from camera
 preview or OpenGL ES rendering. Android 4.3 was also the first release with `MediaCodec` tests in [CTS](http://source.android.com/compatibility/cts-intro.html), which helps ensure consistent behavior between devices.
Android 4.3 also introduced [MediaMuxer](http://developer.android.com/reference/android/media/MediaMuxer.html), which allows the output of the AVC codec (a raw H.264 elementary stream)
 to be converted to .MP4 format, with or without an associated audio stream.
#### Basic Usage
All uses of `MediaCodec` follow a basic pattern:- create and configure the `MediaCodec` object
- loop until done:
- if an input buffer is ready:
- read a chunk of input, copy it into the buffer
- if an output buffer is ready:
- copy the output from the buffer
- release `MediaCodec` object
A single instance of `MediaCodec` handles one specific type of data (e.g. MP3 audio or H.264 video), and may encode or decode. It operates on "raw" data, so any file headers (e.g. ID3 tags) must be stripped
 off. It does not talk to any higher-level system components; it will not play your audio through out the speaker or receive a stream of video over a network. It just takes buffers of data in and spits buffers of data out. (You can use [MediaExtractor](http://developer.android.com/reference/android/media/MediaExtractor.html) to
 strip the wrappers off in most situations.)
Some codecs are very particular about their buffers. They may need to have a particular memory alignment, or have a certain minimum or maximum size, or it may be important to have a certain number of them available.
 To accommodate the wide range of possibilities, buffer allocation is performed by the codecs themselves, rather than the application. You do not hand a buffer with data to`MediaCodec`. You ask it for a buffer, and if one is available, you copy the
 data in.
This may seem contrary to "zero-copy" principles, but in most cases there will be fewer copies because the codec won't have to copy or adjust the data to meet its requirements. In some cases you may be able to use
 the buffer directly, e.g. read data from disk or network directly into the buffer, so a copy won't be necessary.
Input to `MediaCodec` must be done in "access units". When encoding H.264 video that means one frame, when decoding that means a single NAL unit. However, it behaves like a stream in the sense that you
 can't submit a single chunk and expect a chunk to appear shortly from the output. In practice, the codec may want to have several buffers queued up before producing any output.
It is strongly recommended that you start with sample code, rather than trying to figure it out from the documentation.
## Samples
**[EncodeAndMuxTest.java](http://bigflake.com/mediacodec/EncodeAndMuxTest.java.txt)** (requires 4.3, API 18)
> Generates a movie using OpenGL ES. Uses [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html) to encode the movie in an H.264 elementary stream, and [MediaMuxer](http://developer.android.com/reference/android/media/MediaMuxer.html) to
 convert the stream to a .MP4 file.
This was written as if it were a CTS test, but is not part of CTS. It should be straightforward to adapt the code to other environments.
**[CameraToMpegTest.java](http://bigflake.com/mediacodec/CameraToMpegTest.java.txt)** (requires 4.3, API 18)
> Records video from the camera preview and encodes it as an MP4 file. Uses [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html) to encode the movie
 in an H.264 elementary stream, and [MediaMuxer](http://developer.android.com/reference/android/media/MediaMuxer.html) to convert the stream to a .MP4 file. As an added bonus, demonstrates the use of a GLES fragment shader to modify
 the video as it's being recorded.
This was written as if it were a CTS test, but is not part of CTS. It should be straightforward to adapt the code to other environments.
**[Android Breakout game recorder patch](http://bigflake.com/mediacodec/0001-Record-game-into-.mp4.patch)** (requires 4.3, API
 18)
> This is a patch for [Android Breakout](https://code.google.com/p/android-breakout/) v1.0.2 that adds game recording. While the game is playing at 60fps at full screen resolution,
 a 30fps 720p recording is made with the AVC codec. The file is saved in the app-private data area, e.g. `/data/data/com.faddensoft.breakout/files/video.mp4`.
This is essentially the same as `EncodeAndMuxTest.java`, but it's part of a full app rather than an isolated CTS test. One key difference is in the EGL setup, which is done in a way that allows textures to be shared between the display and video
 contexts.
Another approach is to render each game frame to an FBO texture, and then render a full-screen quad with that twice (once for the display, once for the video). This could be faster for games that are expensive to render. Examples of both approaches can be
 found in [Grafika's RecordFBOActivity](https://github.com/google/grafika) class.
**[EncodeDecodeTest.java](https://android.googlesource.com/platform/cts/+/jb-mr2-release/tests/tests/media/src/android/media/cts/EncodeDecodeTest.java)** (requires
 4.3, API 18)
> CTS test. There are three tests that do essentially the same thing, but in different ways. Each test will:
- Generate video frames
- Encode frames with AVC codec
- Decode generated stream
- Test decoded frames to see if they match the original
The generation, encoding, decoding, and checking are near-simultaneous: frames are generated and fed to the encoder, and data from the encoder is fed to the decoder as soon as it becomes available.
The three tests are:
- Buffer-to-buffer. Buffers are software-generated YUV frames in `ByteBuffer` objects, and decoded to the same. This is the slowest (and least portable) approach, but it allows the application to examine and modify the YUV data.
- Buffer-to-surface. Encoding is again done from software-generated YUV data in `ByteBuffer`s, but this time decoding is done to a `Surface`. Output is checked with OpenGL ES, using`glReadPixels()`.
- Surface-to-surface. Frames are generated with OpenGL ES onto an input `Surface`, and decoded onto a `Surface`. This is the fastest approach, but may involve conversions between YUV and RGB.
Each test is run at three different resolutions: 720p (1280x720), QCIF (176x144), and QVGA (320x240).
The buffer-to-buffer and buffer-to-surface tests can be built with Android 4.1 (API 16). However, because the CTS tests did not exist until Android 4.3, a number of devices shipped with broken implementations.
NOTE: the `setByteBuffer()` usage may not be strictly correct, as it doesn't set "csd-1".
**[DecodeEditEncodeTest.java](https://android.googlesource.com/platform/cts/+/jb-mr2-release/tests/tests/media/src/android/media/cts/DecodeEditEncodeTest.java)** (requires
 4.3, API 18)
> CTS test. The test does the following:
- Generate a series of video frames, and encode them with AVC. The encoded data stream is held in memory.
- Decode the generated stream with `MediaCodec`, using an output `Surface`.
- Edit the frame (swap green/blue color channels) with an OpenGL ES fragment shader.
- Encode the frame with `MediaCodec`, using an input `Surface`.
- Decode the edited video stream, verifying the output.
The middle decode-edit-encode pass performs decoding and encoding near-simultaneously, streaming frames directly from the decoder to the encoder. The initial generation and final verification are done as separate passes on video data held in memory.
Each test is run at three different resolutions: 720p (1280x720), QCIF (176x144), and QVGA (320x240).
No software-interpreted YUV buffers are used. Everything goes through `Surface`. There will be conversions between RGB and YUV at certain points; how many and where they happen depends on how the drivers are implemented.
**[ExtractMpegFramesTest.java](http://bigflake.com/mediacodec/ExtractMpegFramesTest.java.txt)** (requires 4.1, API 16) 
**[ExtractMpegFramesTest.java](http://bigflake.com/mediacodec/ExtractMpegFramesTest_egl14.java.txt)** (requires 4.2, API 17)
> Extracts the first 10 frames of video from a .mp4 file and saves them to individual PNG files in `/sdcard/`. Uses [MediaExtractor](http://developer.android.com/reference/android/media/MediaExtractor.html) to
 extract the CSD data and feed individual access units into a [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html) decoder. The frames are decoded to a `Surface` created from [SurfaceTexture](http://developer.android.com/reference/android/graphics/SurfaceTexture.html),
 rendered (off-screen) into a pbuffer, extracted with `glReadPixels()`, and saved to a PNG file with `Bitmap#compress()`.
Decoding the frame and copying it into a `ByteBuffer` with `glReadPixels()` takes about 8ms on the Nexus 5, easily fast enough to keep pace with 30fps input, but the additional steps required to save it to disk as a PNG are expensive
 (about half a second). The cost of saving a frame breaks down roughly like this (which you can get by modifying the test to extract full-size frames from 720p video on a Nexus 5, observing the total time required to save 10 frames, and doing successive runs
 with later stages removed; or by instrumenting with [android.os.Trace](http://developer.android.com/reference/android/os/Trace.html)and using [systrace](http://bigflake.com/systrace/)):
- 0.5% video decode (hardware AVC codec)
- 1.5% `glReadPixels()` into a direct `ByteBuffer`
- 1.5% copying pixel data from `ByteBuffer` to `Bitmap`
- 96.5% PNG compression and file I/O
In theory, a `Surface` from the API 19 [ImageReader](http://developer.android.com/reference/android/media/ImageReader.html) class could be passed to the `MediaCodec` decoder, allowing direct access to the
 YUV data. As of Android 4.4, the `MediaCodec` decoder formats are not supported by `ImageReader`.
One possible way to speed up transfer of RGB data would be to copy the data asynchronously through a [PBO](http://www.opengl.org/wiki/Pixel_Buffer_Object), but in the current implementation the transfer time is dwarfed by the subsequent
 PNG activity, so there's little value in doing so.
The two versions of the source code function identically. One was written against EGL 1.0, the other EGL 1.4. EGL 1.4 is a little easier to work with and has some features that other examples use, but if you want your app to work on Android 4.1 you can't
 use it.
This was written as if it were a CTS test, but is not part of CTS. It should be straightforward to adapt the code to other environments. (If you're having trouble with `awaitNewImage()`, see[this
 article](http://stackoverflow.com/questions/22457623/).)
**[DecoderTest.java](https://android.googlesource.com/platform/cts/+/jb-mr2-release/tests/tests/media/src/android/media/cts/DecoderTest.java)** (requires
 4.1, API 16)
> CTS test. Tests decoding pre-recorded audio streams.
**[EncoderTest.java](https://android.googlesource.com/platform/cts/+/jb-mr2-release/tests/tests/media/src/android/media/cts/EncoderTest.java)** (requires
 4.1, API 16)
> CTS test. Tests encoding of audio streams.
**[MediaMuxerTest.java](https://android.googlesource.com/platform/cts/+/kitkat-release/tests/tests/media/src/android/media/cts/MediaMuxerTest.java)** (requires
 4.3, API 18)
> CTS test. Uses MediaMuxer to clone the audio track, video track, and audio+video tracks from input to output.
**[screenrecord](http://www.bigflake.com/screenrecord/)** (uses non-public native APIs)
> You can access the source code for `screenrecord`, a developer shell command introduced in Android 4.4 (API 19). It demonstrates the use of the native equivalents of `MediaCodec` and `MediaMuxer`in
 a pure-native command. v1.1 uses GLES and the native equivalent of `SurfaceTexture`.
This is FOR REFERENCE ONLY. Non-public APIs are very likely to break between releases and are not guaranteed to have consistent behavior across different devices.
**[Grafika](https://github.com/google/grafika)** (requires 4.3, API 18)
> Test application exercising various graphics & media features. Examples include recording and displaying camera preview, recording OpenGL ES rendering, decoding multiple videos simultaneously, and the use
 of `SurfaceView`, `GLSurfaceView`, and `TextureView`. Highly unstable. Tons of fun.
## FAQ
**Q1.** How do I play the video streams created by `MediaCodec` with the "video/avc" codec?
**A1.** The stream created is a raw H.264 elementary stream. The Totem Movie Player for Linux may work, but many other players won't touch them. You need to use the `MediaMuxer` class to
 create an MP4 file instead. See the [EncodeAndMuxTest sample](http://bigflake.com/mediacodec/#EncodeAndMuxTest).
Q2. Why does my call to `MediaCodec.configure()` fail with an `IllegalStateException` when
 I try to create an encoder?
**A2.** This is usually because you haven't specified all of the [mandatory keys](http://developer.android.com/reference/android/media/MediaFormat.html) required by the encoder.
 See [this stackoverflow item](http://stackoverflow.com/questions/17233835/illegalstateexception-when-mediacodec-configure-android/17243175#17243175) for an example.
Q3. My video decoder is configured but won't accept data. What's wrong?
**A3.** A common mistake is neglecting to set the Codec-Specific Data, mentioned briefly in the documentation, through the keys "csd-0" and "csd-1". This is a bunch of raw data with things like Sequence
 Parameter Set and Picture Parameter Set; all you usually need to know is that the `MediaCodec` encoder generates them and the `MediaCodec` decoder wants them.
If you are feeding the output of the encoder to the decoder, you will note that the first packet you get from the encoder has the [BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG) flag
 set. You need to make sure you propagate this flag to the decoder, so that the first buffer the decoder receives does the setup. Alternatively, you can set the CSD data in the `MediaFormat`, and pass this into the decoder via`configure()`.
 You can see examples of both approaches in the [EncodeDecodeTest sample](http://bigflake.com/mediacodec/#EncodeDecodeTest).
If you're not sure how to set this up, you should probably be using [MediaExtractor](http://developer.android.com/reference/android/media/MediaExtractor.html), which will handle it all
 for you.
Q4. Can I stream data into the decoder?
**A4.** Yes and no. The decoder takes a stream of "access units", which may not be a stream of bytes. For the video decoder, this means you need to preserve the "packet boundaries" established by the
 encoder (e.g. NAL units for H.264 video). For example, see how the `VideoChunks` class in the [DecodeEditEncodeTest sample](http://bigflake.com/mediacodec/#DecodeEditEncodeTest) operates. You can't just read arbitrary
 chunks of the file and pass them in.
Q5. I'm encoding the output of the camera through a YUV preview buffer. Why do the colors look wrong?
**A5.** The color formats for the camera output and the `MediaCodec` encoder input are different. Camera supports YV12 (planar YUV 4:2:0) and NV21 (semi-planar YUV 4:2:0). The `MediaCodec` encoders
 support one or more of:
- #19 COLOR_FormatYUV420Planar (I420)
- #20 COLOR_FormatYUV420PackedPlanar (also I420)
- #21 COLOR_FormatYUV420SemiPlanar (NV12)
- #39 COLOR_FormatYUV420PackedSemiPlanar (also NV12)
- #0x7f000100 COLOR_TI_FormatYUV420PackedSemiPlanar (also also NV12)
I420 has the same general data layout as YV12, but the Cr and Cb planes are reversed. Same with NV12 vs. NV21. So if you try to hand YV12 buffers from the camera to an encoder expecting something else, you'll
 see some odd color effects, like in [these images](http://stackoverflow.com/questions/13703596/mediacodec-and-camera-colorspaces-dont-match).
As of Android 4.4 (API 19), there is still no common input format. Nvidia Tegra 3 devices like the Nexus 7 (2012), and Samsung Exynos devices like the Nexus 10, want `COLOR_FormatYUV420Planar`. Qualcomm
 Adreno devices like the Nexus 4, Nexus 5, and Nexus 7 (2013) want `COLOR_FormatYUV420SemiPlanar`. TI OMAP devices like the Galaxy Nexus want `COLOR_TI_FormatYUV420PackedSemiPlanar`. (This is based on the format that is returned first
 when the AVC codec is queried.)
A more portable, and more efficient, approach is to use the API 18 `Surface` input API, demonstrated in the [CameraToMpegTest
 sample](http://bigflake.com/mediacodec/#CameraToMpegTest). The down side of this is that you have to operate in RGB rather than YUV, which is a problem for image processing software. If you can implement the image manipulation in a fragment shader, perhaps by converting between RGB and YUV before and after
 your computations, you can take advantage of code execution on the GPU.
Note that the `MediaCodec`**decoders** may produce data in ByteBuffers using one of the above formats or in a proprietary format. For example, devices based on Qualcomm SoCs commonly use`OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar32m` (#2141391876
 / 0x7FA30C04).
Surface input uses `COLOR_FormatSurface`, also known as `OMX_COLOR_FormatAndroidOpaque` (#2130708361 / 0x7F000789). For the full list, see `OMX_COLOR_FORMATTYPE` in [OMX_IVCommon.h](https://android.googlesource.com/platform/frameworks/native/+/kitkat-release/include/media/openmax/OMX_IVCommon.h).
Q6. What's this `EGL_RECORDABLE_ANDROID` flag?
**A6.** That tells EGL that the surface it creates must be compatible with the video codecs. Without this flag, EGL might use a buffer format that `MediaCodec` can't understand.
Q7. Can I use the [ImageReader](http://developer.android.com/reference/android/media/ImageReader.html) class
 with `MediaCodec`?
**A7.** No. The `ImageReader` class, added in Android 4.4 (API 19), provides a handy way to access data in a YUV surface. Unfortunately, as of API 19 it only works with buffers from `Camera`.
 Also, there is no corresponding `ImageWriter` class for creating content.
Q8. Do I have to set a presentation time stamp when encoding video?
**A8.** Yes. It appears that some devices will drop frames or encode them at low quality if the presentation time stamp isn't set to a reasonable value (see [this
 stackoverflow item](http://stackoverflow.com/questions/20475332/)).
Remember that the time required by `MediaCodec` is in microseconds. Most timestamps passed around in Java code are in milliseconds or nanoseconds.
Q9. Most of the examples require API 18. I'm coding for API 16. Is there something I should know?
**A9.** Yes. Some key features aren't available until API 18, and some basic features are more difficult to use in API 16.
If you're decoding video, things don't change much. As you can see from the two implementations of [ExtractMpegFramesTest](http://bigflake.com/mediacodec/#ExtractMpegFramesTest), the newer
 version of EGL isn't available, but for many applications that won't matter.
If you're encoding video, things are much worse. Three key points:
- The `MediaCodec` encoders don't accept input from a Surface, so you have to provide the data as raw YUV frames.
- The layout of the YUV frames varies from device to device, and in some cases you have to check for specific vendors by name to handle certain qirks.
- Some devices may not advertise support any usable YUV formats (internal-use only).
- The `MediaMuxer` class doesn't exist, so there's no way to convert the H.264 stream to something that `MediaPlayer` (or many desktop players) will accept. You have to use a 3rd-party library (perhaps [mp4parser](http://stackoverflow.com/questions/22996378/)).
- When the `MediaMuxer` class was introduced in API 18, the behavior of `MediaCodec` encoders was changed to emit `INFO_OUTPUT_FORMAT_CHANGED` at the start, so that you have a convenient `MediaFormat` to feed to
 the muxer. On older versions of Android, this does not happen.
This [stackoverflow item](http://stackoverflow.com/questions/21262797/converting-images-to-video) has additional links and commentary.
The CTS tests for `MediaCodec` were introduced with API 18 (Android 4.3), which in practice means that's the first release where the basic features are likely to work consistently across devices. In particular,
 pre-4.3 devices have been known to drop the last frame or scramble PTS values when decoding.
Q10. Can I use `MediaCodec` in the AOSP emulator?
**A10.** Maybe. The emulator provides a software AVC codec that lacks certain features (e.g. there's no surface input to the encoder). Developing on a physical device will likely be less frustrating.
Q11. Why is the output messed up (all zeroes, too short, etc)?
**A11.** The most common mistake is failing to adjust the `ByteBuffer` position and limit values. As of API 19, `MediaCodec` does not do this for you.
You need to do something like:
  int bufIndex = codec.dequeueOutputBuffer(info, TIMEOUT);
  ByteBuffer outputData = outputBuffers[bufIndex];
  if (info.size != 0) {
      outputData.position(info.offset);
      outputData.limit(info.offset + info.size);
  }
On the input side, you want to call `clear()` on the buffer before copying data into it.
Q12. Why am I seeing `storeMetaDataInBuffers` failures in the log?
**A12.** They look like this (example from a Nexus 5):
E OMXNodeInstance: OMX_SetParameter() failed for StoreMetaDataInBuffers: 0x8000101a
E ACodec  : [OMX.qcom.video.encoder.avc] storeMetaDataInBuffers (output) failed w/ err -2147483648
You can ignore them, they're harmless.
## Further Assistance
Please post all questions to [stackoverflow](http://stackoverflow.com/) with the `android` tag (and, for `MediaCodec` issues, the `mediacodec` tag as well).
 Comments or feature requests for the framework or CTS tests should be made on[the AOSP bug tracker](http://b.android.com/).
