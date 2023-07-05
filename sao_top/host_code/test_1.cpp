#include <iostream>
#include <stdint.h>
#include <fstream>
#include "typedef.h"
#include <cstring>

template <typename T, typename S>
void printArray(const char* name, int xdim, int ydim, T* arr){

	std::cout << name << " :" << std::endl;
	std::cout << std::dec;
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

int cabac_top( UChar globalCtx[MAX_NUM_CTX_MOD],  arith_t initArithState[1],  UChar bitStream[64],  data_in_t data_in_s[1],  data_out_t data_out_s[1]);

int main() {

	UChar globalCtx[MAX_NUM_CTX_MOD];
	UChar testBitStream_maxi[64];
	std::ifstream file("./sao_1.dat", std::ios::binary);
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

	// Setting input buffers
	// TODO : set all the input buffers
	data_in_t data_inp;
	data_out_t data_oup;
	data_inp.ctuAddrsRs = 0;
	data_inp.firstCTU = 1;
	data_inp.s_header.cabac_init_flag = 0;
	data_inp.s_header.slice_type = I_SLICE;
	data_inp.s_header.qp = 32;
	data_inp.s_header.slice_sao_chroma_flag = 1;
	data_inp.s_header.slice_sao_luma_flag = 1;
	data_inp.left_ctu.is_available = false;
	data_inp.up_ctu.is_available= false;

	arith_t initBaeState;

	data_in_t data_inp_s[1];
	data_out_t data_oup_s[1];
	arith_t initBaeState_s[1];

	data_inp_s[0] = data_inp;
	initBaeState_s[0] = initBaeState;


	cabac_top(globalCtx, initBaeState_s, testBitStream_maxi, data_inp_s, data_oup_s);

	data_oup = data_oup_s[0];

	//data_oup = data_oup_s.read();
	UChar expected[2] = {14, 33};


	std::cout << "TEST BENCH RESULTS" << std::endl;
	std::cout << "====================== Sao Buffer Output =========================" << std::endl;
	printArray<UChar, int>("SaoTypeIdx", 3, 1, data_oup.SaoTypeIdx);
	std::cout << "abc" << std::endl;
	printArray<UChar, int>("SaoEoClass", 3, 1, data_oup.SaoEOClass);
	std::cout << "abc" << std::endl;
	printArray<int16_t, int16_t>("SaoOffsetVal", 3, 5, (int16_t*)data_oup.SaoOffsetVal);
	std::cout << "abc" << std::endl;
	printArray<UChar, int>("sao_band_position", 3, 1, data_oup.sao_band_position);

	for(int i=0; i < 2; i++) {
		std::cout << (int)globalCtx[i] << std::endl;
		if(expected[i] != globalCtx[i]){
			return 1;
		}
	}
	std::cout << "Single CTU test Passed" << std::endl;
	return 0;
}
