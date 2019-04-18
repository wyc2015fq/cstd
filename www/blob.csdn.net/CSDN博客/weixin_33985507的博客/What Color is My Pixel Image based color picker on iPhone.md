# What Color is My Pixel? Image based color picker on iPhone - weixin_33985507的博客 - CSDN博客
2012年05月07日 16:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
My Postcards app has a design screen where you can change colors of things, so I needed a color picker. A very easy way to make a color picker on just about any platform is to make some kind of color pallet image file, display it for the user to click on, and do a call like: getPixelColorAtScreenLocation(x,y)
This was available back in 1983 on my [ZX-Spectrum](http://www.zxspectrum.net/), but alas Cocoa Touch doesn’t have it. (If I missed something I hope a reader will set me straight.) What are some alternatives then?
- Ditch the color pallet and use sliders to mix colors. Not very user friendly for the everyday user.
- Manually build a lookup table for position to color. Hard work and what if I want to use a different image?
- Parse the image file myself and find the color at x,y using some third party library. Maybe…
- Bend core graphics to my will and have it ‘parse’ the image file for me and make the uncompressed pixels available.
Googling around turned up a way to make the last technique work, which is good because we’re using existing APIs, and it will work for any image:
- Make an off screen bitmap image context that is uncompressed and has a known byte to pixel mapping.
- Re-draw the color picker image into that context.
- Turn x,y touch co-ordinates into a position in the context’s bitmap buffer.
- Turn the pixel bytes at the offset position into a UIColor.
I’ve wrapped this up in ColorPickerImageView, a subclass of UIImageView.  When you tap it, it uses this approach to get the pixel color you tapped.  You can also register a simple delegate with it to get a callback with the picked color. You put your own image in it, so you can make it a little color pallet in a corner or make it fill the screen. As it’s derived from UIImageView, you use it just like UIImageView in code or in Interface Builder. The  pixel grabbing source code is below, and the whole class is included in: [ColorPicker code to download and copy](http://www.markj.net/wp/wp-content/uploads/2009/03/colorpicker1.zip).
![](https://pic002.cnblogs.com/images/2012/331765/2012050716215255.png)
```
- (UIColor*) getPixelColorAtLocation:(CGPoint)point {
    UIColor* color = nil;
    CGImageRef inImage = self.image.CGImage;
    // Create off screen bitmap context to draw the image into. Format ARGB is 4 bytes for each pixel: Alpa, Red, Green, Blue
    CGContextRef cgctx = [self createARGBBitmapContextFromImage:inImage];
    if (cgctx == NULL) { return nil; /* error */ }
    size_t w = CGImageGetWidth(inImage);
    size_t h = CGImageGetHeight(inImage);
    CGRect rect = {{0,0},{w,h}}; 
    // Draw the image to the bitmap context. Once we draw, the memory
    // allocated for the context for rendering will then contain the
    // raw image data in the specified color space.
    CGContextDrawImage(cgctx, rect, inImage); 
    // Now we can get a pointer to the image data associated with the bitmap
    // context.
    unsigned char* data = CGBitmapContextGetData (cgctx);
    if (data != NULL) {
        //offset locates the pixel in the data from x,y.
        //4 for 4 bytes of data per pixel, w is width of one row of data.
        int offset = 4*((w*round(point.y))+round(point.x));
        int alpha =  data[offset];
        int red = data[offset+1];
        int green = data[offset+2];
        int blue = data[offset+3];
        NSLog(@"offset: %i colors: RGB A %i %i %i  %i",offset,red,green,blue,alpha);
        color = [UIColor colorWithRed:(red/255.0f) green:(green/255.0f) blue:(blue/255.0f) alpha:(alpha/255.0f)];
    }
    // When finished, release the context
    CGContextRelease(cgctx);
    // Free image data memory for the context
    if (data) { free(data); }
    return color;
}
```
```
- (CGContextRef) createARGBBitmapContextFromImage:(CGImageRef) inImage {
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
    // Get image width, height. We'll use the entire image.
    size_t pixelsWide = CGImageGetWidth(inImage);
    size_t pixelsHigh = CGImageGetHeight(inImage);
    // Declare the number of bytes per row. Each pixel in the bitmap in this
    // example is represented by 4 bytes; 8 bits each of red, green, blue, and
    // alpha.
    bitmapBytesPerRow   = (pixelsWide * 4);
    bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
    // Use the generic RGB color space.
    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    if (colorSpace == NULL)
    {
        fprintf(stderr, "Error allocating color space\n");
        return NULL;
    }
    // Allocate memory for image data. This is the destination in memory
    // where any drawing to the bitmap context will be rendered.
    bitmapData = malloc( bitmapByteCount );
    if (bitmapData == NULL)
    {
        fprintf (stderr, "Memory not allocated!");
        CGColorSpaceRelease( colorSpace );
        return NULL;
    }
    // Create the bitmap context. We want pre-multiplied ARGB, 8-bits
    // per component. Regardless of what the source image format is
    // (CMYK, Grayscale, and so on) it will be converted over to the format
    // specified here by CGBitmapContextCreate.
    context = CGBitmapContextCreate (bitmapData,
                                     pixelsWide,
                                     pixelsHigh,
                                     8,      // bits per component
                                     bitmapBytesPerRow,
                                     colorSpace,
                                     kCGImageAlphaPremultipliedFirst);
    if (context == NULL)
    {
        free (bitmapData);
        fprintf (stderr, "Context not created!");
    }
    // Make sure and release colorspace before returning
    CGColorSpaceRelease( colorSpace );
    return context;
}
```
