# zxing生成二维码去白边 - z69183787的专栏 - CSDN博客
2017年07月25日 17:04:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4216
今天遇到了二维码生成的图片需要切掉白边的情况，查看之前的二维码生成代码，发现有这么一行
```
```java
hints.put(EncodeHintType.MARGIN, 1);
```
```
感觉应该是设置边距的意思，果断改成
```
```java
hints.put(EncodeHintType.MARGIN, 0);
```
```
发现其实并不能去掉白边。
于是百度找到方案：
```java
private static BufferedImage getNormalQRCode(String content) throws WriterException {
        Hashtable<EncodeHintType, Object> hints = new Hashtable<EncodeHintType, Object>();
        hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
        hints.put(EncodeHintType.CHARACTER_SET, CHARSET);
        hints.put(EncodeHintType.MARGIN, 1);
        BitMatrix bitMatrix = new MultiFormatWriter().encode(content,
                BarcodeFormat.QR_CODE, QRCODE_SIZE, QRCODE_SIZE, hints);
        //调用去除白边方法
        bitMatrix = deleteWhite(bitMatrix);
        int width = bitMatrix.getWidth();
        int height = bitMatrix.getHeight();
        BufferedImage image = new BufferedImage(width, height,
                BufferedImage.TYPE_INT_RGB);
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                image.setRGB(x, y, bitMatrix.get(x, y) ? BLACK //int 0
                        : WHITE //int 255);
            }
        }
        return image;
    }
ImageIO.write(image, "jpg", response.getOutputStream());  
    private static BitMatrix deleteWhite(BitMatrix matrix) {
        int[] rec = matrix.getEnclosingRectangle();
        int resWidth = rec[2] + 1;
        int resHeight = rec[3] + 1;
        BitMatrix resMatrix = new BitMatrix(resWidth, resHeight);
        resMatrix.clear();
        for (int i = 0; i < resWidth; i++) {
            for (int j = 0; j < resHeight; j++) {
                if (matrix.get(i + rec[0], j + rec[1]))
                    resMatrix.set(i, j);
            }
        }
        return resMatrix;
    }
```
搞定
```java
最近项目里需要生成一些二维码，使用之后发现一些问题，生成之后的图片，白色边框区域太大了，导致二维码内容区域太小。 
百度了一下，有人说设置EncodeHintType.MARGIN属性即可，这个属性值为1-4，实际测试发现并没有什么卵用。（顺便说一下，一些比较老的版本中，这个EncodeHintType只有CHARACTER_SET和ERROR_CORRECTION两种属性设置，比较新的库才新增了其他属性，我这里使用的是3.0版本） 
后来找到另外一个解决办法：自己手动去除黑边，代码如下：
public static Bitmap Create2DCode(String str, int width, int height) {
        try {
            Hashtable<EncodeHintType, Object> hints = new Hashtable<EncodeHintType, Object>();
            hints.put(EncodeHintType.CHARACTER_SET, "utf-8");
            hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
            hints.put(EncodeHintType.MARGIN, 1);
            BitMatrix matrix = new QRCodeWriter().encode(str, BarcodeFormat.QR_CODE, width, height);
            matrix = deleteWhite(matrix);//删除白边
            width = matrix.getWidth();
            height = matrix.getHeight();
            int[] pixels = new int[width * height];
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (matrix.get(x, y)) {
                        pixels[y * width + x] = Color.BLACK;
                    } else {
                        pixels[y * width + x] = Color.WHITE;
                    }
                }
            }
            Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            bitmap.setPixels(pixels, 0, width, 0, 0, width, height);
            return bitmap;
        } catch (Exception e) {
            return null;
        }
    }
    private static BitMatrix deleteWhite(BitMatrix matrix) {
        int[] rec = matrix.getEnclosingRectangle();
        int resWidth = rec[2] + 1;
        int resHeight = rec[3] + 1;
        BitMatrix resMatrix = new BitMatrix(resWidth, resHeight);
        resMatrix.clear();
        for (int i = 0; i < resWidth; i++) {
            for (int j = 0; j < resHeight; j++) {
                if (matrix.get(i + rec[0], j + rec[1]))
                    resMatrix.set(i, j);
            }
        }
        return resMatrix;
    }
```
