

#include <algorithm>
#if defined(NANA_WINDOWS)
	#include <windows.h>
#elif defined(NANA_X11)
	#include <X11/Xlib.h>
#endif

#include "../basic_types.hpp"

struct font
{
private:
		path_type	const ttf_;
    string	const family_;
    double		const size_;
    font_style	const style_;
		native_font_type native_handle_;

public:
  
		font(const char* ttf, const char* font_family, double font_size, const font_style& fs, native_font_type native_font):
			ttf_(ttf),
			family_(font_family),
			size_(font_size),
			style_(fs),
			native_handle_(native_font)
		{}

		~font()
		{
#ifdef NANA_WINDOWS
			::DeleteObject(reinterpret_cast<HFONT>(native_handle_));
#elif defined(NANA_X11)
			auto disp = platform_spec::instance().open_display();
#	ifdef NANA_USE_XFT
			::XftFontClose(disp, reinterpret_cast<XftFont*>(native_handle_));
#	else
			::XFreeFontSet(disp, reinterpret_cast<XFontSet>(native_handle_));
#	endif
#endif
			if (!ttf_.empty())
				platform_abstraction::font_resource(false, ttf_);
		}
	public:
		const char* family() const
		{
			return family_;
		}

		double size() const
		{
			return size_;
		}

		const font_style & style() const
		{
			return style_;
		}

		native_font_type native_handle() const
		{
			return native_handle_;
		}
		font::font()
			: impl_(new impl_type)
		{
			impl_->real_font = default_font(NULL);
		}

		font::font(drawable_type dw)
			: impl_(new impl_type)
		{
			impl_->real_font = dw->font;
		}

		font::font(const font& rhs)
			: impl_(new impl_type)
		{
			if(rhs.impl_)
				impl_->real_font = rhs.impl_->real_font;
		}

		font::font(const char* font_family, double size_pt, const font_style& fs = font_style())
		{
			impl_->real_font = platform_abstraction::make_font(font_family, size_pt, fs);
		}


		font::font(double size_pt, const char* truetype, const font_style& fs = font_style()) 
		{
			impl_->real_font = platform_abstraction::make_font_from_ttf(truetype, size_pt, fs);
		}

		font::~font()
		{
			delete impl_;
		}

		bool font::empty() const
		{
			return ((NULL == impl_) || (NULL == impl_->real_font));
		}

		void font::set_default() const
		{
			if(empty())
				return;

			platform_abstraction::default_font(impl_->real_font);
		}

		string font::name() const
		{
			if(empty()) return string();

			return impl_->real_font->family();
		}

		double font::size() const
		{
			if(empty()) return 0;

			return impl_->real_font->size();
		}

		bool font::bold() const
		{
			if(empty()) return false;
			return (impl_->real_font->style().weight >= 700);
		}

		unsigned font::weight() const
		{
			if(empty()) return 0;
			return impl_->real_font->style().weight;
		}

		bool font::italic() const
		{
			if(empty()) return false;
			return impl_->real_font->style().italic;
		}

		bool font::underline() const
		{
			if(empty()) return false;
			return impl_->real_font->style().underline;
		}

		bool font::strikeout() const
		{
			if(empty()) return false;
			return impl_->real_font->style().strike_out;
		}

		native_font_type font::handle() const
		{
			if(empty())	return NULL;
			return impl_->real_font->native_handle();
		}

		void font::release()
		{
			if(impl_)
				impl_->real_font.reset();
		}

		font & font::operator=(const font& rhs)
		{
			if(impl_ && rhs.impl_ && (this != &rhs))
				impl_->real_font = rhs.impl_->real_font;

			return *this;
		}

		bool font::operator==(const font& rhs) const
		{
			if(empty() == rhs.empty())
				return (empty() || (impl_->real_font == rhs.impl_->real_font));

			return false;
		}

		bool font::operator!=(const font& rhs) const
		{
			if(empty() == rhs.empty())
				return ((empty() == false) && (impl_->real_font != rhs.impl_->real_font));

			return true;
		}
	//end class font

};

class font_interface
{
public:
  virtual ~font_interface() {}
  
	virtual const char* family() const = 0;
  virtual double size() const = 0;
  virtual const font_style & style() const = 0;
  virtual native_font_type native_handle() const = 0;
};

class platform_abstraction
{
public:
		typedef font_interface* font;
    
    static void initialize();
    /// Shutdown before destruction of platform_spec 
    static void shutdown();
    static font* default_font(const font* );
    static font* make_font(const char* font_family, double size_pt, const font_style& fs);
    static font* make_font_from_ttf(const char* ttf, double size_pt, const font_style& fs);
    static void font_resource(bool try_add, const char* ttf);
};


/// \brief off-screen resource defined as ref-counting, can refer one resource
///
/// Load a bitmap into a graphics:
/// \code
/// graphics graph;
/// image img("C:\\ABitmap.bmp");
/// img.paste(graph, 0, 0); //graph may create if it is empty
/// \endcode
class graphics
{
public:
  graphics();
  graphics(const size&);                 ///< size in pixel
  graphics(const graphics&);      ///< the resource is not copyed, the two graphics objects refer to the *SAME* resource
  graphics& operator=(const graphics&);
  
  ~graphics();
  
  bool changed() const;           ///< Returns true if the graphics object is operated
  bool empty() const;             ///< Returns true if the graphics object does not refer to any resource.
  operator bool() const noexcept;
  
  drawable_type handle() const;
  const void* pixmap() const;
  const void* context() const;
  
  void swap(graphics& other) noexcept;
  
  /// Creates a graphics/drawable resource
  /**
  * @param sz The dimension of the graphics to be requested. If sz is empty, it performs as release().
  */
  void make(const size& sz);
  void resize(const size&);
  void typeface(const font&);						///< Selects a specified font type into the graphics object.
  font typeface() const;
  size	text_extent_size(const char*) const;
  size	text_extent_size(const char*, size_t len) const;
  size	text_extent_size(const wchar*) const;    ///< Computes the width and height of the specified string of text.
  size	text_extent_size(const wchar*, size_t length) const;    ///< Computes the width and height of the specified string of text with the specified length.
  size	glyph_extent_size(const wchar*, size_t length, size_t begin, size_t end) const;
  bool glyph_pixels(const wchar *, size_t length, unsigned* pxbuf) const;
  size	bidi_extent_size(const wchar*) const;
  size	bidi_extent_size(const char*) const;
  
  bool text_metrics(unsigned & ascent, unsigned& descent, unsigned& internal_leading) const;
  
  void line_begin(int x, int y);
  
