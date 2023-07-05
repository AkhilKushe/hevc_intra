#include "top.h"
#include "initializer.h"
#include "utils.h"
#include "arith_dec.h"
#include "deBin.h"
#include "intra_se.h"
#include "quad_tree.h"
#include "transform.h"
#include <string.h>

void load_ram(UChar ctxTable[MAX_NUM_CTX_MOD], hls::stream<UChar>& inpStream){
	for(int i=0; i < MAX_NUM_CTX_MOD; i++) {
		ctxTable[i] = inpStream.read();
	}
}

void store_global_ram(volatile UChar globalCtx[MAX_NUM_CTX_MOD], UChar ctxTable[MAX_NUM_CTX_MOD]){
	for(int i=0; i < MAX_NUM_CTX_MOD; i++) {
		globalCtx[i] = ctxTable[i];
	}
}



// TODO : fix top inteface with input/output declaration
//void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile int8_t tranCoeff[64][64], volatile UChar bitStream[1024], hls::stream<data_in_t>& data_in_s, hls::stream<data_out_t>& data_out_s){
void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD],volatile arith_t initArithState[1], volatile int8_t tranCoeff_0[64][64], volatile int8_t tranCoeff_1[64][64], volatile int8_t tranCoeff_2[64][64], volatile char cqtDepth[64][64], volatile UChar IntraPredModeY[64][64], volatile UChar IntraPredModeC[64][64], volatile UChar bitStream[1024], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]){
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 name=cqtD port=cqtDepth
#pragma HLS INTERFACE mode=s_axilite port=cqtDepth
#pragma HLS INTERFACE mode=m_axi bundle=gmem0 name=bst port=bitStream
#pragma HLS INTERFACE mode=s_axilite port=bitStream
#pragma HLS INTERFACE mode=m_axi bundle=gmem1 name=gCtx port=globalCtx
#pragma HLS INTERFACE mode=s_axilite port=globalCtx

#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=tranC port=tranCoeff_0
#pragma HLS INTERFACE mode=s_axilite port=tranCoeff_0
#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=tranC port=tranCoeff_1
#pragma HLS INTERFACE mode=s_axilite port=tranCoeff_1
#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=tranC port=tranCoeff_2
#pragma HLS INTERFACE mode=s_axilite port=tranCoeff_2
#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=State port=initArithState
#pragma HLS INTERFACE mode=s_axilite port=initArithState

#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=predDir port=IntraPredModeY
#pragma HLS INTERFACE mode=s_axilite port=IntraPredModeY
#pragma HLS INTERFACE mode=m_axi bundle=gmem2 name=predDir port=IntraPredModeC
#pragma HLS INTERFACE mode=s_axilite port=IntraPredModeC



#pragma HLS INTERFACE mode=m_axi bundle=gmem3 name=inpt port=data_in_s
#pragma HLS INTERFACE mode=s_axilite port=data_in_s

#pragma HLS INTERFACE mode=m_axi bundle=gmem3 name=outp port=data_out_s
#pragma HLS INTERFACE mode=s_axilite port=data_out_s

	hls::stream<UChar> streamCtxRAM;
#pragma HLS STREAM depth=256 type=fifo variable=streamCtxRAM

	UChar ctxTables[MAX_NUM_CTX_MOD];
	data_in_t data_in;
	data_out_t data_out;
	UChar tempBst[1024];
	arith_t baeState;
	internal_data_t dint;
	UInt symbolVal;

	memcpy(&data_in, (data_in_t*)data_in_s, sizeof(data_in_t));

	initialization_top(data_in.firstCTU, data_in.s_header.slice_type, data_in.s_header.qp, data_in.s_header.cabac_init_flag, globalCtx, streamCtxRAM);
	load_ram(ctxTables, streamCtxRAM);

	memcpy(tempBst, (UChar*)bitStream, 1024*sizeof(UChar));

	// Initialize Arith decoder
	// Export BAE state, Arith initialization only for first CTU, currently only 1 CTU

	if(data_in.firstCTU){
		arith_init(tempBst, baeState);
	} else {
		memcpy(&baeState, (arith_t*)initArithState,sizeof(arith_t));
	}




#ifndef __SYNTHESIS__
	std::cout << std::dec << std::endl;
#endif
	// Initializing variables
	init_buffer_int(dint);
	sao_top(data_in, data_out, baeState, tempBst, ctxTables);
	coding_quadtree(data_in,data_out,dint, baeState, tempBst,ctxTables);
	parseEOSSF(baeState, tempBst,  ctxTables, symbolVal);

#ifndef __SYNTHESIS__
	std::cout << std::dec << std::endl;
	printArray<UChar, int>("Intra Prediction Chroma ", 64, 64, (UChar*)dint.IntraPredModeC);
#endif

//Store Data
	memcpy((int8_t*)tranCoeff_0, dint.TransCoeffLevel_0, 64*64*sizeof(int8_t));
	memcpy((int8_t*)tranCoeff_1, dint.TransCoeffLevel_1, 64*64*sizeof(int8_t));
	memcpy((int8_t*)tranCoeff_2, dint.TransCoeffLevel_2, 64*64*sizeof(int8_t));
	memcpy((char*)cqtDepth, dint.cqtDepth, 64*64*sizeof(char));
	memcpy((UChar*)IntraPredModeY, dint.IntraPredModeY, 64*64*sizeof(UChar));
	memcpy((UChar*)IntraPredModeC, dint.IntraPredModeC, 64*64*sizeof(UChar));
	memcpy((arith_t*)initArithState, &baeState,sizeof(arith_t));
	memcpy((data_out_t*)data_out_s, &data_out,sizeof(data_out_t));

	store_global_ram(globalCtx, ctxTables);
}




