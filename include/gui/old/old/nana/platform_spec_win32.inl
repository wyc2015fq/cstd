
//struct messages
//@brief:	This defines some messages that are used for remote thread invocation.
//			Some Windows APIs are window-thread-dependent, the operation in other thread
//			must be posted to its own thread.

class co_initializer
{
  HMODULE ole32_;
};

struct window_icons
{
  image sml_icon[1];
  image big_icon[1];
};
