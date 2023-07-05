#include "deBin.h"
#include "arith_dec.h"
#include "utils.h"


void parseSAOTypeIdx(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, SAO_TYPE_IDX_CTX_ADDR, ctxTables);
	if(!binVal) {
		symbolVal = binVal;
		return;
	} else {
		decode_decision(BYPASS, state, binVal, bStream, SAO_TYPE_IDX_CTX_ADDR, ctxTables);
		if(binVal) {
			symbolVal = 2;
		} else {
			symbolVal = 1;
		}
	}
#ifndef __SYNTHESIS__
	std::cout << "Decoding SAO type IDX" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl << std::endl;
#endif
}

// TODO : Fix sao merge flag
void parseSAOMergeFlag(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	bool binVal;
	decode_decision(BYPASS, state, binVal, bStream, 0, 0);
	symbolVal = binVal;
#ifndef __SYNTHESIS__
	std::cout << "Decoding Merge Flag" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
};

void parseSAOOffsetAbs(arith_t& state, UChar* bStream, UInt& symbolVal){
	bool binVal;
	symbolVal = 0;
	do {
		decode_decision(BYPASS, state, binVal, bStream, 0, 0);
		symbolVal += binVal;
	} while (binVal & (symbolVal < 5));

//	symbolVal -= 1;
#ifndef __SYNTHESIS__
	std::cout << "Decoding SAO abs offset" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}
void parseSAOOffsetSign(arith_t& state, UChar* bStream, UInt& symbolVal){
	bool binVal;
	decode_decision(BYPASS, state, binVal, bStream, 0, 0);
	symbolVal = binVal;
#ifndef __SYNTHESIS__
	std::cout << "Decoding SAO abs Sign" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}
void parseSAOEO(arith_t& state, UChar* bStream, UInt& symbolVal){
	UChar binIdx;
	binIdx = 0;
	bool binVal;
	symbolVal = 0;

	while(binIdx < 2) {
		decode_decision(BYPASS, state, binVal, bStream, 0, 0);
		symbolVal = (symbolVal << 1) + binVal;
		binIdx +=1;
	}
#ifndef __SYNTHESIS__
	std::cout << "Decoding  SAO EO" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}
void parseSAOBO(arith_t& state, UChar* bStream, UInt& symbolVal){
	UChar binIdx;
	binIdx = 0;
	bool binVal;
	symbolVal = 0;

	while(binIdx < 5) {
		decode_decision(BYPASS, state, binVal, bStream, 0, 0);
		symbolVal = (symbolVal << 1) + binVal;
		binIdx +=1;
	}
#ifndef __SYNTHESIS__
	std::cout << "Decoding SAO BO" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}

bool componentID(int clIdx) {
	if(clIdx==1) return 0;
	return 1;
}

