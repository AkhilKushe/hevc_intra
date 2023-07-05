#ifndef __DEBIN_H__
#define __DEBIN_H__
#include "typedef.h"


// SAO SE Structure
#define SAO_TYPE_IDX_CTX_ADDR	1
#define SAO_MERGE_FLAG_CTX_ADDR	0

void parseSAOTypeIdx(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parseSAOMergeFlag(arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD], UInt& symbolVal);
void parseSAOOffsetAbs(arith_t& state, UChar* bStream, UInt& symbolVal);
void parseSAOOffsetSign(arith_t& state, UChar* bStream, UInt& symbolVal);
void parseSAOEO(arith_t& state, UChar* bStream, UInt& symbolVal);
void parseSAOBO(arith_t& state, UChar* bStream, UInt& symbolVal);

bool componentID(int clIdx);

void sao_top(data_in_t& inp,data_out_t& out, arith_t& state, UChar* bStream, UChar ctxTables[MAX_NUM_CTX_MOD]);





#endif
