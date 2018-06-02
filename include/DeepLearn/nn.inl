typedef struct {
  int* size;
  int n;
  int activation_function;
  double learningRate;
  double momentum;
  double scaling_learningRate;
  double weightPenaltyL2;
  double nonSparsityPenalty;
  double sparsityTarget;
  double inputZeroMaskedFraction;
  double dropoutFraction;
  int testing;
  int output;
  double* data;
  img_t a[10];
  img_t d[10];
  img_t e[1];
  double L;
} nn_t;

enum {
  fun_tanh_opt,
  fun_sigm,
  fun_softmax,
  fun_linear,
};

#define IMINITMAT(IM, H, W, A)  IMINIT(IM, H, W, A, W*sizeof(*(A)),sizeof(*(A)),1)


int nn_get_arg(nn_t* nn, int iLayer, img_t* W, img_t* vW, img_t* p)
{
  int i, off = 0, t;
  int* size = nn->size;
  double* data = nn->data;
  ASSERT((iLayer + 1) < nn->n);

  for (i = 0; i < iLayer; ++i) {
    off += (size[i] + 1) * size[i + 1] * 2 + size[i + 1];
  }

  data += off;
  t = (size[iLayer] + 1) * size[iLayer + 1];
  IMINITMAT(W, (size[iLayer] + 1), size[iLayer + 1], data);
  data += t;
  IMINITMAT(vW, (size[iLayer] + 1), size[iLayer + 1], data);
  data += t;
  IMINITMAT(p, size[i + 1], 1, data);
  return t;
}

int nnsetup(nn_t* nn, int* size, int n)
{
  //NNSETUP creates a Feedforward Backpropagate Neural Network
  // nn = nnsetup(architecture) returns an neural network structure with n=numel(architecture)
  // layers, architecture being a n x 1 vector of layer sizes e.g. [784 100 10]
  nn->size   = size;
  nn->n      = n;

  nn->activation_function              = fun_tanh_opt;   //  Activation functions of hidden layers: 'sigm' (sigmoid) or 'tanh_opt' (optimal tanh).
  nn->learningRate                     = 2;            //  learning rate Note: typically needs to be lower when using 'sigm' activation function and non-normalized inputs.
  nn->momentum                         = 0.5;          //  Momentum
  nn->scaling_learningRate             = 1;            //  Scaling factor for the learning rate (each epoch)
  nn->weightPenaltyL2                  = 0;            //  L2 regularization
  nn->nonSparsityPenalty               = 0;            //  Non sparsity penalty
  nn->sparsityTarget                   = 0.05;         //  Sparsity target
  nn->inputZeroMaskedFraction          = 0;            //  Used for Denoising AutoEncoders
  nn->dropoutFraction                  = 0;            //  Dropout level (http://www.cs.toronto.edu/~hinton/absps/dropout.pdf)
  nn->testing                          = 0;            //  Internal variable. nntest sets this to one.
  nn->output                           = fun_sigm;       //  output unit 'sigm' (=logistic), 'softmax' and 'linear'
  {
    img_t im[1];
    int i = 0, sz = 0;
    img_t* W = im + i++;
    img_t* vW = im + i++;
    img_t* p = im + i++;
    double a;

    for (i = 0; i < n - 1; ++i) {
      sz += (size[i] + 1) * size[i + 1] * 2 + size[i + 1];
    }

    MYREALLOC(nn->data, sz);

    for (i = 1; i < nn->n; ++i) {
      // weights and weight momentum
      // W{i - 1} = (rand(nn->size(i), nn->size(i - 1)+1) - 0.5) * 2 * 4 * sqrt(6 / (nn->size(i) + nn->size(i - 1)));
      nn_get_arg(nn, i - 1, W, vW, p);
      a = 2 * 4 * sqrt(6. / (size[i] + size[i - 1]));
      randfill(W->h * W->w, W->tt.f8, a, -0.5 * a);
      FILL(vW->h * vW->w, vW->tt.f8, 0);
      // average activations (for use with sparsity)
      FILL(p->h * p->w, p->tt.f8, 0);
      //cvShowMat( "W" , "%I64f", size[i-1]+1, size[i], W, size[i]*sizeof(double), sizeof(double), -1);      cvWaitKey(-1);
    }
  }
  return 0;
}
int imsigm(const img_t* P, img_t* X)
{
  //X = 1./(1+exp(-P));
  int i;
  imsetsamesize(X, P);

  for (i = 0; i < P->h * P->w; ++i) {
    double t = P->tt.f8[i];
    X->tt.f8[i] = 1. / (1 + exp(-t));
  }

  return 0;
}
//
int imtanh_opt(const img_t* A, img_t* f)
{
  //f=1.719*tanh(2/3.*A);
  int i;
  imsetsamesize(f, A);

  for (i = 0; i < A->h * A->w; ++i) {
    double t = A->tt.f8[i];
    f->tt.f8[i] = 1.7159 * tanh(2 / 3.*t);
  }

  return 0;
}

