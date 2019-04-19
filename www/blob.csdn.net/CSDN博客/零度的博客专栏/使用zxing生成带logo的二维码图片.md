# 使用zxing生成带logo的二维码图片 - 零度的博客专栏 - CSDN博客
2016年10月19日 17:41:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：1874
很多时候需要我们在生成的二维码中间加logo图片，二维码加logo图片，思想很简单就是：就是将logo图片叠加到生成的二维码图片中间，两个图片叠加一起就是了。但是会遇到一个问题，如果logo加得太大的话，会导致二维码扫描不出来；加的太小则不怎么明显。这是我们需要调整生成二维码的容错率，容错率的设置从7%-30%，容错率设置的越高，二维码的有效像素点就越多。由于我们使用的是zxing,所以我们需要看看怎么设置zxing的二维码容错率。
如：hint.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H); 
ErrorCorrectionLevel就是容错率。另外，发现默认生成出来的图片，除了二维码以外，还有一圈白色边框比较宽，如果背景不是白色的话比较难看。查看zxing源码QRCodeWriter.java中，有个变量QUIET_ZONE_SIZE；是控制的边框的宽度,默认是4，最小值是1，这里我们修改成0或者1都可以，再生成图片，效果就很好了。下面给出生成带logo的二维码图片的代码：
```
package zmx.zxing.test;
import java.awt.AlphaComposite;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.Hashtable;
import javax.imageio.ImageIO;
import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.Base64OutputStream;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.NotFoundException;
import com.google.zxing.WriterException;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;
/**
 * 
 * @author zhangwenchao
 *
 */
public class QRCodeKit {
    public static final String QRCODE_DEFAULT_CHARSET = "UTF-8"; //默认编码
    public static final int QRCODE_DEFAULT_HEIGHT = 300;
    public static final int QRCODE_DEFAULT_WIDTH = 300;
    private static final int BLACK = 0xFF000000;
    private static final int WHITE = 0xFFFFFFFF;
    
    public static void main(String[] args) throws IOException, NotFoundException{
        String data = "http://www.baidu.com";
        File logoFile = new File("D:/projects/testWeb/src/zmx/zxing/test/logo.png");
        BufferedImage image = QRCodeKit.createQRCodeWithLogo(data, logoFile);
        ImageIO.write(image, "png", new File("D:/projects/testWeb/src/zmx/zxing/test/michael_zxing_logo.png"));
        System.out.println("done");
    }
    /**
     * Create qrcode with default settings
     * @author zhangwenchao
     * @param data
     * @return
     */
    public static BufferedImage createQRCode(String data) {
        return createQRCode(data, QRCODE_DEFAULT_WIDTH, QRCODE_DEFAULT_HEIGHT);
    }
    /**
     * Create qrcode with default charset
     * @author zhangwenchao
     * @param data
     * @param width
     * @param height
     * @return
     */
    public static BufferedImage createQRCode(String data, int width, int height) {
        return createQRCode(data, QRCODE_DEFAULT_CHARSET, width, height);
    }
    /**
     * Create qrcode with specified charset
     * @author zhangwenchao
     * @param data
     * @param charset
     * @param width
     * @param height
     * @return
     */
    @SuppressWarnings({ "unchecked", "rawtypes" })
    public static BufferedImage createQRCode(String data, String charset, int width, int height) {
    	Hashtable hint = new Hashtable();
        hint.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
        hint.put(EncodeHintType.CHARACTER_SET, charset);
        return createQRCode(data, charset, hint, width, height);
    }
    /**
     * Create qrcode with specified hint
     * @author zhangwenchao
     * @param data
     * @param charset
     * @param hint
     * @param width
     * @param height
     * @return
     */
    public static BufferedImage createQRCode(String data, String charset, Hashtable<EncodeHintType, ?> hint, int width,
            int height) {
        BitMatrix matrix;
        try {
            matrix = new MultiFormatWriter().encode(new String(data.getBytes(charset), charset), BarcodeFormat.QR_CODE,
                    width, height, hint);
            return toBufferedImage(matrix);
        } catch (WriterException e) {
            throw new RuntimeException(e.getMessage(), e);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage(), e);
        }
    }
    public static BufferedImage toBufferedImage(BitMatrix matrix) {
        int width = matrix.getWidth();
        int height = matrix.getHeight();
        BufferedImage image = new BufferedImage(width, height,
                BufferedImage.TYPE_INT_RGB);
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                image.setRGB(x, y, matrix.get(x, y) ? BLACK : WHITE);
            }
        }
        return image;
    }
    /**
     * Create qrcode with default settings and logo
     * @author zhangwenchao
     * @param data
     * @param logoFile
     * @return
     */
    public static BufferedImage createQRCodeWithLogo(String data, File logoFile) {
        return createQRCodeWithLogo(data, QRCODE_DEFAULT_WIDTH, QRCODE_DEFAULT_HEIGHT, logoFile);
    }
    /**
     * Create qrcode with default charset and logo
     * @author zhangwenchao
     * @param data
     * @param width
     * @param height
     * @param logoFile
     * @return
     */
    public static BufferedImage createQRCodeWithLogo(String data, int width, int height, File logoFile) {
        return createQRCodeWithLogo(data, QRCODE_DEFAULT_CHARSET, width, height, logoFile);
    }
    /**
     * Create qrcode with specified charset and logo
     * @author zhangwenchao
     * @param data
     * @param charset
     * @param width
     * @param height
     * @param logoFile
     * @return
     */
    @SuppressWarnings({ "unchecked", "rawtypes" })
    public static BufferedImage createQRCodeWithLogo(String data, String charset, int width, int height, File logoFile) {
    	Hashtable hint = new Hashtable();
        hint.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);
        hint.put(EncodeHintType.CHARACTER_SET, charset);
        return createQRCodeWithLogo(data, charset, hint, width, height, logoFile);
    }
    /**
     * Create qrcode with specified hint and logo
     * @author zhangwenchao
     * @param data
     * @param charset
     * @param hint
     * @param width
     * @param height
     * @param logoFile
     * @return
     */
    public static BufferedImage createQRCodeWithLogo(String data, String charset, Hashtable<EncodeHintType, ?> hint,
            int width, int height, File logoFile) {
        try {
            BufferedImage qrcode = createQRCode(data, charset, hint, width, height);
            BufferedImage logo = ImageIO.read(logoFile);
            int deltaHeight = height - logo.getHeight();
            int deltaWidth = width - logo.getWidth();
            BufferedImage combined = new BufferedImage(height, width, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g = (Graphics2D) combined.getGraphics();
            g.drawImage(qrcode, 0, 0, null);
            g.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 1f));
            g.drawImage(logo, (int) Math.round(deltaWidth / 2), (int) Math.round(deltaHeight / 2), null);
            return combined;
        } catch (IOException e) {
            throw new RuntimeException(e.getMessage(), e);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage(), e);
        }
    }
    /**
     * Return base64 for image
     * @author zhangwenchao
     * @param image
     * @return
     */
    public static String getImageBase64String(BufferedImage image) {
        String result = null;
        try {
            ByteArrayOutputStream os = new ByteArrayOutputStream();
            OutputStream b64 = new Base64OutputStream(os);
            ImageIO.write(image, "png", b64);
            result = os.toString("UTF-8");
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e.getMessage(), e);
        } catch (IOException e) {
            throw new RuntimeException(e.getMessage(), e);
        }
        return result;
    }
    /**
     * Decode the base64Image data to image
     * @author zhangwenchao
     * @param base64ImageString
     * @param file
     */
    public static void convertBase64StringToImage(String base64ImageString, File file) {
        FileOutputStream os;
        try {
            Base64 d = new Base64();
            byte[] bs = d.decode(base64ImageString);
            os = new FileOutputStream(file.getAbsolutePath());
            os.write(bs);
            os.close();
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e.getMessage(), e);
        } catch (IOException e) {
            throw new RuntimeException(e.getMessage(), e);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage(), e);
        }
    }
}
```

