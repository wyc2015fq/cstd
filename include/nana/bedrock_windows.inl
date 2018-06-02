
#if defined(NANA_WINDOWS)



	void interior_helper_for_menu(MSG& msg, native_window_type menu_window)
	{
		switch(msg.message)
		{
		case WM_KEYDOWN:
		case WM_CHAR:
		case WM_KEYUP:
			msg.hwnd = reinterpret_cast<HWND>(menu_window);
			break;
		}
	}

	void assign_arg(arg_mouse& arg, window* wd, unsigned msg, const parameter_decoder& pmdec)
	{
		arg.window_handle = (wd->root);

		bool set_key_state = true;
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			arg.button = mouse_left_button;
			arg.evt_code = event_code_mouse_down;
			break;
		case WM_RBUTTONDOWN:
			arg.button = mouse_right_button;
			arg.evt_code = event_code_mouse_down;
			break;
		case WM_MBUTTONDOWN:
			arg.button = mouse_middle_button;
			arg.evt_code = event_code_mouse_down;
			break;
		case WM_LBUTTONUP:
			arg.button = mouse_left_button;
			arg.evt_code = event_code_mouse_up;
			break;
		case WM_RBUTTONUP:
			arg.button = mouse_right_button;
			arg.evt_code = event_code_mouse_up;
			break;
		case WM_MBUTTONUP:
			arg.button = mouse_middle_button;
			arg.evt_code = event_code_mouse_up;
			break;
		case WM_LBUTTONDBLCLK:
			arg.button = mouse_left_button;
			arg.evt_code = event_code_dbl_click;
			break;
		case WM_MBUTTONDBLCLK:
			arg.button = mouse_middle_button;
			arg.evt_code = event_code_dbl_click;
			break;
		case WM_RBUTTONDBLCLK:
			arg.button = mouse_right_button;
			arg.evt_code = event_code_dbl_click;
			break;
		case WM_MOUSEMOVE:
			arg.button = mouse_any_button;
			arg.evt_code = event_code_mouse_move;
			break;
		default:
			set_key_state = false;
		}

		if (set_key_state)
		{
			arg.pos.x = pmdec.mouse.x - wd->pos_root.x;
			arg.pos.y = pmdec.mouse.y - wd->pos_root.y;
			arg.alt = (::GetKeyState(VK_MENU) < 0);
			arg.shift = pmdec.mouse.button.shift;
			arg.ctrl = pmdec.mouse.button.ctrl;
			arg.left_button = pmdec.mouse.button.left;
			arg.mid_button = pmdec.mouse.button.middle;
			arg.right_button = pmdec.mouse.button.right;
		}
	}

	void assign_arg(arg_wheel& arg, window* wd, const parameter_decoder& pmdec)
	{
		arg.window_handle = reinterpret_cast<native_window_type>(wd);
		arg.evt_code = event_code_mouse_wheel;

		POINT point = { pmdec.mouse.x, pmdec.mouse.y };
		::ScreenToClient(reinterpret_cast<HWND>(wd->root), &point);

		arg.upwards = (pmdec.mouse.button.wheel_delta >= 0);
		arg.distance = static_cast<unsigned>(arg.upwards ? pmdec.mouse.button.wheel_delta : -pmdec.mouse.button.wheel_delta);

		arg.pos.x = static_cast<int>(point.x) - wd->pos_root.x;
		arg.pos.y = static_cast<int>(point.y) - wd->pos_root.y;
		arg.left_button = pmdec.mouse.button.left;
		arg.mid_button = pmdec.mouse.button.middle;
		arg.right_button = pmdec.mouse.button.right;
		arg.ctrl = pmdec.mouse.button.ctrl;
		arg.shift = pmdec.mouse.button.shift;
	}

	void notifications_window_proc(HWND wd, WPARAM wparam, LPARAM lparam)
	{
#if 0
		arg_notifier arg = {};
		switch (lparam)
		{
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			arg.evt_code = event_code_dbl_click;
			arg.left_button = (lparam == WM_LBUTTONDBLCLK);
			arg.mid_button = (lparam == WM_MBUTTONDBLCLK);
			arg.right_button = (lparam == WM_RBUTTONDBLCLK);
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			arg.evt_code = event_code_mouse_down;
			arg.left_button = (lparam == WM_LBUTTONDOWN);
			arg.mid_button = (lparam == WM_MBUTTONDOWN);
			arg.right_button = (lparam == WM_RBUTTONDOWN);
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			arg.evt_code = event_code_mouse_up;
			arg.left_button = (lparam == WM_LBUTTONUP);
			arg.mid_button = (lparam == WM_MBUTTONUP);
			arg.right_button = (lparam == WM_RBUTTONUP);
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSELEAVE:
			arg.evt_code = (WM_MOUSEMOVE == lparam ? event_code_mouse_move : event_code_mouse_leave);
			arg.left_button = false;
			arg.mid_button = false;
			arg.right_button = false;
			break;
		default:
			return;
		}

		::POINT pos;
		::GetCursorPos(&pos);
		arg.pos.x = pos.x;
		arg.pos.y = pos.y;

		notifications::instance().emit(reinterpret_cast<native_window_type>(wd), static_cast<unsigned short>(wparam), arg);
#endif
    return ;
	}
	//trivial_message
	//	The Windows messaging always sends a message to the native_window_type thread queue when the calling is in other thread.
	//If messages can be finished without expecting Nana's native_window_type manager, the trivail_message function would
	//handle those messages. This is a method to avoid a deadlock, that calling waits for the handling and they require
	//Nana's native_window_type manager.
	bool trivial_message(HWND wd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT & ret)
	{
    bedrock *bed= bedrock_instance();
		switch(msg)
		{
		case messages::async_activate:
			::EnableWindow(wd, true);
			::SetActiveWindow(wd);
			return true;
		case messages::async_set_focus:
			::SetFocus(wd);
			return true;
		case messages::operate_caret:
			//Refer to basis.hpp for this specification.
			switch(wParam)
			{
			case 1: //Delete
				::DestroyCaret();
				break;
			case 2: //SetPos
				::SetCaretPos(reinterpret_cast<messages::caret*>(lParam)->x, reinterpret_cast<messages::caret*>(lParam)->y);
				delete reinterpret_cast<messages::caret*>(lParam);
				break;
			}
			return true;
		case messages::remote_thread_set_window_pos:
			::SetWindowPos(wd, reinterpret_cast<HWND>(wParam), 0, 0, 0, 0, static_cast<UINT>(lParam));
			return true;
		case messages::remote_thread_set_window_text:
			::SetWindowTextW(wd, reinterpret_cast<wchar_t*>(wParam));
			delete [] reinterpret_cast<wchar_t*>(wParam);
			return true;
		case messages::tray:
			notifications_window_proc(wd, wParam, lParam);
			return true;
		default:
			break;
		}

		switch(msg)
		{
		case WM_DESTROY:
		case WM_SHOWWINDOW:
		case WM_SIZING:
		case WM_MOVE:
		case WM_SIZE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_PAINT:
		case WM_CLOSE:
		case WM_MOUSEACTIVATE:
		case WM_GETMINMAXINFO:
		case WM_WINDOWPOSCHANGED:
		case WM_NCDESTROY:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_IME_STARTCOMPOSITION:
		case WM_DROPFILES:
		case WM_MOUSELEAVE:
		case WM_MOUSEWHEEL:	//The WM_MOUSELAST may not include the WM_MOUSEWHEEL/WM_MOUSEHWHEEL when the version of SDK is low.
		case WM_MOUSEHWHEEL:
			return false;
		default:
			if((WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST) || (WM_KEYFIRST <= msg && msg <= WM_KEYLAST))
				return false;
		}

		ret = ::DefWindowProc(wd, msg, wParam, lParam);
		return true;
	}

	void adjust_sizing(window* wd, ::RECT * const r, int edge, unsigned req_width, unsigned req_height)
	{
		unsigned width = static_cast<unsigned>(r->right - r->left) - wd->extra_width;
		unsigned height = static_cast<unsigned>(r->bottom - r->top) - wd->extra_height;

		if(wd->max_track_size.width && (wd->max_track_size.width < req_width))
			req_width = wd->max_track_size.width;
		else if(wd->min_track_size.width && (wd->min_track_size.width > req_width))
			req_width = wd->min_track_size.width;

		if(wd->max_track_size.height && (wd->max_track_size.height < req_height))
			req_height = wd->max_track_size.height;
		else if(wd->min_track_size.height && (wd->min_track_size.height > req_height))
			req_height = wd->min_track_size.height;

		if(req_width != width)
		{
			switch(edge)
			{
			case WMSZ_LEFT:
			case WMSZ_BOTTOMLEFT:
			case WMSZ_TOPLEFT:
				r->left = r->right - static_cast<int>(req_width) - wd->extra_width;
				break;
			case WMSZ_RIGHT:
			case WMSZ_BOTTOMRIGHT:
			case WMSZ_TOPRIGHT:
			case WMSZ_TOP:
			case WMSZ_BOTTOM:
				r->right = r->left + static_cast<int>(req_width) + wd->extra_width;
				break;
			}
		}

		if(req_height != height)
		{
			switch(edge)
			{
			case WMSZ_TOP:
			case WMSZ_TOPLEFT:
			case WMSZ_TOPRIGHT:
				r->top = r->bottom - static_cast<int>(req_height) - wd->extra_height;
				break;
			case WMSZ_BOTTOM:
			case WMSZ_BOTTOMLEFT:
			case WMSZ_BOTTOMRIGHT:
			case WMSZ_LEFT:
			case WMSZ_RIGHT:
				r->bottom = r->top + static_cast<int>(req_height) + wd->extra_height;
				break;
			}
		}
	}

	//Translate OS Virtual-Key into ASCII code
	wchar_t translate_virtual_key(WPARAM vkey)
	{
		switch (vkey)
		{
		case VK_DELETE:
			return 127;
		case VK_DECIMAL:
			return 46;
		}
		return static_cast<wchar_t>(vkey);
	}

