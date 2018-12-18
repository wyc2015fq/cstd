
#ifndef __CNN4_H__
#define __CNN4_H__

struct Net;
void set_debug_info(int level);
Net* net_new();
void net_del(Net* net);
int net_loadjson(Net* net, const char* fn);
int net_train(Net* net);
double net_forward(Net* net);
const float* net_output(Net* net, int idx, int* pcount);
int net_set_input_u8(Net* net, const unsigned char* img_data, int w, const double* mean_values);

#endif // __CNN4_H__