  void bitblt(int x, int y, const graphics& source);     ///<   Transfers the source to the specified point.
  void bitblt(const rectangle& r_dst, native_window_type src);  ///< Transfers the color data corresponding to r_dst from the src window to this graphics.
  void bitblt(const rectangle& r_dst, native_window_type src, const point& p_src);  ///< Transfers the color data corresponding to r_dst from the src window at point p_src to this graphics.
  void bitblt(const rectangle& r_dst, const graphics& src);     ///< Transfers the color data corresponding to r_dst from the src graphics to this graphics.
  void bitblt(const rectangle& r_dst, const graphics& src, const point& p_src);///< Transfers the color data corresponding to r_dst from the src graphics at point p_src to this graphics.
  
  void blend(const rectangle& r, const color&, double fade_rate);
  void blend(const rectangle& blend_r, const graphics& blend_graph, const point& blend_graph_point, double fade_rate);///< blends with the blend_graph.
  
  void blur(const rectangle& r, size_t radius);      ///< Blur process.
  
  void paste(graphics& dst, int x, int y) const;    ///< Paste the graphics object into the dest at (x, y)
  void paste(native_window_type dst, const rectangle&, int sx, int sy) const;  ///< Paste the graphics object into a platform-dependent window at (x, y)
  void paste(native_window_type dst, int dx, int dy, unsigned width, unsigned height, int sx, int sy) const;
  void paste(drawable_type dst, int x, int y) const;
  void paste(const rectangle& r_src, graphics& dst, int x, int y) const;
  void rgb_to_wb();   ///< Transform a color graphics into black&white.
  
  void stretch(const rectangle& src_r, graphics& dst, const rectangle& r) const;
  void stretch(graphics& dst, const rectangle&) const;
  
  void flush();
  
  unsigned width() const;		///< Returns the width of the off-screen buffer.
  unsigned height() const;	///< Returns the height of the off-screen buffer.
  size size() const;
  void setsta();				///< Clears the status if the graphics object had been changed
  void set_changed();
  void release();
  
  /// Saves images as a windows bitmap file
  /// @param file_utf8 A UTF-8 string to a filename
  void save_as_file(const char* file_utf8) const;
  
  color	palette(bool for_text) const;
  graphics&		palette(bool for_text, const color&);
  
  unsigned bidi_string(const point&, const wchar *, size_t len);
  unsigned bidi_string(const point& pos, const char*, size_t len);
  
  void set_pixel(int x, int y, const color&);
  void set_pixel(int x, int y);
  
  void string(const point&, const char* text_utf8);
  void string(const point&, const char* text_utf8, const color&);
  
  void string(point, const wchar*, size_t len);
  void string(const point&, const wchar*);
  void string(const point&, const wchar*, const color&);
  
  void line(const point&, const point&);
  void line(const point&, const point&, const color&);
  void line_to(const point&, const color&);
  void line_to(const point&);
  
  void rect(bool solid);
  void rect(bool solid, const color&);
  void rect(const rectangle& r, bool solid);
  void rect(const rectangle&, bool solid, const color&);
  void frame_rect(const rectangle&, const color& left, const color& top, const color& right, const color& bottom);
  void frame_rect(const rectangle&, const color&, unsigned gap);
  
  void gradual_rect(const rectangle&, const color& from, const color& to, bool vertical);
  void round_rect(const rectangle&, unsigned radius_x, unsigned radius_y, const color&, bool solid, const color& color_if_solid);
private:
  struct implementation;
  implementation* impl_;
};

class draw
{
public:
  draw(graphics& graph);
  
