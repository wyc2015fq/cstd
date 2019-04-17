# [CareerCup] 6.1 Find Heavy Bottle 寻找重瓶子 - Grandyang - 博客园







# [[CareerCup] 6.1 Find Heavy Bottle 寻找重瓶子](https://www.cnblogs.com/grandyang/p/4753743.html)







6.1 You have 20 bottles of pills. 19 bottles have 1.0 gram pills, but one has pills of weight 1.1 grams. Given a scale that provides an exact measurement, how would you find the heavy bottle? You can only use the scale once.



这道题是一道智力题，给我们20个瓶子，里面分别装满了药片，其中有19个瓶子都装了1克的药品，有一个瓶子装了1.1克的药片，我们有一个称，可以精确的称重，但是只允许用一次，让我们找出中的那个。

这题我没想出来，我开始纠结于整瓶称，其实这题可以取若干药品出来称。我们先从简单的开始想，如果只有两瓶，那我们随便从其中一瓶中取一粒出来一称就知道了，那如果有三瓶呢，我们怎么办呢。答案是从第一瓶中取一粒，第二瓶中取两粒，第三瓶中取三粒来称，总共取出了六粒，如果六粒都是一克的话，那么称出来是六克，但是有一瓶是每粒药片1.1克，那么如果第一瓶是较重那瓶，称出来以为6.1克，如果第二瓶较重，则为6.2克，同理若第三瓶较重，则为6.3克，那么这样我们就能根据结果来知道哪瓶较重了。

同理对于20瓶也是一样的，我们从第一瓶取一粒，第二瓶取两粒，第三瓶取三粒，以此类推，第二十瓶取二十粒，如果每瓶都一样重，那称出来应该是210克，那有一瓶中的每粒药片是1.1克，我们根据个数差产生的重量差来确定是哪瓶，计算公式为：(weight - 210) / 0.1












