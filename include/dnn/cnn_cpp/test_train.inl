
#define BOOST_LITTLE_ENDIAN
#include "cstd.h"
#include <iostream>
//#include <boost/timer.hpp>
//#include <boost/progress.hpp>

#include "tiny_cnn.h"
//#define NOMINMAX
//#include "imdebug.h"

void sample1_3layerNN();


typedef network<mse, gradient_descent_levenberg_marquardt> CNN;

// create callback
struct on_enumerate_epoch_t {
  void operator()(CNN& nn) {
#if 0
    std::cout << t.elapsed() << "s elapsed." << std::endl;
    
    result res = nn.test(test_images, test_labels);
    
    std::cout << nn.optimizer().alpha << "," << res.num_success << "/" << res.num_total << std::endl;
    
    nn.optimizer().alpha *= 0.85; // decay learning rate
    nn.optimizer().alpha = MAX(0.00001, nn.optimizer().alpha);
    
    disp.restart(train_images.size());
    t.restart();
#endif
  }
};
on_enumerate_epoch_t on_enumerate_epoch;

struct on_enumerate_minibatch_t {
  void operator()(CNN& nn) {
    //disp += minibatch_size; 
    
    // weight visualization in imdebug
    /*static int n = 0;    
    n+=minibatch_size;
    if (n >= 1000) {
    image img;
    C3.weight_to_image(img);
    imdebug("lum b=8 w=%d h=%d %p", img.width(), img.height(), &img.data()[0]);
    n = 0;
  }*/
  }
};

int test_train(void) {
    CNN nn;
    // construct LeNet-5 architecture
    convolutional_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> C1(32, 32, 5, 1, 6);
    average_pooling_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> S2(28, 28, 6, 2);
    // connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
    static const bool connection[] = {
        O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
        O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
        O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
        X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
        X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
        X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
    };
#undef O
#undef X
    convolutional_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> C3(14, 14, 5, 6, 16, connection_table(connection, 6, 16));
    average_pooling_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> S4(10, 10, 16, 2);
    convolutional_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> C5(5, 5, 5, 16, 120);
    fully_connected_layer<CNN, CNN::Optimizer, CNN::LossFunction, tanh_activation> F6(120, 10);

    assert(C1.param_size() == 156 && C1.connection_size() == 122304);
    assert(S2.param_size() == 12 && S2.connection_size() == 5880);
    assert(C3.param_size() == 1516 && C3.connection_size() == 151600);
    assert(S4.param_size() == 32 && S4.connection_size() == 2000);
    assert(C5.param_size() == 48120 && C5.connection_size() == 48120);

    nn.add(&C1);
    nn.add(&S2);
    nn.add(&C3);
    nn.add(&S4);
    nn.add(&C5);
    nn.add(&F6);

    std::cout << "load models..." << std::endl;

    // load MNIST dataset
    std::vector<label_t> train_labels, test_labels;
    std::vector<vec_t> train_images, test_images;

    parse_mnist_labels("train-labels.idx1-ubyte", &train_labels);
    parse_mnist_images("train-images.idx3-ubyte", &train_images);
    parse_mnist_labels("t10k-labels.idx1-ubyte", &test_labels);
    parse_mnist_images("t10k-images.idx3-ubyte", &test_images);

    std::cout << "start learning" << std::endl;

    int disp = train_images.size();
    
    int minibatch_size = 10;

    nn.optimizer().alpha *= sqrt(minibatch_size);

    on_enumerate_minibatch_t on_enumerate_minibatch;

    // training
    nn.train(train_images, train_labels, minibatch_size, 20, on_enumerate_minibatch, on_enumerate_epoch);

    std::cout << "end training." << std::endl;

    // test and show results
    nn.test(test_images, test_labels).print_detail(std::cout);

    // save networks
    std::ofstream ofs("LeNet-weights");
    ofs << C1 << S2 << C3 << S4 << C5 << F6;
#if 0
#endif
}

#if 0
// learning 3-Layer Networks
void sample1_3layerNN()
{
    const int num_hidden_units = 500;
    typedef network<mse, gradient_descent> neuralnet;
    neuralnet nn;
    fully_connected_layer<neuralnet, tanh_activation> L1(28*28, num_hidden_units);
    fully_connected_layer<neuralnet, tanh_activation> L2(num_hidden_units, 10);

    // load MNIST dataset
    std::vector<label_t> train_labels, test_labels;
    std::vector<vec_t> train_images, test_images;

    parse_mnist_labels("train-labels.idx1-ubyte", &train_labels);
    parse_mnist_images("train-images.idx3-ubyte", &train_images, -1.0, 1.0, 0, 0);
    parse_mnist_labels("t10k-labels.idx1-ubyte", &test_labels);
    parse_mnist_images("t10k-images.idx3-ubyte", &test_images, -1.0, 1.0, 0, 0);

    nn.add(&L1);
    nn.add(&L2);
    nn.optimizer().alpha = 0.001;
    
    boost::progress_display disp(train_images.size());
    boost::timer t;

    // create callback
    auto on_enumerate_epoch = [&](){
        std::cout << t.elapsed() << "s elapsed." << std::endl;

        result res = nn.test(test_images, test_labels);

        std::cout << nn.optimizer().alpha << "," << res.num_success << "/" << res.num_total << std::endl;

        nn.optimizer().alpha *= 0.85; // decay learning rate
        nn.optimizer().alpha = std::max(0.00001, nn.optimizer().alpha);

        disp.restart(train_images.size());
        t.restart();
    };

    auto on_enumerate_data = [&](){ 
        ++disp; 
    };  

    nn.train(train_images, train_labels, 1, 20, on_enumerate_data, on_enumerate_epoch);
}
#endif
