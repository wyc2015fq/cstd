# java中byte数组与int类型的转换的几种方式 - 零度的博客专栏 - CSDN博客
2016年04月11日 15:48:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：387
个人分类：[JavaUtil工具类](https://blog.csdn.net/zmx729618/article/category/6177525)
                
java中byte数组与int类型的转换，在网络编程中这个算法是最基本的算法，我们都知道，在socket传输中，发送、者接收的数据都是 byte数组，但是int类型是4个byte组成的，如何把一个整形int转换成byte数组，同时如何把一个长度为4的byte数组转换为int类型。下面有两种方式。
**[java]**[view
 plain](http://blog.csdn.net/cpcpc/article/details/6669582#)[copy](http://blog.csdn.net/cpcpc/article/details/6669582#)
- publicstaticbyte[] int2byte(int res) {  
- byte[] targets = newbyte[4];  
- 
- targets[0] = (byte) (res & 0xff);// 最低位 
- targets[1] = (byte) ((res >> 8) & 0xff);// 次低位 
- targets[2] = (byte) ((res >> 16) & 0xff);// 次高位 
- targets[3] = (byte) (res >>> 24);// 最高位,无符号右移。 
- return targets;   
- }   
**[java]**[view
 plain](http://blog.csdn.net/cpcpc/article/details/6669582#)[copy](http://blog.csdn.net/cpcpc/article/details/6669582#)
- publicstaticint byte2int(byte[] res) {   
- // 一个byte数据左移24位变成0x??000000，再右移8位变成0x00??0000 
- 
- int targets = (res[0] & 0xff) | ((res[1] << 8) & 0xff00) // | 表示安位或 
- | ((res[2] << 24) >>> 8) | (res[3] << 24);   
- return targets;   
- }   
第二种
**[java]**[view
 plain](http://blog.csdn.net/cpcpc/article/details/6669582#)[copy](http://blog.csdn.net/cpcpc/article/details/6669582#)
- publicstaticvoid main(String[] args) {    
-         ByteArrayOutputStream baos = new ByteArrayOutputStream();    
-         DataOutputStream dos = new DataOutputStream(baos);    
- try {    
-             dos.writeByte(4);    
-             dos.writeByte(1);    
-             dos.writeByte(1);    
-             dos.writeShort(217);    
-           } catch (IOException e) {    
-         e.printStackTrace();    
-     }    
- 
- byte[] aa = baos.toByteArray();    
-     ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());    
-     DataInputStream dis = new DataInputStream(bais);    
- 
- try {    
-         System.out.println(dis.readByte());    
-         System.out.println(dis.readByte());    
-         System.out.println(dis.readByte());    
-         System.out.println(dis.readShort());    
-     } catch (IOException e) {    
-         e.printStackTrace();    
-     }    
- try {    
-         dos.close();    
-         dis.close();    
-     } catch (IOException e) {    
-         e.printStackTrace();    
-     }    
- }    
- 
```java
<span style="background-color: rgb(225, 225, 225);"><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">public</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">class</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="typ" style="font-family:Arial,宋体;font-size:14px;color:#2b91af;">ByteUtil</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">{</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="com" style="font-family:Arial,宋体;font-size:14px;color:gray;">/**
     * 整型转换为4位字节数组
     * @param intValue
     * @return
     */</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">public</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">static</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[]</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> int2Byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">int</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> intValue</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">{</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[]</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> b </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">new</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[</span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">4</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">];</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">for</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">int</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">0</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;"><</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">4</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">++)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">{</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
            b</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">]</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">intValue </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">>></span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">8</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">*</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">3</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">-</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">&</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">0xFF</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">);</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
            </span><span class="com" style="font-family:Arial,宋体;font-size:14px;color:gray;">//System.out.print(Integer.toBinaryString(b[i])+" ");</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
            </span><span class="com" style="font-family:Arial,宋体;font-size:14px;color:gray;">//System.out.print((b[i] & 0xFF) + " ");</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">}</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">return</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> b</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">}</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="com" style="font-family:Arial,宋体;font-size:14px;color:gray;">/**
     * 4位字节数组转换为整型
     * @param b
     * @return
     */</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">public</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">static</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">int</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> byte2Int</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">byte</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[]</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> b</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">{</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">int</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> intValue </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">0</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">for</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">int</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">0</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;"><</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> b</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">.</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">length</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">++)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">{</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
            intValue </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">+=</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">b</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">[</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">]</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">&</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">0xFF</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">)</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;"><<</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">8</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">*</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">(</span><span class="lit" style="font-family:Arial,宋体;font-size:14px;color:#8000;">3</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">-</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> i</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">));</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">}</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
        </span><span class="kwd" style="font-family:Arial,宋体;font-size:14px;color:#008b;">return</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;"> intValue</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">;</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
    </span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">}</span><span class="pln" style="font-family:Arial,宋体;font-size:14px;">
</span><span class="pun" style="font-family:Arial,宋体;font-size:14px;">}</span></span>
```
