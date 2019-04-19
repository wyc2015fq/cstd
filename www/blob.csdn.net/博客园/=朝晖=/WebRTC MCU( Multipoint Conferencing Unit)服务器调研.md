# WebRTC MCU( Multipoint Conferencing Unit)服务器调研 - =朝晖= - 博客园
# [WebRTC MCU( Multipoint Conferencing Unit)服务器调研](https://www.cnblogs.com/dhcn/p/7124837.html)
接触过的有licode、kurento.
licode的缺陷：文档支持有限，licode的app client库只有[js](http://lib.csdn.net/base/javascript)的
kurento的优势：文档齐全，Demo俱备，封装API比较齐全。它的主要特性：
- Networked streaming protocols, including [HTTP](http://doc-kurento.readthedocs.io/en/stable/glossary.html#term-http), [RTP](http://doc-kurento.readthedocs.io/en/stable/glossary.html#term-rtp) and [WebRTC](http://doc-kurento.readthedocs.io/en/stable/glossary.html#term-webrtc).
- Group communications (MCUs( Multipoint Conferencing Unit) and SFUs(Selective Forwarding Unit.) functionality) supporting both media mixing and media routing/dispatching.
- Generic support for computational vision and augmented reality filters.
- Media storage supporting writing operations for [WebM](http://doc-kurento.readthedocs.io/en/stable/glossary.html#term-webm) and [MP4](http://doc-kurento.readthedocs.io/en/stable/glossary.html#term-mp4) and playing in all formats supported by GStreamer.
- Automatic media transcodification between any of the codecs supported by GStreamer including VP8, H.264, H.263, AMR, OPUS, Speex, G.711, etc.
除了以上特性，kurento对于我们要做的WebRTC MCU服务，有专用的Room API，方便实现此类功能。其丰富的视频处理功能也是选用这个解决方案的重要原因之一。也可以基于kurento做Peer2Peer的信令服务。

