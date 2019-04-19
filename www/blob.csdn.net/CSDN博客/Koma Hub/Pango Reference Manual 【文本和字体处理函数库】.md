# Pango Reference Manual 【文本和字体处理函数库】 - Koma Hub - CSDN博客
2019年03月02日 17:02:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：80
Pango 是 GNOME 应用程序中使用的核心文本和字体处理函数库。它对全球使用的不同书写系统有着广泛支持。
**Table of Contents**
[Basic Pango Interfaces](#Basic%20Pango%20Interfaces)
[Rendering with Pango](#Rendering%20with%20Pango)
[Low Level Functionality](#Low%20Level%20Functionality)
[https://www.pango.org/](https://www.pango.org/)
- [Introduction](https://www.pango.org/Introduction)
Pango is a library for laying out and rendering of text, with an emphasis on internationalization. Pango can be used anywhere that text layout is needed, though most of the work on Pango so far has been done in the context of the GTK+ widget toolkit. Pango forms the core of text and font handling for GTK+-2.x.
Pango is designed to be modular; the core Pango layout engine can be used with different font backends. There are three basic backends, with multiple options for rendering with each.
- 
Client side fonts using the FreeType and fontconfig libraries, using [HarfBuzz](https://www.pango.org/HarfBuzz) for complex-text handling. Rendering can be with with Cairo or Xft libraries, or directly to an in-memory buffer with no additional libraries.
- Native fonts on Microsoft Windows using Uniscribe for complex-text handling. Rendering can be done via Cairo or directly using the native Win32 API.
- 
Native fonts on MacOS X using [CoreText](https://www.pango.org/CoreText) for complex-text handling, rendering via Cairo.
The integration of Pango with Cairo ([http://cairographics.org/](http://cairographics.org/)) provides a complete solution with high quality text handling and graphics rendering.
Dynamically loaded modules then handle text layout for particular combinations of script and font backend. Pango ships with a wide selection of modules, including modules for Hebrew, Arabic, Hangul, Thai, and a number of Indic scripts. Virtually all of the world's major scripts are supported.
As well as the low level layout rendering routines, Pango includes PangoLayout, a high level driver for laying out entire blocks of text, and routines to assist in editing internationalized text.
Pango depends on 2.x series of the GLib library; more information about GLib can be found at [http://www.gtk.org/](http://www.gtk.org/).
If this information is insufficient, please see also [http://library.gnome.org/devel/pango/stable/](http://library.gnome.org/devel/pango/stable/)
[https://developer.gnome.org/pango/stable/](https://developer.gnome.org/pango/stable/)
### [Basic Pango Interfaces](https://developer.gnome.org/pango/stable/pango.html)
[Rendering](https://developer.gnome.org/pango/stable/pango-Text-Processing.html) — Functions to run the rendering pipeline
[Glyph Storage](https://developer.gnome.org/pango/stable/pango-Glyph-Storage.html) — Structures for storing information about glyphs
[Fonts](https://developer.gnome.org/pango/stable/pango-Fonts.html) — Structures representing abstract fonts
[Text Attributes](https://developer.gnome.org/pango/stable/pango-Text-Attributes.html) — Font and other attributes for annotating text
[Tab Stops](https://developer.gnome.org/pango/stable/pango-Tab-Stops.html) — Structures for storing tab stops
[Text Attribute Markup](https://developer.gnome.org/pango/stable/PangoMarkupFormat.html) — Simple markup language to encode text with attributes
[Layout Objects](https://developer.gnome.org/pango/stable/pango-Layout-Objects.html) — High-level layout driver objects
[Scripts and Languages](https://developer.gnome.org/pango/stable/pango-Scripts-and-Languages.html) — Identifying writing systems and languages
[Bidirectional Text](https://developer.gnome.org/pango/stable/pango-Bidirectional-Text.html) — Types and functions to help with handling bidirectional text
[Vertical Text](https://developer.gnome.org/pango/stable/pango-Vertical-Text.html) — Laying text out in vertical directions
### [Rendering with Pango](https://developer.gnome.org/pango/stable/rendering.html)
[Win32 Fonts and Rendering](https://developer.gnome.org/pango/stable/pango-Win32-Fonts-and-Rendering.html) — Functions for shape engines to manipulate Win32 fonts
[FreeType Fonts and Rendering](https://developer.gnome.org/pango/stable/pango-FreeType-Fonts-and-Rendering.html) — Functions for shape engines to manipulate FreeType fonts
[Xft Fonts and Rendering](https://developer.gnome.org/pango/stable/pango-Xft-Fonts-and-Rendering.html) — Font handling and rendering with the Xft backend
[Cairo Rendering](https://developer.gnome.org/pango/stable/pango-Cairo-Rendering.html) — Rendering with the Cairo backend
[CoreText Fonts](https://developer.gnome.org/pango/stable/pango-CoreText-Fonts.html) — Font handling with CoreText fonts
### [Low Level Functionality](https://developer.gnome.org/pango/stable/lowlevel.html)
[PangoRenderer](https://developer.gnome.org/pango/stable/PangoRenderer.html) — Rendering driver base class
[PangoFcFontMap](https://developer.gnome.org/pango/stable/PangoFcFontMap.html) — Base fontmap class for Fontconfig-based backends
[PangoFcFont](https://developer.gnome.org/pango/stable/PangoFcFont.html) — Base font class for Fontconfig-based backends
[PangoFcDecoder](https://developer.gnome.org/pango/stable/PangoFcDecoder.html) — Custom font encoding handling
[OpenType Font Handling](https://developer.gnome.org/pango/stable/pango-OpenType-Font-Handling.html) — Obtaining information from OpenType tables
[Coverage Maps](https://developer.gnome.org/pango/stable/pango-Coverage-Maps.html) — Unicode character range coverage storage
[Engines](https://developer.gnome.org/pango/stable/pango-Engines.html) — Language-specific and rendering-system-specific processing
[PangoEngineLang](https://developer.gnome.org/pango/stable/PangoEngineLang.html) — Rendering-system independent script engines
[PangoEngineShape](https://developer.gnome.org/pango/stable/PangoEngineShape.html) — Rendering-system dependent script engines
[Modules](https://developer.gnome.org/pango/stable/pango-Modules.html) — Support for loadable modules
[Miscellaneous Utilities](https://developer.gnome.org/pango/stable/pango-Miscellaneous-Utilities.html) — Various convenience and utility functions
[Version Checking](https://developer.gnome.org/pango/stable/pango-Version-Checking.html) — Tools for checking Pango version at compile- and run-time.
[Object Hierarchy](https://developer.gnome.org/pango/stable/pango-hierarchy.html)
[Annotation Glossary](https://developer.gnome.org/pango/stable/annotation-glossary.html)
[Index of all symbols](https://developer.gnome.org/pango/stable/api-index-full.html)
[Index of deprecated symbols](https://developer.gnome.org/pango/stable/api-index-deprecated.html)
[Index of new symbols in 1.2](https://developer.gnome.org/pango/stable/api-index-1-2.html)
[Index of new symbols in 1.4](https://developer.gnome.org/pango/stable/api-index-1-4.html)
[Index of new symbols in 1.6](https://developer.gnome.org/pango/stable/api-index-1-6.html)
[Index of new symbols in 1.8](https://developer.gnome.org/pango/stable/api-index-1-8.html)
[Index of new symbols in 1.10](https://developer.gnome.org/pango/stable/api-index-1-10.html)
[Index of new symbols in 1.12](https://developer.gnome.org/pango/stable/api-index-1-12.html)
[Index of new symbols in 1.14](https://developer.gnome.org/pango/stable/api-index-1-14.html)
[Index of new symbols in 1.16](https://developer.gnome.org/pango/stable/api-index-1-16.html)
[Index of new symbols in 1.18](https://developer.gnome.org/pango/stable/api-index-1-18.html)
[Index of new symbols in 1.20](https://developer.gnome.org/pango/stable/api-index-1-20.html)
[Index of new symbols in 1.22](https://developer.gnome.org/pango/stable/api-index-1-22.html)
[Index of new symbols in 1.24](https://developer.gnome.org/pango/stable/api-index-1-24.html)
[Index of new symbols in 1.26](https://developer.gnome.org/pango/stable/api-index-1-26.html)
[Index of new symbols in 1.30](https://developer.gnome.org/pango/stable/api-index-1-30.html)
[Index of new symbols in 1.31](https://developer.gnome.org/pango/stable/api-index-1-31-0.html)
[Index of new symbols in 1.32](https://developer.gnome.org/pango/stable/api-index-1-32.html)
[Index of new symbols in 1.32.4](https://developer.gnome.org/pango/stable/api-index-1-32-4.html)
[Index of new symbols in 1.34](https://developer.gnome.org/pango/stable/api-index-1-34.html)
[Index of new symbols in 1.38](https://developer.gnome.org/pango/stable/api-index-1-38.html)
[Index of new symbols in 1.40](https://developer.gnome.org/pango/stable/api-index-1-40.html)
Generated by GTK-Doc V1.25.1
