#ifndef CAFFE_UTIL_UTIL_IMG_H_
#define CAFFE_UTIL_UTIL_IMG_H_


#include "caffe/blob.hpp"




void ResizeBlob_cpu(const Blob* src, const int src_n, const int src_c,
		Blob* dst, const int dst_n, const int dst_c);


void ResizeBlob_cpu(const Blob* src,
		Blob* dst);


void ResizeBlob_cpu(const Blob* src,Blob* dst,
		Blob* loc1, Blob* loc2, Blob* loc3, Blob* loc4);

#ifndef CPU_ONLY

void ResizeBlob_gpu(const Blob* src,Blob* dst,
		Blob* loc1, Blob* loc2, Blob* loc3, Blob* loc4);


void ResizeBlob_gpu(const Blob* src,
		Blob* dst);


void ResizeBlob_gpu(const Blob* src, const int src_n, const int src_c,
		Blob* dst, const int dst_n, const int dst_c);
#endif


void ResizeBlob(const Blob* src,
		Blob* dst);


void BiLinearResizeMat_cpu(const Dtype* src, const int src_h, const int src_w,
		Dtype* dst, const int dst_h, const int dst_w);


void RuleBiLinearResizeMat_cpu(const Dtype* src,Dtype* dst, const int dst_h, const int dst_w,
		const Dtype* loc1, const Dtype* weight1, const Dtype* loc2,const Dtype* weight2,
		const	Dtype* loc3,const Dtype* weight3,const Dtype* loc4, const Dtype* weight4);



void GetBiLinearResizeMatRules_cpu(  const int src_h, const int src_w,
		  const int dst_h, const int dst_w,
		Dtype* loc1, Dtype* weight1, Dtype* loc2, Dtype* weight2,
		Dtype* loc3, Dtype* weight3, Dtype* loc4, Dtype* weight4);

#ifndef CPU_ONLY

void GetBiLinearResizeMatRules_gpu(  const int src_h, const int src_w,
		  const int dst_h, const int dst_w,
		Dtype* loc1, Dtype* weight1, Dtype* loc2, Dtype* weight2,
		Dtype* loc3, Dtype* weight3, Dtype* loc4, Dtype* weight4);


/**
 * be careful, this function is valid only when src and dst are GPU memory
 *
 */

void BiLinearResizeMat_gpu(const Dtype* src, const int src_h, const int src_w,
		Dtype* dst, const int dst_h, const int dst_w);
#endif

//
//
//void BlobAddPadding_cpu(const Blob* src,
//		Blob* dst, const int pad_h,const int pad_w);


void GenerateSubBlobs_cpu(const Blob& src,
		Blob& dst,const int kernel_h, const int kernel_w,
	    const int pad_h, const int pad_w, const int stride_h,
	    const int stride_w);

#ifndef CPU_ONLY

void GenerateSubBlobs_gpu(const Blob& src,
		Blob& dst,const int kernel_h, const int kernel_w,
	    const int pad_h, const int pad_w, const int stride_h,
	    const int stride_w);

#endif


void CropBlobs_cpu( const Blob&src, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst);

#ifndef CPU_ONLY

void CropBlobs_gpu( const Blob&src, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst);
#endif

/**
 * @brief  crop blob. The destination blob will be reshaped.
 */

void CropBlobs( const Blob&src, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst);




void CropBlobs_cpu( const Blob&src, const int src_num_id, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst,
		const int dst_num_id,const int dst_start_h = 0, const int dst_start_w = 0);

#ifndef CPU_ONLY

void CropBlobs_gpu( const Blob&src, const int src_num_id, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst,
		const int dst_num_id,const int dst_start_h = 0, const int dst_start_w = 0);
#endif
/**
 * @brief  crop blob. The destination blob will not be reshaped.
 */

void CropBlobs( const Blob&src, const int src_num_id, const int start_h,
		const int start_w, const int end_h, const int end_w, Blob&dst,
		const int dst_num_id,const int dst_start_h = 0, const int dst_start_w = 0);

/**
 * src(n,c,h,w)  ===>   dst(n_ori,c,new_h,new_w)
 * n contains sub images from (0,0),(0,1),....(nh,nw)
 */

void ConcateSubImagesInBlobs_cpu( const Blob& src,
		Blob& dst,const int kernel_h, const int kernel_w,
	    const int pad_h, const int pad_w, const int stride_h,
	    const int stride_w, const int out_img_h, const int out_img_w);

#ifndef CPU_ONLY

void ConcateSubImagesInBlobs_gpu( const Blob& src,
		Blob& dst,const int kernel_h, const int kernel_w,
	    const int pad_h, const int pad_w, const int stride_h,
	    const int stride_w, const int out_img_h, const int out_img_w);
#endif






#endif   // CAFFE_UTIL_UTIL_IMG_H_
