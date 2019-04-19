# 二维码生成库phpqrcode使用小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月29日 09:47:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：863
所属专栏：[后台架构](https://blog.csdn.net/column/details/31054.html)
<img src="data:image/png;base64,这里是base64编码内容" />
只需要里边的phpqrcode.php这一个文件就可以生成二维码了
我们需要用到里边的QRcode类（第2963行）的png()方法
public static function png($text, $outfile = false, $level = QR_ECLEVEL_L, $size = 3, $margin = 4, $saveandprint=false)
第1个参数$text：二维码包含的内容，可以是链接、文字、json字符串等等；
第2个参数$outfile：默认为false，不生成文件，只将二维码图片返回输出；否则需要给出存放生成二维码图片的文件名及路径；
第3个参数$level：默认为L，这个参数可传递的值分别是L(QR_ECLEVEL_L，7%)、M(QR_ECLEVEL_M，15%)、Q(QR_ECLEVEL_Q，25%)、H(QR_ECLEVEL_H，30%)，这个参数控制二维码容错率，不同的参数表示二维码可被覆盖的区域百分比，也就是被覆盖的区域还能识别；
第4个参数$size：控制生成图片的大小，默认为4；
第5个参数$margin：控制生成二维码的空白区域大小；
第6个参数$saveandprint：保存二维码图片并显示出来，$outfile必须传递图片路径；

不生成文件，会直接输出二维码到浏览器中。
function scerweima2($url=''){
require_once 'phpqrcode.php';
$value = $url; //二维码内容
$errorCorrectionLevel = 'L'; //容错级别
$matrixPointSize = 5; //生成图片大小
//生成二维码图片
$QR = QRcode::png($value,false,$errorCorrectionLevel, $matrixPointSize, 2);
}
//调用查看结果
scerweima2('https://www.baidu.com');
带logo的
<?php 
echo "<h1>PHP QR Code by Dirk</h1><hr/>";
include 'phpqrcode.php';
$value = 'http://blog.csdn.net/luoshengkim?viewmode=contents'; //二维码内容
$errorCorrectionLevel = 'L'; //容错级别
$matrixPointSize = 6; //生成图片大小
// 生成二维码图片 
QRcode::png($value, 'qrcode.png', $errorCorrectionLevel, $matrixPointSize, 2);
echo "QR code generated"."<br/>";
//生成中间带logo的二维码
$logo = 'dirk.jpg'; // logo图片是你自己放到文件夹里的
$QR = 'qrcode.png'; //已经生成的原始二维码图
if($logo !== FALSE)
{ 
$QR = imagecreatefromstring(file_get_contents($QR));
$logo = imagecreatefromstring(file_get_contents($logo));
$QR_width = imagesx($QR);
$QR_height = imagesy($QR);
$logo_width = imagesx($logo);
$logo_height = imagesy($logo);
$logo_qr_width = $QR_width / 5;
$scale = $logo_width / $logo_qr_width;
$logo_qr_height = $logo_height / $scale;
$from_width = ($QR_width - $logo_qr_width) / 2;
//重新组合图片并调整大小
imagecopyresampled($QR, $logo, $from_width, $from_width, 0, 0, $logo_qr_width, $logo_qr_height, $logo_width, $logo_height);
} 
imagepng($QR,'qrWithLogo.png');
echo '<img src="qrWithLogo.png">';
TODO:
[https://github.com/endroid/qrcode](https://github.com/endroid/qrcode) 好像带logo生成，待测试
[http://www.cnblogs.com/txw1958/p/phpqrcode.html](http://www.cnblogs.com/txw1958/p/phpqrcode.html)
