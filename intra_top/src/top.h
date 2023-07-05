#ifndef __TOP_H__
#define __TOP_H__
#include "typedef.h"
#include <hls_stream.h>


//void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile int8_t tranCoeff[64][64], volatile UChar bitStream[1024], hls::stream<data_in_t>& data_in_s, hls::stream<data_out_t>& data_out_s);
//void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile int8_t tranCoeff[64][64], volatile UChar bitStream[1024], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]);
void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD],volatile arith_t initArithState[1], volatile int8_t tranCoeff_0[64][64], volatile int8_t tranCoeff_1[64][64], volatile int8_t tranCoeff_2[64][64], volatile char cqtDepth[64][64], volatile UChar IntraPredModeY[64][64], volatile UChar IntraPredModeC[64][64], volatile UChar bitStream[1024], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]);

#endif
