
#include "cdnn.h"
#include "cdnn_cpu.hpp"




CDnn* GetCDnn(cdnnBrew_t brew) {
	switch (brew)
	{
	case CDNN_CPU:
		return new CDnnCpu();
		break;
	case CDNN_GPU:
		//return new GPUblas();
		break;
	default:
		break;
	}
	return 0;
}
