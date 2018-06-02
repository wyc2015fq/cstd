
#include "cfile.h"
#include "net.inl"
//#include "train.inl"



int test_linear_regression() {
  int i;
  Net net[1] = {0};
  //构造输入数据（我们用神经网络拟合x_data和y_data之间的关系）
  //x_data = np.linspace(-1,1,300)[:, np.newaxis] //-1到1等分300份形成的二维矩阵
  //noise = np.random.normal(0,0.05, x_data.shape) //噪音，形状同x_data在0-0.05符合正态分布的小数
  //y_data = np.square(x_data)-0.5+noise //x_data平方，减0.05，再加噪音值
  enum {N = 50};
  float x_datap[N];
  float y_datap[N];
  for (i = 0; i < countof(x_datap); ++i) {
    float x = x_datap[i] = i*2./(countof(x_datap)-1)-1;
    y_datap[i] = x - 0.5 + rng_normal(NULL, 0, 0.05);
  }
  
  //输入层（1个神经元）
  int xs = net_addBlob(net, "x_data", 1, 1, 1, countof(x_datap), x_datap);
  int ys = net_addBlob(net, "y_data", 1, 1, 1, countof(y_datap), y_datap);
  
  //隐层（10个神经元）
  int W1, b1, W2, b2;
  int Wx_plus_b1 = net_matmul(net, xs, &W1, &b1, 10); //矩阵xs和W1相乘，然后加上偏置
  int output1 = net_relu(net, Wx_plus_b1); //激活函数使用tf.nn.relu
  //int output1 = Wx_plus_b1;
  
  //输出层（1个神经元）
  int Wx_plus_b2 = net_matmul(net, output1, &W2, &b2, 1);
  int output2 = Wx_plus_b2;
  
  net_rng_gaussian(net, W1, 0, 1);
  net_constant(net, b1, 0.1); //偏置，1*10的矩阵，使用0.1填充
  net_rng_gaussian(net, W2, 0, 1);
  net_constant(net, b2, 0.1);
  
#if 0
  //损失
  loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-output2),reduction_indices=[1])); //在第一维上，偏差平方后求和，再求平均值，来计算损失
  train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss); // 使用梯度下降法，设置步长0.1，来最小化损失
#endif
  
  //训练
  for (i=0; i<1000; ++i) { //训练1000次
    //_,loss_value = sess.run([train_step,loss],feed_dict={xs:x_data,ys:y_data}) //进行梯度下降运算，并计算每一步的损失
    Dtype loss_value = net_trainonce(net, output2, ys, 0.1, 1);
    if(i%50==0) {
    }
      printf("%f\n", loss_value); // 每50步输出一次损失
  }
  return 0;
}
