#include "nemesys.h"
#include <iostream>
using namespace std;

#include <QCoreApplication>
#include <QLibrary>
#include <QDebug>
// SyringePumps.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
#include <iostream>
//===========================================================================
//                               INCLUDES
//===========================================================================
#include "nemesys_api.h"
#include "err_codes.h"
using namespace std;

//Typedefs that set the standard convention for calling the NCS functions (dynamically loaded)
typedef int(__stdcall *NCS_Open)(unsigned char, ncs_hdl*);
typedef int(__stdcall *NCS_Close)(ncs_hdl);
typedef int(__stdcall *NCS_Errormsg)(long, char*, unsigned long);
typedef int(__stdcall *NCS_Dose)(ncs_hdl, unsigned char, double*, double*);
typedef int(__stdcall *NCS_Empty)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_Refill)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_FlowRate)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_isDoseF)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_gActiveFlowUnit)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_gActiveVolumeUnit)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_sActiveFlowUnit)(ncs_hdl, unsigned char, unsigned char);
typedef int(__stdcall *NCS_sActiveVolumeUnit)(ncs_hdl, unsigned char, unsigned char);
typedef int(__stdcall *NCS_gActualFlowRate)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_gActualSyringeLevel)(ncs_hdl, unsigned char, double*);

HINSTANCE hGetProcIDDLL;

//===========================================================================
//                             STATIC DATA
//===========================================================================
static ncs_hdl hDev = 0; // stores device handle

Nemesys::Nemesys()
{

}
void Nemesys::testing()
{

}

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
//===========================================================================
// Function checks if the actual flow rate abd returns the value (double)
// Displays the flow rate
//===========================================================================
void getActualFlowRate() {
    NCS_gActualFlowRate getflowrate = (NCS_gActualFlowRate)GetProcAddress(hGetProcIDDLL, "NCS_GetFlowRateIs");
    if (!getflowrate) {
        cout << "failure";
        //return -1;
    }
    double aFlowRate;
    long ErrCode = getflowrate(hDev, 0, &aFlowRate);
    if (ErrCode <0) {
        cout << "error";
        //return -1;
    }
    cout << "\nActual Flow Rate is: " << aFlowRate << "\n";
}
//===========================================================================
// Function checks if the actual syringe level and returns the value (double)
// Displays the syringe level
//===========================================================================
void getActualSyringeLevel() {
    NCS_gActualSyringeLevel getsyringelevel = (NCS_gActualSyringeLevel)GetProcAddress(hGetProcIDDLL, "NCS_GetSyringeLevelIs");
    if (!getsyringelevel) {
        cout << "failure";
        //return -1;
    }
    double aSyringeLevel;
    long ErrCode = getsyringelevel(hDev, 0, &aSyringeLevel);
    if (ErrCode <0) {
        cout << "error";
        //return -1;
    }
    cout << "\nActual SyringeLevel is: " << aSyringeLevel << "\n";
}
//===========================================================================
// Function checks if the dosing unit is still dosing, if it is, it remains in this loop until dosing is done
// At this point, you can move to the next function (command)
// Sleep is required before using this function
//===========================================================================

void CheckDosingStatus() {

    NCS_isDoseF doseFin = (NCS_isDoseF)GetProcAddress(hGetProcIDDLL, "NCS_IsDosingFinished");
    if (!doseFin) {
        cout << "failure";
    }
    long ErrCode = doseFin(hDev, 0);
    while (ErrCode != 1) {
        Sleep(500);
        ErrCode = doseFin(hDev, 0);
        cout << "Dosing is occurring";
        //getActualFlowRate();
        //getActualSyringeLevel();
    }
    cout << "out of loop \n";
    cout << ErrCode;

}

//===========================================================================
//Doses at a certain volume at a certain rate
//Proper use of the parameters is yet to be determined
//===========================================================================

