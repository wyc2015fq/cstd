
typedef drawable_impl_type* drawable_type;
typedef uint32* pixel_color_t;


#if defined(NANA_WINDOWS)
#include <windows.h>
#elif defined(NANA_X11)
#include <X11/Xlib.h>
#endif


	IRect valid_rectangle(const ISIZE& s, const IRect& r)
	{
    IRect good_r, aa_r = {0,0,s.w, s.h};
		overlap(aa_r, r, good_r);
		return good_r;
	}

#if defined(NANA_WINDOWS)
	void assign_windows_bitmapinfo(const ISIZE& sz, BITMAPINFO& bi)
	{
		bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
		bi.bmiHeader.biWidth = sz.w;
		bi.bmiHeader.biHeight = -(int)(sz.h);
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;
		bi.bmiHeader.biSizeImage = (DWORD)(sz.w * sz.h * sizeof(pixel_color_t));
		bi.bmiHeader.biClrUsed = 0;
		bi.bmiHeader.biClrImportant = 0;
	}
#endif
	struct pixel_buffer
  {
    drawable_type drawable; //Attached handle
    IRect valid_r;
    ISIZE pixel_size;
    pixel_color_t * raw_pixel_buffer;
    int bytes_per_line;
    bool	alpha_channel;
#if defined(NANA_X11)
    struct x11_members
    {
      bool attached;
      XImage * image;
    }x11;
#endif
  };//end struct pixel_buffer_storage
  
  
  ISIZE drawable_size(drawable_type dw)
  {
    if(0 == dw) return iSIZE(0, 0);
#if defined(NANA_WINDOWS)
    BITMAP bmp;
    ::GetObject(dw->pixmap, sizeof bmp, &bmp);
    return iSIZE(bmp.bmWidth, bmp.bmHeight);
#elif defined(NANA_X11)
    detail::platform_spec & spec = detail::platform_spec::instance();
    Window root;
    int x, y;
    unsigned w, h;
    unsigned border, depth;
    detail::platform_scope_guard psg;
    ::XGetGeometry(spec.open_display(), dw->pixmap, &root, &x, &y, &w, &h, &border, &depth);
    return iSIZE(w, h);
#endif
  }
  
  
  int alloc_fade_table(unsigned char** pptr, double fade_rate)
  {
    MYREALLOC(*pptr, 0x100 * 2);
    unsigned char* tablebuf = *pptr;
    unsigned char* d_table = tablebuf;
    unsigned char* s_table = d_table + 0x100;
    
    double fade_rate_mul_to_add = 0;
    double fade_rate_2 = fade_rate + fade_rate;
    double fade_rate_3 = fade_rate_2 + fade_rate;
    double fade_rate_4 = fade_rate_3 + fade_rate;
    
    for(int i = 0; i < 0x100; i += 4, fade_rate_mul_to_add += fade_rate_4)
    {
      d_table[0] = (unsigned char)(fade_rate_mul_to_add);
      s_table[0] = i - d_table[0];
      
      d_table[1] = (unsigned char)(fade_rate_mul_to_add + fade_rate);
      s_table[1] = i + 1 - d_table[1];
      
      d_table[2] = (unsigned char)(fade_rate_mul_to_add + fade_rate_2);
      s_table[2] = i + 2 - d_table[2];
      
      d_table[3] = (unsigned char)(fade_rate_mul_to_add + fade_rate_3);
      s_table[3] = i + 3 - d_table[3];
      
      d_table += 4;
      s_table += 4;
    }
    return 0;
  }
  
  pixel_color_t fade_color_intermedia(pixel_color_t fgcolor, const unsigned char* fade_table)
  {
    unsigned char* pclr = (unsigned char*)&fgcolor;
    fade_table += 0x100;
    pclr[0] = fade_table[pclr[0]];
    pclr[1] = fade_table[pclr[1]];
    pclr[2] = fade_table[pclr[2]];
    return fgcolor;
  }
  
  pixel_color_t fade_color_by_intermedia(pixel_color_t bgcolor, pixel_color_t fgcolor_intermedia, const unsigned char* const fade_table)
  {
    unsigned char* pbgclr = (unsigned char*)&bgcolor;
    unsigned char* pfgclr = (unsigned char*)&fgcolor_intermedia;
    pbgclr[0] = fade_table[pbgclr[0]] + pfgclr[0];
    pbgclr[1] = fade_table[pbgclr[1]] + pfgclr[1];
    pbgclr[2] = fade_table[pbgclr[2]] + pfgclr[2];
    return bgcolor;
  }

		bool _m_alloc(pixel_buffer* s)
		{
			if (s->pixel_size.h==0 || s->pixel_size.w==0)
				return false;

			MYREALLOC(s->raw_pixel_buffer, s->pixel_size.w * s->pixel_size.h);
#if defined(NANA_X11)
			x11.image = ::XCreateImage(spec.open_display(), spec.screen_visual(), 32, ZPixmap, 0, (char*)(pxbuf.get()), pixel_size.w, pixel_size.h, 32, 0);
			x11.attached = false;
			if (!x11.image)
				throw runtime_error("Nana.pixel_buffer: XCreateImage failed");

			if ((int)(bytes_per_line) != x11.image->bytes_per_line)
			{
				x11.image->data = NULL;
				XDestroyImage(x11.image);
				throw runtime_error("Nana.pixel_buffer: Invalid pixel buffer context.");
			}
#endif
			return true;
		}

    void pixel_buffer_set(pixel_buffer* s, drawable_type drawable, const IRect& want_r) {
      s->drawable = (drawable);
      s->valid_r = valid_rectangle(drawable_size(drawable), want_r);
      s->pixel_size.w = s->valid_r.w;
      s->pixel_size.h = s->valid_r.h;
#if defined(NANA_WINDOWS)
      s->raw_pixel_buffer = ((pixel_color_t*)((char*)(drawable->pixbuf_ptr + s->valid_r.x) + drawable->bytes_per_line * s->valid_r.y));
      s->bytes_per_line = drawable->bytes_per_line;
#else
      s->raw_pixel_buffer = (NULL);
      s->bytes_per_line = (sizeof(pixel_color_t) * s->valid_r.w);
#endif
      
#if defined(NANA_X11)
			//Ensure that the pixmap is updated before we copy its content.
			::XFlush(spec.open_display());
			x11.image = ::XGetImage(spec.open_display(), drawable->pixmap, valid_r.x, valid_r.y, valid_r.w, valid_r.h, AllPlanes, ZPixmap);
			x11.attached = true;
			if(NULL == x11.image)
				throw runtime_error("Nana.pixel_buffer: XGetImage failed");

			if(32 == x11.image->depth || (24 == x11.image->depth && 32 == x11.image->bitmap_pad))
			{
				if((int)(bytes_per_line) != x11.image->bytes_per_line)
				{
					XDestroyImage(x11.image);
					throw runtime_error("Nana.pixel_buffer: Invalid pixel buffer context.");
				}
				raw_pixel_buffer = (pixel_color_t*)(x11.image->data);
			}
			else if(16 == x11.image->depth)
			{
				//565 to 32
				raw_pixel_buffer = new pixel_color_t[valid_r.w * valid_r.h];
				assign((unsigned char*)(x11.image->data), valid_r.w, valid_r.h, 16, x11.image->bytes_per_line, false);
			}
			else
			{
				XDestroyImage(x11.image);
				//throw runtime_error("Nana.pixel_buffer: The color depth is not supported");
			}
#endif
		}

		void pixel_buffer_storage_free(pixel_buffer* s)
		{
#if defined(NANA_X11)
			if(NULL == drawable) //not attached
				x11.image->data = NULL;	//the image data is allocated by pixel_buffer when it is not attached with a drawable
			else if(x11.attached)	//the image should be uploaded when it is attached.
				put(drawable->pixmap, drawable->context, 0, 0, valid_r.x, valid_r.y, valid_r.w, valid_r.h);

			if(x11.image->data != (char*)(raw_pixel_buffer))
				delete [] raw_pixel_buffer;

			XDestroyImage(x11.image);
#else
      if(NULL == s->drawable)	{//not attached
				free(s->raw_pixel_buffer);
        s->raw_pixel_buffer = NULL;
      }
#endif
		}

		void assign(pixel_buffer* s, const unsigned char* rawbits, size_t w, size_t h, size_t bits_per_pixel, size_t bytes_per_line, bool is_negative)
		{
      int i;
			if (!s->raw_pixel_buffer)
				return;

			pixel_color_t* rawptr = s->raw_pixel_buffer;
			if(32 == bits_per_pixel)
			{
				if((s->pixel_size.w == w) && (s->pixel_size.h == h) && is_negative)
				{
					memcpy(rawptr, rawbits, (s->pixel_size.w * s->pixel_size.h) * 4);
				}
				else
				{
					size_t line_bytes = (s->pixel_size.w < w ? s->pixel_size.w : w) * sizeof(pixel_color_t);

					if(s->pixel_size.h < h)
						h = s->pixel_size.h;

					pixel_color_t* d = rawptr;
					const unsigned char* s1;
					int src_line_bytes;

					if (is_negative)
					{
						s1 = rawbits;
						src_line_bytes = -(int)(bytes_per_line);
					}
					else
					{
						s1 = rawbits + bytes_per_line * (h - 1);
						src_line_bytes = (int)(bytes_per_line);
					}

					for(i = 0; i < h; ++i)
					{
						memcpy(d, s1, line_bytes);
						d += s->pixel_size.w;
						s1 -= src_line_bytes;
					}
				}
			}
			else if(24 == bits_per_pixel)
			{
				if(s->pixel_size.w < w)
					w = s->pixel_size.w;

				if(s->pixel_size.h < h)
					h = s->pixel_size.h;

				pixel_color_t* d = rawptr;
				const unsigned char* s1;

				if (is_negative)
					s1 = rawbits;
				else
					s1 = rawbits + bytes_per_line * (h - 1);

				for(i = 0; i < h; ++i)
				{
					unsigned char* p = (unsigned char*)d;
					const unsigned char* end = (unsigned char*)(d + w);
					const unsigned char* s_p = s1;
					for(; p < end; p+=4)
					{
						p[0] = s_p[0];
						p[1] = s_p[1];
						p[2] = s_p[2];
						s_p += 3;
					}
					d += s->pixel_size.w;
					s1 -= s->bytes_per_line;
				}
			}
			else if(16 == bits_per_pixel)
			{
				if(s->pixel_size.w < w)
					w = s->pixel_size.w;

				if(s->pixel_size.h < h)
					h = s->pixel_size.h;

				unsigned char rgb_table[32];
				for(i =0; i < 32; ++i)
					rgb_table[i] = (unsigned char)(i * 255 / 31);

				int src_bytes_per_line;
				if (!is_negative)
				{
					rawbits += bytes_per_line * (h - 1);
					src_bytes_per_line = (int)(bytes_per_line);
				}
				else
					src_bytes_per_line = -(int)(bytes_per_line);

				pixel_color_t* d = rawptr;
				for (i = 0; i < h; ++i)
				{
					unsigned char* p = (unsigned char*)d;
					unsigned char* end = (unsigned char*)(d + w);
					const unsigned short* s_p = (const unsigned short*)(rawbits);
					for (; p < end; p+=4)
					{
						p[0] = rgb_table[(*s_p >> 11) & 0x1F];
#if defined(NANA_X11)
						p[1] = (*s_p >> 5) & 0x3F;
						p[2] = rgb_table[*s_p & 0x1F];
#else
						p[1] = rgb_table[(*s_p >> 6) & 0x1F];
						p[2] = rgb_table[(*s_p >> 1) & 0x1F];
#endif
						++s_p;
					}
					d += s->pixel_size.w;
					rawbits -= src_bytes_per_line;
				}
			}
		}

