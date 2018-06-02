
#include "Shortcut.h"

int main() {
  CoInitialize(NULL);
  Create(CSIDL_DESKTOP, "notepad.exe", NULL);
  //Remove(CSIDL_DESKTOP, TEXT("Shortcut.lnk"));
  return 0;
}
