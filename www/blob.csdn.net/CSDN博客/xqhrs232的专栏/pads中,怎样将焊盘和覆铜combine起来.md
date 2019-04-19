# pads中,怎样将焊盘和覆铜combine起来 - xqhrs232的专栏 - CSDN博客
2015年03月18日 14:41:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4349
原文地址::[http://zhidao.baidu.com/link?url=bPnWHNyqqdDYLPrwXoSyx__PIH81aiT1MFs81nPmKOrw7mjfh2PQNuI-VU6tl4BkSU7Ro8i6SSPF5b0Q0J-PPIBnGa4GE3rgwbYG-O75_hy](http://zhidao.baidu.com/link?url=bPnWHNyqqdDYLPrwXoSyx__PIH81aiT1MFs81nPmKOrw7mjfh2PQNuI-VU6tl4BkSU7Ro8i6SSPF5b0Q0J-PPIBnGa4GE3rgwbYG-O75_hy)

相关文章
1、pads中如何使敷铜完全连接焊盘----[http://zhidao.baidu.com/question/127717016.html?qbl=relate_question_1&word=pads%CF%C2%B6%CB%B5%E3%B8%FA%CD%AD%C6%A4%BA%CF%B2%A2](http://zhidao.baidu.com/question/127717016.html?qbl=relate_question_1&word=pads%CF%C2%B6%CB%B5%E3%B8%FA%CD%AD%C6%A4%BA%CF%B2%A2)
2、PADS2007 做元件封装时如何将铜皮与焊点关联（合并）----[http://zhidao.baidu.com/link?url=uuMBw647IRQLAO6XHxVhs0b5dcce_I4teirVn9Q9IXTQb9Z_mAjBZm-2P6yoBfZY4U4ln_KzN4XY6B0tbxgJr_](http://zhidao.baidu.com/link?url=uuMBw647IRQLAO6XHxVhs0b5dcce_I4teirVn9Q9IXTQb9Z_mAjBZm-2P6yoBfZY4U4ln_KzN4XY6B0tbxgJr_)
3、pads中想在铜皮上放打几个焊盘便于焊接屏蔽罩，如何实现？eco模式放置了焊盘，但不能和铜皮连接？？？？？----[http://zhidao.baidu.com/link?url=AZWQM2OyCkd0gx9cxCIdWVeY2CjqFpM_a01_Z6yM2hkO5HxlhaNsaF7vlsd_y8szVkwxzhYtwOei1Qty1-go8a](http://zhidao.baidu.com/link?url=AZWQM2OyCkd0gx9cxCIdWVeY2CjqFpM_a01_Z6yM2hkO5HxlhaNsaF7vlsd_y8szVkwxzhYtwOei1Qty1-go8a)
在Layout下是不能将焊盘和覆铜combine的，只能将shap图形combine。
要将焊盘和覆铜合并，只有在封装编辑界面来associate。
//====================================================
单一画块copper指定网络就可以了。
如果是灌铜的话，要在“thermals”里面选择“Non-drilled thermals”--“pad shape”下拉里面有矩形，选择“FLOOOD OVER”就可以了。
