
#include "string.inl"

enum checkstate
{
  unchecked, checked, partial
};

enum window_border
{
  window_border_none,
    window_border_left, window_border_right, window_border_top, window_border_bottom,
    window_border_top_left, window_border_top_right, window_border_bottom_left, window_border_bottom_right
};

enum bground_mode
{
  bground_mode_none,
    bground_mode_basic,
    bground_mode_blend
};

enum flags
{
		flags_super,
      flags_widget = 0x1,
      flags_lite_widget = 0x3,
      flags_root = 0x5
#ifndef WIDGET_FRAME_DEPRECATED
      ,flags_frame = 0x9
#endif
};
//wait for constexpr
struct widget_tag { enum { value = flags_widget }; };
struct lite_widget_tag : public widget_tag { enum { value = flags_lite_widget };  };
struct root_tag : public widget_tag{ enum { value = flags_root };  };
#ifndef WIDGET_FRAME_DEPRECATED
struct frame_tag : public widget_tag{ enum { value = flags_frame }; };
#endif

struct native_window_handle_impl{};
struct event_handle_impl{};
struct native_drawable_impl{};
struct drawable_impl{};
struct native_font_impl{};
struct native_icon_impl{};
struct native_graphics_impl{};

typedef native_window_handle_impl* native_window_type;
typedef native_graphics_impl* native_graphics_type;
typedef event_handle_impl* event_handle;
typedef native_drawable_impl* native_drawable_type;
typedef drawable_impl* drawable_type;
typedef native_font_impl* native_font_type;
typedef native_icon_impl* native_icon_type;

enum event_code
{
  event_code_click,				///< A mouse click event.
    event_code_dbl_click,			///< A mouse double click event.
    event_code_mouse_enter,		///< A mouse enters a widget.
    event_code_mouse_move,			///< A mouse moves over a widget.
    event_code_mouse_leave,		///< A mouse leaves a widget.
    event_code_mouse_down,			///< A mouse button is pressed on a widget.
    event_code_mouse_up,			///< A mouse button is released on a widget.
    event_code_mouse_wheel,		///< A mouse scrolls the wheel on a widget.
    event_code_mouse_drop,			///< A mouse release over a native_window_type that is registered as recipient of drag and drop.
    event_code_expose,				///< 
    event_code_resizing, 			///< A widget's size is sizing. In this event, A widget's size can bed with a new size.
    event_code_resized,			///< A widget's size is changing.
    event_code_move,				///< 
    event_code_unload,				///< A form is closed by clicking the X button, only works for root widget.
    event_code_destroy,			///< A widget is about to be destroyed.
    event_code_focus,				///< A widget's focus is changed.
    event_code_key_press,			///< A keyboard is pressed on a focus widget.
    event_code_key_char,			///< The focus widget received a character.
    event_code_key_release,		///< A keyboard is released on a focus widget.
    event_code_shortkey,			///< The widgets received a shortkey message.
    event_code_elapse,				///< Unoperational events? . A widget received a tick that is sended by timer.
    event_code_end					///< End indicator, it's not an event.
};

struct keyboard
{
  enum{
    //Control Code for ASCII
    keyboard_start_of_headline = 0x1,	//Ctrl+A
      keyboard_end_of_text = 0x3,	//Ctrl+C
      keyboard_backspace = 0x8,
      keyboard_tab = 0x9,
      keyboard_alt = 0x12,
      keyboard_enter_n = 0xA,
      keyboard_enter = 0xD,
      keyboard_enter_r = 0xD,
      keyboard_sync_idel = 0x16,	//Ctrl+V
      keyboard_cancel = 0x18,	//Ctrl+X
      keyboard_end_of_medium = 0x19,	//Ctrl+Y
      keyboard_substitute = 0x1A,	//Ctrl+Z
      keyboard_escape = 0x1B,
      keyboard_space = 0x20,	//Space
      keyboard_del = 0x7F,		//Delete
      keyboard_os_del = keyboard_del,	//Deprecated
      
      //The following names are intuitive name of ASCII control codes
      keyboard_select_all = keyboard_start_of_headline,
      keyboard_copy = keyboard_end_of_text,
      keyboard_paste = keyboard_sync_idel,
      keyboard_cut = keyboard_cancel,
      keyboard_redo = keyboard_end_of_medium,
      keyboard_undo = keyboard_substitute,
      
      //System Code for OS
      keyboard_os_tab = 0x09,
      keyboard_os_shift = 0x10,
      keyboard_os_ctrl = 0x11,
      keyboard_os_pageup = 0x21,
      keyboard_os_pagedown,
      keyboard_os_arrow_left = 0x25,
      keyboard_os_arrow_up,
      keyboard_os_arrow_right,
      keyboard_os_arrow_down,
      keyboard_os_insert = 0x2D,
      keyboard_os_end = 0x23,
      keyboard_os_home //Pos 1
  };
};

enum cursor
{
  cursor_hand	= 60,     ///< displays a hand to indicate a text or an element is clickable
    cursor_arrow	= 68,     ///< the default shape
    cursor_wait	= 150,    ///< indicates the system is currently busy
    cursor_iterm	= 152,    ///< A text caret. Displays a caret to indicate the UI is input able
    cursor_size_we	= 108,
    cursor_size_ns	= 116,
    cursor_size_top_left = 134,
    cursor_size_top_right = 136,
    cursor_size_bottom_left = 12,
    cursor_size_bottom_right = 14
};

