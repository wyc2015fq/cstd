


#include "gui.hpp"
#include <typeinfo>

int test_gui() {
	typedef int(*binop_t)(double a, double b);
	binop_t aaa = [](double a, double b) -> int { return a + b; };
	binop_t bbb = [](double a, double b) -> int { return a * b; };

	const char* aa = typeid(aaa).name();
	const char* bb = typeid(bbb).name();


	Widget* wi = widget()
		->add(widget())
		->add(widget())
		->add(widget());

	Window w;
	w.open()
		.move(100, 100, 500, 500)
		.show(true);
	Window::loop();
	return 0;
}
