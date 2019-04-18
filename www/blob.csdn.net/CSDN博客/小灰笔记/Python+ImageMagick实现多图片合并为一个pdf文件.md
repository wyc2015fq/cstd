# Python+ImageMagick实现多图片合并为一个pdf文件 - 小灰笔记 - CSDN博客





2017年06月28日 23:02:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3645








       前段时间买了不少书，现在手头的书籍积累的越来越多，北京这边租住的小屋子空间越来越满了。自从习惯了笔记本触摸板的手势操作之后，我偶觉得使用电脑看电子文档也挺享受的。于是想把自己的部分书籍使用手机拍照，然后合并成一个pdf文件。

       最初尝试过找成熟的Windows软件，但是始终没有找到一个好用的软件。想写脚本处理，一直也没有实现。偶然查看ImageMagick软件的说明，找到了可以批量合并的方法。于是，这个功能终于可以轻松实现了。

       写了一个简单的小脚本：

import os



defCompressImage(image_name):

    os.system("convert -resize\"600x800>\" %s %s" % (image_name,image_name))



def CompressAll():

       ext_names = ['.JPG','.jpg','.jepg']

       for each_image in os.listdir('./'):

              for ext_name in ext_names:

                     ifeach_image.endswith(ext_name):

                           CompressImage(each_image)

                            break

CompressAll()

os.system("convert*.JPG book.pdf")

       脚本运行之后，生成了一个pdf文件。打开文件如下：

![](https://img-blog.csdn.net/20170628230103063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       从上面的结果可以看出，图片的批量合成pdf文件的功能是实现了。自然，合成的时候需要考虑文件的排序。后期查一下排序的规律为文件重命名一下即可。



