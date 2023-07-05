#include "intra_se.h"
#include "utils.h"

void parsePartMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, PART_MODE_CTX_ADDR, ctxTables);
	symbolVal = 1- binVal;
#ifndef __SYNTHESIS__
	std::cout << "Decoding part mode" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl << std::endl;
#endif

}


void parsePrevIntraLumaPredFlag(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, PREV_INTRA_LUMA_PRED_FLAG_CTX_ADDR, ctxTables);
	symbolVal = binVal;
#ifndef __SYNTHESIS__
	std::cout << "Decoding prev intra luma pred flag" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl << std::endl;
#endif
}

void parseMpmIdx(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	bool binVal;
	symbolVal = 0;
	do {
		decode_decision(BYPASS, state, binVal, bStream, 0, 0);
		symbolVal += binVal;
	} while (binVal & (symbolVal < 2));

//	symbolVal -= 1;
#ifndef __SYNTHESIS__
	std::cout << "Decoding mpm idx" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}


void parseRemIntraLumaPredMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
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
	std::cout << "Decoding rem intra luma pred mode" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl<< std::endl;
#endif
}

void parseIntraChromaPredMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	UChar binIdx =0;

	decode_decision(REGULAR, state, binVal, bStream, INTRA_CHROMA_PRED_MODE_CTX_ADDR, ctxTables);
	symbolVal = (symbolVal << 1) + binVal;
	binIdx+=1;

	if (binVal==1) {
		while ((binIdx < 3)) {
			decode_decision(BYPASS, state, binVal, bStream, INTRA_CHROMA_PRED_MODE_CTX_ADDR, ctxTables);
			symbolVal = (symbolVal << 1) + binVal;
			binIdx+=1;
		}
		symbolVal = symbolVal-4;
	} else {
		symbolVal = 4;
	}

#ifndef __SYNTHESIS__
	std::cout << "Decoding intra chroma pred mode" << std::endl;
	std::cout << "Symbol Val : " << symbolVal << std::endl << std::endl;
#endif
}


