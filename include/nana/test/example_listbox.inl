#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <iostream> 

int main()
{
	using namespace nana;
	form fm;
	listbox lb(fm, rectangle(10, 10, 280, 120));

	lb.append_header("Header", 200);

    // this will attemp to use   
    // item_proxy cat_proxy::append (T &&t, bool set_value=false) 
    // to add a value any of char[4]  = "int"
	
    lb.at(0).append({"int"});
    lb.at(0).append({"double"});

	lb.anyobj(0, 0, 10);    
	lb.anyobj(0, 1, 0.1);   

	int * pi = lb.anyobj<int>(0, 0); 	  // it returns a NULL if there is not an int object specified.
    cout << "Index: (0,0) -> int value: " << *pi << endl; 

	double * pd = lb.anyobj<double>(0, 1); // it returns a NULL if there is not an double object specified.
    cout << "Index: (0,1) -> double value: " << *pd; 

	fm.show();
	exec();
}
