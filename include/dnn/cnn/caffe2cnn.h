
//#include "cstd.h"

#include <stdio.h>
#include <limits.h>
#include <math.h>
//_byteswap_ushort;
#include <fstream>
#include <set>
#include <limits>
#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>

#include "caffe.pb.h"
#include "caffe.pb.cc"

#pragma comment(lib,"libprotobufd.lib")

static inline size_t alignSize(size_t sz, int n)
{
    return (sz + n-1) & -n;
}

// convert float to half precision floating point
static unsigned short float2half(float value)
{
    // 1 : 8 : 23
    union
    {
        unsigned int u;
        float f;
    } tmp;

    tmp.f = value;

    // 1 : 8 : 23
    unsigned short sign = (tmp.u & 0x80000000) >> 31;
    unsigned short exponent = (tmp.u & 0x7F800000) >> 23;
    unsigned int significand = tmp.u & 0x7FFFFF;

//     fprintf(stderr, "%d %d %d\n", sign, exponent, significand);

    // 1 : 5 : 10
    unsigned short fp16;
    if (exponent == 0)
    {
        // zero or denormal, always underflow
        fp16 = (sign << 15) | (0x00 << 10) | 0x00;
    }
    else if (exponent == 0xFF)
    {
        // infinity or NaN
        fp16 = (sign << 15) | (0x1F << 10) | (significand ? 0x200 : 0x00);
    }
    else
    {
        // normalized
        short newexp = exponent + (- 127 + 15);
        if (newexp >= 31)
        {
            // overflow, return infinity
            fp16 = (sign << 15) | (0x1F << 10) | 0x00;
        }
        else if (newexp <= 0)
        {
            // underflow
            if (newexp >= -10)
            {
                // denormal half-precision
                unsigned short sig = (significand | 0x800000) >> (14 - newexp);
                fp16 = (sign << 15) | (0x00 << 10) | sig;
            }
            else
            {
                // underflow
                fp16 = (sign << 15) | (0x00 << 10) | 0x00;
            }
        }
        else
        {
            fp16 = (sign << 15) | (newexp << 10) | (significand >> 13);
        }
    }

    return fp16;
}

static int quantize_weight(float *data, size_t data_length, std::vector<unsigned short>& float16_weights)
{
    float16_weights.resize(data_length);

    for (size_t i = 0; i < data_length; i++)
    {
        float f = data[i];

        unsigned short fp16 = float2half(f);

        float16_weights[i] = fp16;
    }

    // magic tag for half-precision floating point
    return 0x01306B47;
}

static bool quantize_weight(float *data, size_t data_length, int quantize_level, std::vector<float> &quantize_table, std::vector<unsigned char> &quantize_index) {

    assert(quantize_level != 0);
    assert(data != NULL);
    assert(data_length > 0);

    if (data_length < static_cast<size_t>(quantize_level)) {
        fprintf(stderr, "No need quantize,because: data_length < quantize_level");
        return false;
    }

    quantize_table.reserve(quantize_level);
    quantize_index.reserve(data_length);

    // 1. Find min and max value
    float max_value = std::numeric_limits<float>::min();
    float min_value = std::numeric_limits<float>::max();

    for (size_t i = 0; i < data_length; ++i)
    {
        if (max_value < data[i]) max_value = data[i];
        if (min_value > data[i]) min_value = data[i];
    }
    float strides = (max_value - min_value) / quantize_level;

    // 2. Generate quantize table
    for (int i = 0; i < quantize_level; ++i)
    {
        quantize_table.push_back(min_value + i * strides);
    }

    // 3. Align data to the quantized value
    for (size_t i = 0; i < data_length; ++i)
    {
        size_t table_index = int((data[i] - min_value) / strides);
        table_index = std::min<float>(table_index, quantize_level - 1);

        float low_value  = quantize_table[table_index];
        float high_value = low_value + strides;

        // find a nearest value between low and high value.
        float targetValue = data[i] - low_value < high_value - data[i] ? low_value : high_value;

        table_index = int((targetValue - min_value) / strides);
        table_index = std::min<float>(table_index, quantize_level - 1);
        quantize_index.push_back(table_index);
    }

    return true;
}

