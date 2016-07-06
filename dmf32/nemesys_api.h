#ifndef __NEMESYS_API_H
#define __NEMESYS_API_H
//============================================================================
// Cetoni GmbH
// Gewerbegebiet Korbwiesen
// Am Wiesenring 6
// 07554 Korbussen
//
// (c) Copyright 2006 - 2014, cetoni GmbH
//
// All rights reserved. Cetoni's source code is an unpublished work
// and the use of a copyright notice does not imply otherwise. This
// source code contains confidential, trade secret material of
// Cetoni GmbH. Any attempt or participation in deciphering, decoding,
// reverse engineering or in any way altering the source code is
// strictly prohibited, unless the prior written consent Cetoni GmbH
// is obtained.
//
/// \file     nemesys_api.h
/// \author   Uwe Kindler (UK)
/// \version  2.07
/// \date     2012-04-16
/// \brief    API for programming neMESYS devices (NCS - NEMESYS COMMAND SET)
//============================================================================


//============================================================================
//                      ONLINE DOCUMENTATION for DOXYGEN
//
/**
 * \mainpage Overview
 * \tableofcontents
 * \section section_api_introduction Introduction
 * This documentation provides the instructions for the implemented functions.
 * The library is arranged in groups of functions and helps to simplify the
 * programming of the control software based on Windows. This document
 * describes the interface between a program and the Windows DLL
 * (Dynamic Link Library).
 *
 * THERE IS NO WARRANTY FOR THIS LIBRARY, TO THE EXTENT PERMITTED BY
 * APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
 * HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE
 * OF THIS LIBRARY IS WITH YOU. SHOULD THIS  LIBRARY PROVE DEFECTIVE OR
 * INSUFFICIENT, YOU ASSUME THE COST OF ALL ECESSARY SERVICING, REPAIR OR
 * CORRECTION.
 *
 * The latest edition of these documentation, additional documentation and
 * software to the neMESYS dosing system may also be found on the internet
 * under <a href="http://www.cetoni.de/en-service/downloads/manuals.html"
 * target="_blank">http://www.cetoni.de/en-service/downloads/manuals.html</a>
 *
 * ---
 *
 * \section section_general_information General Information
 * The library *nemesys_dll.dll* is an implementation of the neMESYS command
 * set for the communication between neMESYS and a personal computer. Using
 * this library is a simple way to develop your own application. This library
 * is running on each windows 32-bit operating system. You can include it into
 * different programming environments. All the neMESYS commands are implemented
 * and they can be called directly from your own program. You don't have to
 * care about the protocol details. The only thing you have to ensure is
 * properly connected and configured dosing platform. The library
 * *nemesys_dll.dll* offers the whole set of neMESYS commands. Executing
 * neMESYS commands, managing configuration parameters, executing dosing
 * tasks and handling low level communication with the neMESYS control units
 * is the business of this library.
 *
 * ---
 * \section section_directory_layout Installation Directory Content
 * The neMESYS SDK setup installs a number of files into the final installation
 * folder. The following overview shows which files are installed into which
 * folders:
 * - <b>Root Installation Folder</b> - contains the changelog and all other
 *     codumentation files
 *   - <b>Demo</b> - A simpole demo application that uses the nemesys_dll.dll
 *   - <b>Matlab Demo</b> - A demo integration into Matlab
 *   - <b>neMESYS LabVIEW Kit</b> - The neMESYS LabVIEW Kit with all VIs
 *   - <b>RAD Studio 2007 C++</b> - RAD studio C++ integration and import library
 *   - <b>RAD Studio 2007 Delphi</b> - RAD studio Delphi integration
 *   - <b>RS232 Library</b> - Source code of the neMESYS RS232 library
 * .
 *
 * ---
 * \section section_including Including the neMESYS SDK
 * The way how you include the library, depends on the compiler and on the
 * programming language you use. In order to have a correctly working
 * communication, you have to include the libraries to your programming
 * environment. You have to copy these files to the working directory of your
 * system. To open the library *nemesys_dl.dll* you have to use the function
 * NCS_OpenDevice(). You have to do this before you can execute any
 * neMESYS command. At the end of your program you have to call
 * NCS_CloseDevice() to properly close the connection and shutdown. Use the
 * calling convention <i>__stdcall</i> for this library. This convention is managing
 * how the parameters are put on the stack and who is responsible to clean the
 * stack after the function execution.
 *
 * ---
 * \section Parameter Storage
 * The neMESYS library needs to store several device parameters of connected
 * neMESYS dosing units persistent. The library will store these parameters
 * into a *devices.ini* file. The library will create or use the ini file
 * inside the directory <i>C:\\Users\\Public\\Documents\\nemesys</i>. The neMESYS DLL
 * shares this configuration file with the *neMESYS UserInterface* application.
 */
//============================================================================


/**
 * \page page_cbuilder_integrationm DLL Integration into C++ Builder 2007
 * \tableofcontents
 * You need the following files to include the library to the programming
 * environment of *Codegear C++ Builder*.
 * - **nemesys_api.h** - Constant definitions and declarations of the library functions
 * - **nemesys_dll.dll** - Dynamic Link Library
 * - **nemesys_dll.lib** - Import library (OMF Format)
 * .
 *
 * To include the listed files you have to do the following steps:
 * -# Copy the files to the working directory of the project
 * -# Write the instruction `#include "nemesys_api.h"` to your program
 *    to include  the constant definitions and the declarations of the library
 *    functions.
 * -# Add the file *nemesys_dll.lib* to the project. To do this, you have
 *    to open the menu item *Add to project...* of the *Project* menu. Add the
 *    file *nemesys_dll.lib*.
 */

 /**
 * \page page_mingw_integration DLL Integration into MinGW based projects
 * \tableofcontents
 * The MinGW ports of ld support the direct linking, including data symbols,
 * to a dll without the usage of any import libraries. This is much faster and
 * uses much less memory than does the traditional import library method,
 * expecially when linking large libraries or applications. Linking directly to
 * a dll uses no extra command-line switches other than -L and -l, because ld
 * already searches for a number of names to match each library. So you can link
 * directly to the DLL via <i>-lnemesys_dll</i> linker switch. To avoid compiler warnings
 * you need to pass the additional linker switch <i>--enable-stdcall-fixup</i>.
 */


//===========================================================================
//                                DEFINES
//===========================================================================
#ifdef __MINGW32__
#define NCS_Func extern "C"
#elif NEMESYS_DLL
#define NCS_Func extern "C" __declspec (dllexport)
#else
#define NCS_Func extern "C" __declspec (dllimport)
#endif



//===========================================================================
//                                  DATA TYPES
//===========================================================================
///
/// Handle type for passing device handles to and from a function
///
typedef unsigned long ncs_hdl;


