# 数据结构和算法系列  - geohash算法原理及实现方式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年02月12日 17:26:32[initphp](https://me.csdn.net/initphp)阅读数：1081








## 原地址：[http://www.cnblogs.com/dengxinglin/archive/2012/12/14/2817761.html](http://www.cnblogs.com/dengxinglin/archive/2012/12/14/2817761.html)



### **geohash有以下几个特点：**

首先，geohash用一个字符串表示经度和纬度两个坐标。某些情况下无法在两列上同时应用索引 （例如MySQL 4之前的版本，Google App Engine的数据层等），利用geohash，只需在一列上应用索引即可。

其次，geohash表示的并不是一个点，而是一个矩形区域。比如编码wx4g0ec19，它表示的是一个矩形区域。 使用者可以发布地址编码，既能表明自己位于北海公园附近，又不至于暴露自己的精确坐标，有助于隐私保护。

第三，编码的前缀可以表示更大的区域。例如wx4g0ec1，它的前缀wx4g0e表示包含编码wx4g0ec1在内的更大范围。 这个特性可以用于附近地点搜索。首先根据用户当前坐标计算geohash（例如wx4g0ec1）然后取其前缀进行查询 （SELECT * FROM place WHERE geohash LIKE 'wx4g0e%'），即可查询附近的所有地点。

Geohash比直接用经纬度的高效很多。



### **Geohash的原理**

**Geohash的最简单的解释就是：将一个经纬度信息，转换成一个可以排序，可以比较的字符串编码**

首先将纬度范围(-90, 90)平分成两个区间(-90,0)、(0, 90)，如果目标纬度位于前一个区间，则编码为0，否则编码为1。由于39.92324属于(0, 90)，所以取编码为1。然后再将(0, 90)分成 (0, 45), (45, 90)两个区间，而39.92324位于(0, 45)，所以编码为0。以此类推，直到精度符合要求为止，得到纬度编码为1011 1000 1100 0111 1001。
|纬度范围|划分区间0|划分区间1|39.92324所属区间|
|----|----|----|----|
|(-90, 90)|(-90, 0.0)|(0.0, 90)|1|
|(0.0, 90)|(0.0, 45.0)|(45.0, 90)|0|
|(0.0, 45.0)|(0.0, 22.5)|(22.5, 45.0)|1|
|(22.5, 45.0)|(22.5, 33.75)|(33.75, 45.0)|1|
|(33.75, 45.0)|(33.75, 39.375)|(39.375, 45.0)|1|
|(39.375, 45.0)|(39.375, 42.1875)|(42.1875, 45.0)|0|
|(39.375, 42.1875)|(39.375, 40.7812)|(40.7812, 42.1875)|0|
|(39.375, 40.7812)|(39.375, 40.0781)|(40.0781, 40.7812)|0|
|(39.375, 40.0781)|(39.375, 39.7265)|(39.7265, 40.0781)|1|
|(39.7265, 40.0781)|(39.7265, 39.9023)|(39.9023, 40.0781)|1|
|(39.9023, 40.0781)|(39.9023, 39.9902)|(39.9902, 40.0781)|0|
|(39.9023, 39.9902)|(39.9023, 39.9462)|(39.9462, 39.9902)|0|
|(39.9023, 39.9462)|(39.9023, 39.9243)|(39.9243, 39.9462)|0|
|(39.9023, 39.9243)|(39.9023, 39.9133)|(39.9133, 39.9243)|1|
|(39.9133, 39.9243)|(39.9133, 39.9188)|(39.9188, 39.9243)|1|
|(39.9188, 39.9243)|(39.9188, 39.9215)|(39.9215, 39.9243)|1|

经度也用同样的算法，对(-180, 180)依次细分，得到116.3906的编码为1101 0010 1100 0100 0100。

