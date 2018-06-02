    #include <nana/gui/wvl.hpp> 
    #include <iostream> 
    void foo() 
    { 
        //system("cls"); 
        cout<<"foo"<<endl; 
    } 
    int bar() 
    { 
        //system("cls"); 
        cout<<"bar"<<endl; 
        return 1;
    } 
    void foo_with_eventinfo(const arg_mouse& ei) 
    { 
      cout << "foo_with_eventinfo, mouse pos = ("
       << ei.pos.x  << ", "  <<  ei.pos.y  << ")" << endl; 
    } 
    class click_stat 
    { public: 
        click_stat(): n_(0), id(++glob_id) { } 
        click_stat(const click_stat&o):n_(o.n_), id(++glob_id) {} ; 

        void respond   ()   { cout<<"click_stat     "<<id<< " = "<<++n_<<endl; } 
        int  respond_I ()   { cout<<"click_stat int "<<id<< " = "<<++n_<<endl; return 1;} 
        void respond_ei(const arg_mouse& ei) 
        { 
             cout << "click_state "<<id<< " width eventinfo = " << ++n_ 
            << ", mouse pos = ("  <<ei.pos.x<<", "<<ei.pos.y<<")"
            << endl; 
        } 
     private: 
        //click_stat(const click_stat&) ; 
       int n_;
       const int id; 
       static int glob_id ;
    }; 
    int click_stat::glob_id{0};

    int main() 
    { 
        using namespace nana; 
        using fun_t            = function<void(      void      )>  ; 
        using fun_with_param_t = function<void(const arg_mouse&)>  ; 

        form        fm; 
        click_stat  cs; 
    
        fm.events().click(foo); 
        fun_t       f=foo; 
        fm.events().click(f); 
        fm.events().click(bar); 

        fm.events().mouse_down (foo_with_eventinfo); 
        fun_with_param_t fp(foo_with_eventinfo); 
        fm.events().mouse_down(fp);

        fm.events().click( [&cs](){cs.respond();}                  ); 
        f= bind( &click_stat::respond, cs); 
        fm.events().click(  f                                      ); 
        fm.events().click( bind( &click_stat::respond  , &cs) ); 
        fm.events().click( [& ](){cs.respond();}                   ); 
        //fm.events().click( bind( &click_stat::respond_I, &cs) ); 
        fm.events().click( [& ](){cs.respond_I();}                   ); 


        fp= bind( &click_stat::respond_ei, &cs , placeholders::_1 ); 
        fm.events().mouse_down(   fp                                                            );
        fm.events().mouse_down( [&](const arg_mouse&ma){cs.respond_ei(ma);}                     );
        fm.events().mouse_down( bind( &click_stat::respond_ei, cs, placeholders::_1)  );

        fm.show(); 
        exec(); 
    } 