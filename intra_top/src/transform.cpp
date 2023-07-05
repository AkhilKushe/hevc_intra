#include "transform.h"
#include "init_tables.h"
#include "utils.h"

void parseCbfC(UChar trafoDepth, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, CBF_CHROMA_CTX_ADDR+trafoDepth, ctxTables);
	symbolVal = binVal;

#ifndef __SYNTHESIS__
	std::cout << "Decoding Cbf_C" << std::endl;
	std::cout << "ctxInc : " << (int)trafoDepth << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void parseCbfLuma(UChar trafoDepth, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar ctxInc;

	if(trafoDepth==0){
		ctxInc=1;
	} else {
		ctxInc = 0;
	}

	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, CBF_LUMA_CTX_ADDR+ctxInc, ctxTables);
	symbolVal = binVal;

#ifndef __SYNTHESIS__
	std::cout << "Decoding Cbf_luma" << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void parseSplitTransformFlag(UChar log2TrafoSize, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar ctxInc;
	ctxInc = 5-log2TrafoSize;
	symbolVal = 0;
	bool binVal;
	decode_decision(REGULAR, state, binVal, bStream, SPLIT_TRANSFORM_FLAG_CTX_ADDR+ctxInc, ctxTables);
	symbolVal = binVal;

#ifndef __SYNTHESIS__
	std::cout << "Decoding Split Transform Flag" << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif

}

