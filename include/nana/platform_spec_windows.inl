
#if defined(NANA_WINDOWS)
#include <windows.h>
#include <shellapi.h>

//struct messages
//@brief:	This defines some messages that are used for remote thread invocation.
//			Some Windows APIs are window-thread-dependent, the operation in other thread
//			must be posted to its own thread.
struct messages
{
	struct caret
	{
		int x;
		int y;
		unsigned width;
		unsigned height;
		bool visible;
	};

	struct map_thread
	{
		rectangle update_area;
		bool ignore_update_area;
		bool forced;
	};

	struct arg_affinity_execute
	{
		const void* function_ptr;
	};

	enum
	{
		tray = 0x501,

		async_activate,
		async_set_focus,
		remote_flush_surface,
		remote_thread_destroy_window,
		operate_caret,	//wParam: 1=Destroy, 2=SetPos
		remote_thread_set_window_pos,
		remote_thread_set_window_text,

		//Execute a function in a thread with is associated with a specified native window
		affinity_execute,

		user,
	};
};

struct drawable_impl_type
{
	HDC		context;
	HBITMAP	pixmap;
	pixel_argb_t*	pixbuf_ptr;//{NULL};
	size_t		bytes_per_line;

	struct string_spec
	{
		unsigned tab_length;
		unsigned tab_pixels;
		unsigned whitespace_pixels;
	}string;

	unsigned color_;//{ 0xffffffff };
	unsigned text_color_;//{0xffffffff};

  
	drawable_impl_type::drawable_impl_type()
	{
		string.tab_length = 4;
		string.tab_pixels = 0;
		string.whitespace_pixels = 0;
	}

	drawable_impl_type::~drawable_impl_type()
	{
		::DeleteDC(context);
		::DeleteObject(pixmap);
	}

	unsigned drawable_impl_type::get_color() const
	{
		return color_;
	}

	unsigned drawable_impl_type::get_text_color() const
	{
		return text_color_;
	}

	void drawable_impl_type::set_color(const color& clr)
	{
		color_ = (clr & 0xFFFFFF);
	}

	void drawable_impl_type::set_text_color(const color& clr)
	{
		unsigned rgb = (clr & 0xFFFFFF);
		if (text_color_ != rgb)
		{
			::SetTextColor(context, NANA_RGB(rgb));
			text_color_ = rgb;
		}
	}

};

struct co_initializer
{
	HMODULE ole32_;

	co_initializer::co_initializer()
		: ole32_(::LoadLibraryA("OLE32.DLL"))
	{
		if(ole32_)
		{
			typedef HRESULT (__stdcall *CoInitializeEx_t)(LPVOID, DWORD);
			CoInitializeEx_t fn_init = reinterpret_cast<CoInitializeEx_t>(::GetProcAddress(ole32_, "CoInitializeEx"));
			if(0 == fn_init)
			{
				::FreeLibrary(ole32_);
				ole32_ = 0;
				printf("Nana.PlatformSpec.Co_initializer: Can't locate the CoInitializeEx().");
			}
			else
				fn_init(0, COINIT_APARTMENTTHREADED | /*COINIT_DISABLE_OLE1DDE =*/0x4);
		}
		else
			printf("Nana.PlatformSpec.Co_initializer: No Ole32.DLL Loaded.");
	}

	co_initializer::~co_initializer()
	{
		if(ole32_)
		{
			typedef void (__stdcall *CoUninitialize_t)(void);
			CoUninitialize_t fn_unin = reinterpret_cast<CoUninitialize_t>(::GetProcAddress(ole32_, "CoUninitialize"));
			if(fn_unin)
				fn_unin();
			::FreeLibrary(ole32_);
		}
	}
};

void co_initialized() {
    static co_initializer co_init;
}

#endif //NANA_WINDOWS
