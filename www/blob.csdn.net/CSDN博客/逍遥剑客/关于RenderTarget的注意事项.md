# 关于RenderTarget的注意事项 - 逍遥剑客 - CSDN博客
2009年02月11日 13:13:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：8658

1. 设置一个RenderTarget会导致viewport变成跟RenderTarget一样大
2. 反锯齿类型必须跟DepthStencilBuffer一样
3. RenderTarget的类型必须跟DepthStencilBuffer的类型兼容, 可以用IDirect3D9::CheckDepthStencilMatch进行检测
4. DepthStencilBuffer的大小必须>=RenderTarget的大小
5. IDirect3DDevice9::SetRenderTarget的第0个不能为NULL
6. Usage为D3DUSAGE_RENDERTARGET的Texture不能进行反锯齿, 而且Pool必须为D3DPOOL_DEFAULT. 如果想利用RenderTarget做为纹理又想反锯齿, 可以先把场景渲染到一个CreateRenderTarget创建的Surface(或BackBuffer)上, 再用IDirect3DDevice9::StretchRect拷贝到纹理上
7. D3DX提供了一个ID3DXRenderToSurface, 简化了RenderTarget的使用. 注意它的BeginScene跟EndScene与再用IDirect3DDevice9的同名函数对不能嵌套, 因为实际上内部还是调用的IDirect3DDevice9的, 用PIX可以看到它进行了哪些调用. 还有就是这个接口仍然不能反锯齿, 而且每次都要保存/恢复一堆状态, 总觉得不爽
8. RTT不能既做为输入就做为输出目标, 某些显卡上可能只会给一个warning, 有些显卡上则会发生报错/黑屏/死机之类不可预计的事情...另外, Depth stencil texture(参见Hareware shadow map)也有同样的问题, 用完之后要SetTexture(n, NULL)清空, 不然A卡会黑屏/花屏/深度错误, 既使你没有使用, 只要它被寄存器引用了, 显卡还是会当做是正在使用的, 这时就不能做为depth stencil buffer
9. RTT如果想保存到文件中, 是不能直接SaveToTexture的. 需要创建一个OffscreenSurface, 拷贝过去, 再保存. 不过N卡好像不支持DXT1格式的OffscreenSurface, 可以创建Texture, 取其level0的surface代替.
10. N卡在开启了锯齿后冒似所有的RTT都要反锯齿, 不然深度测试会失败-_-
11. Intel的显卡在RTT没有设置DepthBuffer时可能所有绘制全部深度测试失败, 需要关闭深度测试再画.
12. SRGBWRITE不支持Float格式的RT
13. MRT时使用第一个RT的alpha来做alpha test
14. MRT不支持反锯齿, 必须相同bitdepth, 可以不同格式, 必须相同大小
