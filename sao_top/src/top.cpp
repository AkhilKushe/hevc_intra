#include "top.h"
#include "initializer.h"
#include "utils.h"
#include "arith_dec.h"
#include "deBin.h"
#include <string.h>

void load_ram(UChar ctxTable[MAX_NUM_CTX_MOD], hls::stream<UChar>& inpStream, UInt cnt){
	for(int i=0; i < cnt; i++) {
		ctxTable[i] = inpStream.read();
	}
}

void store_global_ram(volatile UChar globalCtx[MAX_NUM_CTX_MOD], UChar ctxTable[MAX_NUM_CTX_MOD], UInt cnt){
	for(int i=0; i < cnt; i++) {
		globalCtx[i] = ctxTable[i];
	}
}


void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile arith_t initArithState[1], volatile UChar bitStream[64], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]){
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 name=gCtx port=globalCtx
#pragma HLS INTERFACE mode=s_axilite port=globalCtx
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 name=bst port=bitStream
#pragma HLS INTERFACE mode=s_axilite port=bitStream
#pragma HLS INTERFACE mode=m_axi bundle=gmem1 name=din port=data_in_s
#pragma HLS INTERFACE mode=s_axilite port=data_in_s
#pragma HLS INTERFACE mode=m_axi bundle=gmem1 name=dout port=data_out_s
#pragma HLS INTERFACE mode=s_axilite port=data_out_s
#pragma HLS INTERFACE mode=m_axi bundle=gmem1 name=BaeState port=initArithState
#pragma HLS INTERFACE mode=s_axilite port=initArithState

	hls::stream<UChar> streamCtxRAM;
#pragma HLS STREAM depth=32 type=fifo variable=streamCtxRAM

	UChar ctxTables[MAX_NUM_CTX_MOD];
	data_in_t data_in;
	data_out_t data_out;
	UChar tempBst[64];
	arith_t baeState;

	memcpy(&data_in, (data_in_t*)data_in_s, sizeof(data_in_t));
	memcpy(tempBst, (UChar*)bitStream, 64*sizeof(UChar));


	initialization_top(data_in.firstCTU, data_in.s_header.slice_type, data_in.s_header.qp, data_in.s_header.cabac_init_flag, globalCtx, streamCtxRAM);
	load_ram(ctxTables, streamCtxRAM, MAX_NUM_CTX_MOD);
	memcpy(tempBst, (UChar*)bitStream, 64*sizeof(UChar));
	if(data_in.firstCTU){
		arith_init(tempBst, baeState);
	} else {
		memcpy(&baeState, (arith_t*)initArithState,sizeof(arith_t));
	}
	sao_top(data_in, data_out, baeState, tempBst, ctxTables);

	memcpy((data_out_t*)data_out_s, &data_out,sizeof(data_out_t));
	memcpy((arith_t*)initArithState, &baeState,sizeof(arith_t));

	store_global_ram(globalCtx, ctxTables, MAX_NUM_CTX_MOD);
}