enum mouse
{
  mouse_any_button, 
    mouse_left_button, 
    mouse_middle_button, 
    mouse_right_button
};

enum z_order_action
{
  z_order_action_none, 
    z_order_action_bottom,       ///< brings a native_window_type at the bottom of z-order.
    z_order_action_top,          ///< brings a widget at the top of the z-order.
    z_order_action_topmost,      ///< brings a native_window_type at the top of the z-order and stays here.
    z_order_action_foreground    ///< brings a native_window_type to the foreground.
};

/// Window appearance structure defined to specify the appearance of a form
struct appearance
{
  bool taskbar;
  bool floating;
  bool no_activate;
  
  bool minimize;
  bool maximize;
  bool sizable;
  
  bool decoration;
  
  appearance::appearance()
    :taskbar(true), floating(false), no_activate(false),
    minimize(true), maximize(true), sizable(true),
    decoration(true)
  {}
  
  appearance::appearance(bool has_decorate, bool taskbar, bool is_float, bool no_activate, bool min, bool max, bool sizable)
    :	taskbar(taskbar), floating(is_float), no_activate(no_activate),
    minimize(min), maximize(max), sizable(sizable),
    decoration(has_decorate)
  {}
};

inline unsigned make_rgb(unsigned char red, unsigned char green, unsigned char blue)
{
		return ((unsigned(red) << 16)|((unsigned(green)<<8))|blue);
}

#define NANA_RGB(a)	(((DWORD)(a) & 0xFF)<<16) |  ((DWORD)(a) & 0xFF00) | (((DWORD)(a) & 0xFF0000) >> 16 )


typedef unsigned color;
/// A constant value for the invalid position.
const size_t npos = static_cast<size_t>(-1);

enum mouse_action
{
  mouse_action_begin,
    mouse_action_normal = mouse_action_begin,
    mouse_action_normal_captured,
    mouse_action_hovered,
    mouse_action_pressed,
    mouse_action_end
};

enum element_state
{
  element_state_normal,
    element_state_hovered,
    element_state_focus_normal,
    element_state_focus_hovered,
    element_state_pressed,
    element_state_disabled
};

union pixel_argb_t
{
  struct element_tag
  {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha_channel;
  }element;
  unsigned value;
};

union pixel_rgba_t
{
  struct element_tag
  {
    unsigned char alpha_channel;
    unsigned char blue;
    unsigned char green;
    unsigned char red;
  }element;
  unsigned value;
};

typedef pixel_argb_t pixel_color_t;

