#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
#include <stdint.h>
#include <ap_int.h>

// Defines
#define MAX_NUM_CTX_MOD             137       ///< maximum number of supported contexts


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

// Data Types
typedef uint8_t UChar;
typedef uint32_t UInt;

typedef struct _bstream_t {
	UInt currIdx;
	UChar n_bits_held;
	UChar held_aligned_word;
} bstream_t;

typedef struct _arith_t {
	ap_uint<32> ivlCurrRange;
	ap_uint<32> ivlOffset;
	bstream_t bstate;
} arith_t;

typedef struct _NbCTU_t {
	char cqtDepth[64];				//Need only edge information, no bulk information required

	//SAO
	UChar SaoTypeIdx[3];

	UChar SaoEOClass[3];
	UChar sao_band_position[3];
	uint16_t SaoOffsetVal[3][5];

	//Intra PU
	UChar IntraPredModeY[64];
	UChar IntraPredModeC[64];

	//quad_tree
	bool is_available;
}NbCTU_t;

typedef struct _pps {
	// Coding unit
	bool transquant_bypass_enabled_flag;
	//residual_coding
	bool sign_data_hiding_enabled_flag;
	bool transform_skip_enabled_flag;
	//MACRO
	UChar log2_max_transform_skip_block_size_minus2;
}pps_t;

typedef struct _sps {
	//quad_tree
	uint32_t pic_width_in_luma_samples;
	uint32_t pic_height_in_luma_samples;
	//coding_unit
	UChar max_transform_hierarchy_depth_intra;

} sps_t;

typedef struct _s_header {
	//SAO
	int qp;
	bool slice_sao_luma_flag;
	bool slice_sao_chroma_flag;
	//CABAC
	SliceType slice_type;
	bool cabac_init_flag;

} s_header_t;

typedef struct _data_in {
	pps_t pps;
	sps_t sps;
	s_header_t s_header;
	NbCTU_t left_ctu;
	NbCTU_t up_ctu;
	//uint32_t ctuAddrsRs;
	bool firstCTU;

	//Taking as input

	UChar PicWidthInCtbsY;
	UChar CtbLog2SizeY;
	UChar Log2MaxTransformSkipSize;
	UChar MinCbLog2SizeY;
	UChar MaxTbLog2SizeY;
	UChar MinTbLog2SizeY;

}data_in_t;

typedef struct _data_out {
	//SAO
	UChar SaoTypeIdx[3];
	UChar SaoEOClass[3];
	UChar sao_band_position[3];
	int16_t SaoOffsetVal[3][5];

}data_out_t;


// Internal buffer
// TODO : currently added output and input buffer components to internal buffer to avoid inteface issue
typedef struct _internal_data {
	//Intra PU
	UChar IntraPredModeY[64][64];
	UChar IntraPredModeC[64][64];

	//Transform Unit
	int8_t TransCoeffLevel_0[64][64];
	int8_t TransCoeffLevel_1[64][64];
	int8_t TransCoeffLevel_2[64][64];

	char cqtDepth[64][64];
	bool split_transform_flag;

} internal_data_t;

#define PART_NxN 0
#define PART_2Nx2N 1

typedef struct _cu {
	uint32_t x;
	uint32_t y;
	UChar log2CbSize;
	UChar depth;

	//SE
	UChar mpm_idx[4];
	UChar rem_intra_luma_pred_mode[4];
	UChar intra_chroma_pred_mode;

	UChar MaxTrafoDepth; //(reshuffle for padding)

	bool prev_intra_luma_pred_flag[4];
	bool part_mode;
	bool cu_transquant_bypass_flag;

	//transform
	bool split_tranform_flag;
	bool transform_skip_flag;
	bool cbf_cb[2];
	bool cbf_cr[2];
	bool cbf_luma[2];

	//Others
	bool IntraSplitFlag;


}CU_t;

typedef struct _tu {
	uint32_t x;
	uint32_t y;
	uint32_t xBase;
	uint32_t yBase;

	UChar log2TrafoSize;
	UChar trafoDepth;
	UChar blkIdx;



	//feedback var
	UChar cLastRiceParam;
	uint32_t cLastAbsLevel;
	uint32_t cAbsLevel;

	UChar ctxSet;
	UChar lastGreater1Ctx;
	UChar lastGreater1Flag;
	UChar greater1Ctx;
	UChar G2ctxSet;

	//SE
	bool transform_skip_flag;
} TU_t;


#endif
