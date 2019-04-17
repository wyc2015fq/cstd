# 深度学习 吴恩达 课后作业1-2 Logistic Regression with a Neural Network mindset - 别说话写代码的博客 - CSDN博客





2017年11月13日 18:19:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：702
所属专栏：[吴恩达深度学习DeepLearning.ai作业](https://blog.csdn.net/column/details/24629.html)
















# Logistic Regression with a Neural Network mindset

Welcome to your first (required) programming assignment! You will build a logistic regression classifier to recognize cats. This assignment will step you through how to do this with a Neural Network mindset, and
 so will also hone your intuitions about deep learning.

Instructions:
- Do not use loops (for/while) in your code, unless the instructions explicitly ask you to do so.

You will learn to:
- Build the general architecture of a learning algorithm, including:
- Initializing parameters
- Calculating the cost function and its gradient
- Using an optimization algorithm (gradient descent)

- Gather all three functions above into a main model function, in the right order.









## 1 - Packages

First, let's run the cell below to import all the packages that you will need during this assignment.
- [numpy](https://hub.coursera-notebooks.org/user/lzkxxedlrtubjabjzuijhp/notebooks/Week%202/Logistic%20Regression%20as%20a%20Neural%20Network/www.numpy.org) is the fundamental package for scientific
 computing with Python.
- [h5py](http://www.h5py.org/) is a common package to interact with a dataset that is stored on an H5 file.
- [matplotlib](http://matplotlib.org/) is a famous library to plot graphs in Python.
- [PIL](http://www.pythonware.com/products/pil/) and [scipy](https://www.scipy.org/) are used here to test your model with your own
 picture at the end.






In [54]:






















**import**numpy**as**np

**import**matplotlib.pyplot**as**plt

**import**h5py

**import**scipy

**from**PIL**import**Image

**from**scipy**import**ndimage

**from**lr_utils**import**load_dataset



**%**matplotlibinline

























## 2 - Overview of the Problem set

Problem Statement: You are given a dataset ("data.h5") containing:


- a training set of m_train images labeled as cat (y=1) or non-cat (y=0)

- a test set of m_test images labeled as cat or non-cat

- each image is of shape (num_px, num_px, 3) where 3 is for the 3 channels (RGB). Thus, each image is square (height = num_px) and (width = num_px).


You will build a simple image-recognition algorithm that can correctly classify pictures as cat or non-cat.

Let's get more familiar with the dataset. Load the data by running the following code.






In [55]:






















*# Loading the data (cat/non-cat)*

train_set_x_orig,
train_set_y,test_set_x_orig,test_set_y,classes=load_dataset()

























We added "_orig" at the end of image datasets (train and test) because we are going to preprocess them. After preprocessing, we will end up with train_set_x and test_set_x (the labels train_set_y and test_set_y don't need
 any preprocessing).

Each line of your train_set_x_orig and test_set_x_orig is an array representing an image. You can visualize an example by running the following code. Feel free also to change theindexvalue
 and re-run to see other images.






In [56]:
























*# Example of a picture*

index=
24

plt.imshow(train_set_x_orig[index])

print(train_set_x_orig.shape)

print("y = "**+**str(train_set_y[:,index])**+**", it's a '"**+**
classes[np.squeeze(train_set_y[:,index])].decode("utf-8")**+**"' picture.")






















(209, 64, 64, 3)

y = [1], it's a 'cat' picture.

















Many software bugs in deep learning come from having matrix/vector dimensions that don't fit. If you can keep your matrix/vector dimensions straight you will go a long way toward eliminating many bugs.

Exercise:Find the values for:


- m_train (number of training examples)

- m_test (number of test examples)

- num_px (= height = width of a training image)


Remember thattrain_set_x_origis a numpy-array of shape (m_train,
 num_px, num_px, 3). For instance, you can accessm_trainby writingtrain_set_x_orig.shape[0].






In [57]:






















*### START CODE HERE ### (≈ 3 lines of code)*

m_train=
train_set_x_orig.shape[0]

m_test=
test_set_x_orig.shape[0]

num_px=
train_set_x_orig.shape[1]

*### END CODE HERE ###*



print("Number of training examples: m_train = "**+**str(m_train))

print("Number of testing examples: m_test = "**+**str(m_test))

print("Height/Width of each image: num_px = "**+**str(num_px))

print("Each image is of size: ("**+**str(num_px)**+**", "**+**str(num_px)**+**", 3)")

print("train_set_x shape: "**+**str(train_set_x_orig.shape))

print("train_set_y shape: "**+**str(train_set_y.shape))

print("test_set_x shape: "**+**str(test_set_x_orig.shape))

print("test_set_y shape: "**+**str(test_set_y.shape))






















Number of training examples: m_train = 209

Number of testing examples: m_test = 50

Height/Width of each image: num_px = 64

Each image is of size: (64, 64, 3)

train_set_x shape: (209, 64, 64, 3)

train_set_y shape: (1, 209)

test_set_x shape: (50, 64, 64, 3)

test_set_y shape: (1, 50)












Expected Output for m_train, m_test and num_px:
|m_train|209|
|----|----|
|m_test|50|
|num_px|64|
|train_set_x_flatten shape|(12288, 209)|
|train_set_y shape|(1, 209)|
|test_set_x_flatten shape|(12288, 50)|
|test_set_y shape|(1, 50)|
|sanity check after reshaping|[17 31 56 22 33]|
|sigmoid([0, 2])|[ 0.5 0.88079708]|
|w|[[ 0.] [ 0.]]|
|b|0|
|dw|[[ 0.99845601] [ 2.39507239]]|
|db|0.00145557813678|
|cost|5.801545319394553|
|w|[[ 0.19033591] [ 0.12259159]]|
|b|1.92535983008|
|dw|[[ 0.67752042] [ 1.41625495]]|
|db|0.219194504541|
|predictions|[[ 1. 1. 0.]]|
|Cost after iteration 0|0.693147|
|⋮⋮|⋮⋮|
|Train Accuracy|99.04306220095694 %|
|Test Accuracy|70.0 %|






Comment: Training accuracy is close to 100%. This is a good sanity check: your model is working and has high enough capacity to fit the training data. Test error is 68%. It is actually
 not bad for this simple model, given the small dataset we used and that logistic regression is a linear classifier. But no worries, you'll build an even better classifier next week!

Also, you see that the model is clearly overfitting the training data. Later in this specialization you will learn how to reduce overfitting, for example by using regularization. Using the code below (and changing
 theindexvariable) you can look at predictions on pictures of the test set.






In [72]:


























*# Example of a picture that was wrongly classified.*

index=
1

plt.imshow(test_set_x[:,index].reshape((num_px,num_px,3)))

print("y = "**+**str(test_set_y[0,index])**+**", you predicted that it is a \""**+**classes[d["Y_prediction_test"][0,index]].decode("utf-8")**+**"\" picture.")


























y = 1, you predicted that it is a "cat" picture.
















Let's also plot the cost function and the gradients.






In [73]:






















*# Plot learning curve (with costs)*

costs=
np.squeeze(d['costs'])

plt.plot(costs)

plt.ylabel('cost')

plt.xlabel('iterations (per hundreds)')

plt.title("Learning rate ="**+**str(d["learning_rate"]))

plt.show()






























Interpretation: You can see the cost decreasing. It shows that the parameters are being learned. However, you see that you could train the model even more on the training set. Try
 to increase the number of iterations in the cell above and rerun the cells. You might see that the training set accuracy goes up, but the test set accuracy goes down. This is called overfitting.









## 6 - Further analysis (optional/ungraded exercise)

Congratulations on building your first image classification model. Let's analyze it further, and examine possible choices for the learning rateαα.









#### Choice of learning rate

Reminder: In order for Gradient Descent to work you must choose the learning rate wisely. The learning rateααdetermines
 how rapidly we update the parameters. If the learning rate is too large we may "overshoot" the optimal value. Similarly, if it is too small we will need too many iterations to converge to the best values. That's why it is crucial to use a well-tuned learning
 rate.

Let's compare the learning curve of our model with several choices of learning rates. Run the cell below. This should take about 1 minute. Feel free also to try different values than the three we have initialized
 thelearning_ratesvariable to contain, and see what happens.






In [76]:






















learning_rates= [0.01,0.001,0.0001]

models= {}

foriinlearning_rates:

print("learning rate is: "+str(i))

models[str(i)] =model(train_set_x,train_set_y,test_set_x,test_set_y,num_iterations=3000,
learning_rate=i,print_cost=False)

print('\n'+"-------------------------------------------------------"+'\n')



foriinlearning_rates:

plt.plot(np.squeeze(models[str(i)]["costs"]),label=str(models[str(i)]["learning_rate"]))



plt.ylabel('cost')

plt.xlabel('iterations')



legend=
plt.legend(loc='upper center',shadow=True)

frame=
legend.get_frame()

frame.set_facecolor('0.90')

plt.show()






















learning rate is: 0.01

train accuracy: 100.0 %

test accuracy: 68.0 %




-------------------------------------------------------




learning rate is: 0.001

train accuracy: 93.77990430622009 %

test accuracy: 74.0 %




-------------------------------------------------------




learning rate is: 0.0001

train accuracy: 73.6842105263158 %

test accuracy: 50.0 %




-------------------------------------------------------















Interpretation:
- Different learning rates give different costs and thus different predictions results.
- If the learning rate is too large (0.01), the cost may oscillate up and down. It may even diverge (though in this example, using 0.01 still eventually ends up at a good value for the cost).
- A lower cost doesn't mean a better model. You have to check if there is possibly overfitting. It happens when the training accuracy is a lot higher than the test accuracy.
- In deep learning, we usually recommend that you:
- Choose the learning rate that better minimizes the cost function.
- If your model overfits, use other techniques to reduce overfitting. (We'll talk about this in later videos.)










## 7 - Test with your own image (optional/ungraded exercise)

Congratulations on finishing this assignment. You can use your own image and see the output of your model. To do that:


1. Click on "File" in the upper bar of this notebook, then click "Open" to go on your Coursera Hub.

2. Add your image to this Jupyter Notebook's directory, in the "images" folder

3. Change your image's name in the following code

4. Run the code and check if the algorithm is right (1 = cat, 0 = non-cat)!







In [ ]:































## START CODE HERE ## (PUT YOUR IMAGE NAME)

my_image=
"my_image.jpg"# change this to the name of your image file

## END CODE HERE ##



# We preprocess the image to fit your algorithm.

fname=
"images/"+my_image

image=
np.array(ndimage.imread(fname,flatten=False))

my_image=
scipy.misc.imresize(image,size=(num_px,num_px)).reshape((1,num_px*num_px*3)).T

my_predicted_image=
predict(d["w"],d["b"],my_image)



plt.imshow(image)

print("y = "+str(np.squeeze(my_predicted_image))+", your algorithm predicts a \""+classes[int(np.squeeze(my_predicted_image)),].decode("utf-8")+"\" picture.")




















**What to remember from this assignment:**
- Preprocessing the dataset is important.
- You implemented each function separately: initialize(), propagate(), optimize(). Then you built a model().
- Tuning the learning rate (which is an example of a "hyperparameter") can make a big difference to the algorithm. You will see more examples of this later in this course!











Finally, if you'd like, we invite you to try different things on this Notebook. Make sure you submit before trying anything. Once you submit, things you can play with include:


- Play with the learning rate and the number of iterations

- Try different initialization methods and compare the results

- Test other preprocessings (center the data, or divide each row by its standard deviation)










Bibliography:
- [http://www.wildml.com/2015/09/implementing-a-neural-network-from-scratch/](http://www.wildml.com/2015/09/implementing-a-neural-network-from-scratch/)
- [https://stats.stackexchange.com/questions/211436/why-do-we-normalize-images-by-subtracting-the-datasets-image-mean-and-not-the-c](https://stats.stackexchange.com/questions/211436/why-do-we-normalize-images-by-subtracting-the-datasets-image-mean-and-not-the-c)
















- 

