void Nemesys::DoseVolume(double vol, double flo) {
    NCS_Dose dose = (NCS_Dose)GetProcAddress(hGetProcIDDLL, "NCS_DoseVolume");

    if (!dose) {
        cout << "failure";

    }
    long ErrCode = dose(hDev, 0, &vol, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        //cout << ErrCode;
        cout << "inside handle error";
    }
    Sleep(2000);
    CheckDosingStatus();
}

//===========================================================================
//Empty the syringe at a given rate, pass a double which represents the flow rate, the units are set to the active flow unit
//===========================================================================

void Nemesys::EmptySyringe(double flo) {
    NCS_Empty empty = (NCS_Empty)GetProcAddress(hGetProcIDDLL, "NCS_EmptySyringe");
    if (!empty) {
        cout << "failure";

    }
    long ErrCode = empty(hDev, 0, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        //cout << ErrCode;
        cout << "inside handle error";
    }
    Sleep(2000);
    CheckDosingStatus();
}

//===========================================================================
//Refill the syringe at a given rate, pass a double which represents the flow rate, the units are set to the active flow unit
//Remains to be tested, but should work in principle
//===========================================================================
void RefillSyringe(double flo) {
    NCS_Refill refill = (NCS_Refill)GetProcAddress(hGetProcIDDLL, "NCS_RefillSyringe");
    if (!refill) {
        cout << "failure";

    }
    long ErrCode = refill(hDev, 0, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        //cout << ErrCode;
        cout << "inside handle error";
    }
    Sleep(2000);
    CheckDosingStatus();
    cout << ErrCode;
}

//===========================================================================
//Passes a pointer to a double, changes the value of that pointer to the max flow rate and returns that value
//===========================================================================
double getMaxFlow(unsigned char dosUnit, double* MaxFlow) {

    NCS_FlowRate flow = (NCS_FlowRate)GetProcAddress(hGetProcIDDLL, "NCS_GetFlowRateMax");
    if (!flow) {
        cout << "failure";
        return -1;
    }
    long ErrCode = flow(hDev, dosUnit, MaxFlow);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << ErrCode;
        cout << 4;
    }

    return *MaxFlow;
}

//===========================================================================
//Returns the active FLOW unit Identifier, IDs will be determined shortly
//===========================================================================

double getActiveFlowUnit() {
    NCS_gActiveFlowUnit getflowunit = (NCS_gActiveFlowUnit)GetProcAddress(hGetProcIDDLL, "NCS_GetActiveFlowUnit");
    if (!getflowunit) {
        cout << "failure";
        return -1;
    }
    long ErrCode = getflowunit(hDev, 0);
    if (ErrCode <0){
        cout << "error";
        return -1;
    }
    return ErrCode;
}
//===========================================================================
//Returns the active VOLUME unit Identifier, IDs will be determined shortly
//===========================================================================

double getActiveVolumeUnit() {
    NCS_gActiveVolumeUnit getvolumeunit = (NCS_gActiveVolumeUnit)GetProcAddress(hGetProcIDDLL, "NCS_GetActiveVolumeUnit");
    if (!getvolumeunit) {
        cout << "failure";
        return -1;
    }
    long ErrCode = getvolumeunit(hDev, 0);
    if (ErrCode <0) {
        cout << "error";
        return -1;
    }
    return ErrCode;
}

//===========================================================================
//Sets the active FLOW units, by using the unit Identifier
//Once the active FLOW unit is set, it remains active even after you close your connection
//===========================================================================

void Nemesys::setActiveFlowUnit(unsigned char fUnit) {
    cout<<"SET ACTIVE FLOW UNIT";
    NCS_sActiveFlowUnit setflowunit = (NCS_sActiveFlowUnit)GetProcAddress(hGetProcIDDLL, "NCS_SetActiveFlowUnit");
    if (!setflowunit) {
        cout << "failure";
    }
    long ErrCode = setflowunit(hDev, 0, fUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        //cout << ErrCode;
        cout << "inside handle error";
    }
    cout << "if no handle error, success!";
}