//---------------------------------------------------------------------------
/// Volume unit identifiers.
/// This enumeration defines the identifiers for all supported volume
/// units. (i.e. for  NCS_SetActiveVolumeUnit() and  NCS_GetActiveVolumeUnit)
///
typedef enum eVolUnitId
{
    NCS_VOL_UNIT_NL = 0, ///< nanolitres
    NCS_VOL_UNIT_UL,     ///< microlitres
    NCS_VOL_UNIT_ML,     ///< millilitres
    NCS_VOL_UNIT_L,      ///< litres
    NCS_VOL_UNIT_MM      ///< millimetres - this unit is independent from syringe configuration
} TVolUnitId;


//---------------------------------------------------------------------------
/// Flow unit identifiers.
/// This enumeration defines the identifiers for all supported flow rate
/// units. Whenever you call a function that requires or returns a flow value
/// you need these identifiers to configure the type of flow rate unit.
///
typedef enum eFlowUnitId
{
    NCS_FLOW_UNIT_NL_S = 0, ///< nanolitres per second
    NCS_FLOW_UNIT_NL_MIN,   ///< nanolitres per minute
    NCS_FLOW_UNIT_UL_S,     ///< microlitres per second
    NCS_FLOW_UNIT_UL_MIN,   ///< microliters per minute
    NCS_FLOW_UNIT_UL_H,     ///< microlitres per hour
    NCS_FLOW_UNIT_ML_MIN,   ///< millilitres per minute
    NCS_FLOW_UNIT_ML_H,     ///< millilitres per hour
    NCS_FLOW_UNIT_MM_S,     ///< millimetres per second - this unit is independent from syringe congiguration
} TFlowUnitId;


//---------------------------------------------------------------------------
/// Valve action identifiers.
/// These identifiers specify the valve action for single flow profile
/// entries. At the start of each flow profile entry a certain valve
/// action can be triggered.
///
typedef enum eValveActions
{
    NCS_VALVE_ACTION_OUT  = 0x00, ///< switch valve to output (dispense)
    NCS_VALVE_ACTION_IN   = 0x01, ///< switch valve to input (aspirate)
    NCS_VALVE_ACTION_NONE = 0xFF  ///< do not change the state of the valve
} TValveAction;


//===========================================================================
//                               INITIALISATION
//
/// \defgroup nem_init Initialisation
/// Initialize communication to device.
/// This group defines all required functions to initialize a correct
/// communication to the device.
/// \see Example neMESYS_Init.cpp
/// \{
/// \example neMESYS_Init.cpp This example shows the initialisation and
/// shutdown procedure for neMESYS API. There are only two functions that
/// need to be called and a proper error handling should be implemented.
//===========================================================================

//---------------------------------------------------------------------------
/// Opens a connection for sending and receiving data.
/// \param[in] bShowStatusWnd If true it displays a status window during
///                           initialisation search for dosing units.
/// \param[out] phDevice      If the error code is ERR_NOERR then this parameter
///                           returns a valid device handle for device access
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_OpenDevice(unsigned char bShowStatusWnd,
                                       ncs_hdl      *phDevice);


//---------------------------------------------------------------------------
/// Closes connection and frees all resources.
/// \param[in] hDevice Valid device handle
/// \return Error code
//
NCS_Func  long   __stdcall NCS_CloseDevice(ncs_hdl hDevice);
/// \} nem_init


//===========================================================================
//                                  DOSING
//
/// \defgroup nem_dosing Dosing
/// Functions for various dosing tasks.
/// This group defines all required functions for execution of different
/// dosing tasks like dosing a certain volume or generating a continuous
/// flow.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Doses a defined volume with a defined flow rate.
/// It is possible to dispense and aspirate using this function. To dispense
/// a certain volume, use positive flow rate values. To aspirate a
/// certain volume use negative flow rate values.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pVolume  Points to buffer with volume value to dose. The
///                         volume value has to be in active volume unit
///                         format. On return the this buffer contains the
///                         realizable volume value.
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format. On  return the this buffer contains the
///                         realizable volume value. Negative values =
///                         aspirate, positive values = dispense
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_DoseVolume(ncs_hdl       hDevice,
                                          unsigned char DosingUnit,
                                          double       *pVolume,
                                          double       *pFlowRate);


//---------------------------------------------------------------------------
/// Moves the pusher until syringe content reaches a certain level.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pVolume   Points to buffer with volume value. The value defines
///                         the volume of reagent in the syringe. The volume value
///                         has to be in active volume unit format. On return the
///                         this buffer contains the realizable volume value.
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format. On  return the this buffer contains the
///                         realizable volume value.
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_SetSyringeLevel(ncs_hdl       hDevice,
                                               unsigned char DosingUnit,
                                               double       *pVolume,
                                               double       *pFlowRate);


//---------------------------------------------------------------------------
/// Generate define fluid stream until dosing unit reaches its limits.
/// You can dispense by providing positive flow rate values and aspirate by
/// providing negative flow rate values.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_GenerateFlow(ncs_hdl       hDevice,
                                            unsigned char DosingUnit,
                                            double       *pFlowRate);


//---------------------------------------------------------------------------
/// Stop dosing of single unit
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_Stop(ncs_hdl  hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Eympties syringe by moving pusher to lower limit of dosing unit.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_EmptySyringe(ncs_hdl       hDevice,
                                            unsigned char DosingUnit,
                                            double       *pFlowRate);


//---------------------------------------------------------------------------
/// Refills syringe by moving pusher to upper limit of dosing unit.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_RefillSyringe(ncs_hdl       hDevice,
                                             unsigned char DosingUnit,
                                             double       *pFlowRate);


//---------------------------------------------------------------------------
/// Stops movement of all dosing units
/// \param[in] hDevice      Valid device handle
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_StopAllUnits(ncs_hdl hDevice);


//---------------------------------------------------------------------------
/// Performs quick stop for all dosing units.
/// Use this function only if you have a serious error or problem.
/// \param[in] hDevice      Valid device handle
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_EmergencyStopAllUnits(ncs_hdl hDevice);


