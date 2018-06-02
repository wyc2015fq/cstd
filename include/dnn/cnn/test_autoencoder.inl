// 
// from __future__ import division, print_function, absolute_import
// 
// import tensorflow as tf
// import numpy as np
// import matplotlib.pyplot as plt

// Import MNIST data
// from tensorflow.examples.tutorials.mnist import input_data
// mnist = input_data.read_data_sets("MNIST_data", one_hot=True)
// 
// // Parameters
// learning_rate = 0.01
// training_epochs = 20
// batch_size = 256
// display_step = 1
// examples_to_show = 10
// 
// // Network Parameters
// n_hidden_1 = 256 // 1st layer num features
// n_hidden_2 = 128 // 2nd layer num features
// n_input = 784 // MNIST data input (img shape: 28*28)
// 
// // tf Graph input (only pictures)
// X = tf.placeholder("float", [None, n_input])
// 
// weights = {
//     'encoder_h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
//     'encoder_h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
//     'decoder_h1': tf.Variable(tf.random_normal([n_hidden_2, n_hidden_1])),
//     'decoder_h2': tf.Variable(tf.random_normal([n_hidden_1, n_input])),
// }
// biases = {
//     'encoder_b1': tf.Variable(tf.random_normal([n_hidden_1])),
//     'encoder_b2': tf.Variable(tf.random_normal([n_hidden_2])),
//     'decoder_b1': tf.Variable(tf.random_normal([n_hidden_1])),
//     'decoder_b2': tf.Variable(tf.random_normal([n_input])),
// }
// 
// 
// // Building the encoder
// def encoder(x):
//     // Encoder Hidden layer with sigmoid activation //1
//     layer_1 = tf.nn.sigmoid(tf.add(tf.matmul(x, weights['encoder_h1']),
//                                    biases['encoder_b1']))
//     // Decoder Hidden layer with sigmoid activation //2
//     layer_2 = tf.nn.sigmoid(tf.add(tf.matmul(layer_1, weights['encoder_h2']),
//                                    biases['encoder_b2']))
//     return layer_2
// 
// 
// // Building the decoder
// def decoder(x):
//     // Encoder Hidden layer with sigmoid activation //1
//     layer_1 = tf.nn.sigmoid(tf.add(tf.matmul(x, weights['decoder_h1']),
//                                    biases['decoder_b1']))
//     // Decoder Hidden layer with sigmoid activation //2
//     layer_2 = tf.nn.sigmoid(tf.add(tf.matmul(layer_1, weights['decoder_h2']),
//                                    biases['decoder_b2']))
//     return layer_2
// 
// // Construct model
// encoder_op = encoder(X)
// decoder_op = decoder(encoder_op)
// 
// // Prediction
// y_pred = decoder_op
// // Targets (Labels) are the input data.
// y_true = X
// 
// // Define loss and optimizer, minimize the squared error
// cost = tf.reduce_mean(tf.pow(y_true - y_pred, 2))
// optimizer = tf.train.RMSPropOptimizer(learning_rate).minimize(cost)
// 
// // Initializing the variables
// init = tf.global_variables_initializer()
// 
// // Launch the graph
// with tf.Session() as sess:
//     sess.run(init)
//     total_batch = int(mnist.train.num_examples/batch_size)
//     // Training cycle
//     for epoch in range(training_epochs):
//         // Loop over all batches
//         for i in range(total_batch):
//             batch_xs, batch_ys = mnist.train.next_batch(batch_size)
//             // Run optimization op (backprop) and cost op (to get loss value)
//             _, c = sess.run([optimizer, cost], feed_dict={X: batch_xs})
//         // Display logs per epoch step
//         if epoch % display_step == 0:
//             print("Epoch:", '%04d' % (epoch+1),
//                   "cost=", "{:.9f}".format(c))
// 
//     print("Optimization Finished!")
// 
//     // Applying encode and decode over test set
//     encode_decode = sess.run(
//         y_pred, feed_dict={X: mnist.test.images[:examples_to_show]})
//     // Compare original images with their reconstructions
//     f, a = plt.subplots(2, 10, figsize=(10, 2))
//     for i in range(examples_to_show):
//         a[0][i].imshow(np.reshape(mnist.test.images[i], (28, 28)))
//         a[1][i].imshow(np.reshape(encode_decode[i], (28, 28)))
//     f.show()
//     plt.draw()
//     plt.waitforbuttonpress()


