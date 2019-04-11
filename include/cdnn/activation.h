

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif


cdnnStatus_t FUN(ActivationForward)(cdnnHandle_t handle, cdnnActivationDescriptorCpu* activationDesc, const Dtype*alpha, const cdnnTensorDescriptorCpu* xDesc, const Dtype*x, const Dtype* beta, const cdnnTensorDescriptorCpu* yDesc, Dtype* y) {
	int ndim = dim_ndim(xDesc->size.dim);
	int count = dim_count(xDesc->size.dim, 0, ndim);
	int n0 = count&-3;
	int i = 0;
	Dtype a = *alpha;
	Dtype b = *beta;
	Dtype z = activationDesc->coef;
	if (dim_cmp(xDesc->size.dim, yDesc->size.dim, 0, ndim) != 0) {
		return CDNN_STATUS_BAD_PARAM;
	}
	switch (activationDesc->mode) {
	case CDNN_ACTIVATION_SIGMOID:
		// 1. / (1. + exp(-x))
		// f(x)=(1 + \exp(-x))^{-1}.
		for (; i < n0; i += 4) {
			y[i + 0] = (a / (1. + exp(-x[i + 0]))) + b*y[i + 0];
			y[i + 1] = (a / (1. + exp(-x[i + 1]))) + b*y[i + 1];
			y[i + 2] = (a / (1. + exp(-x[i + 2]))) + b*y[i + 2];
			y[i + 3] = (a / (1. + exp(-x[i + 3]))) + b*y[i + 3];
		}
		for (; i < count; ++i) {
			y[i] = (a / (1. + exp(-x[i]))) + b*y[i];
		}
		break;
	case CDNN_ACTIVATION_RELU:
		// f(x)=\max(0, x).
		for (; i < n0; i += 4) {
			y[i + 0] = (a * (x[i + 0]>0 ? x[i + 0] : 0)) + b*y[i + 0];
			y[i + 1] = (a * (x[i + 1]>0 ? x[i + 1] : 0)) + b*y[i + 1];
			y[i + 2] = (a * (x[i + 2]>0 ? x[i + 2] : 0)) + b*y[i + 2];
			y[i + 3] = (a * (x[i + 3]>0 ? x[i + 3] : 0)) + b*y[i + 3];
		}
		for (; i < count; ++i) {
			y[i] = (a * (x[i]>0 ? x[i] : 0)) + b*y[i];
		}
		break;
	case CDNN_ACTIVATION_TANH:
		//f(x) = \tanh(x).
		for (; i < n0; i += 4) {
			y[i + 0] = (a * tanh(x[i + 0])) + b*y[i + 0];
			y[i + 1] = (a * tanh(x[i + 1])) + b*y[i + 1];
			y[i + 2] = (a * tanh(x[i + 2])) + b*y[i + 2];
			y[i + 3] = (a * tanh(x[i + 3])) + b*y[i + 3];
		}
		for (; i < count; ++i) {
			y[i] = (a * tanh(x[i])) + b*y[i];
		}
		break;
	case CDNN_ACTIVATION_CLIPPED_RELU:
		// \text{ClippedReLU}(x, z) = \min(\max(0, x), z).
		for (; i < count; ++i) {
			Dtype t0 = x[i] > 0 ? x[i] : 0;
			y[i] = (a * (t0 < z ? t0 : z)) + b*y[i];
		}
		break;
	case CDNN_ACTIVATION_ELU:
		/*\begin{split}f(x) = \left \{ \begin{array}{ll}
x & {\rm if}~ x \ge 0 \\
\alpha (\exp(x) - 1) & {\rm if}~ x < 0,
\end{array} \right.\end{split}
		*/
		// x>0  x
		// x<0 a*(exp(x)-1)

		for (; i < count; ++i) {
			y[i] = a*(MAX(x[i], Dtype(0)) + z * (exp(MIN(x[i], Dtype(0))) - Dtype(1))) + b*y[i];
		}
		break;
	case CDNN_ACTIVATION_IDENTITY:
		break;
	default:
		return CDNN_STATUS_NOT_SUPPORTED;
		break;
	}
	return CDNN_STATUS_SUCCESS;
}

cdnnStatus_t FUN(ActivationBackward)(cdnnHandle_t handle, cdnnActivationDescriptorCpu* activationDesc, const Dtype *alpha, const cdnnTensorDescriptorCpu* yDesc, const Dtype *y, const cdnnTensorDescriptorCpu* dyDesc, const Dtype *dy, const cdnnTensorDescriptorCpu* xDesc, const Dtype *x, const Dtype *beta, const cdnnTensorDescriptorCpu* dxDesc, Dtype *dx) {
	int ndim = dim_ndim(xDesc->size.dim);
	int count = dim_count(xDesc->size.dim, 0, ndim);
	for (int i = 0; i < count; ++i) {
		const Dtype sigmoid_x = x[i];
		dx[i] = dy[i] * sigmoid_x * (1. - sigmoid_x);
	}
	return CDNN_STATUS_SUCCESS;
}