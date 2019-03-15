# PS------Smart blur算法实现

2018年03月30日 14:18:38 [Trent1985](https://me.csdn.net/Trent1985) 阅读数：793



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/79757008

偶尔在网上看到的，关于Smart blur效果的算法实现：

[![img](http://2.bp.blogspot.com/_6ZIqLRChuQg/TFfrJfQrTVI/AAAAAAAAAns/FFDf-L2gbek/s320/orange.jpg)](http://2.bp.blogspot.com/_6ZIqLRChuQg/TFfrJfQrTVI/AAAAAAAAAns/FFDf-L2gbek/s1600/orange.jpg)

*原图*

![img](http://3.bp.blogspot.com/_6ZIqLRChuQg/TFfrDTcYfoI/AAAAAAAAAnk/PGELFoiLj40/s320/orangleBlur.jpg)

*Smart Blur效果图*

*算法过程如下：*

*1，每个像素计算一次；*

*2，每个像素计算以该像素为中心的半径R的正方形区域(边长2R+1)内的方差delta；*

*3，设定方差阈值Threshold；*

*4，计算原图的模糊效果图A(均值模糊或者高斯模糊)；*

*5，如果像素P(x,y)对应的方差delta<Threshold，则按照delta/Threshold的比例将原图与A进行混合或插值，得到结果像素Q(x,y)，如果delta >= Threshold，则Q(x,y) = P(x,y)；*

The algorithm is quite straightforward:

1. March through the image pixel by pixel.
2. For each pixel, analyze an adjacent region (say, the adjoining 5 pixel by 5 pixel square).
3. Calculate some metric of pixel variance for that region.
4. Compare the variance to some predetermined threshold value.
5. If the variance exceeds the threshold, do nothing.

*6. If the variance is less than the threshold, apply blurring to the source pixel. But vary the amount of blurring according to the variance: low variance, more blurring (high variance, less blurring).*

*Here's the full code for SmartBlurFilter:*

```java
import java.awt.image.Kernel;
import java.awt.image.BufferedImage;
import java.awt.image.ConvolveOp;
import java.awt.Graphics;
 
public class SmartBlurFilter {
 
   double SENSITIVITY = 10;
   int REGION_SIZE = 5;
 
   float [] kernelArray = {
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1
   };
 
   Kernel kernel = new Kernel( 9,9, normalizeKernel( kernelArray ) );
 
   float [] normalizeKernel( float [] ar ) {
           int n = 0;
           for (int i = 0; i < ar.length; i++)
           n += ar[i];
           for (int i = 0; i < ar.length; i++)
           ar[i] /= n;
 
           return ar;
   }
 
   public double lerp( double a,double b, double amt) {
           return a + amt * ( b - a );
   }
 
   public double getLerpAmount( double a, double cutoff ) {
 
           if ( a > cutoff )
           return 1.0;
 
           return a / cutoff;
   }
 
   public double rmsError( int [] pixels ) {
 
           double ave = 0;
 
           for ( int i = 0; i < pixels.length; i++ )
           ave += ( pixels[ i ] >> 8 ) & 255;
 
           ave /= pixels.length;
 
           double diff = 0;
           double accumulator = 0;
 
           for ( int i = 0; i < pixels.length; i++ ) {
                   diff = ( ( pixels[ i ] >> 8 ) & 255 ) - ave;
                   diff *= diff;
                   accumulator += diff;
           }
 
           double rms = accumulator / pixels.length;
 
           rms = Math.sqrt( rms );
 
           return rms;
   }
 
   int [] getSample( BufferedImage image, int x, int y, int size ) {
 
           int [] pixels = {};
 
           try {
                   BufferedImage subimage = image.getSubimage( x,y, size, size );
                   pixels = subimage.getRGB( 0,0,size,size,null,0,size );
           }
           catch( Exception e ) {
                   // will arrive here if we requested
                   // pixels outside the image bounds
           }
           return pixels;
   }
 
   int lerpPixel( int oldpixel, int newpixel, double amt ) {
 
           int oldRed = ( oldpixel >> 16 ) & 255;
           int newRed = ( newpixel >> 16 ) & 255;
           int red = (int) lerp( (double)oldRed, (double)newRed, amt ) & 255;
 
           int oldGreen = ( oldpixel >> 8 ) & 255;
           int newGreen = ( newpixel >> 8 ) & 255;
           int green = (int) lerp( (double)oldGreen, (double)newGreen, amt ) & 255;
 
           int oldBlue = oldpixel & 255;
           int newBlue = newpixel & 255;
           int blue = (int) lerp( (double)oldBlue, (double)newBlue, amt ) & 255;
 
           return ( red << 16 ) | ( green << 8 ) | blue;
   }
 
   int [] blurImage( BufferedImage image,
      int [] orig, int [] blur, double sensitivity ) {
 
           int newPixel = 0;
           double amt = 0;
           int size = REGION_SIZE;
 
           for ( int i = 0; i < orig.length; i++ ) {
                   int w = image.getWidth();
                   int [] pix = getSample( image, i % w, i / w, size );
                   if ( pix.length == 0 )
                   continue;
 
                   amt = getLerpAmount ( rmsError( pix ), sensitivity );
                   newPixel = lerpPixel( blur[ i ], orig[ i ],  amt );
                   orig[ i ] = newPixel;
           }
 
           return orig;
   }
 
 
   public BufferedImage filter( BufferedImage image ) {
 
           ConvolveOp convolver = new ConvolveOp(kernel, ConvolveOp.EDGE_NO_OP,
           null);
 
           // clone image into target
           BufferedImage target = new BufferedImage(image.getWidth(), image
           .getHeight(), image.getType());
           Graphics g = target.createGraphics();
           g.drawImage(image, 0, 0, null);
           g.dispose();
 
           int w = target.getWidth();
           int h = target.getHeight();
 
           // get source pixels
           int [] pixels = image.getRGB(0, 0, w, h, null, 0, w);
 
           // blur the cloned image
           target = convolver.filter(target, image);
 
           // get the blurred pixels
           int [] blurryPixels = target.getRGB(0, 0, w, h, null, 0, w);
 
           // go thru the image and interpolate values
           pixels = blurImage(image, pixels, blurryPixels, SENSITIVITY);
 
           // replace original pixels with new ones
           image.setRGB(0, 0, w, h, pixels, 0, w);
           return image;
   }
}
```