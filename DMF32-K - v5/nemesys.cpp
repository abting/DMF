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
typedef int(__stdcall *NCS_Calib)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_Empty)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_Refill)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_FlowRate)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_isDoseF)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_isCalibF)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_gActiveFlowUnit)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_gActiveVolumeUnit)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_sActiveFlowUnit)(ncs_hdl, unsigned char, unsigned char);
typedef int(__stdcall *NCS_sActiveVolumeUnit)(ncs_hdl, unsigned char, unsigned char);
typedef int(__stdcall *NCS_gActualFlowRate)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_gActualSyringeLevel)(ncs_hdl, unsigned char, double*);
typedef int(__stdcall *NCS_stopUnit)(ncs_hdl, unsigned char);
typedef int(__stdcall *NCS_stopAllUnits)(ncs_hdl);
typedef int(__stdcall *NCS_gSyringeLevels)(ncs_hdl, unsigned char, double*, double*);



int NemeSYSdelay = 500; //DELAY AFTER OPENING AND BEFORE CLOSING

HINSTANCE hGetProcIDDLL;

//===========================================================================
//                             STATIC DATA
//===========================================================================
static ncs_hdl hDev = 0; // stores device handle

Nemesys::Nemesys()
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
    if (!open)
    {
        cout << "Failure to load OpenNemesysDev function";
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
        cout << " OpenNemesysDev Error: " << ErrCode;
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
    if (!close)
    {
         cout << "Failure to load CloseNemesysDev function";
    }
    if (hDev)
    {
        ErrCode = close(hDev);
        if (ERR_NOERR != ErrCode)
        {
            HandleError(ErrCode);
            cout << " CloseNemesysDev Error: " << ErrCode;
        }
    }
    return ErrCode;
}

//===========================================================================
// Function checks the actual flow rate and returns the value (double)
// Will be used to monitor the actual syringe level
//===========================================================================
double Nemesys::getActualFlowRate(unsigned char doseUnit) {
    NCS_gActualFlowRate getflowrate = (NCS_gActualFlowRate)GetProcAddress(hGetProcIDDLL, "NCS_GetFlowRateIs");
    if (!getflowrate)
    {
        cout << "Failure to load getActualFlowRate function";
    }
    double aFlowRate;
    long ErrCode = getflowrate(hDev, doseUnit, &aFlowRate);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << " getActual FlowRate Error: " << ErrCode << " on Dosing unit:" << doseUnit;
        return -1;
    }
    return aFlowRate;
}

//===========================================================================
// Function checks the actual flow rate and returns the value (double)
// Will be used to monitor the actual syringe level
//===========================================================================
double Nemesys::getActualSyringeLevel(unsigned char doseUnit) {
    NCS_gActualSyringeLevel getsyringelevel = (NCS_gActualSyringeLevel)GetProcAddress(hGetProcIDDLL, "NCS_GetSyringeLevelIs");
    if (!getsyringelevel)
    {
        cout << "Failure to load getSyringeLevel function";
    }
    double aSyringeLevel;
    long ErrCode = getsyringelevel(hDev, doseUnit, &aSyringeLevel);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << " getActual SyringeLevel Error: " << ErrCode << " on Dosing unit:" << doseUnit;
        return -1;
    }
    return aSyringeLevel;
}

//===========================================================================
// Function checks if the dosing unit is still DOSING
// Returns 0 if it is not finished
// Returns 1 if it is finished
// Returns <0 if there is an error
//===========================================================================
long Nemesys::CheckDosingStatus(unsigned char doseUnit) {

    NCS_isDoseF doseFin = (NCS_isDoseF)GetProcAddress(hGetProcIDDLL, "NCS_IsDosingFinished");
    if (!doseFin)
    {
        cout << "Failure to load CheckDosingStatus function";
    }
    long ErrCode = doseFin(hDev, doseUnit);
    cout << " Dosingstatus error: " << ErrCode;
    return ErrCode;
}

//===========================================================================
// Function checks if the dosing unit is still CALIBRATING
// Returns 0 if it is not finished
// Returns 1 if it is finished
// Returns <0 if there is an error
//===========================================================================
long Nemesys::CheckCalibrateStatus(unsigned char doseUnit) {

    NCS_isCalibF calibFin = (NCS_isCalibF)GetProcAddress(hGetProcIDDLL, "NCS_IsCalibrationFinished");
    if (!calibFin)
    {
        cout << "Failure to load CheckCalibrationStatus function";
    }
    long ErrCode = calibFin(hDev, doseUnit);
    cout <<" Calibration Status Error: " << ErrCode;
    return ErrCode;
}

