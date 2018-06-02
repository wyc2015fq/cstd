#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <iostream> 

void on_menu_item(menu::item_proxy& ip)
{
	size_t index = ip.index(); //Get the index of the clicking item.
    cout << "Menu item index:  " << index << endl; 
}

int main()
{
	using namespace nana;
	form fm;
    fm.caption(L"Popup the menu when right clicking the button");

	;//We need a menu object
	menu mobj;
	mobj.append ( "Item 0", &on_menu_item);
	mobj.append_splitter();
	mobj.append ( "Item 1", &on_menu_item);

	;//Now we need a button.
	button btn(fm, rectangle(point(10, 10), size(100, 25)));
	btn.caption("Popup Menu");

	;//Popup the menu when right clicking the button.
	//btn.events().click(menu_popuper(mobj)) ;

	;//Or popuping the menu with a specified coordinate when any mouse button is clicked.
	btn.events().mouse_down( menu_popuper( mobj, btn, point(100, 36),   mouse::any_button ));

	fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 2, [&btn]() {click(btn); }
#endif

	);
}
