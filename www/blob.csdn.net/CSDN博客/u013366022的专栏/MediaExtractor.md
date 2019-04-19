# MediaExtractor - u013366022的专栏 - CSDN博客
2014年05月21日 10:21:17[slitaz](https://me.csdn.net/u013366022)阅读数：1009
android.toolib.net/reference/android/media/MediaExtractor.html
MediaExtractor extractor =newMediaExtractor();
 extractor.setDataSource(...);int numTracks = extractor.getTrackCount();for(int i =0; i < numTracks;++i){MediaFormat format = extractor.getTrackFormat(i);String mime = format.getString(MediaFormat.KEY_MIME);if(weAreInterestedInThisTrack){
     extractor.selectTrack(i);}}ByteBuffer inputBuffer =ByteBuffer.allocate(...)while(extractor.readSampleData(inputBuffer,...)>=0){int trackIndex = extractor.getSampleTrackIndex();long presentationTimeUs = extractor.getSampleTime();...
   extractor.advance();}
 extractor.release();
 extractor =null;
|||
