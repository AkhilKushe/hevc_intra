// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xcabac_top.h"

extern XCabac_top_Config XCabac_top_ConfigTable[];

XCabac_top_Config *XCabac_top_LookupConfig(u16 DeviceId) {
	XCabac_top_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XCABAC_TOP_NUM_INSTANCES; Index++) {
		if (XCabac_top_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XCabac_top_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XCabac_top_Initialize(XCabac_top *InstancePtr, u16 DeviceId) {
	XCabac_top_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XCabac_top_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XCabac_top_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

