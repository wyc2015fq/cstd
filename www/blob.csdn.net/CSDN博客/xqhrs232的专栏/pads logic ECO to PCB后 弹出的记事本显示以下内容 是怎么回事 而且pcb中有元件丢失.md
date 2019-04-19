# pads logic ECO to PCB后 弹出的记事本显示以下内容 是怎么回事 而且pcb中有元件丢失 - xqhrs232的专栏 - CSDN博客
2015年01月27日 23:27:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1407
原文地址::[http://zhidao.baidu.com/link?url=fcSHtZekIbrCrmXP27i5aSbyJ5BeLD8KqvCobjY3-yqicb4tfifVJHuYBOXpdF-NLtRJ3ZlQvFHAz95ahFm5VK](http://zhidao.baidu.com/link?url=fcSHtZekIbrCrmXP27i5aSbyJ5BeLD8KqvCobjY3-yqicb4tfifVJHuYBOXpdF-NLtRJ3ZlQvFHAz95ahFm5VK)
相关文章
1、PADS layout 把其他图的原件复制过来----[http://zhidao.baidu.com/link?url=NuVKrANY_lWvYPNKdXeg4HCWLLKyDgAa6DpyLBFi02iI3fwGLD2NBQcTm9nEurXtAvBNACM4pDxo9sJJG2hU7a](http://zhidao.baidu.com/link?url=NuVKrANY_lWvYPNKdXeg4HCWLLKyDgAa6DpyLBFi02iI3fwGLD2NBQcTm9nEurXtAvBNACM4pDxo9sJJG2hU7a)
2、pads使用心得交流----[http://zhidao.baidu.com/link?url=kL6EHYeA9g2pz0rGr1skCZ7JHJB7tiroKchFcWyvDC61GjYz-IY_yuzeRehX9IY9zGUQpdf_mkxWyx0vik4Tha](http://zhidao.baidu.com/link?url=kL6EHYeA9g2pz0rGr1skCZ7JHJB7tiroKchFcWyvDC61GjYz-IY_yuzeRehX9IY9zGUQpdf_mkxWyx0vik4Tha)
Because the current design is in default layer mode and the decal is in increased layer mode, you cannot add decal "BSC0805". Open Layer Setup dialog box and change the layer mode of the design to increased layers.
当前PCB使用默认层数，但是你要添加的元件需要增强层支持！
解决方法如下：steup--layer definition，弹出的对话框右下角有个max layer的按钮，点一下然后再点右上角的OK，设置当前PCB为增强层支持，即可使用！
//==================================================
备注::
在 Setup -->Layer Definition  ---> Layers Setup ---> Nonelectrical Layers --> Max Layers
就可以了！
