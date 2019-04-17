# Android音视频编解码库MediaCodec - 一世豁然的专栏 - CSDN博客





2018年04月16日 09:00:51[一世豁然](https://me.csdn.net/Explorer_day)阅读数：731








MediaCodec类可用于访问低级媒体编解码器，即编码器/解码器组件。 它是Android低级多媒体支持基础架构的一部分（通常与MediaExtractor，MediaSync，MediaMuxer，MediaCrypto，MediaDrm，Image，Surface和AudioTrack一起使用）。

![](https://img-blog.csdn.net/20180416071900174)


广义而言，编解码器处理输入数据以生成输出数据。 它异步处理数据并使用一组输入和输出缓冲区。 在简单的层面上，您请求（或接收）一个空输入缓冲区，填充数据并将其发送到编解码器进行处理。 编解码器使用数据并将其转换为其空的输出缓冲区之一。 最后，您请求（或接收）一个填充的输出缓冲区，消耗其内容并将其释放回编解码器。








一、数据类型

编解码器对三种数据进行操作：压缩数据，原始音频数据和原始视频数据。 所有三种数据都可以使用ByteBuffers进行处理，但是您应该使用Surface来处理原始视频数据，以提高编解码器的性能。 Surface使用本地视频缓冲区而不映射或将它们复制到ByteBuffers; 因此，它更有效率。 使用Surface时通常无法访问原始视频数据，但可以使用ImageReader类访问不安全的解码（原始）视频帧。 这可能比使用ByteBuffers更高效，因为某些本地缓冲区可能映射到直接ByteBuffers。 使用ByteBuffer模式时，可以使用Image类和getInput / OutputImage（int）来访问原始视频帧。





一）、压缩数据

输入缓冲器（用于解码器）和输出缓冲器（用于编码器）根据格式的类型包含压缩数据。 对于视频类型，这通常是单个压缩视频帧。 对于音频数据，这通常是单个访问单元（编码音频片段通常包含由格式类型规定的几毫秒的音频），但是这个要求稍微宽松，因为缓冲器可能包含多个编码的音频访问单元。 无论哪种情况，缓冲区都不会以任意字节边界开始或结束，而是在帧/访问单元边界上，除非它们被标记为BUFFER_FLAG_PARTIAL_FRAME。





二）、原始音频数据

原始音频缓冲区包含整个PCM音频数据帧，这是通道顺序中每个通道的一个样本。 每个采样都是以本地字节顺序的16位有符号整数。


```java
short[] getSamplesForChannel(MediaCodec codec, int bufferId, int channelIx) {
   ByteBuffer outputBuffer = codec.getOutputBuffer(bufferId);
   MediaFormat format = codec.getOutputFormat(bufferId);
   ShortBuffer samples = outputBuffer.order(ByteOrder.nativeOrder()).asShortBuffer();
   int numChannels = formet.getInteger(MediaFormat.KEY_CHANNEL_COUNT);
   if (channelIx < 0 || channelIx >= numChannels) {
     return null;
   }
   short[] res = new short[samples.remaining() / numChannels];
   for (int i = 0; i < res.length; ++i) {
     res[i] = samples.get(i * numChannels + channelIx);
   }
   return res;
 }
```




三）、原始视频数据

在ByteBuffer模式下，视频缓冲区根据其颜色格式进行布局。 您可以从getCodecInfo（）。getCapabilitiesForType（...）.colorFormats获取支持的颜色格式。 视频编解码器可支持三种颜色格式：


1、本地原始视频格式：这是由COLOR_FormatSurface标记的，它可以与输入或输出表面一起使用。

2、灵活的YUV缓冲区（例如COLOR_FormatYUV420Flexible）：通过使用getInput / OutputImage（int），可以将这些缓冲区与输入/输出Surface以及ByteBuffer模式结合使用。

3、其他特定格式：这些通常仅在ByteBuffer模式下受支持。 一些颜色格式是供应商特定的。 其他在MediaCodecInfo.CodecCapabilities中定义。 对于等效于灵活格式的颜色格式，仍然可以使用getInput / OutputImage（int）。

自LOLLIPOP_MR1开始，所有视频编解码器都支持灵活的YUV 4：2：0缓冲区。





四）、在旧设备上访问原始视频ByteBuffers

在LOLLIPOP和Image支持之前，您需要使用KEY_STRIDE和KEY_SLICE_HEIGHT输出格式值来了解原始输出缓冲区的布局。


**请注意，在某些设备上，切片高度被宣传为0.这可能意味着切片高度与帧高度相同，或者切片高度是与某个值对齐的帧高度（通常是2）。 不幸的是，在这种情况下没有标准和简单的方法来说明实际的切片高度。 此外，平面格式的U平面的垂直跨度也没有规定或限定，尽管通常它是切片高度的一半。**


KEY_WIDTH和KEY_HEIGHT键指定视频帧的大小; 然而，对于大多数情况，视频（图片）只占据视频帧的一部分。 这由“裁剪矩形”表示。


您需要使用以下键从输出格式获取原始输出图像的裁剪矩形。 如果这些键不存在，则该视频占据整个视频帧。在应用任何旋转之前，在输出帧的上下文中理解裁剪矩形。



|Format Key|Type|Description|
|----|----|----|
|`"crop-left"`|Integer|The left-coordinate (x) of the crop rectangle|
|`"crop-top"`|Integer|The top-coordinate (y) of the crop rectangle|
|`"crop-right"`|Integer|The right-coordinate (x) MINUS 1 of the crop rectangle|
|`"crop-bottom"`|Integer|The bottom-coordinate (y) MINUS 1 of the crop rectangle|
|The right and bottom coordinates can be understood as the coordinates of the right-most valid column/bottom-most valid row of the cropped output image.| | |
视频帧的大小（旋转之前）可以这样计算
：

```java
MediaFormat format = decoder.getOutputFormat(…);
 int width = format.getInteger(MediaFormat.KEY_WIDTH);
 if (format.containsKey("crop-left") && format.containsKey("crop-right")) {
     width = format.getInteger("crop-right") + 1 - format.getInteger("crop-left");
 }
 int height = format.getInteger(MediaFormat.KEY_HEIGHT);
 if (format.containsKey("crop-top") && format.containsKey("crop-bottom")) {
     height = format.getInteger("crop-bottom") + 1 - format.getInteger("crop-top");
 }
```

**另请注意，BufferInfo.offset的含义在各个设备上并不一致。 在某些设备上，偏移量指向裁剪矩形的左上角像素，而在大多数设备上，它指向整个帧的左上角像素。**





二、状态

在其生命周期中，编解码器概念上存在三种状态之一：停止，执行或释放。 停止的集体状态实际上是三种状态的集合：未初始化，已配置和错误，而执行状态在概念上通过三个子状态：刷新，运行和结束流。


![](https://img-blog.csdn.net/20180416074552690)


当您使用其中一种工厂方法创建编解码器时，编解码器处于未初始化状态。 首先，您需要通过配置（...）进行配置，该配置会将其置于Configured状态，然后调用start（）将其移至Executing状态。 在这种状态下，您可以通过上述的缓冲区队列处理来处理数据。


执行状态有三个子状态：刷新，运行和结束流。 在开始（）之后立即编解码器处于Flushed子状态，它保存所有缓冲区。 只要第一个输入缓冲区出列，编解码器就会转移到运行子状态，在该状态下它大部分时间都在使用。 当您使用流结束标记对输入缓冲区进行排队时，编解码器将转换为流结束子状态。 在这种状态下，编解码器不再接受进一步的输入缓冲器，但仍然产生输出缓冲器，直到输出端达到输出端。 您可以在执行状态下使用flush（）随时移回Flushed子状态。


调用stop（）将编解码器返回到未初始化状态，然后再次配置它。 当您使用编解码器时，您必须通过调用release（）来释放它。


在极少数情况下，编解码器可能会遇到错误并进入错误状态。 这是使用来自排队操作的无效返回值或有时通过异常传递的。 调用reset（）使编解码器再次可用。 您可以从任何状态调用它以将编解码器移回未初始化状态。 否则，调用release（）移到终端Released状态。





三、创建

使用MediaCodecList为特定的MediaFormat创建MediaCodec。 在解码文件或流时，您可以从MediaExtractor.getTrackFormat获取所需的格式。 使用MediaFormat.setFeatureEnabled注入要添加的任何特定功能，然后调用MediaCodecList.findDecoderForFormat以获取可处理该特定媒体格式的编解码器的名称。 最后，使用createByCodecName（String）创建编解码器。


**注意：在LOLLIPOP上，MediaCodecList.findDecoder / EncoderForFormat的格式不得包含帧速率。 使用format.setString（MediaFormat.KEY_FRAME_RATE，null）清除格式中的任何现有帧速率设置。**


您还可以使用createDecoder / EncoderByType（String）为特定的MIME类型创建首选编解码器。 但是，这不能用于注入功能，并且可能会创建无法处理特定所需媒体格式的编解码器。





1、创建安全解码器

在KITKAT_WATCH和更早版本上，安全编解码器可能未列在MediaCodecList中，但可能仍在系统中可用。 存在的安全编码解码器只能通过名称实例化，方法是将“.secure”附加到常规编解码器的名称（所有安全编解码器的名称必须以“.secure”结尾。）createByCodecName（String）将抛出IOException错误，如果编解码器不在系统中。


从LOLLIPOP开始，您应该使用媒体格式的FEATURE_SecurePlayback功能来创建安全的解码器。








四、初始化

创建编解码器后，如果要异步处理数据，可以使用setCallback设置回调。 然后，使用特定的媒体格式配置编解码器。 这是您可以为视频制作者指定输出Surface - 生成原始视频数据的编解码器（例如视频解码器）。 您也可以设置安全编解码器的解密参数（请参阅MediaCrypto）。 最后，由于一些编解码器可以在多种模式下运行，因此您必须指定是否要将其用作解码器或编码器。


由于LOLLIPOP，您可以在配置状态下查询生成的输入和输出格式。 您可以使用它来验证最终的配置，例如 颜色格式，然后开始编解码器。


如果您想要使用视频使用者本地处理原始输入视频缓冲区 - 处理原始视频输入的编解码器（例如视频编码器） - 使用配置后的createInputSurface（）为输入数据创建目标Surface。 或者，通过调用setInputSurface（Surface）将编解码器设置为使用先前创建的持久输入曲面。





1、编解码器专用数据

某些格式，特别是AAC音频和MPEG4，H.264和H.265视频格式要求实际数据前缀包含设置数据或编解码器特定数据的多个缓冲区。 处理这种压缩格式时，必须在开始（）之后和任何帧数据之前将此数据提交给编解码器。 在调用queueInputBuffer时，必须使用标志BUFFER_FLAG_CODEC_CONFIG标记这些数据。


特定于编解码器的数据也可以包含在通过密钥“csd-0”，“csd-1”等在ByteBuffer条目中进行配置的格式中。这些密钥始终包含在从MediaExtractor获取的音轨MediaFormat中。 在开始（）时，格式中的编解码器专用数据会自动提交给编解码器; 你绝不能明确地提交这些数据。 如果格式不包含编解码器特定数据，则可以根据格式要求，选择使用指定数量的缓冲区以正确的顺序提交它。 在H.264 AVC的情况下，您还可以连接所有编解码器专用数据并将其作为单个编解码器配置缓冲区提交。


Android使用以下编解码器特定的数据缓冲区。 这些也需要设置为正确的MediaMuxer音轨配置的音轨格式。 用（*）标记的每个参数集和特定于编解码器的数据部分必须以“\ x00 \ x00 \ x00 \ x01”开始代码开头。



|Format|CSD buffer #0|CSD buffer #1|CSD buffer #2|
|----|----|----|----|
|AAC|Decoder-specific information from ESDS*|Not Used|Not Used|
|VORBIS|Identification header|Setup header|Not Used|
|OPUS|Identification header|Pre-skip in nanosecs(unsigned 64-bit [native-order](https://developer.android.google.cn/reference/java/nio/ByteOrder.html#nativeOrder()) integer.)This overrides the pre-skip value in the identification header.|Seek Pre-roll in nanosecs(unsigned 64-bit [native-order](https://developer.android.google.cn/reference/java/nio/ByteOrder.html#nativeOrder())integer.)|
|FLAC|mandatory metadata block (called the STREAMINFO block),optionally followed by any number of other metadata blocks|Not Used|Not Used|
|MPEG-4|Decoder-specific information from ESDS*|Not Used|Not Used|
|H.264 AVC|SPS (Sequence Parameter Sets*)|PPS (Picture Parameter Sets*)|Not Used|
|H.265 HEVC|VPS (Video Parameter Sets*) +SPS (Sequence Parameter Sets*) +PPS (Picture Parameter Sets*)|Not Used|Not Used|
|VP9|VP9 [CodecPrivate](http://wiki.webmproject.org/vp9-codecprivate) Data (optional)|Not Used|Not Used|
**注意：如果编解码器在启动之后立即刷新或在输出缓冲区或输出格式更改返回之前不久，请注意，因为在清空过程中编解码器的特定数据可能会丢失。 在这样的刷新之后，您必须使用标有BUFFER_FLAG_CODEC_CONFIG的缓冲区重新提交数据，以确保正确的编解码器操作。**
编码器（或生成压缩数据的编解码器）将在使用codec-config标志标记的输出缓冲区中的任何有效输出缓冲区之前创建并返回编解码器特定数据。 包含编解码器专用数据的缓冲器没有有意义的时间戳。








五、数据处理

每个编解码器都维护一组由API调用中的缓冲区ID引用的输入和输出缓冲区。 在成功调用start（）之后，客户端“拥有”既没有输入也没有输出缓冲区。 在同步模式下，调用dequeueInput / OutputBuffer（...）以从编解码器获取（获取所有权）输入或输出缓冲区。 在异步模式下，您将通过MediaCodec.Callback.onInput / OutputBufferAvailable（...）回调自动接收可用缓冲区。


在获得输入缓冲区时，填充数据并使用queueInputBuffer - 或queueSecureInputBuffer（如果使用解密）将其提交给编解码器。 不要使用相同的时间戳提交多个输入缓冲区（除非它是特定编解码器的数据标记）。


编解码器将通过异步模式下的onOutputBufferAvailable回调或者响应同步模式下的dequeuOutputBuffer调用返回一个只读输出缓冲区。 输出缓冲区处理完毕后，调用其中一个releaseOutputBuffer方法将缓冲区返回给编解码器。


虽然您不需要立即重新提交/释放缓冲区到编解码器，但握住输入和/或输出缓冲区可能会使编解码器停止工作，并且此行为取决于设备。 特别是，编解码器有可能在产生输出缓冲区之前暂缓，直到所有未完成的缓冲区被释放/重新提交。 因此，尽可能少地尝试保留可用的缓冲区。


根据API版本，您可以通过三种方式处理数据：



|Processing Mode|API version <= 20Jelly Bean/KitKat|API version >= 21Lollipop and later|
|----|----|----|
|Synchronous API using buffer arrays|Supported|Deprecated|
|Synchronous API using buffers|Not Available|Supported|
|Asynchronous API using buffers|Not Available|Supported|


一）使用缓冲区的异步处理

由于LOLLIPOP，首选方法是在调用configure之前通过设置回调来异步处理数据。 异步模式会稍微改变状态转换，因为您必须在flush（）之后调用start（）将编解码器转换为Running子状态并开始接收输入缓冲区。 同样，在初始调用开始时，编解码器将直接移至Running子状态，并通过回调开始传递可用的输入缓冲区。


MediaCodec通常以异步模式使用：


```java
MediaCodec codec = MediaCodec.createByCodecName(name);
 MediaFormat mOutputFormat; // member variable
 codec.setCallback(new MediaCodec.Callback() {
   @Override
   void onInputBufferAvailable(MediaCodec mc, int inputBufferId) {
     ByteBuffer inputBuffer = codec.getInputBuffer(inputBufferId);
     // fill inputBuffer with valid data
     …
     codec.queueInputBuffer(inputBufferId, …);
   }

   @Override
   void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, …) {
     ByteBuffer outputBuffer = codec.getOutputBuffer(outputBufferId);
     MediaFormat bufferFormat = codec.getOutputFormat(outputBufferId); // option A
     // bufferFormat is equivalent to mOutputFormat
     // outputBuffer is ready to be processed or rendered.
     …
     codec.releaseOutputBuffer(outputBufferId, …);
   }

   @Override
   void onOutputFormatChanged(MediaCodec mc, MediaFormat format) {
     // Subsequent data will conform to new format.
     // Can ignore if using getOutputFormat(outputBufferId)
     mOutputFormat = format; // option B
   }

   @Override
   void onError(…) {
     …
   }
 });
 codec.configure(format, …);
 mOutputFormat = codec.getOutputFormat(); // option B
 codec.start();
 // wait for processing to complete
 codec.stop();
 codec.release();
```




二）使用缓冲区的同步处理

由于LOLLIPOP，即使在同步模式下使用编解码器时，也应该使用getInput / OutputBuffer（int）和/或getInput / OutputImage（int）检索输入和输出缓冲区。 这允许框架进行某些优化，例如 处理动态内容时。 如果您调用getInput / OutputBuffers（），则此优化将被禁用。


**注意：不要混合同时使用缓冲区和缓冲区数组的方法。 具体而言，只需在start（）之后或者在使用INFO_OUTPUT_FORMAT_CHANGED值出队输出缓冲区ID之后直接调用getInput / OutputBuffers。**


MediaCodec通常在同步模式下使用：


```java
MediaCodec codec = MediaCodec.createByCodecName(name);
 codec.configure(format, …);
 MediaFormat outputFormat = codec.getOutputFormat(); // option B
 codec.start();
 for (;;) {
   int inputBufferId = codec.dequeueInputBuffer(timeoutUs);
   if (inputBufferId >= 0) {
     ByteBuffer inputBuffer = codec.getInputBuffer(…);
     // fill inputBuffer with valid data
     …
     codec.queueInputBuffer(inputBufferId, …);
   }
   int outputBufferId = codec.dequeueOutputBuffer(…);
   if (outputBufferId >= 0) {
     ByteBuffer outputBuffer = codec.getOutputBuffer(outputBufferId);
     MediaFormat bufferFormat = codec.getOutputFormat(outputBufferId); // option A
     // bufferFormat is identical to outputFormat
     // outputBuffer is ready to be processed or rendered.
     …
     codec.releaseOutputBuffer(outputBufferId, …);
   } else if (outputBufferId == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
     // Subsequent data will conform to new format.
     // Can ignore if using getOutputFormat(outputBufferId)
     outputFormat = codec.getOutputFormat(); // option B
   }
 }
 codec.stop();
 codec.release();
```




三）、使用缓冲区阵列的同步处理（不建议使用）

在版本KITKAT_WATCH和之前，输入和输出缓冲区集由ByteBuffer []数组表示。 在成功调用start（）之后，使用getInput / OutputBuffers（）检索缓冲区数组。 如以下示例所示，使用缓冲区ID-s作为这些数组中的索引（非负数时）。 请注意，尽管数组大小提供了上限，但阵列大小与系统使用的输入和输出缓冲区数量之间没有固有的相关性。


```java
MediaCodec codec = MediaCodec.createByCodecName(name);
 codec.configure(format, …);
 codec.start();
 ByteBuffer[] inputBuffers = codec.getInputBuffers();
 ByteBuffer[] outputBuffers = codec.getOutputBuffers();
 for (;;) {
   int inputBufferId = codec.dequeueInputBuffer(…);
   if (inputBufferId >= 0) {
     // fill inputBuffers[inputBufferId] with valid data
     …
     codec.queueInputBuffer(inputBufferId, …);
   }
   int outputBufferId = codec.dequeueOutputBuffer(…);
   if (outputBufferId >= 0) {
     // outputBuffers[outputBufferId] is ready to be processed or rendered.
     …
     codec.releaseOutputBuffer(outputBufferId, …);
   } else if (outputBufferId == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
     outputBuffers = codec.getOutputBuffers();
   } else if (outputBufferId == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
     // Subsequent data will conform to new format.
     MediaFormat format = codec.getOutputFormat();
   }
 }
 codec.stop();
 codec.release();
```


四）、流结束处理

当您到达输入数据的末尾时，您必须通过指定queueInputBuffer调用中的BUFFER_FLAG_END_OF_STREAM标志将其发送给编解码器。 您可以在最后一个有效的输入缓冲区上执行此操作，或者通过提交一个额外的空输入缓冲区并设置流结束标志。 如果使用空缓冲区，时间戳将被忽略。


编解码器将继续返回输出缓冲区，直到它通过指定dequeueOutputBuffer中设置的MediaCodec.BufferInfo中的相同流结束标志或通过onOutputBufferAvailable返回来指示输出流的结束为止。 这可以在最后一个有效的输出缓冲区上设置，或者在最后一个有效的输出缓冲区之后的空缓冲区上设置。 这个空缓冲区的时间戳应该被忽略。


除非编解码器已被刷新，停止并重新启动，否则在发送输入流结束信号之后，请勿提交其他输入缓冲区。





五）、使用输出Surface

使用输出Surface时，数据处理与ByteBuffer模式几乎相同; 但是，输出缓冲区将不可访问，并表示为空值。 例如。 getOutputBuffer / Image（int）将返回null，getOutputBuffers（）将返回仅包含null-s的数组。


使用输出曲面时，可以选择是否渲染曲面上的每个输出缓冲区。 你有三个选择：


1、不要渲染缓冲区：调用releaseOutputBuffer（bufferId，false）。

2、使用默认时间戳呈现缓冲区：调用releaseOutputBuffer（bufferId，true）。

3、使用特定的时间戳呈现缓冲区：调用releaseOutputBuffer（bufferId，timestamp）。

直到M版本，默认时间戳是缓冲区的显示时间戳（转换为纳秒）。


此外，由于M，您可以使用setOutputSurface动态更改输出Surface。





六）、渲染到Sueface时的转换

如果编解码器配置为Surface模式，任何裁剪矩形，旋转和视频缩放模式将自动应用，但有一个例外：


**在M版本之前，软件解码器在渲染到Surface上时可能未应用旋转。 不幸的是，没有标准和简单的方法来识别软件解码器，或者如果他们应用轮换而不是通过尝试它。**


也有一些警告。

**请注意，将输出显示在Surface上时，不考虑像素宽高比。 这意味着如果您使用VIDEO_SCALING_MODE_SCALE_TO_FIT模式，则必须定位输出Surface，以使其具有适当的最终显示宽高比。 相反，对于具有方形像素（像素宽高比或1：1）的内容，您只能使用VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING模式。**

**另请注意，从N版本开始，对于旋转90度或270度的视频，VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING模式可能无法正常工作。**

**设置视频缩放模式时，请注意每次输出缓冲区更改后必须重置。 由于不推荐使用INFO_OUTPUT_BUFFERS_CHANGED事件，因此每次输出格式更改后都可以这样做。**





七）、使用输出Surfzce

使用输入表面时，没有可访问的输入缓冲区，因为缓冲区会自动从输入表面传递到编解码器。 调用dequeueInputBuffer将抛出一个IllegalStateException，并且getInputBuffers（）返回一个不可写入的虚构ByteBuffer []数组。


调用signalEndOfInputStream（）来发信号结束。 此通话后，输入表面将立即停止向编解码器提交数据。








六、寻求和自适应回放支持

视频解码器（以及一般使用压缩视频数据的编解码器）在搜索和格式更改方面表现不同，无论它们是否支持并配置为自适应播放。 您可以检查解码器是否支持通过CodecCapabilities.isFeatureSupported（String）进行自适应播放。 只有将编解码器配置为解码到Surface上时，才会激活对视频解码器的自适应播放支持。





一）、流边界和关键帧

在start（）或flush（）之后的输入数据从合适的流边界开始非常重要：第一个帧必须是关键帧。 关键帧可以完全自行解码（对于大多数编解码器来说，这意味着I帧），并且在关键帧之后没有要显示的帧指的是关键帧之前的帧。


下表总结了各种视频格式的合适关键帧。



|Format|Suitable key frame|
|----|----|
|VP9/VP8|a suitable intraframe where no subsequent frames refer to frames prior to this frame.(There is no specific name for such key frame.)|
|H.265 HEVC|IDR or CRA|
|H.264 AVC|IDR|
|MPEG-4H.263MPEG-2|a suitable I-frame where no subsequent frames refer to frames prior to this frame.(There is no specific name for such key frame.)|

二）、对于不支持自适应回放的解码器（包括不解码到Surface上时）

为了开始解码与先前提交的数据不相邻的数据（即，在寻找之后），你必须清空解码器。 由于所有输出缓冲区在刷新时立即被撤销，因此您可能需要首先发出信号，然后在调用flush之前等待流结束。 冲洗后的输入数据始于合适的流边界/关键帧，这一点很重要。


注意：刷新后提交的数据格式不能改变; flush（）不支持格式不连续性; 为此，完全stop（） - configure（...） - start（）周期是必要的。


另请注意：如果您在start（）后过早刷新编解码器 - 通常在收到第一个输出缓冲区或输出格式更改之前 - 您需要重新提交编解码器特定数据到编解码器。 有关更多信息，请参阅编解码器特定的数据部分。





三）、适用于支持和配置自适应回放的解码器

为了开始解码与先前提交的数据不相邻的数据（即，在寻找之后），不需要刷新解码器; 然而，不连续性之后的输入数据必须从合适的流边界/关键帧开始。


对于某些视频格式（即H.264，H.265，VP8和VP9），也可以在中途更改图片大小或配置。 为此，您必须将全部新的编解码器专用配置数据与关键帧一起打包到单个缓冲区（包括任何启动代码）中，并将其作为常规输入缓冲区提交。


刚刚发生图片大小更改后，以及在返回任何具有新大小的帧之前，您将收到来自dequeueOutputBuffer的INFO_OUTPUT_FORMAT_CHANGED返回值或onOutputFormatChanged回调。


**注意：就像编解码器专用数据的情况一样，在更改图片大小后不久调用flush（）时要小心。 如果您还没有收到图片尺寸更改的确认，则需要重新请求新图片尺寸。**






七、错误处理

工厂方法createByCodecName和createDecoder / EncoderByType在发生失败时抛出IOException，您必须捕获或声明传递失败。 当方法从不允许的编解码器状态调用时，MediaCodec方法抛出IllegalStateException; 这通常是由于不正确的应用程序API使用。 涉及安全缓冲区的方法可能会抛出MediaCodec.CryptoException，该错误信息可从getErrorCode（）获得更多错误信息。


内部编解码器错误导致MediaCodec.CodecException，这可能是由于媒体内容损坏，硬件故障，资源耗尽等等，即使应用程序正确使用该API也是如此。 接收到CodecException时推荐的操作可以通过调用isRecoverable（）和isTransient（）来确定：


1、可恢复错误：如果isRecoverable（）返回true，则调用stop（），configure（...）和start（）以恢复。

2、瞬间错误：如果isTransient（）返回true，则资源暂时不可用，并且该方法可能会在稍后重试。

3、致命错误：如果isRecoverable（）和isTransient（）都返回false，那么CodecException是致命的，编解码器必须重置或释放。

isRecoverable（）和isTransient（）都不会同时返回true。








八、有效的API调用和API历史



本部分总结了每种状态下有效的API调用和MediaCodec类的API历史记录。 有关API版本号，请参阅Build.VERSION_CODES。
|Symbol|Meaning|
|----|----|
|●|支持|
|⁕|语义改变|
|○|实验支持|
|[ ]|废弃|
|⎋|受限于Surface输入模式|
|⎆|受限于Surface输出模式|
|▧|受限于ByteBuffer输出模式|
|↩|受限于同步模式|
|⇄|受限于异步模式|
|( )|可以被调用，但不应该|



Uninitialized





Configured





Flushed





Running





End of Stream





Error





Released


SDK VersionStateMethod1617181920212223`[createByCodecName](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createByCodecName(java.lang.String))`●●●●●●●●`[createDecoderByType](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createDecoderByType(java.lang.String))`●●●●●●●●`[createEncoderByType](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createEncoderByType(java.lang.String))`●●●●●●●●`[createPersistentInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createPersistentInputSurface())`●16+------`[configure](https://developer.android.google.cn/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))`●●●●●⁕●●-18+-----`[createInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createInputSurface())`⎋⎋⎋⎋⎋⎋--16+16+(16+)--`[dequeueInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))`●●▧▧▧⁕▧↩▧↩▧↩--16+16+16+--`[dequeueOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))`●●●●●⁕↩↩↩--16+16+16+--`[flush](https://developer.android.google.cn/reference/android/media/MediaCodec.html#flush())`●●●●●●●●18+18+18+18+18+18+-`[getCodecInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getCodecInfo())`●●●●●●--(21+)21+(21+)--`[getInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputBuffer(int))`●●●--16+(16+)(16+)--`[getInputBuffers](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputBuffers())`●●●●●[⁕↩][↩][↩]-21+(21+)(21+)(21+)--`[getInputFormat](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputFormat())`●●●--(21+)21+(21+)--`[getInputImage](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputImage(int))`○●●18+18+18+18+18+18+-`[getName](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getName())`●●●●●●--(21+)21+21+--`[getOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputBuffer(int))`●●●--16+16+16+--`[getOutputBuffers](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputBuffers())`●●●●●[⁕↩][↩][↩]-21+16+16+16+--`[getOutputFormat()](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputFormat())`●●●●●●●●--(21+)21+21+--`[getOutputFormat(int)](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputFormat(int))`●●●--(21+)21+21+--`[getOutputImage](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputImage(int))`○●●---16+(16+)--`[queueInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))`●●●●●⁕●●---16+(16+)--`[queueSecureInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#queueSecureInputBuffer(int,%20int,%20android.media.MediaCodec.CryptoInfo,%20long,%20int))`●●●●●⁕●●16+16+16+16+16+16+16+`[release](https://developer.android.google.cn/reference/android/media/MediaCodec.html#release())`●●●●●●●●---16+16+--`[releaseOutputBuffer(int, boolean)](https://developer.android.google.cn/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20boolean))`●●●●●⁕●⁕---21+21+--`[releaseOutputBuffer(int, long)](https://developer.android.google.cn/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20long))`⎆⎆⎆21+21+21+21+21+21+-`[reset](https://developer.android.google.cn/reference/android/media/MediaCodec.html#reset())`●●●21+------`[setCallback](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setCallback(android.media.MediaCodec.Callback))`●●`[⁕](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setCallback(android.media.MediaCodec.Callback,%20android.os.Handler))`-23+-----`[setInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setInputSurface(android.view.Surface))`⎋23+23+23+23+23+(23+)(23+)`[setOnFrameRenderedListener](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setOnFrameRenderedListener(android.media.MediaCodec.OnFrameRenderedListener,%20android.os.Handler))`○ ⎆-23+23+23+23+--`[setOutputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setOutputSurface(android.view.Surface))`⎆19+19+19+19+19+(19+)-`[setParameters](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setParameters(android.os.Bundle))`●●●●●-(16+)(16+)16+(16+)(16+)-`[setVideoScalingMode](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setVideoScalingMode(int))`⎆⎆⎆⎆⎆⎆⎆⎆--18+18+---`[signalEndOfInputStream](https://developer.android.google.cn/reference/android/media/MediaCodec.html#signalEndOfInputStream())`⎋⎋⎋⎋⎋⎋-16+21+(⇄)----`[start](https://developer.android.google.cn/reference/android/media/MediaCodec.html#start())`●●●●●⁕●●--16+16+16+--`[stop](https://developer.android.google.cn/reference/android/media/MediaCodec.html#stop())`●●●●●●●●









九、概要


|### 嵌套类| |
|----|----|
|`class`|`[MediaCodec.BufferInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.BufferInfo.html)`每个缓冲区元数据包括指定相关编解码器（输出）缓冲区中有效数据范围的偏移量和大小。|
|`class`|`[MediaCodec.Callback](https://developer.android.google.cn/reference/android/media/MediaCodec.Callback.html)`MediaCodec回调接口。|
|`class`|`[MediaCodec.CodecException](https://developer.android.google.cn/reference/android/media/MediaCodec.CodecException.html)`内部编解码器错误发生时抛出。|
|`class`|`[MediaCodec.CryptoException](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoException.html)`排队安全输入缓冲区时发生加密错误时抛出。|
|`class`|`[MediaCodec.CryptoInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoInfo.html)`描述（至少部分）加密的输入样本结构的元数据。|
|`class`|`[MediaCodec.MetricsConstants](https://developer.android.google.cn/reference/android/media/MediaCodec.MetricsConstants.html)`|
|`interface`|`[MediaCodec.OnFrameRenderedListener](https://developer.android.google.cn/reference/android/media/MediaCodec.OnFrameRenderedListener.html)`在输出表面上渲染输出帧时调用侦听器|


|### 常量| |
|----|----|
|`int`|`[BUFFER_FLAG_CODEC_CONFIG](https://developer.android.google.cn/reference/android/media/MediaCodec.html#BUFFER_FLAG_CODEC_CONFIG)`这表明标记为这样的缓冲区包含编解码器初始化/编解码器特定数据而不是媒体数据。|
|`int`|`[BUFFER_FLAG_END_OF_STREAM](https://developer.android.google.cn/reference/android/media/MediaCodec.html#BUFFER_FLAG_END_OF_STREAM)`这表示流的结束，即|
|`int`|`[BUFFER_FLAG_KEY_FRAME](https://developer.android.google.cn/reference/android/media/MediaCodec.html#BUFFER_FLAG_KEY_FRAME)`这表示标记为（编码）的缓冲区包含关键帧的数据。|
|`int`|`[BUFFER_FLAG_PARTIAL_FRAME](https://developer.android.google.cn/reference/android/media/MediaCodec.html#BUFFER_FLAG_PARTIAL_FRAME)`这表明缓冲区只包含帧的一部分，并且解码器应该对数据进行批处理，直到没有该标志的缓冲区在解码帧之前出现。|
|`int`|`[BUFFER_FLAG_SYNC_FRAME](https://developer.android.google.cn/reference/android/media/MediaCodec.html#BUFFER_FLAG_SYNC_FRAME)`此常数在API级别21中已弃用。请改用BUFFER_FLAG_KEY_FRAME。|
|`int`|`[CONFIGURE_FLAG_ENCODE](https://developer.android.google.cn/reference/android/media/MediaCodec.html#CONFIGURE_FLAG_ENCODE)`如果此编解码器要用作编码器，请传递此标志。|
|`int`|`[CRYPTO_MODE_AES_CBC](https://developer.android.google.cn/reference/android/media/MediaCodec.html#CRYPTO_MODE_AES_CBC)`|
|`int`|`[CRYPTO_MODE_AES_CTR](https://developer.android.google.cn/reference/android/media/MediaCodec.html#CRYPTO_MODE_AES_CTR)`|
|`int`|`[CRYPTO_MODE_UNENCRYPTED](https://developer.android.google.cn/reference/android/media/MediaCodec.html#CRYPTO_MODE_UNENCRYPTED)`|
|`int`|`[INFO_OUTPUT_BUFFERS_CHANGED](https://developer.android.google.cn/reference/android/media/MediaCodec.html#INFO_OUTPUT_BUFFERS_CHANGED)`此常量在API级别21中已弃用。由于getOutputBuffers（）已被弃用，因此可以忽略此返回值。 每次出队时，客户端都应该使用get-buffer或get-image方法的on命令来请求当前缓冲区。|
|`int`|`[INFO_OUTPUT_FORMAT_CHANGED](https://developer.android.google.cn/reference/android/media/MediaCodec.html#INFO_OUTPUT_FORMAT_CHANGED)`输出格式已更改，后续数据将采用新格式。|
|`int`|`[INFO_TRY_AGAIN_LATER](https://developer.android.google.cn/reference/android/media/MediaCodec.html#INFO_TRY_AGAIN_LATER)`如果在对dequeueOutputBuffer（MediaCodec.BufferInfo，long）的调用中指定了非负超时，则表示该呼叫超时。|
|`[String](https://developer.android.google.cn/reference/java/lang/String.html)`|`[PARAMETER_KEY_REQUEST_SYNC_FRAME](https://developer.android.google.cn/reference/android/media/MediaCodec.html#PARAMETER_KEY_REQUEST_SYNC_FRAME)`请求编码器“很快”产生一个同步帧。|
|`[String](https://developer.android.google.cn/reference/java/lang/String.html)`|`[PARAMETER_KEY_SUSPEND](https://developer.android.google.cn/reference/android/media/MediaCodec.html#PARAMETER_KEY_SUSPEND)`临时挂起/恢复输入数据的编码。|
|`[String](https://developer.android.google.cn/reference/java/lang/String.html)`|`[PARAMETER_KEY_VIDEO_BITRATE](https://developer.android.google.cn/reference/android/media/MediaCodec.html#PARAMETER_KEY_VIDEO_BITRATE)`即时更改视频编码器的目标比特率。|
|`int`|`[VIDEO_SCALING_MODE_SCALE_TO_FIT](https://developer.android.google.cn/reference/android/media/MediaCodec.html#VIDEO_SCALING_MODE_SCALE_TO_FIT)`内容缩放到表面尺寸|
|`int`|`[VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING](https://developer.android.google.cn/reference/android/media/MediaCodec.html#VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING)`内容被缩放，保持其宽高比，整个表面区域被使用，内容可能被裁剪。|


|### 公共方法| |
|----|----|
|`void`|`[configure](https://developer.android.google.cn/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20android.media.MediaCrypto,%20int))([MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html) format, [Surface](https://developer.android.google.cn/reference/android/view/Surface.html) surface, [MediaCrypto](https://developer.android.google.cn/reference/android/media/MediaCrypto.html) crypto, int flags)`配置组件。|
|`void`|`[configure](https://developer.android.google.cn/reference/android/media/MediaCodec.html#configure(android.media.MediaFormat,%20android.view.Surface,%20int,%20android.media.MediaDescrambler))([MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html) format, [Surface](https://developer.android.google.cn/reference/android/view/Surface.html) surface, int flags, [MediaDescrambler](https://developer.android.google.cn/reference/android/media/MediaDescrambler.html) descrambler)`配置要与解扰器一起使用的组件。|
|`static [MediaCodec](https://developer.android.google.cn/reference/android/media/MediaCodec.html)`|`[createByCodecName](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createByCodecName(java.lang.String))([String](https://developer.android.google.cn/reference/java/lang/String.html) name)`如果您知道要实例化的组件的确切名称，请使用此方法将其实例化。|
|`static [MediaCodec](https://developer.android.google.cn/reference/android/media/MediaCodec.html)`|`[createDecoderByType](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createDecoderByType(java.lang.String))([String](https://developer.android.google.cn/reference/java/lang/String.html) type)`实例化支持给定MIME类型的输入数据的首选解码器。|
|`static [MediaCodec](https://developer.android.google.cn/reference/android/media/MediaCodec.html)`|`[createEncoderByType](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createEncoderByType(java.lang.String))([String](https://developer.android.google.cn/reference/java/lang/String.html) type)`实例化支持给定MIME类型的输出数据的首选编码器。|
|`[Surface](https://developer.android.google.cn/reference/android/view/Surface.html)`|`[createInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createInputSurface())()`请求Surface用作编码器的输入，以代替输入缓冲区。|
|`static [Surface](https://developer.android.google.cn/reference/android/view/Surface.html)`|`[createPersistentInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#createPersistentInputSurface())()`创建可与通常具有输入表面的编解码器一起使用的持久输入表面，例如视频编码器。|
|`int`|`[dequeueInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#dequeueInputBuffer(long))(long timeoutUs)`返回要用有效数据填充的输入缓冲区的索引，如果当前没有可用的缓冲区，则返回-1。|
|`int`|`[dequeueOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#dequeueOutputBuffer(android.media.MediaCodec.BufferInfo,%20long))([MediaCodec.BufferInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.BufferInfo.html) info, long timeoutUs)`出队输出缓冲区，最多阻止“timeoutUs”微秒。|
|`void`|`[flush](https://developer.android.google.cn/reference/android/media/MediaCodec.html#flush())()`冲洗组件的输入和输出端口。|
|`[MediaCodecInfo](https://developer.android.google.cn/reference/android/media/MediaCodecInfo.html)`|`[getCodecInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getCodecInfo())()`获取编解码器信息。|
|`[ByteBuffer](https://developer.android.google.cn/reference/java/nio/ByteBuffer.html)`|`[getInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputBuffer(int))(int index)`返回已清除的可写ByteBuffer对象，用于包含输入数据的出列输入缓冲区索引。|
|`[ByteBuffer[]](https://developer.android.google.cn/reference/java/nio/ByteBuffer.html)`|`[getInputBuffers](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputBuffers())()`此方法在API级别21中已弃用。每次输入缓冲区出列时，请使用新的getInputBuffer（int）方法。 注意：自API 21起，自动清除出队输入缓冲区。 如果使用输入表面，请勿使用此方法。|
|`[MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html)`|`[getInputFormat](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputFormat())()`在configure（MediaFormat，Surface，MediaCrypto，int）成功返回以获取编解码器接受的输入格式后调用此函数。|
|`[Image](https://developer.android.google.cn/reference/android/media/Image.html)`|`[getInputImage](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getInputImage(int))(int index)`返回出队输入缓冲区索引的可写Image对象，以包含原始输入视频帧。|
|`[PersistableBundle](https://developer.android.google.cn/reference/android/os/PersistableBundle.html)`|`[getMetrics](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getMetrics())()`返回有关当前编解码器实例的度量标准数据。|
|`[String](https://developer.android.google.cn/reference/java/lang/String.html)`|`[getName](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getName())()`获取组件名称。|
|`[ByteBuffer](https://developer.android.google.cn/reference/java/nio/ByteBuffer.html)`|`[getOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputBuffer(int))(int index)`返回出队输出缓冲区索引的只读ByteBuffer。|
|`[ByteBuffer[]](https://developer.android.google.cn/reference/java/nio/ByteBuffer.html)`|`[getOutputBuffers](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputBuffers())()`此方法在API级别21中已弃用。每次输出缓冲区出列时，请使用新的getOutputBuffer（int）方法。 如果编解码器配置为异步模式，则不支持此方法。 注意：从API 21开始，出队的输出缓冲区的位置和限制将被设置为有效的数据范围。 如果使用输出表面，请勿使用此方法。|
|`[MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html)`|`[getOutputFormat](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputFormat(int))(int index)`返回特定输出缓冲区的输出格式。|
|`[MediaFormat](https://developer.android.google.cn/reference/android/media/MediaFormat.html)`|`[getOutputFormat](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputFormat())()`通过返回INFO_OUTPUT_FORMAT_CHANGED，dequeueOutputBuffer发出格式更改后调用此函数。|
|`[Image](https://developer.android.google.cn/reference/android/media/Image.html)`|`[getOutputImage](https://developer.android.google.cn/reference/android/media/MediaCodec.html#getOutputImage(int))(int index)`返回包含原始视频帧的出列输出缓冲区索引的只读Image对象。|
|`void`|`[queueInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#queueInputBuffer(int,%20int,%20int,%20long,%20int))(int index, int offset, int size, long presentationTimeUs, int flags)`在指定索引处填充输入缓冲区的范围后，将其提交给组件。|
|`void`|`[queueSecureInputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#queueSecureInputBuffer(int,%20int,%20android.media.MediaCodec.CryptoInfo,%20long,%20int))(int index, int offset, [MediaCodec.CryptoInfo](https://developer.android.google.cn/reference/android/media/MediaCodec.CryptoInfo.html) info, long presentationTimeUs, int flags)`与queueInputBuffer类似，但提交可能加密的缓冲区。|
|`void`|`[release](https://developer.android.google.cn/reference/android/media/MediaCodec.html#release())()`释放编解码器实例使用的资源。|
|`void`|`[releaseOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20boolean))(int index, boolean render)`如果完成了缓冲区，则使用此调用将缓冲区返回给编解码器或将其呈现在输出表面上。|
|`void`|`[releaseOutputBuffer](https://developer.android.google.cn/reference/android/media/MediaCodec.html#releaseOutputBuffer(int,%20long))(int index, long renderTimestampNs)`如果已完成缓冲区，则使用此调用更新其表面时间戳并将其返回给编解码器以在输出表面上呈现它。|
|`void`|`[reset](https://developer.android.google.cn/reference/android/media/MediaCodec.html#reset())()`将编解码器返回到其初始（未初始化）状态。|
|`void`|`[setCallback](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setCallback(android.media.MediaCodec.Callback,%20android.os.Handler))([MediaCodec.Callback](https://developer.android.google.cn/reference/android/media/MediaCodec.Callback.html) cb, [Handler](https://developer.android.google.cn/reference/android/os/Handler.html) handler)`为可操作的MediaCodec事件设置异步回调。|
|`void`|`[setCallback](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setCallback(android.media.MediaCodec.Callback))([MediaCodec.Callback](https://developer.android.google.cn/reference/android/media/MediaCodec.Callback.html) cb)`为默认循环中的可操作MediaCodec事件设置异步回调。|
|`void`|`[setInputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setInputSurface(android.view.Surface))([Surface](https://developer.android.google.cn/reference/android/view/Surface.html) surface)`配置编解码器（例如|
|`void`|`[setOnFrameRenderedListener](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setOnFrameRenderedListener(android.media.MediaCodec.OnFrameRenderedListener,%20android.os.Handler))([MediaCodec.OnFrameRenderedListener](https://developer.android.google.cn/reference/android/media/MediaCodec.OnFrameRenderedListener.html) listener, [Handler](https://developer.android.google.cn/reference/android/os/Handler.html) handler)`注册在输出表面上呈现输出帧时要调用的回调。|
|`void`|`[setOutputSurface](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setOutputSurface(android.view.Surface))([Surface](https://developer.android.google.cn/reference/android/view/Surface.html) surface)`动态设置编解码器的输出表面。|
|`void`|`[setParameters](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setParameters(android.os.Bundle))([Bundle](https://developer.android.google.cn/reference/android/os/Bundle.html) params)`将其他参数更改传递给组件实例。|
|`void`|`[setVideoScalingMode](https://developer.android.google.cn/reference/android/media/MediaCodec.html#setVideoScalingMode(int))(int mode)`如果在之前的配置调用中指定了曲面（MediaFormat，Surface，MediaCrypto，int），则指定要使用的缩放模式。|
|`void`|`[signalEndOfInputStream](https://developer.android.google.cn/reference/android/media/MediaCodec.html#signalEndOfInputStream())()`输入信号结束码。|
|`void`|`[start](https://developer.android.google.cn/reference/android/media/MediaCodec.html#start())()`成功配置组件后，调用start。|
|`void`|`[stop](https://developer.android.google.cn/reference/android/media/MediaCodec.html#stop())()`完成解码/编码会话，请注意编解码器实例保持活动状态并准备好再次开始（）。|


|### 保护方法| |
|----|----|
|`void`|`[finalize](https://developer.android.google.cn/reference/android/media/MediaCodec.html#finalize())()`当垃圾收集确定没有更多对该对象的引用时，由对象上的垃圾回收器调用。|


|### 继承方法|
|----|
|From class `[java.lang.Object](https://developer.android.google.cn/reference/java/lang/Object.html)`|



