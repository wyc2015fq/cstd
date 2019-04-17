# C# OCR AspriseOcr测试使用图像文字中英文识别 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月25日 10:30:18[boonya](https://me.csdn.net/boonya)阅读数：3272








OCR AspriseOcr测试使用中英文识别：因为使用AspriseOcr是需要商业授权码的，故使用网上破解dll包进行测试，测试的结果并不理想，可能是因为版本太低所致。如果真的需要做好OCR应用建议购买其授权码。

官网地址：[http://asprise.com/](http://asprise.com/)

### 应用场景
- 图像扫描：对图片进行文字识别
- 文件扫描：对PDF文件进行文字识别
- 条码扫描：对二维码条码进行扫描识别

### 测试示例

注意：需要下载的DLL分别是AspriseOCR.dll、DevIL.dll、ILU.dll将其放在生成的bin/Debug生成目录下，然后就可以测试了，网上有人说需要.net 2.0这个是谬传，我用的是.net framwork 4.5 ，DLL包不用放在注册表里去，放在可执行目录即可，程序运行时会自动加载，因为使用的是DllImport方式导入。

定义接口：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OCR
{
    interface ImageRecognitionInterface
    {

        /// <summary>
        /// 识别图像
        /// </summary>
        /// <param name="imgPath"></param>
        /// <returns></returns>
        string Recognize(string imgPath);
    }
}
```

编写工具类和实现：

```
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OCR.ImageRecognition
{
    /// <summary>
    /// AspriseOCR
    /// </summary>
    /// DLL包引用：AspriseOCR.dll、DevIL.dll、ILU.dll
    /// 下载地址:http://asprise.com/product/ocr/download.php?lang=csharp
    public class AspriseOCR:ImageRecognitionInterface
    {
        [DllImport("AspriseOCR.dll", EntryPoint = "OCR", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr OCR(string file, int type);

        [DllImport("AspriseOCR.dll", EntryPoint = "OCRpart", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr OCRpart(string file, int type, int startX, int startY, int width, int height);

        [DllImport("AspriseOCR.dll", EntryPoint = "OCRBarCodes", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr OCRBarCodes(string file, int type);

        [DllImport("AspriseOCR.dll", EntryPoint = "OCRpartBarCodes", CallingConvention = CallingConvention.Cdecl)]
        static extern IntPtr OCRpartBarCodes(string file, int type, int startX, int startY, int width, int height);


        /// <summary>
        /// 识别图像
        /// </summary>
        /// <param name="imgPath"></param>
        /// <returns></returns>
        public string Recognize(string imgPath)
        {
            try
            {
                int startX = 0, startY = 0;
                Image image = Image.FromFile(imgPath);
                return Marshal.PtrToStringAnsi(OCRpart(imgPath, -1, startX, startY, image.Width, image.Height));
            }
            catch (Exception ex)
            {
                Console.WriteLine("识别图像错误:"+ex.Message);
            }
            return "";
        }
    }
}
```

### 测试效果

这里通过解析中英文能力来测试

英文识别

![](https://img-blog.csdn.net/20180725102708621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180725102752459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

中文识别

![](https://img-blog.csdn.net/20180725102855928?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180725102925982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可见，效果不是很理想，还是要购买其正规渠道的产品才行。

### 示例和包下载

网上的分比较贵，不建议去下载，因为没什么用，我这里分享一个大家可以做测试使用。

链接：[https://pan.baidu.com/s/1Lo3lSnHVvYCfDp-tf9oLnQ](https://pan.baidu.com/s/1Lo3lSnHVvYCfDp-tf9oLnQ)

密码：fmai

AspriseOcr也支持Java图像识别：参考[http://zxs19861202.iteye.com/blog/1749780](http://zxs19861202.iteye.com/blog/1749780)