static int buildAutoEncoder(struct Net* net, DataSize size, int* pinput) {
  int n_hidden_1 = 256; // 1st layer num features
  int n_hidden_2 = 128; // 2nd layer num features
  int n_input = Data_3DSize(size);
// // Building the encoder
  int t = *pinput = net_input(net, size);
  t = add_fc_layer(net, t, n_hidden_1);
  t = net_sigmoid(net, t);
  if (1) {
    t = add_fc_layer(net, t, n_hidden_2);
    t = net_sigmoid(net, t);
    // // Building the decoder
    t = add_fc_layer(net, t, n_hidden_1);
    t = net_sigmoid(net, t);
  }
  t = add_fc_layer(net, t, n_input);
  t = net_sigmoid(net, t);
  return t;
}
int test_autoencoder() {
  const char* mnist_train_images_file = "train-images.idx3-ubyte";
  const char* mnist_train_labels_file = "train-labels.idx1-ubyte";
  const char* mnist_test_images_file = "t10k-images.idx3-ubyte";
  const char* mnist_test_labels_file = "t10k-labels.idx1-ubyte";
  const char* model_file = "mnist_model.txt";
  const char* param_file = "mnist_param.dat";
  const char* param_file2 = "mnist_param.dat_0.dat";
  struct Net net[1] = {0};
  struct sample_t s[1] = {0};
  int input, output;
  int batch = 64;
  int n_train, n_test;
  g_logfile = "log.txt";
  sys_chdir("D:/code/cnn/mnist");
  if (1) {
    // AutoEncoder
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    success = load_mnist(s, mnist_train_images_file, mnist_train_labels_file);
    assert(success && s->size.n > 0);
    //train data & validate data
    //train
    n_train = s->size.n * 0.9;
    n_test = s->size.n - n_train;
    //net_train_test_init(t, s, mnist_fetch_data, n_train, s->size.n - n_train, batch);
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (0 && file_exist(model_file) && file_exist(param_file2)) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildAutoEncoder(net, dataSize(batch, s->size.c, s->size.w, s->size.h), &input);
      //output = buildFCNNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_save(net, model_file, NULL, -1);
    }
    net_print(net, stdout, 0);
    net_loss(net, output, input, MSELoss);
    //net_loss(net, output, &label, MSELoss);
    //net_train(t, net, input, output, label, model_file, param_file);
    {
      
      int train_start = 0;
      int train_end = n_train;
      float train_loss = 0.0f;
      int train_batches = 0;
      int start = train_start;
      int end = train_end;
      //train
      int epochIdx = 0;
      int n = s->size.n;
      float learningRate = 0.1;
      
      int max_epoch;
      int maxBatches;
      int testAfterBatches;
      //float train_rate;
      float decayRate;
      float minLearningRate;
      
      learningRate = 0.1f;
      decayRate = 0.8f;
      minLearningRate = 0.001f;
      testAfterBatches = 200;
      maxBatches = 10000;
      max_epoch = 100*(1+n_test/(batch*10));
      
      logCritical("begin training...");
      for (; epochIdx < 20; ++epochIdx) {
        //before epoch start, shuffle all train data first
        int batchIdx = 0;
        for (; batchIdx < maxBatches; ++batchIdx) {
          int i = start + batchIdx * batch;
          int len = MIN(end - i, batch);
          float batch_loss;
          if (!mnist_fetch_data(NULL, BLOB(input), NULL, i, len, false)) {
            break;
          }
          FNARG(lr) = learningRate;
          batch_loss = net_forward_backward(net);
          net_updateSGD(net);
          train_loss = moving_average(train_loss, train_batches + 1, batch_loss);
          train_batches++;
          if (batchIdx > 0 && (batchIdx % testAfterBatches) == 0)
          {
            logCritical("%6d/%-6d|batch_loss : %f", i, n_train, train_loss);
            train_loss = 0.0f;
            train_batches = 0;
          }
        }
        net_save(net, model_file, param_file, epochIdx);
        if (batchIdx >= maxBatches) {
          break;
        }
        //update learning rate
        learningRate = MAX(learningRate * decayRate, minLearningRate);
      }
      // success = net.saveModel(modelFilePath);
      logCritical("finished training.");
      logCritical("construct net done.");
      sample_free(s);
      net_free(net);
    }
  }
  return 0;
}