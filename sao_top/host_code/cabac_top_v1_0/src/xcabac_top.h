// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XCABAC_TOP_H
#define XCABAC_TOP_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xcabac_top_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XCabac_top_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XCabac_top;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XCabac_top_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XCabac_top_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XCabac_top_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XCabac_top_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XCabac_top_Initialize(XCabac_top *InstancePtr, u16 DeviceId);
XCabac_top_Config* XCabac_top_LookupConfig(u16 DeviceId);
int XCabac_top_CfgInitialize(XCabac_top *InstancePtr, XCabac_top_Config *ConfigPtr);
#else
int XCabac_top_Initialize(XCabac_top *InstancePtr, const char* InstanceName);
int XCabac_top_Release(XCabac_top *InstancePtr);
#endif

void XCabac_top_Start(XCabac_top *InstancePtr);
u32 XCabac_top_IsDone(XCabac_top *InstancePtr);
u32 XCabac_top_IsIdle(XCabac_top *InstancePtr);
u32 XCabac_top_IsReady(XCabac_top *InstancePtr);
void XCabac_top_Continue(XCabac_top *InstancePtr);
void XCabac_top_EnableAutoRestart(XCabac_top *InstancePtr);
void XCabac_top_DisableAutoRestart(XCabac_top *InstancePtr);

void XCabac_top_Set_gCtx(XCabac_top *InstancePtr, u32 Data);
u32 XCabac_top_Get_gCtx(XCabac_top *InstancePtr);
void XCabac_top_Set_BaeState(XCabac_top *InstancePtr, u32 Data);
u32 XCabac_top_Get_BaeState(XCabac_top *InstancePtr);
void XCabac_top_Set_bst(XCabac_top *InstancePtr, u32 Data);
u32 XCabac_top_Get_bst(XCabac_top *InstancePtr);
void XCabac_top_Set_din(XCabac_top *InstancePtr, u32 Data);
u32 XCabac_top_Get_din(XCabac_top *InstancePtr);
void XCabac_top_Set_gCtx_r(XCabac_top *InstancePtr, u32 Data);
u32 XCabac_top_Get_gCtx_r(XCabac_top *InstancePtr);

void XCabac_top_InterruptGlobalEnable(XCabac_top *InstancePtr);
void XCabac_top_InterruptGlobalDisable(XCabac_top *InstancePtr);
void XCabac_top_InterruptEnable(XCabac_top *InstancePtr, u32 Mask);
void XCabac_top_InterruptDisable(XCabac_top *InstancePtr, u32 Mask);
void XCabac_top_InterruptClear(XCabac_top *InstancePtr, u32 Mask);
u32 XCabac_top_InterruptGetEnabled(XCabac_top *InstancePtr);
u32 XCabac_top_InterruptGetStatus(XCabac_top *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