/// See extended CSS color keywords (4.3) in http://www.w3.org/TR/2011/REC-css3-color-20110607/
enum colors
{
  colors_alice_blue = 0xf0f8ff,
    colors_antique_white = 0xfaebd7,
    colors_aqua	= 0xFFFF,
    colors_aquamarine = 0x7fffd4,
    colors_azure	= 0xf0ffff,
    colors_beige	= 0xf5f5dc,
    colors_bisque	= 0xffe4ce,
    colors_black	= 0x0,
    colors_blanched_almond = 0xffebcd,
    colors_blue	= 0x0000FF,
    colors_blue_violet = 0x8a2be2,
    colors_brown	= 0xa52a2a,
    colors_burly_wood = 0xdeb887,
    colors_cadet_blue = 0x5f9ea0,
    colors_chartreuse = 0x7fff00,
    colors_chocolate = 0xd2691e,
    colors_coral = 0xff7f50,
    colors_cornflower_blue = 0x6495ed,
    colors_cornsilk = 0xfff8dc,
    colors_crimson	= 0xdc143c,
    colors_cyan	= 0xffff,
    colors_dark_blue = 0x8b,
    colors_dark_cyan = 0x8b8b,
    colors_dark_goldenrod = 0xb8860b,
    colors_dark_gray = 0xa9a9a9,
    colors_dark_green = 0x6400,
    colors_dark_grey = colors_dark_gray,
    colors_dark_khaki = 0xbdb76b,
    colors_dark_magenta = 0x8b008b,
    colors_dark_olive_green = 0x556b2f,
    colors_colors_dark_orange = 0xff8c00,
    colors_dark_orchid = 0x9932cc,
    colors_dark_red = 0x8b0000,
    colors_dark_salmon = 0xe9976a,
    colors_dark_sea_green = 0x8fbc8f,
    colors_dark_slate_blue = 0x483d8b,
    colors_dark_slate_gray = 0x2f4f4f,
    colors_dark_slate_grey = 0x2f4f4f,
    colors_dark_turquoise = 0xced1,
    colors_dark_violet = 0x9400d3,
    colors_deep_pink = 0xff1493,
    colors_deep_sky_blue = 0xbfff,
    colors_dim_gray = 0x696969,
    colors_dim_grey = colors_dim_gray,
    colors_dodger_blue = 0x1e90ff,
    colors_firebrick = 0xb22222,
    colors_floral_white = 0xfffaf0,
    colors_forest_green = 0x228b22,
    colors_fuchsia	= 0xFF00FF,
    colors_gainsboro = 0xdcdcdc,
    colors_ghost_white = 0xf8f8ff,
    colors_gold = 0xffd700,
    colors_goldenrod = 0xdaa520,
    colors_gray = 0x808080,
    colors_green = 0x008000,
    colors_green_yellow = 0xadff2f,
    colors_grey = colors_gray,
    colors_honeydew = 0xf0fff0,
    colors_hot_pink = 0xff69b4,
    colors_indian_red = 0xcd5c5c,
    colors_indigo	= 0x4b0082,
    colors_ivory = 0xfffff0,
    colors_khaki = 0xf0e68c,
    colors_lavendar = 0xe6e6fa,
    colors_lavender_blush = 0xfff0f5,
    colors_lawn_green = 0x7cfc00,
    colors_lemon_chiffon = 0xfffacd,
    colors_light_blue = 0xadd8e6,
    colors_light_coral = 0xf08080,
    colors_light_cyan = 0xe0ffff,
    colors_light_goldenrod_yellow = 0xfafad2,
    colors_light_gray = 0xd3d3d3,
    colors_light_green = 0x90ee90,
    colors_light_grey = colors_light_gray,
    colors_light_pink = 0xffb6c1,
    colors_light_salmon = 0xffa07a,
    colors_light_sea_green = 0x20b2aa,
    colors_light_sky_blue = 0x87cefa,
    colors_light_slate_gray = 0x778899,
    colors_light_slate_grey = colors_light_slate_gray,
    colors_light_steel_blue = 0xb0c4de,
    colors_light_yellow = 0xffffe0,
    colors_lime	= 0x00FF00,
    colors_lime_green = 0x32cd32,
    colors_linen = 0xfaf0e6,
    colors_magenta = 0xff00ff,
    colors_maroon	= 0x800000,
    colors_medium_aquamarine = 0x66cdaa,
    colors_medium_blue = 0xcd,
    colors_medium_orchid = 0xba55d3,
    colors_medium_purple = 0x9370db,
    colors_medium_sea_green = 0x3cb371,
    colors_medium_slate_blue = 0x7b68ee,
    colors_medium_spring_green = 0xfa9a,
    colors_medium_turquoise = 0x48d1cc,
    colors_medium_violet_red = 0xc71585,
    colors_midnight_blue = 0x191970,
    colors_mint_cream = 0xf5fffa,
    colors_misty_rose = 0xffe4e1,
    colors_moccasin = 0xffe4b5,
    colors_navajo_white = 0xffdead,
    colors_navy	= 0x000080,
    colors_old_lace = 0xfdf5e6,
    colors_olive	= 0x808000,
    colors_olive_drab = 0x6b8e23,
    colors_orange	= 0xffa500,
    colors_orange_red = 0xff4500,
    colors_orchid	= 0xda70d6,
    colors_pale_goldenrod = 0xeee8aa,
    colors_pale_green	= 0x98fb98,
    colors_pale_turquoise = 0xafeeee,
    colors_pale_violet_red = 0xdb7093,
    colors_papaya_whip = 0xffefd5,
    colors_peach_puff = 0xffdab9,
    colors_peru	= 0xcd853f,
    colors_pink	= 0xffc0cb,
    colors_plum	= 0xdda0dd,
    colors_powder_blue = 0xb0e0e6,
    colors_purple	= 0x800080,
    colors_red		= 0xFF0000,
    colors_rosy_brown = 0xbc8f8f,
    colors_royal_blue = 0x4169e1,
    colors_saddle_brown = 0x8b4513,
    colors_salmon = 0xfa8072,
    colors_sandy_brown = 0xf4a460,
    colors_sea_green = 0x2e8b57,
    colors_sea_shell = 0xfff5ee,
    colors_sienna	= 0xa0522d,
    colors_silver	= 0xc0c0c0,
    colors_sky_blue = 0x87ceeb,
    colors_slate_blue = 0x6a5acd,
    colors_slate_gray = 0x708090,
    colors_slate_grey = 0x708090,
    colors_snow	= 0xfffafa,
    colors_spring_green = 0xff7f,
    colors_steel_blue = 0x4682b4,
    colors_tan		= 0xd2b48c,
    colors_teal	= 0x008080,
    colors_thistle	= 0xd8bfd8,
    colors_tomato	= 0xff6347,
    colors_turquoise = 0x40e0d0,
    colors_violet	= 0xee82ee,
    colors_wheat	= 0xf5deb3,
    colors_white	= 0xFFFFFF,
    colors_white_smoke = 0xf5f5f5,
    colors_yellow	= 0xFFFF00,
    colors_yellow_green = 0x9acd32,
    
    //temporary defintions, these will be replaced by color schema
    colors_button_face_shadow_start = 0xF5F4F2,
    colors_button_face_shadow_end = 0xD5D2CA,
    colors_button_face = 0xD4D0C8 , //,light_cyan
    colors_dark_border = 0x404040,
    colors_gray_border = 0x808080,
    colors_highlight = 0x1CC4F7
};

enum arrange
{
  arrange_unknown,
    arrange_horizontal,
    arrange_vertical,
    arrange_horizontal_vertical
};

///The definition of horizontal alignment
enum align
{
  align_left,
    align_center,
    align_right
};

///The definition of vertical alignment
enum align_v
{
  align_top,
    align_centerv,
    align_bottom
};

///The definition of the four corners of the world
enum direction
{
  direction_north,
    direction_south,
    direction_east,
    direction_west,
    direction_southeast
};



#if 0

//Some helper types to identify an integer as color.
struct color_rgb { unsigned c; };
struct color_argb { unsigned c; };
struct color_rgba { unsigned c; };

