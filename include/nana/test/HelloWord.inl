//#include <nana/gui/wvl.hpp>
#include <nana/widgets/label.hpp>

int HelloWord()
{
    using namespace nana;
    form    fm;
    label   lb(fm, rectangle(fm.size()));
    lb.caption("Hello, World");
    fm.show();
    exec();
}