//===========================================================================
//Sets the active VOLUME units, by using the unit Identifier
//Once the active VOLUME unit is set, it remains active even after you close your connection
//===========================================================================

void Nemesys::setActiveVolumeUnit(unsigned char vUnit) {
    cout<<"SET ACTIVE VOLUME UNIT"<<vUnit;
    NCS_sActiveVolumeUnit setvolumeunit = (NCS_sActiveVolumeUnit)GetProcAddress(hGetProcIDDLL, "NCS_SetActiveVolumeUnit");
    if (!setvolumeunit) {
        cout << "failure";
    }
    long ErrCode = setvolumeunit(hDev, 0, vUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        //cout << ErrCode;
        cout << "inside handle error";
    }
    cout << "if no handle error, success!";
}


void Nemesys::openConnection()
{
    //C:\Users\umroot\Desktop\build-dmf32-Desktop_Qt_5_7_0_MSVC2015_32bit-Debug\debug

    hGetProcIDDLL = LoadLibraryA(("nemesys_dll"));

    if (hGetProcIDDLL == NULL) {
        cout << "cannot locate the .dll file";
    }
    else {
        cout << "it has been called";
        //return -1;
    }

    cout << "\n Opening connection in 2 seconds";
    Sleep(2000);
    InitNemesysDev();

}

void Nemesys::closeConnection()
{
    Sleep(2000);

    CloseNemesysDev();
}

/*int main(int argc, char *argv[])
{
/*
    QLibrary myLib ("C:/Users/umroot/Documents/SyringePumps_1/nemesys_dll.dll");

    if(!myLib.load()){
        cout << "failure";
    };

    bool okLoad = myLib.load(); // check load DLL file successful or not
    bool ok = myLib.isLoaded();
    cout << okLoad;
    cout << ok;
    typedef int(__stdcall *NCS_Open)(unsigned char, ncs_hdl*);
    NCS_Open myFunction = (NCS_Open) myLib.resolve("NCS_OpenDevice");
    if (myFunction)
        cout <<"hello";
    long ErrCode = myFunction(true, &hDev);
    cout << ErrCode;


    typedef int(__stdcall *NCS_Open)(unsigned char, ncs_hdl*);
    NCS_Open myFunction = (NCS_Open) myLib.resolve("NCS_SetActiveVolumeUnit");
    if (myFunction)
        cout <<"hello";


    hGetProcIDDLL = LoadLibraryA(("C:/Users/umroot/Documents/SyringePumps_1/nemesys_dll.dll"));

    if (hGetProcIDDLL == NULL) {
        cout << "cannot locate the .dll file";
    }
    else {
        cout << "it has been called";
        //return -1;
    }

    cout << "\n Opening connection in 2 seconds";
    Sleep(2000);
    InitNemesysDev();

    Sleep(2000);

    //unsigned char dUnit = 0; //dosing unit
    //double MaxFlo;
    //getMaxFlow(dUnit, &MaxFlo);
    //cout << MaxFlo;


    //quickly outputs the active volume unit, then sets it and then displays new volume unit
    double unit;
    unit = getActiveVolumeUnit();
    cout << "\n";
    cout << unit;
    cout << "\n";
    unsigned char fUnit1 = 1;
    setActiveVolumeUnit(fUnit1);
    Sleep(3000);
    unit = getActiveVolumeUnit();
    cout << unit;

    double vol1 = 2000;
    double flo1 = -100;
    DoseVolume(vol1, flo1);
    Sleep(4000);
    CheckDosingStatus();

    double e = 100; // empty rate

    Sleep(4000);
    EmptySyringe(e);

    Sleep(2000);
    CheckDosingStatus();


    cout << "\n Closing connection in 2 seconds";
    Sleep(2000);

    CloseNemesysDev();


    return 0;
}
*/

//---------------------------------------------------------------------------
// EOF

