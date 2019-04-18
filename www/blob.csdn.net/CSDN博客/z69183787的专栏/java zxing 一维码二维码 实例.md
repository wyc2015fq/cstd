# java zxing 一维码二维码 实例 - z69183787的专栏 - CSDN博客
2016年08月11日 06:15:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1783
```java
<!-- 一维码 二维码 -->
        <dependency>
            <groupId>com.google.zxing</groupId>
            <artifactId>core</artifactId>
            <version>2.2</version>
        </dependency>
        <dependency>
            <groupId>com.google.zxing</groupId>
            <artifactId>javase</artifactId>
            <version>2.2</version>
        </dependency>
```
```java
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import com.opensymphony.xwork2.ActionSupport;
import org.apache.log4j.Logger;
import org.apache.struts2.ServletActionContext;
import com.opensymphony.xwork2.ActionContext;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;
 
import java.io.IOException;
import javax.imageio.ImageIO;
 
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.LuminanceSource;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.Result;
import com.google.zxing.WriterException;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.client.j2se.MatrixToImageWriter;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.oned.Code128Writer;
import com.google.zxing.qrcode.QRCodeWriter;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
/**
 * 
 * @author hongwang.zhang 20150813
 *
 */
@ParentPackage("struts-default")
@Controller("qrCodeAction")
@Scope("prototype")
public class QrCodeAction extends ExternalBaseAjaxAction {
	/**
	 * 
	 */
	private static final long serialVersionUID = 3472734690385818126L;
    private static Logger logger = Logger.getLogger(QrCodeAction.class);
    private String keycode;
    private String width;
    private String size;
    private String height;
    
    private static final String IMAGETYPE = "png";
    @Action(value="odc")
    public void barCode(){
    	try {
			BarCode(request, response, keycode, width, height);
		} catch (ServletException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    @Action(value="tdc")
    public void qrCode(){
    	try {
    		QrCode(request, response, keycode, size);
		} catch (Exception e) {
			// TODO: handle exception
		}
    }
    
    public static void BarCode(HttpServletRequest req, HttpServletResponse resp,String keycode,String mwidth, String mheight)
            throws ServletException, IOException {
        if (keycode != null && !"".equals(keycode)) {
            ServletOutputStream stream = null;
            try {
                Code128Writer writer = new Code128Writer();
                int width=180;
                int height=60;
                if (mwidth != null && !"".equals(mwidth.trim())) {
                    try{
                        width=Integer.valueOf(mwidth);
                    } catch (NumberFormatException e) {
                                        //TODO output to log 
                    }
                } 
                if (mheight != null && !"".equals(mheight.trim())) {
                    try{
                        height = Integer.valueOf(mheight);
                    } catch (NumberFormatException e) {
                        //TODO output to log 
                    }
                }
                int codeWidth = 3 + // start guard
                        (7 * 6) + // left bars
                        5 + // middle guard
                        (7 * 6) + // right bars
                        3; // end guard
                codeWidth = Math.max(codeWidth, width);
                stream = resp.getOutputStream();
                BitMatrix bitMatrix = new MultiFormatWriter().encode(keycode,
                        BarcodeFormat.CODE_128, codeWidth, height, null);
     
                MatrixToImageWriter.writeToStream(bitMatrix, IMAGETYPE,
                        stream);
                
                
            } catch (WriterException e) {
                e.printStackTrace();
            } finally {
                if (stream != null) {
                    stream.flush();
                    stream.close();
                }
            }
        }
    }
      public static void QrCode(HttpServletRequest req, HttpServletResponse resp,String keycode,String msize){    
           if (keycode != null && !"".equals(keycode)) {
               ServletOutputStream stream = null;
               try {
                   int size=129;//Ĭ�϶�ά���С
                   if (msize != null && !"".equals(msize.trim())) {
                       try{
                           size=Integer.valueOf(msize);
                       } catch (NumberFormatException e) {
                           //TODO output to log
                       }
                   }
                   stream = resp.getOutputStream();
                   QRCodeWriter writer = new QRCodeWriter();
                   BitMatrix m = writer.encode(keycode, BarcodeFormat.QR_CODE, size, size);
                   MatrixToImageWriter.writeToStream(m, IMAGETYPE, stream);
               } catch (WriterException e) {
                   e.printStackTrace();
               } catch (IOException e) {
  				// TODO Auto-generated catch block
  				e.printStackTrace();
  			} finally {
                   if (stream != null) {
                       try {
  						stream.flush();
  						stream.close();
  					} catch (IOException e) {
  						// TODO Auto-generated catch block
  						e.printStackTrace();
  					}
                       
                   }
               }
           }
       }
	public void setKeycode(String keycode) {
        if(keycode != null){
            keycode = keycode.replace("-","");
        }
		this.keycode = keycode;
	}
    public String getWidth() {
        return width;
    }
    public void setWidth(String width) {
        this.width = width;
    }
    public String getSize() {
        return size;
    }
    public void setSize(String size) {
        this.size = size;
    }
    public String getHeight() {
        return height;
    }
    public void setHeight(String height) {
        this.height = height;
    }
}
```
