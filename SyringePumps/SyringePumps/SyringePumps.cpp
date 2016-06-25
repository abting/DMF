// SyringePumps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
//===========================================================================
//                               INCLUDES
//===========================================================================
#include "nemesys_api.h"
#include "err_codes.h"
using namespace std;
typedef int(__stdcall *NCS_Open)(unsigned char, ncs_hdl*);
typedef int(__stdcall *NCS_Close)(ncs_hdl);
typedef int(__stdcall *NCS_Errormsg)(long, char*, unsigned long);
typedef int(__stdcall *NCS_Dose)(ncs_hdl, unsigned char, double*, double*);
typedef int(__stdcall *NCS_Empty)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_FlowRate)(ncs_hdl, unsigned char, double*);

HINSTANCE hGetProcIDDLL;

//===========================================================================
//                             STATIC DATA
//===========================================================================
static ncs_hdl hDev = 0; // stores device handle
						 //===========================================================================
						 // Example error handling functions
						 //===========================================================================
void HandleError(long ErrorCode)
{
	//
	// The application could implement the error handling code here. I.e.
	// it could translate the error code into a human readable error message
	// string and display it. The following function call returns an error
	// message string in ErrMsg.
	//
	NCS_Errormsg error	= (NCS_Errormsg)GetProcAddress(hGetProcIDDLL, "NCS_GetErrorMsg");
	if (!error) {
		cout << "failure";
		
		
	}
	static char ErrMsg[64];
	error(ErrorCode, ErrMsg, sizeof(ErrMsg));

	//
	// Display error message now
	//
}
//===========================================================================
// neMESYS Initialisation
//===========================================================================
long InitNemesysDev(void)
{
	long ErrCode;
	
	
	NCS_Open open = (NCS_Open)GetProcAddress(hGetProcIDDLL, "NCS_OpenDevice");
	if (!open) {
		cout << "failure";
		return -1;
	}
	//
	// First we try to connect to device - this will execute a scan for 
	// connected dosing units and if everything is o.k. then we get a 
	// valid device handle returned.
	//  
	ErrCode = open(true, &hDev);
	if (ERR_NOERR != ErrCode)
	{
		HandleError(ErrCode);
	}

	return ErrCode;
}
//===========================================================================
// Closing connection
//===========================================================================
long CloseNemesysDev(void)
{
	long ErrCode;
	//
	// We only need to close the connection if we have a valid device handle
	// obtained from NCS_OpenDevice().
	//
	NCS_Close close = (NCS_Close)GetProcAddress(hGetProcIDDLL, "NCS_CloseDevice");
	if (!close) {
		cout << "failure";
		return -1;
	}
	if (hDev)
	{
		ErrCode = close(hDev);
		if (ERR_NOERR != ErrCode)
		{
			HandleError(ErrCode);
		}
	}
	return ErrCode;
}



int main()
{
	hGetProcIDDLL = LoadLibrary(_T("C:/Users/umroot/Documents/Visual Studio 2015/Projects/SyringePumps/Library/nemesys_dll.dll"));

	if (hGetProcIDDLL == NULL) {
		cout << "cannot locate the .dll file";
	}
	else {
		cout << "it has been called";
		//return -1;
	}
	
	NCS_Dose dose = (NCS_Dose)GetProcAddress(hGetProcIDDLL, "NCS_DoseVolume");
	
	if (!dose) {
		cout << "failure";
		return -1;
	}
	NCS_Empty empty = (NCS_Empty)GetProcAddress(hGetProcIDDLL, "NCS_EmptySyringe");
	if (!dose) {
		cout << "failure";
		return -1;
	}
	NCS_FlowRate flow = (NCS_FlowRate)GetProcAddress(hGetProcIDDLL, "NCS_GetFlowRateMax");
	if (!dose) {
		cout << "failure";
		return -1;
	}


	InitNemesysDev();
	Sleep(2000);
	double va[2] = { 100, NCS_FLOW_UNIT_UL_S };
	double fa[1];
	double* v = va;
	double* f = fa;
	long ErrCode = flow(hDev, 0, f);
	if (ERR_NOERR != ErrCode)
		{
			HandleError(ErrCode);
			cout << ErrCode;
			cout << 4;
		}
	double ffa [1] = { fa[0]};
	double *ff = ffa;
	double a = fa[0];
	double vol = 500;
	double flo = -100;
	cout << fa[0];

	ErrCode = dose(hDev, 0, &vol, &flo);
	if (ERR_NOERR != ErrCode)
	{
		HandleError(ErrCode);
		cout << ErrCode;
		cout << 4;
	}
	cout << ffa[0];
	//if (ERR_NOERR != ErrCode)
	//{
	//	HandleError(ErrCode);
	//}
	//Sleep(2000);
	//ErrCode = empty(hDev, '0', fa);
	//
	// First we try to connect to device - this will execute a scan for 
	// connected dosing units and if everything is o.k. then we get a 
	// valid device handle returned.
	//  
	
	

	//CloseNemesysDev();
	return 0;
}

//---------------------------------------------------------------------------
// EOF
