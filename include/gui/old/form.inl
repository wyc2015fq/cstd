LRESULT CALLBACK Form_Proc(HWID hwid, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
  case WM_CREATE:
    if (NULL == hwid->onwer) {
      //hwid->onwer = create_window(NULL, false, r, appearance_default);
    }
    break;
  }
  return 0;
}