#if defined(NANA_X11)
		//The implementation of attach in X11 is same with non-attach's, because the image buffer of drawable can't be refered indirectly
		//so the pixel_buffer::open() method may call the attached version of pixel_buffer_storage construction.
		void detach()
		{
			x11.attached = false;
		}

		void put(Drawable dw, GC gc, int src_x, int src_y, int x, int y, unsigned w, unsigned h)
		{
			auto & spec = detail::platform_spec::instance();
			Display * disp = spec.open_display();
			const int depth = spec.screen_depth();

			XImage* img = ::XCreateImage(disp, spec.screen_visual(), depth, ZPixmap, 0, 0, pixel_size.w, pixel_size.h, (16 == depth ? 16 : 32), 0);
			if(sizeof(pixel_color_t) * 8 == depth || 24 == depth)
			{
				img->data = (char*)(raw_pixel_buffer);
				::XPutImage(disp, dw, gc,
							img, src_x, src_y, x, y, w, h);
			}
			else if(16 == depth)
			{
				//The format of Xorg 16bits depth is 565
				unique_ptr<unsigned short[]> table_holder(new unsigned short[256]);
				unsigned short * const fast_table = table_holder.get();
				for(int i = 0; i < 256; ++i)
					fast_table[i] = i * 31 / 255;

				size_t length = w * h;

				unique_ptr<unsigned short[]> px_holder(new unsigned short[length]);
				unsigned short * pixbuf_16bits = px_holder.get();

				if(length == pixel_size.w * pixel_size.h)
				{
					for(auto i = raw_pixel_buffer, end = raw_pixel_buffer + length; i != end; ++i)
					{
						*(pixbuf_16bits++) = (fast_table[i->element.red] << 11) | ( (i->element.green * 63 / 255) << 6) | fast_table[i->element.blue];
					}
				}
				else if(h)
				{
					unsigned sp_line_len = pixel_size.w;
					auto sp = raw_pixel_buffer + (src_x + sp_line_len * src_y);

					unsigned top = 0;
					while(true)
					{
						for(auto i = sp, end = sp + w; i != end; ++i)
						{
							*(pixbuf_16bits++) = (fast_table[i->element.red] << 11) | ((i->element.green * 63 / 255) << 6) | fast_table[i->element.blue];
						}

						if(++top < h)
								sp += sp_line_len;
					}
				}

				img->data = (char*)(px_holder.get());
				::XPutImage(disp, dw, gc,
					img, src_x, src_y, x, y, w, h);
			}
			img->data = NULL;	//Set null pointer to avoid XDestroyImage destroyes the buffer.
			XDestroyImage(img);
		}