  void corner(const rectangle& r, unsigned pixels);
private:
  graphics& graph_;
};


	class pixel_buffer
	{
		struct pixel_buffer_storage;
		typedef bool (pixel_buffer:: * unspecified_bool_t)() const;
	public:
    pixel_buffer() {};
		pixel_buffer(drawable_type, const rectangle& want_rectangle);
		pixel_buffer(drawable_type, size_t top, size_t lines);
		pixel_buffer(size_t width, size_t height);

		~pixel_buffer();

		void attach(drawable_type, const rectangle& want_rectangle);

		bool open(drawable_type);
		bool open(drawable_type, const rectangle& want_rectangle);
		bool open(size_t width, size_t height);

		void alpha_channel(bool enabled);
		bool alpha_channel() const;

		void close();

		bool empty() const;

		operator unspecified_bool_t() const;

		size_t bytes() const;
		size_t bytes_per_line() const;
		size size() const;

		pixel_color_t * at(const point& pos) const;
		pixel_color_t * raw_ptr(size_t row) const;
		pixel_color_t * operator[](size_t row) const;

		void fill_row(size_t row, const unsigned char* buffer, size_t bytes, unsigned bits_per_pixel);

		void put(const unsigned char* rawbits, size_t width, size_t height, size_t bits_per_pixel, size_t bytes_per_line, bool is_negative);
		
		void line(const char* name);
		void line(const point& pos_beg, const point& pos_end, const color&, double fade_rate);

		void rect(const rectangle&, const color&, double fade_rate, bool solid);
		void gradual_rect(const rectangle&, const color& from, const color& to, double fade_rate, bool vertical);
		
		pixel_color_t pixel(int x, int y) const;
		void pixel(int x, int y, pixel_color_t);

		void paste(drawable_type, const point& p_dst) const;
		void paste(const rectangle& s_r, drawable_type, const point& p_dst) const;
		void paste(native_window_type, const point& p_dst) const;
		void stretch(const char* name);
		void stretch(const rectangle& s_r, drawable_type, const rectangle& r) const;
		void blend(const char* name);
		void blend(const rectangle& s_r, drawable_type dw_dst, const point& d_pos, double fade_rate) const;
		void blur(const rectangle& r, size_t radius);

		pixel_buffer rotate(double angle, const color& extend_color);
	private:
		pixel_buffer_storage* storage_;
	};
  

		font_style::font_style(unsigned weight, bool italic, bool underline, bool strike_out) :
			weight(weight),
			italic(italic),
			underline(underline),
			strike_out(strike_out)
		{}
	

		struct drawable_deleter
		{
			void operator()(const drawable_type p) const
			{
#if defined(NANA_WINDOWS)
				delete p;
#elif defined(NANA_X11)
				if(p)
				{
					Display* disp = reinterpret_cast<Display*>(platform_spec::instance().open_display());
	#if defined(NANA_USE_XFT)
					::XftDrawDestroy(p->xftdraw);
	#endif
					::XFreeGC(disp, p->context);
					::XFreePixmap(disp, p->pixmap);
					delete p;
				}
#endif
			}
		};
		//end struct graphics_handle_deleter
	

	//class graphics
		struct graphics::implementation
		{
			shared_ptr<drawable_impl_type> platform_drawable;
			font			font_shadow;
			drawable_type	handle{ NULL };
			size	size;
			pixel_buffer	pxbuf;
			bool changed{ false };
		};

		graphics::graphics()
			: impl_(new implementation)
		{}

		graphics::graphics(const size& sz)
			: impl_(new implementation)
		{
			make(sz);
		}

		graphics::graphics(const graphics& rhs)
			: impl_(new implementation(*rhs.impl_))
		{}

		graphics& graphics::operator=(const graphics& rhs)
		{
			if(this != &rhs)
			{
				*impl_ = *rhs.impl_;
				impl_->changed = true;
			}
			return *this;
		}


		graphics::graphics(graphics&& other)
			: impl_(move(other.impl_))
		{
			other.impl_.reset(new implementation);
		}

		graphics& graphics::operator=(graphics&& other)
		{
			if (this != &other)
			{
				impl_ = move(other.impl_);
				other.impl_.reset(new implementation);
			}

			return *this;
		}

		graphics::~graphics()
		{
			//For instance of unique_ptr pimpl
		}

		bool graphics::changed() const
		{
			return impl_->changed;
		}

		bool graphics::empty() const
		{
			return (!impl_->handle);
		}

		graphics::operator bool() const noexcept
		{
			return (impl_->handle != NULL);
		}

		drawable_type graphics::handle() const
		{
			return impl_->handle;
		}

		const void* graphics::pixmap() const
		{
			//The reinterpret_cast is used for same platform. Under Windows, the type
			//of pixmap can be conversed into void* directly, but under X11, the type is not a pointer.
			return (impl_->handle ? reinterpret_cast<void*>(impl_->handle->pixmap) : NULL);
		}

		const void* graphics::context() const
		{
			return (impl_->handle ? impl_->handle->context : NULL);
		}

		void graphics::swap(graphics& other) noexcept
		{
			if (context() != other.context())
				impl_.swap(other.impl_);
		}

		void graphics::make(const size& sz)
		{
			if (impl_->handle == NULL || impl_->size != sz)
			{
				if (sz.empty())
				{
					release();
					return;
				}

				//The object will be delete while dwptr_ is performing a release.
				shared_ptr<drawable_impl_type> dw{ new drawable_impl_type, drawable_deleter{} };

				//Reuse the old font
				if (impl_->platform_drawable)
				{
					drawable_type reuse = impl_->platform_drawable.get();
					dw->font = reuse->font;
					dw->string.tab_length = reuse->string.tab_length;
				}
				else
					dw->font = impl_->font_shadow.impl_->real_font;

#if defined(NANA_WINDOWS)
				HDC hdc = ::GetDC(NULL);
				HDC cdc = ::CreateCompatibleDC(hdc);
				if (NULL == cdc)
				{
					::ReleaseDC(NULL, hdc);
					throw bad_alloc{};
				}

				BITMAPINFO bmi;
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = sz.width;
				bmi.bmiHeader.biHeight = -static_cast<int>(sz.height);
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = (sz.width * sz.height) << 2;

				HBITMAP bmp = ::CreateDIBSection(cdc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&(dw->pixbuf_ptr)), 0, 0);

				if (NULL == bmp)
				{
					::DeleteDC(cdc);
					::ReleaseDC(NULL, hdc);
					throw bad_alloc{};
				}

				::DeleteObject((HBITMAP)::SelectObject(cdc, bmp));
				::DeleteObject(::SelectObject(cdc, dw->font->native_handle()));

				dw->context = cdc;
				dw->pixmap = bmp;
				::SetBkMode(cdc, TRANSPARENT);

				::ReleaseDC(0, hdc);
#elif defined(NANA_X11)
				auto & spec = platform_spec::instance();
				{
					platform_scope_guard psg;

					spec.set_error_handler();

					Display* disp = spec.open_display();
					int screen = DefaultScreen(disp);
					Window root = ::XRootWindow(disp, screen);
					auto pixmap = ::XCreatePixmap(disp, root, sz.width, sz.height, DefaultDepth(disp, screen));
					if(spec.error_code)
					{
						spec.rev_error_handler();
						throw bad_alloc();
					}
					auto context = ::XCreateGC(disp, pixmap, 0, 0);
					if (spec.error_code)
					{
						::XFreePixmap(disp, pixmap);
						spec.rev_error_handler();
						throw bad_alloc();
					}
#	if defined(NANA_USE_XFT)
					auto xftdraw = ::XftDrawCreate(disp, pixmap, spec.screen_visual(), spec.colormap());
					if (spec.error_code)
					{
						::XFreeGC(disp, context);
						::XFreePixmap(disp, pixmap);

						spec.rev_error_handler();
						throw bad_alloc();
					}

					dw->xftdraw = xftdraw;
#	endif
					dw->pixmap = pixmap;
					dw->context = context;
				}
#endif
				if(dw)
				{
					dw->set_color(colors::black);
					dw->set_text_color(colors::black);
#if defined(NANA_WINDOWS)
					dw->bytes_per_line = sz.width * sizeof(pixel_argb_t);
#else
					dw->update_text_color();
#endif
					impl_->platform_drawable = dw;
					impl_->handle = dw.get();
					impl_->size = sz;

					impl_->handle->string.tab_pixels = raw_text_extent_size(impl_->handle, L"\t", 1).width;
					impl_->handle->string.whitespace_pixels = raw_text_extent_size(impl_->handle, L" ", 1).width;
				}
			}

			if(impl_->changed == false)
				impl_->changed = true;
		}

		void graphics::resize(const size& sz)
		{
			graphics duplicate(move(*this));
			make(sz);
			bitblt(0, 0, duplicate);
		}

		void graphics::typeface(const font& f)
		{
			//Keep the font as a shadow, even if the graphics is empty. Setting the font is futile when the size
			//of a widget is zero.
			impl_->font_shadow = f;
			if(impl_->handle && (false == f.empty()))
			{
				impl_->handle->font = f.impl_->real_font;
#if defined(NANA_WINDOWS)
				::SelectObject(impl_->handle->context, reinterpret_cast<HFONT>(f.impl_->real_font->native_handle()));
#endif
				impl_->handle->string.tab_pixels = raw_text_extent_size(impl_->handle, L"\t", 1).width;
				impl_->handle->string.whitespace_pixels = raw_text_extent_size(impl_->handle, L" ", 1).width;
				
				if (impl_->changed == false)
					impl_->changed = true;
			}
		}

		font graphics::typeface() const
		{
			//The font may be set when the graphics is still empty.
			//it should return the shadow font when the graphics is empty.
			return (impl_->handle ? font(impl_->handle) : impl_->font_shadow);
		}

		size graphics::text_extent_size(const char* text) const
		{
			throw_not_utf8(text);
			return text_extent_size(to_wstring(text));
		}

		size graphics::text_extent_size(const char* text, size_t len) const
		{
			return text_extent_size(string(text, text + len));
		}

		size	graphics::text_extent_size(const wchar_t* text)	const
		{
			return text_extent_size(text, wcslen(text));
		}

		size	graphics::text_extent_size(const wchar* text)	const
		{
			return text_extent_size(text, static_cast<unsigned>(text.length()));
		}

		size	graphics::text_extent_size(const wchar_t* str, size_t len)	const
		{
			return text_extent_size(impl_->handle, str, len);
		}

		size	graphics::text_extent_size(const wchar* str, size_t len)	const
		{
			return text_extent_size(impl_->handle, str, len);
		}

		size graphics::glyph_extent_size(const wchar_t * str, size_t len, size_t begin, size_t end) const
		{
			if(len < end) end = len;
			if (NULL == impl_->handle || NULL == str || 0 == len || begin >= end) return{};

			size sz;
#if defined(NANA_WINDOWS)
			int * dx = new int[len];
			SIZE extents;
			::GetTextExtentExPoint(impl_->handle->context, str, static_cast<int>(len), 0, 0, dx, &extents);
			sz.width = dx[end - 1] - (begin ? dx[begin - 1] : 0);
			unsigned tab_pixels = impl_->handle->string.tab_length * impl_->handle->string.whitespace_pixels;
			const wchar_t * pend = str + end;
			for(const wchar_t * p = str + begin; p != pend; ++p)
			{
				if(*p == '\t')
					sz.width += tab_pixels;
			}
			sz.height = extents.cy;
			delete [] dx;
#elif defined(NANA_X11)
			sz = text_extent_size(str + begin, end - begin);
#endif
			return sz;
		}

		size graphics::glyph_extent_size(const wchar* str, size_t len, size_t begin, size_t end) const
		{
			return glyph_extent_size(str, len, begin, end);
		}

		bool graphics::glyph_pixels(const wchar_t * str, size_t len, unsigned* pxbuf) const
		{
			if(NULL == impl_->handle || NULL == impl_->handle->context || NULL == str || NULL == pxbuf) return false;
			if(len == 0) return true;

			unsigned tab_pixels = impl_->handle->string.tab_length * impl_->handle->string.whitespace_pixels;
#if defined(NANA_WINDOWS)
			int * dx = new int[len];
			SIZE extents;
			::GetTextExtentExPoint(impl_->handle->context, str, static_cast<int>(len), 0, 0, dx, &extents);

			pxbuf[0] = (str[0] == '\t' ? tab_pixels  : dx[0]);

			for(size_t i = 1; i < len; ++i)
			{
				pxbuf[i] = (str[i] == '\t' ? tab_pixels : dx[i] - dx[i - 1]);
			}
			delete [] dx;
#elif defined(NANA_X11) && defined(NANA_USE_XFT)

			auto disp = platform_spec::instance().open_display();
			auto xft = reinterpret_cast<XftFont*>(impl_->handle->font->native_handle());

			XGlyphInfo extents;
			for(size_t i = 0; i < len; ++i)
			{
				if(str[i] != '\t')
				{
					FT_UInt glyphs = ::XftCharIndex(disp, xft, str[i]);
					::XftGlyphExtents(disp, xft, &glyphs, 1, &extents);
					pxbuf[i] = extents.xOff;
				}
				else
					pxbuf[i] = tab_pixels;
			}
#endif
			return true;
		}

		size	graphics::bidi_extent_size(const wchar* str) const
		{
			size sz;
			if(impl_->handle && impl_->handle->context && str.size())
			{
				auto const reordered = unicode_reorder(str, str.size());
				for(auto & i: reordered)
				{
					size t = text_extent_size(i.begin, i.end - i.begin);
					sz.width += t.width;
					if(sz.height < t.height)
						sz.height = t.height;
				}
			}
			return sz;
		}

		size graphics::bidi_extent_size(const char* str) const
		{
			return bidi_extent_size(static_cast<wstring>(charset(str, unicode::utf8)));
		}

		bool graphics::text_metrics(unsigned & ascent, unsigned& descent, unsigned& internal_leading) const
		{
			if(impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::TEXTMETRIC tm;
				::GetTextMetrics(impl_->handle->context, &tm);
				ascent = static_cast<unsigned>(tm.tmAscent);
				descent = static_cast<unsigned>(tm.tmDescent);
				internal_leading = static_cast<unsigned>(tm.tmInternalLeading);
				return true;
#elif defined(NANA_X11)
				if(impl_->handle->font)
				{
	#if defined(NANA_USE_XFT)
					auto fs = reinterpret_cast<XftFont*>(impl_->handle->font->native_handle());
					ascent = fs->ascent;
					descent = fs->descent;
					internal_leading = 0;
	#else
					auto fs = reinterpret_cast<XFontSet>(impl_->handle->font->native_handle());
					XFontSetExtents * ext = ::XExtentsOfFontSet(fs);
					XFontStruct ** fontstructs;
					char ** font_names;
					int size = ::XFontsOfFontSet(fs, &fontstructs, &font_names);
					ascent = 0;
					descent = 0;
					XFontStruct **fontstructs_end = fontstructs + size;
					for(XFontStruct** i = fontstructs; i < fontstructs_end; ++i)
					{
						if(ascent < (*i)->ascent)
							ascent = (*i)->ascent;
						if(descent < (*i)->descent)
							descent = (*i)->descent;
					}
	#endif
					return true;
				}
#endif
			}
			return false;
		}

		void graphics::line_begin(int x, int y)
		{
			if(!impl_->handle)	return;
#if defined(NANA_WINDOWS)
			::MoveToEx(impl_->handle->context, x, y, 0);

#elif defined(NANA_X11)
			impl_->handle->line_begin_pos.x = x;
			impl_->handle->line_begin_pos.y = y;
#endif
		}

		void graphics::bitblt(int x, int y, const graphics& src)
		{
			rectangle r(src.size());
			r.x = x;
			r.y = y;
			bitblt(r, src);
		}

		void graphics::bitblt(const rectangle& r_dst, native_window_type src)
		{
			if(impl_->handle)
			{
#if defined(NANA_WINDOWS)
				HDC dc = ::GetDC(reinterpret_cast<HWND>(src));
				::BitBlt(impl_->handle->context, r_dst.x, r_dst.y, r_dst.width, r_dst.height, dc, 0, 0, SRCCOPY);
				::ReleaseDC(reinterpret_cast<HWND>(src), dc);
#elif defined(NANA_X11)
				::XCopyArea(platform_spec::instance().open_display(),
						reinterpret_cast<Window>(src), impl_->handle->pixmap, impl_->handle->context,
						0, 0, r_dst.width, r_dst.height, r_dst.x, r_dst.y);
#endif
				if(impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::bitblt(const rectangle& r_dst, native_window_type src, const point& p_src)
		{
			if(impl_->handle)
			{
#if defined(NANA_WINDOWS)
				HDC dc = ::GetDC(reinterpret_cast<HWND>(src));
				::BitBlt(impl_->handle->context, r_dst.x, r_dst.y, r_dst.width, r_dst.height, dc, p_src.x, p_src.y, SRCCOPY);
				::ReleaseDC(reinterpret_cast<HWND>(src), dc);
#elif defined(NANA_X11)
				::XCopyArea(platform_spec::instance().open_display(),
						reinterpret_cast<Window>(src), impl_->handle->pixmap, impl_->handle->context,
						p_src.x, p_src.y, r_dst.width, r_dst.height, r_dst.x, r_dst.y);
#endif
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::bitblt(const rectangle& r_dst, const graphics& src)
		{
			if(impl_->handle && src.impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::BitBlt(impl_->handle->context, r_dst.x, r_dst.y, r_dst.width, r_dst.height, src.impl_->handle->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
				::XCopyArea(platform_spec::instance().open_display(),
						src.impl_->handle->pixmap, impl_->handle->pixmap, impl_->handle->context,
						0, 0, r_dst.width, r_dst.height, r_dst.x, r_dst.y);
#endif
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::bitblt(const rectangle& r_dst, const graphics& src, const point& p_src)
		{
			if(impl_->handle && src.impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::BitBlt(impl_->handle->context, r_dst.x, r_dst.y, r_dst.width, r_dst.height, src.impl_->handle->context, p_src.x, p_src.y, SRCCOPY);
#elif defined(NANA_X11)
				::XCopyArea(platform_spec::instance().open_display(),
						src.impl_->handle->pixmap, impl_->handle->pixmap, impl_->handle->context,
						p_src.x, p_src.y, r_dst.width, r_dst.height, r_dst.x, r_dst.y);
#endif
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::blend(const rectangle& r, const color& clr, double fade_rate)
		{
			if (impl_->handle)
			{
				blend(impl_->handle, r, clr.px_color(), fade_rate);
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::blend(const rectangle& blend_r, const graphics& graph, const point& blend_graph_point, double fade_rate)///< blends with the blend_graph.
		{
			if (graph.impl_->handle && impl_->handle && (graph.impl_->handle != impl_->handle))
			{
				pixel_buffer graph_px;

				rectangle r{ blend_graph_point, blend_r.dimension() };

				graph_px.attach(graph.impl_->handle, r);
				graph_px.blend(r, impl_->handle, blend_r.position(), 1 - fade_rate);

				if (graph.impl_->changed == false) graph.impl_->changed = true;
			}
		}

		void graphics::blur(const rectangle& r, size_t radius)
		{
			if(impl_->handle)
			{
				pixel_buffer pixbuf(impl_->handle, 0, 0);
				pixbuf.blur(r, radius);
				pixbuf.paste(impl_->handle, point{});
			}
		}

		void graphics::rgb_to_wb()
		{
			if(impl_->handle)
			{
				//Create the color table for perfermance
				float* tablebuf = new float[0x100 * 3];
				float* table_red = tablebuf;
				float* table_green = tablebuf + 0x100;
				float* table_blue = tablebuf + 0x200;

				for(int i = 0; i < 0x100; ++i)
				{
					table_red[i] = (i * 0.3f);
					table_green[i] = (i * 0.59f);
					table_blue[i] = (i * 0.11f);
				}

				pixel_buffer pixbuf(impl_->handle, 0, 0);

				auto pixels = pixbuf.raw_ptr(0);

				const size sz = drawable_size(impl_->handle);
				const int rest = sz.width % 4;
				const int length_align4 = sz.width - rest;

				for(unsigned y = 0; y < sz.height; ++y)
				{
					const auto end = pixels + length_align4;
					for(; pixels < end; pixels += 4)
					{
						unsigned char gray = static_cast<unsigned char>(table_red[pixels[0].element.red] + table_green[pixels[0].element.green] + table_blue[pixels[0].element.blue] + 0.5f);
						pixels[0].value = gray << 16 | gray << 8| gray;

						gray = static_cast<unsigned char>(table_red[pixels[1].element.red] + table_green[pixels[1].element.green] + table_blue[pixels[1].element.blue] + 0.5f);
						pixels[1].value = gray << 16 | gray << 8 | gray;

						gray = static_cast<unsigned char>(table_red[pixels[2].element.red] + table_green[pixels[2].element.green] + table_blue[pixels[2].element.blue] + 0.5f);
						pixels[2].value = gray << 16 | gray << 8 | gray;

						gray = static_cast<unsigned char>(table_red[pixels[3].element.red] + table_green[pixels[3].element.green] + table_blue[pixels[3].element.blue] + 0.5f);
						pixels[3].value = gray << 16 | gray << 8 | gray;
					}

					for(int i = 0; i < rest; ++i)
					{
						unsigned char gray = static_cast<unsigned char>(table_red[pixels[i].element.red] + table_green[pixels[i].element.green] + table_blue[pixels[i].element.blue] + 0.5f);
						pixels[i].element.red = gray;
						pixels[i].element.green = gray;
						pixels[i].element.blue = gray;
					}

					pixels += rest;
				}
				delete [] tablebuf;

				pixbuf.paste(impl_->handle, point{});
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::paste(graphics& dst, int x, int y) const
		{
			if(impl_->handle && dst.impl_->handle && impl_->handle != dst.impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::BitBlt(dst.impl_->handle->context, x, y, impl_->size.width, impl_->size.height, impl_->handle->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
				Display* display = platform_spec::instance().open_display();
				::XCopyArea(display,
					impl_->handle->pixmap, dst.impl_->handle->pixmap, impl_->handle->context,
						0, 0, impl_->size.width, impl_->size.height, x, y);

				::XFlush(display);
#endif
				dst.impl_->changed = true;
			}
		}

		void graphics::paste(native_window_type dst, const rectangle& r, int x, int y) const
		{
			paste(dst, r.x, r.y, r.width, r.height, x, y);
		}

		void graphics::paste(native_window_type dst, int dx, int dy, unsigned width, unsigned height, int sx, int sy) const
		{
			if(impl_->handle)
			{
#if defined(NANA_WINDOWS)
				HDC dc = ::GetDC(reinterpret_cast<HWND>(dst));
				if(dc)
				{
					::BitBlt(dc, dx, dy, width, height, impl_->handle->context, sx, sy, SRCCOPY);
					::ReleaseDC(reinterpret_cast<HWND>(dst), dc);
				}
#elif defined(NANA_X11)
				auto & spec = platform_spec::instance();
				
				Display * display = spec.open_display();
				
				platform_scope_guard lock;
				
				::XCopyArea(display,
					impl_->handle->pixmap, reinterpret_cast<Window>(dst), impl_->handle->context,
						sx, sy, width, height, dx, dy);

				XWindowAttributes attr;
				spec.set_error_handler();
				::XGetWindowAttributes(display, reinterpret_cast<Window>(dst), &attr);
				if(BadWindow != spec.rev_error_handler() && attr.map_state != IsUnmapped)
					::XMapWindow(display, reinterpret_cast<Window>(dst));
					
				::XFlush(display);
#endif
			}
		}

		void graphics::paste(drawable_type dst, int x, int y) const
		{
			if(impl_->handle && dst && impl_->handle != dst)
			{
#if defined (NANA_WINDOWS)
				::BitBlt(dst->context, x, y, impl_->size.width, impl_->size.height, impl_->handle->context, 0, 0, SRCCOPY);
#elif defined(NANA_X11)
				Display * display = platform_spec::instance().open_display();
				::XCopyArea(display,
					impl_->handle->pixmap, dst->pixmap, impl_->handle->context,
						0, 0, impl_->size.width, impl_->size.height, x, y);
				::XFlush(display);
#endif
			}
		}


		void graphics::paste(const rectangle& r_src, graphics& dst, int x, int y) const
		{
			if(impl_->handle && dst.impl_->handle && impl_->handle != dst.impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::BitBlt(dst.impl_->handle->context, x, y, r_src.width, r_src.height, impl_->handle->context, r_src.x, r_src.y, SRCCOPY);
#elif defined(NANA_X11)
				Display* display = platform_spec::instance().open_display();
				::XCopyArea(display,
					impl_->handle->pixmap, dst.impl_->handle->pixmap, impl_->handle->context,
						r_src.x, r_src.y, r_src.width, r_src.height, x, y);

				::XFlush(display);
#endif
			}
		}

		void graphics::stretch(const rectangle& src_r, graphics& dst, const rectangle& r) const
		{
			if(impl_->handle && dst.impl_->handle && (impl_->handle != dst.impl_->handle))
			{
				pixel_buffer pixbuf(impl_->handle, 0, 0);
				pixbuf.stretch(src_r, dst.impl_->handle, r);
			}
		}

		void graphics::stretch(graphics& dst, const rectangle & r) const
		{
			stretch(rectangle(size()), dst, r);
		}

		void graphics::flush()
		{
#if defined(NANA_WINDOWS)
			::GdiFlush();
#endif
		}

		unsigned graphics::width() const{
			return impl_->size.width;
		}

		unsigned graphics::height() const{
			return impl_->size.height;
		}

		size graphics::size() const{
			return this->impl_->size;
		}

		void graphics::setsta()
		{
			impl_->changed = false;
		}

		void graphics::set_changed()
		{
			impl_->changed = true;
		}

		void graphics::release()
		{
			impl_->platform_drawable.reset();
			impl_->handle = NULL;
			impl_->size.width = impl_->size.height = 0;
		}

		void graphics::save_as_file(const char* file_utf8) const
		{
			if(impl_->handle)
			{
#if defined(NANA_WINDOWS)
				const int iWidth = static_cast<int>(impl_->size.width);
				const int iHeight = static_cast<int>(impl_->size.height);
				BITMAPINFO bmpInfo = {};
				bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
				bmpInfo.bmiHeader.biWidth = iWidth;
				bmpInfo.bmiHeader.biHeight = iHeight;
				bmpInfo.bmiHeader.biPlanes = 1;
				bmpInfo.bmiHeader.biBitCount = 24;

				const size_t lineBytes = ((bmpInfo.bmiHeader.biWidth * 3) + 3) & (~3);
				const size_t imageBytes = iHeight * lineBytes;

				HDC hdcMem = ::CreateCompatibleDC(impl_->handle->context);
				BYTE *pData = NULL;
				HBITMAP hBmp = ::CreateDIBSection(hdcMem, &bmpInfo, DIB_RGB_COLORS, reinterpret_cast<void**>(&pData), 0, 0);

				::SelectObject(hdcMem, hBmp);

				BitBlt(hdcMem, 0, 0, iWidth, iHeight, impl_->handle->context, 0, 0, SRCCOPY);

				BITMAPFILEHEADER bmFileHeader = { 0 };
				bmFileHeader.bfType = 0x4d42;  //bmp
				bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
				bmFileHeader.bfSize = bmFileHeader.bfOffBits + static_cast<DWORD>(imageBytes);

				HANDLE hFile = ::CreateFileW(static_cast<wstring>(charset(file_utf8, unicode::utf8)).data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				DWORD dwWrite = 0;
				::WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
				::WriteFile(hFile, &bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
				::WriteFile(hFile, pData, static_cast<DWORD>(imageBytes), &dwWrite, NULL);
				::CloseHandle(hFile);

				::DeleteObject(hBmp);
				::DeleteDC(hdcMem);
#elif defined(NANA_X11)
				static_cast<void>(file_utf8);	//eliminate unused parameter compil warning.
#endif
			}
		}

		color graphics::palette(bool for_text) const
		{
			if (impl_->handle)
				return static_cast<color_rgb>(for_text ? impl_->handle->get_text_color() : impl_->handle->get_color());

			return{};
		}

		graphics& graphics::palette(bool for_text, const color& clr)
		{
			if (impl_->handle)
			{
				if (for_text)
					impl_->handle->set_text_color(clr);
				else
					impl_->handle->set_color(clr);
			}

			return *this;
		}

		unsigned graphics::bidi_string(const point& pos, const wchar_t * str, size_t len)
		{
			auto moved_pos = pos;

			auto const reordered = unicode_reorder(str, len);
			for (auto & i : reordered)
			{
				string(moved_pos, i.begin, i.end - i.begin);
				moved_pos.x += static_cast<int>(text_extent_size(i.begin, i.end - i.begin).width);
			}
			return static_cast<unsigned>(moved_pos.x - pos.x);
		}

		unsigned graphics::bidi_string(const point& pos, const char* str, size_t len)
		{
			wstring wstr = charset(string(str, str + len), unicode::utf8);
			return bidi_string(pos, wstr.data(), wstr.size());
		}

		void graphics::set_pixel(int x, int y, const color& clr)
		{
			if (impl_->handle)
			{
				impl_->handle->set_color(clr);
				set_pixel(x, y);
			}
		}

		void graphics::set_pixel(int x, int y)
		{
			if (impl_->handle)
			{
#if defined(NANA_WINDOWS)
				::SetPixel(impl_->handle->context, x, y, NANA_RGB(impl_->handle->get_color()));
#elif defined(NANA_X11)
				Display* disp = platform_spec::instance().open_display();
				impl_->handle->update_color();
				::XDrawPoint(disp, impl_->handle->pixmap, impl_->handle->context, x, y);
#endif
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::string(const point& pos, const char* text_utf8)
		{
			string(pos, to_wstring(text_utf8));
		}

		void graphics::string(const point& pos, const char* text_utf8, const color& clr)
		{
			palette(true, clr);
			string(pos, text_utf8);
		}

		void graphics::string(point pos, const wchar_t* str, size_t len)
		{
			if (impl_->handle && str && len)
			{
				auto const end = str + len;
				auto i = find(str, end, '\t');
#if defined(NANA_LINUX) || defined(NANA_MACOS)
				impl_->handle->update_text_color();
#endif
				if (i != end)
				{
					size_t tab_pixels = impl_->handle->string.tab_length * impl_->handle->string.tab_pixels;
					while (true)
					{
						len = i - str;
						if (len)
						{
							//Render a part that does not contains a tab
							draw_string(impl_->handle, pos, str, len);
							pos.x += raw_text_extent_size(impl_->handle, str, len).width;
						}

						str = i;
						while (str != end && (*str == '\t'))
							++str;

						if (str != end)
						{
							//Now i_tab is not a tab, but a non-tab character following the previous tabs
							pos.x += static_cast<int>(tab_pixels * (str - i));
							i = find(str, end, '\t');
						}
						else
							break;
					}
				}
				else
					draw_string(impl_->handle, pos, str, len);
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::string(const point& pos, const wchar_t* str)
		{
			string(pos, str, wcslen(str));
		}

		void graphics::string(const point& pos, const wchar* str)
		{
			string(pos, str.data(), str.size());
		}

		void graphics::string(const point& pos, const ::wchar* text, const color& clr)
		{
			palette(true, clr);
			string(pos, text.data(), text.size());
		}

		void graphics::line(const point& pos1, const point& pos2)
		{
			if (!impl_->handle)	return;
#if defined(NANA_WINDOWS)
			if (pos1 != pos2)
			{
				auto prv_pen = ::SelectObject(impl_->handle->context, ::CreatePen(PS_SOLID, 1, NANA_RGB(impl_->handle->get_color())));

				::MoveToEx(impl_->handle->context, pos1.x, pos1.y, 0);
				::LineTo(impl_->handle->context, pos2.x, pos2.y);

				::DeleteObject(::SelectObject(impl_->handle->context, prv_pen));
			}
			::SetPixel(impl_->handle->context, pos2.x, pos2.y, NANA_RGB(impl_->handle->get_color()));
#elif defined(NANA_X11)
			Display* disp = platform_spec::instance().open_display();
			impl_->handle->update_color();
			::XDrawLine(disp, impl_->handle->pixmap, impl_->handle->context, pos1.x, pos1.y, pos2.x, pos2.y);
#endif
			if (impl_->changed == false) impl_->changed = true;
		}

		void graphics::line(const point& pos_a, const point& pos_b, const color& clr)
		{
			palette(false, clr);
			line(pos_a, pos_b);
		}

		void graphics::line_to(const point& pos, const color& clr)
		{
			if (!impl_->handle) return;
			impl_->handle->set_color(clr);
			line_to(pos);
		}

		void graphics::line_to(const point& pos)
		{
			if (!impl_->handle)	return;
#if defined(NANA_WINDOWS)
			auto prv_pen = ::SelectObject(impl_->handle->context, ::CreatePen(PS_SOLID, 1, NANA_RGB(impl_->handle->get_color())));

			::LineTo(impl_->handle->context, pos.x, pos.y);

			::DeleteObject(::SelectObject(impl_->handle->context, prv_pen));
#elif defined(NANA_X11)
			Display* disp = platform_spec::instance().open_display();
			impl_->handle->update_color();
			::XDrawLine(disp, impl_->handle->pixmap, impl_->handle->context,
				impl_->handle->line_begin_pos.x, impl_->handle->line_begin_pos.y,
				pos.x, pos.y);
			impl_->handle->line_begin_pos = pos;
#endif
			if (impl_->changed == false) impl_->changed = true;
		}

		void graphics::rectangle(bool solid)
		{
			rectangle(rectangle{ size() }, solid);
		}

		void graphics::rectangle(bool solid, const color& clr)
		{
			palette(false, clr);
			rectangle(rectangle{ size() }, solid);
		}

		void graphics::rectangle(const rectangle& r, bool solid)
		{
			if (r.width && r.height && impl_->handle && r.right() > 0 && r.bottom() > 0)
			{
#if defined(NANA_WINDOWS)

				auto brush = ::CreateSolidBrush(NANA_RGB(impl_->handle->get_color()));

				::RECT native_r = { r.x, r.y, r.right(), r.bottom()};

				(solid ? ::FillRect : ::FrameRect)(impl_->handle->context, &native_r, brush);

				::DeleteObject(brush);
#elif defined(NANA_X11)
				Display* disp = platform_spec::instance().open_display();
				impl_->handle->update_color();
				if (solid)
					::XFillRectangle(disp, impl_->handle->pixmap, impl_->handle->context, r.x, r.y, r.width, r.height);
				else
					::XDrawRectangle(disp, impl_->handle->pixmap, impl_->handle->context, r.x, r.y, r.width - 1, r.height - 1);
#endif
				if (impl_->changed == false) impl_->changed = true;
			}
		}

		void graphics::rectangle(const rectangle& r, bool solid, const color& clr)
		{
			palette(false, clr);
			rectangle(r, solid);
		}

		void graphics::frame_rectangle(const rectangle& r, const color& left_clr, const color& top_clr, const color& right_clr, const color& bottom_clr)
		{
			int right = r.right() - 1;
			int bottom = r.bottom() - 1;
			line_begin(r.x, r.y);
			line_to({ right, r.y }, top_clr);
			line_to({ right, bottom }, right_clr);
			line_to({ r.x, bottom }, bottom_clr);
			line_to({ r.x, r.y }, left_clr);
		}

		void graphics::frame_rectangle(const rectangle& r, const color& clr, unsigned gap)
		{
			palette(false, clr);

			if (r.width > gap * 2)
			{
				point left{ r.x + static_cast<int>(gap), r.y }, right{ r.right() - static_cast<int>(gap) - 1, r.y };
				line(left, right);

				left.y = right.y = r.bottom() - 1;
				line(left, right);
			}

			if (r.height > gap * 2)
			{
				point top{ r.x, r.y + static_cast<int>(gap) }, bottom{ r.x, r.bottom() - static_cast<int>(gap) - 1 };
				line(top, bottom);

				top.x = bottom.x = r.right() - 1;
				line(top, bottom);
			}
		}

		void graphics::gradual_rectangle(const rectangle& rct, const color& from, const color& to, bool vertical)
		{
#if defined(NANA_WINDOWS)
			if (impl_->pxbuf.open(impl_->handle))
			{
				impl_->pxbuf.gradual_rectangle(rct, from, to, 0.0, vertical);
				impl_->pxbuf.paste(impl_->handle, point{});
			}
#elif defined(NANA_X11)
			if (NULL == impl_->handle) return;

			rectangle good_rct;
			if(!overlap(rectangle{ size() }, rct, good_rct))
				return;

			double deltapx = double(vertical ? good_rct.height : good_rct.width);
			double r, g, b;
			const double delta_r = (to.r() - (r = from.r())) / deltapx;
			const double delta_g = (to.g() - (g = from.g())) / deltapx;
			const double delta_b = (to.b() - (b = from.b())) / deltapx;

			unsigned last_color = (int(r) << 16) | (int(g) << 8) | int(b);

			Display * disp = platform_spec::instance().open_display();
			impl_->handle->set_color(static_cast<color_rgb>(last_color));
			impl_->handle->update_color();
			const int endpos = deltapx + (vertical ? good_rct.y : good_rct.x);
			if (endpos > 0)
			{
				if (vertical)
				{
					int x1 = good_rct.x, x2 = good_rct.right();
					auto y = good_rct.y;
					for (; y < endpos; ++y)
					{
						::XDrawLine(disp, impl_->handle->pixmap, impl_->handle->context, x1, y, x2, y);
						unsigned new_color = (int(r += delta_r) << 16) | (int(g += delta_g) << 8) | int(b += delta_b);
						if (new_color != last_color)
						{
							last_color = new_color;
							impl_->handle->set_color(static_cast<color_rgb>(last_color));
							impl_->handle->update_color();
						}
					}
				}
				else
				{
					int y1 = good_rct.y, y2 = good_rct.bottom();
					auto x = good_rct.x;
					for (; x < endpos; ++x)
					{
						::XDrawLine(disp, impl_->handle->pixmap, impl_->handle->context, x, y1, x, y2);
						unsigned new_color = (int(r += delta_r) << 16) | (int(g += delta_g) << 8) | int(b += delta_b);
						if (new_color != last_color)
						{
							last_color = new_color;
							impl_->handle->set_color(static_cast<color_rgb>(last_color));
							impl_->handle->update_color();
						}
					}
				}
			}
#endif
			if (impl_->changed == false) impl_->changed = true;
		}

		void graphics::round_rectangle(const rectangle& r, unsigned radius_x, unsigned radius_y, const color& clr, bool solid, const color& solid_clr)
		{
			if (impl_->handle)
			{
#if defined(NANA_WINDOWS)
				impl_->handle->set_color(clr);

				if (solid)
				{
					auto prv_pen = ::SelectObject(impl_->handle->context, ::CreatePen(PS_SOLID, 1, NANA_RGB(impl_->handle->get_color())));
					auto prv_brush = ::SelectObject(impl_->handle->context, ::CreateSolidBrush(NANA_RGB(solid_clr.px_color().value)));

					::RoundRect(impl_->handle->context, r.x, r.y, r.right(), r.bottom(), static_cast<int>(radius_x * 2), static_cast<int>(radius_y * 2));

					::DeleteObject(::SelectObject(impl_->handle->context, prv_brush));
					::DeleteObject(::SelectObject(impl_->handle->context, prv_pen));
				}
				else
				{
					auto brush = ::CreateSolidBrush(NANA_RGB(impl_->handle->get_color()));

					auto region = ::CreateRoundRectRgn(r.x, r.y, r.x + static_cast<int>(r.width) + 1, r.y + static_cast<int>(r.height) + 1, static_cast<int>(radius_x + 1), static_cast<int>(radius_y + 1));

					::FrameRgn(impl_->handle->context, region, brush, 1, 1);

					::DeleteObject(region);
					::DeleteObject(brush);
				}

				if (impl_->changed == false) impl_->changed = true;
#elif defined(NANA_X11)
				if(solid && (clr == solid_clr))
				{
					rectangle(r, true, clr);
				}
				else
				{
					rectangle(r, false, clr);
					if(solid)
						rectangle(rectangle(r).pare_off(1), true, solid_clr);
				}

				//eliminate unused parameter compiler warning.
				static_cast<void>(radius_x);
				static_cast<void>(radius_y);
#endif
			}
		}
	//end class graphics

	//class draw
		draw::draw(graphics& graph)
			: graph_(graph)
		{}

		void draw::corner(const rectangle& r, unsigned pixels)
		{
			if (1 == pixels)
			{
				graph_.set_pixel(r.x, r.y);
				graph_.set_pixel(r.right() - 1, r.y);

				graph_.set_pixel(r.x, r.bottom() - 1);
				graph_.set_pixel(r.right() - 1, r.bottom() - 1);
				return;
			}
			else if (1 < pixels)
			{
				graph_.line(r.position(), point(r.x + pixels, r.y));
				graph_.line(r.position(), point(r.x, r.y + pixels));

				int right = r.right() - 1;
				graph_.line(point(right, r.y), point(right - pixels, r.y));
				graph_.line(point(right, r.y), point(right, r.y - pixels));

				int bottom = r.bottom() - 1;
				graph_.line(point(r.x, bottom), point(r.x + pixels, bottom));
				graph_.line(point(r.x, bottom), point(r.x, bottom - pixels));

				graph_.line(point(right, bottom), point(right - pixels, bottom));
				graph_.line(point(right, bottom), point(right, bottom - pixels));
			}
		}
	//end class draw

    
