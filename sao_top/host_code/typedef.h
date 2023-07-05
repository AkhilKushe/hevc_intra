#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
#include <stdint.h>

// Data Types
typedef uint8_t UChar;
typedef uint32_t UInt;

// Defines
#define MAX_NUM_CTX_MOD             2       ///< maximum number of supported contexts

// Enumerations
enum SliceType
{
  B_SLICE               = 0,
  P_SLICE               = 1,
  I_SLICE               = 2,
  NUMBER_OF_SLICE_TYPES = 3
};

enum ChannelType
{
  CHANNEL_TYPE_LUMA    = 0,
  CHANNEL_TYPE_CHROMA  = 1,
  MAX_NUM_CHANNEL_TYPE = 2
};

enum ComponentID
{
  COMPONENT_Y       = 0,
  COMPONENT_Cb      = 1,
  COMPONENT_Cr      = 2,
  MAX_NUM_COMPONENT = 3
};

enum BAEMode
{
	REGULAR = 0,
	BYPASS = 1,
	TERMINATE = 3
};



typedef struct _bstream_t {
	UInt currIdx;
	UChar n_bits_held;
	UChar held_aligned_word;
} bstream_t;

typedef struct _arith_t {
	uint32_t ivlCurrRange;
	uint32_t ivlOffset;
	bstream_t bstate;
} arith_t;

typedef struct _NbCTU_t {
	//SAO
	UChar SaoTypeIdx[3];
	uint16_t SaoOffsetVal[3][5];
	UChar SaoEOClass[3];
	UChar sao_band_position[3];

	//quad_tree
	bool is_available;
}NbCTU_t;


typedef struct _s_header {
	//SAO
	bool slice_sao_luma_flag;
	bool slice_sao_chroma_flag;
	//CABAC
	SliceType slice_type;
	bool cabac_init_flag;
	int qp;
} s_header_t;

typedef struct _data_in {
	s_header_t s_header;
	NbCTU_t left_ctu;
	NbCTU_t up_ctu;
	uint32_t ctuAddrsRs;
	bool firstCTU;
}data_in_t;

typedef struct _data_out {
	//SAO
	UChar SaoTypeIdx[3];
	int16_t SaoOffsetVal[3][5];
	UChar SaoEOClass[3];
	UChar sao_band_position[3];

}data_out_t;

#endif
