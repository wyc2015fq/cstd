# 使用Goolge开源工具zxing实现二维码读写工具（带Logo） - 零度的博客专栏 - CSDN博客
2017年12月15日 10:43:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：327
工具类：QrCodeCreateUtil.java
```
package org.zwc.zxingtest;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Hashtable;
import javax.imageio.ImageIO;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.EncodeHintType;
import com.google.zxing.LuminanceSource;
import com.google.zxing.ReaderException;
import com.google.zxing.Result;
import com.google.zxing.WriterException;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;
import com.google.zxing.qrcode.QRCodeWriter;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;
/**
 * Created by zhangwenchao on 2017/11/24.
 */
public class QrCodeCreateUtil {
    /**
     * 生成包含字符串信息的二维码图片----不带logo,去白框
     *
     * @param outputStream 文件输出流路径
     * @param content      二维码携带信息
     * @param qrCodeSize   二维码图片大小
     * @param imageFormat  二维码的格式
     * @throws WriterException
     * @throws IOException
     */
    public static boolean createQrCode(OutputStream outputStream, String content,
                                       int qrCodeSize, String imageFormat)
            throws WriterException, IOException {
        //设置二维码纠错级别Map
        Hashtable<EncodeHintType, ErrorCorrectionLevel> hintMap = new Hashtable<EncodeHintType, ErrorCorrectionLevel>();
        // 矫错级别
        hintMap.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);
        //新建QRCodeWriter
        QRCodeWriter qrCodeWriter = new QRCodeWriter();
        //创建比特矩阵(位矩阵)的QR码编码的字符串
        BitMatrix byteMatrix = qrCodeWriter.encode(content, BarcodeFormat.QR_CODE, qrCodeSize, qrCodeSize, hintMap);
        // 使BufferedImage勾画QRCode  (matrixWidth 是行二维码像素点)
        int matrixWidth = byteMatrix.getWidth();
        //新建一个内存缓存图片----图片大小为qrCodeSize-200(注：byteMatrix会存在一些白色边框，根据生成图片的大小白色边框大小不一样)
        BufferedImage image = new BufferedImage(matrixWidth - 200, matrixWidth - 200, BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        //Graphics2D graphics = (Graphics2D) image.getGraphics();
        graphics.setColor(Color.WHITE);
        graphics.fillRect(0, 0, matrixWidth, matrixWidth); //填充白色在图像上
        // 使用比特矩阵画并保存图像
        graphics.setColor(Color.BLACK);
        for (int i = 0; i < matrixWidth; i++) {
            for (int j = 0; j < matrixWidth; j++) {
                if (byteMatrix.get(i, j)) {
                    //去掉边框
                    graphics.fillRect(i - 100, j - 100, 1, 1); //画一个黑色像素
                }
            }
        }
        return ImageIO.write(image, imageFormat, outputStream);
    }
    /**
     * 生成包含字符串信息的二维码图片----带logo
     * 增加Logo其实就是两张图片叠加
     *
     * @param outputStream 文件输出流路径
     * @param content      二维码携带信息
     * @param qrCodeSize   二维码图片大小
     * @param imageFormat  二维码的格式
     * @throws WriterException
     * @throws IOException
     */
    public static boolean createQrCodeWithLogo(OutputStream outputStream, String content,
                                       int qrCodeSize, String imageFormat,String logoImage)
            throws WriterException, IOException {
        //设置二维码纠错级别Map
        Hashtable<EncodeHintType, ErrorCorrectionLevel> hintMap = new Hashtable<EncodeHintType, ErrorCorrectionLevel>();
        // 矫错级别
        hintMap.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);
        //新建QRCodeWriter
        QRCodeWriter qrCodeWriter = new QRCodeWriter();
        //创建比特矩阵(位矩阵)的QR码编码的字符串
        BitMatrix byteMatrix = qrCodeWriter.encode(content, BarcodeFormat.QR_CODE, qrCodeSize, qrCodeSize, hintMap);
        // 使BufferedImage勾画QRCode  (matrixWidth 是行二维码像素点)
        int matrixWidth = byteMatrix.getWidth();
        //新建一个内存缓存图片----图片大小为qrCodeSize-200(注：byteMatrix会存在一些白色边框，根据生成图片的大小白色边框大小不一样)
        BufferedImage image = new BufferedImage(matrixWidth - 200, matrixWidth - 200, BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        //Graphics2D graphics = (Graphics2D) image.getGraphics();
        graphics.setColor(Color.WHITE);
        graphics.fillRect(0, 0, matrixWidth, matrixWidth); //填充白色在图像上
        // 使用比特矩阵画并保存图像
        graphics.setColor(Color.BLACK);
        for (int i = 0; i < matrixWidth; i++) {
            for (int j = 0; j < matrixWidth; j++) {
                if (byteMatrix.get(i, j)) {
                    //去掉边框
                    graphics.fillRect(i - 100, j - 100, 1, 1); //画一个黑色像素
                }
            }
        }
        /**
         * z
         */
        File logoPic = new File(logoImage);  //logo文件
        if (!logoPic.isFile()){
               System.out.print("Logo file is not find !");
               System.exit(0);
        }
        BufferedImage logoBufferedImage = ImageIO.read(logoPic); //读取logo图片
        /**
         * 重新设置logo的大小,设置为二维码图片的20%,因为过大会盖掉二维码
         */
         int widthLogo =  logoBufferedImage.getWidth(null)>image.getWidth()*2/10? (image.getWidth()*2/10) : logoBufferedImage.getWidth(null);
         int heightLogo = logoBufferedImage.getHeight(null)>image.getHeight()*2/10? (image.getHeight()*2/10) : logoBufferedImage.getHeight(null);
        // 计算logo图片放置位置---logo放在中心
        int x = (image.getWidth() - widthLogo) / 2;
        int y = (image.getHeight() - heightLogo) / 2;
        //开始绘制Logo到二维码图片上
        graphics.drawImage(logoBufferedImage, x, y, widthLogo, heightLogo, null);
        graphics.drawRoundRect(x, y, widthLogo, heightLogo, 15, 15);
        graphics.setStroke(new BasicStroke(2));  //边框
        graphics.setColor(Color.GREEN); //边框颜色
        graphics.drawRect(x, y, widthLogo, heightLogo);
        graphics.dispose();
        logoBufferedImage.flush();
        image.flush();
        return ImageIO.write(image, imageFormat, outputStream);
    }
    /**
     * 读二维码并输出携带的信息
     */
    public static void readQrCode(InputStream inputStream) throws IOException {
        //从输入流中获取字符串信息
        BufferedImage image = ImageIO.read(inputStream);
        //将图像转换为二进制位图源
        LuminanceSource source = new BufferedImageLuminanceSource(image);
        BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
        QRCodeReader reader = new QRCodeReader();  //
        Result result = null;
        try {
            result = reader.decode(bitmap);
        } catch (ReaderException e) {
            e.printStackTrace();
        }
        System.out.println(result.getText());
    }
    /**
     * 测试代码
     * @throws WriterException
     */
    public static void main(String[] args) throws IOException, WriterException {
        createQrCode(new FileOutputStream(new File("E:\\qrcode.jpg")), "WE8sASDFDFSDSDF12sdfe3", 800, "JPEG");
        createQrCodeWithLogo(new FileOutputStream(new File("E:\\qrcodeWithLogo.jpg")), "WE8sASDFDFSDSDF12sdfe3", 800, "JPEG","E:\\ling.jpg");
        readQrCode(new FileInputStream(new File("E:\\qrcode.jpg")));
        readQrCode(new FileInputStream(new File("E:\\qrcodeWithLogo.jpg")));
    }
}
```
测试结果：
生成二维码（带Logo）：读取结果：WE8sASDFDFSDSDF12sdfe3
