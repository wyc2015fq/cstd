# hisi出的H264码流结构 - DoubleLi - 博客园






hisi出的H264码流结构：





IDR帧结构如下：

开始码 + nalu + I帧    +    开始码 + nalu + SPS    +     开始码 + nalu + PPS    +   开始码 + nalu + SEI

开始码为：00000001



帧序列结构如下：

IDR帧  +  P帧 + +  P帧 + ...... + IDR帧  +  P帧 + +  P帧 + ...... + IDR帧  +  P帧 + +  P帧 + ......









