// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 4  - ap_continue (Read/Write/SC)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        bit 5 - enable ap_local_deadlock interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (COR/TOW)
//        bit 1 - ap_ready (COR/TOW)
//        bit 5 - ap_local_deadlock (COR/TOW)
//        others - reserved
// 0x10 : Data signal of gCtx
//        bit 31~0 - gCtx[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of BaeState
//        bit 31~0 - BaeState[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of bst
//        bit 31~0 - bst[31:0] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of din
//        bit 31~0 - din[31:0] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of gCtx_r
//        bit 31~0 - gCtx_r[31:0] (Read/Write)
// 0x34 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XCABAC_TOP_CONTROL_ADDR_AP_CTRL       0x00
#define XCABAC_TOP_CONTROL_ADDR_GIE           0x04
#define XCABAC_TOP_CONTROL_ADDR_IER           0x08
#define XCABAC_TOP_CONTROL_ADDR_ISR           0x0c
#define XCABAC_TOP_CONTROL_ADDR_GCTX_DATA     0x10
#define XCABAC_TOP_CONTROL_BITS_GCTX_DATA     32
#define XCABAC_TOP_CONTROL_ADDR_BAESTATE_DATA 0x18
#define XCABAC_TOP_CONTROL_BITS_BAESTATE_DATA 32
#define XCABAC_TOP_CONTROL_ADDR_BST_DATA      0x20
#define XCABAC_TOP_CONTROL_BITS_BST_DATA      32
#define XCABAC_TOP_CONTROL_ADDR_DIN_DATA      0x28
#define XCABAC_TOP_CONTROL_BITS_DIN_DATA      32
#define XCABAC_TOP_CONTROL_ADDR_GCTX_R_DATA   0x30                  // Naming error, this address is for data_out_s
#define XCABAC_TOP_CONTROL_BITS_GCTX_R_DATA   32

