# LiveMediaStreamer - DoubleLi - 博客园






LiveMediaStreamer is an open source multimedia framework that allows the manipulation of multiple audio and video streams in real-time in many possible data flow configurations (called scenarios) through a JSON formatted TCP socket APIor by using the middlware RESTfull API. It is designed following a pipeline pattern. It consists in a number of filters that can be concatenated or connected with each other in order to process a desired data flow. Current filters are:
- **Receivers**: RTSP, RTP, MPEG-TS and RTMP input protocols.
- **Transmitters**: RTSP, RTP, MPEG-TS and MPEG-DASH output protocols.
- **Encoders**: H.264, H.265 and VP9 video codecs and AAC, OPUS and G711 audio codecs support.
- **Decoders**: Same as encoders.
- **Resamplers**: Video resizer or audio resampler.
- **Mixers**: Audio or video mixing filters with configurable layouts (PiP, NxM, …) and effects (fade, blend, crop, …).

### .. when is it suitable to be used?

LiveMediaStreamer framework offers ease to create specific scenarios, from simple to complex configurations.

Its basic use cases might be:
- **Live****streaming**: RTP, RTSP, MPEG-TS and MPEG-DASH
- **Ingestion and transcoding**: streaming network protocols and codecs translation
- **Live audio and video mixing**: real-time configure many audio and video inputs to a desired final layout.









