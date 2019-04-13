
# 速度提升一倍：Facebook本地应用出炉 HTML5惨遭抛弃 - 雷锋网 - CSDN博客


2012年08月24日 11:15:02[leiphone](https://me.csdn.net/leiphone)阅读数：1284标签：[facebook																](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)[html5																](https://so.csdn.net/so/search/s.do?q=html5&t=blog)[ios																](https://so.csdn.net/so/search/s.do?q=ios&t=blog)[产品																](https://so.csdn.net/so/search/s.do?q=产品&t=blog)[css																](https://so.csdn.net/so/search/s.do?q=css&t=blog)[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=css&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=产品&t=blog)个人分类：[业界																](https://blog.csdn.net/leiphone/article/category/873390)
[
																								](https://so.csdn.net/so/search/s.do?q=产品&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=html5&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)
![](http://www.leiphone.com/wp-content/uploads/2012/08/facebook5.jpg)历经6个月的研发，[Facebook](http://www.leiphone.com/tag/facebook)近日对外发布了一款针对[iOS](http://www.leiphone.com/tag/ios)设备而开发的本地应用，其反应速度比旧版提高一倍。其iOS产品经理Mick
 Johnson指出：“现在旧版的Facebook应用有很大的用户群体，但是我们也意识到移动网页对于公司来说更为重要，而植入HTML5的旧版应用达不到群众的期待。”新的版本是采用iOS SDK重新编写打造的，它和旧版在外观上差别不大，但是速度方面有很大的提升，并针对[iPad](http://www.leiphone.com/tag/ipad)重造了部分功能代码。新的应用差不多可以在苹果应用商店下载了。
**一个植入了HTML5代码的旧版Facebook应用并不能达到广大用户的期待。**
对于Facebook的iOS原生应用来说，它在主要在三个方面有很大的速度提升：应用启动、共享新闻滚动还有图片点击查看。其总体速度大约提升了一倍。这个版本部分采用了Facebook Camera和Facebook Messenger两款应用的代码库：其中图片点击查看功能的代码是从Facebook Camera移植过来，而屏幕消息是从Facebook Messenger那克隆过来的。这个原生版本是由一个独立的团队开发，产品经理Johnson表示未来会充分利用公司的代码共享，也会适当向其他团队寻求帮助。在官方的博客上，Facebook大概解释了一下为什么本地编码有利于开发一个更快的应用，但是很多[HTML5](http://www.leiphone.com/tag/html5)所实现出来功能对于本地编码来说是一定的挑战。
![](http://www.leiphone.com/wp-content/uploads/2012/08/115.png)
除了速度的大幅度提升，新版Facebook应用还增加了不少功能，并根据iOS SDK进行代码调整，比如说新的动画实现和手势实现。当中的一个手势功能使得单手浏览更为方便：用户在查看图片的时候，只需要在图片上滑动一下，就可以返回前一个屏幕。另一个功能就是当你浏览共享新闻（News Feed）更新信息的时候，一个名叫”New Stories” 的横幅会浮现在屏幕的最顶端。不过现在尚未清楚的是，这个横幅是否真的用于更新通知，以确保用户能够看到最新的更新消息。不过Facebook Messenger应用的内置横幅是用于显示新的消息，Facebook把这个体验移植到这个新版Facebook应用上也是合理的。
和旧版比起来，新的版本的确快了很多，但是还是有代价的：公司再也不能每日都向公司旗下有所的应用推送更新消息。快速迭代是Facebook的应用优势，不过它需要HTML5的代码配合才能实现。产品经理Johnson表示，Facebook未来会在公司的移动网页上推出六个新功能，目前向苹果应用商店所提交的版本还不够灵活。另外，为了更好的适应“Stories”和其他内容，新版应用的少量模块依然会使用HTML5代码。 随着时间的推移，Facebook未来也会将其他应用向本地化过渡，目前只是忙不过来。最近他们也针对Android版本做出了微小更新。Johnson表示，公司会公平地专注每一个平台。其实用户一直非常迫切期待Facebook能够推出一个原生的iOS应用，因为速度的提升能够带来更好的用户体验。而现在，这个应用来了。
![](http://www.leiphone.com/wp-content/uploads/2012/08/212.png)
**总结：这会估计很多人要趁机吐槽HTML5了。对于移动网页开发来说，HTML5还是不错的选择，不过现在的开发这更多的是采用本地编码+HTML5的混合模式。HTML5在界面表现能力上非常好，要做到同样的效果，本地编码要写一大堆代码，而HTML5只需要简单的CSS就能实现。但在动画表现上，本地编码只需要通过几行简单代码能实现非常华丽的动画，而且非常稳定，用CSS3的话就惨了（编者按：Geek码农****[@涂雅](http://weibo.com/conisyi)****开发体会）。其实二者各有优势，只是用户更在乎速度上的体验，迫使Facebook转向本地编码。不管怎么说，HTML5还是蛮有前景的，不过现在是****[处于炒作高峰期](http://www.leiphone.com/0822-ce6093-html5.html)****，相关的功能效果实现自然跟其他编程语言有很大差距，遭别人鄙视也是正常的。希望大家能宽容对待这门新生技术，请问多少个新生技术一出生就没被人吐槽过？**
Via：[theverge](http://www.theverge.com/2012/8/23/3262782/facebook-for-ios-native-app)

**（****[抽水](http://www.leiphone.com/author/ce6093)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