class color
{
private:
  double r_;
  double g_;
  double b_;
  double a_;	//invisible
public:
  color() {
    r_ = 0.0;
    g_ = 0.0;
    b_ = 0.0;
    a_ = 0.0;	//invisible
  }
  typedef color self;
  self& init(colors) {
    return init((static_cast<unsigned>(clr)& 0xFF0000) >> 16, (static_cast<unsigned>(clr)& 0xFF00) >> 8, static_cast<unsigned>(clr)& 0xFF);
  }
  self& init(colors, double alpha) {
    return init((static_cast<unsigned>(clr)& 0xFF0000) >> 16, (static_cast<unsigned>(clr)& 0xFF00) >> 8, static_cast<unsigned>(clr)& 0xFF, alpha);
  }
  self& init(color_rgb) {
    r_=((static_cast<int>(rgb) >> 16) & 0xFF),
      g_=((static_cast<int>(rgb) >> 8) & 0xFF),
      b_=(static_cast<int>(rgb) & 0xFF),
      a_=(1.0);
    return *this;
  }
  self& init(color_argb) {
    r_=((static_cast<int>(argb) >> 16) & 0xFF),
      g_=((static_cast<int>(argb) >> 8) & 0xFF),
      b_=(static_cast<int>(argb) & 0xFF),
      a_=(((static_cast<int>(argb) >> 24) & 0xFF) / 255.0);
    return *this;
  }
  self& init(color_rgba) {
    r_=((static_cast<int>(rgba) >> 24) & 0xFF),
      g_=((static_cast<int>(rgba) >> 16) & 0xFF),
      b_=((static_cast<int>(rgba) >> 8) & 0xFF),
      a_=((static_cast<int>(rgba) & 0xFF) / 255.0);
    return *this;
  }
  self& init(unsigned red, unsigned green, unsigned blue, double alpha = 1.0) {
    r_=(red), g_=(green), b_=(blue), a_=(alpha);
    if (alpha < 0.0)
      a_ = 0.0;
    else if (alpha > 1.0)
      a_ = 1.0;
    return *this;
  }
  
  //class color
  color::color(colors clr)
  {init(clr);}
  
  color::color(colors clr, double alpha)
  {init(clr, alpha);}
  
  color::color(color_rgb rgb)
  {init(rgb);}
  
  color::color(color_argb argb)
  {init(argb);}
  
  color::color(color_rgba rgba)
  {init(rgba);}
  
  color::color(unsigned red, unsigned green, unsigned blue, double alpha = 1.0)
  {init(red, green, blue, alpha);}
  
#if !defined(USE_STD_REGEX)
  string read_number(char* str, size_t& pos)
  {
    pos = str.find_first_of("0123456789", pos);
    if (pos == str.npos)
      return{};
    
    auto end = str.find_first_not_of("0123456789", pos + 1);
    //integer part
    if (end == str.npos)
    {
      pos = end;
      return str.substr(pos);
    }
    
    if (str[end] == '.')
    {
      auto decimal_end = str.find_first_not_of("0123456789", end + 1);
      if ((decimal_end == str.npos) || (decimal_end == end + 1)) //Because of missing %
        return{};
      
      end = decimal_end;
    }
    
    auto ch = str[end];
    if (ch == '%' || ch == ' ' || ch == ',' || ch == ')')
    {
      auto start = pos;
      pos = end + (ch == '%' ? 1 : 0);
      return str.substr(start, pos - start);
    }
    return{};
  }
#endif
  
  //Initializes the color with a CSS-like string
  //contributor: BigDave(mortis2007 at hotmail co uk)
  //date: February 3, 2015
  //maintainor: Jinhao, extended the support of CSS-spec
  
