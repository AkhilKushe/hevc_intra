#include "arith_dec.h"
#include "utils.h"
#include "init_tables.h"

void arith_init(UChar* bStream, arith_t& state){
	state.ivlCurrRange = 510;
	state.bstate.currIdx = 0;
	state.bstate.held_aligned_word=0;
	state.bstate.n_bits_held=0;
	state.ivlOffset = bitStream_read_bits(bStream, 9, state.bstate);
#ifndef __SYNTHESIS__
	std::cout << "Initial Offset : " <<state.ivlOffset << std::endl;
#endif
}

UChar ctxLoad(UChar ctxTables[MAX_NUM_CTX_MOD], UInt ctxAddr){
	return ctxTables[ctxAddr];
}
void ctxUpdate(UChar ctxTables[MAX_NUM_CTX_MOD], UInt ctxAddr, bool isMPS){
	UChar current_state = ctxTables[ctxAddr];
	if(isMPS){
		ctxTables[ctxAddr] = transMPS[current_state];
	} else {
		ctxTables[ctxAddr] = transLPS[current_state];
	}
}

void decode_regular(arith_t& state, bool& binVal, UChar* bStream, UInt ctxAddr, UChar ctxTables[MAX_NUM_CTX_MOD]){
	UChar ctxState = ctxLoad(ctxTables, ctxAddr);
	int pState = ctxState >> 1;
	bool valMps = ctxState & 1;

	int qRangeIdx = (state.ivlCurrRange >> 6) & 3;
	int ivlLpsRange = lpsTable[pState][qRangeIdx];
	state.ivlCurrRange = state.ivlCurrRange - ivlLpsRange;

	bool isMps = 0;
	if (state.ivlOffset >= state.ivlCurrRange) {
		// LPS is chosen
		binVal = 1 - valMps;
		state.ivlOffset = state.ivlOffset - state.ivlCurrRange;
		state.ivlCurrRange = ivlLpsRange;

	} else {
		binVal = valMps;
		isMps = 1;
	}

	ctxUpdate(ctxTables, ctxAddr, isMps);

		// Renormalization
	UInt val;
	while (state.ivlCurrRange < 256) {
		state.ivlCurrRange <<= 1;
		state.ivlOffset <<= 1;
		val = bitStream_read_bits(bStream, 1, state.bstate);
		state.ivlOffset = state.ivlOffset | val;
	}
#ifndef __SYNTHESIS__
	std::cout << "Decoding Mode : Regular" << std::endl;
	std::cout << "Currr Range : " << std::hex << state.ivlCurrRange << std::endl;
	std::cout << "Offset : " << std::hex << state.ivlOffset << std::endl;
	std::cout << "Bin Val : "<< std::hex << binVal << std::endl;
	std::cout << "input  context : "<< (int)ctxState << std::endl;
#endif
}



void decode_bypass(arith_t& state, bool& binVal, UChar* bStream){
	state.ivlOffset = state.ivlOffset << 1;
	UInt val;
	val = bitStream_read_bits(bStream, 1, state.bstate);

	state.ivlOffset = state.ivlOffset | val;

	if(state.ivlOffset >= state.ivlCurrRange) {
		binVal = 1;
		state.ivlOffset = state.ivlOffset - state.ivlCurrRange;
	} else {
		binVal = 0;
	}
#ifndef __SYNTHESIS__
	std::cout << "Decode Mode : Bypass" << std::endl;
	std::cout << "Currr Range : "<< std::hex << state.ivlCurrRange << std::endl;
	std::cout << "Offset : "<< std::hex << state.ivlOffset << std::endl;
	std::cout << "Bin Val : "<< std::hex << binVal << std::endl;
#endif
}

void decode_terminate(arith_t& state, bool& binVal, UChar* bStream){

}


void decode_decision(BAEMode mode, arith_t& state, bool& binVal, UChar* bStream, UInt ctxAddr, UChar ctxTables[MAX_NUM_CTX_MOD]){
	ArithDec bae;

	switch(mode){
	case REGULAR: decode_regular(state, binVal, bStream, ctxAddr, ctxTables);
				break;
	case BYPASS: decode_bypass(state, binVal, bStream);
				break;
	case TERMINATE: decode_terminate(state, binVal, bStream);
				break;
	}

}
