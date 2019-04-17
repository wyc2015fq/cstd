# MSYS2环境搭建 - DoubleLi - 博客园






本机环境：Windows XP 32位
MSYS2地址：http://sourceforge.net/projects/msys2/

下载32位版本，地址：http://sourceforge.net/projects/msys2/files/Base/i686/
当前压缩包名称：msys2-base-i686-20131208.tar.xz
安装完之后，运行目录下的mingw32_shell.bat
查看软件包清单，输入命令：
1
Pacman -Sl
当前如下：
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
mingw32 mingw-w64-i686-FreeImage 3.15.4-2
mingw32 mingw-w64-i686-ResIL 1.7.9-1
mingw32 mingw-w64-i686-SDL 1.2.15-1
mingw32 mingw-w64-i686-SDL2 2.0.1-1
mingw32 mingw-w64-i686-allegro 5.0.10-1
mingw32 mingw-w64-i686-assimp-git 3.0.1801.3086569-2
mingw32 mingw-w64-i686-binutils 2.24-3
mingw32 mingw-w64-i686-boost 1.55.0-2
mingw32 mingw-w64-i686-bullet 2.82-1
mingw32 mingw-w64-i686-bzip2 1.0.6-1
mingw32 mingw-w64-i686-cegui 0.8.3-1
mingw32 mingw-w64-i686-clang-analyzer-svn r197972-1
mingw32 mingw-w64-i686-clang-svn r197972-1
mingw32 mingw-w64-i686-cloog 0.18.1-1
mingw32 mingw-w64-i686-cmake 2.8.12.1-2
mingw32 mingw-w64-i686-collada-dom-svn 2.4.1.r889-1
mingw32 mingw-w64-i686-crt-svn 4.0.0.6390-1
mingw32 mingw-w64-i686-curl 7.34.0-1
mingw32 mingw-w64-i686-dbus 1.6.18-1
mingw32 mingw-w64-i686-discount 2.1.6-1
mingw32 mingw-w64-i686-dlfcn r19-1
mingw32 mingw-w64-i686-dmake 4.12.2-1
mingw32 mingw-w64-i686-dumb 0.9.3-1
mingw32 mingw-w64-i686-expat 2.1.0-2
mingw32 mingw-w64-i686-fftw 3.3.3-1
mingw32 mingw-w64-i686-flac 1.3.0-1
mingw32 mingw-w64-i686-fontconfig 2.11.0-1
mingw32 mingw-w64-i686-freeglut 2.8.1-1
mingw32 mingw-w64-i686-freetype 2.5.2-1
mingw32 mingw-w64-i686-gc 7.2.d-1
mingw32 mingw-w64-i686-gcc 4.8.2-1
mingw32 mingw-w64-i686-gcc-ada 4.8.2-1
mingw32 mingw-w64-i686-gcc-fortran 4.8.2-1
mingw32 mingw-w64-i686-gcc-objc 4.8.2-1
mingw32 mingw-w64-i686-gdb 7.6.2-4
mingw32 mingw-w64-i686-gettext 0.18.3.1-1
mingw32 mingw-w64-i686-giflib 5.0.5-1
mingw32 mingw-w64-i686-glew 1.10.0-1
mingw32 mingw-w64-i686-glfw 3.0.3-1
mingw32 mingw-w64-i686-glib2 2.38.2-1
mingw32 mingw-w64-i686-glsl-optimizer-git 0.1.0.60329.fe8a93f-1
mingw32 mingw-w64-i686-gmp 5.1.3-2
mingw32 mingw-w64-i686-gnupg 1.4.15-1
mingw32 mingw-w64-i686-gnutls 3.2.7-1
mingw32 mingw-w64-i686-gperf 3.0.4-1
mingw32 mingw-w64-i686-gpgme 1.4.3-1
mingw32 mingw-w64-i686-headers-svn 4.0.0.6390-1
mingw32 mingw-w64-i686-hlsl2glsl-git 0.749.9af03c9-1
mingw32 mingw-w64-i686-hunspell 1.3.2-1
mingw32 mingw-w64-i686-icu 52.1-2
mingw32 mingw-w64-i686-ilmbase 2.1.0-1
mingw32 mingw-w64-i686-intel-tbb 4.2_20131118-1
mingw32 mingw-w64-i686-irrlicht 1.8.1-1
mingw32 mingw-w64-i686-isl 0.12.1-1
mingw32 mingw-w64-i686-jasper 1.900.1-1
mingw32 mingw-w64-i686-lcms 1.19-1
mingw32 mingw-w64-i686-lcms2 2.5-1
mingw32 mingw-w64-i686-libarchive 3.1.2-2
mingw32 mingw-w64-i686-libassuan 2.1.1-1
mingw32 mingw-w64-i686-libfbclient 2.5.2.26540-1
mingw32 mingw-w64-i686-libffi 3.0.13-1
mingw32 mingw-w64-i686-libgcrypt 1.5.3-1
mingw32 mingw-w64-i686-libgnurx 2.5.1-1
mingw32 mingw-w64-i686-libgpg-error 1.12-1
mingw32 mingw-w64-i686-libiconv 1.14-1
mingw32 mingw-w64-i686-libidn 1.28-1
mingw32 mingw-w64-i686-libjpeg-turbo 1.3.0-1
mingw32 mingw-w64-i686-libksba 1.3.0-1
mingw32 mingw-w64-i686-libmangle-svn 4.0.0.6390-1
mingw32 mingw-w64-i686-libmariadbclient 5.5.34-1
mingw32 mingw-w64-i686-libmng 2.0.2-1
mingw32 mingw-w64-i686-libogg 1.3.1-1
mingw32 mingw-w64-i686-libpng 1.6.7-1
mingw32 mingw-w64-i686-librsvg 2.40.1-1
mingw32 mingw-w64-i686-libssh2 1.4.3-1
mingw32 mingw-w64-i686-libtasn1 3.4-1
mingw32 mingw-w64-i686-libtiff 4.0.3-1
mingw32 mingw-w64-i686-libtool 2.4.2-1
mingw32 mingw-w64-i686-libvorbis 1.3.3-1
mingw32 mingw-w64-i686-libwebp 0.3.1-1
mingw32 mingw-w64-i686-libxml2 2.9.1-1
mingw32 mingw-w64-i686-libxslt 1.1.28-1
mingw32 mingw-w64-i686-libyaml 0.1.4-1
mingw32 mingw-w64-i686-llvm-svn r197969-1
mingw32 mingw-w64-i686-lua 5.2.2-1
mingw32 mingw-w64-i686-lzo2 2.06-1
mingw32 mingw-w64-i686-make 4.0.2288.a4937bc-1
mingw32 mingw-w64-i686-mesa 10.0.1-2
mingw32 mingw-w64-i686-mpc 1.0.1-1
mingw32 mingw-w64-i686-mpfr 3.1.2.p5-1
mingw32 mingw-w64-i686-ncurses 5.9-2
mingw32 mingw-w64-i686-nettle 2.7.1-2
mingw32 mingw-w64-i686-nvidia-cg-toolkit 3.1-1
mingw32 mingw-w64-i686-ogre3d 1.9.0-2
mingw32 mingw-w64-i686-ogreassimp-hg r33.ebcbefe6e081-2
mingw32 mingw-w64-i686-ois 1.3-1
mingw32 mingw-w64-i686-openal 1.15.1-1
mingw32 mingw-w64-i686-opencolorio-git 688.ebd6efc-1
mingw32 mingw-w64-i686-openexr 2.1.0-1
mingw32 mingw-w64-i686-openimageio 1.3.9-1
mingw32 mingw-w64-i686-openssl 1.0.1e-1
mingw32 mingw-w64-i686-p11-kit 0.20.1-1
mingw32 mingw-w64-i686-pcre 8.34-1
mingw32 mingw-w64-i686-perl 5.18.1-2
mingw32 mingw-w64-i686-physfs 2.0.3-1
mingw32 mingw-w64-i686-pkgconf 0.9.3-1
mingw32 mingw-w64-i686-postgresql 9.3.2-1
mingw32 mingw-w64-i686-python2 2.7.6-2
mingw32 mingw-w64-i686-qt-creator 3.0.0-1
mingw32 mingw-w64-i686-qt5 5.2.0-2
mingw32 mingw-w64-i686-qtbinpatcher 1.1.0-2
mingw32 mingw-w64-i686-readline 6.2.004-1
mingw32 mingw-w64-i686-ruby 2.0.0_p353-1
mingw32 mingw-w64-i686-sqlite3 3.8.2-1
mingw32 mingw-w64-i686-tcl 8.6.1-2
mingw32 mingw-w64-i686-tinyxml 2.6.2-1
mingw32 mingw-w64-i686-tk 8.6.1-2
mingw32 mingw-w64-i686-tolua 5.2.0-1
mingw32 mingw-w64-i686-tools-svn 4.0.0.6390-1
mingw32 mingw-w64-i686-w32pth 2.0.5-1
mingw32 mingw-w64-i686-winpthreads-svn 4.0.0.6417-1
mingw32 mingw-w64-i686-xalan-c 1.11-1
mingw32 mingw-w64-i686-xerces-c 3.1.1-1
mingw32 mingw-w64-i686-xz 5.0.5-1
mingw32 mingw-w64-i686-yaml-cpp 0.5.1-1
mingw32 mingw-w64-i686-yaml-cpp0.3 0.3.0-1
mingw32 mingw-w64-i686-zlib 1.2.8-2
mingw32 mingw-w64-i686-zziplib 0.13.62-2
mingw64 mingw-w64-x86_64-FreeImage 3.15.4-2
mingw64 mingw-w64-x86_64-ResIL 1.7.9-1
mingw64 mingw-w64-x86_64-SDL 1.2.15-1
mingw64 mingw-w64-x86_64-SDL2 2.0.1-1
mingw64 mingw-w64-x86_64-allegro 5.0.10-1
mingw64 mingw-w64-x86_64-assimp-git 3.0.1801.3086569-2
mingw64 mingw-w64-x86_64-binutils 2.24-3
mingw64 mingw-w64-x86_64-boost 1.55.0-2
mingw64 mingw-w64-x86_64-bullet 2.82-1
mingw64 mingw-w64-x86_64-bzip2 1.0.6-1
mingw64 mingw-w64-x86_64-cegui 0.8.3-1
mingw64 mingw-w64-x86_64-clang-analyzer-svn r197972-1
mingw64 mingw-w64-x86_64-clang-svn r197972-1
mingw64 mingw-w64-x86_64-cloog 0.18.1-1
mingw64 mingw-w64-x86_64-cmake 2.8.12.1-2
mingw64 mingw-w64-x86_64-collada-dom-svn 2.4.1.r889-1
mingw64 mingw-w64-x86_64-crt-svn 4.0.0.6390-1
mingw64 mingw-w64-x86_64-curl 7.34.0-1
mingw64 mingw-w64-x86_64-dbus 1.6.18-1
mingw64 mingw-w64-x86_64-discount 2.1.6-1
mingw64 mingw-w64-x86_64-dlfcn r19-1
mingw64 mingw-w64-x86_64-dmake 4.12.2-1
mingw64 mingw-w64-x86_64-dumb 0.9.3-1
mingw64 mingw-w64-x86_64-expat 2.1.0-2
mingw64 mingw-w64-x86_64-fftw 3.3.3-1
mingw64 mingw-w64-x86_64-flac 1.3.0-1
mingw64 mingw-w64-x86_64-fontconfig 2.11.0-1
mingw64 mingw-w64-x86_64-freeglut 2.8.1-1
mingw64 mingw-w64-x86_64-freetype 2.5.2-1
mingw64 mingw-w64-x86_64-gc 7.2.d-1
mingw64 mingw-w64-x86_64-gcc 4.8.2-1
mingw64 mingw-w64-x86_64-gcc-ada 4.8.2-1
mingw64 mingw-w64-x86_64-gcc-fortran 4.8.2-1
mingw64 mingw-w64-x86_64-gcc-objc 4.8.2-1
mingw64 mingw-w64-x86_64-gdb 7.6.2-4
mingw64 mingw-w64-x86_64-gettext 0.18.3.1-1
mingw64 mingw-w64-x86_64-giflib 5.0.5-1
mingw64 mingw-w64-x86_64-glew 1.10.0-1
mingw64 mingw-w64-x86_64-glfw 3.0.3-1
mingw64 mingw-w64-x86_64-glib2 2.38.2-1
mingw64 mingw-w64-x86_64-glsl-optimizer-git 0.1.0.60329.fe8a93f-1
mingw64 mingw-w64-x86_64-gmp 5.1.3-2
mingw64 mingw-w64-x86_64-gnupg 1.4.15-1
mingw64 mingw-w64-x86_64-gnutls 3.2.7-1
mingw64 mingw-w64-x86_64-gperf 3.0.4-1
mingw64 mingw-w64-x86_64-gpgme 1.4.3-1
mingw64 mingw-w64-x86_64-headers-svn 4.0.0.6390-1
mingw64 mingw-w64-x86_64-hlsl2glsl-git 0.749.9af03c9-1
mingw64 mingw-w64-x86_64-hunspell 1.3.2-1
mingw64 mingw-w64-x86_64-icu 52.1-2
mingw64 mingw-w64-x86_64-ilmbase 2.1.0-1
mingw64 mingw-w64-x86_64-intel-tbb 4.2_20131118-1
mingw64 mingw-w64-x86_64-irrlicht 1.8.1-1
mingw64 mingw-w64-x86_64-isl 0.12.1-1
mingw64 mingw-w64-x86_64-jasper 1.900.1-1
mingw64 mingw-w64-x86_64-lcms 1.19-1
mingw64 mingw-w64-x86_64-lcms2 2.5-1
mingw64 mingw-w64-x86_64-libarchive 3.1.2-2
mingw64 mingw-w64-x86_64-libassuan 2.1.1-1
mingw64 mingw-w64-x86_64-libfbclient 2.5.2.26540-1
mingw64 mingw-w64-x86_64-libffi 3.0.13-1
mingw64 mingw-w64-x86_64-libgcrypt 1.5.3-1
mingw64 mingw-w64-x86_64-libgnurx 2.5.1-1
mingw64 mingw-w64-x86_64-libgpg-error 1.12-1
mingw64 mingw-w64-x86_64-libiconv 1.14-1
mingw64 mingw-w64-x86_64-libidn 1.28-1
mingw64 mingw-w64-x86_64-libjpeg-turbo 1.3.0-1
mingw64 mingw-w64-x86_64-libksba 1.3.0-1
mingw64 mingw-w64-x86_64-libmangle-svn 4.0.0.6390-1
mingw64 mingw-w64-x86_64-libmariadbclient 5.5.34-1
mingw64 mingw-w64-x86_64-libmng 2.0.2-1
mingw64 mingw-w64-x86_64-libogg 1.3.1-1
mingw64 mingw-w64-x86_64-libpng 1.6.7-1
mingw64 mingw-w64-x86_64-librsvg 2.40.1-1
mingw64 mingw-w64-x86_64-libssh2 1.4.3-1
mingw64 mingw-w64-x86_64-libtasn1 3.4-1
mingw64 mingw-w64-x86_64-libtiff 4.0.3-1
mingw64 mingw-w64-x86_64-libtool 2.4.2-1
mingw64 mingw-w64-x86_64-libvorbis 1.3.3-1
mingw64 mingw-w64-x86_64-libwebp 0.3.1-1
mingw64 mingw-w64-x86_64-libxml2 2.9.1-1
mingw64 mingw-w64-x86_64-libxslt 1.1.28-1
mingw64 mingw-w64-x86_64-libyaml 0.1.4-1
mingw64 mingw-w64-x86_64-llvm-svn r197969-1
mingw64 mingw-w64-x86_64-lua 5.2.2-1
mingw64 mingw-w64-x86_64-lzo2 2.06-1
mingw64 mingw-w64-x86_64-make 4.0.2288.a4937bc-1
mingw64 mingw-w64-x86_64-mesa 10.0.1-2
mingw64 mingw-w64-x86_64-mpc 1.0.1-1
mingw64 mingw-w64-x86_64-mpfr 3.1.2.p5-1
mingw64 mingw-w64-x86_64-ncurses 5.9-2
mingw64 mingw-w64-x86_64-nettle 2.7.1-2
mingw64 mingw-w64-x86_64-nvidia-cg-toolkit 3.1-1
mingw64 mingw-w64-x86_64-ogre3d 1.9.0-2
mingw64 mingw-w64-x86_64-ogreassimp-hg r33.ebcbefe6e081-2
mingw64 mingw-w64-x86_64-ois 1.3-1
mingw64 mingw-w64-x86_64-openal 1.15.1-1
mingw64 mingw-w64-x86_64-opencolorio-git 688.ebd6efc-1
mingw64 mingw-w64-x86_64-openexr 2.1.0-1
mingw64 mingw-w64-x86_64-openimageio 1.3.9-1
mingw64 mingw-w64-x86_64-openssl 1.0.1e-1
mingw64 mingw-w64-x86_64-p11-kit 0.20.1-1
mingw64 mingw-w64-x86_64-pcre 8.34-1
mingw64 mingw-w64-x86_64-perl 5.18.1-2
mingw64 mingw-w64-x86_64-physfs 2.0.3-1
mingw64 mingw-w64-x86_64-pkgconf 0.9.3-1
mingw64 mingw-w64-x86_64-postgresql 9.3.2-1
mingw64 mingw-w64-x86_64-python2 2.7.6-2
mingw64 mingw-w64-x86_64-qt-creator 3.0.0-1
mingw64 mingw-w64-x86_64-qt5 5.2.0-1
mingw64 mingw-w64-x86_64-qtbinpatcher 1.1.0-2
mingw64 mingw-w64-x86_64-readline 6.2.004-1
mingw64 mingw-w64-x86_64-ruby 2.0.0_p353-1
mingw64 mingw-w64-x86_64-sqlite3 3.8.2-1
mingw64 mingw-w64-x86_64-tcl 8.6.1-2
mingw64 mingw-w64-x86_64-tinyxml 2.6.2-1
mingw64 mingw-w64-x86_64-tk 8.6.1-2
mingw64 mingw-w64-x86_64-tolua 5.2.0-1
mingw64 mingw-w64-x86_64-tools-svn 4.0.0.6390-1
mingw64 mingw-w64-x86_64-w32pth 2.0.5-1
mingw64 mingw-w64-x86_64-winpthreads-svn 4.0.0.6417-1
mingw64 mingw-w64-x86_64-xalan-c 1.11-1
mingw64 mingw-w64-x86_64-xerces-c 3.1.1-1
mingw64 mingw-w64-x86_64-xz 5.0.5-1
mingw64 mingw-w64-x86_64-yaml-cpp 0.5.1-1
mingw64 mingw-w64-x86_64-yaml-cpp0.3 0.3.0-1
mingw64 mingw-w64-x86_64-zlib 1.2.8-2
mingw64 mingw-w64-x86_64-zziplib 0.13.62-2
msys setconf 0.6.2-1

以安装mingw-w64-i686-zlib为例，命令如下：
1
Pacman -S mingw-w64-i686-zlib
然后按Y进行确认安装，如下图所示：

安装完毕，即会安装到mingw32目录下。接着测试安装jpeg、png、tiff、webp、freetype、glew、glfw成功。

另外，在Qt Creator(MinGW版本)可以设置环境变量来进行引用这些库，切换到“项目”页面，在“构建环境”下方添加变量，如：
1
2
CPLUS_INCLUDE_PATH  D:\msys2\msys32\mingw32\include
LIBRARY_PATH    D:\msys2\msys32\mingw32\lib
如下图所示：



参考资料：
1.Pacman命令介绍 http://xiexiejiao.cn/linux/pacman-introduce.html
2.GCC使用 http://jianlee.ylinux.org/Computer/C%E5%92%8CGNU%E5%BC%80%E5%8F%91/gcc.html

原文链接：http://blog.csdn.net/akof1314/article/details/17842215









