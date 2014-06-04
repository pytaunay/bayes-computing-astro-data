#ifndef _EXP_TERM_H_
#define _EXP_TERM_H_

#include <cuda.h>
#include <cublas_v2.h>

void log_likelihood(double *X, double *isigma, double *mu, double det_sig, const int nobs,const int sizex, double *res, double *LV, double *tmp, double *ones, cublasHandle_t *handle); 

#endif