void parseSkipTransformFlag(UChar cIdx, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	symbolVal = 0;
	bool binVal;
	if (cIdx==0) {
		decode_decision(REGULAR, state, binVal, bStream, TRANSFORM_SKIP_FLAG_CTX_ADDR_0, ctxTables);
	} else {
		decode_decision(REGULAR, state, binVal, bStream, TRANSFORM_SKIP_FLAG_CTX_ADDR_1, ctxTables);
	}
	symbolVal = binVal;

#ifndef __SYNTHESIS__
	std::cout << "Decoding Skip transform flag" << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void parseSigCoeffXPrefix(UChar log2TrafoSize, UChar cIdx, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
#pragma HLS INLINE off
	bool binVal;
	symbolVal = 0;
	UChar binIdx = 0;

	UChar ctxOffset, ctxShift, ctxInc;

	if(cIdx==0) {
		ctxOffset = 3*(log2TrafoSize-2)+((log2TrafoSize-1)>>2);
		ctxShift = (log2TrafoSize+1)>>2;
	} else {
		ctxOffset = 15;
		ctxShift = log2TrafoSize-2;
	}

	UChar cmax=(log2TrafoSize << 1 )-1;


	do {
		ctxInc = (binIdx >> ctxShift)+ctxOffset;
		decode_decision(REGULAR, state, binVal, bStream, SIG_COEFF_X_PREFIX_CTX_ADDR+ctxInc, ctxTables);
		symbolVal += binVal;
		binIdx+=1;
	} while (binVal & (binIdx < cmax));

//	symbolVal -= 1;
#ifndef __SYNTHESIS__
	std::cout << "Sig coeff X prefix" << std::endl;
	std::cout << "ctxOffset : " <<(int) ctxOffset << std::endl;
	std::cout << "ctxShift : " << (int)ctxShift << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl<< std::endl;
#endif
}
void parseSigCoeffYPrefix(UChar log2TrafoSize, UChar cIdx, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
#pragma HLS INLINE off
	bool binVal;
	symbolVal = 0;
	UChar binIdx = 0;

	UChar ctxOffset, ctxShift, ctxInc;

	if(cIdx==0) {
		ctxOffset = 3*(log2TrafoSize-2)+((log2TrafoSize-1)>>2);
		ctxShift = (log2TrafoSize+1)>>2;
	} else {
		ctxOffset = 15;
		ctxShift = log2TrafoSize-2;
	}

	UChar cmax=(log2TrafoSize << 1 )-1;


	do {
		ctxInc = (binIdx >> ctxShift)+ctxOffset;
		decode_decision(REGULAR, state, binVal, bStream, SIG_COEFF_Y_PREFIX_CTX_ADDR+ctxInc, ctxTables);
		symbolVal += binVal;
		binIdx+=1;
	} while (binVal & (binIdx < cmax));

//	symbolVal -= 1;
#ifndef __SYNTHESIS__
	std::cout << "Sig coeff Y prefix" << std::endl;
	std::cout << "ctxOffset : " << (int)ctxOffset << std::endl;
	std::cout << "ctxShift : " <<(int) ctxShift << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl<< std::endl;
#endif
}

void parseSigCoeffSuffix(UInt prefix, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar binIdx;
	binIdx = 0;
	bool binVal;
	symbolVal = 0;

	UChar cmax = ( 1 << ( ( prefix >> 1 ) - 1 ) - 1 );
	UChar fl = FL_CONVERT[cmax];

	while(binIdx < fl) {
		decode_decision(BYPASS, state, binVal, bStream, 0, 0);
		symbolVal = (symbolVal << 1) + binVal;
		binIdx +=1;
	}
#ifndef __SYNTHESIS__
	std::cout << "Suffix val" << std::endl;
	std::cout << "cmax : " <<std::endl;
	std::cout << "Symbol Val : " <<(int) symbolVal << std::endl<< std::endl;
#endif
}

void parseCodedSubBlockFlag(UChar xS, UChar yS, bool coded_sub_block[8][8], UChar log2TrafoSize, UChar cIdx, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar csbf;
	UChar ctxInc;
	bool binVal;
	symbolVal = 0;

	csbf=0;
	if(xS < ((1<<(log2TrafoSize-2))-1)){
		csbf += coded_sub_block[xS+1][yS];
	}

	if(yS < ((1<<(log2TrafoSize-2))-1)){
		csbf += coded_sub_block[xS][yS+1];
	}

	if(cIdx==0){
		ctxInc = min<UChar>(csbf, 1);
	} else {
		ctxInc = min<UChar>(csbf, 1) + 2;
	}
	//Decode bin
	decode_decision(REGULAR, state, binVal, bStream, CODED_SUB_BLOCK_FLAG_CTX_ADDR+ctxInc, ctxTables);
	symbolVal=binVal;
#ifndef __SYNTHESIS__
	std::cout << "////////////////// Decoding coded sub block flag at" << (int)xS << " " <<(int)yS <<  "////////////////" << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void parseSigCoeffFlag(UChar xC, UChar yC, UChar scanIdx, bool coded_sub_block[8][8], UChar log2TrafoSize, UChar cIdx, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar xS, yS, xP, yP;
	UChar prevCsbf;
	UChar sigCtx;
	UChar ctxInc;
	bool binVal;
	symbolVal=0;

	xS = xC>>2;
	yS = yC>>2;
	xP = xC&3;
	yP = yC&3;

	if (log2TrafoSize==2){
		sigCtx = CTX_IDX_MAP[(yC<<2)+xC];
	} else if ((xC+yC)==0){
		sigCtx=0;
	} else{
		prevCsbf=0;
		if (xS < (1<<(log2TrafoSize-2))-1) {
			prevCsbf += coded_sub_block[xS+1][yS];
		}
		if (yS < (1<<(log2TrafoSize-2))-1) {
			prevCsbf += coded_sub_block[xS][yS+1]<<1;
		}
		switch(prevCsbf){
		case 0 : sigCtx = (xP+yP == 0)?2:(xP+yP < 3) ? 1:0;
					break;
		case 1 : sigCtx = (yP==0)?2:(yP==1) ? 1:0;
					break;
		case 2 : sigCtx = (xP==0)?2:(xP==1) ? 1:0;
					break;
		case 3 : sigCtx = 2;
					break;
		}

		if(cIdx==0){
			if (xS+yS>0){
				sigCtx += 3;
			}
			if (log2TrafoSize==3) {
				sigCtx += ((scanIdx==0)?9:15);
			} else {
				sigCtx += 21;
			}
		} else {
			sigCtx += ((log2TrafoSize==3)?9:12);
		}
	}

	ctxInc = (cIdx==0)?sigCtx:(sigCtx+27);

	//Decode bin
	decode_decision(REGULAR, state, binVal, bStream, SIG_COEFF_FLAG_CTX_ADDR+ctxInc, ctxTables);
	symbolVal=binVal;
#ifndef __SYNTHESIS__
	std::cout << "////////////////// Decoding sigCoeffFlag at" << (int)xC << " " <<(int)yC <<  "////////////////" << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void parseCoeffAbsLevelG1(UChar cIdx, TU_t& tu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar ctxSet, greater1Ctx, ctxInc;
	bool binVal;
	ctxSet = tu.ctxSet;
	greater1Ctx = tu.greater1Ctx;
	ctxInc = (ctxSet*4) + min<UChar>(3, greater1Ctx);
	symbolVal=0;

	if(cIdx>0){
		ctxInc+= 16;
	}
	// Decode bin
	decode_decision(REGULAR, state, binVal, bStream, COEFF_ABS_G1_CTX_ADDR+ctxInc, ctxTables);
	symbolVal=binVal;

	tu.G2ctxSet = ctxSet;

	if (greater1Ctx > 0){
		tu.lastGreater1Ctx = symbolVal;
		if(tu.lastGreater1Ctx==1){
			greater1Ctx = 0;
		} else {
			greater1Ctx += 1;
		}
	}

	tu.lastGreater1Ctx=greater1Ctx;
	if(tu.lastGreater1Ctx>0){
		tu.lastGreater1Flag = symbolVal;
		if(tu.lastGreater1Flag==1){
			tu.lastGreater1Ctx=0;
		} else {
			tu.lastGreater1Ctx += 1;
		}
	}
	

	tu.ctxSet = ctxSet;
	tu.greater1Ctx = greater1Ctx;

#ifndef __SYNTHESIS__
	std::cout << "////////////////// Decoding coeff_abs_grater_level1  ////////////////" << std::endl;
	std::cout << "ctxSet : " << (int)ctxSet << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}
void parseCoeffAbsLevelG2(UChar cIdx, TU_t& tu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UChar ctxInc;
	bool binVal;
	ctxInc = tu.G2ctxSet;
	symbolVal=0;

	if(cIdx>0){
		ctxInc += 4;
	}

	// Decode bin
	decode_decision(REGULAR, state, binVal, bStream, COEFF_ABS_G2_CTX_ADDR+ctxInc, ctxTables);
	symbolVal=binVal;
#ifndef __SYNTHESIS__
	std::cout << "////////////////// Decoding coeff_abs_grater_level2  ////////////////" << std::endl;
	std::cout << "ctxInc : " << (int)ctxInc << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}
void parseCoeffAbsRem(UChar baseLevel, TU_t& tu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	UInt cLastAbsLevel;
	UChar cLastRiceParam;
	UInt cAbsLevel;
	UChar cRiceParam;



	cLastAbsLevel= tu.cLastAbsLevel;
	cLastRiceParam=tu.cLastRiceParam;
	symbolVal=0;

	if(cLastAbsLevel > (3*(1<<cLastRiceParam))){
		cRiceParam = min<UChar>(cLastRiceParam+1, 4);
	} else {
		cRiceParam = min<UChar>(cLastRiceParam, 4);
	}

#ifndef __SYNTHESIS__
	std::cout << "/////////////////////  Decoding coeff_abs_remaining_flag /////////////////////" << std::endl;
	std::cout << "cRiceParam :" << (int)cRiceParam << std::endl;
#endif

	// Decode bin
	UInt prefix;
	bool codeWord;
	prefix=0;
	codeWord = 1;

	while(codeWord){
		prefix+=1;
		decode_decision(BYPASS, state, codeWord, bStream, 0, 0);
	}

	codeWord = 1-codeWord;
	prefix = prefix - codeWord;
	codeWord = 0;

	UInt tempSymbolVal;
	UChar cntr;
	cntr=0;
	tempSymbolVal=0;

	if(prefix < 3){
		while(cntr < cRiceParam){
			decode_decision(BYPASS, state, codeWord, bStream, 0, 0);
			cntr+=1;
			tempSymbolVal = (tempSymbolVal <<1)+codeWord;
		}

		symbolVal = (prefix<<cRiceParam)+tempSymbolVal;

	} else {
		while(cntr < (prefix-3+cRiceParam)){
			decode_decision(BYPASS, state, codeWord, bStream, 0, 0);
			cntr+=1;
			tempSymbolVal = (tempSymbolVal <<1)+codeWord;
		}
		symbolVal = (((1<<(prefix-3))+3-1)<<cRiceParam)+tempSymbolVal;
	}


	cAbsLevel = baseLevel + symbolVal;
	tu.cLastAbsLevel = cAbsLevel;
	tu.cLastRiceParam = cRiceParam;
#ifndef __SYNTHESIS__
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}
void parseCoeffSign(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal){
	// Decode bin
	bool binVal;
	symbolVal =0;
	decode_decision(BYPASS, state, binVal, bStream, 0, 0);
	symbolVal=binVal;
#ifndef __SYNTHESIS__
	std::cout << "////////////////// Decoding coeff_sign_flag  ////////////////" << std::endl;
	std::cout << "Symbol Val : " << (int)symbolVal << std::endl << std::endl;
#endif
}

void residual_coding(UChar cIdx,UChar trafoSize, data_in_t& din, data_out_t& dout, internal_data_t& dint,CU_t& cu, TU_t& tu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]){
#ifndef __SYNTHESIS__
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Residual coding clIdx =  " << (int)cIdx << std::endl;
	std::cout << "trafoSize = " << (int)trafoSize << std::endl;
#endif
	UChar last_sig_coeff_x_prefix, last_sig_coeff_y_prefix, last_sig_coeff_x_suffix, last_sig_coeff_y_suffix;
	UChar lastScanPos;
	UChar lastSubBlock;

	bool coded_sub_block[8][8]; 		//Considering max tb size as 32
	bool sig_coeff_flag[32][32];

	// Per subblock vals
	bool coeff_abs_level_greater1_flag[16];
	bool coeff_abs_level_greater2_flag[16];
	bool coeff_sign_flag[16];
	UChar coeff_abs_level_remaining[16];

	UInt symbolVal;
	UChar xC, yC, xS, yS;
	UChar LastSignificantCoeffX, LastSignificantCoeffY;

	UChar subBlockScan[64][2];
	UChar coeffBlockScan[64][2];

	bool inferSbDcSigCoeffFlag =0;
	bool escapeDataPresent = 0;
	UChar firstSigScanPos;
	int8_t lastSigScanPos;
	UChar numGreater1Flag;
	int8_t lastGreater1ScanPos;
	UChar numSigCoeff;
	UChar sumAbsLevel;
	UChar baseLevel;
	UChar cmp;
	int8_t transCoeffVal;
	UInt bitPad;
	bitPad = 0xFFFF;
	lastScanPos = 16;
	lastSubBlock = (1<<(trafoSize-2))*(1<<(trafoSize-2)) -1;

	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			coded_sub_block[i][j] = 0;
		}
	}
	for(int i=0; i<32; i++){
		for(int j=0; j<32; j++){
			sig_coeff_flag[i][j] = 0;
		}
	}

	if (din.pps.transform_skip_enabled_flag && (! cu.cu_transquant_bypass_flag) && (trafoSize <= din.Log2MaxTransformSkipSize)){
		parseSkipTransformFlag(cIdx, state, bStream, ctxTables, symbolVal);
		tu.transform_skip_flag = symbolVal;
	} else {
		tu.transform_skip_flag = 0;
	}

	//bitOut.write(symbolVal);
	//bitOut.write(bitPad);

	parseSigCoeffXPrefix(trafoSize, cIdx, state, bStream, ctxTables, symbolVal);
	last_sig_coeff_x_prefix = symbolVal;
	//bitOut.write(symbolVal);

	parseSigCoeffYPrefix(trafoSize, cIdx, state, bStream, ctxTables, symbolVal);
	last_sig_coeff_y_prefix = symbolVal;
	//bitOut.write(symbolVal);

	LastSignificantCoeffX = last_sig_coeff_x_prefix;
	LastSignificantCoeffY = last_sig_coeff_y_prefix;

	if (last_sig_coeff_x_prefix > 3){
		parseSigCoeffSuffix(last_sig_coeff_x_prefix, state, bStream, ctxTables, symbolVal);
		last_sig_coeff_x_suffix = symbolVal;
		LastSignificantCoeffX =  ( 1 << ( (last_sig_coeff_x_prefix >> 1 ) - 1 ) ) *( 2 + (last_sig_coeff_x_prefix & 1 ) ) + last_sig_coeff_x_suffix;
	}

	if(last_sig_coeff_y_prefix > 3) {
		parseSigCoeffSuffix(last_sig_coeff_y_prefix, state, bStream, ctxTables, symbolVal);
		last_sig_coeff_y_suffix = symbolVal;
		LastSignificantCoeffY =  ( 1 << ( (last_sig_coeff_y_prefix >> 1 ) - 1 ) ) *( 2 + (last_sig_coeff_y_prefix & 1 ) ) + last_sig_coeff_y_suffix;
	}

#ifndef __SYNTHESIS__
	std::cout << "LastSigCoeffX : " << (int)LastSignificantCoeffX << std::endl;
	std::cout << "LastSigCoeffY : " << (int)LastSignificantCoeffY << std::endl;
#endif

	UChar scanIdx;
	get_scanIdx(tu, trafoSize, dint, cIdx, scanIdx);
	UChar temp;
	if(scanIdx==2){
		temp = LastSignificantCoeffX;
		LastSignificantCoeffX = LastSignificantCoeffY;
		LastSignificantCoeffY = temp;
	}

	switch (scanIdx)
	{
	case 0:
		up_right_diagonal_scan((1<<(trafoSize-2)), subBlockScan);
		up_right_diagonal_scan(4, coeffBlockScan);
		break;
	case 1:
		horizontal_scan((1<<(trafoSize-2)), subBlockScan);
		horizontal_scan(4, coeffBlockScan);
		break;
	case 2:
		vertical_scan((1<<(trafoSize-2)), subBlockScan);
		vertical_scan(4, coeffBlockScan);
		break;
	}
	
	do {
		if(lastScanPos==0){
			lastScanPos=16;
			lastSubBlock--;
		}
		lastScanPos--;
		xS = subBlockScan[lastSubBlock][0];
		yS = subBlockScan[lastSubBlock][1];
		xC = (xS<<2)+ coeffBlockScan[lastScanPos][0];
		yC = (yS<<2)+ coeffBlockScan[lastScanPos][1];
#ifndef __SYNTHESIS__
		std::cout << "Checking pos : " << (int)xC << ", " << (int)yC << std::endl;
#endif
	} while ((xC != LastSignificantCoeffX) || (yC != LastSignificantCoeffY));

	// Sub Block processing
	tu.lastGreater1Ctx =1;
	for(int i=lastSubBlock; i>=0; i--){
		// Sub block var initialization
		tu.cLastRiceParam = 0;
		tu.cLastAbsLevel = 0;
		if(i==0 || cIdx>0){
			tu.ctxSet = 0;
		} else {
			tu.ctxSet = 2;
		}
		tu.greater1Ctx = 1;
		if(tu.lastGreater1Ctx==0){
			tu.ctxSet +=1;
		}

		for(int j=0; j<16; j++){
			coeff_abs_level_greater1_flag[j] = 0;
			coeff_abs_level_greater2_flag[j] = 0;
			coeff_sign_flag[j] = 0;
			coeff_abs_level_remaining[j] = 0;
		}

		xS = subBlockScan[i][0];
		yS = subBlockScan[i][1];

		// Directly assuming coded sub block flag is not present
		escapeDataPresent = 0;
		inferSbDcSigCoeffFlag = 0;
		if(i<lastSubBlock && i>0){
			parseCodedSubBlockFlag(xS, yS, coded_sub_block,  tu.log2TrafoSize,  cIdx,  state, bStream, ctxTables, symbolVal);
			coded_sub_block[xS][yS] = symbolVal;
			inferSbDcSigCoeffFlag = 1;
		} else {
			if ((xS == 0 && yS == 0) || (xS == (LastSignificantCoeffX >> 2) && yS == (LastSignificantCoeffY >> 2))) {
				coded_sub_block[xS][yS] = 1;
			} else {
				coded_sub_block[xS][yS] = 0;
			}
		}

		int8_t tmpCnt;
		if (i == lastSubBlock) {
			tmpCnt = lastScanPos - 1;
		} else {
			tmpCnt = 15;
		}

#ifndef __SYNTHESIS__
		std::cout << "Temp Val : " << (int)tmpCnt << std::endl;
		std::cout << "i val : " << (int)i << std::endl;
		std::cout << "lastSubBLock Val : " << (int)lastSubBlock << std::endl << std::endl;
#endif
		//bitOut.write(bitPad);
		for (int n = tmpCnt; n >= 0; n--) {
			xC = (xS << 2) + coeffBlockScan[n][0];
			yC = (yS << 2) + coeffBlockScan[n][1];
			if (coded_sub_block[xS][yS] && (n > 0 || !inferSbDcSigCoeffFlag)) {
				parseSigCoeffFlag(xC, yC, scanIdx, coded_sub_block, trafoSize, cIdx, state, bStream, ctxTables, symbolVal);
				sig_coeff_flag[xC][yC] = symbolVal;
				//bitOut.write(symbolVal);
			} else {
				if ((xC == LastSignificantCoeffX) && (yC == LastSignificantCoeffY) || ((xC & 3) == 0 && (yC & 3) == 0 && inferSbDcSigCoeffFlag && coded_sub_block[xS][yS])) {
					sig_coeff_flag[xC][yC] = 1;
				} else {
					sig_coeff_flag[xC][yC] = 0;
				}
			}

			if (sig_coeff_flag[xC][yC]) {
				inferSbDcSigCoeffFlag = 0;
			}
		}

		// SigCoeffFlag not present for these indices
		for (int n = 15; n > tmpCnt; n--) {
			xC = (xS << 2) + coeffBlockScan[n][0];
			yC = (yS << 2) + coeffBlockScan[n][1];
			if ((xC == LastSignificantCoeffX && yC == LastSignificantCoeffY) || (((xC & 3) == 0 && (yC & 3) == 0) && inferSbDcSigCoeffFlag && coded_sub_block[xS][yS])) {
				sig_coeff_flag[xC][yC] = 1;
			} else {
				sig_coeff_flag[xC][yC] = 0;
			}
		}

        firstSigScanPos = 16;
        lastSigScanPos = -1;
        numGreater1Flag = 0;
        lastGreater1ScanPos = -1;
        //bitOut.write(bitPad);
		for (int n = 15; n >= 0; n--) {
			xC = (xS << 2) + coeffBlockScan[n][0];
			yC = (yS << 2) + coeffBlockScan[n][1];

			if (sig_coeff_flag[xC][yC]) {
				if (numGreater1Flag < 8) {
					parseCoeffAbsLevelG1(cIdx, tu, state, bStream, ctxTables, symbolVal);
					//bitOut.write(symbolVal);
					coeff_abs_level_greater1_flag[n] = symbolVal;
					++numGreater1Flag;
					if (coeff_abs_level_greater1_flag[n] && (lastGreater1ScanPos == -1)) {
						lastGreater1ScanPos = n;
					} else if (coeff_abs_level_greater1_flag[n]) {
						escapeDataPresent = 1;
					}
				} else {
					escapeDataPresent = 1;
				}

				if (lastSigScanPos == -1) {
					lastSigScanPos = n;
				}
				firstSigScanPos = n;
			}
		}

		bool signHidden = (lastSigScanPos - firstSigScanPos) > 3;
		//bitOut.write(bitPad);
		if (lastGreater1ScanPos != -1) {
			parseCoeffAbsLevelG2(cIdx, tu, state, bStream, ctxTables, symbolVal);
			//bitOut.write(symbolVal);
			coeff_abs_level_greater2_flag[lastGreater1ScanPos] = symbolVal;
		}
		//bitOut.write(bitPad);
		for (int n = 15; n >= 0; --n) {
			xC = (xS << 2) + coeffBlockScan[n][0];
			yC = (yS << 2) + coeffBlockScan[n][1];

			if (sig_coeff_flag[xC][yC] && ((!din.pps.sign_data_hiding_enabled_flag) || (!signHidden) || (n != firstSigScanPos))) {
				parseCoeffSign(state, bStream, ctxTables, symbolVal);
				//bitOut.write(symbolVal);
				coeff_sign_flag[n] = symbolVal;
			}
		}

		numSigCoeff = 0;
		sumAbsLevel = 0;
		//bitOut.write(bitPad);
		for (int n = 15; n >= 0; n--) {
			xC = (xS << 2) + coeffBlockScan[n][0];
			yC = (yS << 2) + coeffBlockScan[n][1];
			if (sig_coeff_flag[xC][yC]) {
				baseLevel = 1 + coeff_abs_level_greater1_flag[n] + coeff_abs_level_greater2_flag[n];

				cmp;
				if (numSigCoeff < 8) {
					cmp = (lastGreater1ScanPos == n) ? 3 : 2;
				} else {
					cmp = 1;
				}

				if (baseLevel == cmp) {
					parseCoeffAbsRem(baseLevel, tu, state, bStream, ctxTables, symbolVal);
					//bitOut.write(symbolVal);
					coeff_abs_level_remaining[n] = symbolVal;
				}

				transCoeffVal = (coeff_abs_level_remaining[n] + baseLevel) * (1 - 2 * coeff_sign_flag[n]);

				if (din.pps.sign_data_hiding_enabled_flag && signHidden) {
					sumAbsLevel += (coeff_abs_level_remaining[n] + baseLevel);
					if ((n == firstSigScanPos) && (sumAbsLevel % 2)) {
						transCoeffVal = -transCoeffVal;
					}
				}
#ifndef __SYNTHESIS__
				std::cout << "Setting transform coefficient at " << std::dec << (int)xC << "," << (int)yC << std::endl;
				std::cout << "transCoeffVal = " <<std::dec << (int)transCoeffVal << std::endl;
#endif
				switch (cIdx)
				{
				case 0:
					dint.TransCoeffLevel_0[tu.x+xC][tu.y+yC] = transCoeffVal;
					break;
				case 1:
					dint.TransCoeffLevel_1[tu.x+xC][tu.y+yC] = transCoeffVal;
					break;
				case 2:
					dint.TransCoeffLevel_2[tu.x+xC][tu.y+yC] = transCoeffVal;
					break;
				}
				numSigCoeff++;
			}
		}
	}
}

