#ifndef __TOP_H__
#define __TOP_H__
#include "typedef.h"
#include <hls_stream.h>


void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile arith_t initArithState[1], volatile UChar bitStream[64], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]);

#endif