|经度范围|划分区间0|划分区间1|116.3906所属区间|
|----|----|----|----|
|(-180, 180)|(-180, 0.0)|(0.0, 180)|1|
|(0.0, 180)|(0.0, 90.0)|(90.0, 180)|1|
|(90.0, 180)|(90.0, 135.0)|(135.0, 180)|0|
|(90.0, 135.0)|(90.0, 112.5)|(112.5, 135.0)|1|
|(112.5, 135.0)|(112.5, 123.75)|(123.75, 135.0)|0|
|(112.5, 123.75)|(112.5, 118.125)|(118.125, 123.75)|0|
|(112.5, 118.125)|(112.5, 115.312)|(115.312, 118.125)|1|
|(115.312, 118.125)|(115.312, 116.718)|(116.718, 118.125)|0|
|(115.312, 116.718)|(115.312, 116.015)|(116.015, 116.718)|1|
|(116.015, 116.718)|(116.015, 116.367)|(116.367, 116.718)|1|
|(116.367, 116.718)|(116.367, 116.542)|(116.542, 116.718)|0|
|(116.367, 116.542)|(116.367, 116.455)|(116.455, 116.542)|0|
|(116.367, 116.455)|(116.367, 116.411)|(116.411, 116.455)|0|
|(116.367, 116.411)|(116.367, 116.389)|(116.389, 116.411)|1|
|(116.389, 116.411)|(116.389, 116.400)|(116.400, 116.411)|0|
|(116.389, 116.400)|(116.389, 116.394)|(116.394, 116.400)|0|

接下来将经度和纬度的编码合并，奇数位是纬度，偶数位是经度，得到编码 11100 11101 00100 01111 00000 01101 01011 00001。

最后，用0-9、b-z（去掉a, i, l, o）这32个字母进行base32编码，得到(39.92324, 116.3906)的编码为wx4g0ec1。
|十进制|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|base32|0|1|2|3|4|5|6|7|8|9|b|c|d|e|f|g|
|十进制|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
|base32|h|j|k|m|n|p|q|r|s|t|u|v|w|x|y|z|

解码算法与编码算法相反，先进行base32解码，然后分离出经纬度，最后根据二进制编码对经纬度范围进行细分即可，这里不再赘述。

### **实现代码：**

