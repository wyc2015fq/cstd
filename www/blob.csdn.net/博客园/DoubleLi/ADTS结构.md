# ADTS结构 - DoubleLi - 博客园






ADTS全称是(Audio Data Transport Stream)，是AAC的一种十分常见的传输格式。转载请注明来自：http://www.binkery.com/

ADTS内容及结构
一般情况下ADTS的头信息都是7个字节，分为2部分：
adts_fixed_header();
adts_variable_header();
转载请注明来自：http://www.binkery.com/

转载请注明来自：http://www.binkery.com/

syntax
adts_fixed_header(){
    syncword; 12 bslbf
    ID; 1 bslbf
    layer;2 uimsbf
    protection_absent;1 bslbf
    profile;2 uimsbf
    sampling_frequency_index;4 uimsbf
    private_bit;1 bslbf
    channel_configuration;3 uimsbf
    original_copy;1 bslbf
    home;1 bslbf
}
转载请注明来自：http://www.binkery.com/

syncword ：同步头 总是0xFFF, all bits must be 1，代表着一个ADTS帧的开始转载请注明来自：http://www.binkery.com/

ID：MPEG Version: 0 for MPEG-4, 1 for MPEG-2转载请注明来自：http://www.binkery.com/

Layer：always: '00'转载请注明来自：http://www.binkery.com/

profile：表示使用哪个级别的AAC，有些芯片只支持AAC LC 。在MPEG-2 AAC中定义了3种：
    0:Main profile
    1:Low Complexity profile(LC)
    2:Scalable sampling Rate Profile(SSR)
    3:Reserved
转载请注明来自：http://www.binkery.com/

sampling_frequency_index：表示使用的采样率下标，通过这个下标在 Sampling Frequencies[ ]数组中查找得知采样率的值。
There are 13 supported frequencies:
    0: 96000 Hz
    1: 88200 Hz
    2: 64000 Hz
    3: 48000 Hz
    4: 44100 Hz
    5: 32000 Hz
    6: 24000 Hz
    7: 22050 Hz
    8: 16000 Hz
    9: 12000 Hz
    10: 11025 Hz
    11: 8000 Hz
    12: 7350 Hz
    13: Reserved
    14: Reserved
    15: frequency is written explictly
转载请注明来自：http://www.binkery.com/

channel_configuration: 表示声道数
    0: Defined in AOT Specifc Config
    1: 1 channel: front-center
    2: 2 channels: front-left, front-right
    3: 3 channels: front-center, front-left, front-right
    4: 4 channels: front-center, front-left, front-right, back-center
    5: 5 channels: front-center, front-left, front-right, back-left, back-right
    6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
    7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
    8-15: Reserved转载请注明来自：http://www.binkery.com/

syntax
adts_variable_header(){
    copyright_identiflaction_bit;1 bslbf
    copyright_identification_start;1 bslbf
    acc_frame_length;13 bslbf;
    adts_buffer_fullness; 11 bslbf
    number_of_raw_data_blocks_in_frame;2 uimsfb
}转载请注明来自：http://www.binkery.com/

frame_length : 一个ADTS帧的长度包括ADTS头和AAC原始流.转载请注明来自：http://www.binkery.com/

adts_buffer_fullness：0x7FF 说明是码率可变的码流转载请注明来自：http://www.binkery.com/

下面是来自一个国外网站的介绍。http://wiki.multimedia.cx/index.php?title=ADTS  个人觉得这个网站的这个表结构的说明更加的形象一些。很好理解。1byte = 8 bits ， 每个字段占用多少bits，各个字段的意义。转载请注明来自：http://www.binkery.com/

Audio Data Transport Stream (ADTS) is a format, used by MPEG TS or Shoutcast to stream audio, usually AAC.转载请注明来自：http://www.binkery.com/

Structure
AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP (QQQQQQQQ QQQQQQQQ)
Header consists of 7 or 9 bytes (without or with CRC).
转载请注明来自：http://www.binkery.com/

Letter	Length (bits)	Description
A     12     syncword 0xFFF, all bits must be 1
B     1       MPEG Version: 0 for MPEG-4, 1 for MPEG-2
C     2       Layer: always 0
D     1       protection absent, Warning, set to 1 if there is no CRC and 0 if there is CRC
E     2       profile, the MPEG-4 Audio Object Type minus 1
F     4       MPEG-4 Sampling Frequency Index (15 is forbidden)
G     1       private stream, set to 0 when encoding, ignore when decoding
H     3       MPEG-4 Channel Configuration (in the case of 0, the channel configuration is sent via an inband 

PCE)
I      1       originality, set to 0 when encoding, ignore when decoding
J      1       home, set to 0 when encoding, ignore when decoding
K     1       copyrighted stream, set to 0 when encoding, ignore when decoding
L     1        copyright start, set to 0 when encoding, ignore when decoding
M   13      frame length, this value must include 7 or 9 bytes of header length: FrameLength = 

(ProtectionAbsent == 1 ? 7 : 9) + size(AACFrame)
O    11       Buffer fullness
P     2        Number of AAC frames (RDBs) in ADTS frame minus 1, for maximum compatibility always use 1 AAC frame 

per ADTS frame
Q     16     CRC if protection absent is 0
转载请注明来自：http://www.binkery.com/

Usage in MPEG-TS
ADTS packet must be a content of PES packet. Pack AAC data inside ADTS frame, than pack inside PES packet, then mux 

by TS packetizer.转载请注明来自：http://www.binkery.com/

Usage in Shoutcast
ADTS frames goes one by one in TCP stream. Look for syncword, parse header and look for next syncword after.

from：http://www.binkery.com/









