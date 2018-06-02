#include <nana/gui/wvl.hpp> 
#include <iostream> 
int main() 
{ 
    form form; 
    form.events().click( 
                         []{ cout<<"form is clicked"<<endl; } 
                        ); 
    form.show(); 
    exec(); 
} 