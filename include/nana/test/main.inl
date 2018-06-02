#include <nana/gui/wvl.hpp>
#include <iostream>
#include <exception>

int main()
{
    using namespace nana;
    cout << "Hello main... and nana/gui/wvl.hpp " << endl;    
    try {
            form fm;
            cout << "After fm " << endl; 
            //fm.events().click(clicked);
            fm.show();
            cout << "After fm.show() " << endl;  
            exec();
    }
    catch (exception& e)
    {
            cout << e.what() << endl; 
    }
    catch (...) 
    {
        cout << "Exception occurred" << endl; 
    }

}
