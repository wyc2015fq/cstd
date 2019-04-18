# webrtc交互清单 - tifentan的专栏 - CSDN博客

2016年12月12日 09:23:04[露蛇](https://me.csdn.net/tifentan)阅读数：521


根据http://www.blogjava.net/linli/archive/2014/10/21/418910.html的例子调试。

记录下两个peer的交互流程。

received (second socket):  { event: '_offer',

  data:

   { sdp:

      { type: 'offer',

        sdp: 'v=0\r\no=- 3083476675802231399 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE audio video\r\na=msid-semantic: WMS NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU\r\nm=audio 9 RTP/SAVPF 111 103 104 9 0 8 106 105 13 126\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9
 IN IP4 0.0.0.0\r\na=ice-ufrag:m41jPpea8jajXxCL\r\na=ice-pwd:VAoMacTezRKuB0cmzTrbU7gg\r\na=fingerprint:sha-256 18:27:3D:17:2A:A5:41:E6:39:F4:EF:AF:1C:0A:10:15:DB:44:6E:D0:8E:4D:C8:20:4B:B9:7B:4F:90:D4:03:8C\r\na=setup:actpass\r\na=mid:audio\r\na=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\na=extmap:3
 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=sendrecv\r\na=rtcp-mux\r\na=rtpmap:111 opus/48000/2\r\na=fmtp:111 minptime=10; useinbandfec=1\r\na=rtpmap:103 ISAC/16000\r\na=rtpmap:104 ISAC/32000\r\na=rtpmap:9 G722/8000\r\na=rtpmap:0 PCMU/8000\r\na=rtpmap:8
 PCMA/8000\r\na=rtpmap:106 CN/32000\r\na=rtpmap:105 CN/16000\r\na=rtpmap:13 CN/8000\r\na=rtpmap:126 telephone-event/8000\r\na=maxptime:60\r\na=ssrc:451631496 cname:WSxE1BICjBmW8NU8\r\na=ssrc:451631496 msid:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU 9e3c1e78-ae3a-484d-96f4-28bf0bc3f568\r\na=ssrc:451631496
 mslabel:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU\r\na=ssrc:451631496 label:9e3c1e78-ae3a-484d-96f4-28bf0bc3f568\r\nm=video 9 RTP/SAVPF 100 116 117 96\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9 IN IP4 0.0.0.0\r\na=ice-ufrag:m41jPpea8jajXxCL\r\na=ice-pwd:VAoMacTezRKuB0cmzTrbU7gg\r\na=fingerprint:sha-256
 18:27:3D:17:2A:A5:41:E6:39:F4:EF:AF:1C:0A:10:15:DB:44:6E:D0:8E:4D:C8:20:4B:B9:7B:4F:90:D4:03:8C\r\na=setup:actpass\r\na=mid:video\r\na=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\na=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=extmap:4
 urn:3gpp:video-orientation\r\na=sendrecv\r\na=rtcp-mux\r\na=rtpmap:100 VP8/90000\r\na=rtcp-fb:100 ccm fir\r\na=rtcp-fb:100 nack\r\na=rtcp-fb:100 nack pli\r\na=rtcp-fb:100 goog-remb\r\na=rtpmap:116 red/90000\r\na=rtpmap:117 ulpfec/90000\r\na=rtpmap:96 rtx/90000\r\na=fmtp:96
 apt=100\r\na=ssrc-group:FID 1110212624 3816620239\r\na=ssrc:1110212624 cname:WSxE1BICjBmW8NU8\r\na=ssrc:1110212624 msid:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU 44eefc7e-852f-4bc1-bb80-a3a8c3b33a60\r\na=ssrc:1110212624 mslabel:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU\r\na=ssrc:1110212624
 label:44eefc7e-852f-4bc1-bb80-a3a8c3b33a60\r\na=ssrc:3816620239 cname:WSxE1BICjBmW8NU8\r\na=ssrc:3816620239 msid:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU 44eefc7e-852f-4bc1-bb80-a3a8c3b33a60\r\na=ssrc:3816620239 mslabel:NhOPRUJAu8qdLilSTpiLkZp71U6QnslECeuU\r\na=ssrc:3816620239
 label:44eefc7e-852f-4bc1-bb80-a3a8c3b33a60\r\n' } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 1 udp 2122260223 10.10.10.1 57888 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 1 udp 2122194687 192.168.128.1 57889 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 1 udp 2122129151 192.168.1.222 57890 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 2 udp 2122260222 10.10.10.1 57891 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 2 udp 2122194686 192.168.128.1 57892 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 2 udp 2122129150 192.168.1.222 57893 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 1 udp 2122260223 10.10.10.1 57894 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 1 udp 2122194687 192.168.128.1 57895 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 1 udp 2122129151 192.168.1.222 57896 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 2 udp 2122260222 10.10.10.1 57897 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 2 udp 2122194686 192.168.128.1 57898 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 2 udp 2122129150 192.168.1.222 57899 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 1 tcp 1518280447 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 1 tcp 1518214911 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 1 tcp 1518149375 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 2 tcp 1518280446 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 2 tcp 1518214910 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 2 tcp 1518149374 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 1 tcp 1518280447 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 1 tcp 1518214911 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 1 tcp 1518149375 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 2 tcp 1518280446 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 2 tcp 1518214910 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (second socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 2 tcp 1518149374 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }
received (first socket):  { event: '_answer',

  data:

   { sdp:

      { type: 'answer',

        sdp: 'v=0\r\no=- 8738428608199472176 2 IN IP4 127.0.0.1\r\ns=-\r\nt=0 0\r\na=group:BUNDLE audio video\r\na=msid-semantic: WMS q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP\r\nm=audio 9 RTP/SAVPF 111 103 104 9 0 8 106 105 13 126\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9
 IN IP4 0.0.0.0\r\na=ice-ufrag:FsthAqVthuINOsne\r\na=ice-pwd:gzlialX923tXkbuLF3doxWs8\r\na=fingerprint:sha-256 18:27:3D:17:2A:A5:41:E6:39:F4:EF:AF:1C:0A:10:15:DB:44:6E:D0:8E:4D:C8:20:4B:B9:7B:4F:90:D4:03:8C\r\na=setup:active\r\na=mid:audio\r\na=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level\r\na=extmap:3
 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=sendrecv\r\na=rtcp-mux\r\na=rtpmap:111 opus/48000/2\r\na=fmtp:111 minptime=10; useinbandfec=1\r\na=rtpmap:103 ISAC/16000\r\na=rtpmap:104 ISAC/32000\r\na=rtpmap:9 G722/8000\r\na=rtpmap:0 PCMU/8000\r\na=rtpmap:8
 PCMA/8000\r\na=rtpmap:106 CN/32000\r\na=rtpmap:105 CN/16000\r\na=rtpmap:13 CN/8000\r\na=rtpmap:126 telephone-event/8000\r\na=maxptime:60\r\na=ssrc:2143630110 cname:z7GDdJaSfAbaKRdk\r\na=ssrc:2143630110 msid:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP 28d4c1b0-5cd7-4e48-8814-77f8abbaf012\r\na=ssrc:2143630110
 mslabel:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP\r\na=ssrc:2143630110 label:28d4c1b0-5cd7-4e48-8814-77f8abbaf012\r\nm=video 9 RTP/SAVPF 100 116 117 96\r\nc=IN IP4 0.0.0.0\r\na=rtcp:9 IN IP4 0.0.0.0\r\na=ice-ufrag:FsthAqVthuINOsne\r\na=ice-pwd:gzlialX923tXkbuLF3doxWs8\r\na=fingerprint:sha-256
 18:27:3D:17:2A:A5:41:E6:39:F4:EF:AF:1C:0A:10:15:DB:44:6E:D0:8E:4D:C8:20:4B:B9:7B:4F:90:D4:03:8C\r\na=setup:active\r\na=mid:video\r\na=extmap:2 urn:ietf:params:rtp-hdrext:toffset\r\na=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time\r\na=extmap:4
 urn:3gpp:video-orientation\r\na=sendrecv\r\na=rtcp-mux\r\na=rtpmap:100 VP8/90000\r\na=rtcp-fb:100 ccm fir\r\na=rtcp-fb:100 nack\r\na=rtcp-fb:100 nack pli\r\na=rtcp-fb:100 goog-remb\r\na=rtpmap:116 red/90000\r\na=rtpmap:117 ulpfec/90000\r\na=rtpmap:96 rtx/90000\r\na=fmtp:96
 apt=100\r\na=ssrc-group:FID 541212073 1408721624\r\na=ssrc:541212073 cname:z7GDdJaSfAbaKRdk\r\na=ssrc:541212073 msid:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP bdf00931-0706-4c4d-9c64-6a4e40e502ae\r\na=ssrc:541212073 mslabel:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP\r\na=ssrc:541212073
 label:bdf00931-0706-4c4d-9c64-6a4e40e502ae\r\na=ssrc:1408721624 cname:z7GDdJaSfAbaKRdk\r\na=ssrc:1408721624 msid:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP bdf00931-0706-4c4d-9c64-6a4e40e502ae\r\na=ssrc:1408721624 mslabel:q6pCbvfXde5RbF37O0y94EvI8r4DirTuNBJP\r\na=ssrc:1408721624
 label:bdf00931-0706-4c4d-9c64-6a4e40e502ae\r\n' } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 1 tcp 1518280447 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1256157767 1 tcp 1518280447 10.10.10.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 1 tcp 1518214911 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 1 tcp 1518149375 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 1 udp 2122260223 10.10.10.1 57900 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:490429788 1 tcp 1518214911 192.168.128.1 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:2876988909 1 tcp 1518149375 192.168.1.222 0 typ host tcptype active generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:73174711 1 udp 2122260223 10.10.10.1 57900 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 1 udp 2122194687 192.168.128.1 57901 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:1404608940 1 udp 2122194687 192.168.128.1 57901 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 1 udp 2122129151 192.168.1.222 57902 typ host generation 0',

        sdpMid: 'audio',

        sdpMLineIndex: 0 } } }

received (first socket):  { event: '_ice_candidate',

  data:

   { candidate:

      { candidate: 'candidate:3858276637 1 udp 2122129151 192.168.1.222 57902 typ host generation 0',

        sdpMid: 'video',

        sdpMLineIndex: 1 } } }

