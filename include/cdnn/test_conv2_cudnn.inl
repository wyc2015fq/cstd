
#include <cudnn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// http://www.goldsborough.me/cuda/ml/cudnn/c++/2017/10/01/14-37-23-convolutions_with_cudnn/
 
#define checkCUDNN(expression)                                  \
  {                                                             \
    cudnnStatus_t status = (expression);                        \
    if (status != CUDNN_STATUS_SUCCESS) {                       \
	    printf("Error on line %d : %s\n", __LINE__,       \
	    cudnnGetErrorString(status) );            \
	    std::exit(EXIT_FAILURE);                                \
    }                                                           \
 }
 
int test_conv2_cudnn() {
	int gpu_id = 0;
	bool with_sigmoid = false;
#if 0
	if (argc < 2) {
		std::cerr << "usage: conv <image> [gpu=0] [sigmoid=0]" << std::endl;
		std::exit(EXIT_FAILURE);
	}
 
	gpu_id = (argc > 2) ? std::atoi(argv[2]) : 0;
	with_sigmoid = (argc > 3) ? std::atoi(argv[3]) : 0;
#endif
	printf("GPU: %d\n", gpu_id);
 
	printf("With sigmoid: %d\n", with_sigmoid);
	int in_n = 1;
	int in_h = 100;
	int in_w = 100;
	int in_c = 3;
	//cv::Mat image = load_image(argv[1]);
	int ker_h = 3;
	int ker_w = 3;
	int out_n = 0;
	int out_c = 3;
	int out_h = 0;
	int out_w = 0;

	int in_count = in_n * in_c * in_h * in_w;
	int in_bytes = in_count * sizeof(float); 
	float* cpu_input = (float*)malloc(in_bytes);

	if (NULL == cpu_input) {
		printf("cpu_input==NULL\n");
		return 0;
	}
	for (int i = 0; i < in_count; ++i) {
		cpu_input[i] = i;
	}
 
	cudaSetDevice(gpu_id);
 
	cudnnHandle_t cudnn;
	cudnnCreate(&cudnn);
 
	// 输入张量的描述
	cudnnTensorDescriptor_t input_descriptor;
	checkCUDNN(cudnnCreateTensorDescriptor(&input_descriptor));
	checkCUDNN(cudnnSetTensor4dDescriptor(input_descriptor,
		/*format=*/CUDNN_TENSOR_NHWC,	// 注意是 NHWC，TensorFlow更喜欢以 NHWC 格式存储张量(通道是变化最频繁的地方，即 BGR)，而其他一些更喜欢将通道放在前面
		/*dataType=*/CUDNN_DATA_FLOAT,
		/*out_n=*/in_n,
		/*channels=*/in_c,
		/*image_height=*/in_h,
		/*image_width=*/in_w));
 
	// 卷积核的描述（形状、格式）
	cudnnFilterDescriptor_t kernel_descriptor;
	checkCUDNN(cudnnCreateFilterDescriptor(&kernel_descriptor));
	checkCUDNN(cudnnSetFilter4dDescriptor(kernel_descriptor,
		/*dataType=*/CUDNN_DATA_FLOAT,
		/*format=*/CUDNN_TENSOR_NCHW,	// 注意是 NCHW
		/*out_c=*/out_c,
		/*in_channels=*/in_c,
		/*kernel_height=*/ker_h,
		/*kernel_width=*/ker_w));
 
	// 卷积操作的描述（步长、填充等等）
	cudnnConvolutionDescriptor_t convolution_descriptor;
	checkCUDNN(cudnnCreateConvolutionDescriptor(&convolution_descriptor));
	checkCUDNN(cudnnSetConvolution2dDescriptor(convolution_descriptor,
		/*pad_height=*/1,
		/*pad_width=*/1,
		/*vertical_stride=*/1,
		/*horizontal_stride=*/1,
		/*dilation_height=*/1,
		/*dilation_width=*/1,
		/*mode=*/CUDNN_CROSS_CORRELATION, // CUDNN_CONVOLUTION
		/*computeType=*/CUDNN_DATA_FLOAT));
 
	// 计算卷积后图像的维数
	checkCUDNN(cudnnGetConvolution2dForwardOutputDim(convolution_descriptor,
		input_descriptor,
		kernel_descriptor,
		&out_n,
		&out_c,
		&out_h,
		&out_w));
 
	printf("Output Image[NHWC]: %d %d %d %d\n", out_n, out_h, out_w, out_c);
 
	// 卷积输出张量的描述
	cudnnTensorDescriptor_t output_descriptor;
	checkCUDNN(cudnnCreateTensorDescriptor(&output_descriptor));
	checkCUDNN(cudnnSetTensor4dDescriptor(output_descriptor,
		/*format=*/CUDNN_TENSOR_NHWC,
		/*dataType=*/CUDNN_DATA_FLOAT,
		/*out_n=*/1,
		/*channels=*/out_c,
		/*image_height=*/in_h,
		/*image_width=*/in_w));
 
	// 卷积算法的描述
	// cudnn_tion_fwd_algo_gemm——将卷积建模为显式矩阵乘法，
	// cudnn_tion_fwd_algo_fft——它使用快速傅立叶变换(FFT)进行卷积或
	// cudnn_tion_fwd_algo_winograd——它使用Winograd算法执行卷积。
	cudnnConvolutionFwdAlgo_t convolution_algorithm;
	checkCUDNN(
		cudnnGetConvolutionForwardAlgorithm(cudnn,
		input_descriptor,
		kernel_descriptor,
		convolution_descriptor,
		output_descriptor,
		CUDNN_CONVOLUTION_FWD_PREFER_FASTEST, // CUDNN_CONVOLUTION_FWD_SPECIFY_?WORKSPACE_LIMIT（在内存受限的情况下，memoryLimitInBytes 设置非 0 值）
		/*memoryLimitInBytes=*/0,
		&convolution_algorithm));
 
	// 计算 cuDNN 它的操作需要多少内存
	size_t workspace_bytes = 0;
	checkCUDNN(cudnnGetConvolutionForwardWorkspaceSize(cudnn,
		input_descriptor,
		kernel_descriptor,
		convolution_descriptor,
		output_descriptor,
		convolution_algorithm,
		&workspace_bytes));
	printf("Workspace size: %lf MB\n", (workspace_bytes / 1048576.0));
	//assert(workspace_bytes > 0);
 
	// *************************************************************************
	// 分配内存， 从 cudnnGetConvolutionForwardWorkspaceSize 计算而得
	void* d_workspace = NULL;
	cudaMalloc(&d_workspace, workspace_bytes);
 
	// 从 cudnnGetConvolution2dForwardOutputDim 计算而得
	int out_bytes = out_n * out_c * out_h * out_w * sizeof(float);
 
	float* gpu_input{ nullptr };
	cudaMalloc(&gpu_input, in_bytes);
	cudaMemcpy(gpu_input, cpu_input, in_bytes, cudaMemcpyHostToDevice);
 
	float* gpu_output = NULL;
	cudaMalloc(&gpu_output, out_bytes);
	cudaMemset(gpu_output, 0, out_bytes);
	// *************************************************************************
	// clang-format off
	const float kernel_template[3][3] = {
		{ 1, 1, 1 },
		{ 1, -8, 1 },
		{ 1, 1, 1 }
	};
	// clang-format on
 
	int ker_count = in_n*out_n*ker_h*ker_w;
	int ker_bytes = ker_count * sizeof(float);
	float* cpu_kernel = (float*)malloc(ker_bytes); // NCHW
	for (int kernel = 0; kernel < out_n; ++kernel) {
		for (int channel = 0; channel < in_n; ++channel) {
			for (int row = 0; row < ker_h; ++row) {
				for (int column = 0; column < ker_w; ++column) {
					cpu_kernel[(((kernel * in_n) + channel)*ker_h + row)*ker_w+column] = kernel_template[row][column];
				}
			}
		}
	}
 
	float* gpu_kernel = NULL;
	cudaMalloc(&gpu_kernel, ker_bytes);
	cudaMemcpy(gpu_kernel, cpu_kernel, ker_bytes, cudaMemcpyHostToDevice);
	
	const float alpha = 1.0f, beta = 0.0f;
 
	// 真正的卷积操作 ！！！前向卷积
	checkCUDNN(cudnnConvolutionForward(cudnn,
		&alpha,
		input_descriptor,
		gpu_input,
		kernel_descriptor,
		gpu_kernel,
		convolution_descriptor,
		convolution_algorithm,
		d_workspace, // 注意，如果我们选择不需要额外内存的卷积算法，d_workspace可以为nullptr。
		workspace_bytes,
		&beta,
		output_descriptor,
		gpu_output));
 
	if (with_sigmoid) {
		// 描述激活
		cudnnActivationDescriptor_t activation_descriptor;
		checkCUDNN(cudnnCreateActivationDescriptor(&activation_descriptor));
		checkCUDNN(cudnnSetActivationDescriptor(activation_descriptor,
			CUDNN_ACTIVATION_SIGMOID,
			CUDNN_PROPAGATE_NAN,
			/*relu_coef=*/0));
 
		// 前向 sigmoid 激活函数
		checkCUDNN(cudnnActivationForward(cudnn,
			activation_descriptor,
			&alpha,
			output_descriptor,
			gpu_output,
			&beta,
			output_descriptor,
			gpu_output));
		cudnnDestroyActivationDescriptor(activation_descriptor);
	}
 
	float* cpu_output = (float*)malloc(out_bytes);
	cudaMemcpy(cpu_output, gpu_output, out_bytes, cudaMemcpyDeviceToHost);

	free(cpu_input);
	free(cpu_output);
	free(cpu_kernel);
	//save_image("../cudnn-out.png", cpu_output, out_h, out_w);
 
	cudaFree(gpu_kernel);
	cudaFree(gpu_input);
	cudaFree(gpu_output);
	cudaFree(d_workspace);

	// 销毁
	cudnnDestroyTensorDescriptor(input_descriptor);
	cudnnDestroyTensorDescriptor(output_descriptor);
	cudnnDestroyFilterDescriptor(kernel_descriptor);
	cudnnDestroyConvolutionDescriptor(convolution_descriptor);
 
	cudnnDestroy(cudnn);
	return 0;
}

#pragma comment(lib,"cudart.lib")
#pragma comment(lib,"cublas.lib")
#pragma comment(lib,"curand.lib")
#pragma comment(lib,"cudnn.lib")