int imdgemm(CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, double alpha, const img_t* A, const img_t* B, double beta, img_t* C)
{
  int M = (TransA == CblasNoTrans) ? A->h : A->w;
  int N = (TransB == CblasNoTrans) ? B->w : B->h;
  int K = (TransA == CblasNoTrans) ? A->w : A->h;
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  img_t im[1] = {0};
  C = (A == C || B == C) ? im : C;
  ASSERT(A->w == B->h);
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A->tt.f8, lda, B->tt.f8, ldb, beta, C->tt.f8, N);

  if (C == im) {
    img_t t;
    CV_SWAP(*im, *C, t);
  }

  imfree(im);
  return 0;
}

int imaddcol_f8(const img_t* x, double value, img_t* a)
{
  img_t im[1] = {0};
  int i, m = x->h;

  if (x == a) {
    imclone2(x, im);
    x = im;
  }

  imsetsize_f8(a, x->h, x->w + 1, 1);
  memcpy2d(a->tt.f8 + 1, a->s, x->tt.f8, x->s, x->h, x->s);

  for (i = 0; i < m; ++i) {
    a->tt.f8[i * a->w] = value;
  }

  imfree(im);
  return 0;
}

//NNFF performs a feedforward pass
// nn = nnff(nn, x, y) returns an neural network structure with updated
// layer activations, error and loss (nn.a, nn.e and nn.L)
int nnff(nn_t* nn, const img_t* x, const img_t* y)
{
  int n = nn->n, m = x->h, i = 0, j, k;
  img_t im[10] = {0};
  img_t* mean_a = im + i++;
  img_t* a = nn->a;
  img_t W[1], vW[1], p[1];

  //a = [ones(m,1) x];
  imaddcol_f8(x, 1, a);

  //feedforward pass
  for (i = 1; i < n - 1; ++i) {
    a = nn->a + i;
    nn_get_arg(nn, i - 1, W, vW, p);
    imdgemm(CblasNoTrans, CblasTrans, 1, a - 1, W, 0, a);

    switch (nn->activation_function) {
    case fun_sigm:
      // Calculate the unit's outputs (including the bias term)
      // a{i} = sigm(nn->a{i - 1} * nn->W{i - 1}');
      imsigm(a, a);

    case fun_tanh_opt:
      // nn->a{i} = tanh_opt(nn->a{i - 1} * nn->W{i - 1}');
      imtanh_opt(a, a);
    }

    //dropout
    if (nn->dropoutFraction > 0) {
      if (nn->testing) {
        //nn->a{i} = nn->a{i}.*(1 - nn->dropoutFraction);
        double t = 1 - nn->dropoutFraction;

        for (j = 0; j < a->h * a->w; ++j) {
          a->tt.f8[j] *= t;
        }
      }
      else {
        //nn->dropOutMask{i} = (rand(size(nn->a{i}))>nn->dropoutFraction);
        //nn->a{i} = nn->a{i}.*nn->dropOutMask{i};
        for (j = 0; j < a->h * a->w; ++j) {
          double t = mtreal3() > nn->dropoutFraction;
          a->tt.f8[j] *= t;
        }
      }
    }

    //calculate running exponential activations for use with sparsity
    if (nn->nonSparsityPenalty > 0) {
      //nn->p{i} = 0.99 * nn->p{i} + 0.01 * mean(nn->a{i}, 1);
      immean_f8(a, OptRow, mean_a);
      ASSERT(mean_a->h == p->h && mean_a->w == p->w);

      for (j = 0; j < p->h * p->w; ++j) {
        a->tt.f8[j] = 0.99 * a->tt.f8[j] + 0.01 * mean_a->tt.f8[j];
      }
    }

    // Add the bias term
    //nn->a{i} = [ones(m,1) nn->a{i}];
    imaddcol_f8(a, 1, a);
  }

  a = nn->a + i;
  nn_get_arg(nn, i - 1, W, vW, p);
  imdgemm(CblasNoTrans, CblasTrans, 1, a - 1, W, 0, a);

  switch (nn->output) {
  case fun_sigm:
    //nn->a{n} = sigm(nn->a{n - 1} * nn->W{n - 1}');
    imsigm(a, a);
    break;

  case fun_linear:
    //nn->a{n} = nn->a{n - 1} * nn->W{n - 1}';
    break;

  case fun_softmax:
    //nn->a{n} = nn->a{n - 1} * nn->W{n - 1}';
    //nn->a{n} = exp(bsxfun(@minus, nn->a{n}, max(nn->a{n},[],2)));
    //nn->a{n} = bsxfun(@rdivide, nn->a{n}, sum(nn->a{n}, 2));
    ASSERT(0);
    break;
  }

  img_t* e = nn->e;
  //error and loss
  //nn->e = y - nn->a{n};
  imminus_f8(y, a, e);
  double L = 0;

  switch (nn->output) {
  case fun_linear:
  case fun_sigm:

    //nn->L = 1/2 * sum(sum(nn->e .^ 2)) / m;
    for (j = 0; j < e->h * e->w; ++j) {
      L += e->tt.f8[j] * e->tt.f8[j];
    }

    break;

  case fun_softmax:

    //nn->L = -sum(sum(y .* log(nn->a{n}))) / m;
    for (j = 0; j < y->h * y->w; ++j) {
      L -= y->tt.f8[j] * log(a->tt.f8[j]);
    }

    break;
  }

  nn->L = L / m;
  imfrees(im, 10);
  return 0;
}