//===========================================================================
//Doses a given volume at a given rate
//Proper use of the parameters is needs to be determined (i.e. how to aspirate or dispense; positive or negative)
//Units are set to the active flow unit
//===========================================================================

void Nemesys::DoseVolume(unsigned char doseUnit, double vol, double flo) {
    NCS_Dose dose = (NCS_Dose)GetProcAddress(hGetProcIDDLL, "NCS_DoseVolume");

    if (!dose)
    {
        cout << "Failure to load DOSE UNIT function";
    }
    long ErrCode = dose(hDev, doseUnit, &vol, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE DOSING ERROR: " << ErrCode;
    }
}

//===========================================================================
//Calibrates a given dosing unit
//===========================================================================

void Nemesys::CalibrateUnit(unsigned char doseUnit) {
    NCS_Calib calibUnit = (NCS_Calib)GetProcAddress(hGetProcIDDLL, "NCS_Calibrate");

    if (!calibUnit)
    {
        cout << "Failure to load CALIBRATE UNIT function";
    }
    long ErrCode = calibUnit(hDev, doseUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE CALIBRATE ERROR: " << ErrCode;
    }
}
//===========================================================================
//Empty the syringe at a given rate, pass a double which represents the flow rate
//Units are set to the active flow unit
//===========================================================================

void Nemesys::EmptySyringe(unsigned char doseUnit, double flo) {
    NCS_Empty empty = (NCS_Empty)GetProcAddress(hGetProcIDDLL, "NCS_EmptySyringe");
    if (!empty)
    {
        cout << "Failure to load empty function";
    }
    long ErrCode = empty(hDev, doseUnit, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE EMPTY SYRINGE ERROR: " << ErrCode;
    }
}

//===========================================================================
//Refill the syringe at a given rate, pass a double which represents the flow rate
//Units are set to the active flow unit
//===========================================================================
void Nemesys::RefillSyringe(unsigned char doseUnit, double flo) {
    NCS_Refill refill = (NCS_Refill)GetProcAddress(hGetProcIDDLL, "NCS_RefillSyringe");
    if (!refill)
    {
        cout << "Failure to load refill function";
    }
    long ErrCode = refill(hDev, doseUnit, &flo);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE REFILL SYRINGE ERROR: " << ErrCode;
    }
}

//===========================================================================
//Passes a pointer to a double, changes the value of that pointer to the max flow rate and returns that value
//===========================================================================
double getMaxFlow(unsigned char dosUnit, double* MaxFlow) {
    NCS_FlowRate flow = (NCS_FlowRate)GetProcAddress(hGetProcIDDLL, "NCS_GetFlowRateMax");
    if (!flow)
    {
        cout << "Failure to load MAXFLOW function";
    }
    long ErrCode = flow(hDev, dosUnit, MaxFlow);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE getMAXFLOW ERROR: " << ErrCode;
    }
    return *MaxFlow;
}

//===========================================================================
//Returns the active FLOW unit Identifier, IDs will be determined shortly
//===========================================================================

double getActiveFlowUnit(unsigned char doseUnit) {
    NCS_gActiveFlowUnit getflowunit = (NCS_gActiveFlowUnit)GetProcAddress(hGetProcIDDLL, "NCS_GetActiveFlowUnit");
    if (!getflowunit)
    {
        cout << "Failure to load GETACTIVEFLOWUNIT function";
        return -1;
    }
    long ErrCode = getflowunit(hDev, doseUnit);
    if (ErrCode <0)
    {
        HandleError(ErrCode);
        cout << "INSIDE GETACTIVEFLOWUNIT ERROR: " << ErrCode;
    }
    return ErrCode;
}
//===========================================================================
//Returns the active VOLUME unit Identifier, IDs will be determined shortly
//===========================================================================

double getActiveVolumeUnit(unsigned char doseUnit) {
    NCS_gActiveVolumeUnit getvolumeunit = (NCS_gActiveVolumeUnit)GetProcAddress(hGetProcIDDLL, "NCS_GetActiveVolumeUnit");
    if (!getvolumeunit)
    {
        cout << "Failure to load GETACTIVEVOLUMEUNIT function";
        return -1;
    }
    long ErrCode = getvolumeunit(hDev, doseUnit);
    if (ErrCode <0) {
        HandleError(ErrCode);
        cout << "INSIDE ETACTIVEVOLUMEUNIT ERROR: " << ErrCode;
    }
    return ErrCode;
}

//===========================================================================
//Sets the active FLOW units, by using the unit Identifier
//Once the active FLOW unit is set, it remains active even after you close your connection
//===========================================================================

