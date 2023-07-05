#ifndef __UTILS_H__
#define __UTILS_H__

#include "typedef.h"

template <typename T=int>
inline T max (const T a, const T b) {
	if(a > b) return a;
	return b;
}

template <typename T=int>
inline T min (const T a, const T b) {
	if(a < b) return a;
	return b;
}


template <typename T=int>
inline T Clip3 (const T minVal, const T maxVal, const T a) {
	return min<T>(max<T>(minVal, a) , maxVal);
}  ///< general min/max clip

#ifndef __SYNTHESIS__
	template <typename T, typename S>
	void printArray(const char* name, int xdim, int ydim, T* arr){
		std::cout << name << " :" << std::endl;
		T val;
		for(int x=0; x<xdim; x++){
			for(int y=0; y<ydim; y++){
				val = arr[x*ydim+y];
				std::cout << (S)val << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#endif

UChar _read_byte(volatile UChar* bStream, bstream_t& state);

UInt bitStream_read_bits(volatile UChar* bStream, UChar numBits, bstream_t& state);

void pattern_generator(uint16_t& cuIdx, UChar& depth, bool split_flag, bool& end_of_ctu);

void get_scanIdx(TU_t& tu, UChar log2TrafoSize, internal_data_t& dint, UChar cIdx, UChar& scanIdx);
void up_right_diagonal_scan(UChar blkSize, UChar diagScan[64][2]);
void horizontal_scan(UChar blkSize, UChar horScan[64][2]);
void vertical_scan(UChar blkSize, UChar verScan[64][2]);
void init_buffer_int(internal_data_t& dint);

#endif
