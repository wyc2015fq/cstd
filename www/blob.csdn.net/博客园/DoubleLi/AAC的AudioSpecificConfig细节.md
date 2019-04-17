# AAC的AudioSpecificConfig细节 - DoubleLi - 博客园






AAC格式里有个复杂的AudioSpecificConfig, 在FLV格式里称为AAC sequence header。在正式播放ADTS AAC数据包之前，需要用AudioSpecificConfig生成一个audio_tag交给播放器。

其实AudioSpecificConfig的内容，完全可以通过ADTS的7字节头生成，具体来说，AudioSpecificConfig需要3部分数据：profile(LC,Main,HE)，sample_rate, channel，这3个数据在ADTS头里都可以找到。具体算法见代码。

有了这3个数据以后，可以进行合并生成2个字节，就是所谓的AudioSpecificConfig，见代码部分config1/config2的算法。



//output AAC config header:



var profile:int = ((payload[2]&0xc0)>>6)+1;

var sample_rate:int = (payload[2]&0x3c)>>2;

var channel:int = ((payload[2]&0x1)<<2)|((payload[3]&0xc0)>>6);



var config1:int = (profile<<3)|((sample_rate&0xe)>>1);

var config2:int = ((sample_rate&0x1)<<7)|(channel<<3);



var aacSeqHeader:ByteArray = new ByteArray();

aacSeqHeader.writeByte(config1);

aacSeqHeader.writeByte(config2);



根据第一个ADTS包生成的aacSeqHeader，打包到audio_tag里，交给NetStream，后面的ADTS包就可以直接打包成audio_tag播放了。









