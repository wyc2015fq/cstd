# How to use Deep Learning when you have Limited Data - jiangjingxuan的博客 - CSDN博客





2017年02月03日 14:17:23[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：542
个人分类：[深度学习](https://blog.csdn.net/jiangjingxuan/article/category/6694821)









Disclaimer: I’m building
[nanonets.ai](https://nanonets.ai) to help build ML with less data

> I think AI is akin to building a rocket ship. You need a huge engine and a lot of fuel. If you have a large engine and a tiny amount of fuel, you won’t make it to orbit. If you have
 a tiny engine and a ton of fuel, you can’t even lift off. To build a rocket you need a huge engine and a lot of fuel.

> 
The analogy to deep learning is that the rocket engine is the deep learning models and the fuel is the huge amounts of data we can feed to these algorithms. — [Andrew
 Ng](https://www.wired.com/brandlab/2015/05/andrew-ng-deep-learning-mandate-humans-not-just-machines/)

There has been a recent surge in popularity of Deep Learning, achieving state of the art performance in various taskslike [Language Translation](https://techcrunch.com/2016/09/27/google-unleashes-deep-learning-tech-on-language-with-neural-machine-translation/), playing [Strategy Games](https://blog.google/topics/machine-learning/alphago-machine-learning-game-go/) and [Self Driving Cars](https://devblogs.nvidia.com/parallelforall/deep-learning-self-driving-cars/) requiring millions of data points. One common barrier for using deep learning to solve problems is the amount of data needed to train a model. The requirement of large data arises because of the large number of parameters in the model
 that machines have to learn.

A few examples of number of parameters in these recent models are:


![](https://cdn-images-1.medium.com/max/800/1*s97lxRRv4aJYhlB3RyulPQ.png)


Details of Deep Learning Models

### Neural Networks aka Deep Learning are layered structures which can be stacked together (think LEGO)

Deep Learning is nothing but Large Neural networks, they can be thought of as a flow chart where data comes in from one side and inference/knowledge comes out the other. You can also break the neural
 network, pull it apart and take the inference out from wherever you please. You might get nothing meaningful but you can do it nonetheless eg
[Google DeepDream](https://deepdreamgenerator.com/)


![](https://cdn-images-1.medium.com/max/800/1*fk2ZjHyqYH04rLTgvQYs4A.png)


### Size(Model) ∝ Size(Data) ∝ Complexity(Problem)

There is an interesting almost linear relationship in the amount of data required and the size of the model. Basic reasoning is that your model should be large enough to capture relations in your
 data (eg textures and shapes in images, grammar in text and phonemes in speech) along with specifics of your problem (eg number of categories). Early layers of the model capture high level relations between the different parts of the input (like edges and
 patterns). Later layers capture information that helps make the final decision; usually information that can help discriminate between the desired outputs. Therefore if the complexity of the problem is high (like Image Classification) the number of parameters
 and the amount of data required is also very large.


![](https://cdn-images-1.medium.com/max/800/0*jQ0pyjQn1yzZSARN.png)


What AlexNet sees at every step

### Transfer Learning to the Rescue!

When working on a problem specific to your domain, often the amount of data needed to build models of this size is impossible to find. However models trained on one task capture relations in the data
 type and can easily be reused for different problems in the same domain. This technique is referred to as
[Transfer Learning](https://cs231n.github.io/transfer-learning/).


![](https://cdn-images-1.medium.com/max/800/0*ovwBU8FJHCqqvsOr.gif)


Qiang Yang, Sinno Jialin Pan, “A Survey on Transfer Learning”, *IEEE Transactions on Knowledge & Data Engineering*, vol. 22, no. , pp. 1345–1359, October 2010, doi:10.1109/TKDE.2009.191

> 
Transfer Learning is like the best kept secret that nobody is trying to keep. Everybody in the industry knows about it but nobody outside does.


![](https://cdn-images-1.medium.com/max/800/1*vi6ahYprTO6DfAAwDGMYyg.png)

[Google Trends Machine
 Learning vs Deep Learning vs Transfer Learning](https://www.google.com/trends/explore?date=2014-01-01%202017-01-01&q=Deep%20Learning,Machine%20Learning,Transfer%20Learning#TIMESERIES)
Referring to [Awesome — Most Cited Deep Learning Papers](https://github.com/terryum/awesome-deep-learning-papers) for the top papers in Deep Learning, More than 50% of the papers use some form of Transfer Learning or Pretraining. Transfer Learning becomes more and more applicable for people with limited resources (data and
 compute) unfortunately the idea has not been socialised nearly enough as it should. The people who need it the most don’t know about it yet.

> If Deep Learning is the holy grail and data is the gate keeper, transfer learning is the key.

With transfer learning, we can take a pretrained model, which was trained on a large readily available dataset (trained on a completely different task, with the same input but different output).
 Then try to find layers which output reusable features. We use the output of that layer as input features to train a much smaller network that requires a smaller number of parameters. This smaller network only needs to learn the relations for your specific
 problem having already learnt about patterns in the data from the pretrained model. This way a model trained to
[detect Cats can be reused to Reproduce the work of Van Gogh](https://techcrunch.com/2016/06/24/prisma-uses-ai-to-turn-your-photos-into-graphic-novel-fodder-double-quick/)




![](https://cdn-images-1.medium.com/max/800/1*tECctD6W1FCYDvCHbAWD-A.jpeg)


Another major advantage of using transfer learning is how well the model generalizes. Larger models tend to overfit (ie modeling the data more than the underlying phenomenon) the data and don’t
 work as well when you test it out on unseen data. Since transfer learning allows the model to see different types of data its learning underlying rules of the world better.

> Think of overfitting as memorizing as opposed to learning. — [James
 Faghmous](https://medium.com/@nomadic_mind/new-to-machine-learning-avoid-these-three-mistakes-73258b3848a4)

### Data Reduction because of Transfer Learning

Let’s say you want to end the debate of blue and black vs. white and gold dress. You start collecting images of verified blue black dresses and white gold dresses. To build an accurate model on your
 own like the one mentioned above (with 140M parameters!!), to train this model you will need to find 1.2M images which is an impossible task. So you give transfer learning a shot.

Calculating the number of parameters needed to train for this problem using transfer learning:

No of parameters = [Size(inputs) + 1] * [Size(outputs) + 1]

= [2048+1]*[1+1]~ 4098 parameters

We see a reduction in number of parameters from 1.4*10⁸ to 4*10³ which is 5 orders of magnitude. So we should be fine collecting less than hundred images of dresses. Phew!

If your impatient and can’t wait to find out the actual color of the dress, scroll down to the bottom and see how to build the model for dresses yourself.







### A step by step guide to Transfer Learning — Using a Toy Example for Sentiment Analysis

In this Toy example we have 72 Movie Reviews
- 62 have no assigned sentiment, these will be used to pretrain the model
- 8 have sentiment assigned to it, these will be use to train the model
- 2 have sentiment assigned to it, these will be used to test the models

Since we only have 8 labelled sentences (sentences that have sentiment associated with them) we first pretrain the model to just predict context. If we trained a model on just the 8 sentences it gives
 a 50% accuracy (50% is as good as flipping a coin to predict).

To solve this problem we will use transfer learning, first training a model on 62 sentences. We then use a part of the first model and train a sentiment classifier on top of it. Training on the 8 sentences
 it produces 100% accuracy when testing on the remaining 2.




![](https://cdn-images-1.medium.com/max/600/1*jMPEODvRVOs9fLZg86hs0Q.png)


### Step 1

We will train a net to model the relationship between words. We pass a word found in a sentence and try to predict the words that occur in the same sentence. Here in code, embedding matrix has size
 of vocabulary x embedding_size which stores a vector representation of each word (We are using size 4 here).


![](https://cdn-images-1.medium.com/max/600/1*v7AJifrWYWKcsmUclA_T3Q.png)


### Step 2

We will train this graph such that words occurring in similar context should get similar vector representations. We will preprocess these sentences by removing stop words and tokenizing them. We pass
 a single word at a time and try to minimize distance of its own vector to surrounding words and increase the distance to a few random words not in its context.


![](https://cdn-images-1.medium.com/max/600/1*K19dET_OfJ3ep7nnN8Mlvg.png)


### Step 3

Now we will try to predict the sentiment of a sentence. Where we have 10 (8 training + 2 test) sentences labelled positive and negative. Since the previous model already has vectors learned for all
 the words and the vectors have property of numerically representing context of word this will make predicting sentiment easier.

Instead of using sentences directly, we set the vector of the sentence to the average of all out its words (in actually tasks we would use something like an LSTM instead). The sentence vector will
 be passed as an input and the output will be score of being positive or negative. We will use one hidden layer in between and train model on our labelled sentences. As you can see, only on 10 examples of each, we have achieved 100% test accuracy using this
 model.



Even though this is a toy example we can see the very significant accuracy improvment going from 50% -> 100 using Transfer Learning. To see the entire example and code check here:

[https://gist.github.com/prats226/9fffe8ba08e378e3d027610921c51a78](https://gist.github.com/prats226/9fffe8ba08e378e3d027610921c51a78)

### Some Real Examples of Transfer Learning

In Images: [Image Enhancement](https://github.com/alexjc/neural-enhance), [Style Transfer](http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Gatys_Image_Style_Transfer_CVPR_2016_paper.pdf), [Object Detection](http://papers.nips.cc/paper/5638-faster-r-cnn-towards-real-time-object-detection-with-region-proposal-networks), [Skin Cancer Detection](http://news.stanford.edu/2017/01/25/artificial-intelligence-used-identify-skin-cancer/).

In Text: [Zero Shot Translation](https://research.googleblog.com/2016/11/zero-shot-translation-with-googles.html), [Sentiment Classification](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.383.1327&rep=rep1&type=pdf)







### Difficulty implementing Transfer Learning

Even though it takes less data to build a model it requires significantly more expertise to make it work. If you look at the example above, just count the number of hard coded parameters and imagine
 having to play around with them till the model worked. This makes actual usage of transfer learning tough.

Some of the issues with transfer learning are listed below:
- Finding a large dataset to pretrain on
- Deciding which model to use for pretraining
- Difficult to debug which of the two models is not working
- Not knowing how much additional data is enough to train the model
- Difficulty in deciding where to stop using the pretrained model
- Deciding the numer of layers and number of parameters in the model used on top of the pretrained model
- Hosting and serving the combined models
- Updating the pretrained model when more data or better techniques becomes available

> Finding a data scientist is hard. Finding people who understand who a data scientist is, is equally hard. — [Krzysztof
 Zawadzki](http://www.marketingdistillery.com/2014/08/30/data-science-skill-set-explained/)

### NanoNets make Transfer Learning easier

Having personally experienced these problems we set out to solve them by building an easy to use cloud based Deep Learning service that uses Transfer Learning. It contains a set of pretrained models
 that have been trained on millions of parameters. You upload your own data (or search the web for data), it selects the best model to use for your task, creates a new NanoNet on top of the existing pretrained model and fits the NanoNet to your data.




![](https://cdn-images-1.medium.com/max/800/0*C_2eTc2Hnu2OgXii.)


Transfer Learning with NanoNets (architecture is only for representation)

### Building your first NanoNet (Image Classification)
- Select the categories you want to work with here


![](https://cdn-images-1.medium.com/max/800/1*qozAjdaD4w49BHasLnrvLQ.png)


2. With one click we Search the Web and build a model (you can also Upload Your own Images)


![](https://cdn-images-1.medium.com/max/800/1*GxzeuEnrei2fV6esN7mbYQ.png)


3. Solve the Mystery of the Blue vs Gold Dress (Once the model is ready we give you an easy to use web interface to upload a test image as well as a language agnostic API)



