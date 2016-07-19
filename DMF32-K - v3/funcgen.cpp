#include "funcgen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "visa.h"
#include "visatype.h"


    /********************************************************************/
    /*                Read and Write to a USBTMC Instrument             */
    /*                                                                  */
    /* This code demonstrates sending synchronous read & write commands */
    /* to an USB Test & Measurement Class (USBTMC) instrument using     */
    /* NI-VISA                                                          */
    /* The example writes the "*IDN?\n" string to all the USBTMC        */
    /* devices connected to the system and attempts to read back        */
    /* results using the write and read functions.                      */
    /*                                                                  */
    /* The general flow of the code is                                  */
    /*    Open Resource Manager                                         */
    /*    Open VISA Session to an Instrument                            */
    /*    Write the Identification Query Using viWrite                  */
    /*    Try to Read a Response With viRead                            */
    /*    Close the VISA Session                                        */
    /********************************************************************/

    #if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
    /* Functions like strcpy are technically not secure because they do */
    /* not contain a 'length'. But we disable this warning for the VISA */
    /* examples since we never copy more than the actual buffer size.   */
    #define _CRT_SECURE_NO_DEPRECATE
    #endif


    static ViSession defaultRM;
    static ViSession instr;
    static ViUInt32 numInstrs;
    static ViFindList findList;
    static ViUInt32 retCount;
    static ViUInt32 writeCount;
    static ViStatus status;
    static char instrResourceString[VI_FIND_BUFLEN];
    //#define DEFAULT_LOGICAL_ADDRESS "usb0::0x0957::0x1507::MY48016978::INSTR"
    //ViRsrc add = DEFAULT_LOGICAL_ADDRESS;
    //ViRsrc add = "usb0::0x0957::0x1507::MY48016978::INSTR";
    static unsigned char buffer[100];
    static char stringinput[512];


    funcGen::funcGen()
    {

    }


    void funcGen::send_voltage(float volt)
    {
        int i = 0;

        ///*
        //* First we must call viOpenDefaultRM to get the manager
        //* handle.  We will store this handle in defaultRM.
        //*/
        status = viOpenDefaultRM(&defaultRM);
        //if (status < VI_SUCCESS)
        //{
        //	printf("Could not open a session to the VISA Resource Manager!\n");
        //	exit(EXIT_FAILURE);
        //}

        ///* Find all the USB TMC VISA resources in our system and store the  */
        ///* number of resources in the system in numInstrs.                  */
        //status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);

        //if (status < VI_SUCCESS)
        //{
        //	printf("An error occurred while finding resources.\nHit enter to continue.");
        //	fflush(stdin);
        //	getchar();
        //	viClose(defaultRM);
        //	return status;
        //}

        ///*
        //* Now we will open VISA sessions to all USB TMC instruments.
        //* We must use the handle from viOpenDefaultRM and we must
        //* also use a string that indicates which instrument to open.  This
        //* is called the instrument descriptor.  The format for this string
        //* can be found in the function panel by right clicking on the
        //* descriptor parameter. After opening a session to the
        //* device, we will get a handle to the instrument which we
        //* will use in later VISA functions.  The AccessMode and Timeout
        //* parameters in this function are reserved for future
        //* functionality.  These two parameters are given the value VI_NULL.
        //*/

        //for (i = 0; i<numinstrs; i++)
        //{
        //	if (i > 0)
        //		vifindnext(findlist, instrresourcestring);

        //starts communication with the function generator, usb address: default_logical_address
            status = viOpen(defaultRM, "usb0::0x0957::0x1507::MY48016978::INSTR", VI_NULL, VI_NULL, &instr);

            if (status < VI_SUCCESS)
            {
                printf("Cannot open a session to the device %d.\n", i + 1);
                //continue;
            }

            /*
            * At this point we now have a session open to the USB TMC instrument.
            * We will now use the viWrite function to send the device the string "*IDN?\n",
            * asking for the device's identification.
            */

            //Sets the wave function to Sinusoidal
            strcpy(stringinput, "FUNCtion SINusoid\n");
            status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);


            //Sets the voltage in V
            std::ostringstream string_volt;
            string_volt << "VOLTage " << volt << "\n";
            //strcpy(stringinput, "VOLTage 0.05 \n");
            status = viWrite(instr, (ViBuf)string_volt.str().c_str(), (ViUInt32)(string_volt).str().size(), &writeCount);

            //Asks the device for identifications
//            strcpy(stringinput, "*IDN?\n");
//            status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);

//            if (status < VI_SUCCESS)
//            {
//                printf("Error writing to the device %d.\n", i + 1);
//                status = viClose(instr);
//                //continue;
//            }


            /*
            * Now we will attempt to read back a response from the device to
            * the identification query that was sent.  We will use the viRead
            * function to acquire the data.  We will try to read back 100 bytes.
            * This function will stop reading if it finds the termination character
            * before it reads 100 bytes.
            * After the data has been read the response is displayed.
            */
        /*	status = viRead(instr, buffer, 100, &retCount);
            if (status < VI_SUCCESS)
            {
                printf("Error reading a response from the device %d.\n", i + 1);
            }
            else
            {
                printf("\nDevice %d: %*s\n", i + 1, retCount, buffer);
            }*/
            //status = viClose(instr);





        /*
       * Now we will close the session to the instrument using
        * viClose. This operation frees all system resources.
        */
        status = viClose(defaultRM);
        //printf("Hit enter to continue.");

         fflush(stdin);
         //getchar();

    }
