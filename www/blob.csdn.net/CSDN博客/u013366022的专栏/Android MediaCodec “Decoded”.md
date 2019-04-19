# Android MediaCodec “Decoded” - u013366022的专栏 - CSDN博客
2014年05月14日 15:25:35[slitaz](https://me.csdn.net/u013366022)阅读数：691
Android has a great media library allowing all sorts of things. Until recently though, there was no way to encode/decode audio/video
 giving developers the ability to do literally anything. Fortunately Jelly Bean release introduced the android.media.MediaCodec API.The
 API is designed following the same principles/architecture of  OpenMAX, a well known standard in the media Industry.Transitioning from a pure high level MediaPlayer to the encoder/decoder level can be a big pain though. There is a lot more to be aware of when
 you are manipulating the tiny little bits that make great media file:///C:/Users/Kimi/AppData/Local/youdao/ynote/images/F6ACB9CA72804E18A9E34F00B95C9B50/icon_smile.gifIn this post I will describe how to use the API, highlighting the essential things to be
 aware of.1.Get To Know Your MediaAnother new class introduced in Jelly Bean is theandroid.media.MediaExtractor.
 It is pretty clear what it is all about, extract the metadata from your media and a lot more.
AssetFileDescriptor sampleFD = getResources().openRawResourceFd(
                                R.raw.test);
                MediaExtractor extractor;
                MediaCodec codec;
                ByteBuffer[] codecInputBuffers;
                ByteBuffer[] codecOutputBuffers;
                extractor = new MediaExtractor();
                extractor.setDataSource(sampleFD.getFileDescriptor(),
                                sampleFD.getStartOffset(), sampleFD.getLength());
                Log.d(TAG, String.format("TRACKS #: %d", extractor.getTrackCount()));
                MediaFormat format = extractor.getTrackFormat(0);
                String mime = format.getString(MediaFormat.KEY_MIME);
                Log.d(TAG, String.format("MIME TYPE : %s", mime));
2. Create your DecoderA decoder is generally seen as a NODE with INPUT and OUTPUT buffers. You take an input buffer from it,
 fill it and give it back to the decoder for decoding to take place. On the other side of the NODE, you take an output buffer and “render” it. This example will play an audio sample file using theandroid.media.AudioTrack API.
[backcolor=initial !important]AssetFileDescriptor
 sampleFD = getResources().openRawResourceFd(
                                R.raw.test);
                MediaExtractor extractor;
                MediaCodec codec;
                ByteBuffer[] codecInputBuffers;
                ByteBuffer[] codecOutputBuffers;
                extractor = new MediaExtractor();
                extractor.setDataSource(sampleFD.getFileDescriptor(),
                                sampleFD.getStartOffset(), sampleFD.getLength());
                Log.d(TAG, String.format("TRACKS #: %d", extractor.getTrackCount()));
                MediaFormat format = extractor.getTrackFormat(0);
                String mime = format.getString(MediaFormat.KEY_MIME);
                Log.d(TAG, String.format("MIME TYPE : %s", mime));
3. It`s All About BuffersLet the Buffer party begin file:///C:/Users/Kimi/AppData/Local/youdao/ynote/images/928F1025FB274DA4AD6B4861A5CEF454/icon_smile.gif
 See bellow how the INPUT side of the decoder is managed:
int inputBufIndex = codec.dequeueInputBuffer(TIMEOUT_US);
                if (inputBufIndex >= 0) {
                        ByteBuffer dstBuf = codecInputBuffers[inputBufIndex];
                        int sampleSize = extractor.readSampleData(dstBuf, 0);
                        long presentationTimeUs = 0;
                        if (sampleSize < 0) {
                                sawInputEOS = true;
                                sampleSize = 0;
                        } else {
                                presentationTimeUs = extractor.getSampleTime();
                        }
                        codec.queueInputBuffer(inputBufIndex, 0, sampleSize,
                                        presentationTimeUs,
                                        sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);
                        if(!sawInputEOS){
                                extractor.advance();
                        }
                }
And now how to pull OUTPUT buffers with the decoded media from the decoder:
final int res = codec.dequeueOutputBuffer(info, TIMEOUT_US);
                if(res >= 0){
                        int outputBufIndex = res;
                        ByteBuffer buf = codecOutputBuffers[outputBufIndex];
                        final byte[] chunk = new byte[info.size];
                        buf.get(chunk);
                        buf.clear();
                        if(chunk.length > 0){
                                audioTrack.write(chunk,0,chunk.length);
                        }
                        codec.releaseOutputBuffer(outputBufIndex, false);
                        if((info.flags && MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0){
                                sawOutputEOS = true;
                        }
                } else if(res == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED){
                        codecOutputBuffers = codec.getOutputBuffers();
                } else if(res == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED){
                        final MediaFormat offormat = codec.getOutputFormat();
                        Log.d(TAG, "Output format has changed to" + offormat);
                        mAudioTrack.setPlaybackRate(oformat.getInteger(MediaFormat.KEY_SAMPLE_RATE));
                }
And that’s it. This is the most simple usage of this such powerful API. For further questions send me a note and I’ll give
 you more insights…