void transform_unit(data_in_t& din, data_out_t& dout, internal_data_t& dint, CU_t& cu, TU_t& tu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]){
	UChar log2TrafoSizeC;
	UChar cbfLuma, cbfChroma;
	bool cbf_id;

	if(tu.log2TrafoSize==2){
		cbf_id=1;
	} else {
		cbf_id = 2;
	}

	log2TrafoSizeC = max<UChar>(2, tu.log2TrafoSize-1);
	cbfLuma = cu.cbf_luma[0];
	cbfChroma = cu.cbf_cb[cbf_id] || cu.cbf_cr[cbf_id];
#ifndef __SYNTHESIS__
	std::cout << "Residual coding specs" << std::endl;
	std::cout << "cbf_cb" << (int)cu.cbf_cb[0] << " " << (int)cu.cbf_cb[1] << std::endl;
	std::cout << "cbf_cr" << (int)cu.cbf_cr[0] << " " << (int)cu.cbf_cr[1] << std::endl;
	std::cout << "cbf_luma" << (int)cu.cbf_luma[0] << " " << (int)cu.cbf_luma[1] << std::endl;
	std::cout << "cbf_cb" << (int)tu.blkIdx  << std::endl;
#endif
	if(cbfLuma || cbfChroma){
		if(cbfLuma){
			residual_coding( 0, tu.log2TrafoSize, din, dout, dint, cu, tu, state, bStream, ctxTables);
		}

		if(tu.log2TrafoSize>2){
			//tu.log2TrafoSize = log2TrafoSizeC;
			if(cu.cbf_cb[0]){
				residual_coding(1,log2TrafoSizeC, din, dout, dint, cu, tu, state, bStream, ctxTables);
			}
			if(cu.cbf_cr[0]){
				residual_coding(2,log2TrafoSizeC, din, dout, dint, cu, tu, state, bStream, ctxTables);
			}

		} else if(tu.blkIdx==3){
			if(cu.cbf_cb[1]){
				residual_coding(1,tu.log2TrafoSize, din, dout, dint, cu, tu, state, bStream, ctxTables);
			}
			if(cu.cbf_cr[1]){
				residual_coding(2,tu.log2TrafoSize, din, dout, dint, cu, tu, state, bStream, ctxTables);
			}
		}
	}
}

