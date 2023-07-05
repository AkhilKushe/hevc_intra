#ifndef __INTRA_SE_H__
#define __INTRA_SE_H__
#include "arith_dec.h"
#include "typedef.h"

// Intra PU SE Structure
#define PART_MODE_CTX_ADDR	5
#define PREV_INTRA_LUMA_PRED_FLAG_CTX_ADDR	6
#define INTRA_CHROMA_PRED_MODE_CTX_ADDR	7

void parsePartMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parsePrevIntraLumaPredFlag(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parseMpmIdx(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parseRemIntraLumaPredMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parseIntraChromaPredMode(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);


void setIntraPredMode(UChar partIdx, UChar partSize, CU_t& cu ,data_in_t& din, data_out_t&dout, internal_data_t& dinternal);

#endif
