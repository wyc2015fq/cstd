#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <iostream> 

using namespace nana;

//class menu_popuper
//{
//	public:
//		//C++03
//	menu_popuper(menu&, mouse bttn= mouse::right_button);
//	menu_popuper(menu&, window owner, const point& pos, mouse bttn=  mouse::right_button	);
//
//		//C++11
//	menu_popuper(menu&, mouse = mouse::right_button);
//	menu_popuper(menu&, window owner, const point& pos, mouse bttn= mouse::right_button);
//
//	void operator()(const eventinfo&);
//		
//private: 			//Implemented-Specified private members
//		
//};

// Now let's use it. There is a button, it popups the menu when it is clicked.


void on_menu_item(menu::item_proxy& ip)
{
	size_t index = ip.index(); //Get the index of the clicking item.
    cout << "Menu item index:  " << index << endl; 
}

int main()
{
	form fm;

		//We need a menu object
	menu mobj;
	mobj.append ( "Item 0", &on_menu_item);
	mobj.append_splitter();
	mobj.append ( ("Item 1"), &on_menu_item);

		//Now we need a button.
	button btn(fm, rectangle(point(10, 10), size(100, 25)));
	btn.caption(("Popup Menu"));

	;//Popup the menu when right clicking the button.
	btn.events().mouse_down(menu_popuper(mobj));

	;//Or popuping the menu with a specified coordinate when any mouse button is clicked.
	;//btn.make_event<events::click> ( menu_popuper( mobj, btn, point(0, 26),
	;//                                mouse::any_button );

	fm.show();
	exec();
}

