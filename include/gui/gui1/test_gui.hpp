
#include "gui.hpp"

#include "test_signalslot.inl"

uchar icons16[] = {
#include "D:/code/git/directui_src/App/res/icons16.txt"
};

int test_gui()
{
  Image bm;
  ImageLoad(&bm, icons16, sizeof(icons16), 0);
  test_signalslot();
  //Platform::Window* wwaa = new Platform::Window();
  //typedef int(*binop_t)(double a, double b);
  //binop_t aaa = [](double a, double b) -> int { return a + b; };
  //binop_t bbb = [](double a, double b) -> int { return a * b; };
  //wwaa->Create();
  //wwaa->Show();
  //const char* aa = typeid(aaa).name();
  //const char* bb = typeid(bbb).name();
  Widget* wi = widget()
               ->add(widget())
               ->add(label())
               ->add(button());
  Window* w = new Window();
  w->Create();
  w->Move(100, 100, 500, 500);
  w->SetCaption("Hello, World");
  w->Show(true);
  w->widget(wi);
  Platform::MessageLoop();
  return 0;
}
