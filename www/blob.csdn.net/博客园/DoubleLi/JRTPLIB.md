# JRTPLIB - DoubleLi - 博客园






JRTPLIB is an object-oriented RTP library written in C++. It was first developed for my thesis at the School for Knowledge Technology (or 'School voor Kennistechnologie' in Dutch), a cooperation between the [Hasselt University](http://www.uhasselt.be/) and the [Maastricht University](http://www.unimaas.nl/).

The latest version of the library is 3.11.1 (March 2017). You can take a look at the [ChangeLog](https://github.com/j0r1/JRTPLIB/blob/master/ChangeLog) and browse previous versions or development code at [GitHub](https://github.com/j0r1/JRTPLIB).

The 3.x.x series is a complete rewrite of the library and is meant to be compliant with RFC 3550. Also, the library now provides many useful components which can be helpful in building all sorts of RTP-capable applications. This version was developed at the [Expertise Centre for Digital Media](http://www.edm.uhasselt.be/) (EDM), a research centre of the [Hasselt University](http://www.uhasselt.be/).

The older [2.x](http://research.edm.uhasselt.be/jori/jrtplib/jrtplib_old.html) series is also still available. Note that this version is not compliant with RFC 3550!



## Features

The library offers support for the Real-time Transport Protocol (RTP), defined in RFC 3550. It makes it very easy to send and receive RTP packets and the RTCP (RTP Control Protocol) functions are handled entirely internally. For more detailed information you should take a look at the documentation included in the package.



## Platforms

Currently, the library is known to work on the following platforms:
- GNU/Linux
- MS-Windows (both Win32 and WinCE)
- Solaris

The library will probably work on other unix-like environments too.



## What's the price?

The library is totally FREE!

However, I enjoy getting snail mail, so feel free to send a nice post card to



EDM t.a.v. Jori Liesenborgs
Wetenschapspark 2
3590 Diepenbeek
Belgium



## Download

Several package formats are available:
- [jrtplib-3.11.1.tar.bz2](http://research.edm.uhasselt.be/jori/jrtplib/jrtplib-3.11.1.tar.bz2)
- [jrtplib-3.11.1.tar.gz](http://research.edm.uhasselt.be/jori/jrtplib/jrtplib-3.11.1.tar.gz)
- [jrtplib-3.11.1.zip](http://research.edm.uhasselt.be/jori/jrtplib/jrtplib-3.11.1.zip)

The library uses the [JThread](http://research.edm.uhasselt.be/jori/page/CS/Jthread.html) library to automatically poll for incoming data in the background, so you may want to install it too. Of course, if you'd rather not have it installed, the library will also work without JThread (but you'll have to poll for incoming data yourself). Note that this version requires at least JThread 1.3.0

If you're working with Visual Studio and you're having trouble getting started, perhaps the following document can help: [visual_studio_jthread_jrtplib_compilation.txt](https://github.com/j0r1/JRTPLIB/blob/master/visual_studio_jthread_jrtplib_compilation.txt)

The documentation can be generated using [Doxygen](http://www.stack.nl/~dimitri/doxygen/), but is also available on-line at [jrtplib.readthedocs.io](http://jrtplib.readthedocs.io/).



### EXAMPLES:

The following examples are included in the library package:
- [example1.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example1.cpp)
- [example2.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example2.cpp)
- [example3.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example3.cpp)
- [example4.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example4.cpp)
- [example5.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example5.cpp)
- [example6.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example6.cpp)
- [example7.cpp](https://github.com/j0r1/JRTPLIB/blob/master/examples/example7.cpp)



## Copyright & disclaimer

The following copyright notice and disclaimer applies to the library.



`Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:`



`The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.`



`THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.`

So basically, you can use the library in any way you like, as long as you leave the copyright and disclaimer in the sources.



## Projects using JRTPLIB

Since I'm always curious about where my work is being used, you're welcome to add a description of the project in which you're using JRTPLIB.
- [List projects](http://research.edm.uhasselt.be/jori/cgi-bin/listprojects.py?name=jrtplib)
- [Add your project to the list](http://research.edm.uhasselt.be/jori/cgi-bin/addproject?name=jrtplib)



## Contact

If you have any questions, remarks, suggestions or you would simply like to tell me what you are using the library for (I'm always interested), you can send me an email at this address: `jori.liesenborgs@gmail.com`



from:http://research.edm.uhasselt.be/jori/page/CS/Jrtplib.html









