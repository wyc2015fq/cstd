# RTP 打包H264与AAC - DoubleLi - 博客园








- static int h264_parse(Track *tr, uint8_t *data, size_t len)  
- {  
-     h264_priv *priv = tr->private_data;  
- //    double nal_time; // see page 9 and 7.4.1.2  
-     size_t nalsize = 0, index = 0;  
-     uint8_t *p, *q;  
-     if (priv->is_avc) {  
-         while (1) {  
-             unsigned int i;  
-             if(index >= len) break;  
-             //get the nal size  
-             nalsize = 0;  
-             for(i = 0; i < priv->nal_length_size; i++)  
-                 nalsize = (nalsize << 8) | data[index++];  
-             if(nalsize <= 1 || nalsize > len) {  
-                 if(nalsize == 1) {  
-                     index++;  
-                     continue;  
-                 } else {  
-                     fnc_log(FNC_LOG_VERBOSE, "[h264] AVC: nal size %d", nalsize);  
-                     break;  
-                 }  
-             }  
-             if (DEFAULT_MTU >= nalsize) {  
-                 mparser_buffer_write(tr,  
-                                      tr->properties.pts,  
-                                      tr->properties.dts,  
-                                      tr->properties.frame_duration,  
-                                      1,  
-                                      data + index, nalsize);  
-                 fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL");  
-             } else {  
-             // single NAL, to be fragmented, FU-A;  
-                 frag_fu_a(data + index, nalsize, DEFAULT_MTU, tr);  
-             }  
-             index += nalsize;  
-         }  
-     } else {  
-         //seek to the first startcode  
-         for (p = data; p
-             if (p[0] == 0 && p[1] == 0 && p[2] == 1) {  
-                 break;  
-             }  
-         }  
-         if (p >= data + len) return ERR_PARSE;  
-         while (1) {  
-         //seek to the next startcode [0 0 1]  
-             for (q = p; q
-                 if (q[0] == 0 && q[1] == 0 && q[2] == 1) {  
-                     break;  
-                 }  
-             }  
-             if (q >= data + len) break;  
-             if (DEFAULT_MTU >= q - p) {  
-                 fnc_log(FNC_LOG_VERBOSE, "[h264] Sending NAL %d",p[0]&0x1f);  
-                 mparser_buffer_write(tr,  
-                                      tr->properties.pts,  
-                                      tr->properties.dts,  
-                                      tr->properties.frame_duration,  
-                                      1,  
-                                      p, q - p);  
-                 fnc_log(FNC_LOG_VERBOSE, "[h264] single NAL");  
-             } else {  
-                 //FU-A  
-                 fnc_log(FNC_LOG_VERBOSE, "[h264] frags");  
-                 frag_fu_a(p, q - p, DEFAULT_MTU, tr);  
-             }  
-             p = q;  
-         }  
-         // last NAL  
-         fnc_log(FNC_LOG_VERBOSE, "[h264] last NAL %d",p[0]&0x1f);  
-         if (DEFAULT_MTU >= len - (p - data)) {  
-             fnc_log(FNC_LOG_VERBOSE, "[h264] no frags");  
-             mparser_buffer_write(tr,  
-                                  tr->properties.pts,  
-                                  tr->properties.dts,  
-                                  tr->properties.frame_duration,  
-                                  1,  
-                                  p, len - (p - data));  
-         } else {  
-             //FU-A  
-             fnc_log(FNC_LOG_VERBOSE, "[h264] frags");  
-             frag_fu_a(p, len - (p - data), DEFAULT_MTU, tr);  
-         }  
-     }  
-     fnc_log(FNC_LOG_VERBOSE, "[h264] Frame completed");  
-     return ERR_NOERROR;  
- }  




2. AAC的RTP封包





**[c-sharp]**[view plain](http://blog.csdn.net/moruite/article/details/6102758#)[copy](http://blog.csdn.net/moruite/article/details/6102758#)



- static int aac_parse(Track *tr, uint8_t *data, size_t len)  
- {  
-     //XXX handle the last packet on EOF  
-     int off = 0;  
-     uint32_t payload = DEFAULT_MTU - AU_HEADER_SIZE;  
-     uint8_t *packet = g_malloc0(DEFAULT_MTU);  
-     if(!packet) return ERR_ALLOC;  
- // trim away extradata  
- //    data += AAC_EXTRA;  
- //    len -= AAC_EXTRA;  
-     packet[0] = 0x00;  
-     packet[1] = 0x10;  
-     packet[2] = (len & 0x1fe0) >> 5;  
-     packet[3] = (len & 0x1f) << 3;  
-     if (len > payload) {  
-         while (len > payload) {  
-             memcpy(packet + AU_HEADER_SIZE, data + off, payload);  
-             mparser_buffer_write(tr,  
-                                  tr->properties.pts,  
-                                  tr->properties.dts,  
-                                  tr->properties.frame_duration,  
-                                  0,  
-                                  packet, DEFAULT_MTU);  
-             len -= payload;  
-             off += payload;  
-         }  
-     }  
-     memcpy(packet + AU_HEADER_SIZE, data + off, len);  
-     mparser_buffer_write(tr,  
-                          tr->properties.pts,  
-                          tr->properties.dts,  
-                          tr->properties.frame_duration,  
-                          1,  
-                          packet, len + AU_HEADER_SIZE);  
-     g_free(packet);  
-     return ERR_NOERROR;  
- }  




上面的变量 AU_HEADER_SIZE=4







来自： [http://blog.csdn.net/moruite/article/details/6102758](http://blog.csdn.net/moruite/article/details/6102758)