php版本的实现方式：[http://blog.dixo.net/downloads/geohash-php-class/](http://blog.dixo.net/downloads/geohash-php-class/)  我下载了一个上传的

** php：**

geohash.class.php



```php
<?php
 /**
  * Geohash generation class
  * http://blog.dixo.net/downloads/
  *
  * This file copyright (C) 2008 Paul Dixon (paul@elphin.com)
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  */
 
 
 
 /**
 * Encode and decode geohashes
 *
 */
 class Geohash
 {
     private $coding="0123456789bcdefghjkmnpqrstuvwxyz";
     private $codingMap=array();
     
     public function Geohash()
     {
         //build map from encoding char to 0 padded bitfield
         for($i=0; $i<32; $i++)
         {
             $this->codingMap[substr($this->coding,$i,1)]=str_pad(decbin($i), 5, "0", STR_PAD_LEFT);
         }
         
     }
     
     /**
     * Decode a geohash and return an array with decimal lat,long in it
     */
     public function decode($hash)
     {
         //decode hash into binary string
         $binary="";
         $hl=strlen($hash);
         for($i=0; $i<$hl; $i++)
         {
             $binary.=$this->codingMap[substr($hash,$i,1)];
         }
         
         //split the binary into lat and log binary strings
         $bl=strlen($binary);
         $blat="";
         $blong="";
         for ($i=0; $i<$bl; $i++)
         {
             if ($i%2)
                 $blat=$blat.substr($binary,$i,1);
             else
                 $blong=$blong.substr($binary,$i,1);
             
         }
         
         //now concert to decimal
         $lat=$this->binDecode($blat,-90,90);
         $long=$this->binDecode($blong,-180,180);
         
         //figure out how precise the bit count makes this calculation
         $latErr=$this->calcError(strlen($blat),-90,90);
         $longErr=$this->calcError(strlen($blong),-180,180);
                 
         //how many decimal places should we use? There's a little art to
         //this to ensure I get the same roundings as geohash.org
         $latPlaces=max(1, -round(log10($latErr))) - 1;
         $longPlaces=max(1, -round(log10($longErr))) - 1;
         
         //round it
         $lat=round($lat, $latPlaces);
         $long=round($long, $longPlaces);
         
         return array($lat,$long);
     }
 
     
     /**
     * Encode a hash from given lat and long
     */
     public function encode($lat,$long)
     {
         //how many bits does latitude need?    
         $plat=$this->precision($lat);
         $latbits=1;
         $err=45;
         while($err>$plat)
         {
             $latbits++;
             $err/=2;
         }
         
         //how many bits does longitude need?
         $plong=$this->precision($long);
         $longbits=1;
         $err=90;
         while($err>$plong)
         {
             $longbits++;
             $err/=2;
         }
         
         //bit counts need to be equal
         $bits=max($latbits,$longbits);
         
         //as the hash create bits in groups of 5, lets not
         //waste any bits - lets bulk it up to a multiple of 5
         //and favour the longitude for any odd bits
         $longbits=$bits;
         $latbits=$bits;
         $addlong=1;
         while (($longbits+$latbits)%5 != 0)
         {
             $longbits+=$addlong;
             $latbits+=!$addlong;
             $addlong=!$addlong;
         }
         
         
         //encode each as binary string
         $blat=$this->binEncode($lat,-90,90, $latbits);
         $blong=$this->binEncode($long,-180,180,$longbits);
         
         //merge lat and long together
         $binary="";
         $uselong=1;
         while (strlen($blat)+strlen($blong))
         {
             if ($uselong)
             {
                 $binary=$binary.substr($blong,0,1);
                 $blong=substr($blong,1);
             }
             else
             {
                 $binary=$binary.substr($blat,0,1);
                 $blat=substr($blat,1);
             }
             $uselong=!$uselong;
         }
         
         //convert binary string to hash
         $hash="";
         for ($i=0; $i<strlen($binary); $i+=5)
         {
             $n=bindec(substr($binary,$i,5));
             $hash=$hash.$this->coding[$n];
         }
         
         
         return $hash;
     }
     
     /**
     * What's the maximum error for $bits bits covering a range $min to $max
     */
     private function calcError($bits,$min,$max)
     {
         $err=($max-$min)/2;
         while ($bits--)
             $err/=2;
         return $err;
     }
     
     /*
     * returns precision of number
     * precision of 42 is 0.5
     * precision of 42.4 is 0.05
     * precision of 42.41 is 0.005 etc
     */
     private function precision($number)
     {
         $precision=0;
         $pt=strpos($number,'.');
         if ($pt!==false)
         {
             $precision=-(strlen($number)-$pt-1);
         }
         
         return pow(10,$precision)/2;
     }
     
     
     /**
     * create binary encoding of number as detailed in http://en.wikipedia.org/wiki/Geohash#Example
     * removing the tail recursion is left an exercise for the reader
     */
     private function binEncode($number, $min, $max, $bitcount)
     {
         if ($bitcount==0)
             return "";
         
         #echo "$bitcount: $min $max<br>";
             
         //this is our mid point - we will produce a bit to say
         //whether $number is above or below this mid point
         $mid=($min+$max)/2;
         if ($number>$mid)
             return "1".$this->binEncode($number, $mid, $max,$bitcount-1);
         else
             return "0".$this->binEncode($number, $min, $mid,$bitcount-1);
     }
     
 
     /**
     * decodes binary encoding of number as detailed in http://en.wikipedia.org/wiki/Geohash#Example
     * removing the tail recursion is left an exercise for the reader
     */
     private function binDecode($binary, $min, $max)
     {
         $mid=($min+$max)/2;
         
         if (strlen($binary)==0)
             return $mid;
             
         $bit=substr($binary,0,1);
         $binary=substr($binary,1);
         
         if ($bit==1)
             return $this->binDecode($binary, $mid, $max);
         else
             return $this->binDecode($binary, $min, $mid);
     }
 }
 
 
 
 
 
 
 ?>
```


Java版本：



```java
View Code
 import java.io.File;  
 import java.io.FileInputStream;  
 import java.util.BitSet;  
 import java.util.HashMap;  
   
   
 public class Geohash {  
   
     private static int numbits = 6 * 5;  
     final static char[] digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8',  
             '9', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j', 'k', 'm', 'n', 'p',  
             'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };  
       
     final static HashMap<Character, Integer> lookup = new HashMap<Character, Integer>();  
     static {  
         int i = 0;  
         for (char c : digits)  
             lookup.put(c, i++);  
     }  
   
     public static void main(String[] args)  throws Exception{  
   
         System.out.println(new Geohash().encode(45, 125));  
               
     }  
 
     public double[] decode(String geohash) {  
         StringBuilder buffer = new StringBuilder();  
         for (char c : geohash.toCharArray()) {  
   
             int i = lookup.get(c) + 32;  
             buffer.append( Integer.toString(i, 2).substring(1) );  
         }  
           
         BitSet lonset = new BitSet();  
         BitSet latset = new BitSet();  
           
         //even bits  
         int j =0;  
         for (int i=0; i< numbits*2;i+=2) {  
             boolean isSet = false;  
             if ( i < buffer.length() )  
               isSet = buffer.charAt(i) == '1';  
             lonset.set(j++, isSet);  
         }  
           
         //odd bits  
         j=0;  
         for (int i=1; i< numbits*2;i+=2) {  
             boolean isSet = false;  
             if ( i < buffer.length() )  
               isSet = buffer.charAt(i) == '1';  
             latset.set(j++, isSet);  
         }  
           
         double lon = decode(lonset, -180, 180);  
         double lat = decode(latset, -90, 90);  
           
         return new double[] {lat, lon};       
     }  
       
     private double decode(BitSet bs, double floor, double ceiling) {  
         double mid = 0;  
         for (int i=0; i<bs.length(); i++) {  
             mid = (floor + ceiling) / 2;  
             if (bs.get(i))  
                 floor = mid;  
             else  
                 ceiling = mid;  
         }  
         return mid;  
     }  
       
       
     public String encode(double lat, double lon) {  
         BitSet latbits = getBits(lat, -90, 90);  
         BitSet lonbits = getBits(lon, -180, 180);  
         StringBuilder buffer = new StringBuilder();  
         for (int i = 0; i < numbits; i++) {  
             buffer.append( (lonbits.get(i))?'1':'0');  
             buffer.append( (latbits.get(i))?'1':'0');  
         }  
         return base32(Long.parseLong(buffer.toString(), 2));  
     }  
   
     private BitSet getBits(double lat, double floor, double ceiling) {  
         BitSet buffer = new BitSet(numbits);  
         for (int i = 0; i < numbits; i++) {  
             double mid = (floor + ceiling) / 2;  
             if (lat >= mid) {  
                 buffer.set(i);  
                 floor = mid;  
             } else {  
                 ceiling = mid;  
             }  
         }  
         return buffer;  
     }  
   
     public static String base32(long i) {  
         char[] buf = new char[65];  
         int charPos = 64;  
         boolean negative = (i < 0);  
         if (!negative)  
             i = -i;  
         while (i <= -32) {  
             buf[charPos--] = digits[(int) (-(i % 32))];  
             i /= 32;  
         }  
         buf[charPos] = digits[(int) (-i)];  
   
         if (negative)  
             buf[--charPos] = '-';  
         return new String(buf, charPos, (65 - charPos));  
     }  
   
 }
```




### 一些观点

引用阿里云以为技术专家的博客上的讨论：

1.两个离的越近，geohash的结果相同的位数越多，对么？

这一点是有些用户对geohash的误解，虽然geo确实尽可能的将位置相近的点hash到了一起，可是这并不是严格意义上的(实际上也并不可能，因为毕竟多一维坐标)，

例如在方格4的左下部分的点和大方格1的右下部分的点离的很近，可是它们的geohash值一定是相差的相当远，因为头一次的分块就相差太大了，很多时候我们对geohash的值进行简单的排序比较，结果貌似真的能够找出相近的点，并且似乎还是按照距离的远近排列的，可是实际上会有一些点被漏掉了。

上述这个问题，可以通过搜索一个格子，周围八个格子的数据，统一获取后再进行过滤。这样就在编码层次解决了这个问题。

2.既然不能做到将相近的点hash值也相近，那么geohash的意义何在呢？

我觉得geohash还是相当有用的一个算法，毕竟这个算法通过无穷的细分，能确保将每一个小块的geohash值确保在一定的范围之内，这样就为灵活的周边查找和范围查找提供了可能。



常见的一些应用场景

A、如果想查询附近的点？如何操作

查出改点的gehash值，然后到数据库里面进行前缀匹配就可以了。



B、如果想查询附近点，特定范围内，例如一个点周围500米的点，如何搞？

可以查询结果，在结果中进行赛选，将geohash进行解码为经纬度，然后进行比较



 *在纬度相等的情况下：

 *经度每隔0.00001度，距离相差约1米；

 *每隔0.0001度，距离相差约10米；

 *每隔0.001度，距离相差约100米；

 *每隔0.01度，距离相差约1000米；

 *每隔0.1度，距离相差约10000米。

 *在经度相等的情况下：

 *纬度每隔0.00001度，距离相差约1.1米；

 *每隔0.0001度，距离相差约11米；

 *每隔0.001度，距离相差约111米；

 *每隔0.01度，距离相差约1113米；

 *每隔0.1度，距离相差约11132米。



参考资料：

[http://iamzhongyong.iteye.com/blog/1399333](http://iamzhongyong.iteye.com/blog/1399333)

[http://tech.idv2.com/2011/06/17/location-search/](http://tech.idv2.com/2011/06/17/location-search/)

[http://blog.sina.com.cn/s/blog_62ba0fdd0100tul4.html](http://blog.sina.com.cn/s/blog_62ba0fdd0100tul4.html)





