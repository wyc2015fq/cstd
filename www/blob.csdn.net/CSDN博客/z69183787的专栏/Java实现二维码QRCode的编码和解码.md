# Java实现二维码QRCode的编码和解码 - z69183787的专栏 - CSDN博客
2015年08月05日 17:02:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3113
周末试用下Android手机的二维码扫描软件，扫描了下火车票、名片等等，觉得非常不错很有意思的。当然Java也可以实现这些，现在就分享下如何简单用Java实现二维码中QRCode的编码和解码（可以手机扫描验证）。
涉及到的一些主要类库，方便大家下载：
- 
编码 lib：[Qrcode_swetake.jar](http://www.micmiu.com/wp-content/uploads/2012/03/Qrcode_swetake-jar.zip) （官网介绍
 — http://www.swetake.com/qr/index-e.html）
- 
解码 lib：[qrcode.jar](http://www.micmiu.com/wp-content/uploads/2012/03/qrcode-jar.zip)                 （官网介绍
 — http://sourceforge.jp/projects/qrcode/）
QRcode的详细介绍可参考：[http://www.qrcode.com/en](http://www.qrcode.com/en)
后来发现一个更好的条形码和二维码的开源软件（ZXing），详见:[条形码/二维码之开源利器ZXing图文介绍](http://www.micmiu.com/enterprise-app/java-zxing/)
【一】、编码：
QRCodeEncoderHandler.java


```
```
|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374757677787980818283848586878889909192|packagemichael.qrcode;import java.awt.Color;import java.awt.Graphics2D;import java.awt.image.BufferedImage;import java.io.File;import javax.imageio.ImageIO;import com.swetake.util.Qrcode;/** * 二维码生成器 * @blog http://www.micmiu.com * @author Michael */publicclassQRCodeEncoderHandler{/**     * 生成二维码(QRCode)图片     * @param content     * @param imgPath     */publicvoidencoderQRCode(Stringcontent,StringimgPath){try{Qrcode qrcodeHandler=newQrcode();qrcodeHandler.setQrcodeErrorCorrect('M');qrcodeHandler.setQrcodeEncodeMode('B');qrcodeHandler.setQrcodeVersion(7);System.out.println(content);byte[]contentBytes=content.getBytes("gb2312");BufferedImage bufImg=newBufferedImage(140,140,BufferedImage.TYPE_INT_RGB);Graphics2D gs=bufImg.createGraphics();gs.setBackground(Color.WHITE);gs.clearRect(0,0,140,140);// 设定图像颜色 > BLACKgs.setColor(Color.BLACK);// 设置偏移量 不设置可能导致解析出错intpixoff=2;// 输出内容 > 二维码if(contentBytes.length&gt;0&amp;&amp;contentBytes.length&lt;120){boolean[][]codeOut=qrcodeHandler.calQrcode(contentBytes);for(inti=0;i&lt;codeOut.length;i++){for(intj=0;j&lt;codeOut.length;j++){if(codeOut[j][i]){gs.fillRect(j *3+pixoff,i *3+pixoff,3,3);}}}}else{System.err.println("QRCode content bytes length = "+contentBytes.length+" not in [ 0,120 ]. ");}gs.dispose();bufImg.flush();File imgFile=newFile(imgPath);// 生成二维码QRCode图片ImageIO.write(bufImg,"png",imgFile);}catch(Exceptione){e.printStackTrace();}}/**     * @param args the command line arguments     */publicstaticvoidmain(String[]args){StringimgPath="D:/test/twocode/Michael_QRCode.png";Stringcontent="Hello 大大、小小,welcome to QRCode!"+"\nMyblog [ http://sjsky.iteye.com ]"+"\nEMail [ sjsky007@gmail.com ]"+"\nTwitter [ @suncto ]";QRCodeEncoderHandler handler=newQRCodeEncoderHandler();handler.encoderQRCode(content,imgPath);System.out.println("encoder QRcode success");}}|
运行后生成的二维码图片如下：
![](http://www.micmiu.com/wp-content/uploads/2012/03/qrcode_encoder.png)
此时就可用手机的二维码扫描软件（本人用的：android 快拍二维码 ）来测试下，识别成功的截图如下：
![](http://www.micmiu.com/wp-content/uploads/2012/03/qrcode_andriod.png)
喜欢的朋友可以下载后试一试，做一些名片或者自己喜欢的东西。当然Java也可以对二维码图片解码，具体看下面关于解码的内容。
ps：有关上述代码中对输出内容限制的解释：由于各个版本、各个纠错等级的不同，实际容量是不一样的，上述限制只是为了演示简单实现控制而已。
【二】、解码：
QRCodeDecoderHandler.java

``
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273747576777879808182838485|packagemichael.qrcode;importjava.awt.image.BufferedImage;importjava.io.File;importjava.io.IOException;importjavax.imageio.ImageIO;importjp.sourceforge.qrcode.QRCodeDecoder;importjp.sourceforge.qrcode.data.QRCodeImage;importjp.sourceforge.qrcode.exception.DecodingFailedException;/*** @blog http://www.micmiu.com* @author Michael*/publicclassQRCodeDecoderHandler{/**     * 解码二维码     * @param imgPath     * @return String     */publicStringdecoderQRCode(StringimgPath){// QRCode 二维码图片的文件FileimageFile=newFile(imgPath);BufferedImagebufImg=null;StringdecodedData=null;try{bufImg=ImageIO.read(imageFile);QRCodeDecoderdecoder=newQRCodeDecoder();decodedData=newString(decoder.decode(newJ2SEImage(bufImg)));// try {// System.out.println(new String(decodedData.getBytes("gb2312"),// "gb2312"));// } catch (Exception e) {// // TODO: handle exception// }}catch(IOExceptione){System.out.println("Error: "+e.getMessage());e.printStackTrace();}catch(DecodingFailedExceptiondfe){System.out.println("Error: "+dfe.getMessage());dfe.printStackTrace();}returndecodedData;}/**     * @param args the command line arguments     */publicstaticvoidmain(String[]args){QRCodeDecoderHandlerhandler=newQRCodeDecoderHandler();StringimgPath="d:/test/twocode/Michael_QRCode.png";StringdecoderContent=handler.decoderQRCode(imgPath);System.out.println("解析结果如下：");System.out.println(decoderContent);System.out.println("========decoder success!!!");}classJ2SEImageimplementsQRCodeImage{BufferedImagebufImg;publicJ2SEImage(BufferedImagebufImg){this.bufImg=bufImg;}publicintgetWidth(){returnbufImg.getWidth();}publicintgetHeight(){returnbufImg.getHeight();}publicintgetPixel(intx,inty){returnbufImg.getRGB(x,y);}}}|
运行结果如下（解码出的内容和之前输入的内容一致 ）：
> 
解析结果如下：
Hello 大大、小小,welcome to QRCode!
Myblog [ http://sjsky.iteye.com ]
EMail [ sjsky007@gmail.com ]
Twitter [ @suncto ]
========decoder success!!!
