#include <nana/gui/wvl.hpp>

void when_exit(const arg_unload& ei)
{
    msgbox m(ei.window_handle,"msgbox example", msgbox::yes_no);
    m.icon(m.icon_question);
    m<<"Are you sure you want to exit the game?";
    ei.cancel = (m() != m.pick_yes);
}
int main()
{
    using namespace nana;
    form fm;
    fm.events().unload(when_exit);
    fm.show();
    exec();
}