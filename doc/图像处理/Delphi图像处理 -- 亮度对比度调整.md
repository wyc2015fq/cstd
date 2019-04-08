# Delphi图像处理 -- 亮度/对比度调整

2007年09月07日 23:15:00 [阿发伯](https://me.csdn.net/maozefa) 阅读数：14410



阅读提示：

​    **《**[**Delphi图像处理**](http://blog.csdn.net/maozefa/article/details/7188354)**》系列以效率为侧重点，一般代码为PASCAL，核心代码采用BASM。**

​    **《C++图像处理》系列以代码清晰，可读性为主，全部使用C++代码。**

​    **尽可能保持二者内容一致，可相互对照。**

   **本文代码必须包括文章《Delphi图像处理 -- 数据类型及公用过程》中的ImageData.pas单元。**

  

​        本人已经写过几篇关于亮度调整的文章，但是关于图像的对比度调整的过程和文章却一直没有写，其原因是一直没找到一个好的算法。可能有人会说，图像的亮度，对比度调整是最简单的图形操作，其算法网上可说是一搜一大把，确实如此，可就是这最简单的操作，网上的文章却五花八门，我拣几个试了一下，好像都不太理想，关键是算法太简单，实际操作效果不好，于是想，Photoshop的对比度还是较好的，而且也通用，但偏偏网上没有介绍它的算法，用了大半天时间研究了一下，再花了1个来小时写了个Delphi过程，试了一下，居然和Photoshop的对比度调整完全一样的效果！于是认真写了个测试程序，把亮度和对比度放在一起进行调整（亮度和对比度处理过程为各自独立的，其中亮度过程基本是本BLOG文章《[GDI+ 在Delphi程序的应用 -- 调整图像亮度](http://blog.csdn.net/maozefa/archive/2007/01/28/1496383.aspx)》的代码），可是效果却和Photoshop大不一样了，是什么原因呢，Photoshop的亮度调整算法是最简单的那种，与我的亮度过程做出来的是一样的（效果比较图参见《[GDI+ 在Delphi程序的应用 -- 线性调整图像亮度](http://blog.csdn.net/maozefa/archive/2007/08/25/1758962.aspx)》），而前面说了，对比度过程算法也是和Photoshop一样的，可放在一起调整就不行了，无论是先调整亮度，还是先调整对比度都这样。后来仔细分析了一下，Photoshop是用一个函数处理亮度/对比度，而且亮度调整是按对比度的正负分别对待的，下面是实现代码（包括一个简单的例子）：

```delphi
function _CheckRgb(Rgb: Integer): Integer;
asm
    test    eax, eax
    jge     @@1
    xor     eax, eax
    ret
@@1:
    cmp     eax, 255
    jle     @@2
    mov     eax, 255
@@2:
end;
 
procedure ImageBrightContrast(var Data: TImageData; Bright, Contrast, Threshold: Integer);
var
  vs: TGrayTable;
  cv: Single;
  i, v: Integer;
  height, dataOffset: Integer;
begin
  if (Bright = 0) and (Contrast = 0) then Exit;
  if Contrast <= -255 then cv := -1 else cv := Contrast / 255;
  if (Contrast > 0) and (Contrast < 255) then
    cv := 1 / (1 - cv) - 1;
  for i := 0 to 255 do
  begin
    if Contrast > 0 then v := _CheckRgb(i + bright) else v := i;
    if Contrast >= 255 then
    begin
      if v >= Threshold then v := 255 else v := 0;
    end
    else
      v := _CheckRgb(v + Round((v - Threshold) * cv));
    if Contrast <= 0 then vs[i] := _CheckRgb(v + bright) else vs[i] := v;
  end;
  asm
    mov     eax, Data
    call    _SetDataRegs
    mov     height, edx
    mov     dataOffset, ebx
    lea     esi, vs;
@@yLoop:
    push    ecx
@@xLoop:
    movzx   eax, [edi].TARGBQuad.Blue
    movzx   ebx, [edi].TARGBQuad.Green
    movzx   edx, [edi].TARGBQuad.Red
    mov     al, [esi+eax]
    mov     bl, [esi+ebx]
    mov     dl, [esi+edx]
    mov     [edi].TARGBQuad.Blue, al
    mov     [edi].TARGBQuad.Green, bl
    mov     [edi].TARGBQuad.Red, dl
    add     edi, 4
    loop    @@xLoop
    pop     ecx
    add     edi, dataOffset
    dec     height
    jnz     @@yLoop
@@Exit:
  end;
end;
 
procedure TForm1.Button3Click(Sender: TObject);
var
  bmp: TGpBitmap;
  g: TGpGraphics;
  data: TImageData;
begin
  bmp := TGpBitmap.Create('..\media\source1.jpg');
  g := TGpGraphics.Create(Canvas.Handle);
  g.DrawImage(bmp, 0, 0);
  data := LockGpBitmap(bmp);
  ImageBrightContrast(data, 50, 0, 121);
  UnlockGpBitmap(bmp, data);
  g.DrawImage(bmp, data.Width, 0);
  g.Free;
  bmp.Free;
end;
```

 

​    下面对亮度/对比度的原理简单介绍一下。

​    一、Photoshop对比度算法。可以用下面的公式来表示：

​    (1)、nRGB = RGB + (RGB - Threshold) * Contrast / 255

​    公式中，nRGB表示图像像素新的R、G、B分量，RGB表示图像像素R、G、B分量，Threshold为给定的阈值，Contrast为处理过的对比度增量。

​    Photoshop对于对比度增量，是按给定值的正负分别处理的：

​    当增量等于-255时，是图像对比度的下端极限，此时，图像RGB各分量都等于阈值，图像呈全灰色，灰度图上只有1条线，即阈值灰度；

​    当增量大于-255且小于0时，直接用上面的公式计算图像像素各分量；

​    当增量等于 255时，是图像对比度的上端极限，实际等于设置图像阈值，图像由最多八种颜色组成，灰度图上最多8条线，即红、黄、绿、青、蓝、紫及黑与白；

​    当增量大于0且小于255时，则先按下面公式(2)处理增量，然后再按上面公式(1)计算对比度：

​    (2)、nContrast = 255 * 255 / (255 - Contrast) - 255

​    公式中的nContrast为处理后的对比度增量，Contrast为给定的对比度增量。

​    二、图像亮度调整。本文采用的是最常用的非线性亮度调整（Phoposhop CS3以下版本也是这种亮度调整方式，CS3及以上版本也保留了该亮度调整方式的选项）。

​    三、图像亮度/对比度综合调整算法。这个很简单，当亮度、对比度同时调整时，如果对比度大于0，现调整亮度，再调整对比度；当对比度小于0时，则相反，先调整对比度，再调整亮度。

​    在亮度/对比度调整函数ImageBrightContrast中，首先按前面介绍的原理制造了一个256个元素大小的查找表，然后对图像数据逐像素按R、G、B分量值在查找表中取得调整后的数据，因此处理速度相当快。

​    下面是对比度255时的运行截图：

![img](http://hi.csdn.net/attachment/201112/14/0_1323825573NlnM.gif)

 

​    **《Delphi图像处理》系列使用GDI+单元下载地址和说明见文章《**[**GDI+ for VCL基础 -- GDI+ 与 VCL**](http://blog.csdn.net/maozefa/article/details/1880484)**》。**

​    **因水平有限，错误在所难免，欢迎指正和指导。邮箱地址：**[**maozefa@hotmail.com**](mailto:maozefa@hotmail.com)

​    **这里可访问《Delphi图像处理 -- 文章索引》。**

 