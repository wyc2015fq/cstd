# 决策树的三种Hadoop开源实现 - 三少GG - CSDN博客
2013年12月11日 18:18:11[三少GG](https://me.csdn.net/scut1135)阅读数：2713
 It gives users new ability to create **big data decision trees**, and easily extract and perform predictive analytics on data that is stored in the Hadoop Distributed File System (HDFS).
**1.**
### [Implementation of C4.5 Algorithm using Hadoop Map Reduce Paradigm](http://btechfreakz.blogspot.com/2013/04/implementation-of-c45-algorithm-using.html)
**2.**
[Mahout](http://mahout.apache.org/),
 a collection of machine learning algorithms for Hadoop, didn't yet have a [C4.5](http://en.wikipedia.org/wiki/C4.5_algorithm)-like
 implementation for [decision
 trees](http://en.wikipedia.org/wiki/Decision_tree_learning).
[http://ashenfad.blogspot.com/2011/03/decision-trees-hadoop-part-3-results.html](http://ashenfad.blogspot.com/2011/03/decision-trees-hadoop-part-3-results.html)
**3.**
## [Retarget Campaign for Abandoned Shopping Carts with Decision Tree](http://pkghosh.wordpress.com/2013/11/06/retarget-campaign-for-abandoned-shopping-carts-with-decision-tree/)
Posted on[November
 6, 2013](http://pkghosh.wordpress.com/2013/11/06/retarget-campaign-for-abandoned-shopping-carts-with-decision-tree/)by[Pranab](http://pkghosh.wordpress.com/author/pkghosh/)

Rate This
Research has shown that customers who have abandoned shopping carts, [when
 subjected to retargeting email campaign, often come back and in many cases end up buying more](http://seewhy.com/blog/2011/02/04/cart-abandoners-spend-155-more/)than what was originally in the shopping cart.
There are many attributes of such email campaigns. In this post, we will find the attribute values  that produce the  maximum  effectiveness for such  retargeting campaigns, by including some of those attributes.  A Hadoop based decision tree algorithm will
 be used to mine existing retargeting campaign data. The solution is part of my open source project[avenir](https://github.com/pranab/avenir).
## Abandoned Shopping Cart Email Campaign
[This
 post](http://blog.hubspot.com/blog/tabid/6307/bid/32581/7-Clever-Email-Campaigns-That-Get-Customers-Buying-Again.aspx) contains an interesting list of 7 most effective email campaigns. One of them is for abandoned shopping carts to recover potentially lost sale.. There are  various parameters  as listed below that contribute to the effectiveness as listed below
- Timing of the campaign following abandonment 
- Products on the cart
- Amount being spent
- Any promotion in the email
- Any product recommendation in the email, based on what’s in the shopping cart
- Campaign creative
- Customer age
- Customer sex
We will be considering only two of the attributes listed; timing and product
 recommendation. I don’t mean to imply that the other attributes are not as important. The idea is to show the usefulness such tools based on decision tree. In the real data mining project all the attributes above will be taken into account and a full
 blown decision tree will be built.
## Campaign Timing and Product Recommendation
I have combined  two attributes  into one categorical attributes.  The first attributecampaign timing defines the time elapsed since the abandonment
 event and email sent to the customer. We will consider 3 cases: 1 hour,    2 hours and  3 hours.  According to research, most customers go
 cold within a short time after the abandoning the cart.
As additional enticement, the email may include product recommendations based on what’s in the shopping cart.  That’s our second attribute. We will consider the cases: 1.
 Cross sell recommendation 2. Social recommendation, based on what other users have purchased 3. No recommendation.
The 9 values of the categorical attribute that we will call campaign type are obtained by taking all possible pairs of the following values.
|Time elapsed|1 Hr|2 Hr|3 Hr|
|----|----|----|----|
|Recommendation|Cross sell|Social|None|
## Quick Note on Decision Tree
[Decision trees](http://www.cs.cmu.edu/afs/cs.cmu.edu/project/theo-20/www/mlbook/ch3.pdf) have
 been around as a very successful and popular machine learning technique. One reason for it’s popularity is that it yields interpretable result, so that you can extract rules like if
  featureAttr1 < x and featureAttr2 > y then classAttr = z. It can also be thought of as a rule mining system.
The basic idea is to split the data into different sub population based on splitting some attribute, such that each sub population is as homogeneous
 as possible with respect to class attribute. This process is repeated for various attributes to break the sub populations into smaller sub populations ina iterative way.
There are different statistical and information theory based criteria for defining the homogeneity of the data e.g. entropy, gini
 index etc. [Details
 can be found in my earlier post](http://pkghosh.wordpress.com/2013/03/18/analytic-is-your-doctors-friend/).
For numerical attribute, we can create multiple split points between min and max range of the attribute. If you choose n split points, you will have (n+1) segments or splits. For
 categorical attributes we can create splits by breaking the values (cardinality of the categorical attribute)  into mutually exclusive sets of 2 , 3 or more sets.
Our feature attribute campaign type has a cardinality of 9 distinct values. We are going to use a split size of 2. The
 set containing all 9 values will be split into al possible sets of 2 mutually exclusive sets, where each one of them is a split.  Generally, given a a maximum split size n, we will create all splits of size 2, 3 all the way to n.
Our decision tree algorithm will find the split that results in most homogeneous sub populations. Once that is done we will be able conclude that when campaign type is one of several types corresponding to the optimum split, the customer is most likely return
 and make purchase responding to the retarget campaign.
## Decision Tree Map Reduce
Here is some sample input, consisting 1. customer ID 2. campaign type 3. Cost of items in shopping cart and 4. Whether customer returned and completed
 purchase. In our analysis the only  feature attribute we will use is campaign type. The last attribute is the class attribute which is binary i.e. whether customer returned and purchased the items left in the shopping cart.
1988527,1N,119,Y
1225473,3S,316,N
1713851,1C,275,N
1876866,2C,182,Y
1970338,3N,212,Y
1805869,3N,30,N
1852129,2N,310,N
The map reduce implementation is in the class  [ClassPartitionGenerator](https://github.com/pranab/avenir/blob/master/src/main/java/org/avenir/explore/ClassPartitionGenerator.java).
 My [earlier post](http://pkghosh.wordpress.com/2013/03/18/analytic-is-your-doctors-friend/)contains
 details of the implementations. The mapper key output is a tuple consisting of 1. attribute ordinal 2. split 3. split segment index and 4. class attribute
 value. The mapper value output is the count of 1.
We end up having 255 possible splits. Each split is consists 2 mutually exclusive sets of the 9 values of the categorical feature attribute. The general
 problem of generating splits  is ated as follows. If there are  n items that need to split into k mutually exclusive sets, then how many such cases are there. There is [factorial
 based formula](http://www.statlect.com/subon2/compar1.htm) to compute this. It works out to 255 for our case. The mapper processes each record for all the splits. The reducers gathers all the count values
We are using gini index as the splitting criteria. Among
 all the splits, the one with lowest gini index will produce the most homogeneous sub populations. Actually we calculate information gain which is defined as the decrease in gin index. We make 2 runs of the MR. For the first run we calculate gini
 index for the whole population without any splitting attributes being involved. In the second run, we consider different splits and calculate gini index for each split.
Then we take the difference between the results of the first and second  run, which is called the information gain. The split with the highest information
 gain in the best split i.e the most discriminating. For such splits, one of the split sets will correspond to mostly successful campaign, and the other one mostly unsuccessful campaign.
## Combiners Combat Combinatorial Explosion
In each run of the map reduce, we could find candidate splits for multiple attributes as specified through the configuration parameter split.attributes.  This results in large fan out from the mapper. Each
 mapper input record may generate several hundred records as we we process each record for all the splits for all the  attributes and all the possible class attribute values.
For example, an input of size 1 million records may generate  several billion mapper output records. To combat the problem, I have used a combiner, which will significantly cut down the  amount of of data to be shuffled. If there are n attributes,  the number
 of records generated for each input record is as follows.
m  = c * sum(s(i)) | i = 1 to n
where
s(i) = he number of splits for the i th attribute
c = the cardinality the class attribute
With combiner,  each mapper will be emitting m number of records which is significantly less. It’s most likely to be  even smaller than the
 input size.
## Results
Here is some sample output showing the best split along with some other. The fields are 1. attribute ordinal 2. split 3. gain
1,[1C, 2C, 2S]:[1S, 1N, 2N, 3C, 3S, 3N],0.026436046040040653
The highlighted row shows the best split with a gain of .053. The campaign is most effective when the types are 1C and 1S i.e. when the campaign emails
 are sent 1 hour after the shopping cart is abandoned with cross sell or socially recommended products. The result is not surprising and agrees with our intuition.
Since we used 2 parameters as input among many, we have simplistic model and got results that we expected. Had we considered other input parameters,
 we may have found different and interesting result e.g, for certain customer segment, 2 hour delay produced most recovery of abandoned shopping carts.  Here is the [tutorial
 to run this example](https://github.com/pranab/avenir/blob/master/resource/abandoned_shopping_cart_retarget_tutorial.txt).
## Going  Forward
Although I have the core splitting logic of decision tree implemented along with various splitting criteria,what is missing is a driver that that will generate the full tree by running multiple Hadoop jobs iteratively by repeatedly finding new optimum split
 and splitting the data accordingly. When that is done I will be back with another post.
