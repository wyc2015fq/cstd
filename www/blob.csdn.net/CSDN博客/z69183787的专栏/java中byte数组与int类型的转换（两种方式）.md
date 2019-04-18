# java中byte数组与int类型的转换（两种方式） - z69183787的专栏 - CSDN博客
2014年08月14日 21:20:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：24554
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
public class ByteUtil {
    /**
     * 整型转换为4位字节数组
     * @param intValue
     * @return
     */
    public static byte[] int2Byte(int intValue) {
        byte[] b = new byte[4];
        for (int i = 0; i < 4; i++) {
            b[i] = (byte) (intValue >> 8 * (3 - i) & 0xFF);
            //System.out.print(Integer.toBinaryString(b[i])+" ");
            //System.out.print((b[i] & 0xFF) + " ");
        }
        return b;
    }
    /**
     * 4位字节数组转换为整型
     * @param b
     * @return
     */
    public static int byte2Int(byte[] b) {
        int intValue = 0;
        for (int i = 0; i < b.length; i++) {
            intValue += (b[i] & 0xFF) << (8 * (3 - i));
        }
        return intValue;
    }
}
```
### 最近在做些与编解码相关的事情，又遇到了byte和int的转换，看着那些关于反码、补码的说明依旧头疼，还是记下些实用的方法吧。
**int -> byte**
可以直接使用强制类型转换: byte b = (byte) aInt;
这个操作是直接截取int中最低一个字节，如果int大于255，则值就会变得面目全非了。
对于通过InputStream.read()获取的int，可采用这个方法将值还原。
**byte -> int**
这里有两种情况，一种是要求保持值不变，例如进行数值计算，可采用强制类型转换：int i = (int) aByte;
另一种是要求保持最低字节中各个位不变，3个高字节全部用0填充，例如进行编解码操作，
则需要采用位操作：int i = b & 0xff;
**int InputStream.read()**
该函数返回一个int类型，范围从0至255，如果到达流末尾，返回-1。通过ByteArrayInputStream的源码可以看到是如何从byte转到int
public synchronized int read() {
    return (pos < count) ? (buf[pos++] & 0xff) : -1;
}
**int <-> byte[]**
代码转自：[java int 与 byte转换](http://blog.csdn.net/openpk/article/details/5430591)
public static byte[] toByteArray(int iSource, int iArrayLen) {
    byte[] bLocalArr = new byte[iArrayLen];
    for (int i = 0; (i < 4) && (i < iArrayLen); i++) {
        bLocalArr[i] = (byte) (iSource >> 8 * i & 0xFF);
    }
    return bLocalArr;
}
// 将byte数组bRefArr转为一个整数,字节数组的低位是整型的低字节位
public static int toInt(byte[] bRefArr) {
    int iOutcome = 0;
    byte bLoop;
    for (int i = 0; i < bRefArr.length; i++) {
        bLoop = bRefArr[i];
        iOutcome += (bLoop & 0xFF) << (8 * i);
    }
    return iOutcome;
}