static window* GetWindowByHWND(HWND hwnd)
{
  bedrock* bed = bedrock_instance();
  window* wd = (window*) GetWindowLongPtrA(hwnd, GWLP_USERDATA);
  return bed->hg_windows != 0 ? wd : 0;
}

LRESULT CALLBACK Bedrock_WIN32_WindowProc(HWND root_window, UINT message, WPARAM wParam, LPARAM lParam)
{
  LRESULT window_proc_value = 0;
  if(trivial_message(root_window, message, wParam, lParam, window_proc_value))
    return window_proc_value;
  
  static TRACKMOUSEEVENT track = {sizeof track, 0x00000002};
  native_window_type const native_window = reinterpret_cast<native_window_type>(root_window);
  window* msgwnd = GetWindowByHWND(root_window);
  
  if(msgwnd)
  {
    parameter_decoder pmdec;
    pmdec.raw_param.lparam = lParam;
    pmdec.raw_param.wparam = wParam;
    
    switch (message)
    {
    case WM_IME_STARTCOMPOSITION:
      if (1)
      {
        HFONT native_font = 0;
        LOGFONTW logfont;
        ::GetObjectW(reinterpret_cast<HFONT>(native_font), sizeof logfont, &logfont);
        
        HIMC imc = imm_get_context(root_window);
        imm_set_composition_font(imc, &logfont);
        
        POINT pos;
        ::GetCaretPos(&pos);
        
        COMPOSITIONFORM cf = { CFS_POINT };
        cf.ptCurrentPos = pos;
        imm_set_composition_window(imc, &cf);
        imm_release_context(root_window, imc);
      }
      break;
    case WM_GETMINMAXINFO:
      {
        bool take_over = false;
        MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
        
        if (!msgwnd->min_track_size.empty())
        {
          mmi->ptMinTrackSize.x = static_cast<LONG>(msgwnd->min_track_size.width + msgwnd->extra_width);
          mmi->ptMinTrackSize.y = static_cast<LONG>(msgwnd->min_track_size.height + msgwnd->extra_height);
          take_over = true;
        }
        
        if (false == msgwnd->flags.fullscreen)
        {
          if (msgwnd->max_track_size.width && msgwnd->max_track_size.height)
          {
            mmi->ptMaxTrackSize.x = static_cast<LONG>(msgwnd->max_track_size.width + msgwnd->extra_width);
            mmi->ptMaxTrackSize.y = static_cast<LONG>(msgwnd->max_track_size.height + msgwnd->extra_height);
            if (mmi->ptMaxSize.x > mmi->ptMaxTrackSize.x)
              mmi->ptMaxSize.x = mmi->ptMaxTrackSize.x;
            if (mmi->ptMaxSize.y > mmi->ptMaxTrackSize.y)
              mmi->ptMaxSize.y = mmi->ptMaxTrackSize.y;
            
            take_over = true;
          }
        }
        
        return 0;
      }
      break;
    case WM_SHOWWINDOW:
      break;
    case WM_WINDOWPOSCHANGED:
      break;
    case WM_SETFOCUS:
      break;
    case WM_KILLFOCUS:
      break;
    case WM_MOUSEACTIVATE:
      break;
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
      break;
    case WM_NCLBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
      break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
      break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
      break;
    case WM_MOUSEMOVE:
      break;
    case WM_MOUSELEAVE:
      break;
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
      break;
    case WM_DROPFILES:
      {
        HDROP drop = reinterpret_cast<HDROP>(wParam);
        POINT mswin_pos;
        ::DragQueryPoint(drop, &mswin_pos);
        
#if 0
        const point pos(mswin_pos.x, mswin_pos.y);
        arg_dropfiles dropfiles;
        
        unique_ptr<wchar_t[]> varbuf;
        size_t bufsize = 0;
        
        unsigned size = ::DragQueryFile(drop, 0xFFFFFFFF, 0, 0);
        for(unsigned i = 0; i < size; ++i)
        {
          unsigned reqlen = ::DragQueryFile(drop, i, 0, 0) + 1;
          if(bufsize < reqlen)
          {
            varbuf.reset(new wchar_t[reqlen]);
            bufsize = reqlen;
          }
          
          ::DragQueryFile(drop, i, varbuf.get(), reqlen);
          
          dropfiles.files.emplace_back(to_utf8(varbuf.get()));
        }
        
        while(msgwnd && (msgwnd->flags.dropable == false))
          msgwnd = msgwnd->parent;
        
        if(msgwnd)
        {
          dropfiles.pos = pos;
          
          wd_manager.calc_window_point(msgwnd, dropfiles.pos);
          dropfiles.window_handle = reinterpret_cast<native_window_type>(msgwnd);
          
          msgwnd->annex.events_ptr->mouse_dropfiles.emit(dropfiles, reinterpret_cast<native_window_type>(msgwnd));
          wd_manager.do_lazy_refresh(msgwnd, false);
        }
        
        
#endif
        ::DragFinish(drop);
      }
      break;
    case WM_SIZING:
      {
        ::RECT* const r = reinterpret_cast<RECT*>(lParam);
        unsigned width = static_cast<unsigned>(r->right - r->left) - msgwnd->extra_width;
        unsigned height = static_cast<unsigned>(r->bottom - r->top) - msgwnd->extra_height;
        
        if(msgwnd->max_track_size.width || msgwnd->min_track_size.width)
        {
          if(wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_TOPLEFT)
          {
            if(msgwnd->max_track_size.width && (width > msgwnd->max_track_size.width))
              r->left = r->right - static_cast<int>(msgwnd->max_track_size.width) - msgwnd->extra_width;
            if(msgwnd->min_track_size.width && (width < msgwnd->min_track_size.width))
              r->left = r->right - static_cast<int>(msgwnd->min_track_size.width) - msgwnd->extra_width;
          }
          else if(wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT)
          {
            if(msgwnd->max_track_size.width && (width > msgwnd->max_track_size.width))
              r->right = r->left + static_cast<int>(msgwnd->max_track_size.width) + msgwnd->extra_width;
            if(msgwnd->min_track_size.width && (width < msgwnd->min_track_size.width))
              r->right = r->left + static_cast<int>(msgwnd->min_track_size.width) + msgwnd->extra_width;
          }
        }
        
        if(msgwnd->max_track_size.height || msgwnd->min_track_size.height)
        {
          if(wParam == WMSZ_TOP || wParam == WMSZ_TOPLEFT || wParam == WMSZ_TOPRIGHT)
          {
            if(msgwnd->max_track_size.height && (height > msgwnd->max_track_size.height))
              r->top = r->bottom - static_cast<int>(msgwnd->max_track_size.height) - msgwnd->extra_height;
            if(msgwnd->min_track_size.height && (height < msgwnd->min_track_size.height))
              r->top = r->bottom - static_cast<int>(msgwnd->min_track_size.height) - msgwnd->extra_height;
          }
          else if(wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_BOTTOMRIGHT)
          {
            if(msgwnd->max_track_size.height && (height > msgwnd->max_track_size.height))
              r->bottom = r->top + static_cast<int>(msgwnd->max_track_size.height) + msgwnd->extra_height;
            if(msgwnd->min_track_size.height && (height < msgwnd->min_track_size.height))
              r->bottom = r->top + static_cast<int>(msgwnd->min_track_size.height) + msgwnd->extra_height;
          }
        }
        
        size size_before(	static_cast<unsigned>(r->right - r->left - msgwnd->extra_width),
										static_cast<unsigned>(r->bottom - r->top - msgwnd->extra_height));
        
        arg_resizing arg;
        arg.window_handle = reinterpret_cast<native_window_type>(msgwnd);
        arg.width = size_before.width;
        arg.height = size_before.height;
        
        switch (wParam)
        {
        case WMSZ_LEFT:
          arg.border = window_border_left;		break;
        case WMSZ_RIGHT:
          arg.border = window_border_right;	break;
        case WMSZ_BOTTOM:
          arg.border = window_border_bottom;	break;
        case WMSZ_BOTTOMLEFT:
          arg.border = window_border_bottom_left;	break;
        case WMSZ_BOTTOMRIGHT:
          arg.border = window_border_bottom_right;	break;
        case WMSZ_TOP:
          arg.border = window_border_top;	break;
        case WMSZ_TOPLEFT:
          arg.border = window_border_top_left;	break;
        case WMSZ_TOPRIGHT:
          arg.border = window_border_top_right;	break;
        }
        
        //brock.emit(event_code_resizing, msgwnd, arg, false, &context);
        
        if (arg.width != width || arg.height != height)
        {
          adjust_sizing(msgwnd, r, static_cast<int>(wParam), arg.width, arg.height);
          return TRUE;
        }
      }
      break;
    case WM_SIZE:
      if(wParam != SIZE_MINIMIZED) {
        //size(msgwnd, size(pmdec.size.width, pmdec.size.height), true, true);
      }
      break;
    case WM_MOVE:
      //brock.event_move(msgwnd, (int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam));
      break;
    case WM_PAINT:
      {
        ::PAINTSTRUCT ps;
        ::BeginPaint(root_window, &ps);
        {
          //Don't copy root_graph to the native_window_type directly, otherwise the edge nimbus effect will be missed.
          rectangle update_area(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
          if (!update_area.empty())
            msgwnd->paint((native_window_type)(msgwnd), (native_graphics_type)ps.hdc, &update_area);
        }
        ::EndPaint(root_window, &ps);
        return 0;
      }
      break;
    case WM_SYSCHAR:
      break;
    case WM_SYSKEYDOWN:
      break;
    case WM_SYSKEYUP:
#if 0
      def_window_proc = true;
      if (brock.shortkey_occurred(false) == false)
      {
        msgwnd = msgwnd->root_widget->other.attribute.root->menubar;
        if(msgwnd)
        {
          //Don't call default native_window_type proc to avoid popuping system menu.
          def_window_proc = false;
          
          bool set_focus = (brock.focus() != msgwnd) && (!msgwnd->root_widget->flags.ignore_menubar_focus);
          if (set_focus)
            wd_manager.set_focus(msgwnd, false, arg_focus::reason::general);
          
          arg_keyboard arg;
          arg.evt_code = event_code_key_release;
          arg.window_handle = reinterpret_cast<native_window_type>(msgwnd);
          arg.ignore = false;
          arg.key = static_cast<wchar_t>(wParam);
          brock.get_key_state(arg);
          brock.emit(event_code_key_release, msgwnd, arg, true, &context);
          
          if (!set_focus)
          {
            brock.set_menubar_taken(NULL);
            msgwnd->root_widget->flags.ignore_menubar_focus = false;
          }
        }
      }
#endif
      break;
    case WM_KEYDOWN:
      break;
    case WM_CHAR:
      return 0;
    case WM_KEYUP:
      if(wParam != VK_MENU) //MUST NOT BE AN ALT
      {
      }
      break;
    case WM_CLOSE:
      {
      }
      break;
    case WM_DESTROY:
      //if (msgwnd->root == brock.get_menu())
      {
        //brock.erase_menu(false);
        //brock.delay_restore(3);	//Restores if delay_restore not decleared
      }
      //wd_manager.destroy(msgwnd);
      //platform_spec::instance().release_window_icon(msgwnd->root);
      break;
    case WM_NCDESTROY:
      
      break;
    default:
      break;
    }
  }
  
  return ::DefWindowProc(root_window, message, wParam, lParam);
}

void get_key_state(arg_keyboard& kb)
{
  kb.ctrl = (0 != (::GetKeyState(VK_CONTROL) & 0x80));
  kb.shift = (0 != (::GetKeyState(VK_SHIFT) & 0x80));
}

const wchar_t* translate(cursor id)
{
  const wchar_t* name = IDC_ARROW;
  
  switch(id)
  {
  case cursor_arrow:
    name = IDC_ARROW;	break;
  case cursor_wait:
    name = IDC_WAIT;	break;
  case cursor_hand:
    name = IDC_HAND;	break;
  case cursor_size_we:
    name = IDC_SIZEWE;	break;
  case cursor_size_ns:
    name = IDC_SIZENS;	break;
  case cursor_size_bottom_left:
  case cursor_size_top_right:
    name = IDC_SIZENESW;	break;
  case cursor_size_top_left:
  case cursor_size_bottom_right:
    name = IDC_SIZENWSE;	break;
  case cursor_iterm:
    name = IDC_IBEAM;	break;
  }
  return name;
}

//Dynamically set a cursor for a native_window_type
void set_cursor(native_window_handle_impl* wd, cursor cur)
{
#if 0
  HCURSOR this_cur = reinterpret_cast<HCURSOR>(
#ifdef _WIN64
    ::GetClassLongPtr(reinterpret_cast<HWND>(wd), GCLP_HCURSOR)
#else
    ::GetClassLong(reinterpret_cast<HWND>(wd), GCL_HCURSOR)
#endif
    );
  
  if(this_cur != thrd->cursor.handle)
  {
#ifdef _WIN64
    ::SetClassLongPtr(reinterpret_cast<HWND>(wd), GCLP_HCURSOR,
      reinterpret_cast<LONG_PTR>(thrd->cursor.handle));
#else
    ::SetClassLong(reinterpret_cast<HWND>(wd), GCL_HCURSOR,
      static_cast<unsigned long>(reinterpret_cast<size_t>(thrd->cursor.handle)));
#endif
  }
#endif
}

void define_state_cursor(native_window_handle_impl* wd, cursor cur)
{
  set_cursor(wd, cur);
  HCURSOR cur_handle = ::LoadCursor(NULL, translate(cur));
  ::SetCursor(cur_handle);
  ::ShowCursor(TRUE);
}

void undefine_state_cursor(native_window_handle_impl * wd)
{
#if 0
  HCURSOR rev_handle = ::LoadCursor(NULL, IDC_ARROW);
  if (!wd_manager().available(wd))
  {
    ::ShowCursor(FALSE);
    ::SetCursor(rev_handle);
    return;
  }
  
  wd->root_widget->other.attribute.root->state_cursor = cursor_arrow;
  wd->root_widget->other.attribute.root->state_cursor_window = NULL;
  
  auto pos = native_interface::cursor_position();
  auto native_handle = native_interface::find_window(pos.x, pos.y);
  
  if (!native_handle)
  {
    ::ShowCursor(FALSE);
    ::SetCursor(rev_handle);
    return;
  }
  
  native_interface::calc_window_point(native_handle, pos);
  auto rev_wd = wd_manager().find_window(native_handle, pos);
  if (rev_wd)
  {
    set_cursor(rev_wd, rev_wd->predef_cursor, thrd);
    rev_handle = thrd->cursor.handle;
  }
  ::ShowCursor(FALSE);
  ::SetCursor(rev_handle);
#endif
}

int wait_event(int delay)
{
  int time0 = GetTickCount();
  bedrock* bed = bedrock_instance();
  
  for (;;) {
    MSG message;
    window* win = NULL;
    int is_processed = 0;
    
    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay) || bed->hg_windows == 0) {
      return -1;
    }
    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }
    if (bed->hg_windows) {
      window* w = bed->hg_windows;
      native_window_type cur_win = (native_window_type)(message.hwnd);
      for (; w != 0 && is_processed == 0; w = w->next) {
        if (w->root == cur_win) {
          is_processed = 1;
          win = w;
        }
      }
    }
    
    if (win) {
      switch (message.message) {
      case WM_DESTROY:
      case WM_CHAR:
        DispatchMessage(&message);
        return (int) message.wParam;
      case WM_SYSKEYDOWN:
        if (message.wParam == VK_F10) {
          is_processed = 1;
          return (int)(message.wParam << 16);
        }
        break;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
        if (1) {
          TranslateMessage(&message);
          DispatchMessage(&message);
          return (int)(message.wParam << 16);
        } else {
          DispatchMessage(&message);
          is_processed = 1;
        }
        break;
      case WM_KEYDOWN:
        TranslateMessage(&message);
        if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
          message.wParam == VK_HOME || message.wParam == VK_END ||
          message.wParam == VK_UP || message.wParam == VK_DOWN ||
          message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
          message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
          message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
          DispatchMessage(&message);
          is_processed = 1;
          return (int)(message.wParam << 16);
        }
      default:
        DispatchMessage(&message);
        is_processed = 1;
        break;
      }
    }
    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }
  return 0;
}

#endif //NANA_WINDOWS
