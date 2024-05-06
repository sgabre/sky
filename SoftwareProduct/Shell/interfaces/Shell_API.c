/** @addtogroup   UI
@{
********************************************************************************
*   @name       I_UI
*   @file       I_UI.c
*
*   @brief      Interface to User Interface
*
*   @authors    S. Schuepbach
*   @date       14.06.2017
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include <stdio.h>
#include "SoftwareComponent_API.h"
#include "../sources/SoftwareComponent.h"


/*== Defines =================================================================*/

/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Global Variables ========================================================*/


/*== Module Constants ========================================================*/


/*== Function Prototypes =====================================================*/


/*== Module Variables ========================================================*/



/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Internal Functions ======================================================*/
/*============================================================================*/


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/


/*============================================================================*/
/*== Implementation of Object Member Functions ===============================*/
/*============================================================================*/


// BUZ Dummy Functions
static eSoftwareComponentError mSoftwareComponent_SetDummy (uint16_t unFrequency)
{
	//ASSERT_FAIL();
	printf("%s Dummy...\n",__FUNCTION__);
	return SOFTWARE_COMPONENT_OK_ERROR;
}
static eSoftwareComponentError mSoftwareComponent_Dummy(void)
{
	//ASSERT_FAIL();
	printf("%s Dummy...\n",__FUNCTION__);
	return SOFTWARE_COMPONENT_OK_ERROR;
}


// Weak Function(s) for Initialization
/*
__weak void SoftwareComponent_Setup(void)
{
	
}
*/



/*============================================================================*/
/*== Instantiation of SoftwareComponent Object ==========================================*/
/*============================================================================*/

static iSoftwareComponent_ts mSoftwareComponentInterfaces =
{
	.Set = mSoftwareComponent_SetDummy,
	.Enable = mSoftwareComponent_Dummy,
	.Disable = mSoftwareComponent_Dummy
};



/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

iSoftwareComponent_ts *iSoftwareComponent(void)
{
	return &mSoftwareComponentInterfaces;
}

void iSoftwareComponent_Setup(void)
{
	SoftwareComponent_Setup();	
}

/// @}



