# 深度学习：tensorflow1.9.0 - 去向前方的博客 - CSDN博客





2018年07月12日 16:19:14[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：768








# Major Features And Improvements
- Updated docs for tf.keras: New Keras-based get started and programmers guide page.
- Update tf.keras to the Keras 2.1.6 API.
- Added tf.keras.layers.CuDNNGRU and tf.keras.layers.CuDNNLSTM layers. Try it.
- Adding support of core feature columns and losses to gradient boosted trees estimators.
- The python interface for the TFLite Optimizing Converter has been expanded, and the command line interface (AKA: toco, tflite_convert) is once again included in the standard pip installation.
- Improved data-loading and text processing with: 

    tf.decode_compressed 

    tf.string_strip 

    tf.strings.regex_full_match- Added experimental support for new pre-made Estimators: 

    tf.contrib.estimator.BaselineEstimator 

    tf.contrib.estimator.RNNClassifier 

    tf.contrib.estimator.RNNEstimator- The distributions.Bijector API supports broadcasting for Bijectors with new API changes.

# Breaking Changes
- If you’re opening empty variable scopes; replace variable_scope(”, …) by variable_scope(tf.get_variable_scope(), …).
- Headers used for building custom ops have been moved from site-packages/external into site-packages/tensorflow/include/external.

# Bug Fixes and Other Changes
- 
tfe.Network is deprecated. Please inherit from tf.keras.Model.

- 
Layered variable names have changed in the following conditions: 

      Using tf.keras.layers with custom variable scopes. 

      Using tf.layers in a subclassed tf.keras.Model class. See here for more details- 
tf.data: 

    Dataset.from_generator() now accepts an args list, in order to create nested generators. 

    Dataset.list_files() now produces determinstic results when shuffle=False or a seed is passed. 

    tf.contrib.data.sample_from_datasets() and tf.contrib.data.choose_from_datasets() make it easier to sample or deterministically choose elements from multiple datasets. 

    tf.contrib.data.make_csv_dataset() now supports line breaks in quoted strings, and two infrequently used arguments removed. 

    (C++) DatasetBase::DebugString() is now const. 

    (C++) DatasetBase::MakeIterator() has been renamed to DatasetBase::MakeIteratorInternal(). 

    (C++) IteratorBase::Initialize() method was added to support raising errors during iterator construction.- Eager Execution: 

Added the ability to pause recording operations for gradient computation via tf.GradientTape.stop_recording. 

Updated documentation, introductory notebooks.- 
tf.keras: 

    Move Keras code out of _impl folder and remove API files. 

    tf.keras.Model.save_weights now saves in TensorFlow format by default. 

    Enable dataset iterators to be passed to tf.keras.Model training/eval methods.- 
TensorFlow Debugger (tfdbg) 

    Fix an issue in which the TensorBoard Debugger Plugin could not handle total source file size exceeding gRPC message size limit (4 MB).

- 
tf.contrib: 

    tf.contrib.framework.zero_initializer supports ResourceVariable. 

    Adding “constrained_optimization” to tensorflow/contrib.- Other: 

Add GCS Configuration Ops. 

Changing signature of MakeIterator to enable propagating error status. 

KL divergence for two Dirichlet distributions. 

More consistent GcsFileSystem behavior for certain reads past EOF. 

Update benchmark for tf.scan to match ranges across eager and graph modes. 

Fixed bug in tf.reduce_prod gradient for complex dtypes. 

Allow the use of ‘.’ in variables (e.g. “hparams.parse(‘a.b=1.0’)”), which would previously raise an error. This will correspond to an attribute name with an embedded ‘.’ symbol (e.g. ‘a.b’), which can only be accessed indirectly (e.g. through getattr and setattr). To set this up the user will first need to explicitly add the variable to the hparam object (e.g. “hparams.add_hparam(name=’a.b’, value=0.0)”). 

Benchmark for tf.scan in graph and eager modes. 

Added complex128 support to FFT, FFT2D, FFT3D, IFFT, IFFT2D, and IFFT3D. 

Making ids unique in nn.embedding_lookup_sparse. This helps to reduce RPC calls for looking up the embeddings when there are repeated ids in the batch. 

Support indicator column in boosted trees. 

Prevent tf.gradients() from backpropagating through integer tensors. 

LinearOperator[1D,2D,3D]Circulant added to tensorflow.linalg. 

Conv3D, Conv3DBackpropInput, Conv3DBackpropFilter now supports arbitrary. 

Added tf.train.Checkpoint for reading/writing object-based checkpoints. 

Added LinearOperatorKronecker, a dense-free implementation of the Kronecker Product. 

Allow LinearOperator to broadcast. 

SavedModelBuilder will now deduplicate asset names that point to files with the same basename and the same contents. Note that this may result in new asset files included in SavedModels in cases where assets with the same name but different contents were previously overwriting each other.














