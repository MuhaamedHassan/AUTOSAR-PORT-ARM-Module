/*
 * main.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Mohamed
 */


#include <assert.h>
#include "Port.h"
#include "Det.h"
#include "tm4c123gh6pm_registers.h"


#define PORT_INVALID_PINS_NUM    100
#define Port_INVALID_MODE_NUM    20

boolean Det_TestLastReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    boolean result = (Det_ModuleId == ModuleId) && (Det_InstanceId == InstanceId) &&
                     (Det_ApiId == ApiId) && (Det_ErrorId == ErrorId);
    Det_ModuleId = 0;
    Det_InstanceId = 0;
    Det_ApiId = 0;
    Det_ErrorId = 0;

    return result;
}
/************************************************************************************
* Test ID :TestCase1
* Description: Test For Set Direction ,Set Mode ,Get Version Or Refresh Direction
*             The PORT_E_UNINIT Should Be Reported
* Pre-requisits: Port Module is not initialized before Run this test case.
* Test Procedure : 1. Call Port_SetPinDirection DET error PORT_E_UNINIT should be reported.
*                  2. Call Port_SetPinMode DET error PORT_E_UNINIT should be reported.
*                  3. Call Port_RefreshPortDirection DET error PORT_E_UNINIT should be reported.
*************************************************************************************/
void TestCase1(void)
{
	Port_SetPinDirection(PortConfig_PORTA_PIN0_ID_INDEX,PORT_PIN_OUT);
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_SetPinDirection_SID,PORT_E_UNINIT));

	Port_SetPinMode(PortConfig_PORTA_PIN3_ID_INDEX,PortConfig_Mode_GPIO);
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_SetPinMode_SID,PORT_E_UNINIT));

	Port_RefreshPortDirection();
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_RefreshPortDirection_SID,PORT_E_UNINIT));
}
/************************************************************************************
* Test ID :TestCase2
* Description: Test for Port_Init and Port_GetVersionInfo API's report the correct DET
*              error in case the API called with NULL pointer.
* Pre-requisits: No Need
* Test Procedure : 1. Call Port_Init DET error PORT_E_PARAM_CONFIG should be reported.
*                  2. Call Port_GetVersionInfo DET error Port_E_PARAM_POINTER should be reported.
*************************************************************************************/
void TestCase2(void)
{
    Port_Init(NULL_PTR);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_Init_SID,PORT_E_PARAM_CONFIG));


#if (PORT_VERSION_INFO_API == STD_ON)
    Port_GetVersionInfo(NULL_PTR);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_GetVersionInfo_SID,PORT_E_PARAM_POINTER));
#endif
}
/************************************************************************************
* Test ID :TestCase3
* Description: Test For SetPinDirection And SetPinMode For Invalid Pin Number
*              In This Case Should Get PORT_E_PARAM_PIN
* Pre-requisits: Port Module is initialized before Run this test case.
* Test Procedure : 1. Call Port_SetPinDirection DET error PORT_E_PARAM_PIN
*                     should be reported.
*                  2. Call Port_SetPinMode DET error PORT_E_PARAM_PIN
*                     should be reported.
*************************************************************************************/
void TestCase3(void)
{
	Port_SetPinDirection(PORT_INVALID_PINS_NUM,PORT_PIN_OUT);
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_SetPinDirection_SID,PORT_E_PARAM_PIN));

	Port_SetPinMode(PORT_INVALID_PINS_NUM,PortConfig_Mode_GPIO);
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_SetPinMode_SID,PORT_E_PARAM_PIN));

}
/************************************************************************************
* Test ID :TestCase4
* Description: Test For  SetPinMode For Invalid Mode Number
*              In This Case Should Get PORT_E_PARAM_INVALID_MODE
* Pre-requisits: Port Module is initialized before Run this test case.
* Test Procedure :1. Call Port_SetPinMode DET error PORT_E_PARAM_INVALID_MODE
*                     should be reported.
*************************************************************************************/
void TestCase4(void)
{
	Port_SetPinMode(PortConfig_PORTA_PIN0_ID_INDEX,Port_INVALID_MODE_NUM);
	assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,Port_SetPinMode_SID,PORT_E_PARAM_INVALID_MODE));
}
/************************************************************************************
* Test ID :TestCase5
* Description: Test for GetVersionInfo API functionality.
* Pre-requisits: No Need
* Test Procedure : Call Port_GetVersionInfo and get the required information.
*************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void TestCase5(void)
{
    Std_VersionInfoType Version_Info;
    Port_GetVersionInfo(&Version_Info);
    assert(Version_Info.vendorID == PORT_VENDOR_ID);
    assert(Version_Info.moduleID == PORT_MODULE_ID);
    assert(Version_Info.sw_major_version == PORT_SW_MAJOR_VERSION);
    assert(Version_Info.sw_minor_version == PORT_SW_MINOR_VERSION);
    assert(Version_Info.sw_patch_version == PORT_SW_PATCH_VERSION);
}
#endif
/************************************************************************************
* Test ID :TestCase6
* Description: Test for SetPinDirection API functionality.
* Pre-requisits: No Need
* Test Procedure : Call Port_SetPinDirection and Check The Direction Of Pin
*************************************************************************************/
void TestCase6(void)
{
	uint8 PORTA_PIN0 = PORT_PIN_OUT ;
	uint8 PORTA_PIN1 = PORT_PIN_IN ;
	while(PORTA_PIN0 != PORT_PIN_IN)
	{
		Port_SetPinDirection(PortConfig_PORTA_PIN0_ID_INDEX,PORT_PIN_IN);
		PORTA_PIN0 = GPIO_PORTA_DIR_REG ;
	}
	while(PORTA_PIN1 != 2 )
	{
		Port_SetPinDirection(PortConfig_PORTA_PIN1_ID_INDEX,PORT_PIN_OUT);
		PORTA_PIN1 = GPIO_PORTA_DIR_REG ;

	}
}
/************************************************************************************
* Test ID :TestCase7
* Description: Test for SetPinMode API functionality.
* Pre-requisits: No Need
* Test Procedure : Call Port_SetPinMode and Check The Mode Of Pin
*************************************************************************************/

void TestCase7(void)
{
	uint32 PORTA_PIN0_MODE = 0x0000000F;

	while (PORTA_PIN0_MODE == 0x0000000F)
	{
		Port_SetPinMode(PortConfig_PORTA_PIN0_ID_INDEX,PortConfig_Mode_UART);

		PORTA_PIN0_MODE = GPIO_PORTA_PCTL_REG ;
	}
}
int main (void)
{


	TestCase1();

	Port_Init(&Port_Configuration);

	TestCase2();

	TestCase3();

	TestCase4();

#if (PORT_VERSION_INFO_API == STD_ON)
    /* Run the Fifth Test Case */
    TestCase5();
#endif

    TestCase6();

    TestCase7();

}




