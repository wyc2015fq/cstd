# undefined reference to `hb_font_destroy' - DumpDoctorWang的博客 - CSDN博客





2018年06月05日 16:04:30[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：261标签：[链接库](https://so.csdn.net/so/search/s.do?q=链接库&t=blog)
个人分类：[C++](https://blog.csdn.net/DumpDoctorWang/article/category/7156241)









使用OpenCV3.2.0的时候，编译链接，得到一大堆错误，如下

```cpp
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_buffer_get_glyph_infos'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_font_destroy'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_ft_font_create'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_buffer_add_utf8'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_buffer_destroy'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_shape'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_buffer_guess_segment_properties'
/usr/local/lib/libopencv_freetype.so.3.2.0: undefined reference to `hb_buffer_create'
```

这是因为在在编译的的时候没有引入harfbuzz这个库，至于是怎么确定是这个库呢？那就是搜索这些函数名字，就能得到这个库的名字，然后再搜索“Ubuntu harfbuzz”,在一个Ubuntu官方的网页上就能找到这个库的完整名字:libharfbuzz-dev(一般-dev结尾的就是我们需要的软件包)，然后打开终端输入

```cpp
sudo apt-get install libharfbuzz-dev
```

然后在你的项目的附加库里面加上harfbuzz，CMakeLists.txt只需要在target_link_libraries()加一项harfbuzz就可以。其他编译方式怎么加库自行搜索。

上面提供了一个普适的方法来解决因为没有引入库导致的链接问题。





