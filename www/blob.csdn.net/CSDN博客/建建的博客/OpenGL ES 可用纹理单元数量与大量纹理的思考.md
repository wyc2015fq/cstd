# OpenGL ES 可用纹理单元数量与大量纹理的思考 - 建建的博客 - CSDN博客
2018年03月19日 14:53:24[纪建](https://me.csdn.net/u013898698)阅读数：134
OpenGL ES 可用纹理单元数量与大量纹理的思考
太阳火神的美丽人生 ([http://blog.csdn.net/opengl_es](http://blog.csdn.net/opengl_es))
本文遵循“[署名-非商业用途-保持一致](http://creativecommons.org/licenses/by-nc-sa/2.5/cn/)”创作公用协议
[转载请保留此句：太阳火神的美丽人生 -  本博客专注于 敏捷开发及移动和物联设备研究：iOS、Android、Html5、Arduino、pcDuino，否则，出自本博客的文章拒绝转载或再转载，谢谢合作。](http://blog.csdn.net/opengl_es)
2013-12-29更新：
写完下面这些，感觉思路有些清晰了，可是我的中心意思，还没表达出来。
这个情况，实际生活中也发生过，其实应该在跟别人说之前，先自已说出来，
要不然，你以为你明白了，但真正能找到适当的语言来表达出来，才能真正让别人明白。
没有这个人后的准备步骤，那么在人前，就会越说越乱，而自已还感觉很清晰的样子。
不过还有一种可能，
不是你的问题，而是听者，你再怎么用简炼、平实的话来说，都听不懂，
那这个还是你的问题，你为什么要找一个不专业的人，作为你的听众呢？
哈哈哈，有时侯，就是经常发生这样的事情，你还必须向不专业或根本就不粘边的人，说明白技术细节，
这时，最好的办法是别说技术本身，挑他关心的事情来说就好了，
虽然，到了，旁人听不懂你的胡邹什么，但你需要向其说明的人，确听得津津乐道，
因为他只关心他关心的事情，并不关心你和旁人所关心的事情，虽然你和旁人都是技术专家。
切忌，切忌！
切记，切记！
见人说人话，见鬼说鬼话，无论用中文，还是英文，最终解答清楚对方关心的是最重要的，
即对方心里的想法，而非他嘴上说的，因为他毕竟不专业，说出的问题，可能都是比萨斜塔......
纹理单元是 OpenGL ES 的一个状态，表示当前要处理的该单元目标；
简单地说，是个容器，它本身并没有内容，是需要装入其它内容的，即纹理对象。
一个纹理贴图，可以创建一个纹理对象，该纹理对象是缓存在 GPU 的内存中的，可以同时有很多个纹理对象缓存在 GPU 中，以减少 CPU 与 GPU 之间的数据交换，提高效率。
这么多个纹理对象，当前渲染引擎处理某个片元时，该用哪一个呢？
那要看你把它装入哪个纹理单元了。
而且创建了一个纹理对象后，要给该纹理对象填充贴图图片的数据，也得先把纹理对象绑到一个纹理单元上，再针对这个纹理单元来操作。
就像是你拿瓶子去打小烧酒一样，直接往你的瓶子里倒，这不合规矩，那样会洒，算谁的呀！
所以得先弄个漏斗插到瓶口上，然后往漏斗里倒酒。
一家店可能就那么两三个漏斗，但瓶子可就多了去了，不过记住，要操作纹理对象，一定要先绑到纹理单元上，或者简单地理解为，料要夹在机床上，才能加工。
从下面的声明中，可以看得出，一共有32个可用的漏斗，或者是32个机床，每个机床旁边堆了多少块料，这个就不知道了，而且一个机床旁的原料，可以放到另一个机床上加工。
看来机床和待加工的原料，这种比喻比较贴切一些。
自个儿理解去吧，我可以很肯定按这个比喻，我能很好的理解。
至于做到，还在进行时，希望在田野上！
**[objc]**[view plain](http://blog.csdn.net/opengl_es/article/details/17652717#)[copy](http://blog.csdn.net/opengl_es/article/details/17652717#)
- // 声明纹理对象句柄
- GLuint textureHandle = 0;  
- // 生成纹理对象，并将句柄赋给 textureHandle
- glGenTextures(1, &textureHandle);  
- // 激活当前 GL_TEXTURE_2D 纹理单元为 GL_TEXTURE1，32个同时只能有一个是在处理的
- glActiveTexture(GL_TEXTURE1);  
- // 将纹理对象绑定到 GL_TEXTURE_2D 当前激活的纹理单元 GL_TEXTURE1 上
- // GL_TEXTURE_2D 当前操作台，GL_TEXTURE1 是当前用的漏斗子，textureHandle 是酒瓶子，绑定到当前操作台就是把当前选定的漏斗子插到这个瓶子上
- glBindTexture(GL_TEXTURE_2D, textureHandle);  
- 
- // 一堆不知道干什么的东西，据说可以用来解决锯齿，实际没看到有效果
- glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);  
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
- 
- // 把酒通过这个选定的漏斗 GL_TEXTURE1 灌到瓶子里：最终是图片存储到 GPU 上分配的纹理对象 textureHandle 上去
- glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);  
- // 生成 Mipmap 是针对操作台 GL_TEXTURE_2D 上的选定漏斗 GL_TEXTURE1 所插在的瓶子里的酒的，即处理图片
- glGenerateMipmap(GL_TEXTURE_2D);  
OpenGLES/ES2/gl.h 中有如下定义
**[objc]**[view plain](http://blog.csdn.net/opengl_es/article/details/17652717#)[copy](http://blog.csdn.net/opengl_es/article/details/17652717#)
- /* TextureUnit */
- #define GL_TEXTURE0                                      0x84C0
- #define GL_TEXTURE1                                      0x84C1
- #define GL_TEXTURE2                                      0x84C2
- #define GL_TEXTURE3                                      0x84C3
- #define GL_TEXTURE4                                      0x84C4
- #define GL_TEXTURE5                                      0x84C5
- #define GL_TEXTURE6                                      0x84C6
- #define GL_TEXTURE7                                      0x84C7
- #define GL_TEXTURE8                                      0x84C8
- #define GL_TEXTURE9                                      0x84C9
- #define GL_TEXTURE10                                     0x84CA
- #define GL_TEXTURE11                                     0x84CB
- #define GL_TEXTURE12                                     0x84CC
- #define GL_TEXTURE13                                     0x84CD
- #define GL_TEXTURE14                                     0x84CE
- #define GL_TEXTURE15                                     0x84CF
- #define GL_TEXTURE16                                     0x84D0
- #define GL_TEXTURE17                                     0x84D1
- #define GL_TEXTURE18                                     0x84D2
- #define GL_TEXTURE19                                     0x84D3
- #define GL_TEXTURE20                                     0x84D4
- #define GL_TEXTURE21                                     0x84D5
- #define GL_TEXTURE22                                     0x84D6
- #define GL_TEXTURE23                                     0x84D7
- #define GL_TEXTURE24                                     0x84D8
- #define GL_TEXTURE25                                     0x84D9
- #define GL_TEXTURE26                                     0x84DA
- #define GL_TEXTURE27                                     0x84DB
- #define GL_TEXTURE28                                     0x84DC
- #define GL_TEXTURE29                                     0x84DD
- #define GL_TEXTURE30                                     0x84DE
- #define GL_TEXTURE31                                     0x84DF
- #define GL_ACTIVE_TEXTURE                                0x84E0
