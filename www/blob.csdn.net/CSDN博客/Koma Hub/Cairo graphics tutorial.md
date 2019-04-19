# Cairo graphics tutorial - Koma Hub - CSDN博客
2019年01月15日 22:11:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：30
个人分类：[GTK																[GUI																[Cairo](https://blog.csdn.net/Rong_Toa/article/category/8614705)](https://blog.csdn.net/Rong_Toa/article/category/7517387)](https://blog.csdn.net/Rong_Toa/article/category/7156203)
[http://zetcode.com/gfx/cairo/](http://zetcode.com/gfx/cairo/)
This is Cairo graphics tutorial. The tutorial will teach you the basics of graphics programming in Cairo with the C programming language. This tutorial is for beginners and intermediate developers.
## Table of contents
- [Introduction](http://zetcode.com/gfx/cairo/cairolib/)
- [Cairo definitions](http://zetcode.com/gfx/cairo/cairodefinitions/)
- [Cairo backends](http://zetcode.com/gfx/cairo/cairobackends/)
- [Basic drawing](http://zetcode.com/gfx/cairo/basicdrawing/)
- [Shapes and fills](http://zetcode.com/gfx/cairo/shapesfills/)
- [Gradients](http://zetcode.com/gfx/cairo/gradients/)
- [Transparency](http://zetcode.com/gfx/cairo/transparency/)
- [Compositing](http://zetcode.com/gfx/cairo/compositing/)
- [Clipping and masking](http://zetcode.com/gfx/cairo/clippingmasking/)
- [Transformations](http://zetcode.com/gfx/cairo/transformations/)
- [Text](http://zetcode.com/gfx/cairo/cairotext/)
- [Images in Cairo](http://zetcode.com/gfx/cairo/cairoimages/)
- [Root window](http://zetcode.com/gfx/cairo/root/)
## Cairo
Cairo is a library for creating 2D vector graphics. It is written in the C programming language. There are bindings for other computer languages. Python, Perl, C++, C# or Java. Cairo is a multiplatform library. It works on Linux, BSDs, OSX.
[Tweet](https://twitter.com/share)
## Related tutorials
[PyCairo tutorial](http://zetcode.com/gfx/pycairo/) presents the Cairo library with the Python language. You may also look at the[GTK+ tutorial](http://zetcode.com/gui/gtk2/).
# Cairo: A Vector Graphics Library
for Cairo 1.16.0
[Drawing](https://www.cairographics.org/manual/cairo-drawing.html)
[cairo_t](https://www.cairographics.org/manual/cairo-cairo-t.html) — The cairo drawing context
[Paths](https://www.cairographics.org/manual/cairo-Paths.html) — Creating paths and manipulating path data
[cairo_pattern_t](https://www.cairographics.org/manual/cairo-cairo-pattern-t.html) — Sources for drawing
[Regions](https://www.cairographics.org/manual/cairo-Regions.html) — Representing a pixel-aligned area
[Transformations](https://www.cairographics.org/manual/cairo-Transformations.html) — Manipulating the current transformation matrix
[text](https://www.cairographics.org/manual/cairo-text.html) — Rendering text and glyphs
[Raster Sources](https://www.cairographics.org/manual/cairo-Raster-Sources.html) — Supplying arbitrary image data
[Tags and Links](https://www.cairographics.org/manual/cairo-Tags-and-Links.html) — Hyperlinks and document structure
[Fonts](https://www.cairographics.org/manual/cairo-fonts.html)
[cairo_font_face_t](https://www.cairographics.org/manual/cairo-cairo-font-face-t.html) — Base class for font faces
[cairo_scaled_font_t](https://www.cairographics.org/manual/cairo-cairo-scaled-font-t.html) — Font face at particular size and options
[cairo_font_options_t](https://www.cairographics.org/manual/cairo-cairo-font-options-t.html) — How a font should be rendered
[FreeType Fonts](https://www.cairographics.org/manual/cairo-FreeType-Fonts.html) — Font support for FreeType
[Win32 Fonts](https://www.cairographics.org/manual/cairo-Win32-Fonts.html) — Font support for Microsoft Windows
[Quartz (CGFont) Fonts](https://www.cairographics.org/manual/cairo-Quartz-(CGFont)-Fonts.html) — Font support via CGFont on OS X
[User Fonts](https://www.cairographics.org/manual/cairo-User-Fonts.html) — Font support with font data provided by the user
[Surfaces](https://www.cairographics.org/manual/cairo-surfaces.html)
[cairo_device_t](https://www.cairographics.org/manual/cairo-cairo-device-t.html) — interface to underlying rendering system
[cairo_surface_t](https://www.cairographics.org/manual/cairo-cairo-surface-t.html) — Base class for surfaces
[Image Surfaces](https://www.cairographics.org/manual/cairo-Image-Surfaces.html) — Rendering to memory buffers
[PDF Surfaces](https://www.cairographics.org/manual/cairo-PDF-Surfaces.html) — Rendering PDF documents
[PNG Support](https://www.cairographics.org/manual/cairo-PNG-Support.html) — Reading and writing PNG images
[PostScript Surfaces](https://www.cairographics.org/manual/cairo-PostScript-Surfaces.html) — Rendering PostScript documents
[Recording Surfaces](https://www.cairographics.org/manual/cairo-Recording-Surfaces.html) — Records all drawing operations
[Win32 Surfaces](https://www.cairographics.org/manual/cairo-Win32-Surfaces.html) — Microsoft Windows surface support
[SVG Surfaces](https://www.cairographics.org/manual/cairo-SVG-Surfaces.html) — Rendering SVG documents
[Quartz Surfaces](https://www.cairographics.org/manual/cairo-Quartz-Surfaces.html) — Rendering to Quartz surfaces
[XCB Surfaces](https://www.cairographics.org/manual/cairo-XCB-Surfaces.html) — X Window System rendering using the XCB library
[XLib Surfaces](https://www.cairographics.org/manual/cairo-XLib-Surfaces.html) — X Window System rendering using XLib
[XLib-XRender Backend](https://www.cairographics.org/manual/cairo-XLib-XRender-Backend.html) — X Window System rendering using XLib and the X Render extension
[Script Surfaces](https://www.cairographics.org/manual/cairo-Script-Surfaces.html) — Rendering to replayable scripts
[Utilities](https://www.cairographics.org/manual/cairo-support.html)
[cairo_matrix_t](https://www.cairographics.org/manual/cairo-cairo-matrix-t.html) — Generic matrix operations
[Error handling](https://www.cairographics.org/manual/cairo-Error-handling.html) — Decoding cairo's status
[Version Information](https://www.cairographics.org/manual/cairo-Version-Information.html) — Compile-time and run-time version checks.
[Types](https://www.cairographics.org/manual/cairo-Types.html) — Generic data types
[Index](https://www.cairographics.org/manual/index-all.html)
[Index of new symbols in 1.2](https://www.cairographics.org/manual/index-1.2.html)
[Index of new symbols in 1.4](https://www.cairographics.org/manual/index-1.4.html)
[Index of new symbols in 1.6](https://www.cairographics.org/manual/index-1.6.html)
[Index of new symbols in 1.8](https://www.cairographics.org/manual/index-1.8.html)
[Index of new symbols in 1.10](https://www.cairographics.org/manual/index-1.10.html)
[Index of new symbols in 1.12](https://www.cairographics.org/manual/index-1.12.html)
[Index of new symbols in 1.14](https://www.cairographics.org/manual/index-1.14.html)
[Index of new symbols in 1.16](https://www.cairographics.org/manual/index-1.16.html)
[A. Creating a language binding for cairo](https://www.cairographics.org/manual/language-bindings.html)
[General considerations](https://www.cairographics.org/manual/language-bindings.html#bindings-general)
[Memory management](https://www.cairographics.org/manual/bindings-memory.html)
[Multiple return values](https://www.cairographics.org/manual/bindings-return-values.html)
[Overloading and optional arguments](https://www.cairographics.org/manual/bindings-overloading.html)
[Streams and File I/O](https://www.cairographics.org/manual/bindings-streams.html)
[Error handling](https://www.cairographics.org/manual/bindings-errors.html)
[Patterns](https://www.cairographics.org/manual/bindings-patterns.html)
[Surfaces](https://www.cairographics.org/manual/bindings-surfaces.html)
[Fonts](https://www.cairographics.org/manual/bindings-fonts.html)
[cairo_path_t](https://www.cairographics.org/manual/bindings-path.html)

