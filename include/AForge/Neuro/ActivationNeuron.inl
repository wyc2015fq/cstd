// AForge Neural Net Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2007-2012
// contacts@aforgenet.com
//


/// <summary>
/// Activation neuron.
/// </summary>
/// 
/// <remarks><para>Activation neuron computes weighted sum of its inputs, adds
/// threshold value and then applies <see cref="ActivationFunction">activation function</see>.
/// The neuron isusually used in multi-layer neural networks.</para></remarks>
/// 
/// <seealso cref="IActivationFunction"/>
/// 
struct ActivationNeuron : Neuron
{
  /// <summary>
  /// Threshold value.
  /// </summary>
  /// 
  /// <remarks>The value is added to inputs weighted sum before it is passed to activation
  /// function.</remarks>
  /// 
  double threshold;// = 0.0;
  
  /// <summary>
  /// Activation function.
  /// </summary>
  /// 
  /// <remarks>The function is applied to inputs weighted sum plus
  /// threshold value.</remarks>
  /// 
  IActivationFunction* function;// = null;
  
  /// <summary>
  /// Threshold value.
  /// </summary>
  /// 
  /// <remarks>The value is added to inputs weighted sum before it is passed to activation
  /// function.</remarks>
  /// 
  
  /// <summary>
  /// Neuron's activation function.
  /// </summary>
  /// 
  
  /// <summary>
  /// Initializes a new instance of the <see cref="ActivationNeuron"/> class.
  /// </summary>
  /// 
  /// <param name="inputs">Neuron's inputs count.</param>
  /// <param name="function">Neuron's activation function.</param>
  /// 
  ActivationNeuron( int inputs, IActivationFunction* function0 )
    : Neuron( inputs )
  {
    function = function0;
  }
  
  /// <summary>
  /// Randomize neuron.
  /// </summary>
  /// 
  /// <remarks>Calls base struct <see cref="Neuron.Randomize">Randomize</see> method
  /// to randomize neuron's weights and then randomizes threshold's value.</remarks>
  /// 
  void Randomize( )
  {
    // randomize weights
    //base.Randomize( );
    // randomize threshold
    threshold = rand()*1./RAND_MAX;
  }
  
  /// <summary>
  /// Computes output value of neuron.
  /// </summary>
  /// 
  /// <param name="input">Input vector.</param>
  /// 
  /// <returns>Returns neuron's output value.</returns>
  /// 
  /// <remarks><para>The output value of activation neuron is equal to value
  /// of nueron's activation function, which parameter is weighted sum
  /// of its inputs plus threshold value. The output value is also stored
  /// in <see cref="Neuron.Output">Output</see> property.</para>
  /// 
  /// <para><note>The method may be called safely from multiple threads to compute neuron's
  /// output value for the specified input values. However, the value of
  /// <see cref="Neuron.Output"/> property in multi-threaded environment is not predictable,
  /// since it may hold neuron's output computed from any of the caller threads. Multi-threaded
  /// access to the method is useful in those cases when it is required to improve performance
  /// by utilizing several threads and the computation is based on the immediate return value
  /// of the method, but not on neuron's output property.</note></para>
  /// </remarks>
  /// 
  /// <exception cref="ArgumentException">Wrong length of the input vector, which is not
  /// equal to the <see cref="Neuron.InputsCount">expected value</see>.</exception>
  /// 
  double Compute( const double* input )
  {
    int i;
    // check for corrent input vector
    // if ( input.Length != inputsCount ) throw new ArgumentException( "Wrong length of the input vector." );
    
    // initial sum value
    double sum = 0.0;
    
    // compute weighted sum of inputs
    for (i = 0; i < inputsCount; i++ )
    {
      sum += weights[i] * input[i];
    }
    sum += threshold;
    
    // local variable to avoid mutlithreaded conflicts
    output = function->Function( sum );
    // assign output property as well (works correctly for single threaded usage)
    return output;
  }
    };
    
