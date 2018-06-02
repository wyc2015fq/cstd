#include <iostream>

#include <nana/gui/wvl.hpp>
#include <nana/threads/pool.hpp>
#include <nana/system/platform.hpp>

void foo()
{    
    cout<<"This is foo"<<endl;
}

void foo2()
{
     system::sleep(500);
    cout<<"This is foo2"<<endl;
}

int main()
{
    threads::pool pool;
    for(int i = 0; i < 10; ++i)
        pool.push(foo);
    pool.signal();          //Make a signal and the signal will be triggered when the
                            //tasks which are pushed before it are finished.
    pool.push(foo2);
    pool.wait_for_signal(); //Wait until the signal is triggered. The wait might be finished
                            //before finish of foo2, because the signal is made before pushing foo2.

	Wait(1);

}