#endif

	bool pixel_buffer_open(pixel_buffer* s, drawable_type drawable)
	{
		ISIZE sz = drawable_size(drawable);
		if(0==sz.h || 0==sz.w) return false;

#if defined(NANA_WINDOWS)
		BITMAPINFO bmpinfo;
		assign_windows_bitmapinfo(sz, bmpinfo);
		size_t read_lines = ::GetDIBits(drawable->context, drawable->pixmap, 0, (UINT)(sz.h), s->raw_pixel_buffer, &bmpinfo, DIB_RGB_COLORS);
		return (sz.h == read_lines);
#elif defined(NANA_X11)
		try
		{
			storage_ = (pixel_buffer_storage)(drawable, IRect{sz});
			storage_->detach();
			return true;
		}
		catch(...)
		{}
#endif
		return false;
	}

	bool pixel_buffer_open(pixel_buffer* s, drawable_type drawable, const IRect & want_rectangle)
  {
		IRect r, want_r;
		ISIZE sz = drawable_size(drawable);
		if(want_rectangle.x >= (int)(sz.w) || want_rectangle.y >= (int)(sz.h))
			return false;

    want_r = want_rectangle;
		if(want_r.w == 0) want_r.w = sz.w - want_r.x;
		if(want_r.h == 0) want_r.h = sz.h - want_r.y;

		if (false == overlap(iRect(0,0,sz.w,sz.h), want_r, r))
			return false;
#if defined(NANA_WINDOWS)
		BITMAPINFO bmpinfo;
		assign_windows_bitmapinfo(iSIZE(want_r.w, want_r.h), bmpinfo);

		bool need_dup = (r.w != sz.w || r.h != sz.h);

		HDC context = drawable->context;
		HBITMAP pixmap = drawable->pixmap;
		HBITMAP orig_bmp;
		if(need_dup)
		{
			context = ::CreateCompatibleDC(drawable->context);
			pixmap = ::CreateCompatibleBitmap(drawable->context, (int)(want_r.w), (int)(want_r.h));
			orig_bmp = (HBITMAP)(::SelectObject(context, pixmap));
			::BitBlt(context, r.x - want_r.x, r.y - want_r.y, r.w, r.h, drawable->context, r.x, r.y, SRCCOPY);
		}

		size_t read_lines = ::GetDIBits(context, pixmap, 0, (UINT)(want_r.h), s->raw_pixel_buffer, &bmpinfo, DIB_RGB_COLORS);

		if(need_dup)
		{
			::SelectObject(context, orig_bmp);
			::DeleteObject(pixmap);
			::DeleteDC(context);
		}

		return (want_r.h == read_lines);
#elif defined(NANA_X11)
		detail::platform_spec & spec = detail::platform_spec::instance();
		Window root;
		int x, y;
		unsigned w, h;
		unsigned border, depth;
		detail::platform_scope_guard psg;
		::XFlush(spec.open_display());
		::XGetGeometry(spec.open_display(), drawable->pixmap, &root, &x, &y, &w, &h, &border, &depth);
		XImage * image = ::XGetImage(spec.open_display(), drawable->pixmap, r.x, r.y, r.w, r.h, AllPlanes, ZPixmap);

		storage_ = make_shared<pixel_buffer_storage)(want_r.w, want_r.h);
		auto pixbuf = storage_->raw_pixel_buffer;
		if(image->depth == 32 || (image->depth == 24 && image->bitmap_pad == 32))
		{
			if(want_r.w != (unsigned)(image->w) || want_r.h != (unsigned)(image->h))
			{
				pixbuf += (r.x - want_r.x);
				pixbuf += (r.y - want_r.y) * want_r.w;
				const char* img_data = image->data;
				for(int i = 0; i < image->h; ++i)
				{
					memcpy(pixbuf, img_data, image->bytes_per_line);
					img_data += image->bytes_per_line;
					pixbuf += want_r.w;
				}
			}
			else
				memcpy(pixbuf, image->data, image->bytes_per_line * image->h);
		}
		else if(16 == image->depth)
		{
			//The format of Xorg 16bits depth is 565
			unique_ptr<unsigned[]> table_holder(new unsigned[32]);
			unsigned * const fast_table = table_holder.get();
			for(unsigned i = 0; i < 32; ++i)
				fast_table[i] = (i * 255 / 31);

			pixbuf += (r.x - want_r.x);
			pixbuf += (r.y - want_r.y) * want_r.w;
			const char* img_data = image->data;
			for(int i = 0; i < image->h; ++i)
			{
				const unsigned short * const px_data = (const unsigned short*)(img_data);

				for(int x = 0; x < image->w; ++x)
				{
					pixbuf[x].element.red		= fast_table[(px_data[x] >> 11) & 0x1F];
					pixbuf[x].element.green	= (px_data[x] >> 5) & 0x3F;
					pixbuf[x].element.blue	= fast_table[px_data[x] & 0x1F];
					pixbuf[x].element.alpha_channel = 0;
				}
				img_data += image->bytes_per_line;
				pixbuf += want_r.w;
			}
		}
		else
		{
			XDestroyImage(image);
			return false;
		}

		XDestroyImage(image);
