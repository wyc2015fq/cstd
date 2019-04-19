# Really Fast Software Blending 混合 - xqhrs232的专栏 - CSDN博客
2012年11月01日 21:35:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：802
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://www.starming.com/index.php?action=plugin&v=wave&tpl=union&ac=viewgrouppost&gid=56&tid=4429](http://www.starming.com/index.php?action=plugin&v=wave&tpl=union&ac=viewgrouppost&gid=56&tid=4429)
相关网帖
1.32bit Bitmap and AlphaBlend----[http://forums.codeguru.com/showthread.php?327701-32bit-Bitmap-and-AlphaBlend](http://forums.codeguru.com/showthread.php?327701-32bit-Bitmap-and-AlphaBlend)
2.A very basic alpha blend algorithm----[http://www.codeproject.com/Tips/179710/A-very-basic-alpha-blend-algorithm](http://www.codeproject.com/Tips/179710/A-very-basic-alpha-blend-algorithm)
3.[How
 to do alpha blend fast?](http://stackoverflow.com/questions/1102692/how-to-do-alpha-blend-fast)----[http://stackoverflow.com/questions/1102692/how-to-do-alpha-blend-fast](http://stackoverflow.com/questions/1102692/how-to-do-alpha-blend-fast)
# Concept
Alpha blending is slow. It always involves lots of multiplication, or division. No way around that. Either you use lookup tables (and array accessing requires multiplication), or you use multiplication, or you use division. You can get a good approximation
 of alpha blending with bit-shifting, but that抯 just an approximation.
What I set out to do was to create as fast an alpha blending formula as possible, without wasting lots of memory on lookup tables, or lots of clock cycles on math. And as far as I can tell, I did it.
# Benchmarks
Alright, before I waste your time with my code, here抯 some proof that my method is fast:
![](http://t.cyol.com/cache/temp/1000/56/img/wave/2010/03/img_1269937417_0.png)
Note the horrible floating point performance of the P2-350. I wrote two other versions of my alpha blending algorithm, one using a << 1 bitshift, and one using a * float multiply. All used the same skeleton as my enhanced blend routines. The tests were conducted
 using a small optimized VB application, and each test involved blending 10 million (that抯 right, 10 million) pairs of 32-bit color values. Each test used a different set of values. The additive blend is faster than my alpha blend, as demonstrated by the benchmark
 � it only has to lookup 3 values instead of 6. I抳e provided the code used for each of the three alpha blend test functions, so you can see that I attempted to optimize all three.
# Code
> newcolor.channels.red = clipByte((source.channels.red+dest.channels.red) >> 1);
newcolor.channels.green = clipByte((source.channels.green+dest.channels.green) >> 1);
newcolor.channels.blue = clipByte((source.channels.blue+dest.channels.blue) >> 1);
float alphaS, alphaD;
alphaS = (float)alpha / (float)255; alphaD = 1 - alphaS;
newcolor.channels.red = clipByte((source.channels.red*alphaS)+(dest.channels.red*alphaD));
newcolor.channels.green = clipByte((source.channels.green*alphaS)+(dest.channels.green*alphaD));
newcolor.channels.blue = clipByte((source.channels.blue*alphaS)+(dest.channels.blue*alphaD));
f2level * sourceTable; f2level * destTable;
sourceTable = aTable(alpha); destTable = aTable(255 - alpha);
newcolor.channels.red = clipByte(sourceTable->values[source.channels.red] +
                        destTable->values[dest.channels.red]);
newcolor.channels.green = clipByte(sourceTable->values[source.channels.green] +
                          destTable->values[dest.channels.green]);
newcolor.channels.blue = clipByte(sourceTable->values[source.channels.blue] +
                         destTable->values[dest.channels.blue]);
# How it works
I used lookup tables for this formula, but I used them in a way which I抳e never seen used before. I created a standard 256*256 element __int8 (Byte for you VB people) lookup table, but I created it in a way as to allow you to create a pointer to each group
 of 256 elements� this will probably make more sense later. I also came up with an extremely fast way of eliminating a large number of branching instructions in my code (I checked the assembly myself) by using lots of tricky boolean operations.
Let抯 get to some code.
> struct LookupLevel {
  unsigned __int8 Values[256];
};
struct LookupTable {
  LookupLevel Levels[256];
} AlphaTable;
__inline unsigned __int8 clipByte(int value) {
  value = (0 & (-(int)(value < 0))) | (value & (-(int)!(value < 0)));
  value = (255 & (-(int)(value > 255))) | (value & (-(int)!(value > 255)));
  return value;
}
int InitTable() {
float fValue,
fAlpha;
int iValue, iAlpha;
  for (iAlpha = 0; iAlpha < 256; iAlpha++) {
    fAlpha = ((float)iAlpha) / 255;
    for (iValue = 0; iValue < 256; iValue++) {
      fValue = ((float)iValue) / 255;
      AlphaTable.Levels[iAlpha].Values[iValue] = clipByte((int)((fValue * fAlpha) * 255));
    }
  }
  return true;
}
LookupLevel * LookupPointer(int alpha) {
  return &AlphaTable.Levels[clipByte(alpha)];
}
# Explanation
Well, now that we have that (very small) piece of example code, I抦 going to explain it in detail.
The two structs are layered in a way to allow you to easily retrieve a pointer to a single 256 element array, for a single alpha value. This is very efficient for static-alpha blits, in which the alpha values for the image are the same the entire time. Blitting
 images with alpha channels is somewhat slower, but still very fast. I抳e been using this technique for months, and have done some extensive optimization.
Now, first, we start with the clipByte function. That function uses some cryptic-looking bit manipulation to clip a value to a byte. When optimized by Visual C++ 6.0抯 compiler, it results in exactly 15 assembly instructions, none of which involve any sort of
 branching (or cache-thrashing). A variation of this can also be used for masking and other techniques. Here抯 what the assembly for clipByte looks like:
> mov    ecx, DWORD PTR _value$[esp-4]
xor    eax, eax
test   ecx, ecx
setge  al
neg    eax
and    ecx, eax
cmp    ecx, 255
setle  al
neg    al
and    al, cl
cmp    ecx, 255
setg   cl
neg    cl
or     al, cl
ret    4
Pretty small, isn抰 it? It抯 fast, too.
Now, onto the InitTable function. Please excuse my horrible naming convention, I抦 one of the world抯 worst coders. InitTable is pretty simple, it calculates a value from 0-255 for each element of the lookup table. It抯 moderately fast, and probably could be made
 faster if I ever needed to speed it up. (It currently takes my P3-750 between 0 and 10 milliseconds to run the initialization routine. More than fast enough for me.)
Alright, hopefully you understand it now. Let抯 move on to the actual use of the lookup tables: Alpha blending.
I抦 not going to demonstrate an alpha blended blit routine here, because it would take far too much time. Perhaps in another article. For now I抦 going to demonstrate 3 kinds of color blending. Note that I use the 32-bit BGRA colorspace in all my code, but it
 is easy to adapt it to other colorspaces.
> struct ColorChannels
{
  unsigned __int8 Blue;
  unsigned __int8 Green;
  unsigned __int8 Red;
  unsigned __int8 Alpha;
};
union Pixel32 {
  ColorChannels Channels;
  __int32 Value;
};
__int32 AlphaBlend(Pixel32 DestColor, Pixel32 SourceColor, __int32 Alpha) {
Pixel32 NewColor;
LookupLevel *SourceTable;
LookupLevel *DestTable;
  SourceTable = LookupPointer(Alpha);
  DestTable = LookupPointer(255 - Alpha);
  // The clipByte's aren't really necessary, but they're there to remove the
  // chance for an occasional overflow. They only eat a couple clock cycles anyway.
  NewColor.Channels.Red = clipByte(SourceTable->Values[SourceColor.Channels.Red] +
                          DestTable->Values[DestColor.Channels.Red]);
  NewColor.Channels.Green = clipByte(SourceTable->Values[SourceColor.Channels.Green] +
                            DestTable->Values[DestColor.Channels.Green]);
  NewColor.Channels.Blue = clipByte(SourceTable->Values[SourceColor.Channels.Blue] +
                           DestTable->Values[DestColor.Channels.Blue]);
  return NewColor.Value;
}
# Explanation
Alright, there we have it. A fully functional alpha blending routine that will blend two colors. Pretty small, isn抰 it? Now, here抯 how it works:
It gets two pointers: One for the destination pixels, one for the source pixels. Each is a pointer to a 256-element __int8 (Byte for you VB people) lookup table. Then, it uses those tables to blend each color channel of the source and destination pixels. It抯
 quite fast, even in its unoptimized state. I have the clipByte() around the values, just in case one of the calculations comes out to 256 or something. You could remove the clipByte and probably not have any problems, but it抯 there anyway.
When writing a blitter, I suggest doing the calculations inside the blitter itself. It would be much faster, as you抎 only have to get those two pointers once per blit, and you could avoid the whole temporary color variable and save your results directly to
 the destination pixel.
Note that if you want to do this with 15-bit or 16-bit colors, you won抰 be able to use a pixel struct, but the lookup table will work just fine with absolutely no modification whatsoever, as long as your alpha values are 0-255. You could also optimize it a
 bit more by removing the clipByte from the LookupPointer function, as well.
Now that you hopefully understand the alpha blend function, I抦 going to show you an additive blend function, and a subtractive blend function. Quite handy functions to have.
> __int32 SubtractiveBlend(Pixel32 DestColor, Pixel32 SourceColor, __int32 Alpha) {
Pixel32 NewColor;
LookupLevel *
SourceTable;
  SourceTable = LookupPointer(Alpha);
  // These clipByte's ARE necessary. Don't remove them.
  NewColor.Channels.Red = clipByte(DestColor.Channels.Red -
                          SourceTable->Values[SourceColor.Channels.Red]);
  NewColor.Channels.Green = clipByte(DestColor.Channels.Green -
                            SourceTable->Values[SourceColor.Channels.Green]);
  NewColor.Channels.Blue = clipByte(DestColor.Channels.Blue -
                           SourceTable->Values[SourceColor.Channels.Blue]);
  return NewColor.Value;
}
__int32 AdditiveBlend(Pixel32 DestColor, Pixel32 SourceColor, __int32 Alpha) {
Pixel32 NewColor;
LookupLevel *
SourceTable;
  SourceTable = LookupPointer(Alpha);
  // These clipByte's ARE necessary. Don't remove them.
  NewColor.Channels.Red = clipByte(SourceTable->Values[SourceColor.Channels.Red] +
                          DestColor.Channels.Red);
  NewColor.Channels.Green = clipByte(SourceTable->Values[SourceColor.Channels.Green] +
                            DestColor.Channels.Green);
  NewColor.Channels.Blue = clipByte(SourceTable->Values[SourceColor.Channels.Blue] +
                           DestColor.Channels.Blue);
  return NewColor.Value;
}
# Explanation
Alright, there you have it. Additive Blend and Subtractive Blend functions. They抮e actually faster than the alpha blend function, believe it or not. You COULD remove the clipByte call, but then you抎 have tons of overflow problems, so just leave it there. I
 may have screwed up the algorithm a bit, as these aren抰 the actual functions I use, I rewrote these from scratch.
Hopefully this article has shown you a few ways to optimize your code, and maybe even given you insipriation for how to optimize one of your graphics routines. If you like the article, have any questions, or would like to point something out to me, feel free
 to email me at [janusfury@citlink.net](mailto:janusfury@citlink.net).
Also, I might be writing another article soon on blitters and more graphics operations, if I get enough response to this article.
//===================================
备注::
1.DirectDraw does not have native alphablending, only colorkeying
2.
You can always calculate the alpha of red and blue at the same time. You can also use this trick with the SIMD implementation mentioned before.
```
int colora = 0xFFFFFF; // a color
int colorb = 0xFF007F; // other color
int rb = (colora & 0xFF00FF) + (alpha * (colorb & 0xFF00FF));
int g = (colora & 0x00FF00) + (alpha * (colorb & 0x00FF00));
return (rb & 0xFF00FF) + (g & 0x00FF00);
```
3.
