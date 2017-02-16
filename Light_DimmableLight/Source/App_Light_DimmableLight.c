/*****************************************************************************
 *
 * MODULE:             JN-AN-1171
 *
 * COMPONENT:          App_Light_DimmableLight.c
 *
 * DESCRIPTION:        ZLL Demo: Dimmable Light Implementation
 *
 ****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products [NXP Microcontrollers such as JN5168, JN5164,
 * JN5161, JN5148, JN5142, JN5139].
 * You, and any third parties must reproduce the copyright and warranty notice
 * and any other legend of ownership on each copy or partial copy of the
 * software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright NXP B.V. 2012. All rights reserved
 *
 ***************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zps_gen.h"
#include "AppHardwareApi.h"
#include "App_Light_DimmableLight.h"
#include "app_zcl_light_task.h"
#include "app_common.h"
#include "dbg.h"
#include <string.h>
#include "os.h"
#include "app_light_interpolation.h"
#include "DriverBulb_Shim.h"
#include "ep_Light.h"

#ifdef DEBUG_LIGHT_TASK
#define TRACE_LIGHT_TASK  TRUE
#else
#define TRACE_LIGHT_TASK FALSE
#endif


#ifdef DEBUG_PATH
#define TRACE_PATH  TRUE
#else
#define TRACE_PATH  FALSE
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

tsCLD_ZllDeviceTable sDeviceTable = { ZLL_NUMBER_DEVICES,
		{
				{ 0,
						ZLL_PROFILE_ID,
						DIMMABLE_LIGHT_DEVICE_ID,
						LIGHT_DIMMABLELIGHT_LIGHT_00_ENDPOINT,
						2,
						0,
						0}
		}
};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PRIVATE void vOverideProfileId(uint16* pu16Profile, uint8 u8Ep);
PRIVATE void ep_SetIdentifyTime(uint8 epId, uint16 u16Time);
/****************************************************************************
 *
 * NAME: eApp_ZLL_RegisterEndpoint
 *
 * DESCRIPTION:
 * Register ZLL endpoints
 *
 * PARAMETER
 * Type                        Name                  Descirption
 * tfpZCL_ZCLCallBackFunction  fptr                  Pointer to ZCL Callback function
 * tsZLL_CommissionEndpoint    psCommissionEndpoint  Pointer to Commission Endpoint
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/
PUBLIC teZCL_Status eApp_ZLL_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr,
		tsZLL_CommissionEndpoint* psCommissionEndpoint)
{

	sDeviceTable.asDeviceRecords[0].u64IEEEAddr = *((uint64*)pvAppApiGetMacAddrLocation());

	ZPS_vAplZdoRegisterProfileCallback(vOverideProfileId);
	zps_vSetIgnoreProfileCheck();

	eZLL_RegisterCommissionEndPoint(LIGHT_DIMMABLELIGHT_COMMISSION_ENDPOINT, fptr, psCommissionEndpoint);


	ep_00_Init(fptr);

	return E_ZCL_SUCCESS;

}


/****************************************************************************
 *
 * NAME: vOverideProfileId
 *
 * DESCRIPTION: Allows the application to over ride the profile in the
 * simple descriptor (0xc05e) with the ZHA profile id (0x0104)
 * required for on air packets
 *
 *
 * PARAMETER: pointer to the profile  to be used, the end point sending the data
 *
 * RETURNS: void
 *
 ****************************************************************************/
PRIVATE void vOverideProfileId(uint16* pu16Profile, uint8 u8Ep)
{
	switch(u8Ep){

	case LIGHT_DIMMABLELIGHT_LIGHT_00_ENDPOINT:

		*pu16Profile = 0x0104;

		return;


	default:
		return;

	}


}

/****************************************************************************
 *
 * NAME: vAppGetEPId
 *
 * DESCRIPTION:
 * Gets endpoint id
 *
 *  RETURNS:
 *  Returns endpoint id
 *
 ****************************************************************************/
//PUBLIC uint8 u8AppGetEpId(void)
//{
//	return LIGHT_DIMMABLELIGHT_LIGHT_00_ENDPOINT;
//}

/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZLL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_Init()
{


}

PRIVATE void ep_SetIdentifyTime(uint8 epId, uint16 u16Time)
{
	ep_00_SetIdentifyTime(u16Time);
}

PUBLIC void ep_SetBulbState(uint8 epId){

	ep_00_SetCurentBulbState();

}

PUBLIC bool ep_IsIdentifying(uint8 epId){

	return ep_00_IsIdentifying();
}

/****************************************************************************
 *
 * NAME: APP_vHandleIdentify
 *
 * DESCRIPTION:
 * ZLL Device Specific identify
 *
 * PARAMETER: the identify time
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void ep_HandleIdentify(uint8 epId ){

	ep_00_HandleIdentify();
}

/****************************************************************************
 *
 * NAME: vIdEffectTick
 *
 * DESCRIPTION:
 * ZLL Device Specific identify tick
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vIdEffectTick(uint8 epId) {

	if (epId != LIGHT_DIMMABLELIGHT_LIGHT_00_ENDPOINT) {
		return;
	}

	ep_00_IdEffectTick();
}

/****************************************************************************
 *
 * NAME: vStartEffect
 *
 * DESCRIPTION:
 * ZLL Device Specific identify effect set up
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void ep_StartEffect(uint8 epId, uint8 u8Effect) {

	ep_00_StartEffect(u8Effect);
}

/****************************************************************************
 *
 * NAME: vSetBulbState
 *
 * DESCRIPTION:
 * ZLL Device Specific build driver interface
 *
 * PARAMETER: the on/off state, the level
 *
 * RETURNS: void
 *
 ****************************************************************************/




/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
