# Java 处理 iphone拍照后 图片EXIF属性翻转90度的方法 - z69183787的专栏 - CSDN博客
2015年12月15日 21:25:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6943
http://blog.csdn.net/ghsau/article/details/8472486
翻转后 orientation 属性为6 。
```java
public static void main(String[] args) throws ImageProcessingException, IOException {
        File jpegFile= new File("C:\\Users\\Administrator\\Desktop\\IMG_0362.JPG");
        Metadata metadata = ImageMetadataReader.readMetadata(jpegFile);
        Directory directory = metadata.getDirectory(ExifIFD0Directory.class);
        JpegDirectory jpegDirectory = (JpegDirectory)metadata.getDirectory(JpegDirectory.class);
//        int orientation =0;
//        Metadata metadata = JpegMetadataReader.readMetadata(newFile);
//        for (Directory directory : metadata.getDirectories())
//        {
//            for (Tag tag : directory.getTags())
//            {
//                if ("Orientation".equalsIgnoreCase(tag.getTagName()))
//                {
//                    orientation=getOrientation(tag.getDescription());
//                }
//            }
//        }
//        Integer turn=360;
//        if(orientation==0||orientation==1)
//        {
//            turn=360;
//        }
//        else if(orientation==3)
//        {
//            turn=180;
//        }
//        else if(orientation==6)
//        {
//            turn=90;
//        }
//        else if(orientation==8)
//        {
//            turn=270;
//        }
        int orientation = 1;
        try {
            orientation = directory.getInt(ExifIFD0Directory.TAG_ORIENTATION);
        } catch (MetadataException me) {
            logger.warn("Could not get orientation");
        }
        System.out.println(orientation);
        BufferedImage src = ImageIO.read(jpegFile);
        BufferedImage des = RotateImage.Rotate(src, 90);
        ImageIO.write(des,"jpg", new File("C:\\Users\\Administrator\\Desktop\\IMG_0362.JPG"));
//        FileInputStream fip = new FileInputStream(jpegFile);
//        LLJTran llj = new LLJTran(fip);
//        try {
//            llj.read(LLJTran.READ_INFO, true);
//        } catch (LLJTranException e) {
//            e.printStackTrace();
//        }
//
//        Exif exif = (Exif) llj.getImageInfo();
//        Entry e = new Entry(Exif.RATIONAL);
//        exif.setTagValue(Exif.ORIENTATION_TOPLEFT,1,e,true);
//        llj.refreshAppx(); // Recreate Marker Data for changes done
//        //改写后的文件，文件必须存在
//        OutputStream out = new BufferedOutputStream(new FileOutputStream("C:\\Users\\Administrator\\Desktop\\IMG_0362.JPG"));
//        // Transfer remaining of image to output with new header.
//        try {
//            llj.xferInfo(null, out, LLJTran.REPLACE, LLJTran.REPLACE);
//        } catch (LLJTranException e1) {
//            e1.printStackTrace();
//        }
//        fip.close();
//        out.close();
//        llj.freeMemory();
    }
    public static int getOrientation(String orientation)
    {
        int tag = 0;
        if ("Top, left side (Horizontal / normal)".equalsIgnoreCase(orientation)) {
            tag = 1;
        } else if ("Top, right side (Mirror horizontal)".equalsIgnoreCase(orientation)) {
            tag = 2;
        } else if ("Bottom, right side (Rotate 180)".equalsIgnoreCase(orientation)) {
            tag = 3;
        } else if ("Bottom, left side (Mirror vertical)".equalsIgnoreCase(orientation)) {
            tag = 4;
        } else if ("Left side, top (Mirror horizontal and rotate 270 CW)".equalsIgnoreCase(orientation)) {
            tag = 5;
        } else if ("Right side, top (Rotate 90 CW)".equalsIgnoreCase(orientation)) {
            tag = 6;
        } else if ("Right side, bottom (Mirror horizontal and rotate 90 CW)".equalsIgnoreCase(orientation)) {
            tag = 7;
        } else if ("Left side, bottom (Rotate 270 CW)".equalsIgnoreCase(orientation)) {
            tag = 8;
        }
        return  tag;
    }
```
翻转：
```java
package com.xxxx.xxx.xxx.xxx;
import java.awt.*;
import java.awt.image.BufferedImage;
/**
 * Created by Administrator on 2015/12/15.
 */
public class RotateImage {
    public static BufferedImage Rotate(Image src, int angel) {
        int src_width = src.getWidth(null);
        int src_height = src.getHeight(null);
        // calculate the new image size
        Rectangle rect_des = CalcRotatedSize(new Rectangle(new Dimension(
                src_width, src_height)), angel);
        BufferedImage res = null;
        res = new BufferedImage(rect_des.width, rect_des.height,
                BufferedImage.TYPE_INT_RGB);
        Graphics2D g2 = res.createGraphics();
        // transform
        g2.translate((rect_des.width - src_width) / 2,
                (rect_des.height - src_height) / 2);
        g2.rotate(Math.toRadians(angel), src_width / 2, src_height / 2);
        g2.drawImage(src, null, null);
        return res;
    }
    public static Rectangle CalcRotatedSize(Rectangle src, int angel) {
        // if angel is greater than 90 degree, we need to do some conversion
        if (angel >= 90) {
            if(angel / 90 % 2 == 1){
                int temp = src.height;
                src.height = src.width;
                src.width = temp;
            }
            angel = angel % 90;
        }
        double r = Math.sqrt(src.height * src.height + src.width * src.width) / 2;
        double len = 2 * Math.sin(Math.toRadians(angel) / 2) * r;
        double angel_alpha = (Math.PI - Math.toRadians(angel)) / 2;
        double angel_dalta_width = Math.atan((double) src.height / src.width);
        double angel_dalta_height = Math.atan((double) src.width / src.height);
        int len_dalta_width = (int) (len * Math.cos(Math.PI - angel_alpha
                - angel_dalta_width));
        int len_dalta_height = (int) (len * Math.cos(Math.PI - angel_alpha
                - angel_dalta_height));
        int des_width = src.width + len_dalta_width * 2;
        int des_height = src.height + len_dalta_height * 2;
        return new Rectangle(new Dimension(des_width, des_height));
    }
}
```