#endif
        return true;
	}

	bool pixel_buffer_open(size_t w, size_t h)
	{
		if(w && h)
		{
      
			return true;
		}
		return false;
	}
  
  void blend(drawable_type dw, const IRect& area, pixel_color_t color, double fade_rate)
  {
    if (fade_rate <= 0) return;
    if (fade_rate > 1) fade_rate = 1;
    
    ISIZE sz = drawable_size(dw);
    IRect r;
    IRect r1 = iRect(0,0, sz.w, sz.h);
    unsigned char* pclr = (unsigned char*)&color;
    if (false == ::overlap(r1, area, r))
      return;
    
    unsigned red = (unsigned)((color & 0xFF0000) * fade_rate);
    unsigned green = (unsigned)((color & 0xFF00) * fade_rate);
    unsigned blue = (unsigned)((color & 0xFF) * fade_rate);
    
    double lrate = 1 - fade_rate;
    pixel_buffer pixbuf[1] = {0};
    pixel_buffer_set(dw, r.y, r.h);
    
    for (size_t row = 0; row < r.h; ++row)
    {
      auto i = pixbuf->raw_pixel_buffer + (row) + r.x;
      const auto end = i + r.w;
      for (; i < end; ++i)
      {
        unsigned px_r = (((unsigned)((i->value & 0xFF0000) * lrate) + red) & 0xFF0000);
        unsigned px_g = (((unsigned)((i->value & 0xFF00) * lrate) + green) & 0xFF00);
        unsigned px_b = (((unsigned)((i->value & 0xFF) * lrate) + blue) & 0xFF);
        i->value = (px_r | px_g | px_b);
      }
    }
    pixbuf.paste(IRect(r.x, 0, r.w, r.h), dw, IPOINT{r.x, r.y});
  }

	void pixel_buffer_paste(pixel_buffer* sp, const IRect& src_r, drawable_type drawable, const IPOINT& p_dst)
	{
		if(drawable && sp)
		{
			if(sp->alpha_channel)
			{
				IRect s_good_r, d_good_r;
				if(overlap(src_r, sp->pixel_size, IRect(p_dst.x, p_dst.y, src_r.w, src_r.h), paint::detail::drawable_size(drawable), s_good_r, d_good_r))
				{
					pixel_buffer d_pixbuf;
					d_pixbuf.attach(drawable, d_good_r);
					(*(sp->img_pro.alpha_blend))->process(*this, s_good_r, d_pixbuf, iPOINT(d_good_r.x, d_good_r.y));
				}
				return;
			}
#if defined(NANA_WINDOWS)
			BITMAPINFO bi;
			assign_windows_bitmapinfo(sp->pixel_size, bi);

			::SetDIBitsToDevice(drawable->context,
				p_dst.x, p_dst.y, src_r.w, src_r.h,
				src_r.x, (int)(sp->pixel_size.h) - src_r.y - src_r.h, 0, sp->pixel_size.h,
				sp->raw_pixel_buffer, &bi, DIB_RGB_COLORS);
#elif defined(NANA_X11)
			sp->put(drawable->pixmap, drawable->context, src_r.x, src_r.y, p_dst.x, p_dst.y, src_r.w, src_r.h);
#endif
		}
	}

	void pixel_buffer::paste(native_window_type wd, const IPOINT& p_dst) const
	{
		auto sp = storage_.get();
		if(NULL == wd || NULL == sp)	return;
#if defined(NANA_WINDOWS)
		HDC	handle = ::GetDC((HWND)(wd));
		if(handle)
		{
			BITMAPINFO bi;
			assign_windows_bitmapinfo(sp->pixel_size, bi);

			::SetDIBitsToDevice(handle,
				p_dst.x, p_dst.y, sp->pixel_size.w, sp->pixel_size.h,
				0, 0, 0, sp->pixel_size.h,
				sp->raw_pixel_buffer, &bi, DIB_RGB_COLORS);

			::ReleaseDC((HWND)(wd), handle);
		}
#elif defined(NANA_X11)
		auto & spec = detail::platform_spec::instance();
		Display * disp = spec.open_display();
		sp->put((Window)(wd), XDefaultGC(disp, XDefaultScreen(disp)), 0, 0, p_dst.x, p_dst.y, sp->pixel_size.w, sp->pixel_size.h);
#endif
	}

	void pixel_buffer::line(const string& name)
	{
		if (storage_ && name.ISIZE())
		{
			auto & img_pro = storage_->img_pro;
			img_pro.line_receptacle = detail::image_process_provider::instance().ref_line(name);
			if(img_pro.line_receptacle == *detail::image_process_provider::instance().line())
				img_pro.line = detail::image_process_provider::instance().line();
			else
				img_pro.line = &img_pro.line_receptacle;
		}
	}

	void pixel_buffer::line(const IPOINT &pos_beg, const IPOINT &pos_end, const ::color& clr, double fade_rate)
	{
		auto sp = storage_.get();
		if(NULL == sp) return;

		//Test if the line intersects the IRect, and retrive the two points that
		//are always in the area of IRect, good_pos_beg is left IPOINT, good_pos_end is right.
		IPOINT good_pos_beg, good_pos_end;
		if(intersection(IRect(sp->pixel_size), pos_beg, pos_end, good_pos_beg, good_pos_end))
			(*(sp->img_pro.line))->process(*this, good_pos_beg, good_pos_end, clr, fade_rate);
	}

	void pixel_buffer::rectangle(const IRect &r, const ::color& clr, double fade_rate, bool solid)
	{
		auto sp = storage_.get();
		if((NULL == sp) || (fade_rate == 1.0)) return;

		bool fade = (fade_rate != 0.0);
		unsigned char * fade_table = NULL;
		unique_ptr<unsigned char[]> autoptr;

		auto rgb_color = clr.px_color().value;
		pixel_color_t rgb_imd;
		if(fade)
		{
			autoptr = detail::alloc_fade_table(1 - fade_rate);
			fade_table = autoptr.get();
			rgb_imd.value = rgb_color;
			rgb_imd = detail::fade_color_intermedia(rgb_imd, fade_table);
		}

		int xbeg = (0 <= r.x ? r.x : 0);
		int xend = (int)(r.x + r.w < sp->pixel_size.w ? r.x + r.w : sp->pixel_size.w);
		const int ybeg = (0 <= r.y ? r.y : 0);
		int yend = (int)(r.y + r.h < sp->pixel_size.h ? r.y + r.h : sp->pixel_size.h);

		const auto p_rgb = sp->raw_pixel_buffer + ybeg * sp->pixel_size.w;
		if (solid)
		{
			auto lineptr = p_rgb + xbeg;
			auto end = p_rgb + xend;
			if (fade)
			{
				for (int top = ybeg; top < yend; ++top)
				{
					for (auto i = lineptr; i != end; ++i)
					{
						*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
					}
					lineptr += sp->pixel_size.w;
					end = lineptr + (xend - xbeg);
				}
			}
			else
			{
				for (int top = ybeg; top < yend; ++top)
				{
					for (auto i = lineptr; i != end; ++i)
						i->value = rgb_color;

					lineptr += sp->pixel_size.w;
					end = lineptr + (xend - xbeg);
				}
			}
			return;
		}

		if((ybeg == r.y) && (r.y + (int)(r.h) == yend))
		{
			auto i = p_rgb + xbeg;
			auto end = p_rgb + xend;
			auto i_other = sp->raw_pixel_buffer + (yend - 1) * sp->pixel_size.w + xbeg;
			if(fade)
			{
				for(;i != end; ++i, ++i_other)
				{
					*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
					*i_other = detail::fade_color_by_intermedia(*i_other, rgb_imd, fade_table);
				}
			}
			else
			{
				for(;i != end; ++i, ++i_other)
				{
					i->value = rgb_color;
					i_other->value = rgb_color;
				}
			}
		}
		else
		{
			if(ybeg == r.y)
			{
				auto i = p_rgb;
				auto end = p_rgb + xend;
				if(fade)
				{
					for(; i != end; ++i)
						*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
				}
				else
				{
					for(;i != end; ++i)
						i->value = rgb_color;
				}
			}

			if(r.y + (int)(r.h) == yend)
			{
				auto p_rgb = sp->raw_pixel_buffer + (yend - 1) * sp->pixel_size.w;
				auto i = p_rgb;
				auto end = p_rgb + xend;

				if(fade)
				{
					for(; i != end; ++i)
						*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
				}
				else
				{
					for(;i != end; ++i)
						i->value = rgb_color;
				}
			}
		}

		if((xbeg == r.x) && (r.x + (int)(r.w) == xend))
		{
			auto i = p_rgb + xbeg;
			auto end = sp->raw_pixel_buffer + (yend - 1) * sp->pixel_size.w + xbeg;
			auto i_other = p_rgb + (xend - 1);

			if(fade)
			{
				while(true)
				{
					*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
					*i_other = detail::fade_color_by_intermedia(*i_other, rgb_imd, fade_table);
					if(i == end)
						break;

					i += sp->pixel_size.w;
					i_other += sp->pixel_size.w;
				}
			}
			else
			{
				while(true)
				{
					i->value = rgb_color;
					i_other->value = rgb_color;
					if(i == end)
						break;

					i += sp->pixel_size.w;
					i_other += sp->pixel_size.w;
				}
			}
		}
		else
		{
			if(xbeg == r.x)
			{
				auto i = p_rgb + xbeg;
				auto end = sp->raw_pixel_buffer + (yend - 1) * sp->pixel_size.w + xbeg;
				if(fade)
				{
					while(true)
					{
						*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
						if(i == end)	break;

						i += sp->pixel_size.w;
					}
				}
				else
				{
					while(true)
					{
						i->value = rgb_color;
						if(i == end)	break;

						i += sp->pixel_size.w;
					}
				}
			}

			if(r.x + (int)(r.w) == xend)
			{
				auto i = p_rgb + (xend - 1);
				auto end = sp->raw_pixel_buffer + (yend - 1) * sp->pixel_size.w + (xend - 1);
				if(fade)
				{
					while(true)
					{
						*i = detail::fade_color_by_intermedia(*i, rgb_imd, fade_table);
						if(i == end)	break;
						i += sp->pixel_size.w;
					}
				}
				else
				{
					while(true)
					{
						i->value = rgb_color;
						if(i == end)	break;
						i += sp->pixel_size.w;
					}
				}
			}
		}
	}

	void pixel_buffer::gradual_rectangle(const ::IRect& draw_rct, const ::color& from, const ::color& to, double /*fade_rate*/, bool vertical)
	{
		auto sp = storage_.get();
		if (NULL == sp) return;

		IRect rct;
		if (false == overlap(IRect(sp->pixel_size), draw_rct, rct))
			return;

		int deltapx = int(vertical ? rct.h : rct.w);
		if (sp && deltapx)
		{
			auto beg = from.px_color().value;
			auto end = to.px_color().value;
			unsigned r, g, b;
			const int delta_r = (int(end & 0xFF0000) - int(r = (beg & 0xFF0000))) / deltapx;
			const int delta_g = (int((end & 0xFF00) << 8) - int(g = ((beg & 0xFF00) << 8))) / deltapx;
			const int delta_b = (int((end & 0xFF) << 16) - int(b = ((beg & 0xFF) << 16))) / deltapx;

			auto pxbuf = sp->raw_pixel_buffer + rct.x + rct.y * sp->pixel_size.w;
			if (vertical)
			{
				unsigned align_4 = (rct.w & ~3);
				unsigned align_reset = rct.w & 3;
				while (deltapx--)
				{
					pixel_color_t px;
					px.value = ((r += delta_r) & 0xFF0000) | (((g += delta_g) & 0xFF0000) >> 8) | (((b += delta_b) & 0xFF0000) >> 16);

					auto dpx = pxbuf;
					for (auto dpx_end = pxbuf + align_4; dpx != dpx_end; dpx += 4)
					{
						*dpx = px;
						dpx[1] = px;
						dpx[2] = px;
						dpx[3] = px;
					}

					for (auto dpx_end = dpx + align_reset; dpx != dpx_end; ++dpx)
						*dpx = px;

					pxbuf += sp->pixel_size.w;
				}
			}
			else
			{
				auto pxbuf_end = pxbuf + rct.w;

				auto dpx_end = pxbuf + rct.h * sp->pixel_size.w;
				for (; pxbuf != pxbuf_end; ++pxbuf)
				{
					pixel_color_t px;
					px.value = ((r += delta_r) & 0xFF0000) | (((g += delta_g) & 0xFF0000) >> 8) | (((b += delta_b) & 0xFF0000) >> 16);
					for (auto dpx = pxbuf; dpx != dpx_end; dpx += sp->pixel_size.w)
						*dpx = px;
					++dpx_end;
				}
			}
		}
	}

	//stretch
	void pixel_buffer::stretch(const string& name)
	{
		if (storage_ && name.ISIZE())
		{
			auto& img_pro = storage_->img_pro;
			auto op_default = detail::image_process_provider::instance().stretch();
			img_pro.stretch_receptacle = detail::image_process_provider::instance().ref_stretch(name);
			if(img_pro.stretch_receptacle == *op_default)
				img_pro.stretch = op_default;
			else
				img_pro.stretch = &img_pro.stretch_receptacle;
		}
	}

	void pixel_buffer::stretch(const IRect& src_r, drawable_type drawable, const IRect& r) const
	{
		auto sp = storage_.get();
		if(NULL == sp) return;

		IRect good_src_r, good_dst_r;
		if(overlap(src_r, sp->pixel_size, r, paint::detail::drawable_size(drawable), good_src_r, good_dst_r))
		{
			pixel_buffer dst;
			dst.attach(drawable, good_dst_r);
			(*(sp->img_pro.stretch))->process(*this, good_src_r, dst, IRect(0, 0, good_dst_r.w, good_dst_r.h));
		}
	}

	//blend
	void pixel_buffer::blend(const string& name)
	{
		if (storage_ && name.ISIZE())
		{
			auto& img_pro = storage_->img_pro;
			auto op_default = detail::image_process_provider::instance().blend();
			img_pro.blend_receptacle = detail::image_process_provider::instance().ref_blend(name);
			if(img_pro.blend_receptacle == *op_default)
				img_pro.blend = op_default;
			else
				img_pro.blend = &img_pro.blend_receptacle;
		}
	}

	void pixel_buffer::blend(const IRect& s_r, drawable_type dw_dst, const IPOINT& d_pos, double fade_rate) const
	{
		auto sp = storage_.get();
		if(NULL == sp) return;

		IRect s_good_r, d_good_r;
		if(overlap(s_r, sp->pixel_size, IRect(d_pos.x, d_pos.y, s_r.w, s_r.h), paint::detail::drawable_size(dw_dst), s_good_r, d_good_r))
		{
			pixel_buffer d_pixbuf;
			d_pixbuf.attach(dw_dst, d_good_r);
			(*(sp->img_pro.blend))->process(*this, s_good_r, d_pixbuf, IPOINT(), fade_rate);
		}
	}

	void pixel_buffer::blur(const ::IRect& r, size_t radius)
	{
		auto sp = storage_.get();
		if(NULL == sp || radius < 1)	return;

		::IRect good_r;
		if (overlap(r, ::IRect{ this->ISIZE() }, good_r))
			(*(sp->img_pro.blur))->process(*this, good_r, radius);
	}


	//x' = x*cos(angle) - y*sin(angle)
	//y' = y*cos(angle) - x*sin(angle)
	class calc_rotate
	{
		enum class angles
		{
			none, half_pi, pi, triangle_pi
		};
	public:
		calc_rotate(double angle, const basic_point<double>& origin):
			specific_{ _m_spec(angle) },
			sin_a_{ sin(angle * degree_) },
			cos_a_{ cos(angle * degree_) },
			origin_{ origin }
		{
		}

		basic_point<double> from(const IPOINT& p)
		{
			switch (specific_)
			{
			case angles::half_pi:
				return{ p.y - origin_.y, origin_.x - p.x};
			case angles::pi:
				return{ origin_.x - p.x, origin_.y - p.y};
			case angles::triangle_pi:
				return{ origin_.y - p.y, p.x - origin_.x};
			default:
				break;
			}

			return{
				(p.x - origin_.x) * cos_a_ + (p.y - origin_.y) * sin_a_,
				(p.y - origin_.y) * cos_a_ - (p.x - origin_.x) * sin_a_
			};
		}

		basic_point<double> to(const IPOINT& p)
		{
			switch (specific_)
			{
			case angles::half_pi:
				return{ origin_.y - p.y, p.x - origin_.x };
			case angles::pi:
				return{ origin_.x - p.x, origin_.y - p.y };
			case angles::triangle_pi:
				return{ p.y - origin_.y, origin_.x - p.x };
			default:
				break;
			}

			return{
				(p.x - origin_.x) * cos_a_ - (p.y - origin_.y) * sin_a_,
				(p.y - origin_.y) * cos_a_ + (p.x - origin_.x) * sin_a_
			};
		}
	private:
		static angles _m_spec(double angle)
		{
			if (90.0 == angle)
				return angles::half_pi;
			else if (180.0 == angle)
				return angles::pi;
			else if (270.0 == angle)
				return angles::triangle_pi;

			return angles::none;
		}
	private:
		const angles specific_;
		const double degree_{ acos(-1) / 180 };
		const double sin_a_;
		const double cos_a_;
		const basic_point<double> origin_;
	};

	pixel_buffer pixel_buffer::rotate(double angle, const color& extend_color)
	{
		auto sp = storage_.get();
		if (!sp)
			return{};

		if (angle <= 0 || 360 <= angle)
		{
			return{*this};
		}

		const basic_point<double> origin{ (sp->pixel_size.w - 1) / 2.0, (sp->pixel_size.h - 1) / 2.0 };
		calc_rotate point_rotate{ angle, origin };

		ISIZE size_rotated{ sp->pixel_size };

		if (90 == angle || 270 == angle)
		{
			size_rotated.shift();
		}
		else
		{
			IPOINT pw, ph;
			if (angle < 180)
			{
				ph.x = (int)(sp->pixel_size.w);
			}
			else if (angle > 180)
			{
				pw.x = (int)(sp->pixel_size.w);
			}

			size_rotated.w = (unsigned)(abs(point_rotate.from(pw).x) * 2) + 1;
			size_rotated.h = (unsigned)(abs(point_rotate.from(ph).y) * 2) + 1;
		}

		pixel_buffer rotated_pxbuf{ size_rotated.w, size_rotated.h };

		const basic_point<double> rotated_origin{ (size_rotated.w - 1) / 2.0, (size_rotated.h - 1) / 2.0 };

		for (int y = 0; y < (int)(size_rotated.h); ++y)
		{
			auto buf = rotated_pxbuf.raw_ptr(y);

			basic_point<double> dest{ -rotated_origin.x, y - rotated_origin.y };
			dest = dest + origin;

			const int right_point = (int)(dest.x) + (int)(size_rotated.w);

			for (IPOINT point_dest{ (int)(dest.x), (int)(dest.y) }; point_dest.x < right_point; ++point_dest.x)
			{
				auto point_source = point_rotate.to(point_dest) + origin;

				if (0 <= point_source.x && point_source.x <= (double)(sp->pixel_size.w - 1) && 0 <= point_source.y && point_source.y <= (double)(sp->pixel_size.h - 1))
				{
					*buf = this->raw_ptr((size_t)(point_source.y))[(int)(point_source.x)];
				}
				else
					*buf = extend_color.px_color();

				++buf;
			}
		}


		return rotated_pxbuf;
	}
