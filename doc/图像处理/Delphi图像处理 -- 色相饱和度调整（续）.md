# Delphi图像处理 -- 色相/饱和度调整（续）

2013年01月03日 19:29:26 [阿发伯](https://me.csdn.net/maozefa) 阅读数：4993



阅读提示：

​    **《**[**Delphi图像处理**](http://blog.csdn.net/maozefa/article/details/7188354)**》系列以效率为侧重点，一般代码为PASCAL，核心代码采用BASM。**

​    **《C++图像处理》系列以代码清晰，可读性为主，全部使用C++代码。**

​    **尽可能保持二者内容一致，可相互对照。**

​    **本文代码必须包括文章《Delphi图像处理 -- 数据类型及公用过程》中的ImageData.pas单元。**

 

​    最近一段时间，在整理以前的文章，前几天刚刚将**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》**一文修改整理完毕，可昨天不知怎么突然想到其实**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》**中的代码还有优化的余地，因为在我的几篇关于图像色相/饱和度调整的文章里，一直是按照SHL为基础写的代码，事实上Photoshop的色相/饱和度调整功能也确实是以SHL为基础的，但是毕竟Photoshop的色相/饱和度调整并不是SHL调整，无论是明度、色相还是饱和度都有着很大独立性，它们之间的关系是松散性的，并非象SHL那样，三者联系得很紧，在**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》**一文中，明度部分已经是独立调整的，但色相和饱和度部分还有着一些联系，主要是共用像素的S、L部分，但事实上，用SHL调整色相是很麻烦的，如果采用SHV来调整色相则会简单得多（当然，这只是指我采用的SSE代码，如果不是SSE代码，就很难说了，因为在**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》。**一文的色相调整代码中，只计算了中间值，最大值和最小值保留原来的，这样本身就节省了不少代码）。改进代码写完后，本想覆盖更新**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》**中的代码，但又觉得以前的代码还是保留为好，因此，把这改进后的代码作为续篇，以便二者之间对照比较：

```delphi
procedure GetBrightTable(Bright: Integer; var Table: TGrayTable);
asm
    push    ebx
    cmp     eax, -255
    jge     @@1
    mov     eax, -255
    jmp     @@2
@@1:
    cmp     eax, 255
    jle     @@2
    mov     eax, 255
@@2:
    push    eax
    mov     ebx, 255
    fild    dword ptr[esp]
    fwait
    mov     [esp], ebx
    fidiv   dword ptr[esp]// Bright / 255
    fwait
    xor     ecx, ecx
    test    eax, eax
    jg      @@Loop
    xor     ebx, ebx      // mask = Bright > 0? 255 : 0
@@Loop:
    mov     [esp], ecx
    xor     [esp], ebx
    fild    dword ptr[esp]
    fmul    st(0), st(1)
    fistp   dword ptr[esp]
    fwait
    mov     eax, [esp]
    add     eax, ecx
    mov     [edx], al    // Table[i] = (i ^ mask) * Bright / 255
    inc     edx
    inc     ecx
    cmp     ecx, 256
    jb      @@Loop
    ffree   st
    pop     eax
    pop     ebx
end;
 
procedure HSBSetBright(var Data: TImageData; const Table: TGrayTable);
asm
    push    ebp
    push    esi
    push    edi
    push    ebx
    mov     esi, edx
    call    _SetDataRegs
    mov     ebp, edx
@@yLoop:
    push    ecx
@@xLoop:
    movzx   eax, [edi].TARGBQuad.Blue
    movzx   edx, [edi].TARGBQuad.Green
    mov     al, [esi+eax]
    mov     dl, [esi+edx]
    mov     [edi].TARGBQuad.Blue, al
    mov     [edi].TARGBQuad.Green, dl
    movzx   eax, [edi].TARGBQuad.Red
    mov     al, [esi+eax]
    mov     [edi].TARGBQuad.Red, al
    add     edi, 4
    loop    @@xLoop
    pop     ecx
    add     edi, ebx
    dec     ebp
    jnz     @@yLoop
    pop     ebx
    pop     edi
    pop     esi
    pop     ebp
end;
 
procedure HSBSetHueAndSaturation(var Data: TImageData; Hv, Sv: Integer);
const
  _fc2: Single = 2.0;
  _fc4: Single = 4.0;
  _fc6: Single = 6.0;
  _fc128: Single = 128.0;
var
  Hv0: Integer;
  fHv: Single;
  width, height, datOffset: Integer;
asm
    push      esi
    push      edi
    push      ebx
    push      ecx
    mov       Hv0, edx
    call      _SetDataRegs
    mov       width, ecx
    mov       height, edx
    mov       datOffset, ebx
    pop       ebx             // Sv
    pxor      xmm7, xmm7
    pxor      xmm3, xmm3      // xmm3清零，便于色相处理时的或运算
    mov       eax, 1
    cvtsi2ss  xmm6, eax
    mov       eax, 60
    cvtsi2ss  xmm5, Hv0
    cvtsi2ss  xmm4, eax
    divss     xmm5, xmm4
    movss     fHv, xmm5       // fHv = Hv / 60
    mov       eax, 255
    cvtsi2ss  xmm5, ebx
    cvtsi2ss  xmm4, eax
    divss     xmm5, xmm4
    movss     xmm4, xmm5      // xmm4 = xmm5 = Sv / 255
    test      ebx, ebx
    jle       @@1
    movaps    xmm5, xmm6      // if (Sv > 0)
    subss     xmm5, xmm4      //   xmm5 = 1 / (1 - xmm4) - 1
    rcpss     xmm5, xmm5
    subss     xmm5, xmm6
@@1:
    pshufd    xmm5, xmm5, 0
@@yLoop:
    push      width
@@xLoop:
    movzx     ecx, [edi].TARGBQuad.Blue
    movzx     edx, [edi].TARGBQuad.Green
    movzx     eax, [edi].TARGBQuad.Red
    cmp       ecx, edx        // ecx = rgbMax
    jge       @@3             // edx = rgbMin
    xchg      ecx, edx
@@3:
    cmp       ecx, eax
    jge       @@4
    xchg      ecx, eax
@@4:
    cmp       edx, eax
    cmova     edx, eax
    mov       eax, ecx
    sub       eax, edx
    jz        @@next          // if (delta == 0) continue
 
    cvtsi2ss  xmm3, eax       // xmm3 = delta = rgbMax - rgbmin
    cmp       Hv0, 0
    jne       @@6
    movd      xmm0, [edi]
    punpcklbw xmm0, xmm7
    punpcklwd xmm0, xmm7
    cvtdq2ps  xmm0, xmm0
    jmp       @@20
@@6:
    // 按HSV调整色相，比HSL快40%
    movss     xmm2, fHv       // add = fHv
    cmp       cl, [edi].TARGBQuad.Red
    jne       @@8             // if (R == rgbMax) eax = G - B
    movzx     eax, [edi].TARGBQuad.Green
    movzx     esi, [edi].TARGBQuad.Blue
    jmp       @@10
@@8:
    cmp       cl, [edi].TARGBQuad.Green
    jne       @@9
    movzx     eax, [edi].TARGBQuad.Blue
    movzx     esi, [edi].TARGBQuad.Red
    addss     xmm2, _fc2      // if (G == rgbMax) eax = B - R; add += 2
    jmp       @@10
@@9:
    movzx     eax, [edi].TARGBQuad.Red
    movzx     esi, [edi].TARGBQuad.Green
    addss     xmm2, _fc4      // if (B == rgbMax) eax = R - G; add += 4
@@10:
    sub       eax, esi
    cvtsi2ss  xmm1, eax
    divss     xmm1, xmm3
    addss     xmm1, xmm2      // H = eax / delta + add
    comiss    xmm1, xmm7
    jae       @@11
    addss     xmm1, _fc6      // if (H < 0) H += 6
    jmp       @@12
@@11:
    comiss    xmm1, _fc6
    jb        @@12
    subss     xmm1, _fc6      // else if (H >= 6) H -= 6
@@12:
    cvtss2si  esi, xmm1       // index = Round(H)
    cvtsi2ss  xmm2, esi
    subss     xmm1, xmm2      // extra = H - index
    comiss    xmm1, xmm7      // if (extra < 0) // 如果index发生五入
    jae       @@13            // {
    dec       esi             //   index --
    addss     xmm1, xmm6      //   extra ++
@@13:                         // }
    test      esi, 1
    jnz       @@14
    movaps    xmm2, xmm1
    movss     xmm1, xmm6
    subss     xmm1, xmm2      // if ((index & 1) == 0) extra = 1 - extra
@@14:
    mulss     xmm1, xmm3      // xmm1 = delta * extra
    pslldq    xmm1, 4
    orps      xmm1, xmm3
    movlhps   xmm1, xmm7      // xmm1 = 0  0  delta*extra  delta
    cvtsi2ss  xmm0, ecx       // xmm0 = V = rgbMax
    pshufd    xmm0, xmm0, 0   // xmm0 = V  V  V  V
    subps     xmm0, xmm1      // xmm0 - xmm1 = NAN  V  T  P
    jmp       @@jmpTable[esi*4].Pointer
@@jmpTable:   dd  offset  @@H60
              dd  offset  @@H120
              dd  offset  @@H180
              dd  offset  @@H240
              dd  offset  @@H300
              dd  offset  @@H360
              dd  offset  @@H60// 当H=6.0时，SSE判断误差导致index=6，实际应为0
@@H360:                       // 300 - 359 (V, P, T)
    pshufd    xmm0, xmm0, 11100001b
    jmp       @@H60
@@H300:                       // 240 - 299 (T, P, V)
    pshufd    xmm0, xmm0, 11010010b
    jmp       @@H60
@@H240:                       // 180 - 239 (P, T, V)
    pshufd    xmm0, xmm0, 11000110b
    jmp       @@H60
@@H180:                       // 120 - 179 (P, V, T)
    pshufd    xmm0, xmm0, 11001001b
    jmp       @@H60
@@H120:                       // 60 - 119  (T, V, P)
    pshufd    xmm0, xmm0, 11011000b
@@H60:                        // 0 - 59    (V, T, P)
    test      ebx, ebx
    je        @@25
@@20:
    // 调整饱和度。先按HSL计算亮度
    add       ecx, edx        // ecx = rgbMar + rgbMin
    cvtsi2ss  xmm2, ecx
    divss     xmm2, _fc2      // xmm3 = L = ecx / 2
    pshufd    xmm2, xmm2, 0
    movaps    xmm1, xmm0
    subps     xmm0, xmm2      // rgb0 = rgb - L
    test      ebx, ebx
    jle       @@23
    // 如果饱和度增量为正，按SHL计算饱和度以控制新的饱和度上限
    comiss    xmm2, _fc128    // if (Sv > 0)
    jb        @@21            // {
    neg       ecx
    add       ecx, 510        //   if (L >= 128) ecx = 510 - ecx
@@21:
    cvtsi2ss  xmm2, ecx
    divss     xmm3, xmm2      //   S = delta / ecx
    addss     xmm3, xmm4
    comiss    xmm3, xmm6
    jb        @@23
    subss     xmm3, xmm4      //   if ((xmm4 + S) >= 1)
    rcpss     xmm2, xmm3      //     rgb0 = rgb0 * (1 / S - 1)
    subss     xmm2, xmm6      //   else
    pshufd    xmm2, xmm2, 0   //     // 事先已经计算好存放在xmm5中
    mulps     xmm0, xmm2      //     rgb0 = rgb0 * (1 / (1 - xmm4) - 1)
    jmp       @@24            // }
@@23:                         // else
    mulps     xmm0, xmm5      //   rgb0 = rgb0 * fSv
@@24:
    addps     xmm0, xmm1      // rgb += rgb0
@@25:
    cvtps2dq  xmm0, xmm0
    packssdw  xmm0, xmm7
    packuswb  xmm0, xmm7
    mov       al, [edi].TARGBQuad.Alpha
    movd      [edi], xmm0
    mov       [edi].TARGBQuad.Alpha, al
@@next:
    add       edi, 4
    dec       width
    jnz       @@xLoop
    add       edi, datOffset
    pop       width
    dec       height
    jnz       @@yLoop
    pop       ebx
    pop       edi
    pop       esi
end;
 
procedure ImageHSBAdjustment(var Data: TImageData; hValue, sValue, bValue: Integer);
var
  BrightTab: TGrayTable;
begin
  if hValue > 180 then hValue := 180
  else if hValue < -180 then hValue := -180;
  if sValue > 255 then sValue := 255
  else if sValue < -255 then sValue := -255;
  if bValue <> 0 then GetBrightTable(bValue, BrightTab);
  if (sValue > 0) and (bValue <> 0) then
    HSBSetBright(Data, BrightTab);
  if (hValue <> 0) or (sValue <> 0) then
  begin
    HSBSetHueAndSaturation(Data, hValue, sValue);
  end;
  if (sValue <= 0) and (bValue <> 0) then
    HSBSetBright(Data, BrightTab);
end;
```

​    从上面的代码结构看，色相、饱和度和明度三者基本“独立”了，明度不用说，早独立了，色相和饱和度之间共享的部分仅仅是像素RGB值的最大值和差值（HSBSetHueAndSaturation过程中的ecx和xmm3）。经过简单测试，改进的代码速度提升不少，色相、饱和度和明度三者全部调整的时间只相当于**《**[**Delphi图像处理 -- 色相/饱和度调整**](http://blog.csdn.net/maozefa/article/details/4779033)**》**中的色相部分的调整时间。

​    最后鄙视一下CSDN，近来一直整理修改以前的文章，每一篇文章都重新加了4 - 5个标签，可是没过几天，文章的标签没了，我以为是我没设置好，又重新加了一遍，几天后，又没了！不带这么玩人的，再次鄙视CSDN。

 

​    **《Delphi图像处理》系列使用GDI+单元下载地址和说明见文章《**[**GDI+ for VCL基础 -- GDI+ 与 VCL**](http://blog.csdn.net/maozefa/article/details/1880484)**》。**

​    **因水平有限，错误在所难免，欢迎指正和指导。邮箱地址：**[**maozefa@hotmail.com**](mailto:maozefa@hotmail.com)

​    **这里可访问《Delphi图像处理 -- 文章索引》。**



 