static bool read_proto_from_text(const char* filepath, google::protobuf::Message* message)
{
    std::ifstream fs(filepath, std::ifstream::in);
    if (!fs.is_open())
    {
        fprintf(stderr, "open failed %s\n", filepath);
        return false;
    }

    google::protobuf::io::IstreamInputStream input(&fs);
    bool success = google::protobuf::TextFormat::Parse(&input, message);

    fs.close();

    return success;
}

static bool read_proto_from_binary(const char* filepath, google::protobuf::Message* message)
{
    std::ifstream fs(filepath, std::ifstream::in | std::ifstream::binary);
    if (!fs.is_open())
    {
        fprintf(stderr, "open failed %s\n", filepath);
        return false;
    }

    google::protobuf::io::IstreamInputStream input(&fs);
    google::protobuf::io::CodedInputStream codedstr(&input);

    codedstr.SetTotalBytesLimit(INT_MAX, INT_MAX / 2);

    bool success = message->ParseFromCodedStream(&codedstr);

    fs.close();

    return success;
}

int caffe2cnn(int argc, char** argv)
{
#if 0
    if (!(argc == 3 || argc == 5 || argc == 6))
    {
        fprintf(stderr, "Usage: %s [caffeproto] [caffemodel] [ncnnproto] [ncnnbin] [quantizelevel]\n", argv[0]);
        return -1;
    }

    const char* caffeproto = argv[1];
    const char* caffemodel = argv[2];
    const char* ncnn_prototxt = argc >= 5 ? argv[3] : "ncnn.proto";
    const char* ncnn_modelbin = argc >= 5 ? argv[4] : "ncnn.bin";
    const char* quantize_param = argc == 6 ? argv[5] : "0";
	int quantize_level = atoi(quantize_param);
#endif
#if 0
#define WORK_PATH   "E:/code/cstd/include1/dnn/ncnn/mtcnn2/"
#define NAME   WORK_PATH "det1"
	const char* caffeproto = NAME ".prototxt";
	const char* caffemodel = NAME ".caffemodel";
	const char* ncnn_prototxt = NAME ".txt";
	const char* ncnn_modelbin = NAME ".dat";
	const char* quantize_param = 0;
	int quantize_level = 0;
#endif
#if 1
#define WORK_PATH   "E:/code/cstd/include1/dnn/ncnn/squeezenet/"
#define NAME   WORK_PATH "squeezenet_v1.1"
	const char* caffeproto = NAME ".prototxt";
	const char* caffemodel = NAME ".caffemodel";
	const char* ncnn_prototxt = NAME ".txt";
	const char* ncnn_modelbin = NAME ".dat";
	const char* quantize_param = 0;
	int quantize_level = 0;
#endif

    if (quantize_level != 0 && quantize_level != 256 && quantize_level != 65536) {
        fprintf(stderr, "%s: only support quantize level = 0, 256, or 65536", argv[0]);
        return -1;
    }

    caffe::NetParameter proto;
    caffe::NetParameter net;

    // load
    bool s0 = read_proto_from_text(caffeproto, &proto);
    if (!s0)
    {
        fprintf(stderr, "read_proto_from_text failed\n");
        return -1;
    }

    bool s1 = read_proto_from_binary(caffemodel, &net);
    if (!s1)
    {
        fprintf(stderr, "read_proto_from_binary failed\n");
        return -1;
    }

    FILE* pp = fopen(ncnn_prototxt, "wb");

    // magic
    //fprintf(pp, "7767517\n");

    // rename mapping for identical bottom top style
    std::map<std::string, std::string> blob_name_decorated;

    // bottom blob reference
    std::map<std::string, int> bottom_reference;

    // global definition line
    // [layer count] [blob count]
    int layer_count = proto.layer_size();
    std::set<std::string> blob_names;
    for (int i=0; i<layer_count; i++)
    {
        const caffe::LayerParameter& layer = proto.layer(i);

        for (int j=0; j<layer.bottom_size(); j++)
        {
            std::string blob_name = layer.bottom(j);
            if (blob_name_decorated.find(blob_name) != blob_name_decorated.end())
            {
                blob_name = blob_name_decorated[blob_name];
            }

            blob_names.insert(blob_name);

            if (bottom_reference.find(blob_name) == bottom_reference.end())
            {
                bottom_reference[blob_name] = 1;
            }
            else
            {
                bottom_reference[blob_name] = bottom_reference[blob_name] + 1;
            }
        }

        if (layer.bottom_size() == 1 && layer.top_size() == 1 && layer.bottom(0) == layer.top(0))
        {
            std::string blob_name = layer.top(0) + "_" + layer.name();
            blob_name_decorated[layer.top(0)] = blob_name;
            blob_names.insert(blob_name);
        }
        else
        {
            for (int j=0; j<layer.top_size(); j++)
            {
                std::string blob_name = layer.top(j);
                blob_names.insert(blob_name);
            }
        }
    }

	bottom_reference.clear();
    // remove bottom_reference entry with reference equals to one
    int splitncnn_blob_count = 0;
    std::map<std::string, int>::iterator it = bottom_reference.begin();
    while (it != bottom_reference.end())
    {
        if (it->second == 1)
        {
            bottom_reference.erase(it++);
        }
        else
        {
            splitncnn_blob_count += it->second;
//             fprintf(stderr, "%s %d\n", it->first.c_str(), it->second);
            ++it;
        }
    }
	//fprintf(pp, "%lu %lu\n", layer_count + bottom_reference.size(), blob_names.size() + splitncnn_blob_count);
    fprintf(pp, "//%lu %lu\n", layer_count, blob_names.size());

    // populate
    blob_name_decorated.clear();
    int internal_split = 0;
	std::vector<caffe::BlobProto> vb;
	std::vector<std::string> vs;

    for (int i=0; i<layer_count; i++)
    {
        const caffe::LayerParameter& layer = proto.layer(i);

		fprintf(pp, "//");
        // layer definition line, repeated
        // [type] [name] [bottom blob count] [top blob count] [bottom blobs] [top blobs] [layer specific params]
        if (layer.type() == "Convolution")
        {
			fprintf(pp, "%-16s", "conv");
        }
        else if (layer.type() == "Python")
        {
            const caffe::PythonParameter& python_param = layer.python_param();
            std::string python_layer_name = python_param.layer();
            if (python_layer_name == "ProposalLayer")
                fprintf(pp, "%-16s", "Proposal");
            else
                fprintf(pp, "%-16s", python_layer_name.c_str());
        }
        else
        {
            fprintf(pp, "%-16s", layer.type().c_str());
        }
		// find blob binary by layer name
		int netidx = net.layer_size();
		for (netidx = 0; netidx<net.layer_size(); netidx++)
		{
			if (net.layer(netidx).name() == layer.name())
			{
				break;
			}
		}
		int w_size = 0;
		if (netidx<net.layer_size()) {
		  const caffe::LayerParameter& binlayer = net.layer(netidx);
		  w_size = binlayer.blobs_size();
		}
		//fprintf(pp, " %-16s", layer.name().c_str());
		fprintf(pp, " %d %d %d", layer.bottom_size(), layer.top_size(), w_size);

        for (int j=0; j<layer.bottom_size(); j++)
        {
            std::string blob_name = layer.bottom(j);
            if (blob_name_decorated.find(layer.bottom(j)) != blob_name_decorated.end())
            {
                blob_name = blob_name_decorated[layer.bottom(j)];
            }

            if (bottom_reference.find(blob_name) != bottom_reference.end())
            {
                int refidx = bottom_reference[blob_name] - 1;
                bottom_reference[blob_name] = refidx;

                char splitsuffix[256];
                sprintf(splitsuffix, "_splitncnn_%d", refidx);
                blob_name = blob_name + splitsuffix;
            }

            fprintf(pp, " %s", blob_name.c_str());
        }

        // decorated
        if (layer.bottom_size() == 1 && layer.top_size() == 1 && layer.bottom(0) == layer.top(0))
        {
            std::string blob_name = layer.top(0) + "_" + layer.name();
            blob_name_decorated[layer.top(0)] = blob_name;

            fprintf(pp, " %s", blob_name.c_str());
        }
        else
        {
            for (int j=0; j<layer.top_size(); j++)
            {
                std::string blob_name = layer.top(j);
                fprintf(pp, " %s", blob_name.c_str());
            }
        }

		if (w_size > 0) {
			const caffe::LayerParameter& binlayer = net.layer(netidx);
			for (int j = 0; j < w_size; j++)
			{
				const caffe::BlobProto& blob = binlayer.blobs(j);
				std::string blob_name = blob.GetTypeName();
				//fprintf(pp, " %s_w_%d", layer.type().c_str(), j);
				char buf[256];
				sprintf(buf, "%s_w_%d", layer.name().c_str(), j);
				fprintf(pp, " %s", buf);
				vs.push_back(buf);
				vb.push_back(blob);
			}
		}
		
        // layer specific params
        if (layer.type() == "BatchNorm")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);

            const caffe::BlobProto& mean_blob = binlayer.blobs(0);
            const caffe::BlobProto& var_blob = binlayer.blobs(1);
            fprintf(pp, " %d", (int)mean_blob.data_size());

            const caffe::BatchNormParameter& batch_norm_param = layer.batch_norm_param();
            float eps = batch_norm_param.eps();

            //std::vector<float> ones(mean_blob.data_size(), 1.f);
            //fwrite(ones.data(), sizeof(float), ones.size(), bp);// slope
        }
        else if (layer.type() == "Concat")
        {
            const caffe::ConcatParameter& concat_param = layer.concat_param();
            int dim = concat_param.axis() - 1;
            fprintf(pp, " axis=%d", dim);
        }
        else if (layer.type() == "Convolution")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);

            const caffe::ConvolutionParameter& convolution_param = layer.convolution_param();
            fprintf(pp, " num_output=%d", convolution_param.num_output());
            fprintf(pp, " kernel_size=%d", convolution_param.kernel_size(0));
			if (convolution_param.dilation_size()>0 && convolution_param.dilation(0)>1)
				fprintf(pp, " dilation=%d", convolution_param.dilation_size() != 0 ? convolution_param.dilation(0) : 1);
			if (convolution_param.stride_size()>0 && convolution_param.stride(0)>1)
				fprintf(pp, " stride=%d", convolution_param.stride_size() != 0 ? convolution_param.stride(0) : 1);
			if (convolution_param.pad_size()>0 && convolution_param.pad(0)>0)
				fprintf(pp, " pad=%d", convolution_param.pad_size() != 0 ? convolution_param.pad(0) : 0);
			if (convolution_param.bias_term())
				fprintf(pp, " bias_term=%d", convolution_param.bias_term());

            if (convolution_param.group() != 1)
            {
                fprintf(pp, " group=%d", convolution_param.group());
            }

        }
        else if (layer.type() == "Crop")
        {
            const caffe::CropParameter& crop_param = layer.crop_param();
            int num_offset = crop_param.offset_size();
            int woffset = (num_offset == 2) ? crop_param.offset(0) : 0;
            int hoffset = (num_offset == 2) ? crop_param.offset(1) : 0;
            fprintf(pp, " %d", woffset);
            fprintf(pp, " %d", hoffset);
        }
        else if (layer.type() == "Deconvolution")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);

            const caffe::BlobProto& weight_blob = binlayer.blobs(0);
            const caffe::ConvolutionParameter& convolution_param = layer.convolution_param();
            fprintf(pp, " %d", convolution_param.num_output());
            fprintf(pp, " %d", convolution_param.kernel_size(0));
            fprintf(pp, " %d", convolution_param.dilation_size() != 0 ? convolution_param.dilation(0) : 1);
            fprintf(pp, " %d", convolution_param.stride_size() != 0 ? convolution_param.stride(0) : 1);
            fprintf(pp, " %d", convolution_param.pad_size() != 0 ? convolution_param.pad(0) : 0);
            fprintf(pp, " %d", convolution_param.bias_term());
			fprintf(pp, " %d", weight_blob.data_size());
			fprintf(pp, " %d", 0);
        }
        else if (layer.type() == "DetectionOutput")
        {
            const caffe::DetectionOutputParameter& detection_output_param = layer.detection_output_param();
            const caffe::NonMaximumSuppressionParameter& nms_param = detection_output_param.nms_param();
            fprintf(pp, " num_classes=%d", detection_output_param.num_classes());
            fprintf(pp, " top_k=%d", nms_param.top_k());
            fprintf(pp, " keep_top_k=%d", detection_output_param.keep_top_k());
			float nms_threshold = nms_param.nms_threshold();
			float confidence_threshold = detection_output_param.confidence_threshold();
			fprintf(pp, " nms_threshold=%g", nms_threshold);
			fprintf(pp, " confidence_threshold=%g", confidence_threshold);
        }
        else if (layer.type() == "Dropout")
        {
            const caffe::DropoutParameter& dropout_param = layer.dropout_param();
            if (!dropout_param.scale_train())
            {
            }
			float scale = 1.f - dropout_param.dropout_ratio();
			//fprintf(pp, " %g", scale);
			fprintf(pp, " scale=%g", scale);
        }
        else if (layer.type() == "Eltwise")
        {
            const caffe::EltwiseParameter& eltwise_param = layer.eltwise_param();
            int coeff_size = eltwise_param.coeff_size();
            fprintf(pp, " %d", (int)eltwise_param.operation());
            fprintf(pp, " %d", coeff_size);
            for (int j=0; j<coeff_size; j++)
            {
                fprintf(pp, ",%g", eltwise_param.coeff(j));
            }
        }
        else if (layer.type() == "ELU")
        {
            const caffe::ELUParameter& elu_param = layer.elu_param();
            fprintf(pp, " %g", elu_param.alpha());
        }
        else if (layer.type() == "InnerProduct")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);

            const caffe::InnerProductParameter& inner_product_param = layer.inner_product_param();
            fprintf(pp, " num_output=%d", inner_product_param.num_output());
            fprintf(pp, " bias_term=%d", inner_product_param.bias_term());
        }
        else if (layer.type() == "Input")
        {
            const caffe::InputParameter& input_param = layer.input_param();
            const caffe::BlobShape& bs = input_param.shape(0);
			const char* dims_chs = "nchw";
            for (int j=0; j<std::min((int)bs.dim_size(), 4); j++)
            {
				if (bs.dim(j)>1)
					fprintf(pp, " input_%c=%ld", dims_chs[j], bs.dim(j));
            }
        }
        else if (layer.type() == "Interp")
        {
            const caffe::InterpParameter& interp_param = layer.interp_param();
            fprintf(pp, " %d", 2);
            fprintf(pp, " %g", (float)interp_param.zoom_factor());
            fprintf(pp, " %g", (float)interp_param.zoom_factor());
            fprintf(pp, " %d", interp_param.height());
            fprintf(pp, " %d", interp_param.width());
        }
        else if (layer.type() == "LRN")
        {
            const caffe::LRNParameter& lrn_param = layer.lrn_param();
            fprintf(pp, " %d", lrn_param.norm_region());
            fprintf(pp, " %d", lrn_param.local_size());
            fprintf(pp, " %g", lrn_param.alpha());
            fprintf(pp, " %g", lrn_param.beta());
        }
        else if (layer.type() == "MemoryData")
        {
            const caffe::MemoryDataParameter& memory_data_param = layer.memory_data_param();
            fprintf(pp, " %d", memory_data_param.width());
            fprintf(pp, " %d", memory_data_param.height());
            fprintf(pp, " %d", memory_data_param.channels());
        }
        else if (layer.type() == "Normalize")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);
            const caffe::BlobProto& scale_blob = binlayer.blobs(0);
            const caffe::NormalizeParameter& norm_param = layer.norm_param();
            fprintf(pp, " %d", norm_param.across_spatial());
            fprintf(pp, " %d", norm_param.channel_shared());
            fprintf(pp, " %g", norm_param.eps());
            fprintf(pp, " %d", scale_blob.data_size());
        }
        else if (layer.type() == "Permute")
        {
            const caffe::PermuteParameter& permute_param = layer.permute_param();
            int order_size = permute_param.order_size();
            int order_type = 0;
            if (order_size == 0)
                order_type = 0;
            if (order_size == 1)
            {
                int order0 = permute_param.order(0);
                if (order0 == 0)
                    order_type = 0;
                // permute with N not supported
            }
            if (order_size == 2)
            {
                int order0 = permute_param.order(0);
                int order1 = permute_param.order(1);
                if (order0 == 0)
                {
                    if (order1 == 1) // 0 1 2 3
                        order_type = 0;
                    else if (order1 == 2) // 0 2 1 3
                        order_type = 2;
                    else if (order1 == 3) // 0 3 1 2
                        order_type = 4;
                }
                // permute with N not supported
            }
            if (order_size == 3 || order_size == 4)
            {
                int order0 = permute_param.order(0);
                int order1 = permute_param.order(1);
                int order2 = permute_param.order(2);
                if (order0 == 0)
                {
                    if (order1 == 1)
                    {
                        if (order2 == 2) // 0 1 2 3
                            order_type = 0;
                        if (order2 == 3) // 0 1 3 2
                            order_type = 1;
                    }
                    else if (order1 == 2)
                    {
                        if (order2 == 1) // 0 2 1 3
                            order_type = 2;
                        if (order2 == 3) // 0 2 3 1
                            order_type = 3;
                    }
                    else if (order1 == 3)
                    {
                        if (order2 == 1) // 0 3 1 2
                            order_type = 4;
                        if (order2 == 2) // 0 3 2 1
                            order_type = 5;
                    }
                }
                // permute with N not supported
            }
            fprintf(pp, " %d", order_type);
        }
        else if (layer.type() == "Pooling")
        {
			char* poolname[] = { "MAX","AVE", "STOCHASTIC","DEF", "DEF_ALL", "DEF_ALL2", "DEF_ALL3", "DEF_ALL4", "LOWRES" };
            const caffe::PoolingParameter& pooling_param = layer.pooling_param();
            fprintf(pp, " pool=%s", poolname[pooling_param.pool()&7]);
            fprintf(pp, " kernel_size=%d", pooling_param.kernel_size());
			if (pooling_param.stride()>1)
				fprintf(pp, " stride=%d", pooling_param.stride());
			if (pooling_param.pad()>0)
				fprintf(pp, " pad=%d", pooling_param.pad());
			if (pooling_param.has_global_pooling() && pooling_param.global_pooling())
				fprintf(pp, " global=%d", pooling_param.has_global_pooling() ? pooling_param.global_pooling() : 0);
        }
        else if (layer.type() == "Power")
        {
            const caffe::PowerParameter& power_param = layer.power_param();
            fprintf(pp, " %g", power_param.power());
            fprintf(pp, " %g", power_param.scale());
            fprintf(pp, " %g", power_param.shift());
        }
        else if (layer.type() == "PReLU")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);
            const caffe::BlobProto& slope_blob = binlayer.blobs(0);
            //fwrite(slope_blob.data().data(), sizeof(float), slope_blob.data_size(), bp);
        }
        else if (layer.type() == "PriorBox")
        {
            const caffe::PriorBoxParameter& prior_box_param = layer.prior_box_param();

            int num_aspect_ratio = prior_box_param.aspect_ratio_size();
            for (int j=0; j<prior_box_param.aspect_ratio_size(); j++)
            {
                float ar = prior_box_param.aspect_ratio(j);
                if (fabs(ar - 1.) < 1e-6) {
                    num_aspect_ratio--;
                }
            }

            float variances[4] = {0.1f, 0.1f, 0.1f, 0.1f};
            if (prior_box_param.variance_size() == 4)
            {
                variances[0] = prior_box_param.variance(0);
                variances[1] = prior_box_param.variance(1);
                variances[2] = prior_box_param.variance(2);
                variances[3] = prior_box_param.variance(3);
            }
            else if (prior_box_param.variance_size() == 1)
            {
                variances[0] = prior_box_param.variance(0);
                variances[1] = prior_box_param.variance(0);
                variances[2] = prior_box_param.variance(0);
                variances[3] = prior_box_param.variance(0);
            }

            int flip = prior_box_param.has_flip() ? prior_box_param.flip() : 1;
            int clip = prior_box_param.has_clip() ? prior_box_param.clip() : 0;
            int image_width = -233;
            int image_height = -233;
            if (prior_box_param.has_img_size())
            {
                image_width = prior_box_param.img_size();
                image_height = prior_box_param.img_size();
            }
            else if (prior_box_param.has_img_w() && prior_box_param.has_img_h())
            {
                image_width = prior_box_param.img_w();
                image_height = prior_box_param.img_h();
            }

            float step_width = -233;
            float step_height = -233;
            if (prior_box_param.has_step())
            {
                step_width = prior_box_param.step();
                step_height = prior_box_param.step();
            }
            else if (prior_box_param.has_step_w() && prior_box_param.has_step_h())
            {
                step_width = prior_box_param.step_w();
                step_height = prior_box_param.step_h();
            }

            fprintf(pp, " -23300=%d", prior_box_param.min_size_size());
            for (int j=0; j<prior_box_param.min_size_size(); j++)
            {
                fprintf(pp, ",%g", prior_box_param.min_size(j));
            }
            fprintf(pp, " -23301=%d", prior_box_param.max_size_size());
            for (int j=0; j<prior_box_param.max_size_size(); j++)
            {
                fprintf(pp, ",%g", prior_box_param.max_size(j));
            }
            fprintf(pp, " -23302=%d", num_aspect_ratio);
            for (int j=0; j<prior_box_param.aspect_ratio_size(); j++)
            {
                float ar = prior_box_param.aspect_ratio(j);
                if (fabs(ar - 1.) < 1e-6) {
                    continue;
                }
                fprintf(pp, ",%g", ar);
            }
            fprintf(pp, " 3=%g", variances[0]);
            fprintf(pp, " 4=%g", variances[1]);
            fprintf(pp, " 5=%g", variances[2]);
            fprintf(pp, " 6=%g", variances[3]);
            fprintf(pp, " 7=%d", flip);
            fprintf(pp, " 8=%d", clip);
            fprintf(pp, " 9=%d", image_width);
            fprintf(pp, " 10=%d", image_height);
            fprintf(pp, " 11=%g", step_width);
            fprintf(pp, " 12=%g", step_height);
            fprintf(pp, " 13=%g", prior_box_param.offset());
        }
        else if (layer.type() == "Python")
        {
            const caffe::PythonParameter& python_param = layer.python_param();
            std::string python_layer_name = python_param.layer();
            if (python_layer_name == "ProposalLayer")
            {
                int feat_stride = 16;
                sscanf(python_param.param_str().c_str(), "'feat_stride': %d", &feat_stride);

                int base_size = 16;
//                 float ratio;
//                 float scale;
                int pre_nms_topN = 6000;
                int after_nms_topN = 300;
                float nms_thresh = 0.7;
                int min_size = 16;
                fprintf(pp, " %d", feat_stride);
                fprintf(pp, " %d", base_size);
                fprintf(pp, " %d", pre_nms_topN);
                fprintf(pp, " %d", after_nms_topN);
                fprintf(pp, " %g", nms_thresh);
                fprintf(pp, " %d", min_size);
            }
        }
        else if (layer.type() == "ReLU")
        {
            const caffe::ReLUParameter& relu_param = layer.relu_param();
            //fprintf(pp, " %g", relu_param.negative_slope());
			float negative_slope = relu_param.negative_slope();
			fprintf(pp, " slope=%g", negative_slope);
			//fwrite(&negative_slope, sizeof(float), 1, bp);
        }
        else if (layer.type() == "Reshape")
        {
            const caffe::ReshapeParameter& reshape_param = layer.reshape_param();
            const caffe::BlobShape& bs = reshape_param.shape();
            if (bs.dim_size() == 1)
            {
                fprintf(pp, " %ld -233 -233", bs.dim(0));
            }
            else if (bs.dim_size() == 2)
            {
                fprintf(pp, " %ld %ld -233", bs.dim(1), bs.dim(0));
            }
            else if (bs.dim_size() == 3)
            {
                fprintf(pp, " %ld %ld %ld", bs.dim(2), bs.dim(1), bs.dim(0));
            }
            else // bs.dim_size() == 4
            {
                fprintf(pp, " %ld %ld %ld", bs.dim(3), bs.dim(2), bs.dim(1));
            }
            fprintf(pp, " 0");// permute
        }
        else if (layer.type() == "ROIPooling")
        {
            const caffe::ROIPoolingParameter& roi_pooling_param = layer.roi_pooling_param();
            fprintf(pp, " %d", roi_pooling_param.pooled_w());
            fprintf(pp, " %d", roi_pooling_param.pooled_h());
            fprintf(pp, " %g", roi_pooling_param.spatial_scale());
        }
        else if (layer.type() == "Scale")
        {
            const caffe::LayerParameter& binlayer = net.layer(netidx);

            const caffe::BlobProto& weight_blob = binlayer.blobs(0);
            const caffe::ScaleParameter& scale_param = layer.scale_param();
            fprintf(pp, " %d", (int)weight_blob.data_size());
            fprintf(pp, " %d", scale_param.bias_term());
        }
        else if (layer.type() == "Slice")
        {
            const caffe::SliceParameter& slice_param = layer.slice_param();
            if (slice_param.has_slice_dim())
            {
                int num_slice = layer.top_size();
                fprintf(pp, " -23300=%d", num_slice);
                for (int j=0; j<num_slice; j++)
                {
                    fprintf(pp, ",-233");
                }
            }
            else
            {
                int num_slice = slice_param.slice_point_size() + 1;
                fprintf(pp, " -23300=%d", num_slice);
                int prev_offset = 0;
                for (int j=0; j<slice_param.slice_point_size(); j++)
                {
                    int offset = slice_param.slice_point(j);
                    fprintf(pp, ",%d", offset - prev_offset);
                    prev_offset = offset;
                }
                fprintf(pp, ",-233");
            }
        }
        else if (layer.type() == "Softmax")
        {
            const caffe::SoftmaxParameter& softmax_param = layer.softmax_param();
            int dim = softmax_param.axis() - 1;
            fprintf(pp, " axis=%d", dim);
        }
        else if (layer.type() == "Threshold")
        {
            const caffe::ThresholdParameter& threshold_param = layer.threshold_param();
            fprintf(pp, " %g", threshold_param.threshold());
        }

        fprintf(pp, "\n");

        // add split layer if top reference larger than one
        if (layer.bottom_size() == 1 && layer.top_size() == 1 && layer.bottom(0) == layer.top(0))
        {
            std::string blob_name = blob_name_decorated[layer.top(0)];
            if (bottom_reference.find(blob_name) != bottom_reference.end())
            {
                int refcount = bottom_reference[blob_name];
                if (refcount > 1)
                {
                    char splitname[256];
                    sprintf(splitname, "splitncnn_%d", internal_split);
                    fprintf(pp, "%-16s %-16s %d %d", "Split", splitname, 1, refcount);
                    fprintf(pp, " %s", blob_name.c_str());

                    for (int j=0; j<refcount; j++)
                    {
                        fprintf(pp, " %s_splitncnn_%d", blob_name.c_str(), j);
                    }
                    fprintf(pp, "\n");

                    internal_split++;
                }
            }
        }
        else
        {
            for (int j=0; j<layer.top_size(); j++)
            {
                std::string blob_name = layer.top(j);
                if (bottom_reference.find(blob_name) != bottom_reference.end())
                {
                    int refcount = bottom_reference[blob_name];
                    if (refcount > 1)
                    {
                        char splitname[256];
                        sprintf(splitname, "splitncnn_%d", internal_split);
                        fprintf(pp, "%-16s %-16s %d %d", "Split", splitname, 1, refcount);
                        fprintf(pp, " %s", blob_name.c_str());

                        for (int j=0; j<refcount; j++)
                        {
                            fprintf(pp, " %s_splitncnn_%d", blob_name.c_str(), j);
                        }
                        fprintf(pp, "\n");

                        internal_split++;
                    }
                }
            }
        }

    }

	for (int i = 0; i < vb.size(); i++) {
		caffe::BlobProto& blob = vb[i];
		caffe::BlobShape shape = blob.shape();
		int dim[] = {1,1,1,1};
		int n = shape.dim_size();
		for (int j = 0; j < n; ++j) {
			dim[j] = shape.dim(j);
		}
		//fprintf(pp, "//%16s %3d\n", vs[i].c_str(), n);
		fprintf(pp, "//%-20s %3d %3d %3d %3d\n", vs[i].c_str(), dim[0], dim[1], dim[2], dim[3]);
	}
	FILE* bp = fopen(ncnn_modelbin, "wb");

	for (int i = 0; i < vb.size(); i++) {
		caffe::BlobProto& blob = vb[i];
		for (int j = 0; j < blob.data_size(); j++)
		{
			float tmp = blob.data().data()[j];
			fwrite(&tmp, sizeof(float), 1, bp);
			fprintf(pp, "%g,\n", tmp);
		}
	}


    fclose(pp);
    fclose(bp);

    return 0;
}