  color::color(const char* css_color)
    : a_(1.0)
  {
    const char * excpt_what = "color: invalid rgb format";
    
    auto pos = css_color.find_first_not_of(' ');
    if (pos == css_color.npos)
      throw invalid_argument(excpt_what);
    
    if ('#' == css_color[pos])
    {
      if (css_color.size() < pos + 4)
        throw invalid_argument(excpt_what);
      
      auto endpos = css_color.find_first_not_of("0123456789abcdefABCDEF", pos + 1);
      if (endpos == css_color.npos)
        endpos = static_cast<decltype(endpos)>(css_color.size());
      
      if ((endpos - pos != 4) && (endpos - pos != 7))
        throw invalid_argument(excpt_what);
      
      auto n = stoi(css_color.substr(pos + 1, endpos - pos - 1), NULL, 16);
      
      if (endpos - pos == 4)
      {
        r_ = ((0xF00 & n) >> 4) | ((0xF00 & n) >> 8);
        g_ = (0xF0 & n) | ((0xF0 & n) >> 4);
        b_ = (0xF & n) | ((0xF & n) << 4);
      }
      else
      {
        r_ = (0xFF0000 & n) >> 16;
        g_ = (0xFF00 & n) >> 8;
        b_ = (0xFF & n);
      }
      
      return;
    }
    
    //tolower is not allowed because of concept requirements
    transform(css_color.begin(), css_color.end(), css_color.begin(), [](char ch){
      if('A' <= ch && ch <= 'Z')
        return static_cast<char>(ch - ('A' - 'a'));
      return ch;
    });
    auto endpos = css_color.find(' ', pos + 1);
    if (endpos == css_color.npos)
      endpos = css_color.size();
    
    if ((endpos - pos == 11) && (css_color.substr(pos, 11) == "transparent"))
    {
      r_ = 0;
      g_ = 0;
      b_ = 0;
      a_ = 0;
      return;
    }
    
    auto type_end = css_color.find_first_of(" (", pos + 1);
    
    if (type_end == css_color.npos || ((type_end - pos != 3) && (type_end - pos != 4)))	//rgb/hsl = 3, rgba/hsla = 4
      throw invalid_argument(excpt_what);
    
    bool has_alpha = false;
    if (type_end - pos == 4) //maybe rgba/hsla
    {
      if (css_color[pos + 3] != 'a')
        throw invalid_argument(excpt_what);
      has_alpha = true;
    }
    
    auto type_name = css_color.substr(pos, 3);
    pos = css_color.find_first_not_of(' ', type_end);
    if (pos == css_color.npos || css_color[pos] != '(')
      throw invalid_argument(excpt_what);
    
    auto str = read_number(css_color, ++pos);
    if (str.empty())
      throw invalid_argument(excpt_what);
    
    if ("rgb" == type_name)
    {
      vector<string> rgb;
      
      rgb.emplace_back(move(str));
      
      const bool is_real = (rgb.back().back() == '%');
      
      for (int i = 0; i < 2; ++i)
      {
        pos = css_color.find_first_not_of(' ', pos);
        if (pos == css_color.npos || css_color[pos] != ',')
          throw invalid_argument(excpt_what);
        
        str = read_number(css_color, ++pos);
        if (str.empty())
          throw invalid_argument(excpt_what);
        
        rgb.emplace_back(move(str));
        if (rgb.size() == 3)
          break;
      }
      
      if (rgb.size() != 3)
        throw invalid_argument(excpt_what);
      
      if (is_real)
      {
        auto pr = stod(rgb[0].substr(0, rgb[0].size() - 1));
        r_ = (pr > 100 ? 255.0 : 2.55 * pr);
        
        pr = stod(rgb[1].substr(0, rgb[1].size() - 1));
        g_ = (pr > 100 ? 255.0 : 2.55 * pr);
        
        pr = stod(rgb[2].substr(0, rgb[2].size() - 1));
        b_ = (pr > 100 ? 255.0 : 2.55 * pr);
      }
      else
      {
        r_ = stod(rgb[0]);
        if (r_ > 255.0)	r_ = 255;
        
        g_ = stod(rgb[1]);
        if (g_ > 255.0)	g_ = 255;
        
        b_ = stod(rgb[2]);
        if (b_ > 255.0)	b_ = 255;
      }
    }
    else if ("hsl" == type_name)
    {
      if (str.back() == '%')
        throw invalid_argument(excpt_what);
      
      auto h = stod(str);
      
      pos = css_color.find_first_not_of(' ', pos);
      if (pos == css_color.npos || css_color[pos] != ',')
        throw invalid_argument(excpt_what);
      
      str = read_number(css_color, ++pos);
      if (str.empty() || str.back() != '%')
        throw invalid_argument(excpt_what);
      
      auto s = stod(str.substr(0, str.size() - 1));
      
      pos = css_color.find_first_not_of(' ', pos);
      if (pos == css_color.npos || css_color[pos] != ',')
        throw invalid_argument(excpt_what);
      
      str = read_number(css_color, ++pos);
      if (str.empty() || str.back() != '%')
        throw invalid_argument(excpt_what);
      
      auto l = stod(str.substr(0, str.size() - 1));
      
      from_hsl(h, s / 100, l / 100);
    }
    else
      throw invalid_argument(excpt_what);	//invalid color type
    
    if (has_alpha)
    {
      str = read_number(css_color, ++pos);
      if (str.empty() || str.back() == '%')
        throw invalid_argument(excpt_what); //invalid alpha value
      
      a_ = stod(str);
    }
}

color& color::from_rgb(unsigned red, unsigned green, unsigned blue)
{
  r_ = red;
  g_ = green;
  b_ = blue;
  return *this;
}

double rgb_from_hue(double v1, double v2, double h)
{
  if (h < 0.0)
    h += 1.0;
  else if (h > 1.0)
    h -= 1.0;
  
  if (h < 0.1666666) return v1 + (v2 - v1) * (6.0 * h);
  if (h < 0.5) return v2;
  if (h < 0.6666666) return v1 + (v2 - v1) * (4.0 - h * 6.0);
  return v1;
}

color& color::from_hsl(double hue, double saturation, double lightness)
{
  if (0.0 == saturation)
  {
    r_ = lightness * 255.0;
    g_ = r_;
    b_ = r_;
  }
  else
  {
    double var2;
    if (lightness < 0.5)
      var2 = lightness * (1.0 + saturation);
    else
      var2 = (lightness + saturation) - (saturation * lightness);
    
    double var1 = 2.0 * lightness - var2;
    
    hue /= 360;
    r_ = 255.0 * rgb_from_hue(var1, var2, hue + 0.33333);
    g_ = 255.0 * rgb_from_hue(var1, var2, hue);
    b_ = 255.0 * rgb_from_hue(var1, var2, hue - 0.33333);
  }
  return *this;
}

color& color::alpha(double al)
{
  if (al < 0.0)
    a_ = 0.0;
  else if (al > 1.0)
    a_ = 1.0;
  else
    a_ = al;
  return *this;
}

color color::blend(const color& bgcolor, double alpha) const
{
  color result;
  result.r_ = r_ * (1.0 - alpha) + bgcolor.r_ * alpha;
  result.g_ = g_ * (1.0 - alpha) + bgcolor.g_ * alpha;
  result.b_ = b_ * (1.0 - alpha) + bgcolor.b_ * alpha;
  result.a_ = 1.0;
  return result;
}

bool color::invisible() const
{
  return (a_ == 0.0);
}

pixel_color_t color::px_color() const
{
  return argb();
}

pixel_argb_t color::argb() const
{
  pixel_argb_t argb;
  argb.element.red = static_cast<unsigned>(r_);
  argb.element.green = static_cast<unsigned>(g_);
  argb.element.blue = static_cast<unsigned>(b_);
  argb.element.alpha_channel = static_cast<unsigned>(a_ * 255);
  return argb;
}

pixel_rgba_t color::rgba() const
{
  pixel_rgba_t rgba;
  rgba.element.red = static_cast<unsigned>(r_);
  rgba.element.green = static_cast<unsigned>(g_);
  rgba.element.blue = static_cast<unsigned>(b_);
  rgba.element.alpha_channel = static_cast<unsigned>(a_ * 255);
  return rgba;
}

const double& color::r() const
{
  return r_;
}
const double& color::g() const
{
  return g_;
}
const double& color::b() const
{
  return b_;
}

const double& color::a() const
{
  return a_;
}

bool color::operator==(const color& other) const
{
  return (px_color().value == other.px_color().value);
}
bool color::operator!=(const color& other) const
{
  return (px_color().value != other.px_color().value);
}

//end class color

friend color operator+(const color&, const color&);
};

