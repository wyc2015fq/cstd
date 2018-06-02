#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/menubar.hpp>

int main(int argc, char **argv)
{
    form fm;
    place pl(fm);
    menubar mn(fm);

    pl.div("<vertical <menu weight=25>>");
    pl.field("menu") << mn; 

    mn.push_back("test");
    mn.at(0).append("open modal window", [&fm](menu::item_proxy& ip)
    {
        form fm2(fm);
        fm2.show();
        fm2.modality();
    });

    pl.collocate();

    fm.show();

    exec();

    return 0;
}

 // NOW FIXED !!
    //After opening fm2 via the Menu and closing the Form, the error occurs.

    //last callstack entry is:
    //0 0x00000000005781b6 unique_ptr<basic_event<arg_mouse>::docker, default_delete<basic_event<arg_mouse>::docker> >::~unique_ptr

    //if i understand correctly, the error occurs in the destructor of an unique_ptr.

    //The system i'm using is nana with the latest master branch, windows 8.1 with mingw (gcc 4.9.2).

    //If you need further information please let me know.

    //best regards, jan
    //^

    //additional note:
    //if you open a window via a button or something else, no error occurs. this makes me guess the error is relating to menubars
