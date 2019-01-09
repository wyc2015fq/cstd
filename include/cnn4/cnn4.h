
#ifndef __CNN4_H__
#define __CNN4_H__

struct CnnNet;
void cnnnet_set_debug_info(int level);
CnnNet* cnnnet_new();
void cnnnet_del(CnnNet* net);
int cnnnet_loadjson(CnnNet* net, const char* fn);
int cnnnet_savejson(CnnNet* net, const char* fn);
int cnnnet_train(CnnNet* net);
double cnnnet_forward(CnnNet* net);
const float* cnnnet_output(CnnNet* net, int idx, int* pcount);
int cnnnet_set_input_u8(CnnNet* net, const unsigned char* img_data, int h, int w, int c, double scale, const double* mean_values, int mean_values_size);

#endif // __CNN4_H__
