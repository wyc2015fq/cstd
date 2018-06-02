


#ifndef _STRARR_H_
#define _STRARR_H_

#ifdef __cplusplus
extern "C" {
#endif

char ** StrArrFromRect(long *dim, void *data);
void StrArrToRect(char **strarr, long **dimenp, void **datap);
int StrArrLen(char **str_arr);
int StrArrMaxLen(char **str_arr);

#ifdef __cplusplus
}
#endif

#endif  /* _STRARR_H_ */
