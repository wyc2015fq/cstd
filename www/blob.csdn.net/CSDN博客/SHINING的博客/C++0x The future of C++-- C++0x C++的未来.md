# C++0x: The future of C++-- C++0x:C++的未来 - SHINING的博客 - CSDN博客
2012年10月11日 23:22:50[snsn1984](https://me.csdn.net/snsn1984)阅读数：1493标签：[c++																[features																[compiler																[multithreading																[initialization																[iterator](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=multithreading&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=features&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
# C++0x: The future of C++
[By Alex Allain](https://plus.google.com/113987539774523532573?rel=author)
## What is C++0x?
C++0x was the working name for the new standard for C++, adding many language features that I'll cover in this series on C++11. In September 2011, C++0x was officially published as the new C++11 standard, and many
 compilers now provide support for some of the core C++11 features.
C++11 includes a wide range of features: major new features like [lambda](http://www.cprogramming.com/c++11/c++11-lambda-closures.html) support and "move semantics", usability improvements like
 type inference through the auto keyword, simplified looping over containers, and many improvements that will make templates easier to write and easier to use. This series on C++11 will cover all of these features and many more.
## Should you care about C++11?
Most definitely. C++11 adds many new language features to C++. C++11 should fix many annoyances and reduce the overall verbosity of C++ as well as provide new tools, such as lambda expressions, that increase its overall
 expressiveness and clarity. Fetaures like move semantics improve the basic efficiency of the language, allowing you to write faster code, and the improvements to the template system make it much easier to write generic code.
The new standard library will also include many new features, including adding multithreading support directly into C++ and improved smart pointers that will simplify memory management for those who aren't already
 using features like [boost::shared_ptr](http://www.boost.org/doc/libs/1_46_1/libs/smart_ptr/shared_ptr.htm).
I've started using several new C++11 features professionally and I'm loving it. Some of the new features I'm fond of include the new meaning of the auto keyword, simplifications like better handling of right angle
 brackets in templates, lambda expressions and the new function declaration syntax.
## How was C++11 developed?
I can't go on further about C++11 without acknowledging the hard work done by the [C++ standards committee](http://www.open-std.org/jtc1/sc22/wg21/)--a
 group of experts from academia and industry who have met many times to work through all the edge cases and design a programming language that can be implemented across multiple platforms by multiple compilers, producing efficient and reasonably maintainable
 code. The next standard, C++11, looks to be a fantastic addition to the flexibility and power of C++.
## What is C++11 about?
### Language usability
Having started to use C++11, I'd say that the most fundamental way of looking at it is that it makes C++ a much more usable language. This isn't to say that it makes it a simpler language--there are lots of new features--but
 it provides a lot of functionality that makes it easier to program. Let's look at one example, the auto keyword.
In C++11, if the compiler is able to determine the type of a variable from its initialization, you don't need to provide the type. For example, you can write code such as
int x = 3;
auto y = x;
and the compiler will deduce that y is an int. This, of course, isn't a shining example of where auto is really useful. Auto really comes into its own when working with [templates](http://www.cprogramming.com/tutorial/templates.html) and
 especially the [STL](http://www.cprogramming.com/tutorial/stl/stlintro.html). Why is that? Imagine working with an iterator:map<string, string> address_book;
address_book[ "Alex" ] = "webmaster@cprogramming.com";
// add a bunch of people to address_book
Now you want to iterate over the elements of the address_book. To do it, you need an iterator:map<string, string>::iterator itr = address_book.begin();
That's an awfully long type declaration for something that you already know the type of! Wouldn't it be nice to simply write:auto itr = address_book.begin();
The code is much shorter, and frankly, I think it's more readable, not less, because the template syntax obscures everything else on that line. This is one of my favorite new features, and I find it eliminates a lot
 of headaches and hard-to-track-down compiler errors, and just generally saves time without losing expressiveness.
#### Ranged For Loops
Now, the iterator example is one where C++11 has come up with an even better way of handling this--something called a range-based for loop (which almost every language has nowadays). The idea is so elegant, an example
 should suffice:
vector<int> vec;
vec.push_back( 10 );
vec.push_back( 20 );
for (int &i : vec ) 
{
        cout << i;
}
All you need to do is give your variable and the range to iterate over (defined as something with iterators available via calls to begin and end--so all STL containers) and you're set! This is a pretty new  feature,
 available as far as I know only in GCC 4.6.
But what if you want to iterate over a map? How do you put in the type for a value stored in a map? With a vector, you know the value is an int. With a map, it's essentially a pair, with .first and .second giving
 you the key and value. But with auto, we don't need to worry about getting the exact type right, you can simply do this:
for ( auto address_entry : address_book )
{
        cout  << address_entry.first << " < " << address_entry.second << ">" <<endl;
}
Which prints out as:
Alex <webmaster@cprogramming.com>
Isn't that a nice combination of new features in C++11? It feels like it was designed that way :)
#### Right angle brackets
And I have one more usability improvement for you--in previous versions of the C++ standard, if you wrote a template that had another template:
vector<vector<int> > vector_of_int_vectors;
You had to write a space between the two closing angle brackets. This was not only annoying, but if you did write >> without a space, you'd get obtuse and confusing compiler error messages. The reason for this behavior
 had to do with an obscure C++ lexer trait called the [maximal munch rule](http://en.wikipedia.org/wiki/Maximal_munch). The good news is, you no longer need to worry about it! Say hello to
vector<vector<int>> vector_of_int_vectors;
True, this seems like a small thing, but it's a victory of human code writers over machine tools. Plus it's much less ugly. Compiler support for right-angle brackets is great: GCC since 4.3, MSVC since version 8 (!)
 and the Intel compiler since version 11.
### Mulithreading
For the first time, the C++11 standard will include a memory model and corresponding libraries for multithreading, meaning that you'll be able to write standards-compliant multithreading code. The new standard will
 provide for all the normal threading functionality, such as threads and thread-local storage and atomic operations. It will also include an interesting set of features, futures and promises. The basic idea of futures and promises is that you can write code
 that says, "this object, a future, stands for a result that hasn't been computed yet" and the work to compute the value can take place in the background. When the value is needed, you ask the future for it; if the value is ready, you get it; if not, you wait.
I'll go into more depth on mulithreading in a later article in this series.
### Lots of other stuff
The number of features in C++11 is incredibly exciting. You can get a taste for what's available on the [C++11 page at Wikipedia](http://en.wikipedia.org/wiki/C%2B%2B0x), and I plan to dive into
 many of these features in more depth in this series, including:
- [How auto, decltype, and the new function syntax work together to create better code](http://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html)
- [Lambda functions](http://www.cprogramming.com/c++11/c++11-lambda-closures.html)
- [Usability improvements like range-based for loops](http://www.cprogramming.com/c++11/c++11-ranged-for-loop.html)
- [Performance improvements like generalized constant expressions](http://www.cprogramming.com/c++11/c++11-compile-time-processing-with-constexpr.html)
- [Performance improvements like rvalue references and move semantics](http://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html)
- [Type safety improvements like nullptr and strongly typed enumerations](http://www.cprogramming.com/c++11/c++11-nullptr-strongly-typed-enum-class.html)
- Language expressiveness improvements like the ability to explicitly override or make "final" virtual functions
- Improvements to object construction, like initialization lists, delegating constructors and explicit control over auto-generated functions
- Template improvements including templated typedefs, variadic templates, uniform initialization and static assertions
- The new C++ memory model and the feature it supports: multithreading
- Improvements to the standard library including regular expressions, hashtables and improved smart pointers
- What features were removed or deprecated by the standard, and why
## Compiler Support for C++11
Of course, no language feature matters if it's not available to use, and the good news is that many compilers now support the new C++11 features. The Apache foundation has compiled a very useful list of C++11 language
 features and the compilers that support them: [Compiler Support for C++11](http://wiki.apache.org/stdcxx/C%2B%2B0xCompilerSupport). If you're interested in GCC, this page describes the [GCC
 4.7 support for C++11](http://gcc.gnu.org/gcc-4.7/cxx0x_status.html).
Some compilers, such as GCC, do not automatically enable support for these features--for example, to enable C++11 features, you must compile with -std=c++0x. Nonetheless, they are still valuable if you're working
 on a project where you can control the choice of compiler and set of language features. 
## Keeping up with the news
To the the latest updates on C++11, you can subscribe to [our RSS feed](http://feeds2.feedburner.com/Cprogrammingcom), or join me over on [Facebook](https://www.facebook.com/cprogrammingcom) or [twitter](http://twitter.com/#%21/alexallain).
