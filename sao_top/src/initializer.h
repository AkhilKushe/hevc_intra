#ifndef __INITIALIZER_H__
#define __INITIALIZER_H__
#include "typedef.h"
#include "hls_stream.h"

class Initialize {
public:
	Initialize(){};
	~Initialize(){};

	UChar calc_ctxState(int qp, UChar initVal);
	void load_global(volatile UChar globalCtx[MAX_NUM_CTX_MOD], hls::stream<UChar> &ctxOut);
	void stream_init_buffer(UChar* ctxTable, int qp, SliceType initType, hls::stream<UChar> &ctxOut);
	void load_local(SliceType sliceType, int qp, bool cabac_init_flag, hls::stream<UChar> &ctxOut);

};

void initialization_top(bool firstSlice, SliceType sliceType, int qp, bool cabac_init_flag, volatile UChar * globalCtx, hls::stream<UChar> &ctxOut);

#endif