void Nemesys::setActiveFlowUnit(unsigned char doseUnit, unsigned char fUnit) {

    NCS_sActiveFlowUnit setflowunit = (NCS_sActiveFlowUnit)GetProcAddress(hGetProcIDDLL, "NCS_SetActiveFlowUnit");
    if (!setflowunit)
    {
        cout << "Failure to load SETACTIVEFLOWUNIT function";
    }
    long ErrCode = setflowunit(hDev, doseUnit, fUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE SETACTIVEFLOWUNIT ERROR: " << ErrCode;
    }
}

//===========================================================================
//Sets the active VOLUME units, by using the unit Identifier
//Once the active VOLUME unit is set, it remains active even after you close your connection
//===========================================================================

void Nemesys::setActiveVolumeUnit(unsigned char doseUnit, unsigned char vUnit) {

    NCS_sActiveVolumeUnit setvolumeunit = (NCS_sActiveVolumeUnit)GetProcAddress(hGetProcIDDLL, "NCS_SetActiveVolumeUnit");
    if (!setvolumeunit) {
        cout << "Failure to load SETACTIVEVOLUMEUNIT function";
    }
    long ErrCode = setvolumeunit(hDev, doseUnit, vUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE SETACTIVEVOLUMEUNIT ERROR: " << ErrCode;
    }
  }
//===========================================================================
//Stops a dosing unit from dosing
//Can be executed during dosing or calibration(avoid calibration if possible)
//===========================================================================
void Nemesys::StopUnit(unsigned char doseUnit){
    NCS_stopUnit stopunit = (NCS_stopUnit)GetProcAddress(hGetProcIDDLL, "NCS_Stop");
    if (!stopunit) {
        cout << "Failure to load StopUnit function";
    }
    long ErrCode = stopunit(hDev, doseUnit);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE StopUnit ERROR: " << ErrCode;
    }
}
//===========================================================================
//Stops ALL dosing units from dosing
//Can be executed during dosing or calibration(avoid calibration if possible)
//===========================================================================
void StopAllUnits(){
    NCS_stopAllUnits stopallunits = (NCS_stopAllUnits)GetProcAddress(hGetProcIDDLL, "NCS_StopAllUnits");
    if (!stopallunits) {
        cout << "Failure to load StopALLUnits function";
    }
    long ErrCode = stopallunits(hDev);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE StopALLUnits ERROR: " << ErrCode;
    }
}

//===========================================================================
//Stops ALL dosing units from dosing
//Can be executed during dosing or calibration(avoid calibration if possible)
//NEEDS WORK ON; Not producing the correct units yet.
//===========================================================================
void Nemesys::getSyringeLevels(unsigned char doseUnit, double minSL, double maxSL){
    NCS_gSyringeLevels getsyringelevels = (NCS_gSyringeLevels)GetProcAddress(hGetProcIDDLL, "NCS_GetSyringeLimits");
    if (!getsyringelevels) {
        cout << "Failure to load getSyringeLevels function";
    }
    long ErrCode = getsyringelevels(hDev, doseUnit, &minSL, &maxSL);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE getSyringeLevels ERROR: " << ErrCode;
    }
}

//===========================================================================
//Stops ALL dosing units from dosing
//Can be executed during dosing or calibration(avoid calibration if possible)
//NEEDS WORK ON; Not producing the correct units yet.
//===========================================================================
/*void Nemesys::getSyringeLevels(unsigned char doseUnit, double minSL, double maxSL){
    NCS_gSyringeLevels getsyringelevels = (NCS_gSyringeLevels)GetProcAddress(hGetProcIDDLL, "NCS_GetSyringeLimits");
    if (!getsyringelevels) {
        cout << "Failure to load getSyringeLevels function";
    }
    long ErrCode = getsyringelevels(hDev, doseUnit, &minSL, &maxSL);
    if (ERR_NOERR != ErrCode)
    {
        HandleError(ErrCode);
        cout << "INSIDE getSyringeLevels ERROR: " << ErrCode;
    }
    double Slevels [2] = {minSL, maxSL};
    return Slevels;
}*/
//===========================================================================
//Opens the connection with the NemeSYS System
//Before doing so, the DLL must be loaded
//===========================================================================
void Nemesys::openConnection()
{
    hGetProcIDDLL = LoadLibraryA(("nemesys_dll"));

    if (hGetProcIDDLL == NULL) {
        cout << "cannot locate the .dll file";
    }
    else {
        cout << "DLL has successfully been loaded";
    }
    Sleep(NemeSYSdelay);
    InitNemesysDev();

}
//===========================================================================
//Closes the connection with the NemeSYS System
//===========================================================================
void Nemesys::closeConnection()
{
    Sleep(NemeSYSdelay);
    CloseNemesysDev();
}


//---------------------------------------------------------------------------
// EOF

