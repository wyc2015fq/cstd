#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <iostream>

struct person
{
    string name;
    bool         gender;
    unsigned     age;
};

listbox::oresolver& operator << ( listbox::oresolver& orr , const person& pr)
{
	orr << pr.name;
	orr << (pr.gender ?  ("Male") :  ("Female"));
	orr << pr.age;  
	return orr ;
}
ostream & operator << (ostream& orr , const person& pr)
{
	orr << pr.name;
	orr << (pr.gender ?  ("Male") :  ("Female"));
	orr << pr.age;
	return orr ;
}
listbox::iresolver& operator >> ( listbox::iresolver& orr , person& pr)
{
	orr >> pr.name;
	string gender;
	orr >> gender;
	pr.gender = gender ==  ("Male") ;
	orr >> pr.age;  
	return orr ;
}



int main()
{
	using namespace nana;

	form fm;
	listbox lb(fm, rectangle(10, 10, 280, 120));

	auto cat = lb.at(0);
	if (cat.begin() == cat.end())
		wcout <<  ("true ");
	else 		
		wcout <<  ("false ");

	wcout << "\n";

	lb.append_header( ("Name"), 100);
	lb.append_header( ("Gender"), 60);
	lb.append_header( ("Age"), 60);

	//lb.resolver(resolver()); //Pass the user defined resolver to the listbox

	person pr;
	pr.name =  ("Peter");
	pr.gender = true;
	pr.age = 10;
	lb.at(0).append (pr); //person to item. resolver::decode() is required

	person prr;
	lb.at(0).at(0).resolve_to( prr); //item to person. resolver::encode() is required
	cout << pr << "\n";
	cout << prr << "\n";
	fm.show();
	exec();
}