void sao_top(data_in_t& inp,data_out_t& out, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]){

#ifndef __SYNTHESIS__
	std::cout << "SAO DECODING " << std::endl << std::endl;
#endif
	UInt symbolVal;
	bool sao_merge_left_flag=0;
	bool sao_merge_up_flag=0;
	UChar sao_type_idx_luma=0;
	UChar sao_type_idx_chroma=0;
	UChar sao_offset_abs[3][4]={{0, 0, 0, 0},{0, 0, 0, 0}, {0, 0, 0, 0}};
	bool sao_offset_sign[3][4] = {{0, 0, 0, 0},{0, 0, 0, 0}, {0, 0, 0, 0}};
	UChar sao_eo_class_luma;
	UChar sao_eo_class_chroma;


	if(inp.left_ctu.is_available){
		parseSAOMergeFlag(state, bStream, ctxTables, symbolVal);
		sao_merge_left_flag = symbolVal;
	} else {
		sao_merge_left_flag = 0;
	}
	if(inp.up_ctu.is_available && sao_merge_left_flag){
		parseSAOMergeFlag(state, bStream, ctxTables, symbolVal);
		sao_merge_up_flag = symbolVal;
	} else {
		sao_merge_up_flag = 0;
	}

	if( (!sao_merge_up_flag) && (!sao_merge_left_flag)) {
		for(int clIdx=0; clIdx < 3; clIdx++) {
			if ( (inp.s_header.slice_sao_luma_flag & clIdx ==0) || (inp.s_header.slice_sao_chroma_flag & clIdx  > 0) ) {
				if(clIdx ==0) {
					parseSAOTypeIdx(state, bStream, ctxTables, symbolVal);
					sao_type_idx_luma = symbolVal;
					out.SaoTypeIdx[0] = sao_type_idx_luma;
				}

				if(clIdx==1) {
					parseSAOTypeIdx(state, bStream, ctxTables, symbolVal);
					sao_type_idx_chroma = symbolVal;
					out.SaoTypeIdx[1] = out.SaoTypeIdx[2] = sao_type_idx_chroma;
				}

				if(out.SaoTypeIdx[clIdx] > 0) {
					for(int i=0; i < 4; i++) {
						parseSAOOffsetAbs(state, bStream, symbolVal);
						sao_offset_abs[clIdx][i] = symbolVal;
					}
					if(out.SaoTypeIdx[clIdx]==1) {
						for(int i=0; i < 4; i++) {
							if(sao_offset_abs[clIdx][i]>0) {
								parseSAOOffsetSign(state, bStream, symbolVal);
								sao_offset_sign[clIdx][i] = symbolVal;
							}
						}
						parseSAOBO(state, bStream, symbolVal);
						out.sao_band_position[clIdx] = symbolVal;
					} else {
						if(clIdx==0) {
							parseSAOEO(state, bStream, symbolVal);
							sao_eo_class_luma = symbolVal;
							out.SaoEOClass[0] = sao_eo_class_luma;
						}

						if (clIdx==1) {
							parseSAOEO(state, bStream, symbolVal);
							sao_eo_class_chroma = symbolVal;
							out.SaoEOClass[1] = out.SaoEOClass[2] = sao_eo_class_chroma;
						}
						sao_offset_sign[clIdx][0] = sao_offset_sign[clIdx][1] = 0;
						sao_offset_sign[clIdx][2] = sao_offset_sign[clIdx][3] = 1;
					}
				}

			}
			out.SaoOffsetVal[clIdx][0] = 0;
			for(int i=0; i<4; i++){
				if (sao_offset_sign[clIdx][i]) {
					out.SaoOffsetVal[clIdx][i+1] = -1*sao_offset_abs[clIdx][i];
				} else{
					out.SaoOffsetVal[clIdx][i+1] = sao_offset_abs[clIdx][i];
				}
			}
		}
	} else {
		// Taking from neighbour
		// TODO : make it pipelines
		if (sao_merge_left_flag) {
			for(int i=0; i<3; i++){
				out.SaoTypeIdx[i] = inp.left_ctu.SaoTypeIdx[i];
				out.SaoEOClass[i] = inp.left_ctu.SaoEOClass[i];
				out.sao_band_position[i] = inp.left_ctu.sao_band_position[i];
			}
			for(int i=0; i<3; i++){
				for(int j=0; j<5; j++){
					out.SaoOffsetVal[i][j] = inp.left_ctu.SaoOffsetVal[i][j];
				}
			}
		}
		if (sao_merge_up_flag) {
			for(int i=0; i<3; i++){
				out.SaoTypeIdx[i] = inp.up_ctu.SaoTypeIdx[i];
				out.SaoEOClass[i] = inp.up_ctu.SaoEOClass[i];
				out.sao_band_position[i] = inp.up_ctu.sao_band_position[i];
			}
			for(int i=0; i<3; i++){
				for(int j=0; j<5; j++){
					out.SaoOffsetVal[i][j] = inp.up_ctu.SaoOffsetVal[i][j];
				}
			}
		}
	}
#ifndef __SYNTHESIS__
	std::cout << "====================== Internal Sao Buffer =========================" << std::endl;
	printArray<bool, int>("sao_offset_sign", 3, 4, (bool*)sao_offset_sign);
	printArray<UChar, int>("sao_offset_abs", 3, 4, (UChar*)sao_offset_abs);
#endif

}