color operator+(const color& x, const color& y)
{
  double a = x.a_ + y.a_;
  auto r = static_cast<unsigned>(x.r_ + y.r_);
  auto g = static_cast<unsigned>(x.g_ + y.g_);
  auto b = static_cast<unsigned>(x.b_ + y.b_);
  
  return color{
    r > 255 ? 255 : r,
      g > 255 ? 255 : g,
      b > 255 ? 255 : b,
      a > 1.0 ? 1.0 : a };
}
#endif

template<typename T>
struct basic_point
{
  //typedef-names
  typedef T value_type;
  value_type x, y;
  //data member
  
  //member functions
  basic_point() {x=0; y=0;}
  
  basic_point(value_type x_, value_type y_)
    : x(x_), y(y_)
  {}
  
  bool operator==(const basic_point& other) const noexcept
  {
    return (x == other.x && y == other.y);
  }
  
  bool operator!=(const basic_point& other) const noexcept
  {
    return (x != other.x || y != other.y);
  }
  
  bool operator<(const basic_point& other) const noexcept
  {
    return ((y < other.y) || (y == other.y && x < other.x));
  }
  
  bool operator<=(const basic_point& other) const noexcept
  {
    return ((y < other.y) || (y == other.y && x <= other.x));
  }
  
  bool operator>(const basic_point& other) const noexcept
  {
    return ((y > other.y) || (y == other.y && x > other.x));
  }
  
  bool operator>=(const basic_point& other) const noexcept
  {
    return ((y > other.y) || (y == other.y && x >= other.x));
  }
  
  basic_point operator-(const basic_point& other) const noexcept
  {
    return{ x - other.x, y - other.y };
  }
  
  basic_point operator+(const basic_point& other) const noexcept
  {
    return{ x + other.x, y + other.y };
  }
  
  basic_point& operator-=(const basic_point& other) noexcept
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  
  basic_point& operator+=(const basic_point& other) noexcept
  {
    x += other.x;
    y += other.y;
    return *this;
  }
};

typedef basic_point<int> point;
typedef basic_point<unsigned> upoint;

struct size
{
  typedef unsigned value_type;
  //struct size
  size::size():width(0), height(0){}
  size::size(value_type width, value_type height) : width(width), height(height){}
  
  bool size::empty() const
  {
    return (0 == width || 0 == height);
  }
  
  bool size::is_hit(const point& pos) const
  {
    return (0 <= pos.x && pos.x < static_cast<int>(width) && 0 <= pos.y && pos.y < static_cast<int>(height));
  }
  
  size& size::shift()
  {
    T_SWAP(int, width, height);
    return *this;
  }
  
  bool size::operator==(const size& rhs) const
  {
    return (width == rhs.width) && (height == rhs.height);
  }
  
  bool size::operator!=(const size& rhs) const
  {
    return (width != rhs.width) || (height != rhs.height);
  }
  
  size size::operator+(const size& sz) const
  {
    return size(width + sz.width, height + sz.height);
  }
  //end struct size
  
  
  value_type width;
  value_type height;
};

struct rectangle
{
  int x;
  int y;
  unsigned width;
  unsigned height;
  
  //struct rectangle
  rectangle::rectangle()
    :x(0), y(0), width(0), height(0)
  {}
  
  rectangle::rectangle(int x, int y, unsigned width, unsigned height)
    :x(x), y(y), width(width), height(height)
  {}
  
  rectangle::rectangle(const size & sz)
    :x(0), y(0), width(sz.width), height(sz.height)
  {}
  
  rectangle::rectangle(const point & pos, const size& sz)
    : x(pos.x), y(pos.y), width(sz.width), height(sz.height)
  {}
  
