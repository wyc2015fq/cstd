# Skywind Inside » 超越 SDL/DirectDraw/GDI 性能的位图库
## 超越 SDL/DirectDraw/GDI 性能的位图库
April 29th, 2016[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
开源一个高性能位图库，之前对我的二维图形库 [pixellib](https://github.com/skywind3000/pixellib) 进行了精简和重写，最终形成一个只包含两个文件（BasicBitmap.h, BasicBitmap.cpp）的图形基础库。
在今天 GPU 绘制横行天下的时候，任然有很多时候需要使用到纯 CPU实现的图形库，比如图像处理，视频预处理与合成，界面，以及GPU无法使用的情况（比如某个应用把gpu占满了，或者无法通过gpu做一些十分灵活的事情时），纹理处理，简单图片加载保存等。
支持 SSE2/AVX 优化，比 DirectDraw 快 40%（全系统内存绘制），比 SDL 快 10%，比GDI快 38%。如果你需要一个方便的高性能位图库，足够高性能的同时保证足够紧凑。
如果你有上述需求，那么你和我一样需要用到 BasicBitmap，只需要把 BasicBitmap.h/.cpp 两个文件拷贝到你的代码中即可。我正是为了这个目的编写了这两个文件。
# 特性介绍
- 高度优化的 C++ 代码，可以在任意平台编译并运行
- 多重像素格式，从8位到32位：A8R8G8B8, R8G8B8, A4R4G4B4, R5G6B5, A8, 等.
- Blit (Bit Blt) ，包含透明和非透明的模式。
- 像素格式快速转换
- 使用不同的 Compositor 进行 Blending
- 使用不同的过滤器进行缩放（nearest, linear, bilinear）
- 高质量位图重采样（Bicubic/Box）
- 支持从内存或者文件直接读取 BMP/TGA 文件
- 支持从内存或者文件直接读取 PNG/JPEG 文件（Windows下）
- 保存图片为 BMP/PPM 文件
- 核心绘制函数可以被外部实现通过设置函数指针重载（比如 SSE2实现）
- 比 DirectDraw 快 40% 的性能进行绘制（打开 AVX/SSE2支持）
- 比 GDI 的 AlphaBlend 函数快34%的性能进行混色
- Self-contained, 不依赖任何其他第三方库
- 高度紧凑，只需要拷贝 BasicBitmap.h/.cpp 两个文件到你项目即可
# 项目地址
- [https://github.com/skywind3000/BasicBitmap](https://github.com/skywind3000/BasicBitmap)
# Blit 性能比较
Full window (800×600) blitting (both opacity and transparent), compare to GDI/SDL/DirectDraw:
|32 Bits Blit|Opacity|Transparent|
|----|----|----|
|BasicBitmap C++|fps=2325|fps=1368|
|BasicBitmap AVX/SSE2|fps=2904|fps=2531|
|GDI|fps=2333|fps=1167|
|SDL|fps=2671|fps=1015|
|DirectDraw|fps=2695|fps=2090|
Note: use BltFast with DirectDrawSurface7 in System Memory to perform Opacity & Transparent blit. BitBlt and TransparentBlt(msimg32.dll) are used in the GDI testing case.
|16 Bits Blit|Opacity|Transparent|
|----|----|----|
|BasicBitmap C++|fps=4494|fps=1253|
|BasicBitmap AVX/SSE2|fps=9852|fps=2909|
|DirectDraw BltFast|fps=5889|fps=861|
Blitting performance in SDL & GDI are slower than DirectDraw, just compare to ddraw as well.
|8 Bits Blit|Opacity|Transparent|
|----|----|----|
|BasicBitmap C++|fps=11142|fps=1503|
|BasicBitmap AVX/SSE2|fps=18181|fps=5449|
|DirectDraw BltFast|fps=14705|fps=4832|
DirectDrawSurface in *Video Memory* takes the benefit of hardware acceleration which is definitely faster than BasicBitmap. If you really need hardware acceleration, use OpenGL/DX as well.
*BasicBitmap* is a software implementation which aims to achieve the best performance in all other software implementations: like GDI/GDI+, SDL/DirectDraw in System Memory, for examples.
So just compare to DirectDrawSurface in *System Memory*. Use it in the condition that you only need a lightweight software solution: GUI/Cross Platform/hardware unavailable/image processing/video compositing, etc.
# 混色性能比较
|SRC OVER|FPS|
|----|----|
|BasicBitmap C++|594|
|BasicBitmap SSE2|1731|
|GDI (msimg32.dll)|1137|
note: 800×600 full window src-over blending vs GDI’s AlphaBlend function (in msimg32.dll).
