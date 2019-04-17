# Java解析JT808协议示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月21日 13:18:26[boonya](https://me.csdn.net/boonya)阅读数：428








原文地址：[https://blog.csdn.net/hylexus/article/details/54987786](https://blog.csdn.net/hylexus/article/details/54987786)
- [JT808协议扫盲](https://blog.csdn.net/hylexus/article/details/54987786#1-jt808%E5%8D%8F%E8%AE%AE%E6%89%AB%E7%9B%B2)- [1 数据类型](https://blog.csdn.net/hylexus/article/details/54987786#11-%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B)
- [2 消息结构](https://blog.csdn.net/hylexus/article/details/54987786#12-%E6%B6%88%E6%81%AF%E7%BB%93%E6%9E%84)
- [3 消息头](https://blog.csdn.net/hylexus/article/details/54987786#13-%E6%B6%88%E6%81%AF%E5%A4%B4)

- [解析](https://blog.csdn.net/hylexus/article/details/54987786#2-%E8%A7%A3%E6%9E%90)- [1 消息体实体类](https://blog.csdn.net/hylexus/article/details/54987786#21-%E6%B6%88%E6%81%AF%E4%BD%93%E5%AE%9E%E4%BD%93%E7%B1%BB)
- [2 字节数组到消息体实体类的转换](https://blog.csdn.net/hylexus/article/details/54987786#22-%E5%AD%97%E8%8A%82%E6%95%B0%E7%BB%84%E5%88%B0%E6%B6%88%E6%81%AF%E4%BD%93%E5%AE%9E%E4%BD%93%E7%B1%BB%E7%9A%84%E8%BD%AC%E6%8D%A2)- [21 消息转换器](https://blog.csdn.net/hylexus/article/details/54987786#221-%E6%B6%88%E6%81%AF%E8%BD%AC%E6%8D%A2%E5%99%A8)
- [22 用到的工具类](https://blog.csdn.net/hylexus/article/details/54987786#222-%E7%94%A8%E5%88%B0%E7%9A%84%E5%B7%A5%E5%85%B7%E7%B1%BB)- [221 BCD操作工具类](https://blog.csdn.net/hylexus/article/details/54987786#2221-bcd%E6%93%8D%E4%BD%9C%E5%B7%A5%E5%85%B7%E7%B1%BB)
- [222 位操作工具类](https://blog.csdn.net/hylexus/article/details/54987786#2222-%E4%BD%8D%E6%93%8D%E4%BD%9C%E5%B7%A5%E5%85%B7%E7%B1%BB)


- [3 和netty结合](https://blog.csdn.net/hylexus/article/details/54987786#23-%E5%92%8Cnetty%E7%BB%93%E5%90%88)- [31 netty处理器链](https://blog.csdn.net/hylexus/article/details/54987786#231-netty%E5%A4%84%E7%90%86%E5%99%A8%E9%93%BE)
- [32 netty针对于JT808的消息处理器](https://blog.csdn.net/hylexus/article/details/54987786#232-netty%E9%92%88%E5%AF%B9%E4%BA%8Ejt808%E7%9A%84%E6%B6%88%E6%81%AF%E5%A4%84%E7%90%86%E5%99%A8)
- [33 用到的其他类](https://blog.csdn.net/hylexus/article/details/54987786#233-%E7%94%A8%E5%88%B0%E7%9A%84%E5%85%B6%E4%BB%96%E7%B1%BB)


- [demo级别java示例](https://blog.csdn.net/hylexus/article/details/54987786#3-demo%E7%BA%A7%E5%88%ABjava%E7%A4%BA%E4%BE%8B)

本篇文章将介绍JT808协议的解析思路。

# 1 JT808协议扫盲

## 1.1 数据类型
|数据类型|描述及要求|
|----|----|
|BYTE|无符号单字节整形（字节， 8 位）|
|WORD|无符号双字节整形（字， 16 位）|
|DWORD|无符号四字节整形（双字， 32 位）|
|BYTE[n]|n 字节|
|BCD[n]|8421 码， n 字节|
|STRING|GBK 编码，若无数据，置空|

## 1.2 消息结构
|标识位|消息头|消息体|校验码|标识位|
|----|----|----|----|----|
|1byte(0x7e)|16byte||1byte|1byte(0x7e)|

## 1.3 消息头

```
消息ID(0-1)   消息体属性(2-3)  终端手机号(4-9)  消息流水号(10-11)    消息包封装项(12-15)

byte[0-1]   消息ID word(16)
byte[2-3]   消息体属性 word(16)
        bit[0-9]    消息体长度
        bit[10-12]  数据加密方式
                        此三位都为 0，表示消息体不加密
                        第 10 位为 1，表示消息体经过 RSA 算法加密
                        其它保留
        bit[13]     分包
                        1：消息体卫长消息，进行分包发送处理，具体分包信息由消息包封装项决定
                        0：则消息头中无消息包封装项字段
        bit[14-15]  保留
byte[4-9]   终端手机号或设备ID bcd[6]
        根据安装后终端自身的手机号转换
        手机号不足12 位，则在前面补 0
byte[10-11]     消息流水号 word(16)
        按发送顺序从 0 开始循环累加
byte[12-15]     消息包封装项
        byte[0-1]   消息包总数(word(16))
                        该消息分包后得总包数
        byte[2-3]   包序号(word(16))
                        从 1 开始
        如果消息体属性中相关标识位确定消息分包处理,则该项有内容
        否则无该项
```

# 2 解析

整个消息体结构中最复杂的就是消息头了。

## 2.1 消息体实体类

以下是对整个消息体抽象出来的一个java实体类。

```
import java.nio.channels.Channel;

public class PackageData {

    /**
     * 16byte 消息头
     */
    protected MsgHeader msgHeader;

    // 消息体字节数组
    protected byte[] msgBodyBytes;

    /**
     * 校验码 1byte
     */
    protected int checkSum;

    //记录每个客户端的channel,以便下发信息给客户端
    protected Channel channel;

    public MsgHeader getMsgHeader() {
        return msgHeader;
    }

    //TODO set 和 get 方法在此处省略

    //消息头
    public static class MsgHeader {
        // 消息ID
        protected int msgId;

        /////// ========消息体属性
        // byte[2-3]
        protected int msgBodyPropsField;
        // 消息体长度
        protected int msgBodyLength;
        // 数据加密方式
        protected int encryptionType;
        // 是否分包,true==>有消息包封装项
        protected boolean hasSubPackage;
        // 保留位[14-15]
        protected String reservedBit;
        /////// ========消息体属性

        // 终端手机号
        protected String terminalPhone;
        // 流水号
        protected int flowId;

        //////// =====消息包封装项
        // byte[12-15]
        protected int packageInfoField;
        // 消息包总数(word(16))
        protected long totalSubPackage;
        // 包序号(word(16))这次发送的这个消息包是分包中的第几个消息包, 从 1 开始
        protected long subPackageSeq;
        //////// =====消息包封装项

        //TODO set 和 get 方法在此处省略
    }

}
```

## 2.2 字节数组到消息体实体类的转换

### 2.2.1 消息转换器

```
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import cn.hylexus.jt808.util.BCD8421Operater;
import cn.hylexus.jt808.util.BitOperator;
import cn.hylexus.jt808.vo.PackageData;
import cn.hylexus.jt808.vo.PackageData.MsgHeader;

public class MsgDecoder {

    private static final Logger log = LoggerFactory.getLogger(MsgDecoder.class);

    private BitOperator bitOperator;
    private BCD8421Operater bcd8421Operater;

    public MsgDecoder() {
        this.bitOperator = new BitOperator();
        this.bcd8421Operater = new BCD8421Operater();
    }

    //字节数组到消息体实体类
    public PackageData queueElement2PackageData(byte[] data) {
        PackageData ret = new PackageData();

        // 1. 16byte 或 12byte 消息头
        MsgHeader msgHeader = this.parseMsgHeaderFromBytes(data);
        ret.setMsgHeader(msgHeader);

        int msgBodyByteStartIndex = 12;
        // 2. 消息体
        // 有子包信息,消息体起始字节后移四个字节:消息包总数(word(16))+包序号(word(16))
        if (msgHeader.isHasSubPackage()) {
            msgBodyByteStartIndex = 16;
        }

        byte[] tmp = new byte[msgHeader.getMsgBodyLength()];
        System.arraycopy(data, msgBodyByteStartIndex, tmp, 0, tmp.length);
        ret.setMsgBodyBytes(tmp);

        // 3. 去掉分隔符之后，最后一位就是校验码
        // int checkSumInPkg =
        // this.bitOperator.oneByteToInteger(data[data.length - 1]);
        int checkSumInPkg = data[data.length - 1];
        int calculatedCheckSum = this.bitOperator.getCheckSum4JT808(data, 0, data.length - 1);
        ret.setCheckSum(checkSumInPkg);
        if (checkSumInPkg != calculatedCheckSum) {
            log.warn("检验码不一致,msgid:{},pkg:{},calculated:{}", msgHeader.getMsgId(), checkSumInPkg, calculatedCheckSum);
        }
        return ret;
    }

    private MsgHeader parseMsgHeaderFromBytes(byte[] data) {
        MsgHeader msgHeader = new MsgHeader();

        // 1. 消息ID word(16)
        // byte[] tmp = new byte[2];
        // System.arraycopy(data, 0, tmp, 0, 2);
        // msgHeader.setMsgId(this.bitOperator.twoBytesToInteger(tmp));
        msgHeader.setMsgId(this.parseIntFromBytes(data, 0, 2));

        // 2. 消息体属性 word(16)=================>
        // System.arraycopy(data, 2, tmp, 0, 2);
        // int msgBodyProps = this.bitOperator.twoBytesToInteger(tmp);
        int msgBodyProps = this.parseIntFromBytes(data, 2, 2);
        msgHeader.setMsgBodyPropsField(msgBodyProps);
        // [ 0-9 ] 0000,0011,1111,1111(3FF)(消息体长度)
        msgHeader.setMsgBodyLength(msgBodyProps & 0x1ff);
        // [10-12] 0001,1100,0000,0000(1C00)(加密类型)
        msgHeader.setEncryptionType((msgBodyProps & 0xe00) >> 10);
        // [ 13_ ] 0010,0000,0000,0000(2000)(是否有子包)
        msgHeader.setHasSubPackage(((msgBodyProps & 0x2000) >> 13) == 1);
        // [14-15] 1100,0000,0000,0000(C000)(保留位)
        msgHeader.setReservedBit(((msgBodyProps & 0xc000) >> 14) + "");
        // 消息体属性 word(16)<=================

        // 3. 终端手机号 bcd[6]
        // tmp = new byte[6];
        // System.arraycopy(data, 4, tmp, 0, 6);
        // msgHeader.setTerminalPhone(this.bcd8421Operater.bcd2String(tmp));
        msgHeader.setTerminalPhone(this.parseBcdStringFromBytes(data, 4, 6));

        // 4. 消息流水号 word(16) 按发送顺序从 0 开始循环累加
        // tmp = new byte[2];
        // System.arraycopy(data, 10, tmp, 0, 2);
        // msgHeader.setFlowId(this.bitOperator.twoBytesToInteger(tmp));
        msgHeader.setFlowId(this.parseIntFromBytes(data, 10, 2));

        // 5. 消息包封装项
        // 有子包信息
        if (msgHeader.isHasSubPackage()) {
            // 消息包封装项字段
            msgHeader.setPackageInfoField(this.parseIntFromBytes(data, 12, 4));
            // byte[0-1] 消息包总数(word(16))
            // tmp = new byte[2];
            // System.arraycopy(data, 12, tmp, 0, 2);
            // msgHeader.setTotalSubPackage(this.bitOperator.twoBytesToInteger(tmp));
            msgHeader.setTotalSubPackage(this.parseIntFromBytes(data, 12, 2));

            // byte[2-3] 包序号(word(16)) 从 1 开始
            // tmp = new byte[2];
            // System.arraycopy(data, 14, tmp, 0, 2);
            // msgHeader.setSubPackageSeq(this.bitOperator.twoBytesToInteger(tmp));
            msgHeader.setSubPackageSeq(this.parseIntFromBytes(data, 12, 2));
        }
        return msgHeader;
    }

    protected String parseStringFromBytes(byte[] data, int startIndex, int lenth) {
        return this.parseStringFromBytes(data, startIndex, lenth, null);
    }

    private String parseStringFromBytes(byte[] data, int startIndex, int lenth, String defaultVal) {
        try {
            byte[] tmp = new byte[lenth];
            System.arraycopy(data, startIndex, tmp, 0, lenth);
            return new String(tmp, "UTF-8");
        } catch (Exception e) {
            log.error("解析字符串出错:{}", e.getMessage());
            e.printStackTrace();
            return defaultVal;
        }
    }

    private String parseBcdStringFromBytes(byte[] data, int startIndex, int lenth) {
        return this.parseBcdStringFromBytes(data, startIndex, lenth, null);
    }

    private String parseBcdStringFromBytes(byte[] data, int startIndex, int lenth, String defaultVal) {
        try {
            byte[] tmp = new byte[lenth];
            System.arraycopy(data, startIndex, tmp, 0, lenth);
            return this.bcd8421Operater.bcd2String(tmp);
        } catch (Exception e) {
            log.error("解析BCD(8421码)出错:{}", e.getMessage());
            e.printStackTrace();
            return defaultVal;
        }
    }

    private int parseIntFromBytes(byte[] data, int startIndex, int length) {
        return this.parseIntFromBytes(data, startIndex, length, 0);
    }

    private int parseIntFromBytes(byte[] data, int startIndex, int length, int defaultVal) {
        try {
            // 字节数大于4,从起始索引开始向后处理4个字节,其余超出部分丢弃
            final int len = length > 4 ? 4 : length;
            byte[] tmp = new byte[len];
            System.arraycopy(data, startIndex, tmp, 0, len);
            return bitOperator.byteToInteger(tmp);
        } catch (Exception e) {
            log.error("解析整数出错:{}", e.getMessage());
            e.printStackTrace();
            return defaultVal;
        }
    }
}
```
- 

### 2.2.2 用到的工具类

2.2.2.1 BCD操作工具类

```
package cn.hylexus.jt808.util;

public class BCD8421Operater {

    /**
     * BCD字节数组===>String
     * 
     * @param bytes
     * @return 十进制字符串
     */
    public String bcd2String(byte[] bytes) {
        StringBuilder temp = new StringBuilder(bytes.length * 2);
        for (int i = 0; i < bytes.length; i++) {
            // 高四位
            temp.append((bytes[i] & 0xf0) >>> 4);
            // 低四位
            temp.append(bytes[i] & 0x0f);
        }
        return temp.toString().substring(0, 1).equalsIgnoreCase("0") ? temp.toString().substring(1) : temp.toString();
    }

    /**
     * 字符串==>BCD字节数组
     * 
     * @param str
     * @return BCD字节数组
     */
    public byte[] string2Bcd(String str) {
        // 奇数,前补零
        if ((str.length() & 0x1) == 1) {
            str = "0" + str;
        }

        byte ret[] = new byte[str.length() / 2];
        byte bs[] = str.getBytes();
        for (int i = 0; i < ret.length; i++) {

            byte high = ascII2Bcd(bs[2 * i]);
            byte low = ascII2Bcd(bs[2 * i + 1]);

            // TODO 只遮罩BCD低四位?
            ret[i] = (byte) ((high << 4) | low);
        }
        return ret;
    }

    private byte ascII2Bcd(byte asc) {
        if ((asc >= '0') && (asc <= '9'))
            return (byte) (asc - '0');
        else if ((asc >= 'A') && (asc <= 'F'))
            return (byte) (asc - 'A' + 10);
        else if ((asc >= 'a') && (asc <= 'f'))
            return (byte) (asc - 'a' + 10);
        else
            return (byte) (asc - 48);
    }
}
```

2.2.2.2 位操作工具类

```
package cn.hylexus.jt808.util;

import java.util.Arrays;
import java.util.List;

public class BitOperator {

    /**
     * 把一个整形该为byte
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte integerTo1Byte(int value) {
        return (byte) (value & 0xFF);
    }

    /**
     * 把一个整形该为1位的byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] integerTo1Bytes(int value) {
        byte[] result = new byte[1];
        result[0] = (byte) (value & 0xFF);
        return result;
    }

    /**
     * 把一个整形改为2位的byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] integerTo2Bytes(int value) {
        byte[] result = new byte[2];
        result[0] = (byte) ((value >>> 8) & 0xFF);
        result[1] = (byte) (value & 0xFF);
        return result;
    }

    /**
     * 把一个整形改为3位的byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] integerTo3Bytes(int value) {
        byte[] result = new byte[3];
        result[0] = (byte) ((value >>> 16) & 0xFF);
        result[1] = (byte) ((value >>> 8) & 0xFF);
        result[2] = (byte) (value & 0xFF);
        return result;
    }

    /**
     * 把一个整形改为4位的byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] integerTo4Bytes(int value){
        byte[] result = new byte[4];
        result[0] = (byte) ((value >>> 24) & 0xFF);
        result[1] = (byte) ((value >>> 16) & 0xFF);
        result[2] = (byte) ((value >>> 8) & 0xFF);
        result[3] = (byte) (value & 0xFF);
        return result;
    }

    /**
     * 把byte[]转化位整形,通常为指令用
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public int byteToInteger(byte[] value) {
        int result;
        if (value.length == 1) {
            result = oneByteToInteger(value[0]);
        } else if (value.length == 2) {
            result = twoBytesToInteger(value);
        } else if (value.length == 3) {
            result = threeBytesToInteger(value);
        } else if (value.length == 4) {
            result = fourBytesToInteger(value);
        } else {
            result = fourBytesToInteger(value);
        }
        return result;
    }

    /**
     * 把一个byte转化位整形,通常为指令用
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public int oneByteToInteger(byte value) {
        return (int) value & 0xFF;
    }

    /**
     * 把一个2位的数组转化位整形
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public int twoBytesToInteger(byte[] value) {
        // if (value.length < 2) {
        // throw new Exception("Byte array too short!");
        // }
        int temp0 = value[0] & 0xFF;
        int temp1 = value[1] & 0xFF;
        return ((temp0 << 8) + temp1);
    }

    /**
     * 把一个3位的数组转化位整形
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public int threeBytesToInteger(byte[] value) {
        int temp0 = value[0] & 0xFF;
        int temp1 = value[1] & 0xFF;
        int temp2 = value[2] & 0xFF;
        return ((temp0 << 16) + (temp1 << 8) + temp2);
    }

    /**
     * 把一个4位的数组转化位整形,通常为指令用
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public int fourBytesToInteger(byte[] value) {
        // if (value.length < 4) {
        // throw new Exception("Byte array too short!");
        // }
        int temp0 = value[0] & 0xFF;
        int temp1 = value[1] & 0xFF;
        int temp2 = value[2] & 0xFF;
        int temp3 = value[3] & 0xFF;
        return ((temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3);
    }

    /**
     * 把一个4位的数组转化位整形
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public long fourBytesToLong(byte[] value) throws Exception {
        // if (value.length < 4) {
        // throw new Exception("Byte array too short!");
        // }
        int temp0 = value[0] & 0xFF;
        int temp1 = value[1] & 0xFF;
        int temp2 = value[2] & 0xFF;
        int temp3 = value[3] & 0xFF;
        return (((long) temp0 << 24) + (temp1 << 16) + (temp2 << 8) + temp3);
    }

    /**
     * 把一个数组转化长整形
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public long bytes2Long(byte[] value) {
        long result = 0;
        int len = value.length;
        int temp;
        for (int i = 0; i < len; i++) {
            temp = (len - 1 - i) * 8;
            if (temp == 0) {
                result += (value[i] & 0x0ff);
            } else {
                result += (value[i] & 0x0ff) << temp;
            }
        }
        return result;
    }

    /**
     * 把一个长整形改为byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] longToBytes(long value){
        return longToBytes(value, 8);
    }

    /**
     * 把一个长整形改为byte数组
     * 
     * @param value
     * @return
     * @throws Exception
     */
    public byte[] longToBytes(long value, int len) {
        byte[] result = new byte[len];
        int temp;
        for (int i = 0; i < len; i++) {
            temp = (len - 1 - i) * 8;
            if (temp == 0) {
                result[i] += (value & 0x0ff);
            } else {
                result[i] += (value >>> temp) & 0x0ff;
            }
        }
        return result;
    }

    /**
     * 得到一个消息ID
     * 
     * @return
     * @throws Exception
     */
    public byte[] generateTransactionID() throws Exception {
        byte[] id = new byte[16];
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 0, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 2, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 4, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 6, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 8, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 10, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 12, 2);
        System.arraycopy(integerTo2Bytes((int) (Math.random() * 65536)), 0, id, 14, 2);
        return id;
    }

    /**
     * 把IP拆分位int数组
     * 
     * @param ip
     * @return
     * @throws Exception
     */
    public int[] getIntIPValue(String ip) throws Exception {
        String[] sip = ip.split("[.]");
        // if (sip.length != 4) {
        // throw new Exception("error IPAddress");
        // }
        int[] intIP = { Integer.parseInt(sip[0]), Integer.parseInt(sip[1]), Integer.parseInt(sip[2]),
                Integer.parseInt(sip[3]) };
        return intIP;
    }

    /**
     * 把byte类型IP地址转化位字符串
     * 
     * @param address
     * @return
     * @throws Exception
     */
    public String getStringIPValue(byte[] address) throws Exception {
        int first = this.oneByteToInteger(address[0]);
        int second = this.oneByteToInteger(address[1]);
        int third = this.oneByteToInteger(address[2]);
        int fourth = this.oneByteToInteger(address[3]);

        return first + "." + second + "." + third + "." + fourth;
    }

    /**
     * 合并字节数组
     * 
     * @param first
     * @param rest
     * @return
     */
    public byte[] concatAll(byte[] first, byte[]... rest) {
        int totalLength = first.length;
        for (byte[] array : rest) {
            if (array != null) {
                totalLength += array.length;
            }
        }
        byte[] result = Arrays.copyOf(first, totalLength);
        int offset = first.length;
        for (byte[] array : rest) {
            if (array != null) {
                System.arraycopy(array, 0, result, offset, array.length);
                offset += array.length;
            }
        }
        return result;
    }

    /**
     * 合并字节数组
     * 
     * @param rest
     * @return
     */
    public byte[] concatAll(List<byte[]> rest) {
        int totalLength = 0;
        for (byte[] array : rest) {
            if (array != null) {
                totalLength += array.length;
            }
        }
        byte[] result = new byte[totalLength];
        int offset = 0;
        for (byte[] array : rest) {
            if (array != null) {
                System.arraycopy(array, 0, result, offset, array.length);
                offset += array.length;
            }
        }
        return result;
    }

    public float byte2Float(byte[] bs) {
        return Float.intBitsToFloat(
                (((bs[3] & 0xFF) << 24) + ((bs[2] & 0xFF) << 16) + ((bs[1] & 0xFF) << 8) + (bs[0] & 0xFF)));
    }

    public float byteBE2Float(byte[] bytes) {
        int l;
        l = bytes[0];
        l &= 0xff;
        l |= ((long) bytes[1] << 8);
        l &= 0xffff;
        l |= ((long) bytes[2] << 16);
        l &= 0xffffff;
        l |= ((long) bytes[3] << 24);
        return Float.intBitsToFloat(l);
    }

    public int getCheckSum4JT808(byte[] bs, int start, int end) {
        if (start < 0 || end > bs.length)
            throw new ArrayIndexOutOfBoundsException("getCheckSum4JT808 error : index out of bounds(start=" + start
                    + ",end=" + end + ",bytes length=" + bs.length + ")");
        int cs = 0;
        for (int i = start; i < end; i++) {
            cs ^= bs[i];
        }
        return cs;
    }

    public int getBitRange(int number, int start, int end) {
        if (start < 0)
            throw new IndexOutOfBoundsException("min index is 0,but start = " + start);
        if (end >= Integer.SIZE)
            throw new IndexOutOfBoundsException("max index is " + (Integer.SIZE - 1) + ",but end = " + end);

        return (number << Integer.SIZE - (end + 1)) >>> Integer.SIZE - (end - start + 1);
    }

    public int getBitAt(int number, int index) {
        if (index < 0)
            throw new IndexOutOfBoundsException("min index is 0,but " + index);
        if (index >= Integer.SIZE)
            throw new IndexOutOfBoundsException("max index is " + (Integer.SIZE - 1) + ",but " + index);

        return ((1 << index) & number) >> index;
    }

    public int getBitAtS(int number, int index) {
        String s = Integer.toBinaryString(number);
        return Integer.parseInt(s.charAt(index) + "");
    }

    @Deprecated
    public int getBitRangeS(int number, int start, int end) {
        String s = Integer.toBinaryString(number);
        StringBuilder sb = new StringBuilder(s);
        while (sb.length() < Integer.SIZE) {
            sb.insert(0, "0");
        }
        String tmp = sb.reverse().substring(start, end + 1);
        sb = new StringBuilder(tmp);
        return Integer.parseInt(sb.reverse().toString(), 2);
    }
}
```

## 2.3 和netty结合

### 2.3.1 netty处理器链

```
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import cn.kkbc.tpms.tcp.service.TCPServerHandler;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.timeout.IdleStateHandler;
import io.netty.util.concurrent.Future;

public class TCPServer2 {

    private Logger log = LoggerFactory.getLogger(getClass());
    private volatile boolean isRunning = false;

    private EventLoopGroup bossGroup = null;
    private EventLoopGroup workerGroup = null;
    private int port;

    public TCPServer2() {
    }

    public TCPServer2(int port) {
        this();
        this.port = port;
    }

    private void bind() throws Exception {
        this.bossGroup = new NioEventLoopGroup();
        this.workerGroup = new NioEventLoopGroup();
        ServerBootstrap serverBootstrap = new ServerBootstrap();
        serverBootstrap.group(bossGroup, workerGroup)//
                .channel(NioServerSocketChannel.class) //
                .childHandler(new ChannelInitializer<SocketChannel>() { //
                    @Override
                    public void initChannel(SocketChannel ch) throws Exception {
                        //超过15分钟未收到客户端消息则自动断开客户端连接
                        ch.pipeline().addLast("idleStateHandler",
                                new IdleStateHandler(15, 0, 0, TimeUnit.MINUTES));
                        //ch.pipeline().addLast(new Decoder4LoggingOnly());
                        // 1024表示单条消息的最大长度，解码器在查找分隔符的时候，达到该长度还没找到的话会抛异常
                        ch.pipeline().addLast(
                                new DelimiterBasedFrameDecoder(1024, Unpooled.copiedBuffer(new byte[] { 0x7e }),
                                        Unpooled.copiedBuffer(new byte[] { 0x7e, 0x7e })));
                        ch.pipeline().addLast(new TCPServerHandler());
                    }
                }).option(ChannelOption.SO_BACKLOG, 128) //
                .childOption(ChannelOption.SO_KEEPALIVE, true);

        this.log.info("TCP服务启动完毕,port={}", this.port);
        ChannelFuture channelFuture = serverBootstrap.bind(port).sync();

        channelFuture.channel().closeFuture().sync();
    }

    public synchronized void startServer() {
        if (this.isRunning) {
            throw new IllegalStateException(this.getName() + " is already started .");
        }
        this.isRunning = true;

        new Thread(() -> {
            try {
                this.bind();
            } catch (Exception e) {
                this.log.info("TCP服务启动出错:{}", e.getMessage());
                e.printStackTrace();
            }
        }, this.getName()).start();
    }

    public synchronized void stopServer() {
        if (!this.isRunning) {
            throw new IllegalStateException(this.getName() + " is not yet started .");
        }
        this.isRunning = false;

        try {
            Future<?> future = this.workerGroup.shutdownGracefully().await();
            if (!future.isSuccess()) {
                log.error("workerGroup 无法正常停止:{}", future.cause());
            }

            future = this.bossGroup.shutdownGracefully().await();
            if (!future.isSuccess()) {
                log.error("bossGroup 无法正常停止:{}", future.cause());
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        this.log.info("TCP服务已经停止...");
    }

    private String getName() {
        return "TCP-Server";
    }

    public static void main(String[] args) throws Exception {
        TCPServer2 server = new TCPServer2(20048);
        server.startServer();

        // Thread.sleep(3000);
        // server.stopServer();
    }

}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116

### 2.3.2 netty针对于JT808的消息处理器

```
package cn.hylexus.jt808.service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import cn.hylexus.jt808.server.SessionManager;
import cn.hylexus.jt808.service.codec.MsgDecoder;
import cn.hylexus.jt808.vo.PackageData;
import cn.hylexus.jt808.vo.Session;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.handler.timeout.IdleState;
import io.netty.handler.timeout.IdleStateEvent;
import io.netty.util.ReferenceCountUtil;

public class TCPServerHandler extends ChannelInboundHandlerAdapter { // (1)

    private final Logger logger = LoggerFactory.getLogger(getClass());

    // 一个维护客户端连接的类
    private final SessionManager sessionManager;
    private MsgDecoder decoder = new MsgDecoder();

    public TCPServerHandler() {
        this.sessionManager = SessionManager.getInstance();
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws InterruptedException { // (2)
        try {
            ByteBuf buf = (ByteBuf) msg;
            if (buf.readableBytes() <= 0) {
                // ReferenceCountUtil.safeRelease(msg);
                return;
            }

            byte[] bs = new byte[buf.readableBytes()];
            buf.readBytes(bs);

            PackageData jt808Msg = this.decoder.queueElement2PackageData(bs);
            // 处理客户端消息
            this.processClientMsg(jt808Msg);
        } finally {
            release(msg);
        }
    }

    private void processClientMsg(PackageData jt808Msg) {
        // TODO 更加消息ID的不同,分别实现自己的业务逻辑
        if (jt808Msg.getMsgHeader().getMsgId() == 0x900) {
            // TODO ...
        } else if (jt808Msg.getMsgHeader().getMsgId() == 0x9001) {
            // TODO ...
        }
        // else if(){}
        // else if(){}
        // else if(){}
        // else if(){}
        // ...
        else {
            logger.error("位置消息,消息ID={}", jt808Msg.getMsgHeader().getMsgId());
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) { // (4)
        logger.error("发生异常:{}", cause.getMessage());
        cause.printStackTrace();
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        Session session = Session.buildSession(ctx.channel());
        sessionManager.put(session.getId(), session);
        logger.debug("终端连接:{}", session);
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        final String sessionId = ctx.channel().id().asLongText();
        Session session = sessionManager.findBySessionId(sessionId);
        this.sessionManager.removeBySessionId(sessionId);
        logger.debug("终端断开连接:{}", session);
        ctx.channel().close();
        // ctx.close();
    }

    @Override
    public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
        if (IdleStateEvent.class.isAssignableFrom(evt.getClass())) {
            IdleStateEvent event = (IdleStateEvent) evt;
            if (event.state() == IdleState.READER_IDLE) {
                Session session = this.sessionManager.removeBySessionId(Session.buildId(ctx.channel()));
                logger.error("服务器主动断开连接:{}", session);
                ctx.close();
            }
        }
    }

    private void release(Object msg) {
        try {
            ReferenceCountUtil.release(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

### 2.3.3 用到的其他类

```
package cn.hylexus.jt808.server;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;

import cn.hylexus.jt808.vo.Session;

public class SessionManager {

    private static volatile SessionManager instance = null;
    // netty生成的sessionID和Session的对应关系
    private Map<String, Session> sessionIdMap;
    // 终端手机号和netty生成的sessionID的对应关系
    private Map<String, String> phoneMap;

    public static SessionManager getInstance() {
        if (instance == null) {
            synchronized (SessionManager.class) {
                if (instance == null) {
                    instance = new SessionManager();
                }
            }
        }
        return instance;
    }

    public SessionManager() {
        this.sessionIdMap = new ConcurrentHashMap<>();
        this.phoneMap = new ConcurrentHashMap<>();
    }

    public boolean containsKey(String sessionId) {
        return sessionIdMap.containsKey(sessionId);
    }

    public boolean containsSession(Session session) {
        return sessionIdMap.containsValue(session);
    }

    public Session findBySessionId(String id) {
        return sessionIdMap.get(id);
    }

    public Session findByTerminalPhone(String phone) {
        String sessionId = this.phoneMap.get(phone);
        if (sessionId == null)
            return null;
        return this.findBySessionId(sessionId);
    }

    public synchronized Session put(String key, Session value) {
        if (value.getTerminalPhone() != null && !"".equals(value.getTerminalPhone().trim())) {
            this.phoneMap.put(value.getTerminalPhone(), value.getId());
        }
        return sessionIdMap.put(key, value);
    }

    public synchronized Session removeBySessionId(String sessionId) {
        if (sessionId == null)
            return null;
        Session session = sessionIdMap.remove(sessionId);
        if (session == null)
            return null;
        if (session.getTerminalPhone() != null)
            this.phoneMap.remove(session.getTerminalPhone());
        return session;
    }

    public Set<String> keySet() {
        return sessionIdMap.keySet();
    }

    public void forEach(BiConsumer<? super String, ? super Session> action) {
        sessionIdMap.forEach(action);
    }

    public Set<Entry<String, Session>> entrySet() {
        return sessionIdMap.entrySet();
    }

    public List<Session> toList() {
        return this.sessionIdMap.entrySet().stream().map(e -> e.getValue()).collect(Collectors.toList());
    }

}
```

# 3 demo级别java示例

请移步: [https://github.com/hylexus/jt-808-protocol](https://github.com/hylexus/jt-808-protocol)