  bool rectangle::operator==(const rectangle& rhs) const
  {
    return (width == rhs.width) && (height == rhs.height) && (x == rhs.x) && (y == rhs.y);
  }
  
  bool rectangle::operator!=(const rectangle& rhs) const
  {
    return (width != rhs.width) || (height != rhs.height) || (x != rhs.x) || (y != rhs.y);
  }
  
  point rectangle::position() const noexcept
  {
    return point(x, y);
  }
  
  rectangle& rectangle::position(const point& p) noexcept
  {
    x = p.x;
    y = p.y;
    return *this;
  }
  
  size rectangle::dimension() const noexcept
  {
    return size(width, height);
  }
  
  rectangle& rectangle::dimension(const size& sz) noexcept
  {
    width = sz.width;
    height = sz.height;
    return *this;
  }
  
  rectangle& rectangle::pare_off(int pixels)
  {
    x += pixels;
    y += pixels;
    auto const px_twice = (pixels << 1);
    if (px_twice > static_cast<int>(width))
      width = 0;
    else
      width -= px_twice;
    
    if (px_twice > static_cast<int>(height))
      height = 0;
    else
      height -= px_twice;
    
    return *this;
  }
  
  int rectangle::right() const noexcept
  {
    return x + static_cast<int>(width);
  }
  
  int rectangle::bottom() const noexcept
  {
    return y + static_cast<int>(height);
  }
  
  bool rectangle::is_hit(int pos_x, int pos_y) const
  {
    return	(x <= pos_x && pos_x < x + static_cast<int>(width)) &&
      (y <= pos_y && pos_y < y + static_cast<int>(height));
  }
  
  bool rectangle::is_hit(const point& pos) const
  {
    return	(x <= pos.x && pos.x < x + static_cast<int>(width)) &&
      (y <= pos.y && pos.y < y + static_cast<int>(height));
  }
  
  bool rectangle::empty() const
  {
    return (0 == width) || (0 == height);
  }
  
  rectangle& rectangle::shift()
  {
    T_SWAP(int, x, y);
    T_SWAP(int, width, height);
    return *this;
  }
  //end struct rectangle
};

struct rectangle_rotator
{
public:
  bool rotated_;
  rectangle area_;
  
  //class rectangle_rotator
  rectangle_rotator::rectangle_rotator(bool rotated, const rectangle& area)
    : rotated_(rotated),
    area_(area)
  {}
  
  int rectangle_rotator::x() const
  {
    return (rotated_ ? area_.y : area_.x);
  }
  
  int & rectangle_rotator::x_ref()
  {
    return (rotated_ ? area_.y : area_.x);
  }
  
  int rectangle_rotator::y() const
  {
    return (rotated_ ? area_.x : area_.y);
  }
  
  int & rectangle_rotator::y_ref()
  {
    return (rotated_ ? area_.x : area_.y);
  }
  
  unsigned rectangle_rotator::w() const
  {
    return (rotated_ ? area_.height : area_.width);
  }
  
  unsigned & rectangle_rotator::w_ref()
  {
    return (rotated_ ? area_.height : area_.width);
  }
  
  unsigned rectangle_rotator::h() const
  {
    return (rotated_ ? area_.width : area_.height);
  }
  
  unsigned & rectangle_rotator::h_ref()
  {
    return (rotated_ ? area_.width : area_.height);
  }
  
  int rectangle_rotator::right() const
  {
    return (rotated_ ? area_.y + static_cast<int>(area_.height) : area_.x + static_cast<int>(area_.width));
  }
  
  int rectangle_rotator::bottom() const
  {
    return (rotated_ ? area_.x + static_cast<int>(area_.width) : area_.y + static_cast<int>(area_.height));
  }
  
  const rectangle& rectangle_rotator::result() const
  {
    return area_;
  }
  //end class rectangle_rotator
  
};//end class rectangle_rotator


/// Interface for caret operations
class caret_interface
{
public:
  virtual ~caret_interface() {}
  
  virtual bool activated() const = 0;
  virtual void disable_throw() = 0;
  
  virtual void effective_range(const rectangle& range) = 0;
  
  virtual void position(const point& pos) = 0;
  virtual point position() const = 0;
  
  virtual void dimension(const size& size) = 0;
  virtual size dimension() const  = 0;
  
  virtual void visible(bool visibility) = 0;
  virtual bool visible() const = 0;
};//end class caret_interface

/// The system-wide parameters for mouse wheel
struct mouse_wheel
{
  unsigned lines;			///< The number of lines to scroll when the vertical mouse wheel is moved.
  unsigned characters;	///< The number of characters to scroll when the horizontal mouse wheel is moved.
  
  mouse_wheel::mouse_wheel()
    : lines(3), characters(3)
  {
#if defined(NANA_WINDOWS)
    // https://msdn.microsoft.com/en-us/library/ms997498.aspx
    //#define SPI_SETWHEELSCROLLCHARS   0x006D
#	ifndef SPI_GETWHEELSCROLLCHARS
#		define SPI_GETWHEELSCROLLCHARS   0x006C
#	endif
    ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
    ::SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &characters, 0);
#endif
  }
  
  
};


struct font_style
{
  unsigned weight;	//normal
  bool italic;
  bool underline;
  bool strike_out;
  
