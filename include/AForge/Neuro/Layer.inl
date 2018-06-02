// AForge Neural Net Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2007-2012
// contacts@aforgenet.com
//


    /// <summary>
    /// Base neural layer class.
    /// </summary>
    /// 
    /// <remarks>This is a base neural layer class, which represents
    /// collection of neurons.</remarks>
    /// 
    
    struct Layer
    {
        /// <summary>
        /// Layer's inputs count.
        /// </summary>
        int inputsCount;// = 0;

        /// <summary>
        /// Layer's neurons count.
        /// </summary>
        int neuronsCount;// = 0;

        /// <summary>
        /// Layer's neurons.
        /// </summary>
        Neuron* neurons;

        /// <summary>
        /// Layer's output vector.
        /// </summary>
        double* output;

        /// <summary>
        /// Layer's inputs count.
        /// </summary>

        /// <summary>
        /// Layer's neurons.
        /// </summary>
        /// 

        /// <summary>
        /// Layer's output vector.
        /// </summary>
        /// 
        /// <remarks><para>The calculation way of layer's output vector is determined by neurons,
        /// which comprise the layer.</para>
        /// 
        /// <para><note>The property is not initialized (equals to <see langword="null"/>) until
        /// <see cref="Compute"/> method is called.</note></para>
        /// </remarks>
        /// 

        /// <summary>
        /// Initializes a new instance of the <see cref="Layer"/> class.
        /// </summary>
        /// 
        /// <param name="neuronsCount">Layer's neurons count.</param>
        /// <param name="inputsCount">Layer's inputs count.</param>
        /// 
        /// <remarks>Protected contructor, which initializes <see cref="inputsCount"/>,
        /// <see cref="neuronsCount"/> and <see cref="neurons"/> members.</remarks>
        /// 
        Layer( int neuronsCount, int inputsCount )
        {
            this->inputsCount = MAX( 1, inputsCount );
            this->neuronsCount = MAX( 1, neuronsCount );
            // create collection of neurons
            neurons = new Neuron[this->neuronsCount];
        }

        /// <summary>
        /// Compute output vector of the layer.
        /// </summary>
        /// 
        /// <param name="input">Input vector.</param>
        /// 
        /// <returns>Returns layer's output vector.</returns>
        /// 
        /// <remarks><para>The actual layer's output vector is determined by neurons,
        /// which comprise the layer - consists of output values of layer's neurons.
        /// The output vector is also stored in <see cref="Output"/> property.</para>
        /// 
        /// <para><note>The method may be called safely from multiple threads to compute layer's
        /// output value for the specified input values. However, the value of
        /// <see cref="Output"/> property in multi-threaded environment is not predictable,
        /// since it may hold layer's output computed from any of the caller threads. Multi-threaded
        /// access to the method is useful in those cases when it is required to improve performance
        /// by utilizing several threads and the computation is based on the immediate return value
        /// of the method, but not on layer's output property.</note></para>
        /// </remarks>
        /// 
        virtual double* Compute( double* input )
        {
            // local variable to avoid mutlithread conflicts
            int i;
            // compute each neuron
            for (i = 0; i < neuronsCount; i++ )
                output[i] = neurons[i].Compute( input );

            // assign output property as well (works correctly for single threaded usage)

            return output;
        }

        /// <summary>
        /// Randomize neurons of the layer.
        /// </summary>
        /// 
        /// <remarks>Randomizes layer's neurons by calling <see cref="Neuron.Randomize"/> method
        /// of each neuron.</remarks>
        /// 
        virtual void Randomize( )
        {
          int i;
            for (i = 0; i < neuronsCount; i++ )
                neurons[i].Randomize( );
        }
    };