// NNBP performs backpropagation
// nn = nnbp(nn) returns an neural network structure with updated weights 
int nnbp(nn_t* nn) {
#if 0
   int n = nn->n, i = 0, j;
   img_t im[10] = {0};
   img_t* d = nn->d;
   img_t* a = nn->a;
   img_t* e = nn->e;
   a = nn->a + n-1;
   d = nn->d + n-1;
   imsetsize_f8(d, a->h, a->w, 1);
    double sparsityError = 0;
    switch (nn->output) {
    case fun_sigm:
      //d{n} = - nn->e .* (nn->a{n} .* (1 - nn->a{n}));
      for (j=0; j<a->h*a->w; ++j) {
        double t = a->tt.f8[j];
        d->tt.f8[j] = - e->tt.f8[j] * t * (1 - t);
      }
      break;
    case fun_softmax:
    case fun_linear:
      //d{n} = - nn->e;
      for (j=0; j<a->h*a->w; ++j) {
        d->tt.f8[j] = -e->tt.f8[j];
      }
      break;
    }
    for (i = (n - 1); i>=2; --i) {
        // Derivative of the activation function
      switch (nn->activation_function) {
      case fun_sigm:
        //d_act = nn->a{i} .* (1 - nn->a{i});
        break;
      case fun_tanh_opt:
        //d_act = 1.7159 * 2/3 * (1 - 1/(1.7159)^2 * nn->a{i}.^2);
        break;
      }
        
      if(nn->nonSparsityPenalty>0) {
            pi = repmat(nn->p{i}, size(nn->a{i}, 1), 1);
            sparsityError = [zeros(size(nn->a{i},1),1) nn->nonSparsityPenalty * (-nn->sparsityTarget ./ pi + (1 - nn->sparsityTarget) ./ (1 - pi))];
      }
        
        % Backpropagate first derivatives
        if i+1==n % in this case in d{n} there is not the bias term to be removed             
            d{i} = (d{i + 1} * nn->W{i} + sparsityError) .* d_act; % Bishop (5.56)
        else % in this case in d{i} the bias term has to be removed
            d{i} = (d{i + 1}(:,2:end) * nn->W{i} + sparsityError) .* d_act;
        end
        
        if(nn->dropoutFraction>0)
            d{i} = d{i} .* [ones(size(d{i},1),1) nn->dropOutMask{i}];
        end

    }

    for i = 1 : (n - 1)
        if i+1==n
            nn->dW{i} = (d{i + 1}' * nn->a{i}) / size(d{i + 1}, 1);
        else
            nn->dW{i} = (d{i + 1}(:,2:end)' * nn->a{i}) / size(d{i + 1}, 1);      
        end
    end
    imfrees(im, 10);
#endif
    return 0;
}


typedef struct {
  int numepochs; // ÑµÁ·´ÎÊý
  int batchsize;
  int validation;
} nntrain_opt_t;

//NNTRAIN trains a neural net
// [nn, L] = nnff(nn, x, y, opts) trains the neural network nn with input x and
// output y for opts->numepochs epochs, with minibatches of size
// opts->batchsize. Returns a neural network nn with updated activations,
// errors, weights and biases, (nn.a, nn.e, nn.W, nn.b) and L, the sum
// squared error for each training minibatch.
int nntrain(nn_t* nn, const img_t* train_x, const img_t* train_y, nntrain_opt_t* opts, const img_t* val_x, const img_t* val_y, img_t* L)
{
  //assert(isfloat(train_x), 'train_x must be a float');
  //assert(nargin == 4 || nargin == 6,'number ofinput arguments must be 4 or 6')
  img_t im[10] = {0};
  int i = 0, l, n = 1, j;
  img_t* train_e               = im + i++;
  img_t* train_e_frac          = im + i++;
  img_t* val_e                 = im + i++;
  img_t* val_e_frac            = im + i++;
  img_t* fhandle            = im + i++;
  img_t* batch_x            = im + i++;
  img_t* batch_y            = im + i++;

  if (NULL == L) {
    L = im + i++;
  }

  opts->validation = 0;

  if (val_x && val_y) {
    opts->validation = 1;
  }

  int m = train_x->h;

  int batchsize = opts->batchsize;
  int numepochs = opts->numepochs;

  int numbatches = m / batchsize;
  //assert(rem(numbatches, 1) == 0, 'numbatches must be a integer');

  // L = zeros(numepochs*numbatches,1);
  imsetsize_f8(L, numepochs * numbatches, 1, 1);
  int* kk = NULL;
  MYREALLOC(kk, m);

  for (i = 0; i < numepochs; ++i) {
    //tic;
    randperm(m, kk);

    for (l = 0; l < numbatches; ++l) {
      IRECT rc = iRECT2(0, l * batchsize, train_x->w, batchsize);
      imsubcopy(train_x, rc, batch_x);
      imsubcopy(train_y, rc, batch_y);

      //Add noise to input (for use in denoising autoencoder)
      if (nn->inputZeroMaskedFraction > 0) {
        for (j = 0; j < batch_x->h * batch_x->w; ++j) {
          double t = mtreal3() > nn->inputZeroMaskedFraction;
          batch_x->tt.f8[j] *= t;
        }
      }

      nnff(nn, batch_x, batch_y);
#if 0
      nn = nnbp(nn);
      nn = nnapplygrads(nn);

      L(n) = nn->L;
#endif
      n = n + 1;
    }

#if 0
    t = toc;

    if opts->validation == 1
    loss = nneval(nn, loss, train_x, train_y, val_x, val_y);

    str_perf = sprintf('; Full-batch train mse = //f, val mse = //f', loss.train.e(end), loss.val.e(end));
    else {
      loss = nneval(nn, loss, train_x, train_y);
    }

    str_perf = sprintf('; Full-batch train err = //f', loss.train.e(end));
    end

    if ishandle(fhandle) {
      nnupdatefigures(nn, fhandle, loss, opts, i);
    }

    end

    disp(['epoch ' num2str(i) '/' num2str(opts->numepochs) '. Took ' num2str(t) ' seconds' '. Mini-batch mean squared error on training set is ' num2str(mean(L((n - numbatches): (n - 1)))) str_perf]);
    nn->learningRate = nn->learningRate * nn->scaling_learningRate;
    end

#endif
  }

  FREE(kk);
  return 0;
}