# Java 将字节数组转化为16进制的多种方案 - z69183787的专栏 - CSDN博客
2014年07月02日 11:04:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：744
个人分类：[Java-知识																[面试																[工具-进制转换](https://blog.csdn.net/z69183787/article/category/2693541)](https://blog.csdn.net/z69183787/article/category/2185959)](https://blog.csdn.net/z69183787/article/category/2175279)
很多时候我们需要将字节数组转化为16进制字符串来保存，尤其在很多加密的场景中，例如保存密钥等。因为字节数组，除了写入文件或者以二进制的形式写入[数据库](http://www.2cto.com/database/)以外，无法直接转为为字符串，因为字符串结尾有\0，当然肯定还有其他原因。
下面提供几种[Java](http://www.2cto.com/kf/ware/Java/)中使用的方案：
方案一：直接利用BigInteger的方法，应该是最简单的方案了。
```java
/**
     * 利用签名辅助类，将字符串字节数组
     * @param str
     * @return
     */
    public static byte[] md5(String str)
    {
        byte[] digest = null;
        try
        {
            MessageDigest md = MessageDigest.getInstance("md5");
            return digest = md.digest(str.getBytes());
 
        } catch (NoSuchAlgorithmException e)
        {
            e.printStackTrace();
        }
        return null;
    }
    /**
     * 方式一
     * 
     * @param bytes
     * @return
     */
    public static String bytes2hex01(byte[] bytes)
    {
        /**
         * 第一个参数的解释，记得一定要设置为1
         *  signum of the number (-1 for negative, 0 for zero, 1 for positive).
         */
        BigInteger bigInteger = new BigInteger(1, bytes);
        return bigInteger.toString(16);
    }
```
方案二：将每个字节与0xFF进行与运算，然后转化为10进制，然后借助于Integer再转化为16进制
```java
/**
     * 方式二
     * 
     * @param bytes
     * @return
     */
    public static String bytes2hex02(byte[] bytes)
    {
        StringBuilder sb = new StringBuilder();
        String tmp = null;
        for (byte b : bytes)
        {
            // 将每个字节与0xFF进行与运算，然后转化为10进制，然后借助于Integer再转化为16进制
            tmp = Integer.toHexString(0xFF & b);
            if (tmp.length() == 1)// 每个字节8为，转为16进制标志，2个16进制位
            {
                tmp = "0" + tmp;
            }
            sb.append(tmp);
        }
 
        return sb.toString();
 
    }
```
方案三：分别取出字节的高四位与低四位然后分别得出10进制0-15这样的值，再利用一个字符串数组完美完成。对于转化的理解，当然最推荐第三种方式了。
```java
/**
     * 方式三
     * 
     * @param bytes
     * @return
     */
    public static String bytes2hex03(byte[] bytes)
    {
        final String HEX = "0123456789abcdef";
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        for (byte b : bytes)
        {
            // 取出这个字节的高4位，然后与0x0f与运算，得到一个0-15之间的数据，通过HEX.charAt(0-15)即为16进制数
            sb.append(HEX.charAt((b >> 4) & 0x0f));
            // 取出这个字节的低位，与0x0f与运算，得到一个0-15之间的数据，通过HEX.charAt(0-15)即为16进制数
            sb.append(HEX.charAt(b & 0x0f));
        }
 
        return sb.toString();
    }
```