void transform_tree_rec(uint16_t tuIdx, UChar trafoDepth, UChar log2TrafoSize, bool& transform_split, data_in_t& din, data_out_t& dout, internal_data_t& dint, CU_t& cu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]){
	UChar x0, y0, blkIdx;
	UChar tuAddr, cod_x, cod_y;

	switch(trafoDepth){
	case 0: x0 = cu.x;
			y0 = cu.y;
			blkIdx=0;
			break;

	case 1: tuAddr = tuIdx-1;
			cod_y = tuAddr >> 1;
			cod_x = tuAddr & 1;
			x0 = cu.x + cod_x*(1<<log2TrafoSize);
			y0 = cu.y + cod_y*(1<<log2TrafoSize);
			blkIdx=tuAddr;
			break;

	case 2: tuAddr = tuIdx -5;
			tuAddr = TS2RS[tuAddr];
			cod_y = tuAddr >> 2;
			cod_x = tuAddr & 3;
			x0 = cu.x + cod_x*(1<<log2TrafoSize);
			y0 = cu.y + cod_y*(1<<log2TrafoSize);
			blkIdx = (tuIdx-5)>>4;
			break;

	case 3: tuAddr = tuIdx -21;
			blkIdx = tuAddr >> 4;
			tuAddr = tuAddr&15;
			tuAddr = TS2RS[tuAddr];
			cod_y = tuAddr >> 2;
			cod_x = tuAddr & 3;
			x0 = cu.x + cod_x*(1<<log2TrafoSize);
			y0 = cu.y + cod_y*(1<<log2TrafoSize);
			if(blkIdx>>1){
				y0+=(1<<(log2TrafoSize+2));
			}
			if(blkIdx&1){
				x0+=(1<<(log2TrafoSize+2));
			}
			break;
	}
	UChar xBase;
	UChar yBase;
	xBase = x0;
	yBase = y0;
	if(blkIdx>>1){
		yBase -= (1<<log2TrafoSize);
	}
	if(blkIdx&1){
		xBase -= (1<<log2TrafoSize);
	}

#ifndef __SYNTHESIS__
	std::cout << "===================== Transform Tree Depth " << (int)trafoDepth << " at " << (int)x0 <<" " << (int)y0 << "==========" << std::endl;
#endif

	TU_t tu;
	UInt symbolVal;
	tu.x = x0;
	tu.y = y0;
	tu.xBase = xBase;
	tu.yBase = yBase;
	tu.log2TrafoSize = log2TrafoSize;
	tu.blkIdx = blkIdx;
	tu.trafoDepth = trafoDepth;
	tu.lastGreater1Flag = 0;
	tu.lastGreater1Ctx=0;
	tu.greater1Ctx=0;
	tu.ctxSet=0;
	tu.cLastRiceParam=0;
	tu.cLastAbsLevel=0;
	tu.cAbsLevel=0;
	tu.G2ctxSet=0;

	cu.cbf_cb[1] = cu.cbf_cb[0];
	cu.cbf_cr[1] = cu.cbf_cr[0];
	cu.cbf_luma[1] = cu.cbf_luma[0];
	if(     (log2TrafoSize <= din.MaxTbLog2SizeY) &&
			(log2TrafoSize > din.MinTbLog2SizeY) &&
			(trafoDepth < cu.MaxTrafoDepth) &&
			(!(cu.IntraSplitFlag && (trafoDepth==0))))
	{
		parseSplitTransformFlag(log2TrafoSize, state, bStream, ctxTables, symbolVal);
		transform_split = symbolVal;
	} else {
		if((log2TrafoSize>din.MaxTbLog2SizeY)||(trafoDepth==0 && cu.IntraSplitFlag)){
			transform_split = 1;
		} else {
			transform_split = 0;
		}
	}

	if(log2TrafoSize>2){
		if(trafoDepth==0 || cu.cbf_cb[1]){
			parseCbfC(trafoDepth, state, bStream, ctxTables, symbolVal);
			cu.cbf_cb[0] = symbolVal;
		} else {
			cu.cbf_cb[0] = 0;
		}
		if(trafoDepth==0 || cu.cbf_cr[1]){
			parseCbfC(trafoDepth, state, bStream, ctxTables, symbolVal);
			cu.cbf_cr[0] = symbolVal;
		} else {
			cu.cbf_cr[0] = 0;
		}
	}
	if(!transform_split){
		parseCbfLuma(trafoDepth, state, bStream, ctxTables, symbolVal);
		cu.cbf_luma[0] = symbolVal;
		transform_unit(din, dout, dint,  cu,  tu,  state,  bStream,  ctxTables);
	}
}

void transform_tree(data_in_t& din, data_out_t& dout, internal_data_t& dint, CU_t& cu, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]){
	UChar trafoDepth;
	uint16_t tuIdx;
	bool end_of_tt;
	bool split_flag;
	trafoDepth=0;
	tuIdx=0;
	end_of_tt=0;

	while(!end_of_tt){
		transform_tree_rec( tuIdx,  trafoDepth,  cu.log2CbSize-trafoDepth, split_flag, din, dout, dint, cu, state, bStream, ctxTables);
		pattern_generator(tuIdx, trafoDepth, split_flag, end_of_tt);
	}
}



