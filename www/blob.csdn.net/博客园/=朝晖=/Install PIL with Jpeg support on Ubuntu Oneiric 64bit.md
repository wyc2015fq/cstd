# Install PIL with Jpeg support on Ubuntu Oneiric 64bit - =朝晖= - 博客园
# [Install PIL with Jpeg support on Ubuntu Oneiric 64bit](https://www.cnblogs.com/dhcn/p/7105597.html)
from：[http://jj.isgeek.net/2011/09/install-pil-with-jpeg-support-on-ubuntu-oneiric-64bits/](http://jj.isgeek.net/2011/09/install-pil-with-jpeg-support-on-ubuntu-oneiric-64bits/)
I am posting this because it took me ages to figure out how to solve this one.
I could not get PIL to compile with JPEG, Zlib or freetype support on my virtualenv. I am using Ubuntu Oneiric Beta1 on a [Lenovo Thinkpad X220](http://jj.isgeek.net/2011/08/ordered-a-new-laptop/). That is a 64bit installation.
As read in every blog post out there, you first need to install the system libraries so PIL can find them.
$ sudo apt-get install libjpeg libjpeg-dev libfreetype6 libfreetype6-dev zlib1g-dev
After that, your regular `pip install PIL` should work under most situations. In my case I was still seeing this summary after the installation:
    ---------------------------------------------------------------
    *** TKINTER support not available
    --- JPEG support not available
    --- ZLIB (PNG/ZIP) support not available
    --- FREETYPE2 support not available
    *** LITTLECMS support not available
    ---------------------------------------------------------------
After a lot of googling around, I found [this solution on Ubuntu forums](http://ubuntuforums.org/showpost.php?p=10811107&postcount=5).
It turns out that the APT installations put the libraries under `/usr/lib/x86_64-[Linux](http://lib.csdn.net/base/linux)-gnu` and PIL will search for them in `/usr/lib/`. So you have to create symlinks for PIL to see them.
# ln -s /usr/lib/x86_64-linux-gnu/libjpeg.so /usr/lib
# ln -s /usr/lib/x86_64-linux-gnu/libfreetype.so /usr/lib
# ln -s /usr/lib/x86_64-linux-gnu/libz.so /usr/lib
Now proceed and reinstal PiL, `pip install -U PIL`:
    ---------------------------------------------------------------
    *** TKINTER support not available
    --- JPEG support available
    --- ZLIB (PNG/ZIP) support available
    --- FREETYPE2 support available
    *** LITTLECMS support not available
    ---------------------------------------------------------------
tada ![:D](http://jj.isgeek.net/wp-includes/images/smilies/icon_biggrin.gif) !
Update:
I just had this issue again on Ubuntu Precise, I found a solution here:
[http://www.sandersnewmedia.com/why/2012/04/16/installing-pil-virtualenv-ubuntu-1204-precise-pangolin/](http://www.sandersnewmedia.com/why/2012/04/16/installing-pil-virtualenv-ubuntu-1204-precise-pangolin/)
# ln -s /usr/lib/`uname -i`-linux-gnu/libfreetype.so /usr/lib/
# ln -s /usr/lib/`uname -i`-linux-gnu/libjpeg.so /usr/lib/
# ln -s /usr/lib/`uname -i`-linux-gnu/libz.so /usr/lib/
Note the `uname -i` that prints the box’s arquitecture. This is a much more generic solution!
[How to do it on Ubuntu Precise Pagolin 12.04](http://codeinthehole.com/writing/how-to-install-pil-on-64-bit-ubuntu-1204/)
--------------------------------------------------------------------------------
后来根据http://effbot.org/downloads/#Imaging PIL源码安装包中的README方法解决更方便：
```
sudo apt-get install libjpeg62-dev  
sudo apt-get install zlib1g-dev  
sudo apt-get install libfreetype6-dev  
sudo apt-get install liblcms1-dev
```