  font_style() {
    weight = 400;
    italic = false;
    underline = false;
    strike_out = false;
  }
  font_style(unsigned weight, bool italic = false, bool underline = false, bool strike_out = false);
};


struct arg_mouse//		: public event_arg
{
		event_code evt_code; ///< what kind of mouse event?
    native_window_type window_handle;  ///< A handle to the event native_window_type
    point pos;   ///< cursor position in the event native_window_type
    mouse button;	///< indicates a button which triggers the event
    
    bool left_button;	///< true if mouse left button is pressed
    bool mid_button;	///< true if mouse middle button is pressed
    bool right_button;	///< true if mouse right button is pressed
    bool alt;			///< true if keyboard alt is pressed
    bool shift;			///< true if keyboard Shift is pressed
    bool ctrl;			///< true if keyboard Ctrl is pressed
    
    /// Checks if left button is operated,
    bool is_left_button() const
    {
      return (event_code_mouse_move == evt_code ? left_button : (mouse_left_button == button));
    }
};

/// \brief in arg_wheel event_code is event_code_mouse_wheel 

/// The type arg_wheel is derived from arg_mouse, a handler 
/// with prototype void(const arg_mouse&) can be set for mouse_wheel.
struct arg_wheel : public arg_mouse
{
		enum wheel{
      vertical,
        horizontal
    };
    
    wheel	which;		///< which wheel is rotated
    bool	upwards;	///< true if the wheel is rotated to the top/left, depends on which and false otherwise
    unsigned distance;	///< expressed in multiples or divisions of 120
};

struct arg_dropfiles //: public event_arg  
{
		native_window_type	window_handle;	    ///<  A handle to the event native_window_type
    point	pos;	            ///<  cursor position in the event native_window_type
    void*	files;	///<  external filenames
};

struct arg_expose //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    bool exposed;	                ///< the native_window_type is visible?
};

struct arg_focus //: public event_arg
{
		/// A constant to indicate how keyboard focus emitted.
		enum reason
    {
      general,	///< the focus is received by OS native native_window_type manager.
        tabstop,	///< the focus is received by pressing tab.
        mouse_press ///< the focus is received by pressing a mouse button.
    };
    
    native_window_type window_handle;			///< A handle to the event native_window_type
    native_window_type receiver;	///< it is a native native_window_type handle, and specified which native_window_type receives focus
    bool	getting;						///< the native_window_type received focus?
    reason	focus_reason;					///< determines how the widget receives keyboard focus, it is ignored when 'getting' is equal to false
};

struct arg_keyboard //: public event_arg
{
		event_code evt_code;	    ///< it is event_code_key_press in current event
    native_window_type window_handle;	///< A handle to the event native_window_type
    mutable wchar_t key;	///< the key corresponding to the key pressed
    mutable bool ignore;	    ///< this member is only available for key_char event, set 'true' to ignore the input.
    bool ctrl;	                ///< keyboard Ctrl is pressed?
    bool shift;	                ///< keyboard Shift is pressed
};

struct arg_move //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    int x;	                        ///< 
    int y;	                        ///< 
};

struct arg_resized //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    unsigned width;	                ///< new width in pixels.
    unsigned height;	            ///< new height in pixels.
};

struct arg_resizing //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    window_border border;	        ///< the native_window_type is being resized by moving border
    mutable unsigned width;	        ///< new width in pixels. If it is modified, the native_window_type's width will be the modified value
    mutable unsigned height;	    ///< new height in pixels. If it is modified, the native_window_type's height will be the modified value
};

struct arg_unload //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    mutable bool cancel;	        ///< 
};

struct arg_destroy //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
};
/// a higher level event argument than just mouse down
struct arg_click //: public event_arg
{
		native_window_type window_handle;	///< A handle to the event native_window_type
    const arg_mouse* mouse_args;	///< If it is not null, it refers to the mouse arguments for click event emitted by mouse, NULL otherwise.
};

enum visible_state
{
		invisible, visible, displayed
};


/// Define some constant about tab category, these flags can be combine with operator |
struct tab_type
{
		enum t
    {
      none,		///< process by nana
        tabstop,	///< move to the next tabstop window
        eating,		///< process by current window
    };
};

enum update_state
{
  update_state_none, update_state_lazy, update_state_refreshed, update_state_request_refresh
};

struct window_flags
{
  bool enabled	:1;
  bool dbl_click	:1;
  bool captured	:1;	///< if mouse button is down, it always receive mouse move even the mouse is out of its rectangle
  bool modal		:1;
  bool take_active:1;	///< If take_active is false, other.active_window still keeps the focus.
  bool refreshing	:1;
  bool destroying	:1;
  bool dropable	:1; ///< Whether the window has make mouse_drop event.
  bool fullscreen	:1;	///< When the window is maximizing whether it fit for fullscreen.
  bool borderless :1;
  bool make_bground_declared	: 1;	///< explicitly make bground for bground effects
  bool ignore_menubar_focus	: 1;	///< A flag indicates whether the menubar sets the focus.
  bool ignore_mouse_focus		: 1;	///< A flag indicates whether the widget accepts focus when clicking on it
  bool space_click_enabled : 1;		///< A flag indicates whether enable mouse_down/click/mouse_up when pressing and releasing whitespace key.
  unsigned Reserved	:18;
  unsigned char tab;		///< indicate a window that can receive the keyboard TAB
  mouse_action	action;
  mouse_action	action_before;
};
