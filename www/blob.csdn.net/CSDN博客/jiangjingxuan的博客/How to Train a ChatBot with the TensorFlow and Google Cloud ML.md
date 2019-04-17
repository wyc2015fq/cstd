# How to Train a ChatBot with the TensorFlow and Google Cloud ML - jiangjingxuan的博客 - CSDN博客





2017年02月07日 09:37:38[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：612








In our [previous article](https://blog.kovalevskyi.com/rnn-based-chatbot-for-6-hours-b847d2d92c43#.k2e4exd1x) we discussed how to train the RNN based chatbot on a AWS GPU instance. Today we will see how we can easily do the training of the same network, on the
[Google Cloud ML](https://cloud.google.com/ml/) and with the [Google Cloud Shell](https://cloud.google.com/shell/). With the Cloud Shell you will not need to do literally anything on your local machine! For the sake of the experiment, exactly the same network as we did in our previous article will be used. However, one can use any
[TensorFlow](https://www.tensorflow.org/) network.




![](https://cdn-images-1.medium.com/max/800/1*sBn8YUEN-iXk-tafJgjGZw.png)


> 
Special thanks to my patrons from my [Patreon](https://www.patreon.com/b0noi) page who made the article possible:

> 
Aleksandr Shepeliev, Sergei Ten, Alexey Polietaiev, Никита Пензин, Igor German, Mikhail Sobol, Карнаухов Андрей, Sergei, Matveev Evgeny, Anton Potemkin.







Even though the material in the article is self-contained, I am strongly advising you to go through each and every link in order to understand what is going on under the hood.

### Pre-Requirements

There is only one pre-requirement that the reader must satisfy in order to perform all the steps here: to have a Google Cloud Account with billing enabled.







Let’s start our journey by answering 2 main questions:
- What is the Google Cloud ML?
- What is the Google Cloud Shell?

### What Is The Google Cloud ML?

In order to understand what it is, let’s look into the official
[definition](https://cloud.google.com/ml/docs/):

> Google Cloud Machine Learning brings the power and flexibility of TensorFlow to the cloud. You can use its components to select and extract features from your data, train your machine
 learning models, and get predictions using the managed resources of Google Cloud Platform.

I don’t know about you, but for me, this definition does not say much. So, let me explain what it actually can do for you:
- deploy your code to the instance that has everything that needed to train the TensorFlow model;
- provide access to the Google Cloud Storage buckets for your code;
- execute your code that is responsible for the training;
- host the model in the cloud for you;
- use the trained model to give the predictions for the future data.

The focus of the current article will be primarily on the first 3 items. In the following articles, we will see how to deploy the trained model to the Google Cloud ML and how to predict data using
 your cloud-hosted model.

### What Is The Google Cloud Shell?

Again, let’s start with the official definition:

> Google Cloud Shell is a shell environment for managing resources hosted on Google Cloud Platform.

And, again, it does not say much to me. So, let me explain what this is actually about. The Cloud Shell is, basically, a provisioned instance in the cloud that:
- has Debian based OS on board;
- to the shell of which you can access via the Web;
- that has everything needed to work with the Google Cloud.

Yes, you got it right, you have a 100% free instance with the shell, that you can have access to from anywhere via the Web.




![](https://cdn-images-1.medium.com/max/800/1*X9-GtckUNPDMlVbnSl-DVQ.png)


But nothing comes for free, in the case of the Cloud Shell — one can access it only via the Web with some limitations (personally, I hate to use any other terminals but
[iTerm](https://www.iterm2.com/)). [I have asked on the StackOverflow](http://stackoverflow.com/questions/41324702/is-it-possible-to-use-the-google-cloud-shell-via-a-ssh) if it is possible to use the Cloud Shell with your own terminal. But for now, there is a way to make your life easier by installing the special
[Chrome plugin](https://chrome.google.com/webstore/detail/ssh-for-google-cloud-plat/ojilllmhjhibplnppnamldakhpmdnibd) that, at least, enables a terminal friendly key bindings.

You can read more about Cloud Shell features
[here](https://cloud.google.com/shell/docs/features).

### The Overall Process

The overall process includes following steps:
- Prepare the Cloud Shell environment for the training
- Prepare the Cloud Storage
- Prepare the data for training
- Prepare the train script
- Test the training process locally
- Train
- Talk to the chatbot

#### Prepare The Cloud Shell Environment For The Training

Now, it’s the best time to open the Cloud Shell. It is super easy, you just need to:
- open your console: [https://console.cloud.google.com/](https://console.cloud.google.com/) and
- press on the shell icon in the right upper corner:


![](https://cdn-images-1.medium.com/max/800/1*KF44Mc0M3q5vHCSYoSci0Q.png)


In case of any problems, here is a small page, that describes how
[to start the Shell](https://cloud.google.com/shell/docs/starting-cloud-shell)with more details.

*All the following examples will be executed in the Cloud Shell.*

Also, if this is the first time when you are going to use the Cloud ML with the Cloud Shell — you need to prepare all the required dependencies. That can be done by just executing the one-liner:
curl https://raw.githubusercontent.com/GoogleCloudPlatform/cloudml-samples/master/tools/setup_cloud_shell.sh | bash
It will install all the required packages. Also, you need to update your PATH variable:
export PATH=${HOME}/.local/bin:${PATH}
One can check whether everything is installed successfully or not by running one simple command:
➜ curl https://raw.githubusercontent.com/GoogleCloudPlatform/cloudml-samples/master/tools/check_environment.py | python  ...
You are using pip version 8.1.1, however version 9.0.1 is available.
You should consider upgrading via the 'pip install --upgrade pip' command.
You are using pip version 8.1.1, however version 9.0.1 is available.
You should consider upgrading via the 'pip install --upgrade pip' command.
Your active configuration is: [cloudshell-12345]
Success! Your environment is configured 
Now it’s time to decide which project you will use in order to train the network. I have the dedicated project for all my experiments with the ML. Anyway, it’s up to you, to choose the project.

Let me show you my commands that allow me to switch between projects easily:
➜  gprojects
PROJECT_ID             NAME            PROJECT_NUMBER
ml-lab-123456          ml-lab          123456789012
...➜  gproject ml-lab-123456
Updated property [core/project].
Now, if you would like to use the same magic, here is what you need to add to your .bashrc/.zshrc/other_rc file:
function gproject() {
  gcloud config set project $1
}function gprojects() {
  gcloud projects list
}
Okey, so now we have prepared the Cloud Shell and have switched to the desired project. What next? If this is the very first time when you are using the Cloud ML with the project, you will be required
 to initialize it. And again, this can be done with just one line:
➜ gcloud beta ml init-projectCloud ML needs to add its service accounts to your project (ml-lab-123456) as Editors. This will enable Cloud Machine Learning to access resources in your project when running your training and prediction jobs.Do you want to continue (Y/n)?  Added serviceAccount:cloud-ml-service@ml-lab-123456-1234a.iam.gserviceaccount.com as an Editor to project 'ml-lab-123456'.
Finally, the Cloud Shell can be considered as one that has been prepared. We can move on to the next step.

#### Prepare The Cloud Storage

First of all, let me explain, why would we need cloud storage? Since we are going to train the model in the cloud it will not have any access to the local file system of your current machine. It means
 that all the required input needs to be stored somewhere in the cloud. As well as, we will need to store the output — somewhere.

Let’s create a brand new bucket that would be used for the training:
➜ PROJECT_NAME=chatbot_generic
➜ TRAIN_BUCKET=gs://${PROJECT_NAME}
➜ gsutil mb ${TRAIN_BUCKET}
Creating gs://chatbot_generic/...
Here I need to tell you something, if you look on the
[official guide](https://cloud.google.com/ml/docs/how-tos/getting-set-up) you will find the following text:

> **Warning:** You must specify a region (like
`**us-central1**`) for your bucket, not a multi-region location (like
`**us**`). Learn more in the
[development environment overview](https://cloud.google.com/ml/docs/concepts/environment-overview#cloud_storage_buckets).

However, if you try to use the regional bucket instead of a multi-regional, the script will fail to write there anything (do not worry,
[the issue has been filed](https://github.com/tensorflow/tensorflow/issues/6493)).

In the perfect world with ponies where everything works as expected, it is very important to set the region here, and the region should be consistent with the region that will be used during the training.
 Otherwise, it might have a negative impact on a speed of the training.

Now we are ready to prepare the input data for the training.

#### Prepare The Data For The Training

This time (compared to the
[previous article](https://blog.kovalevskyi.com/rnn-based-chatbot-for-6-hours-b847d2d92c43#.k2e4exd1x)) we will use a slightly modified version of the script that prepares the input data. I would encourage you to read how the script is working and what it is doing in the README. But for now, here is how you can prepare the input data (you
 might replace “*td src*” with “*mkdir src; cd src*”):
➜ td src
➜ ~/src$ git clone [https://github.com/b0noI/dialog_converter.git](https://github.com/b0noI/dialog_converter.git)Cloning into 'dialog_converter'...
remote: Counting objects: 63, done.
remote: Compressing objects: 100% (4/4), done.
remote: Total 63 (delta 0), reused 0 (delta 0), pack-reused 59
Unpacking objects: 100% (63/63), done.
Checking connectivity... done.➜ ~/src$ cd dialog_converter/
➜ ~/src/dialog_converter$ git checkout converter_that_produces_test_data_as_well_as_train_data
Branch converter_that_produces_test_data_as_well_as_train_data set up to track remote branch converter_that_produces_test_data_as_well_as_train_data from origin.
Switched to a new branch 'converter_that_produces_test_data_as_well_as_train_data'➜ ~/src/dialog_converter$ python converter.py 
➜ ~/src/dialog_converter$ ls
converter.py  LICENSE  movie_lines.txt  README.md  test.a  test.b  train.a  train.b
One might be wondering, what is “td”? It is actually a short form of “to dir” and it’s one of the most frequently used commands. In order to use it you need to update your rc file with the following
 code:
function td() {
  mkdir $1
  cd $1
}
This time we will improve quality of our model by splitting our data set into 2 groups: training and test. That’s why we can see 4 files instead of 2, how it was during the process described in the
 prev article.

Ok, now we have data, let’s upload it to the bucket:
➜ ~/src/dialog_converter$ gsutil cp test.* ${TRAIN_BUCKET}/inputCopying file://test.a [Content-Type=application/octet-stream]...
Copying file://test.b [Content-Type=chemical/x-molconn-Z]...                    
\ [2 files][  2.8 MiB/  2.8 MiB]      0.0 B/s                                   
Operation completed over 2 objects/2.8 MiB.                                      ➜ ~/src/dialog_converter$ gsutil cp train.* ${TRAIN_BUCKET}/inputCopying file://train.a [Content-Type=application/octet-stream]...
Copying file://train.b [Content-Type=chemical/x-molconn-Z]...                   - [2 files][ 11.0 MiB/ 11.0 MiB]                                                
Operation completed over 2 objects/11.0 MiB.                                     ➜ ~/src/dialog_converter$ gsutil ls ${TRAIN_BUCKET}gs://chatbot_generic/input/➜ ~/src/dialog_converter$ gsutil ls ${TRAIN_BUCKET}/inputgs://chatbot_generic/input/test.a
gs://chatbot_generic/input/test.b
gs://chatbot_generic/input/train.a
gs://chatbot_generic/input/train.b
#### Prepare The Training Script

At this moment, we can prepare the training script. We will use the
[translate.py](https://github.com/tensorflow/models/blob/master/tutorials/rnn/translate/translate.py), even though, the current implementation does not allow to be used with the Cloud ML, so we will need a small refactoring. As usual, I have created a[feature request](https://github.com/tensorflow/models/issues/814) and provided you with a [branch with all the required changes](https://github.com/b0noI/models/tree/translate_tutorial_supports_google_cloud_ml). Let’s clone it:
➜ ~/src/dialog_converter$ cd ..
➜ ~/src$ git clone [https://github.com/b0noI/models.git](https://github.com/b0noI/models.git)Cloning into 'models'...
remote: Counting objects: 1813, done.
remote: Compressing objects: 100% (39/39), done.
remote: Total 1813 (delta 24), reused 0 (delta 0), pack-reused 1774
Receiving objects: 100% (1813/1813), 49.34 MiB | 39.19 MiB/s, done.
Resolving deltas: 100% (742/742), done.
Checking connectivity... done.➜ ~/src$ cd models/
➜ ~/src/models$ git checkout translate_tutorial_supports_google_cloud_mlBranch translate_tutorial_supports_google_cloud_ml set up to track remote branch translate_tutorial_supports_google_cloud_ml from origin.
Switched to a new branch 'translate_tutorial_supports_google_cloud_ml'➜ ~/src/models$ cd tutorials/rnn/translate/
Again, pay attention that we are not using the
**master** branch!

#### Test The Training Script Locally

Since the remote training costs money it might be a good idea to test the training locally. The problem here is that local training of our network definitely will kill the Cloud Shell instance. And
 you would have to restart it. Do not worry, nothing will be lost in the case if this happens, but still, it’s probably not something that we would want. Luckily, our script includes a self testing mode that we can use. Let’s start the training locally in a
 self-test mode:
➜ ~/src/models/tutorials/rnn/translate$ cd ..
➜ ~/src/models/tutorials/rnn$ gcloud beta ml local train \
>   --package-path=translate \
>   --module-name=translate.translate \
>   -- \
>   --self_testSelf-test for neural translation model.
Pay attention to the folder from which we are executing the command.

Looks like that a self-test has been finished successfully. Let’s talk a little bit about keys that we are using here:
- package-path — the path to the python package that needs to be deployed to the remote server in order to execute the training;
- module-name — name of the module that needs to be executed during the training;
- 
“- -” —everything that follows after, will be sent as input arguments to the module;
- self_test — tells the module to run a self-test without actual training.

#### Training

This is the most exciting part. Before we start a training we need to prepare all the required buckets that will be used during the process and set all the local variables:
➜ ~/src/models/tutorials/rnn$ INPUT_TRAIN_DATA_A=${TRAIN_BUCKET}/input/train.a
➜ ~/src/models/tutorials/rnn$ INPUT_TRAIN_DATA_B=${TRAIN_BUCKET}/input/train.b
➜ ~/src/models/tutorials/rnn$ INPUT_TEST_DATA_A=${TRAIN_BUCKET}/input/test.a
➜ ~/src/models/tutorials/rnn$ INPUT_TEST_DATA_B=${TRAIN_BUCKET}/input/test.b➜ ~/src/models/tutorials/rnn$ JOB_NAME=${PROJECT_NAME}_$(date +%Y%m%d_%H%M%S)
➜ ~/src/models/tutorials/rnn$ echo ${JOB_NAME}
chatbot_generic_20161224_203332➜ ~/src/models/tutorials/rnn$ TRAIN_PATH=${TRAIN_BUCKET}/${JOB_NAME}
➜ ~/src/models/tutorials/rnn$ echo ${TRAIN_PATH}
gs://chatbot_generic/chatbot_generic_20161224_203332
The job’s name needs to be unique each time when we start the training. Now let’s change the current folder to translate (do not ask =) ):
➜ ~/src/models/tutorials/rnn$ cd translate/
At this moment we are ready to start the training. Let’s first create a command that we will need to execute in order to discuss the details, before the actual execution:
gcloud beta ml jobs submit training ${JOB_NAME} \
  --package-path=. \
  --module-name=translate.translate \
  --staging-bucket="${TRAIN_BUCKET}" \
  --region=us-central1 \
  -- \
  --from_train_data=${INPUT_TRAIN_DATA_A} \
  --to_train_data=${INPUT_TRAIN_DATA_B} \
  --from_dev_data=${INPUT_TEST_DATA_A} \
  --to_dev_data=${INPUT_TEST_DATA_B} \
  --train_dir="${TRAIN_PATH}" \
  --data_dir="${TRAIN_PATH}" \
  --steps_per_checkpoint=5 \
  --from_vocab_size=45000 \
  --to_vocab_size=45000
Let’s first discuss some new flags of the training command:
- staging-bucket — the bucket, that should be used during the deployment, it makes perfect sense to use the same bucket as for training;
- region — the region where you want to start the process.

Now let’s discuss the new flags that will be passed to our script:
- from_train_data — source “from” that will be used during the training process;
- to_train_data — same, but for “to” source;
- from_dev_data/to_dev_data — same, but for test (or “dev” as it called by the script) data, that will be used to evaluate the loss after training;
- train_dir — the train dir, could be cloud based (YAY! exactly what we need);
- steps_per_checkpoint — how many steps should be executed before saving a checkpoint. 5 is a small value, I have set it to this small value only to verify that the training process goes without any
 problem. Later on I’ll restart the process with the bigger value (200 for example);
- from_vocab_size/to_vocab_size — in order to understand what this is, you need to read prev article. You should find out that the default value (40k) is smaller than the amount of unique words in
 our dialogues. Though, in our case (after lower casing all the dialogs) 45k might be more than actually needed. Will see.

Looks like everything is set to actually start the training, so let’s rock(be patient, it will take some time for the execution to be started)…
➜ ~/src/models/tutorials/rnn/translate$ gcloud beta ml jobs submit training ${JOB_NAME} \
>   --package-path=. \
>   --module-name=translate.translate \
>   --staging-bucket="${TRAIN_BUCKET}" \
>   --region=us-central1 \
>   -- \
>   --from_train_data=${INPUT_TRAIN_DATA_A} \
>   --to_train_data=${INPUT_TRAIN_DATA_B} \
>   --from_dev_data=${INPUT_TEST_DATA_A} \
>   --to_dev_data=${INPUT_TEST_DATA_B} \
>   --train_dir="${TRAIN_PATH}" \
>   --data_dir="${TRAIN_PATH}" \
>   --steps_per_checkpoint=5 \
>   --from_vocab_size=45000 \
>   --to_vocab_size=45000INFO    2016-12-24 20:49:24 -0800       unknown_task            Validating job requirements...
INFO    2016-12-24 20:49:25 -0800       unknown_task            Job creation request has been successfully validated.
INFO    2016-12-24 20:49:26 -0800       unknown_task            Job chatbot_generic_20161224_203332 is queued.
INFO    2016-12-24 20:49:31 -0800       service         Waiting for job to be provisioned.
INFO    2016-12-24 20:49:36 -0800       service         Waiting for job to be provisioned....
INFO    2016-12-24 20:53:15 -0800       service         Waiting for job to be provisioned.
INFO    2016-12-24 20:53:20 -0800       service         Waiting for job to be provisioned.
INFO    2016-12-24 20:53:20 -0800       service         Waiting for TensorFlow to start....
INFO    2016-12-24 20:54:56 -0800       master-replica-0                Successfully installed translate-0.0.0
INFO    2016-12-24 20:54:56 -0800       master-replica-0                Running command: python -m translate.translate --from_train_data=gs://chatbot_generic/input/train.a --to_train_data=gs://chatbot_generic/input/train.b --from_dev_data=gs://chatbot_generic/input/test.a --to_dev_data=gs://chatbot_generic/input/test.b --train_dir=gs://chatbot_generic/chatbot_generic_20161224_203332 --steps_per_checkpoint=5 --from_vocab_size=45000 --to_vocab_size=45000
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Creating vocabulary /tmp/vocab45000 from data gs://chatbot_generic/input/train.b
INFO    2016-12-24 20:56:21 -0800       master-replica-0                  processing line 100000
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Tokenizing data in gs://chatbot_generic/input/train.b
INFO    2016-12-24 20:56:21 -0800       master-replica-0                  tokenizing line 100000
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Tokenizing data in gs://chatbot_generic/input/train.a
INFO    2016-12-24 20:56:21 -0800       master-replica-0                  tokenizing line 100000
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Tokenizing data in gs://chatbot_generic/input/test.b
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Tokenizing data in gs://chatbot_generic/input/test.a
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Creating 3 layers of 1024 units.
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Created model with fresh parameters.
INFO    2016-12-24 20:56:21 -0800       master-replica-0                Reading development and training data (limit: 0).
INFO    2016-12-24 20:56:21 -0800       master-replica-0                  reading data line 100000
You can monitor the state of your training. You just need to open another tab in your Cloud Shell (or the tmux window), create the required variables:
➜ JOB_NAME=chatbot_generic_20161224_213143
➜ gcloud beta ml jobs describe ${JOB_NAME}
...
Now we can actually stop the job and restart it with the default amount of steps per checkpoint (200). The updated command should look like:
➜ ~/src/models/tutorials/rnn/translate$ gcloud beta ml jobs submit training ${JOB_NAME} \
>   --package-path=. \
>   --module-name=translate.translate \
>   --staging-bucket="${TRAIN_BUCKET}" \
>   --region=us-central1 \
>   -- \
>   --from_train_data=${INPUT_TRAIN_DATA_A} \
>   --to_train_data=${INPUT_TRAIN_DATA_B} \
>   --from_dev_data=${INPUT_TEST_DATA_A} \
>   --to_dev_data=${INPUT_TEST_DATA_B} \
>   --train_dir="${TRAIN_PATH}" \
>   --data_dir="${TRAIN_PATH}" \
>   --from_vocab_size=45000 \
>   --to_vocab_size=45000
#### Talk To The Chatbot

Since you can start using the latest checkpoint from any other machine without interrupting or impacting the training process, this is probably the biggest advantage of using the Cloud Storage.

Now, for example, I’m going to show you how to chat with the bot, that is still bing trained, after only 1600 iterations.

Also, this is the only step that can’t be done in the Cloud Shell. Why? If you are really asking me — Cloud Shell was never designed to run a heavy task on it, so it will gloriously die with honor
 and with the OutOfMemory error.

Okay, so here is how you can start chatting from your local machine:
mkdir ~/tmp-datagsutil cp gs://chatbot_generic/chatbot_generic_20161224_232158/translate.ckpt-1600.meta ~/tmp-data...gsutil cp gs://chatbot_generic/chatbot_generic_20161224_232158/translate.ckpt-1600.index ~/tmp-data...gsutil cp gs://chatbot_generic/chatbot_generic_20161224_232158/translate.ckpt-1600.data-00000-of-00001 ~/tmp-data...gsutil cp gs://chatbot_generic/chatbot_generic_20161224_232158/checkpoint ~/tmp-dataTRAIN_PATH=...
python -m translate.translate \
  --data_dir="${TRAIN_PATH}" \
  --train_dir="${TRAIN_PATH}" \
  --from_vocab_size=45000 \
  --to_vocab_size=45000 \
  --decodeReading model parameters from /Users/b0noi/tmp-data/translate.ckpt-1600
> Hi there
you ? . . . . . . . .
> What do you want?
i . . . . . . . . .
> yes, you
i ? . . . . . . . .
> hi
you ? . . . . . . . .
> who are you?
i . . . . . . . . .
> yes you!
what ? . . . . . . . .
> who are you?
i . . . . . . . . .
>
you ' . . . . . . . .
The TRAIN_PATH should point to the “tmp_data” folder, and you need to be in the “models/tutorials/rnn” folder of the model repo.

As you can see chatbot is not super smart after 1600 iterations. If you want to know how the conversation will look like after 50+k iterations please have a look at the
[prev article](https://blog.kovalevskyi.com/rnn-based-chatbot-for-6-hours-b847d2d92c43#.k2e4exd1x), since the main purpose of this one was not to train the perfect chatbot, but to explain how to do it, so anyone can train it better.

### Post factum

I hope that my article helped you to learn what the Google Cloud ML is and how you can use it in order to train your own NN. I also hope that you have enjoyed reading it, and if so, you can support
 me on my [Patreon](https://www.patreon.com/b0noi) page and/or by liking/sharing the article.

If you have spotted any problems while executing the steps, please write a comment to me so I could update the article so no one else would have the same problem.