//---------------------------------------------------------------------------
/// Doses a defined volume with a defined flow rate.
/// Extended version of the NCS_DoseVolume() with parameters for volume
/// and flow rate units.
/// It is possible to dispense and aspirate using this function. To dispense
/// a certain volume, use positive flow rate values. To aspirate a
/// certain volume use negative flow rate values.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pVolume  Points to buffer with volume value to dose.
///                         On return the this buffer contains the
///                         realizable volume value.
/// \param[in] VolUnit      Valid volume unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. On
///                          return the this buffer contains the
///                          realizable volume value. Negative values =
///                          aspirate, positive values = dispense
/// \param[in] FlowUnit     Valid flow unit
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_DoseVolumeEx(ncs_hdl        hDevice,
                                            unsigned char  DosingUnit,
                                            double        *pVolume,
                                            TVolUnitId     VolUnit,
                                            double        *pFlowRate,
                                            TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Moves the pusher until syringe content reaches a certain level.
/// Extended version of the NCS_SetSyringeLevel() with parameters for volume
/// and flow rate units.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pVolume  Points to buffer with volume value to dose.
///                         On return the this buffer contains the
///                         realizable volume value.
/// \param[in] VolUnit      Valid volume unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. On
///                          return the this buffer contains the
///                          realizable volume value.
/// \param[in] FlowUnit      Valid flow unit
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_SetSyringeLevelEx(ncs_hdl        hDevice,
                                                 unsigned char  DosingUnit,
                                                 double        *pVolume,
                                                 TVolUnitId     VolUnit,
                                                 double        *pFlowRate,
                                                 TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Generate define fluid stream until dosing unit reaches its limits.
/// Extended version of the NCS_GenerateFlow() with parameters for volume
/// and flow rate units. You can dispense by providing positive flow rate
/// values and aspirate by providing negative flow rate values.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \param[in] FlowUnit Flow unit for value in pFlowRate
/// \retval ERR_NOERR indicates success
/// \retval -ERR_PERM Movement not permitted because target position is
///                   outside of syringe or device limits
///
NCS_Func  long   __stdcall NCS_GenerateFlowEx(ncs_hdl         hDevice,
                                              unsigned char  DosingUnit,
                                              double        *pFlowRate,
                                              TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Eympties syringe by moving pusher to lower limit of dosing unit.
/// Extended version of the NCS_EmptySyringe() with parameters for flow
/// rate units.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \param[in] FlowUnit Flow unit for value in pFlowRate
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_EmptySyringeEx(ncs_hdl        hDevice,
                                              unsigned char  DosingUnit,
                                              double        *pFlowRate,
                                              TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Refills syringe by moving pusher to upper limit of dosing unit.
/// Extended version of the NCS_RefillSyrine() with parameters for flow
/// rate units.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \param[in,out] pFlowRate Points to buffer with flow rate value. The
///                         flow rate value has to be in active flow rate unit
///                         format.
/// \param[in] FlowUnit Flow unit for value in pFlowRate
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_RefillSyringeEx(ncs_hdl         hDevice,
                                               unsigned char  DosingUnit,
                                               double        *pFlowRate,
                                               TFlowUnitId    FlowUnit);

/// \} nem_dosing



//===========================================================================
//                                DEVICE STATE
//
/// \defgroup nem_devstate Device State
/// Query device state.
/// This group defines all required functions for reading and writing
/// device state specific parameters.
/// Each neMESYS pump maintains an internal state machine. The devic states
/// are
///  - fault state
///  - disabled
///  - operational
///
/// A pump can execute dosing command only in device state operational. If
/// a device error occures, i.e. if the device reaches a limit sensor, then
/// the device goes from operational state into fault state. To check if
/// the device is in fault state you can call the function
/// NCS_IsInFaultState(). To leave the fault state, you can call the function
/// NCS_ClearFault().
///
/// \note
/// If NCS_ClearFault() does not clear the fault state, then
/// there might be a serious failure in device control electronics and you
/// should call the technical support.
///
/// After the NCS_ClearFault() call, the pump should be in state disabled.
/// That means, the error condition was resolved properly but the device is
/// still not ready to process dosing commands. To set the device operational
/// again, you should call NCS_SetOperational(). You can the call the
/// function  NCS_IsOperational() to check if the device is no in operational
/// state.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Check if dosing unit is in fault state.
/// If dosing unit is in fault state then you can try to clear the fault
/// state by calling NCS_ClearFault()
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
///
/// \retval 0  Dosing is not in fault state
/// \retval 1  Dosing unit is in fault state
/// \retval <0 Error code
///
NCS_Func  long  __stdcall NCS_IsInFaultState(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Check if dosing unit is operational.
/// If a dosing unit is not in fault state but it is not operational then you
/// should call the function NCS_SetOperational() to set the unit into
/// an operational state.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
///
/// \retval 0  Dosing unit is not operational
/// \retval 1  Dosing unit is operational
/// \retval <0 Error code
///
NCS_Func  long  __stdcall NCS_IsOperational(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Clears fault state of single dosing unit.
/// If a device error occures you need to call this function to set the dosing
/// unit into an error free state.  If a serious error occured then the
/// device will be still in fault state after a call to this function.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_ClearFault(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Sets dosing unit into an operational state and apply voltage to pump drive.
/// Dosing is possible only in operational state. If a device error occured
/// you first need to call NCS_ClearFault() and then you need to call this
/// function to set a single dosing unit into an operational state.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit to set operational
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_SetOperational(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Sets dosing unit into an disabled state.
/// Disable dosing unit and turn voltage of drive unit off.
/// No dosing is possible in this state.
/// \param[in] hDevice      Valid device handle
/// \param[in] DosingUnit   Number of dosing unit to set disabled
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long   __stdcall NCS_SetDisabled(ncs_hdl hDevice, unsigned char DosingUnit);

/// \} nem_devstate




//===========================================================================
//                           CONFIGURATION
//
/// \defgroup nem_config Configuration
/// Configurate dosing unit parameters.
/// This group defines all required functions for reading and writing
/// configuration parameters of the dosing system an single dosing unts.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Read syringe parameters.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to query
/// \param[out] pInnerDiameter_mm Inner diameter of the syringe tube in
///                               millimetres.
/// \param[out] pMaxPistonStroke_mm The maximum piston stroke defines the
///                                 maximum position the piston can be moved
///                                 to before it slips out of the syringe tube.
///                                 The maximum piston stroke limits the maximum
///                                 travel range of the neMESYS pusher.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetSyringeParam(ncs_hdl        hDevice,
                                              unsigned char  DosingUnit,
                                              double        *pInnerDiameter_mm,
                                              double        *pMaxPistonStroke_mm);


//---------------------------------------------------------------------------
/// Set syringe parameters.
/// If you change the syringe in one device, you need to setup the new
/// syringe parameters to get proper conversion of flow rate und volume
/// units.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to configure
/// \param[in]  InnerDiameter_mm  Inner diameter of the syringe tube in
///                               millimetres.
/// \param[in]  MaxPistonStroke_mm  The maximum piston stroke defines the
///                                 maximum position the piston can be moved
///                                 to before it slips out of the syringe tube.
///                                 The maximum piston stroke limits the maximum
///                                 travel range of the neMESYS pusher.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_SetSyringeParam(ncs_hdl       hDevice,
                                              unsigned char DosingUnit,
                                              double        InnerDiameter_mm,
                                              double        MaxPistonStroke_mm);



//---------------------------------------------------------------------------
/// Set syringe limits.
/// The maximum travel range of the pusher is limited by the length of the
/// linear drive and by the maxmimum piston stroke of the syringe.
/// With this function it is possible to apply syringe limits that may
/// limit the travel range even further. The drive unit stops as soon as it
/// reaches a limit.
/// \param[in]  hDevice     Valid device handle
/// \param[in]  DosingUnit  Number of dosing unit to configure
/// \param[in]  LimitMin_mm  The minimum limit to apply. This prevents the
///                          drive unit from traveling to the zero position.
///                          Apply a value of 0 to set the default value.
/// \param[in]  LimitMax_mm  The maximum limit to apply. This prevents the
///                          drive unit from traveling the complete syringe
///                          piston stroke. To set it to the maximum you should
///                          set it to a value > 80 mm because this is more then
///                          the physical linear drive range.
/// \return Error code - ERR_NOERR indicates success
/// \see NCS_GetSyringeLimits()
///
NCS_Func long __stdcall NCS_SetSyringeLimits(ncs_hdl       hDevice,
                                             unsigned char DosingUnit,
                                             double        LimitMin_mm,
                                             double        LimitMax_mm);



//---------------------------------------------------------------------------
/// Returns syringe limits.
/// \param[in]  hDevice     Valid device handle
/// \param[in]  DosingUnit  Number of dosing unit to configure
/// \param[in]  pLimitMin_mm Returns the minimum limit.
/// \param[in]  pLimitMax_mm Returns the maximum limit.
/// \return Error code - ERR_NOERR indicates success
/// \see NCS_SetSyringeLimits()
///
NCS_Func long __stdcall NCS_GetSyringeLimits(ncs_hdl       hDevice,
                                             unsigned char DosingUnit,
                                             double       *pLimitMin_mm,
                                             double       *pLimitMax_mm);


//---------------------------------------------------------------------------
/// Get maximum flow rate that is realizable with current dosing unit configuration.
/// The maximum flow rate depends on the mechanical configuration of the
/// dosing unit (gear) and on the syringe configuration. If larger syringes
/// are used then larger flow rates are realizable.
/// \param[in]  hDevice        Valid device handle
/// \param[in]  DosingUnit     Number of dosing unit to query
/// \param[out] pFlowRateMax   Maximum flow rate.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowRateMax(ncs_hdl       hDevice,
                                             unsigned char DosingUnit,
                                             double       *pFlowRateMax);


//---------------------------------------------------------------------------
/// Get maximum syringe fill level for a single dosing unit.
/// Extended version of function NCS_GetSyringeLevelMax(). The volume unit
/// for the syringle level is configurable here.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to query
/// \param[out] pSyringeLevelMax  Maximum syringe fill level
/// \param[in]  VolUnit           Volume unit for value returned
///                               in pSyringeLevelMax
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelMaxEx(ncs_hdl         hDevice,
                                                   unsigned char   DosingUnit,
                                                   double         *pSyringeLevelMax,
                                                   TVolUnitId      VolUnit);

//---------------------------------------------------------------------------
/// Get minimum syringe fill level for a single dosing unit.
/// Normally the minimum syringe fill level is 0. But the minimum level may
/// may be limited by the software position limit to a higher level. The
/// pusher only travels between the maximum and minimum syringe level.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to query
/// \param[out] pSyringeLevelMin  Maximum syringe fill level
/// \param[in]  VolUnit           Volume unit for value returned
///                               in pSyringeLevelMax
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelMinEx(ncs_hdl         hDevice,
                                                   unsigned char   DosingUnit,
                                                   double         *pSyringeLevelMin,
                                                   TVolUnitId      VolUnit);


//---------------------------------------------------------------------------
/// Get maximum flow rate that is realizable with current dosing unit configuration.
/// The maximum flow rate depends on the mechanical configuration of the
/// dosing unit (gear) and on the syringe configuration. If larger syringes
/// are used then larger flow rates are realizable.
/// Extended version of function NCS_GetFlowRateMax(). The flow unit
/// for the maximum flow rate is configurable here.
/// \param[in]  hDevice        Valid device handle
/// \param[in]  DosingUnit     Number of dosing unit to query
/// \param[out] pFlowRateMax   Maximum flow rate.
/// \param[in]  FlowUnit       Flow unit identifier for flow returned in
///                            pFlowrateMax
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowRateMaxEx(ncs_hdl         hDevice,
                                               unsigned char  DosingUnit,
                                               double        *pFlowRateMax,
                                               TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Get maximum syringe fill level for a single dosing unit.
/// This function returns the maximum syringe level in preconfigured
/// SI units.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to query
/// \param[out] pSyringeLevelMax  Maximum syringe fill level
/// \return Error code - ERR_NOERR indicates success
/// \see NCS_GetSyringeLevelMaxEx(), NCS_SetActiveVolumeUnit()
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelMax(ncs_hdl      hDevice,
                                                 unsigned char DosingUnit,
                                                 double       *pSyringeLevelMax);


//---------------------------------------------------------------------------
/// Get minimum syringe fill level for a single dosing unit.
/// This function returns the minimum syringe level in preconfigured
/// SI units.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Number of dosing unit to query
/// \param[out] pSyringeLevelMin  Minimum syringe fill level
/// \return Error code - ERR_NOERR indicates success
/// \see NCS_GetSyringeLevelMinEx(), NCS_SetActiveVolumeUnit()
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelMin(ncs_hdl      hDevice,
                                                 unsigned char DosingUnit,
                                                 double       *pSyringeLevelMin);


//---------------------------------------------------------------------------
/// Get number of dosing unnits connected to dosing platform.
/// \param[in]  hDevice           Valid device handle
/// \return Number of dosing units (0..32) or error code (< 0)
///
NCS_Func  long  __stdcall NCS_GetNumberOfDosingUnits(ncs_hdl hDevice);


//---------------------------------------------------------------------------
/// Execute calibration for single dosing unit.
/// During calibration move the dosing unit moves the pusher to the lowest
/// possible position. This position is lower than the normal zero position
/// during normal operation. Therfore you should only execute this reference
/// move if you are sure that no syringe is fitted on the device.
/// \warning Executing the calibration move with a syringe fitted on the device
///          may cause damage to the syringe.
/// \param[in]  hDevice     Valid device handle
/// \param[in]  DosingUnit  Dosing unit to calibrate
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_Calibrate(ncs_hdl  hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Execute homing move (calibration) with a certain homing method.
/// The homing method depends on the drive units that are built into the syringe
/// pump. If you know the drive unit type and if you have the drive documentation
/// (e.g. von maxon EPOS drive unit), you can execute a homing move with a
/// certain homing method.
/// \warning Executing the calibration move with a syringe fitted on the device
///          may cause damage to the syringe.
/// \param[in]  hDevice     Valid device handle
/// \param[in]  DosingUnit  Dosing unit to calibrate
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_FindHome(ncs_hdl  hDevice, unsigned char DosingUnit,
    char HomingMethod);


//---------------------------------------------------------------------------
/// Checks if a certain dosing unit is a high pressure module.
/// You can call this function to identify the type of dosing unit.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
///
/// \retval 0  Dosing unit is not a high pressure module
/// \retval 1  Dosing unit is a high pressure module
/// \retval <0 Error code - Error occured
///
NCS_Func long __stdcall NCS_IsHighPressureModule(ncs_hdl       hDevice,
                                                 unsigned char DosingUnit);
/// \}




//===========================================================================
//                           VOLUME AND FLOW UNITS
//
/// \defgroup nem_units Volume an Flow Units
/// Configure, select and query volume and flow unit parameters.
/// This group defines a set of functions for configuration of flow rate
/// units and volume units.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Get number of supported flow units (i.e. nl/s, µl/s, ml/min ...).
/// \param[in]  hDevice     Valid device handle
/// \return Number of flow units (> 0) or error code (< 0)
///
NCS_Func  long  __stdcall NCS_GetNumberOfFlowUnits(ncs_hdl hDevice);


//---------------------------------------------------------------------------
/// Get number of supported volume units (i.e. nl, µl, ml...).
/// \param[in]  hDevice     Valid device handle
/// \return Number of flow units (> 0) or error code (< 0)
///
NCS_Func  long  __stdcall NCS_GetNumberOfVolumeUnits(ncs_hdl hDevice);


//---------------------------------------------------------------------------
/// Get string for certain flow unit.
/// \param[in]  hDevice            Valid device handle
/// \param[in]  FlowUnitId         Identifier of flow unit (0 - NumberOfFlowUnits - 1)
/// \param[out] pUnitStringShort   Pointer of buffer that stores short unit strins
///                                (i.e. nl/s, ml/min...)
/// \param[in]  MaxStringShortSize Size of buffer pUnitStringShort
/// \param[out] pUnitStringLong    Pointer of buffer that stores long unit string
///                                (i.e. Nanolitres/Second, Millilitres/Minute...)
/// \param[in]  MaxStringLongSize  Size of buffer pUnitStringLong
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowUnitString(ncs_hdl       hDevice,
                                                unsigned char FlowUnitId,
                                                char         *pUnitStringShort,
                                                unsigned char MaxStringShortSize,
                                                char         *pUnitStringLong,
                                                unsigned char MaxStringLongSize);


//---------------------------------------------------------------------------
/// Get string for certain volume unit.
/// \param[in]  hDevice          Valid device handle
/// \param[in]  VolUnitId        Identifier of volume unit (0 - NumberOfVolumeUnits - 1)
/// \param[out] pUnitStringShort Pointer of buffer that stores short unit strins
///                              (i.e. nl, min...)
/// \param[in]  MaxStringShortSize Size of buffer pUnitStringShort
/// \param[out] pUnitStringLong  Pointer of buffer that stores long unit string
///                              (i.e. Nanolitres, Millilitres ...)
/// \param[in]  MaxStringLongSize Size of buffer pUnitStringLong
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetVolumeUnitString(ncs_hdl       hDevice,
                                                  unsigned char VolUnitId,
                                                  char         *pUnitStringShort,
                                                  unsigned char MaxStringShortSize,
                                                  char         *pUnitStringLong,
                                                  unsigned char MaxStringLongSize);


//---------------------------------------------------------------------------
/// Get identifier of active flow unit.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit to query
/// \return Active flow unit (>= 0) or error code (< 0)
///
NCS_Func  long  __stdcall NCS_GetActiveFlowUnit(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Get identifier of active svolume unit.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit to query
/// \return Active volume unit (>= 0) or error code (< 0)
///
NCS_Func  long  __stdcall NCS_GetActiveVolumeUnit(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Set active flow unit for single device.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit to configure
/// \param[in]  FlowUnit   Flow unit to set as active flow unit
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_SetActiveFlowUnit(ncs_hdl hDevice, unsigned char DosingUnit, unsigned char FlowUnit);


//---------------------------------------------------------------------------
/// Set active volume unit for single device.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit to configure
/// \param[in]  VolUnit   Volume unit to set as active volume unit
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_SetActiveVolumeUnit(ncs_hdl hDevice, unsigned char DosingUnit, unsigned char VolUnit);
/// \} nem_units




//===========================================================================
//                              FLOW PROFILE
//
/// \defgroup nem_flowprofile Flow Profile
/// Setup, execute and stop flow profile tables.
/// This group defines all required functions for configuration and
/// execution of flow profiles (flow tables). The flow profile enables the
/// autonomous processing of completely preconfigured flow tables. The
/// processing of these flow tables leads to a certain flow profile generated
/// by the certain pump unit. <br>
/// For each step of the profile the flow rate, the duration and the valve
/// action are configurable.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Get state of runnning flow profile.
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Dosing unit to query
/// \param[out] pRemainingCycles  Returns the number of remaining flow profile
///                               processing cycles.
/// \param[out] pActiveValueIndex Index of active flow profile entry in
///                               flow table
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowProfileState(ncs_hdl       hDevice,
                                                 unsigned char  DosingUnit,
                                                 unsigned long *pRemainingCycles,
                                                 unsigned long *pActiveValueIndex);


//---------------------------------------------------------------------------
/// Clear flow profile.
/// Clears the list of flow profile values. Do this bevore you start to
/// store new values into the flow profile with NCS_AddFlowProfileValue().
/// \param[in]  hDevice           Valid device handle
/// \param[in]  DosingUnit        Dosing unit to clear flow profile
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall  NCS_ClearFlowProfile(ncs_hdl       hDevice,
                                              unsigned char DosingUnit);

//---------------------------------------------------------------------------
/// Add single flow profile value to flow profile of certain dosing unit.
/// \param[in] hDevice             Valid device handle
/// \param[in] DosingUnit          Dosing unit to configure
/// \param[in] Flow                Single flow value to add to end of flow
///                                profile
/// \param[in] FlowUnit            Unit of the flow value in Flow parameter
/// \param[in] Duration            Time in multiple of 100 ms a certain flow
///                                shall remain constant. I.e. a duration of
///                                2 would produce a flow of 200 milliseconds.
/// \param[in] ValveAction         Identifies the valve action that should be
///                                triggered at the start of this flow profile
///                                entry. The valve actions are defined in
///                                the type TValveAction.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_AddFlowProfileValue(ncs_hdl        hDevice,
                                                unsigned char  DosingUnit,
                                                double         Flow,
                                                unsigned int   FlowUnit,
                                                unsigned long  Duration,
                                                unsigned int   ValveAction);

//---------------------------------------------------------------------------
/// Query number of flow values values in current flow profile.
/// \param[in] hDevice          Valid device handle
/// \param[in] DosingUnit       Dosing unit to query
/// \param[out] pNumberOfValues Stores number of flow values.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetNumberOfFlowProfileValues(ncs_hdl        hDevice,
                                                           unsigned char  DosingUnit,
                                                           unsigned long *pNumberOfValues);


//---------------------------------------------------------------------------
/// Read a certain entry from flow profile table.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit to query
/// \param[in]  Index      Index into table of flow values (flow profile)
/// \param[out] pValue     Stores flow rate of flow profile entry
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowProfileValue(ncs_hdl        hDevice,
                                                  unsigned char  DosingUnit,
                                                  unsigned long  Index,
                                                  double        *pValue);


//---------------------------------------------------------------------------
/// Start processing of flow profile for single dosing unit.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to start
/// \param[in]  StartIndex    Index of the flow table entry to start from
/// \param[in]  Cycles        Number of cycles to process current flow profile
///                           (0 = infinite processing until user stops operation)
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_StartFlowProfile(ncs_hdl       hDevice,
                                               unsigned char DosingUnit,
                                               unsigned long StartIndex,
                                               unsigned long Cycles);
/// \}




//===========================================================================
//                                DOSING INFO
//
/// \defgroup nem_dosing_info Dosing Info
/// Functions to query dosing parameters and device states.
/// This group defines all required functions for query of different dosing
/// parameters and device states.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Get actual syringe fill level.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pVolume       Stores actual syringe level in configured
///                           volume unit.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelIs(ncs_hdl        hDevice,
                                                unsigned char  DosingUnit,
                                                double        *pVolume);


//---------------------------------------------------------------------------
/// Get actual flow rate of single dosing unit.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pFlowRateIs   Stores actual flow rate in configured
///                           flow rateunit.
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowRateIs(ncs_hdl       hDevice,
                                           unsigned char  DosingUnit,
                                           double        *pFlowRateIs);


//---------------------------------------------------------------------------
/// Get actual syringe fill level (extended version).
/// The volume unit of the returned syringe level value is configurable in
/// this extended version of NCS_GetSyringeLevelIs().
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pVolume       Stores actual syringe level in configured
///                           volume unit.
/// \param[in] VolUnit        Volume unit for value returned in pVolume
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetSyringeLevelIsEx(ncs_hdl        hDevice,
                                                  unsigned char DosingUnit,
                                                  double       *pVolume,
                                                  TVolUnitId    VolUnit);


//---------------------------------------------------------------------------
/// Get actual flow rate of single dosing unit (extended version).
/// The flow unit of the returned flow rate value is configurable in
/// this extended version of NCS_GetFlowRateIsEx().
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pFlowRateIs   Stores actual flow rate in configured
///                           flow rate unit.
/// \param[in] FlowUnit       Flow unit for value in pFlowRateIs
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetFlowRateIsEx(ncs_hdl        hDevice,
                                              unsigned char  DosingUnit,
                                              double        *pFlowRateIs,
                                              TFlowUnitId    FlowUnit);


//---------------------------------------------------------------------------
/// Get actual current value of single dosing unit in mA.
/// The current actual value [mA] represents the current actual value
/// filtered by 1st order digital low-pass filter with a cut-off frequency
/// of 50 Hz.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pCurrent      Stores actual current value in mA
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func  long  __stdcall NCS_GetCurrentIs(ncs_hdl        hDevice,
                                           unsigned char  DosingUnit,
                                           short         *pCurrent);


//---------------------------------------------------------------------------
/// Get actual flow rate of single dosing unit.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
///
/// \retval 0  Calibration is not finished
/// \retval 1  Calibration finished
/// \retval <0 Error code
///
NCS_Func  long  __stdcall NCS_IsCalibrationFinished(ncs_hdl       hDevice,
                                                    unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Check if dosing unit finished dosing (is stopped).
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
///
/// \retval 0  Dosing is not finished
/// \retval 1  Dosing finished - device stopped
/// \retval <0 Error code
///
NCS_Func long  __stdcall NCS_IsDosingFinished(ncs_hdl       hDevice,
                                              unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Get actual pressure value of single dosing unit in bar.
/// \param[in]  hDevice    Valid device handle
/// \param[in]  DosingUnit Dosing unit query for currents pressure value
/// \param[out] pPressure  Returns the current pressure measured by the
///                        integrated pressure sensor of the high pressure
///                        module
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_GetPressureIs(ncs_hdl       hDevice,
                                          unsigned char DosingUnit,
                                          float        *pPressure);
/// \}




//===========================================================================
//                        DIGITAL / ANALOG IO
//
/// \defgroup nem_io Digital/Analog IO
/// Functions for control of digital and analog input and output.
/// This group defines all required functions for reading digital and analog
/// inputs anf for writing digital outputs.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Digital input channel identifier for function NCS_ReadDigInput().
/// You can use these identifiers to select a certain digital input channel
/// according to the purpose of this channel configured in the pump drive.
///
typedef enum eDigInChannelId
{
    NCS_DIG_IN_NEGATIVE_LIMIT = 0,
    NCS_DIG_IN_POSITIVE_LIMIT,
    NCS_DIG_IN_HOME_SWITCH,
    NCS_DIG_IN_POSITION_MARKER,
    NCS_DIG_IN_DRIVE_ENABLE,
    NCS_DIG_IN_GENERAL_PURPOSE_H = 8,
    NCS_DIG_IN_GENERAL_PURPOSE_G,
    NCS_DIG_IN_GENERAL_PURPOSE_F,
    NCS_DIG_IN_GENERAL_PURPOSE_E,
    NCS_DIG_IN_GENERAL_PURPOSE_D,
    NCS_DIG_IN_GENERAL_PURPOSE_C,
    NCS_DIG_IN_GENERAL_PURPOSE_B,
    NCS_DIG_IN_GENERAL_PURPOSE_A,
} TDigInChannelId;


//---------------------------------------------------------------------------
/// Digital ourput channel identifier for function NCS_WriteDigOutput()
/// You can use these identifiers to select a certain digital output channel
/// according to the purpose of this channel configured in the pump drive.
///
typedef enum eDigOutChannelId
{
    NCS_DIG_OUT_READY = 0,
    NCS_DIG_OUT_GENERAL_PURPOSE_E = 11,
    NCS_DIG_OUT_GENERAL_PURPOSE_D,
    NCS_DIG_OUT_GENERAL_PURPOSE_C,
    NCS_DIG_OUT_GENERAL_PURPOSE_B,
    NCS_DIG_OUT_GENERAL_PURPOSE_A
} TDigOutChannelId;


//---------------------------------------------------------------------------
/// Read digital input state of one channel of a single dosing unit.
/// \param[in]  hDevice      Valid device handle
/// \param[in]  DosingUnit   Dosing unit query for digital input state
/// \param[in]  DigInPurpose Purpose of the digital input
/// \param[out] pDigInState  Returns the current digital input state (0 or 1)
///
/// \return Error code - ERR_NOERR indicates success
/// \see TDigInChannelId enumeration for a list of symbolic digital input
///      channel identifiers.
///
NCS_Func long __stdcall NCS_ReadDigInput(ncs_hdl 	    hDevice,
                                         unsigned char  DosingUnit,
                                         unsigned char  Channel,
                                         unsigned char *pDigInState);


//---------------------------------------------------------------------------
/// Read all digital input states of a single dosing unit.
/// \param[in]  hDevice      Valid device handle
/// \param[in]  DosingUnit   Dosing unit query for digital input states
/// \param[out] pDigInStates Returns the current state of all digital input
///                          channels. Each bit represents one channel (bit0 =
///                          channel 0, bit1 = channel 1...)
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadDigInputs(ncs_hdl 	      hDevice,
                                          unsigned char   DosingUnit,
                                          unsigned short *pDigInStates);


//---------------------------------------------------------------------------
/// Write digital output state of one channel of a single dosing unit.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing to access.
/// \param[in]  DigOutPurpose Channel of the digital output
/// \param[in]  DigOutState   Digital output state to set (0 or 1)
///
/// \return Error code - ERR_NOERR indicates success
/// \see TDigOutChannelId enumeration for a list of symbolic digital output
///      channel identifiers.
///
NCS_Func long __stdcall NCS_WriteDigOutput(ncs_hdl 	     hDevice,
                                           unsigned char DosingUnit,
                                           unsigned char Channel,
                                           unsigned char DigOutState);


//---------------------------------------------------------------------------
/// Write all digital output states of a single dosing unit.
/// \param[in]  hDevice      Valid device handle
/// \param[in]  DosingUnit   Dosing unit to access.
/// \param[in]  DigOutStates Write state of all digital output channels as
///                          bitmask. Each bit represents one channl.
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_WriteDigOutputs(ncs_hdl 	   hDevice,
                                            unsigned char  DosingUnit,
                                            unsigned short DigOutStates);


//---------------------------------------------------------------------------
/// Read analog input state of one channel of a single dosing unit.
/// \param[in]  hDevice          Valid device handle
/// \param[in]  DosingUnit       Dosing unit to access.
/// \param[in]  Channel          Analog input channel (0 or 1)
/// \param[out] pVoltageAnalogIn Returns the current analog input state
///                              in the range of 0 - 5000 mV.
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadAnalogInput(ncs_hdl 	    hDevice,
                                            unsigned char   DosingUnit,
                                            unsigned char   Channel,
                                            short          *pVoltageAnalogIn);
/// \}




//===========================================================================
//                           VALVE CONTROL
//
/// \defgroup nem_valve Valve Control
/// Functions for control of integrated two-way-valve.
/// This group defines all required functions for valve control,
/// configuration and reading of valve states (if a valve is installed).
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Switch valve to input or output.
/// Switch valve to input for taking up reagent and to output for
/// delivering of reagent.
/// After switching on power supply the valve is always switched to output.
/// \param[in] hDevice        Valid device handle
/// \param[in] DosingUnit     Dosing unit to change
/// \param[in] SwitchToOutput 0 - valves switches to input
///                           1 - valve switches to output
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long  __stdcall NCS_SwitchValve(ncs_hdl hDevice, unsigned char DosingUnit, unsigned char SwitchToOutput);


//---------------------------------------------------------------------------
/// Check status of valve.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to change
///
/// \retval  0  Valve is switched to input (taking up reagent)
/// \retval  1  Valve is switched to output (delivering reagent)
/// \retval <0  Error code
///
NCS_Func long __stdcall NCS_IsValveSwitchedToOutput(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Turn valve automatic on / off.
/// If valve automatic is on the valve will be switched as soon as the dosing
/// changes
/// \param[in]  hDevice          Valid device handle
/// \param[in]  DosingUnit       Dosing unit to change
/// \param[in]  ValveAutomaticOn 0 - Valve automatic off
///                              1 - Valve automatic on
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_SetValveAutomatic(ncs_hdl hDevice, unsigned char DosingUnit, unsigned char ValveAutomaticOn);


//---------------------------------------------------------------------------
/// Checks if valve automatic is switched on or off.
/// If valve automatic is on the valve will be switched as soon as the dosing
/// changes
/// \param[in]  hDevice          Valid device handle
/// \param[in]  DosingUnit       Dosing unit to change
///
/// \retval 0   Valve automatic switching is turned off
/// \retval 1   Valve automatic switching is turned on
/// \retval <0  Error code
///
NCS_Func long __stdcall NCS_IsValveAutomaticOn(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Checks if a valve is installed.
/// \param[in]  hDevice          Valid device handle
/// \param[in]  DosingUnit       Dosing unit to change
///
/// \retval 0   No valve instaled on this dosing unit
/// \retval 1   Valve is installed
/// \retval <0  Error code
///
NCS_Func long __stdcall NCS_IsValveInstalled(ncs_hdl hDevice, unsigned char DosingUnit);


//---------------------------------------------------------------------------
/// Activate / deactivate inverted valve switching.
/// Inverted valve switching swithes the valve into the opposite direction
/// of the normal valve switching.
/// \param[in]  hDevice     Valid device handle
/// \param[in]  DosingUnit  Dosing unit to change
/// \param[in]  Invert      0 - Inverted valve switching actived
///                         1 - Inverted valve switching deactivated
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_SetInvertedValveSwitching(ncs_hdl       hDevice,
                                                      unsigned char DosingUnit,
                                                      unsigned char Invert);

//---------------------------------------------------------------------------
/// Checks if inverted valve switching is on or off.
/// \param[in]  hDevice          Valid device handle
/// \param[in]  DosingUnit       Dosing unit to change
///
/// \retval 0   Valve switching is direct (not inverted)
/// \retval 1   Valve switching is inverted
/// \retval <0  Error code
///
NCS_Func long __stdcall NCS_IsInvertedValveSwitchingOn(ncs_hdl       hDevice,
                                                       unsigned char DosingUnit);
/// \}




//===========================================================================
//                              ERROR HANDLING
//
/// \defgroup nem_error_handling Error Handling
/// This group defines all required error handling functions.
/// All functions of neMESYS API return an error code. This error code is
/// returned as a signed 32 bit integer. If no error occured, then ERR_NOERR
/// is returned. The value of ERR_NOERR is 0. If an error occurred, then
/// the error code is returned a s a negative value. I.e. if a cartain
/// function parameter is out of range, then -ERR_PARAM_RANGE will be
/// returned. That means you can check if the return code is < 0 to know if
/// an error occured.
///
/// To check if a device error occured during dosage, you should periodically
/// call NCS_IsInFaultState(). If this function returns true then you can
/// call the function NCS_GetLastDevErr() to read the last error from the
/// device. You can use the functions NCS_GetErrMsg() or NCS_GetDevErrorMsg()
/// to translate an error code into a human readable string.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Get an descriptive error message for a certain error code.
/// \param[in]  ErrCode Error code returned by function
/// \param[out] pErrMsgBuf  Buffer for storing the error string
/// \param[in]  ErrMsgBugSize Size of string buffer pErrorString points to
///
NCS_Func void __stdcall NCS_GetErrorMsg(long          ErrCode,
                                        char         *pErrMsgBuf,
                                        unsigned long ErrMsgBugSize);


//---------------------------------------------------------------------------
/// Get last error from device that is in fault state.
/// If a device is in fault state, then this funtcion returns the error
/// code of this device.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit to query
/// \param[out] pLastErr      Last device error that occured
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_GetLastDevErr(ncs_hdl       hDevice,
                                          unsigned char DosingUnit,
                                          long         *pLastErr);


//---------------------------------------------------------------------------
/// Get an descriptive error message for a certain device error code.
/// \param[in]  hDevice       Valid device handle
/// \param[in]  DosingUnit    Dosing unit number of dosing unit that caused error
/// \param[in]  DevErrCode    The device error code (NCS_GetLastDevErr())
/// \param[in]  pErrString    Buffer for storing the returned error string
/// \param[in]  MaxErrStringSize  Size of buffer pErrString
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_GetDevErrorMsg(ncs_hdl       hDevice,
                                           unsigned char DosingUnit,
                                           long          DevErrCode,
                                           char         *pErrString,
                                           unsigned char MaxErrStringSize);
/// \}


//===========================================================================
//                              DEVICE FUNCTIONS
//
/// \defgroup nem_device_functions Device Specific Functions
/// This group defines all required error functions for controlling the device
/// directly by reading and writing the object dictionary and process data.
/// Please refer to the technical documentation of the device for more
/// information on controlling the device directly with those functions.
/// \{
//===========================================================================

//---------------------------------------------------------------------------
/// Read process data.
/// \param[in]  hDevice Valid device handle
/// \param[in]  DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pBuf Buffer for data storage of read data
/// \param[in] wSize Size of buffer (size of data to read)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub Sub index into object dictionary
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadProcData(ncs_hdl    	hDevice,
                                         unsigned char 	DosingUnit,
                                         void      	   *pBuf,
                                         unsigned short	wSize,
                                         unsigned short	wObjDicIdx,
                                         unsigned char	ObjDicSub);


//---------------------------------------------------------------------------
/// Read process data and interpret it as an signed integer value.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pBuf Buffer for data storage of read data
/// \param[in] ByteSize Data size in byte (Byte = 1; Word = 2; DWORD = 4)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub Sub index into object dictionary
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadProcDataInt(ncs_hdl    		hDevice,
                                            unsigned char 	DosingUnit,
                                            long           *pBuf,
                                            unsigned char   ByteSize,
                                            unsigned short	wObjDicIdx,
                                            unsigned char	ObjDicSub);

//---------------------------------------------------------------------------
/// Write data into process image of this device.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pBuf Buffer for data to write
/// \param[in] wSize Size of buffer (size of data to write)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub Sub index into object dictionary
/// \param[in] bImmediately Send process data immediately
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code
///
NCS_Func long __stdcall NCS_WriteProcData(ncs_hdl   	 hDevice,
                                          unsigned char  DosingUnit,
                                          void     	    *pBuf,
                                          unsigned short wSize,
                                          unsigned short wObjDicIdx,
                                          unsigned char  ObjDicSub,
                                          bool    		 bImmediately);


//---------------------------------------------------------------------------
/// Write a signed integer value into process image of this device.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] Data Data to write
/// \param[in] ByteSize Data size in byte (Byte = 1; Word = 2; DWORD = 4)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub Sub index into object dictionary
/// \param[in] bImmediately Send process data immediately
///
/// \return Error code
///
NCS_Func long __stdcall NCS_WriteProcDataInt(ncs_hdl   	     hDevice,
                                             unsigned char   DosingUnit,
                                             long     	     Data,
                                             unsigned char   ByteSize,
                                             unsigned short  wObjDicIdx,
                                             unsigned char   ObjDicSub,
                                             bool    		 bImmediately);

//---------------------------------------------------------------------------
/// Read object dictionary entry.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pDataBuf Buffer for data storage of read data
/// \param[in] wSize Size of buffer (size of data to read)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub  Sub index into object dictionary
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadObjDicEntry(ncs_hdl   	   hDevice,
                                            unsigned char  DosingUnit,
                                            void          *pDataBuf,
                                            unsigned short wSize,
                                            unsigned short wObjDicIdx,
                                            unsigned char  ObjDicSub,
                                            bool           bConsiderEndian);


//---------------------------------------------------------------------------
/// Read object dictionary entry and interpret as signed integer value.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pDataBuf Buffer for data storage of read data
/// \param[in] ByteSize Data size in byte (Byte = 1; Word = 2; DWORD = 4)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub  Sub index into object dictionary
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_ReadObjDicEntryInt(ncs_hdl   	  hDevice,
                                               unsigned char  DosingUnit,
                                               long          *pDataBuf,
                                               unsigned char  ByteSize,
                                               unsigned short wObjDicIdx,
                                               unsigned char  ObjDicSub);

//---------------------------------------------------------------------------
/// Write an object dictionary entry.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] pDataBuf Buffer for data storage of read data
/// \param[in] wSize Size of buffer (size of data to read)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub  Sub index into object dictionary
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_WriteObjDicEntry(ncs_hdl   	    hDevice,
                                             unsigned char  DosingUnit,
                                             const void    *pDataBuf,
                                             unsigned short wSize,
                                             unsigned short wObjDicIdx,
                                             unsigned char  ObjDicSub,
                                             bool 		    bConsiderEndian);


//---------------------------------------------------------------------------
/// Write a signed integer object dictionary entry.
/// \param[in] hDevice Valid device handle
/// \param[in] DosingUnit Dosing unit number of dosing unit that caused error
/// \param[in] Data Data to write
/// \param[in] ByteSize Data size in byte (Byte = 1; Word = 2; DWORD = 4)
/// \param[in] wObjDicIdx Index into object dictionary
/// \param[in] ObjDicSub  Sub index into object dictionary
/// \param[in] bConsiderEndian Endianess does matter
///
/// \return Error code - ERR_NOERR indicates success
///
NCS_Func long __stdcall NCS_WriteObjDicEntryInt(ncs_hdl   	    hDevice,
                                                unsigned char	DosingUnit,
                                                long	        Data,
                                                unsigned char   ByteSize,
                                                unsigned short  wObjDicIdx,
                                                unsigned char   ObjDicSub);



/// \}

//---------------------------------------------------------------------------
#endif // __NEMEYS_API_H

