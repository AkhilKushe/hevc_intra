#include "utils.h"

UChar _read_byte(volatile UChar* bStream, bstream_t& state){
	UChar retVal;
	retVal = bStream[state.currIdx];
	state.currIdx += 1;
	return retVal;
}

UInt bitStream_read_bits(volatile UChar* bStream, UChar numBits, bstream_t& state) {
	UInt retVal = 0;

	if(numBits <= state.n_bits_held) {
		retVal = state.held_aligned_word >> (state.n_bits_held - numBits);
		retVal &= ~(0xff << numBits);                           // Mask after reading
		state.n_bits_held -= numBits;
		return retVal;
	}



	numBits -= state.n_bits_held;
	retVal = state.held_aligned_word & ~(0xff << state.n_bits_held);
	retVal <<= numBits;

	UInt aligned_word = 0;
	UInt numBytes = (numBits -1) >> 3;

	switch (numBytes) {
		case 3:
			aligned_word = _read_byte(bStream, state) << 24;
		case 2:
			aligned_word |= _read_byte(bStream, state) << 16;
		case 1:
			aligned_word |= _read_byte(bStream, state) << 8;
		case 0:
			aligned_word |= _read_byte(bStream, state) ;
	}

	UInt next_n_held_bits = (32- numBits) % 8;

	retVal |= (aligned_word >> next_n_held_bits);
	state.n_bits_held = next_n_held_bits;
	state.held_aligned_word = aligned_word;

	return retVal;
}
