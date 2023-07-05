#include <iostream>
#include <stdint.h>
#include <hls_stream.h>
#include <fstream>
#include "typedef.h"


typedef uint8_t UChar;
typedef uint32_t UInt;

template <typename T, typename S>
void printArray(const char* name, int xdim, int ydim, T* arr){

	std::cout << name << " :" << std::endl;
	T val;
	bool flip=0;
	for(int x=0; x<xdim; x++){
		for(int y=0; y<ydim; y++){
			val = arr[x*ydim+y];
			std::cout << (S)val << " ";
			flip = !flip;
		}
		std::cout <<std::endl <<"----------------"<< std::endl;
	}
	std::cout << std::endl;
}
//void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD], volatile int8_t tranCoeff[64][64], volatile UChar bitStream[1024], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]);
void cabac_top(volatile UChar globalCtx[MAX_NUM_CTX_MOD],volatile arith_t initArithState[1], volatile int8_t tranCoeff_0[64][64], volatile int8_t tranCoeff_1[64][64], volatile int8_t tranCoeff_2[64][64], volatile char cqtDepth[64][64], volatile UChar IntraPredModeY[64][64], volatile UChar IntraPredModeC[64][64], volatile UChar bitStream[1024], volatile data_in_t data_in_s[1], volatile data_out_t data_out_s[1]);

int main() {
	UChar testBitStream_maxi[1024];
	std::ifstream file("F:\\petaProj\\cabac_hls\\cabac\\tb_src\\ctu_bitstream.dat", std::ios::binary);
	if(!file){
		std::cerr << "Could not openfile" << std::endl;
		return 1;
	}
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	memset(testBitStream_maxi, 0, sizeof(testBitStream_maxi));
	file.read((char*)testBitStream_maxi, fileSize);
	std::cout  << "File Size : " << fileSize << std::endl;

	UChar globalCtx[512];

	hls::stream<UInt, 32> bOut;

	// Setting input buffers
	data_in_t data_inp;
	//data_inp.ctuAddrsRs = 0;
	data_inp.firstCTU = 1;
	data_inp.s_header.cabac_init_flag = 0;
	data_inp.s_header.slice_type = I_SLICE;
	data_inp.s_header.qp = 32;
	data_inp.s_header.slice_sao_chroma_flag = 1;
	data_inp.s_header.slice_sao_luma_flag = 1;

	data_inp.left_ctu.is_available = false;
	data_inp.up_ctu.is_available= false;

	data_inp.PicWidthInCtbsY=20;
	data_inp.CtbLog2SizeY=6;
	data_inp.Log2MaxTransformSkipSize=2;
	data_inp.MinCbLog2SizeY=3;
	data_inp.MaxTbLog2SizeY=5;
	data_inp.MinTbLog2SizeY=2;

	data_inp.pps.log2_max_transform_skip_block_size_minus2=1;
	data_inp.pps.sign_data_hiding_enabled_flag=1;
	data_inp.pps.transform_skip_enabled_flag=1;
	data_inp.pps.transquant_bypass_enabled_flag=0;

	data_inp.sps.pic_width_in_luma_samples=1280;
	data_inp.sps.max_transform_hierarchy_depth_intra=2;


	//hls::stream<data_in_t> data_inp_s;
	//hls::stream<data_out_t> data_oup_s;
	data_in_t data_inp_s[1];
	data_out_t data_oup_s[1];

	data_inp_s[0] = data_inp;
	data_out_t data_oup;

	//hls::stream<int8_t> tranCoeff;
	int8_t tranCoeff_0[64][64];
	int8_t tranCoeff_1[64][64];
	int8_t tranCoeff_2[64][64];
	UChar IntraPredModeY[64][64];
	UChar IntraPredModeC[64][64];
	char cqtDepth[64][64];
	arith_t arith_s[1];
	arith_t initArithState;
	arith_s[0] = initArithState;

	cabac_top(globalCtx, arith_s, tranCoeff_0, tranCoeff_1, tranCoeff_2, cqtDepth, IntraPredModeY, IntraPredModeC, testBitStream_maxi, data_inp_s, data_oup_s);
	//data_oup = data_oup_s.read();
	data_oup = data_oup_s[0];

	UChar expected[5] = {14, 33, 2, 29, 49};

	std::cout << "TEST BENCH RESULTS" << std::endl;
	for(int i=0; i < MAX_NUM_CTX_MOD; i++) {
		std::cout << (int)globalCtx[i] << std::endl;

		//if(expected[i] != globalCtx[i]){
		//	return 1;
		//}
	}

	std::cout << "====================== Sao Buffer Output =========================" << std::endl;
	printArray<UChar, int>("SaoTypeIdx", 3, 1, data_oup.SaoTypeIdx);
	std::cout << "abc" << std::endl;
	printArray<UChar, int>("SaoEoClass", 3, 1, data_oup.SaoEOClass);
	std::cout << "abc" << std::endl;
	printArray<int16_t, int16_t>("SaoOffsetVal", 3, 5, (int16_t*)data_oup.SaoOffsetVal);
	std::cout << "abc" << std::endl;
	printArray<UChar, int>("sao_band_position", 3, 1, data_oup.sao_band_position);

	std::cout << "====================== Transform Output =========================" << std::endl;
	printArray<int8_t, int>("Transform Coefficients luma ", 64, 64, (int8_t*)tranCoeff_0);
	printArray<int8_t, int>("Transform Coefficients cr", 64, 64, (int8_t*)tranCoeff_1);
	printArray<int8_t, int>("Transform Coefficients cb", 64, 64, (int8_t*)tranCoeff_2);

	printArray<UChar, int>("Intra prediction luma ", 64, 64, (UChar*)IntraPredModeY);
	printArray<UChar, int>("Intra Prediction Chroma ", 64, 64, (UChar*)IntraPredModeC);

	printArray<char, int>("Coding quad Tree depth", 64, 64, (char*)cqtDepth);

	std::cout << "====================== BAE decoder state =========================" << std::endl;
	std::cout << "Current range : " << std::hex<< (int)arith_s[0].ivlCurrRange << std::endl;
	std::cout << "Current Offset : " << (int)arith_s[0].ivlOffset << std::endl;

	return 0;
}
