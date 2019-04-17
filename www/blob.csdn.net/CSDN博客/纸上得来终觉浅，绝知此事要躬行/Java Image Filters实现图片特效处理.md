# Java Image Filters实现图片特效处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年10月07日 20:56:35[boonya](https://me.csdn.net/boonya)阅读数：3194









     Java Image Filters 是由 Jhlabs 开发的一组用来处理 Java 图像的类库，提供各种常用的图像处理效果，例如反转色、扭曲、水波纹、凹凸、黑白效果等等数十种效果，如下图所示，更多的效果请看其网站首页：
- ![](http://www.jhlabs.com/ip/filters/thumbs/ChannelMixFilter.jpg)[ChannelMixFilter](http://www.jhlabs.com/ip/filters/ChannelMixFilter.html) -
 Mixes the RGB channels
- ![](http://www.jhlabs.com/ip/filters/thumbs/ContrastFilter.jpg)[ContrastFilter](http://www.jhlabs.com/ip/filters/ContrastFilter.html) -
 Adjusts brightness and contrast
- ![](http://www.jhlabs.com/ip/filters/thumbs/CurvesFilter.jpg)[CurvesFilter](http://www.jhlabs.com/ip/filters/CurvesFilter.html) -
 Apply adjustment curves to an image
- ![](http://www.jhlabs.com/ip/filters/thumbs/DiffusionFilter.jpg)[DiffusionFilter](http://www.jhlabs.com/ip/filters/DiffusionFilter.html) -
 Error-diffusion dithering
- ![](http://www.jhlabs.com/ip/filters/thumbs/DitherFilter.jpg)[DitherFilter](http://www.jhlabs.com/ip/filters/DitherFilter.html) -
 Ordered dithering
- ![](http://www.jhlabs.com/ip/filters/thumbs/ExposureFilter.jpg)[ExposureFilter](http://www.jhlabs.com/ip/filters/ExposureFilter.html) -
 Change the exposure of an image
- ![](http://www.jhlabs.com/ip/filters/thumbs/GainFilter.jpg)[GainFilter](http://www.jhlabs.com/ip/filters/GainFilter.html) -
 Adjusts gain and bias
- ![](http://www.jhlabs.com/ip/filters/thumbs/GammaFilter.jpg)[GammaFilter](http://www.jhlabs.com/ip/filters/GammaFilter.html) -
 Adjusts image gamma
- ![](http://www.jhlabs.com/ip/filters/thumbs/GrayFilter.jpg)[GrayFilter](http://www.jhlabs.com/ip/filters/GrayFilter.html) -
 Grays out an image
- ![](http://www.jhlabs.com/ip/filters/thumbs/GrayscaleFilter.jpg)[GrayscaleFilter](http://www.jhlabs.com/ip/filters/GrayscaleFilter.html) -
 Converts to grayscale
- ![](http://www.jhlabs.com/ip/filters/thumbs/HSBAdjustFilter.jpg)[HSBAdjustFilter](http://www.jhlabs.com/ip/filters/HSBAdjustFilter.html) -
 Adjusts hue, saturation and brightness
- ![](http://www.jhlabs.com/ip/filters/thumbs/InvertAlphaFilter.jpg)[InvertAlphaFilter](http://www.jhlabs.com/ip/filters/InvertAlphaFilter.html) -
 Inverts the alpha channel
- ![](http://www.jhlabs.com/ip/filters/thumbs/InvertFilter.jpg)[InvertFilter](http://www.jhlabs.com/ip/filters/InvertFilter.html) -
 Inverts image colors
- ![](http://www.jhlabs.com/ip/filters/thumbs/LevelsFilter.jpg)[LevelsFilter](http://www.jhlabs.com/ip/filters/LevelsFilter.html) -
 Adjust image levels
- ![](http://www.jhlabs.com/ip/filters/thumbs/LookupFilter.jpg)[LookupFilter](http://www.jhlabs.com/ip/filters/LookupFilter.html) -
 Change image colors with a lookup table
- ![](http://www.jhlabs.com/ip/filters/thumbs/Blank.jpg)[MapColorsFilter](http://www.jhlabs.com/ip/filters/MapColorsFilter.html) -
 Replace a color
- ![](http://www.jhlabs.com/ip/filters/thumbs/MaskFilter.jpg)[MaskFilter](http://www.jhlabs.com/ip/filters/MaskFilter.html) -
 Channel masking
- ![](http://www.jhlabs.com/ip/filters/thumbs/PosterizeFilter.jpg)[PosterizeFilter](http://www.jhlabs.com/ip/filters/PosterizeFilter.html) -
 Posterization
- ![](http://www.jhlabs.com/ip/filters/thumbs/QuantizeFilter.jpg)[QuantizeFilter](http://www.jhlabs.com/ip/filters/QuantizeFilter.html) -
 Quantize an image to 256 colors for, say, GIF export
- ![](http://www.jhlabs.com/ip/filters/thumbs/RescaleFilter.jpg)[RescaleFilter](http://www.jhlabs.com/ip/filters/RescaleFilter.html) -
 Multiplies colors by a scaling factor
- ![](http://www.jhlabs.com/ip/filters/thumbs/RGBAdjustFilter.jpg)[RGBAdjustFilter](http://www.jhlabs.com/ip/filters/RGBAdjustFilter.html) -
 Adjusts red, green and blue levels
- ![](http://www.jhlabs.com/ip/filters/thumbs/SolarizeFilter.jpg)[SolarizeFilter](http://www.jhlabs.com/ip/filters/SolarizeFilter.html) -
 Solarization
- ![](http://www.jhlabs.com/ip/filters/thumbs/ThresholdFilter.jpg)[ThresholdFilter](http://www.jhlabs.com/ip/filters/ThresholdFilter.html) -
 Thresholding
- ![](http://www.jhlabs.com/ip/filters/thumbs/TritoneFilter.jpg)[TritoneFilter](http://www.jhlabs.com/ip/filters/TritoneFilter.html) -
 Create a tri-tone image

From ： [http://www.oschina.net/p/javaimagefilters](http://www.oschina.net/p/javaimagefilters)

download： [http://www.oschina.net/action/project/go?id=8447&p=download](http://www.oschina.net/action/project/go?id=8447&p=download)



