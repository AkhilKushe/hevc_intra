// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xcabac_top.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XCabac_top_CfgInitialize(XCabac_top *InstancePtr, XCabac_top_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XCabac_top_Start(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL) & 0x80;
    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XCabac_top_IsDone(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XCabac_top_IsIdle(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XCabac_top_IsReady(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XCabac_top_Continue(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL) & 0x80;
    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL, Data | 0x10);
}

void XCabac_top_EnableAutoRestart(XCabac_top *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XCabac_top_DisableAutoRestart(XCabac_top *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_AP_CTRL, 0);
}

void XCabac_top_Set_gCtx(XCabac_top *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GCTX_DATA, Data);
}

u32 XCabac_top_Get_gCtx(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GCTX_DATA);
    return Data;
}

void XCabac_top_Set_BaeState(XCabac_top *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_BAESTATE_DATA, Data);
}

u32 XCabac_top_Get_BaeState(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_BAESTATE_DATA);
    return Data;
}

void XCabac_top_Set_bst(XCabac_top *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_BST_DATA, Data);
}

u32 XCabac_top_Get_bst(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_BST_DATA);
    return Data;
}

void XCabac_top_Set_din(XCabac_top *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_DIN_DATA, Data);
}

u32 XCabac_top_Get_din(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_DIN_DATA);
    return Data;
}

void XCabac_top_Set_gCtx_r(XCabac_top *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GCTX_R_DATA, Data);
}

u32 XCabac_top_Get_gCtx_r(XCabac_top *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GCTX_R_DATA);
    return Data;
}

void XCabac_top_InterruptGlobalEnable(XCabac_top *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GIE, 1);
}

void XCabac_top_InterruptGlobalDisable(XCabac_top *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_GIE, 0);
}

void XCabac_top_InterruptEnable(XCabac_top *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_IER);
    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_IER, Register | Mask);
}

void XCabac_top_InterruptDisable(XCabac_top *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_IER);
    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_IER, Register & (~Mask));
}

void XCabac_top_InterruptClear(XCabac_top *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCabac_top_WriteReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_ISR, Mask);
}

u32 XCabac_top_InterruptGetEnabled(XCabac_top *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_IER);
}

u32 XCabac_top_InterruptGetStatus(XCabac_top *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCabac_top_ReadReg(InstancePtr->Control_BaseAddress, XCABAC_TOP_CONTROL_ADDR_ISR);
}

