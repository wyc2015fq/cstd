# MediaCodec Class Overview - u013366022的专栏 - CSDN博客
2014年05月14日 15:01:22[slitaz](https://me.csdn.net/u013366022)阅读数：1390
public final class
# MediaCodec
extends [Object](http://developer.android.com/reference/java/lang/Object.html)
   ↳
android.media.MediaCodec
## Class Overview
MediaCodec class can be used to access low-level media codec, i.e. encoder/decoder components.
MediaCodec is generally used like this:
MediaCodec codec =MediaCodec.createDecoderByType(type);
 codec.configure(format,...);
 codec.start();ByteBuffer[] inputBuffers = codec.getInputBuffers();ByteBuffer[] outputBuffers = codec.getOutputBuffers();for(;;){int inputBufferIndex = codec.dequeueInputBuffer(timeoutUs);if(inputBufferIndex >=0){// fill inputBuffers[inputBufferIndex] with valid data...
     codec.queueInputBuffer(inputBufferIndex,...);}int outputBufferIndex = codec.dequeueOutputBuffer(timeoutUs);if(outputBufferIndex >=0){// outputBuffer is ready to be processed or rendered....
     codec.releaseOutputBuffer(outputBufferIndex,...);}elseif(outputBufferIndex ==MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED){
     outputBuffers = codec.getOutputBuffers();}elseif(outputBufferIndex ==MediaCodec.INFO_OUTPUT_FORMAT_CHANGED){// Subsequent data will conform to new format.MediaFormat format = codec.getOutputFormat();...}}
 codec.stop();
 codec.release();
 codec =null;
Each codec maintains a number of input and output buffers that are referred to by index in API calls. The contents of these buffers is represented by the ByteBuffer[] arrays accessible through getInputBuffers() and getOutputBuffers(). After a successful call
 to `[start()](http://developer.android.com/reference/android/media/MediaCodec.html#start())` the client "owns"
 neither input nor output buffers, subsequent calls to `[dequeueInputBuffer(long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))` and
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
 then transfer ownership from the codec to the client.
The client is not required to resubmit/release buffers immediately to the codec, the sample code above simply does this for simplicity's sake.
Once the client has an input buffer available it can fill it with data and submit it it to the codec via a call to 
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))
```
.
The codec in turn will return an output buffer to the client in response to 
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
. After the output buffer has been processed a call to
```
[releaseOutputBuffer(int,
 boolean)](http://developer.android.com/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20boolean))
```
 will return it to the codec. If a video surface has been provided in the call to 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
,
```
[releaseOutputBuffer(int,
 boolean)](http://developer.android.com/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20boolean))
```
 optionally allows rendering of the buffer to the surface.
Input buffers (for decoders) and Output buffers (for encoders) contain encoded data according to the format's type. For video types this data is all the encoded data representing a single moment in time, for audio
 data this is slightly relaxed in that a buffer may contain multiple encoded frames of audio. In either case, buffers do not start and end on arbitrary byte boundaries, this is not a stream of bytes, it's a stream of access units.
Most formats also require the actual data to be prefixed by a number of buffers containing setup data, or codec specific data, i.e. the first few buffers submitted to the codec object after starting it must be codec
 specific data marked as such using the flag `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)` in
 a call to 
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))
```
. Codec specific data included in the format passed to 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
 (in ByteBuffer entries with keys "csd-0", "csd-1", ...) is automatically submitted to the codec, this data MUST NOT be submitted explicitly by the client. Once the client reaches the end of the input data it signals the
 end of the input stream by specifying a flag of `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)` in
 the call to
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))
```
. The codec will continue to return output buffers until it eventually signals the end of the output stream by specifying the same flag (`[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`)
 on the BufferInfo returned in 
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
. In order to start decoding data that's not adjacent to previously submitted data (i.e. after a seek) it is necessary to `[flush()](http://developer.android.com/reference/android/media/MediaCodec.html#flush())` the
 decoder. Any input or output buffers the client may own at the point of the flush are immediately revoked, i.e. after a call to `[flush()](http://developer.android.com/reference/android/media/MediaCodec.html#flush())` the
 client does not own any buffers anymore. Note that the format of the data submitted after a flush must not change, flush does not support format discontinuities, for this a full stop(), configure(), start() cycle is necessary.
## ![](https://img-blog.csdn.net/20140514150027531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzM2NjAyMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
void[finalize](http://developer.android.com/reference/android/media/MediaCodec.html#finalize())()
Invoked when the garbage collector has detected that this instance is no longer reachable.
final void[stop](http://developer.android.com/reference/android/media/MediaCodec.html#stop())()
Finish the decode/encode session, note that the codec instance remains active and ready to be `[start()](http://developer.android.com/reference/android/media/MediaCodec.html#start())`ed
 again.
![](http://developer.android.com/assets/images/triangle-closed.png) From
 class [java.lang.Object](http://developer.android.com/reference/java/lang/Object.html)
## Constants
#### public static final int BUFFER_FLAG_CODEC_CONFIG
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
This indicated that the buffer marked as such contains codec initialization / codec specific data instead of media data.
Constant Value: 2 (0x00000002)
#### public static final int BUFFER_FLAG_END_OF_STREAM
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
This signals the end of stream, i.e. no buffers will be available after this, unless of course, `[flush()](http://developer.android.com/reference/android/media/MediaCodec.html#flush())` follows.
Constant Value: 4 (0x00000004)
#### public static final int BUFFER_FLAG_SYNC_FRAME
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
This indicates that the buffer marked as such contains the data for a sync frame.
Constant Value: 1 (0x00000001)
#### public static final int CONFIGURE_FLAG_ENCODE
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
If this codec is to be used as an encoder, pass this flag.
Constant Value: 1 (0x00000001)
#### public static final int CRYPTO_MODE_AES_CTR
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Constant Value: 1 (0x00000001)
#### public static final int CRYPTO_MODE_UNENCRYPTED
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Constant Value: 0 (0x00000000)
#### public static final int INFO_OUTPUT_BUFFERS_CHANGED
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
The output buffers have changed, the client must refer to the new set of output buffers returned by `[getOutputBuffers()](http://developer.android.com/reference/android/media/MediaCodec.html#getOutputBuffers())` from
 this point on.
Constant Value: -3 (0xfffffffd)
#### public static final int INFO_OUTPUT_FORMAT_CHANGED
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
The output format has changed, subsequent data will follow the new format. `[getOutputFormat()](http://developer.android.com/reference/android/media/MediaCodec.html#getOutputFormat())` returns
 the new format.
Constant Value: -2 (0xfffffffe)
#### public static final int INFO_TRY_AGAIN_LATER
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
If a non-negative timeout had been specified in the call to 
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
, indicates that the call timed out.
Constant Value: -1 (0xffffffff)
#### public static final [String](http://developer.android.com/reference/java/lang/String.html)PARAMETER_KEY_REQUEST_SYNC_FRAME
Added in [API level 19](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Request that the encoder produce a sync frame "soon". Provide an Integer with the value 0.
Constant Value: "request-sync"
#### public static final [String](http://developer.android.com/reference/java/lang/String.html)PARAMETER_KEY_SUSPEND
Added in [API level 19](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Temporarily suspend/resume encoding of input data. While suspended input data is effectively discarded instead of being fed into the encoder. This parameter really only makes sense to use with an
 encoder in "surface-input" mode, as the client code has no control over the input-side of the encoder in that case. The value is an Integer object containing the value 1 to suspend or the value 0 to resume.
Constant Value: "drop-input-frames"
#### public static final [String](http://developer.android.com/reference/java/lang/String.html)PARAMETER_KEY_VIDEO_BITRATE
Added in [API level 19](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Change a video encoder's target bitrate on the fly. The value is an Integer object containing the new bitrate in bps.
Constant Value: "video-bitrate"
#### public static final int VIDEO_SCALING_MODE_SCALE_TO_FIT
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
The content is scaled to the surface dimensions
Constant Value: 1 (0x00000001)
#### public static final int VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
The content is scaled, maintaining its aspect ratio, the whole surface area is used, content may be cropped
Constant Value: 2 (0x00000002)
## Public Methods
#### public void configure([MediaFormat](http://developer.android.com/reference/android/media/MediaFormat.html) format, [Surface](http://developer.android.com/reference/android/view/Surface.html) surface, [MediaCrypto](http://developer.android.com/reference/android/media/MediaCrypto.html) crypto, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Configures a component.
##### Parameters
flags
Specify `[CONFIGURE_FLAG_ENCODE](http://developer.android.com/reference/android/media/MediaCodec.html#CONFIGURE_FLAG_ENCODE)` to
 configure the component as an encoder.
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createByCodecName([String](http://developer.android.com/reference/java/lang/String.html) name)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
If you know the exact name of the component you want to instantiate use this method to instantiate it. Use with caution. Likely to be used with information obtained from `[MediaCodecList](http://developer.android.com/reference/android/media/MediaCodecList.html)`
##### Parameters
name
The name of the codec to be instantiated.
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createDecoderByType([String](http://developer.android.com/reference/java/lang/String.html) type)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Instantiate a decoder supporting input data of the given mime type. The following is a partial list of defined mime types and their semantics:
- "video/x-vnd.on2.vp8" - VP8 video (i.e. video in .webm)
- "video/x-vnd.on2.vp9" - VP9 video (i.e. video in .webm)
- "video/avc" - H.264/AVC video
- "video/mp4v-es" - MPEG4 video
- "video/3gpp" - H.263 video
- "audio/3gpp" - AMR narrowband audio
- "audio/amr-wb" - AMR wideband audio
- "audio/mpeg" - MPEG1/2 audio layer III
- "audio/mp4a-latm" - AAC audio (note, this is raw AAC packets, not packaged in LATM!)
- "audio/vorbis" - vorbis audio
- "audio/g711-alaw" - G.711 alaw audio
- "audio/g711-mlaw" - G.711 ulaw audio
##### Parameters
type
The mime type of the input data.
#### public static [MediaCodec](http://developer.android.com/reference/android/media/MediaCodec.html)createEncoderByType([String](http://developer.android.com/reference/java/lang/String.html) type)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Instantiate an encoder supporting output data of the given mime type.
##### Parameters
type
The desired mime type of the output data.
#### public final [Surface](http://developer.android.com/reference/android/view/Surface.html)createInputSurface()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Requests a Surface to use as the input to an encoder, in place of input buffers. This may only be called after 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
 and before `[start()](http://developer.android.com/reference/android/media/MediaCodec.html#start())`.
The application is responsible for calling release() on the Surface when done.
#### public final int dequeueInputBuffer(long timeoutUs)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Returns the index of an input buffer to be filled with valid data or -1 if no such buffer is currently available. This method will return immediately if timeoutUs == 0, wait indefinitely for the
 availability of an input buffer if timeoutUs < 0 or wait up to "timeoutUs" microseconds if timeoutUs > 0.
##### Parameters
timeoutUs
The timeout in microseconds, a negative timeout indicates "infinite".
#### public final int dequeueOutputBuffer([MediaCodec.BufferInfo](http://developer.android.com/reference/android/media/MediaCodec.BufferInfo.html) info, long timeoutUs)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Dequeue an output buffer, block at most "timeoutUs" microseconds. Returns the index of an output buffer that has been successfully decoded or one of the INFO_* constants below.
##### Parameters
timeoutUs
The timeout in microseconds, a negative timeout indicates "infinite".
#### public final void flush()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Flush both input and output ports of the component, all indices previously returned in calls to `[dequeueInputBuffer(long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))` and 
```
[dequeueOutputBuffer(MediaCodec.BufferInfo,
 long)](http://developer.android.com/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))
```
 become invalid.
#### public [MediaCodecInfo](http://developer.android.com/reference/android/media/MediaCodecInfo.html)getCodecInfo()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Get the codec info. If the codec was created by createDecoderByType or createEncoderByType, what component is chosen is not known beforehand, and thus the caller does not have the MediaCodecInfo.
#### public [ByteBuffer[]](http://developer.android.com/reference/java/nio/ByteBuffer.html)getInputBuffers()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Call this after start() returns.
#### public final [String](http://developer.android.com/reference/java/lang/String.html)getName()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Get the component name. If the codec was created by createDecoderByType or createEncoderByType, what component is chosen is not known beforehand.
#### public [ByteBuffer[]](http://developer.android.com/reference/java/nio/ByteBuffer.html)getOutputBuffers()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Call this after start() returns and whenever dequeueOutputBuffer signals an output buffer change by returning `[INFO_OUTPUT_BUFFERS_CHANGED](http://developer.android.com/reference/android/media/MediaCodec.html#INFO_OUTPUT_BUFFERS_CHANGED)`
#### public final [MediaFormat](http://developer.android.com/reference/android/media/MediaFormat.html)getOutputFormat()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Call this after dequeueOutputBuffer signals a format change by returning `[INFO_OUTPUT_FORMAT_CHANGED](http://developer.android.com/reference/android/media/MediaCodec.html#INFO_OUTPUT_FORMAT_CHANGED)`
#### public final void queueInputBuffer(int index, int offset, int size, long presentationTimeUs, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
After filling a range of the input buffer at the specified index submit it to the component. Many decoders require the actual compressed data stream to be preceded by "codec specific data", i.e.
 setup data used to initialize the codec such as PPS/SPS in the case of AVC video or code tables in the case of vorbis audio. The class `[MediaExtractor](http://developer.android.com/reference/android/media/MediaExtractor.html)` provides
 codec specific data as part of the returned track format in entries named "csd-0", "csd-1" ... These buffers should be submitted using the flag `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)`.
 To indicate that this is the final piece of input data (or rather that no more input data follows unless the decoder is subsequently flushed) specify the flag `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`.
##### Parameters
flags
A bitmask of flags `[BUFFER_FLAG_SYNC_FRAME](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_SYNC_FRAME)`, `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)` or `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`.
##### Throws
[MediaCodec.CryptoException](http://developer.android.com/reference/android/media/MediaCodec.CryptoException.html)
if a crypto object has been specified in 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
#### public final void queueSecureInputBuffer(int index, int offset, [MediaCodec.CryptoInfo](http://developer.android.com/reference/android/media/MediaCodec.CryptoInfo.html) info, long presentationTimeUs, int flags)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Similar to 
```
[queueInputBuffer(int,
 int, int, long, int)](http://developer.android.com/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))
```
 but submits a buffer that is potentially encrypted.
##### Parameters
flags
A bitmask of flags `[BUFFER_FLAG_SYNC_FRAME](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_SYNC_FRAME)`, `[BUFFER_FLAG_CODEC_CONFIG](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)` or `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`.
##### Throws
[MediaCodec.CryptoException](http://developer.android.com/reference/android/media/MediaCodec.CryptoException.html)
if an error occurs while attempting to decrypt the buffer. An error code associated with the exception helps identify the reason for the failure.
#### public final void release()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Make sure you call this when you're done to free up any opened component instance instead of relying on the garbage collector to do this for you at some point in the future.
#### public final void releaseOutputBuffer(int index, boolean render)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
If you are done with a buffer, use this call to return the buffer to the codec. If you previously specified a surface when configuring this video decoder you can optionally render the buffer.
##### Parameters
render
If a valid surface was specified when configuring the codec, passing true renders this output buffer to the surface.
#### public final void setParameters([Bundle](http://developer.android.com/reference/android/os/Bundle.html) params)
Added in [API level 19](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Communicate additional parameter changes to the component instance.
#### public final void setVideoScalingMode(int mode)
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
If a surface has been specified in a previous call to 
```
[configure(MediaFormat,
 Surface, MediaCrypto, int)](http://developer.android.com/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))
```
 specifies the scaling mode to use. The default is "scale to fit".
#### public final void signalEndOfInputStream()
Added in [API level 18](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Signals end-of-stream on input. Equivalent to submitting an empty buffer with `[BUFFER_FLAG_END_OF_STREAM](http://developer.android.com/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)` set.
 This may only be used with encoders receiving input from a Surface created by `[createInputSurface()](http://developer.android.com/reference/android/media/MediaCodec.html#createInputSurface())`.
#### public final void start()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
After successfully configuring the component, call start. On return you can query the component for its input/output buffers.
#### public final void stop()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Finish the decode/encode session, note that the codec instance remains active and ready to be `[start()](http://developer.android.com/reference/android/media/MediaCodec.html#start())`ed
 again. To ensure that it is available to other client call `[release()](http://developer.android.com/reference/android/media/MediaCodec.html#release())` and
 don't just rely on garbage collection to eventually do this for you.
## Protected Methods
#### protected void finalize()
Added in [API level 16](http://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels)
Invoked when the garbage collector has detected that this instance is no longer reachable. The default implementation does nothing, but this method can be overridden to free resources.
Note that objects that override `finalize` are significantly more expensive than objects that don't. Finalizers may be run
 a long time after the object is no longer reachable, depending on memory pressure, so it's a bad idea to rely on them for cleanup. Note also that finalizers are run on a single VM-wide finalizer thread, so doing blocking work in a finalizer is a bad idea.
 A finalizer is usually only necessary for a class that has a native peer and needs to call a native method to destroy that peer. Even then, it's better to provide an explicit `close` method
 (and implement `[Closeable](http://developer.android.com/reference/java/io/Closeable.html)`), and insist
 that callers manually dispose of instances. This works well for something like files, but less well for something like a `BigInteger` where typical calling code would have to deal with
 lots of temporaries. Unfortunately, code that creates lots of temporaries is the worst kind of code from the point of view of the single finalizer thread.
If you *must* use finalizers, consider at least providing your own `[ReferenceQueue](http://developer.android.com/reference/java/lang/ref/ReferenceQueue.html)` and
 having your own thread process that queue.
Unlike constructors, finalizers are not automatically chained. You are responsible for calling `super.finalize()` yourself.
Uncaught exceptions thrown by finalizers are ignored and do not terminate the finalizer thread. See *Effective Java* Item 7, "Avoid finalizers" for more.