void setIntraPredMode(UChar partIdx, UChar partSize, CU_t& cu ,data_in_t& din, data_out_t&dout, internal_data_t& dinternal){
	UChar candIntraPredModeA;
	UChar candIntraPredModeB;
	UChar candModeList[3];
	bool availableA, availableB;

	UChar PredModeY;
	UChar PredModeC;

	uint32_t x = cu.x + (partIdx&1)*partSize;
	uint32_t y = cu.y + (partIdx>>1)*partSize;

	if (x==0) {
		if (din.left_ctu.is_available) {
			availableA = true;
		} else {
			availableA = false;
		}
	} else {
		availableA = true;
	}
	if (y==0) {
		if (din.up_ctu.is_available) {
			availableB = true;
		} else {
			availableB = false;
		}
	} else {
		availableB = true;
	}

	// for A
	if (!availableA) {
		candIntraPredModeA = 1;				// INTRA_DC
	} else {
		if (x -1 < 0) {
			candIntraPredModeA = din.left_ctu.IntraPredModeY[y];
		} else {
			candIntraPredModeA = dinternal.IntraPredModeY[x-1][y];

		}
	}

	// For B
	if (!availableB) {
		candIntraPredModeB = 1;				// INTRA_DC
	} else {
		if (y -1 < 0) {
			candIntraPredModeB = din.up_ctu.IntraPredModeY[x];
		} else {
			candIntraPredModeB = dinternal.IntraPredModeY[x][y-1];

		}
	}

	if (candIntraPredModeA==candIntraPredModeB){
		if (candIntraPredModeA < 2) {
			candModeList[0] = 0;
			candModeList[1] = 1;
			candModeList[2] = 26;
		} else {
			candModeList[0] = candIntraPredModeA;
			candModeList[1] = 2+((candIntraPredModeA+29)%32);
			candModeList[2] = 2+((candIntraPredModeA-2+1)%32);
		}
	} else {
		if ((candIntraPredModeA != 0)&&(candIntraPredModeB!=0)){
			candModeList[0] = candIntraPredModeA;
			candModeList[1] = candIntraPredModeB;
			candModeList[2] = 0;
		} else if ((candIntraPredModeA != 1)&&(candIntraPredModeB!=1)){
			candModeList[0] = candIntraPredModeA;
			candModeList[1] = candIntraPredModeB;
			candModeList[2] = 1;
		} else{
			candModeList[0] = candIntraPredModeA;
			candModeList[1] = candIntraPredModeB;
			candModeList[2] = 26;
		}
	}

	UChar temp;
	if (cu.prev_intra_luma_pred_flag[partIdx]) {
		PredModeY = candModeList[cu.mpm_idx[partIdx]];
	} else {
		if (candModeList[0] > candModeList[1]) {
			temp = candModeList[0];
			candModeList[0] = candModeList[1];
			candModeList[1] = temp;
		}
		if (candModeList[0] > candModeList[2]) {
			temp = candModeList[0];
			candModeList[0] = candModeList[2];
			candModeList[2] = temp;
		}
		if (candModeList[1] > candModeList[2]) {
			temp = candModeList[1];
			candModeList[1] = candModeList[2];
			candModeList[2] = temp;
		}
		PredModeY = cu.rem_intra_luma_pred_mode[partIdx];
		for(int i=0; i<3; i++){
			if(PredModeY >= candModeList[i]) {
				PredModeY +=1;
			}
		}
	}

	if (partIdx==0) {
		switch(cu.intra_chroma_pred_mode) {
		case 0: PredModeC = 0;
				break;
		case 1: PredModeC = 26;
				break;
		case 2: PredModeC = 10;
				break;
		case 3: PredModeC = 1;
				break;
		case 4: PredModeC=PredModeY;
				break;
		}

		if ((PredModeC==PredModeY)&&(cu.intra_chroma_pred_mode!=4)){
			PredModeC  =34;
		}
		if (PredModeC==34) {
			PredModeC = PredModeY;
		}

		if (cu.part_mode==PART_NxN){
			// set pred buffer chroma
			for(int i=x; i<(x+partSize); i++) {
				for(int j=y; j<(y+partSize); j++){
					dinternal.IntraPredModeC[i][j] = PredModeC;
				}
			}
		} else {
			for(int i=x; i<(x+2*partSize); i++) {
				for(int j=y; j<(y+2*partSize); j++){
					dinternal.IntraPredModeC[i][j] = PredModeC;
				}
			}
		}
	}
	// set pred buffer luma
	for(int i=x; i<(x+partSize); i++) {
		for(int j=y; j<(y+partSize); j++){
			dinternal.IntraPredModeY[i][j] = PredModeY;
		}
	}

#ifndef __SYNTHESIS__
	std::cout << "Part Idx : " << std::dec << (int)partIdx << std::endl;
	std::cout << "Part size : "<< std::dec <<  (int)partSize << std::endl;
	std::cout << "Avail A : " << availableA << std::endl;
	std::cout << "avail B : " << availableB << std::endl;
	std::cout << "CandIntraPredModeA : " <<  (int)candIntraPredModeA << std::endl;
	std::cout << "CandIntraPredModeB : " <<  (int)candIntraPredModeB << std::endl;

	printArray<UChar, int>("Candidate mode list", 3, 1, candModeList);

	std::cout << "Pred Mode Luma : " << (int) PredModeY << std::endl;
	std::cout << "Pred Mode Chroma : " << (int) PredModeC << std::endl;

	//printArray<UChar, int>("Pred mode luma arr :", 64, 64, (UChar*)dinternal.IntraPredModeY);
	//printArray<UChar, int>("Pred mode CHroma arr :", 64, 64, (UChar*)dinternal.IntraPredModeC);

	std::cout << std::endl <<std::endl;

#endif

}

