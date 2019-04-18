# Aerosolve 简介 - 长歌行 - CSDN博客





2017年11月12日 21:49:12[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：241








*这是2016年做的一个PPT，摘录其中一部分放在这儿吧*

![这里写图片描述](https://img-blog.csdn.net/20171112214126136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Aerosolve是AirBnB的机器学习库，AirBnb会用这个库进行定价和匹配房间。在GitHub中可以搜到。

![这里写图片描述](https://img-blog.csdn.net/20171112214242766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

As the places are unique, it’s difficult for pricing. 

由于

Airbnb provided a better dynamic price way for human, so that the customer can make the right choice.

They try to predict the demand with a few features , such as seasonality price…

These features interact in complex ways and can result in machine learning models that are difficult to interpret. 

So we went about building a package to produce machine learning models that facilitate interpretation and understanding. 

![这里写图片描述](https://img-blog.csdn.net/20171112214505971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

The normal price system is easy.
There are million of unique house with different location, size, fitment and so on. And users only have different requirement of serve, food and guideness or help they can get from the houseload.

Some regular factors(such as seasonal weather changes and irregular factors(such as local events)will make the problem even more complicated.

![这里写图片描述](https://img-blog.csdn.net/20171112214553196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Set the rental simply by change the slider. 

Tips about the effect of season or local event.

![这里写图片描述](https://img-blog.csdn.net/20171112214629594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

For exmaple
From this picture, we found that during the SXSW(A music festival held in austin in texas state) .

The demand for accommodation increases, so airbnb will ask the host to open at the peak period.

![这里写图片描述](https://img-blog.csdn.net/20171112214655275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Relationship of price and demand for rent. 

The red line represents the human’s experience. We ususlly thind the demand will reduce when prices increase.  

In aerosolve, we can simpliy add our experience into the system with a text configuration.

The black line is from the model which is trained from the millions of data.

This model with actual data correct the human hypothesis while allowing the man to set the initial value of the variable.

![这里写图片描述](https://img-blog.csdn.net/20171112214718106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

The two list of pictures shows the difference of preference between the pro and guest

On the left, image ordering trained from professional photographer ratings.

On the right, image ordering trained from organic books, clicks and impressions.

The professional photographers tend to prefer pictures of ornate, brightly lit living rooms, while the guests seem to prefer warm colors and cozy bedrooms.

![这里写图片描述](https://img-blog.csdn.net/20171112214736809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

They also create algorithm to automatically generate local neighborhoods information. Here they use KD tree to index the data. With the kd tree, the Airbnb can quickly query or get the statistic information in a certain area.

The kd tree index are also used to compute local features for machine learning models.

![这里写图片描述](https://img-blog.csdn.net/20171112214757182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171112214821112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





