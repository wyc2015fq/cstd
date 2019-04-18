# 提高zxing生成二维码的容错率及zxing生成二维码的边框设置 - z69183787的专栏 - CSDN博客
2016年06月01日 10:11:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1313
提高zxing生成二维码的容错率及zxing生成二维码的边框设置(zxi，有需要的朋友可以参考下。
最近做了一个项目要生成二维码,跟几年前不同,最近大家都喜欢在二维码中间加logo.
加logo倒是不难,两个图片叠一起就是了,但是遇到一个新问题,logo加得太大的话,会导致二维码扫描不出来;加的太小则不怎么明显.
上网看看网上在线生成二维码的地方,发现都有容错率的设置,从7%-30%,容错率越高,二维码的有效像素点就越多.
由于我们使用的是zxing,所以我们需要看看怎么设置zxing的二维码容错率.
翻阅了zxing的源码,在QRCodeWriter.java中有这么一段:
ErrorCorrectionLevel errorCorrectionLevel = ErrorCorrectionLevel.L;
    if (hints != null) {
      ErrorCorrectionLevel requestedECLevel = (ErrorCorrectionLevel) hints.get(EncodeHintType.ERROR_CORRECTION);
      if (requestedECLevel != null) {
        errorCorrectionLevel = requestedECLevel;
      }
    }
这段描述了默认的容错级别是L,代码中注释是7%,显然是比较低的.设置方式也显而易见了,通过一个hashtables传入参数即可.因此将生成二维码的代码改成如下 
/**
	 * 用字符串生成二维码
	 * 
	 * @param str
	 * @author zhouzhe@lenovo-cw.com
	 * @return
	 * @throws WriterException
	 */
	public static Bitmap Create2DCode(String str, int picWidth, int picHeight) throws WriterException {
		// 生成二维矩阵,编码时指定大小,不要生成了图片以后再进行缩放,这样会模糊导致识别失败
		Hashtable hints = new Hashtable();
		hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
		BitMatrix matrix = new MultiFormatWriter().encode(str, BarcodeFormat.QR_CODE, picWidth, picHeight, hints);
		int width = matrix.getWidth();
		int height = matrix.getHeight();
		// 二维矩阵转为一维像素数组,也就是一直横着排了
		int[] pixels = new int[width * height];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (matrix.get(x, y)) {
					pixels[y * width + x] = 0xff000000;
				} else {
					pixels[y * width + x] = 0xffffffff;
				}
			}
		}
		Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		// 通过像素数组生成bitmap,具体参考api
		bitmap.setPixels(pixels, 0, width, 0, 0, width, height);
		return bitmap;
	}
这样 生成的二维码想对默认的容错级别L,像素密度大不少,容错率也提升了,这样中间的logo图便可以放更大的logo图了. 
另外发现默认生成出来的图片,除了二维码以外,还有一圈白色边框比较宽,如果背景不是白色的话比较难看.随即查了一下,
发现还是在QRCodeWriter.java中,有个变量QUIET_ZONE_SIZE; 是控制的边框的宽度,默认是4,最小值是1,这里我们修改成0或者1都可以.再生成图片,发现效果就很好了.
