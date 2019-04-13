
# We're on the cusp of deep learning for the masses. You can thank Google later - peghoty - CSDN博客


2013年09月22日 15:21:16[皮果提](https://me.csdn.net/peghoty)阅读数：1559标签：[word2vec																](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)[google																](https://so.csdn.net/so/search/s.do?q=google&t=blog)[deep learning																](https://so.csdn.net/so/search/s.do?q=deep learning&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=google&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)个人分类：[杂七杂八																](https://blog.csdn.net/peghoty/article/category/1521713)
[
																								](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)


作者:**Derrick Harris**2013/08/16
**Summary**

Google researchers have developed new methods for analyzing language using deep learning techniques. They’ve also open sourced an implementation of their work so any researchers can experiment with it. It could be the first of
 many deep learning tools designed for mass consumption (大众消费).
Google silently did something revolutionary on Thursday. It open sourced a tool called word2vec, prepackaged (预先包装的) deep-learning software designed to understand the relationships between words with no human guidance. Just input
 a textual data set and let underlying predictive models get to work learning.
“This is a really, really, really big deal,” saidJeremy Howard, president and chief scientist of data-science competition platform Kaggle. “… It’s going to enable whole new classes of products that have never existed before.” Think of Siri on steroids, for starters, or perhaps emulators (模拟器) that
 could mimic your writing style down to the tone.

**When deep learning works, it works great**

To understand Howard’s excitement, let’s go back a few days. It was Monday and I was watching him give a presentation in Chicago about how deep learning was dominating the competition in Kaggle, the online platform where organization
 present vexing (令人烦恼的) predictive problems and data scientists compete to create the best models. Whenever someone has used a deep learning model to tackle one of the challenges, he told the room, it has performed better than any model ever previously devised
 to tackle that specific problem.
But there’s a catch: deep learning is really hard. So far, only a handful of (少量的) teams in hundreds of Kaggle competitions have used it. Most of them have includedGeoffrey Hintonor have been
 associated with him.

Hinton is a University of Toronto professor who pioneered the use of deep learning for**image recognition**and is now a distinguished engineer at Google, as well. What got Google really interested in Hinton — at least
 to the point where it hired him — was his work in an image-recognition competition called**ImageNet**. For years the contest’s winners had been improving only incrementally on previous results, until Hinton and his team used deep learning to improve
 by an order of magnitude.

**Neural networks: A way-simplified overview**

Deep learning, Howard explained, is essentially a bigger, badder take on the neural network models that have been around for some time. It’s particularly useful for**analyzing image, audio, text, genomic (****基因组****)**and other multidimensional data that doesn’t lend itself well to traditional machine learning techniques.

Neural networks work by analyzing inputs (e.g., words or images) and recognizing the features that comprise them as well as how all those features relate to each other. With images, for example, a neural network model might recognize
 various formations of pixels or intensities of pixels as features.

Trained against a set of labeled data, the output of a neural network might be the classification of an input as a dog or cat, for example. In cases where there is no labeled training data — a process called**self-taught learning**— neural networks can be used to identify the common features of their inputs and group similar inputs even though the models can’t predict what they actually are. Like when Google researchers constructed neural networks that were able to recognize cats and
 human faces without having been trained to do so.

**Stacking neural networks to do deep learning**

In deep learning, multiple neural networks are “stacked” on top of each other, or layered, in order to create models that are even better at prediction because each new layer learns from the ones before it. In Hinton’s approach,
 each layer randomly omits features — a process called “**dropout**” — to minimize the chances the model will overfit itself to just the data upon which it was trained (通过 dropout 来最小化模型产生过拟合的几率). That’s a technical way of saying the model won’t
 work as well when trying to analyze new data.

So dropout or similar techniques are critical to helping deep learning models understand the real causality (因果关系) between the inputs and the outputs, Howard explained during a call on Thursday. It’s likelooking at the same thing
 under the same lighting all the time versus looking at it in different lighting and from different angles. You’ll see new aspects and won’t see others, he said, “But the underlying structure is going to be the same each time.”

Still, it’s difficult to create accurate models and to program them to run on the number of computing cores necessary to process them in a reasonable timeframe. It also can be difficult to train them on enough data to guarantee
 accuracy in an unsupervised environment. That’s why so much of the cutting-edge (前沿) work in the field is still done by experts such as**Hinton**,**Jeff Dean**and**Andrew Ng**, all of whom had or still have strong ties
 to Google.

There are open source tools such as**Theano**and**PyLearn2**that try to minimize the complexity, Howard told the audience on Monday, but a user-friendly, commercialized software package (用户友好的商业软件包)
 could be revolutionary. If data scientists in places outside Google could simply (a relative term if ever there was one) input their multidimensional data and train models to learn it, that could make other approaches to predictive modeling all but obsolete
 (过时的). It wouldn’t be inconceivable (难以置信的), Howard noted, that a software package like this could emerge within the next year.

**Enter word2vec**

Which brings us back to word2vec. Google calls it “an efficient implementation of the continuous bag-of-words and skip-gram architectures for computing vector representations of words.” Those “architectures” are two new natural-language
 processing techniques developed by Google researchers Tomas Mikolov, Ilya Sutskever, and Quoc Le (Google Fellow Jeff Dean was also involved, although modestly, he told me.)**They’re like neural networks, only simpler so they can be trained on larger
 data sets.**

Kaggle’s Howard calls word2vec the “crown jewel” of natural language processing. “It’s the English language compressed down to a list of numbers,” he said.

Word2vec is designed to run on a system as small as a single multicore machine (Google tested its underlying techniques over days across more than 100 cores on its data center servers). Its creators have shown how it can recognize
 the similarities among words (e.g., the countries in Europe) as well as how they’re related to other words (e.g., countries and capitals). It’s able to decipher (解密, 破译) analogical (相似的, 类推的) relationships (e.g., short is to shortest as big is to biggest),
 word classes (e.g.,carnivore (食肉动物) and cormorant (鸬鹚) both relate to animals) and “linguistic
 regularities” (e.g., “vector(‘king’) – vector(‘man’) + vector(‘woman’) is close to vector(‘queen’)).

Right now, the word2vec Google Code page notes, “The linearity of the vector operations seems to weakly hold also for the addition of several vectors, so it is possible to add several word or phrase vectors to form representation
 of short sentences.”

This is accomplished by turning words into numbers that correlate with their characteristics, Howard said. Words that express positive sentiment, adjectives, nouns associated with sporting events — they’ll all have certain numbers
 in common based on how they’re used in the training data (so bigger data is better).

**Smarter models means smarter apps**

If this is all too esoteric (难懂的), think about these methods applied to auto-correct or word suggestions in text-messaging (编写短信) apps. Current methods for doing this might be as simple as suggesting words that are usually paired
 together, Howard explained, meaning a suggestion is could be based solely on the word immediately before it. Using deep-learning-based approaches, a texting app could take into account the entire sentence, for example, because the app would have a better understanding
 of what the all words really mean in context.

Maybe you could average out all the numbers in a tweet, Howard suggested, and get a vector output that would accurately infer the sentiment, subject and level of formality of the tweet. Really, the possibilities are limited only
 to the types of applications people can think up to take advantage of word2vec’s deep understanding of natural language.

The big caveat (警告), however, is researchers and industry data scientists still need to learn how to use word2vec. There hasn’t been a lot of research done on how to best use these types of models, Howard said, and the thousands
 of researchers working on other methods of natural language processing aren’t going to jump ship to Google’s tools overnight. Still, he believes the community will come around andword2vec and its underlying techniques could make all other approaches to
 natural language processing obsolete.

And this is just the start. A year from now, Howard predicts, deep learning will have surpassed (超过) a whole class of algorithms in other fields (i.e., things other than speech recognition, image recognition and natural language
 processing), and a year after that it will be integrated into all sorts of software packages. The only questions — and they’re admittedly (公认地,无可否认地) big ones — is how smart deep learning models can get (and whether they’ll run into another era of hardware
 constraints (硬件约束) that**graphical processing units**helped resolve earlier this millennium) and how accessible software packages like word2vec can make deep learning even for relatively unsophisticated users.

“Maybe in 10 years’ time,” Howard proposed, “we’ll get to that next level.”


原文链接:[http://gigaom.com/2013/08/16/were-on-the-cusp-of-deep-learning-for-the-masses-you-can-thank-google-later/](http://gigaom.com/2013/08/16/were-on-the-cusp-of-deep-learning-for-the-masses-you-can-thank-google-later/)



