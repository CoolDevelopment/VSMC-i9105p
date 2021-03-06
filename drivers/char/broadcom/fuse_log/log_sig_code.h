//*********************************************************************
//
// (c)1999-2011 Broadcom Corporation
//
// Unless you and Broadcom execute a separate written software license agreement governing use of this software,
// this software is licensed to you under the terms of the GNU General Public License version 2,
// available at http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
//
//*********************************************************************

/* C Header for all SDL signal codes and processe IDs for logging purpose only
**
*/

/*
 * List of all process names for gentrcon.
 *
 * ms_processes =
 * {
 *    cmc
 *    cmm
 *    cms
 *    dch
 *    df2
 *    dll
 *    drl
 *    dtn
 *    dtt
 *    gl1s
 *    gme
 *    gmm
 *    gmr
 *    grr
 *    l1u
 *    llc
 *    mac
 *    mma
 *    mmc
 *    mme
 *    mmr
 *    mmreg
 *    mncc
 *    mnr
 *    mns
 *    mnss
 *    oms
 *    rlc
 *    rrc
 *    sim
 *    sme
 *    smr
 *    snp
 *    ubmc
 *    umacc
 *    umacdl
 *    umacul
 *    urabmupdcp
 *    urlcc
 *    urlcdl
 *    urlcul
 *    urrcbp
 *    urrcdc
 *    urrcm
 *    many_receiver
 *    no_receiver
 * }
 */

#ifndef LOG_SIG_CODE_H
#define LOG_SIG_CODE_H

#ifndef UNDER_LINUX
#include "proc_id.h"
#endif

#define P_l1u			0	// 0,1
#define P_umacdl		6	// 6,7
#define P_umacul		8	// 8,9
#define P_urabmupdcp		10	// 10
#define P_urlcdl		13	// 13,14
#define P_urlcul		15	// 15,16
#define P_urrcdc		19	// 19,20
#define P_urrcm			21	// 21,22
#define P_mmc			35	// 35,36
#define P_rlc			41	// 41,42
#define P_gl1s			47	// 47

#define	P_log_gl1s			(P_gl1s|0x80)
#define	P_log_l1u			(P_l1u|0x80)
#define	P_log_mmc			(P_mmc|0x80)
#define	P_log_rlc			(P_rlc|0x80)
#define	P_log_umacdl			(P_umacdl|0x80)
#define	P_log_umacul			(P_umacul|0x80)
#define	P_log_urabmupdcp		(P_urabmupdcp|0x80)
#define	P_log_urlcdl			(P_urlcdl|0x80)
#define	P_log_urlcul			(P_urlcul|0x80)
#define	P_log_urrcdc			(P_urrcdc|0x80)
#define	P_log_urrcm			(P_urrcm|0x80)

#define	P_dsp2g				245
#define	P_dspxc				246
#define	P_log_group			247
#define	P_dspfw				248
#define	P_log_api			249
#define	P_log_general		250
#define P_umtsfw			251
#define	P_itmsg				252

#define	P_no_receiver		253
#define	P_many_receiver		254

#include "logapi.h"

typedef enum {
#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_urrcm */
#ifndef SDTENV
	URRCM_RESEL_TRACE_EVAL = ((P_log_urrcm << 16) | 1),
#else
	URRCM_RESEL_TRACE_EVAL,
#endif
	URRCM_RESEL_TRACE_CONFIG,
	URRCM_TRACE_FILTERED_INTRA,
	URRCM_TRACE_FILTERED_INTER,
	URRCM_TRACE_FILTERED_GSM,
	/* End of log_urrcm */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

#if defined(STACK_wedge) && defined(UMTS) && defined(UMTS_HSDPA)
	/* Start of log_umacdl */
#ifndef SDTENV
	UMACDL_HSDPA_TRACE_STATE = ((P_log_umacdl << 16) | 1),
#else
	UMACDL_HSDPA_TRACE_STATE,
#endif
	UMACDL_HSDPA_TRACE_STATE_CHANGE,
	UMACDL_AMR_DL,
	UMACDL_LOG_LCH_TRCH_INFO,
	/* End of log_umacdl */
#endif				// #if defined(STACK_wedge) && defined(UMTS) && defined(UMTS_HSDPA)

	/* Start of log_gl1s */
#ifndef SDTENV
	EGPRS_TX_BLK = ((P_log_gl1s << 16) | 1),
#else
	EGPRS_TX_BLK,
#endif
	EDGE_RX_HDR,
	DSP_DATA_OBSOLETE,
	AGC_TRACK_DATA,
	RX_TRACK_DATA,
	SACCH_TRACK_DATA,
	TBF_TRACK_DATA,
	PG_TRACK_DATA,
	TCH_F_TRACK_DATA,
	TCH_H_TRACK_DATA,
	L1DATA_FRAMEENTRY,
	L1DATA_ENTRYDATA,
	L1DATA_CELLDATA,
	L1DATA_EVENT,
	L1DATA_MON_BCCH,
	L1DATA_MON_RAW,
	L1DATA_FSC_LOGGING,
	L1DATA_SEARCH_REQ,
	SC_TRACK_DATA,
	VMS_TRACE,
	SCH_TRACK_DATA,
	BP_TRACK_DATA,
	PH_SDCCH_REQ,
	PH_FACCH_REQ,
	PH_SACCH_REQ,
	L1DATA_RTVMS,		///<Payload type {::RTVMS_LOG_t}
	L1DATA_3G_EVENT,	///<Payload type {::T_VMS_EVENT_IF}
	L1DATA_EMA_RXLEV,
	L1DATA_PCH_MON,
	L1DATA_SNR_SPEECH,
	L1DATA_SNR_SACCH,
	L1DATA_SNR_TBF,
	L1DATA_SNR_IDLE,
	L1DATA_USF,
	L1DATA_DCH_PWRSTEP,
	L1DATA_TXPWR,
	L1DATA_AMR,
	L1DATA_SACCH,
	L1DATA_FACCH,
	L1DATA_BPM_STATUS,	/* //<Payload type {::BPM_STATUS_LOG_t} */
	L1DATA_TWIF,		/*//<Payload type {::TWIF_LOG_t} */
	AUDIO_DATA_OBSOLETE,
	L1DATA_RF_LOGGING,
	VMS_TRACE_KEYS,
	MPH_NCELL_RXLEV_IND_CHART,	/* //<Payload type {::CHART_X_Y} */
	MPH_RXLEV_CNF_CHART,	/* //<Payload type {::CHART_X_Y} */
	/* End of log_gl1s */

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_l1u */
#ifndef SDTENV
	UMTS_TX_DATA = ((P_log_l1u << 16) | 1),
#else
	UMTS_TX_DATA,
#endif
	SPINNER_SM_TX_POWER_ARRAY,
	SPINNER_RSSI_RESULTS,
	SPINNER_CELLS_FOUND,
	SPINNER_MEASUREMENT_IND,
	SPINNER_FACHPCH_INFO_IND,
	SPINNER_FACHPCH_DATA_IND,
	SPINNER_DCH_INFO_IND,
	SPINNER_DCH_DATA_IND,
	SPINNER_BCH_DATA_IND,
	MACHS_HDR,
	SPINNER_SM_DPCCH_POWER_ARRAY,
	L1U_TRACE_RSSI_SCAN,	// MobC00074069, SKS, 4/21/09 
	SL1R_TRACE_DATA,
	MACEHS_HDR,
	L1U_TRACE_RSSI_SCAN_CHART,	/* //<Payload type {::CHART_X_Y} */
	/* End of log_l1u */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_umacul */
#ifndef SDTENV
	UMTS_HS_TX_INFO = ((P_log_umacul << 16) | 1),
#else
	UMTS_HS_TX_INFO,
#endif
	UMTS_HS_GRANT_INFO,
	MACES_HDR,
	UMAC_LOG_GENERIC,
	MACE_PREEVAL_PERNTX_INFO,
	UMAC_UL_PREEVAL_TFCI_INFO,
	MACIS_HDR,
	UMTS_HS_UL_DTX_INFO,
	UMTS_HS_DL_DRX_INFO,
	UMTS_HS_NODEB_DRX_INFO,
	UMACUL_AMR_UL,
	UMTS_HS_DL_DRX_GRANT_MONITORING,
	UMTS_HS_DL_DRX_GRANT_MONITORING_TIMER,
	UMTS_HS_DL_DRX_L1_MON_TICK_TIMER,
	UMTS_HS_DL_DRX_DATA_AVAILABLE,
	UMTS_HS_UL_DTX_PREAMBLES,
	/* End of log_umacul */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_urabmupdcp */
#ifndef SDTENV
	URABMPDCP_LOG_GENERIC = ((P_log_urabmupdcp << 16) | 1),
#else
	URABMPDCP_LOG_GENERIC,
#endif
	/* End of log_urabmupdcp */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_urlcdl */
#ifndef SDTENV
	URLC_STATS_INFO = ((P_log_urlcdl << 16) | 1),
#else
	URLC_STATS_INFO,
#endif
	/* End of log_urlcdl */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of log_urlcul */
#ifndef SDTENV
	URLC_LOG_GENERIC = ((P_log_urlcul << 16) | 1),
#else
	URLC_LOG_GENERIC,
#endif
	URLC_UL_STATS_RBS_INFO,
	URLC_DL_STATS_RBS_INFO,
	UAS_CIPHER_KEYS_IND,
	URLC_SDU_Q_WATERMARK_THRESHOLD,	/* MobC00223725: IPC buffer overflow signal */
	/* End of log_urlcul */

	/* Start of log_urrcdc */
#ifndef SDTENV
	URRCDC_TRACE_DB_CFG = ((P_log_urrcdc << 16) | 1),
#else
	URRCDC_TRACE_DB_CFG,
#endif
	URRC_LOG_GENERIC,
	/* End of log_urrcdc */
#endif				// #if defined(STACK_wedge) && defined(UMTS)

	/* Start of log_mmc */
#ifndef SDTENV
	MMC_LOCAL_VALUE = ((P_log_mmc << 16) | 1),
#else
	MMC_LOCAL_VALUE,
#endif
	/* End of log_mmc */

	/* Start of log_rlc */
#ifndef SDTENV
	RLC_TRACE_UL_WIN_VARS = ((P_log_rlc << 16) | 1),
	RLC_TRACE_DL_WIN_VARS,
#else
	RLC_TRACE_UL_WIN_VARS,
	RLC_TRACE_DL_WIN_VARS,
#endif
	/* End of log_rlc */

	/* Start of dsp2g */
#ifndef SDTENV
	DSP2G_LOG_GROUP = ((P_dsp2g << 16) | 1),
#else
	DSP2G_LOG_GROUP,
#endif
	/* End of dsp2g */

	/* Start of dspxc */
#ifndef SDTENV
	DSPXC_LOG_GROUP = ((P_dspxc << 16) | 1),
#else
	DSPXC_LOG_GROUP,
#endif
	/* End of dspxc */

	/* Start of dspfw */
	DSP_PROFILE = ((P_dspfw << 16) | 1),
	DSP_RAW_DATA,
	DSP_DEBUG_DATA,
	/* End of dspfw */

	/* Start of log_api */
	API_CAPI_API_CAPI_MSG_DUMMY_1 = ((P_log_api << 16) | 1),
	API_CAPI_API_CAPI_MSG_DUMMY_2 = ((P_log_api << 16) | 2),
	API_CAPI_API_CAPI_MSG_DUMMY_3 = ((P_log_api << 16) | 3),
	API_CAPI_API_CAPI_MSG_DUMMY_4 = ((P_log_api << 16) | 4),
	API_CAPI_API_CAPI_MSG_DUMMY_5 = ((P_log_api << 16) | 5),
	API_CAPI_API_CAPI_MSG_DUMMY_6 = ((P_log_api << 16) | 6),
	API_CAPI_API_CAPI_MSG_DUMMY_7 = ((P_log_api << 16) | 7),
	API_CAPI_MSG_SYS_POWERDOWN_REQ = ((P_log_api << 16) | 8),	///<Payload type {::T_PhoneCtrlApi_ProcessPowerDownReq}
	API_CAPI_MSG_SYS_POWERUP_NORF_REQ = ((P_log_api << 16) | 9),	///<Payload type {::T_PhoneCtrlApi_ProcessNoRfReq}
	API_CAPI_MSG_SYS_NORF_CALIB_REQ = ((P_log_api << 16) | 10),	///<Payload type {::T_PhoneCtrlApi_ProcessNoRfToCalib}
	API_CAPI_MSG_SYS_POWERUP_REQ = ((P_log_api << 16) | 11),	///<Payload type {::T_PhoneCtrlApi_ProcessPowerUpReq}
	API_CAPI_API_CAPI_MSG_DUMMY_12 = ((P_log_api << 16) | 12),
	API_CAPI_API_CAPI_MSG_DUMMY_13 = ((P_log_api << 16) | 13),
	API_CAPI_API_CAPI_MSG_DUMMY_14 = ((P_log_api << 16) | 14),
	API_CAPI_MSG_SIM_UPDATE_SMSCAPEXC_FLAG_REQ = ((P_log_api << 16) | 15),	///<Payload type {::T_SimApi_UpdateSMSCapExceededFlag}
	API_CAPI_MSG_MS_SELECT_BAND_REQ = ((P_log_api << 16) | 16),	///<Payload type {::T_NetRegApi_SelectBand}
	API_CAPI_MSG_MS_SET_RAT_BAND_REQ = ((P_log_api << 16) | 17),	///<Payload type {::T_NetRegApi_SetSupportedRATandBand}
	API_CAPI_MSG_MS_CELL_LOCK_REQ = ((P_log_api << 16) | 18),	///<Payload type {::T_NetRegApi_CellLock}
	API_CAPI_MSG_MS_GET_MCC_COUNTRY_REQ = ((P_log_api << 16) | 19),	///<Payload type {::T_PLMN_GetCountryByMcc}
	API_CAPI_MSG_MS_PLMN_INFO_BY_CODE_REQ = ((P_log_api << 16) | 20),	///<Payload type {::T_MS_GetPLMNEntryByIndex}
	API_CAPI_API_CAPI_MSG_DUMMY_21 = ((P_log_api << 16) | 21),
	API_CAPI_MSG_MS_PLMN_INFO_BY_INDEX_REQ = ((P_log_api << 16) | 22),	///<Payload type {::T_MS_GetPLMNByCode}
	API_CAPI_MSG_PLMN_SELECT_REQ = ((P_log_api << 16) | 23),	///<Payload type {::T_NetRegApi_PlmnSelect}
	API_CAPI_MSG_MS_PLMN_ABORT_REQ = ((P_log_api << 16) | 24),	///<Payload type {::T_NetRegApi_AbortPlmnSelect}
	API_CAPI_API_CAPI_MSG_DUMMY_25 = ((P_log_api << 16) | 25),
	API_CAPI_MSG_MS_SET_PLMN_MODE_REQ = ((P_log_api << 16) | 26),	///<Payload type {::T_NetRegApi_SetPlmnMode}
	API_CAPI_API_CAPI_MSG_DUMMY_27 = ((P_log_api << 16) | 27),
	API_CAPI_MSG_MS_SET_PLMN_FORMAT_REQ = ((P_log_api << 16) | 28),	///<Payload type {::T_MS_SetPlmnFormat}
	API_CAPI_MSG_MS_MATCH_PLMN_REQ = ((P_log_api << 16) | 29),	///<Payload type {::T_MS_IsMatchedPLMN}
	API_CAPI_MSG_MS_SEARCH_PLMN_REQ = ((P_log_api << 16) | 30),	///<Payload type {::T_NetRegApi_SearchAvailablePLMN}
	API_CAPI_MSG_MS_ABORT_PLMN_REQ = ((P_log_api << 16) | 31),	///<Payload type {::T_NetRegApi_AbortSearchPLMN}
	API_CAPI_MSG_MS_AUTO_SEARCH_REQ = ((P_log_api << 16) | 32),	///<Payload type {::T_NetRegApi_AutoSearchReq}
	API_CAPI_MSG_MS_PLMN_NAME_REQ = ((P_log_api << 16) | 33),	///<Payload type {::T_NetRegApi_GetPLMNNameByCode}
	API_CAPI_MSG_SYS_GET_SYSTEM_STATE_REQ = ((P_log_api << 16) | 34),	///<Payload type {::T_PhoneCtrlApi_GetSystemState}
	API_CAPI_MSG_SYS_SET_SYSTEM_STATE_REQ = ((P_log_api << 16) | 35),	///<Payload type {::T_PhoneCtrlApi_SetSystemState}
	API_CAPI_MSG_SYS_GET_RX_LEVEL_REQ = ((P_log_api << 16) | 36),	///<Payload type {::T_PhoneCtrlApi_GetRxSignalInfo}
	API_CAPI_API_CAPI_MSG_DUMMY_37 = ((P_log_api << 16) | 37),
	API_CAPI_API_CAPI_MSG_DUMMY_38 = ((P_log_api << 16) | 38),
	API_CAPI_API_CAPI_MSG_DUMMY_39 = ((P_log_api << 16) | 39),
	API_CAPI_API_CAPI_MSG_DUMMY_40 = ((P_log_api << 16) | 40),
	API_CAPI_MSG_SET_POWER_DOWN_TIMER_REQ = ((P_log_api << 16) | 41),	///<Payload type {::T_PhoneCtrlApi_SetPowerDownTimer}
	API_CAPI_MSG_SIM_PARAM_REC_NUM_REQ = ((P_log_api << 16) | 42),	///<Payload type {::T_SimApi_GetSmsParamRecNum}
	API_CAPI_MSG_SIM_GET_SMSMEMEXC_FLAG_REQ = ((P_log_api << 16) | 43),	///<Payload type {::T_SimApi_GetSmsMemExceededFlag}
	API_CAPI_MSG_SIM_IS_TEST_SIM_REQ = ((P_log_api << 16) | 44),	///<Payload type {::T_SimApi_IsTestSIM}
	API_CAPI_MSG_SIM_PIN_REQ_REQ = ((P_log_api << 16) | 45),	///<Payload type {::T_SimApi_IsPINRequired}
	API_CAPI_MSG_SIM_CARD_PHASE_REQ = ((P_log_api << 16) | 46),	///<Payload type {::T_SimApi_GetCardPhase}
	API_CAPI_MSG_SIM_TYPE_REQ = ((P_log_api << 16) | 47),	///<Payload type {::T_SimApi_GetSIMType}
	API_CAPI_MSG_SIM_PRESENT_REQ = ((P_log_api << 16) | 48),	///<Payload type {::T_SimApi_GetPresentStatus}
	API_CAPI_MSG_SIM_PIN_OPERATION_REQ = ((P_log_api << 16) | 49),	///<Payload type {::T_SimApi_IsOperationRestricted}
	API_CAPI_MSG_SIM_PIN_BLOCK_REQ = ((P_log_api << 16) | 50),	///<Payload type {::T_SimApi_IsPINBlocked}
	API_CAPI_MSG_SIM_PUK_BLOCK_REQ = ((P_log_api << 16) | 51),	///<Payload type {::T_SimApi_IsPUKBlocked}
	API_CAPI_MSG_SIM_IS_INVALID_REQ = ((P_log_api << 16) | 52),	///<Payload type {::T_SimApi_IsInvalidSIM}
	API_CAPI_MSG_SIM_DETECT_REQ = ((P_log_api << 16) | 53),	///<Payload type {::T_SimApi_DetectSim}
	API_CAPI_MSG_SIM_GET_RUIM_SUPP_FLAG_REQ = ((P_log_api << 16) | 54),	///<Payload type {::T_SimApi_GetRuimSuppFlag}
	API_CAPI_MSG_SIM_VERIFY_CHV_REQ = ((P_log_api << 16) | 55),	///<Payload type {::T_SimApi_SendVerifyChvReq}
	API_CAPI_MSG_SIM_CHANGE_CHV_REQ = ((P_log_api << 16) | 56),	///<Payload type {::T_SimApi_SendChangeChvReq}
	API_CAPI_MSG_SIM_ENABLE_CHV_REQ = ((P_log_api << 16) | 57),	///<Payload type {::T_SimApi_SendSetChv1OnOffReq}
	API_CAPI_MSG_SIM_UNBLOCK_CHV_REQ = ((P_log_api << 16) | 58),	///<Payload type {::T_SimApi_SendUnblockChvReq}
	API_CAPI_MSG_SIM_SET_FDN_REQ = ((P_log_api << 16) | 59),	///<Payload type {::T_SimApi_SendSetOperStateReq}
	API_CAPI_MSG_SIM_IS_PBK_ALLOWED_REQ = ((P_log_api << 16) | 60),	///<Payload type {::T_SimApi_IsPbkAccessAllowed}
	API_CAPI_MSG_SIM_PBK_INFO_REQ = ((P_log_api << 16) | 61),	///<Payload type {::T_SimApi_SendPbkInfoReq}
	API_CAPI_MSG_SIM_MAX_ACM_REQ = ((P_log_api << 16) | 62),	///<Payload type {::T_SimApi_SendReadAcmMaxReq}
	API_CAPI_MSG_SIM_ACM_MAX_UPDATE_REQ = ((P_log_api << 16) | 63),	///<Payload type {::T_SimApi_SendWriteAcmMaxReq}
	API_CAPI_MSG_SIM_ACM_VALUE_REQ = ((P_log_api << 16) | 64),	///<Payload type {::T_SimApi_SendReadAcmReq}
	API_CAPI_MSG_SIM_ACM_UPDATE_REQ = ((P_log_api << 16) | 65),	///<Payload type {::T_SimApi_SendWriteAcmReq}
	API_CAPI_MSG_SIM_ACM_INCREASE_REQ = ((P_log_api << 16) | 66),	///<Payload type {::T_SimApi_SendIncreaseAcmReq}
	API_CAPI_MSG_SIM_SVC_PROV_NAME_REQ = ((P_log_api << 16) | 67),	///<Payload type {::T_SimApi_SendReadSvcProvNameReq}
	API_CAPI_MSG_SIM_PUCT_DATA_REQ = ((P_log_api << 16) | 68),	///<Payload type {::T_SimApi_SendReadPuctReq}
	API_CAPI_MSG_SIM_SERVICE_STATUS_REQ = ((P_log_api << 16) | 69),	///<Payload type {::T_SimApi_GetServiceStatus}
	API_CAPI_MSG_SIM_PIN_STATUS_REQ = ((P_log_api << 16) | 70),	///<Payload type {::T_SimApi_GetPinStatus}
	API_CAPI_MSG_SIM_PIN_OK_STATUS_REQ = ((P_log_api << 16) | 71),	///<Payload type {::T_SimApi_IsPinOK}
	API_CAPI_MSG_SIM_IMSI_REQ = ((P_log_api << 16) | 72),	///<Payload type {::T_SimApi_GetIMSI}
	API_CAPI_MSG_SIM_GID_DIGIT_REQ = ((P_log_api << 16) | 73),	///<Payload type {::T_SimApi_GetGID1}
	API_CAPI_MSG_SIM_GID_DIGIT2_REQ = ((P_log_api << 16) | 74),	///<Payload type {::T_SimApi_GetGID2}
	API_CAPI_MSG_SIM_HOME_PLMN_REQ = ((P_log_api << 16) | 75),	///<Payload type {::T_SimApi_GetHomePlmn}
	API_CAPI_MSG_SIM_APDU_FILEID_REQ = ((P_log_api << 16) | 76),	///<Payload type {::T_simmiApi_GetMasterFileId}
	API_CAPI_MSG_SIM_OPEN_SOCKET_REQ = ((P_log_api << 16) | 77),	///<Payload type {::T_SimApi_SendOpenSocketReq}
	API_CAPI_MSG_SIM_SELECT_APPLI_REQ = ((P_log_api << 16) | 78),	///<Payload type {::T_SimApi_SendSelectAppiReq}
	API_CAPI_MSG_SIM_DEACTIVATE_APPLI_REQ = ((P_log_api << 16) | 79),	///<Payload type {::T_SimApi_SendDeactivateAppiReq}
	API_CAPI_MSG_SIM_CLOSE_SOCKET_REQ = ((P_log_api << 16) | 80),	///<Payload type {::T_SimApi_SendCloseSocketReq}
	API_CAPI_MSG_SIM_ATR_DATA_REQ = ((P_log_api << 16) | 81),	///<Payload type {::T_SimApi_GetAtrData}
	API_CAPI_MSG_SIM_DFILE_INFO_REQ = ((P_log_api << 16) | 82),	///<Payload type {::T_SIM_SubmitDFileInfoReqOld}
	API_CAPI_MSG_SIM_EFILE_INFO_REQ = ((P_log_api << 16) | 83),	///<Payload type {::T_SimApi_SubmitEFileInfoReq}
	API_CAPI_MSG_SIM_SEND_EFILE_INFO_REQ = ((P_log_api << 16) | 84),	///<Payload type {::T_SimApi_SendEFileInfoReq}
	API_CAPI_MSG_SIM_SEND_DFILE_INFO_REQ = ((P_log_api << 16) | 85),	///<Payload type {::T_SimApi_SendDFileInfoReq}
	API_CAPI_MSG_SIM_WHOLE_EFILE_DATA_REQ = ((P_log_api << 16) | 86),	///<Payload type {::T_SimApi_SubmitWholeBinaryEFileReadReq}
	API_CAPI_MSG_SIM_SEND_WHOLE_EFILE_DATA_REQ = ((P_log_api << 16) | 87),	///<Payload type {::T_SimApi_SendWholeBinaryEFileReadReq}
	API_CAPI_MSG_SIM_EFILE_DATA_REQ = ((P_log_api << 16) | 88),	///<Payload type {::T_SimApi_SubmitBinaryEFileReadReq}
	API_CAPI_MSG_SIM_SEND_EFILE_DATA_REQ = ((P_log_api << 16) | 89),	///<Payload type {::T_SimApi_SendBinaryEFileReadReq}
	API_CAPI_MSG_SIM_RECORD_EFILE_DATA_REQ = ((P_log_api << 16) | 90),	///<Payload type {::T_SimApi_SubmitRecordEFileReadReq}
	API_CAPI_MSG_SIM_SEND_RECORD_EFILE_DATA_REQ = ((P_log_api << 16) | 91),	///<Payload type {::T_SimApi_SendRecordEFileReadReq}
	API_CAPI_MSG_SIM_EFILE_UPDATE_REQ = ((P_log_api << 16) | 92),	///<Payload type {::T_SimApi_SubmitBinaryEFileUpdateReq}
	API_CAPI_MSG_SIM_SEND_EFILE_UPDATE_REQ = ((P_log_api << 16) | 93),	///<Payload type {::T_SimApi_SendBinaryEFileUpdateReq}
	API_CAPI_MSG_SIM_LINEAR_EFILE_UPDATE_REQ = ((P_log_api << 16) | 94),	///<Payload type {::T_SimApi_SubmitLinearEFileUpdateReq}
	API_CAPI_MSG_SIM_SEND_LINEAR_EFILE_UPDATE_REQ = ((P_log_api << 16) | 95),	///<Payload type {::T_SimApi_SendLinearEFileUpdateReq}
	API_CAPI_MSG_SIM_SEEK_RECORD_REQ = ((P_log_api << 16) | 96),	///<Payload type {::T_SimApi_SubmitSeekRecordReq}
	API_CAPI_MSG_SIM_SEND_SEEK_RECORD_REQ = ((P_log_api << 16) | 97),	///<Payload type {::T_SimApi_SendSeekRecordReq}
	API_CAPI_MSG_SIM_CYCLIC_EFILE_UPDATE_REQ = ((P_log_api << 16) | 98),	///<Payload type {::T_SimApi_SubmitCyclicEFileUpdateReq}
	API_CAPI_MSG_SIM_SEND_CYCLIC_EFILE_UPDATE_REQ = ((P_log_api << 16) | 99),	///<Payload type {::T_SimApi_SendCyclicEFileUpdateReq}
	API_CAPI_MSG_SIM_PIN_ATTEMPT_REQ = ((P_log_api << 16) | 100),	///<Payload type {::T_SimApi_SendRemainingPinAttemptReq}
	API_CAPI_MSG_SIM_CACHE_DATA_READY_REQ = ((P_log_api << 16) | 101),	///<Payload type {::T_SimApi_IsCachedDataReady}
	API_CAPI_MSG_SIM_SERVICE_CODE_STATUS_REQ = ((P_log_api << 16) | 102),	///<Payload type {::T_SimApi_GetServiceCodeStatus}
	API_CAPI_MSG_SIM_CHECK_CPHS_REQ = ((P_log_api << 16) | 103),	///<Payload type {::T_SimApi_CheckCphsService}
	API_CAPI_MSG_SIM_CPHS_PHASE_REQ = ((P_log_api << 16) | 104),	///<Payload type {::T_SimApi_GetCphsPhase}
	API_CAPI_MSG_SIM_SMS_SCA_REQ = ((P_log_api << 16) | 105),	///<Payload type {::T_SimApi_GetSmsSca}
	API_CAPI_MSG_SIM_ICCID_PARAM_REQ = ((P_log_api << 16) | 106),	///<Payload type {::T_SimApi_GetIccid}
	API_CAPI_MSG_SIM_ALS_STATUS_REQ = ((P_log_api << 16) | 107),	///<Payload type {::T_SimApi_IsALSEnabled}
	API_CAPI_MSG_SIM_ALS_DEFAULT_LINE_REQ = ((P_log_api << 16) | 108),	///<Payload type {::T_SimApi_GetAlsDefaultLine}
	API_CAPI_MSG_SIM_SET_ALS_DEFAULT_REQ = ((P_log_api << 16) | 109),	///<Payload type {::T_SimApi_SetAlsDefaultLine}
	API_CAPI_MSG_SIM_CALLFWD_COND_FLAG_REQ = ((P_log_api << 16) | 110),	///<Payload type {::T_SimApi_GetCallForwardUnconditionalFlag}
	API_CAPI_MSG_SIM_APP_TYPE_REQ = ((P_log_api << 16) | 111),	///<Payload type {::T_SimApi_GetApplicationType}
	API_CAPI_MSG_SIM_PUCT_UPDATE_REQ = ((P_log_api << 16) | 112),	///<Payload type {::T_SimApi_SendWritePuctReq}
	API_CAPI_MSG_SIM_RESTRICTED_ACCESS_REQ = ((P_log_api << 16) | 113),	///<Payload type {::T_SimApi_SubmitRestrictedAccessReq}
	API_CAPI_API_CAPI_MSG_DUMMY_114 = ((P_log_api << 16) | 114),
	API_CAPI_API_CAPI_MSG_DUMMY_115 = ((P_log_api << 16) | 115),
	API_CAPI_API_CAPI_MSG_DUMMY_116 = ((P_log_api << 16) | 116),
	API_CAPI_API_CAPI_MSG_DUMMY_117 = ((P_log_api << 16) | 117),
	API_CAPI_API_CAPI_MSG_DUMMY_118 = ((P_log_api << 16) | 118),
	API_CAPI_API_CAPI_MSG_DUMMY_119 = ((P_log_api << 16) | 119),
	API_CAPI_API_CAPI_MSG_DUMMY_120 = ((P_log_api << 16) | 120),
	API_CAPI_API_CAPI_MSG_DUMMY_121 = ((P_log_api << 16) | 121),
	API_CAPI_API_CAPI_MSG_DUMMY_122 = ((P_log_api << 16) | 122),
	API_CAPI_MSG_ADC_START_REQ = ((P_log_api << 16) | 123),	///<Payload type {::T_ADCMGR_Start}
	API_CAPI_API_CAPI_MSG_DUMMY_124 = ((P_log_api << 16) | 124),
	API_CAPI_API_CAPI_MSG_DUMMY_125 = ((P_log_api << 16) | 125),
	API_CAPI_API_CAPI_MSG_DUMMY_126 = ((P_log_api << 16) | 126),
	API_CAPI_API_CAPI_MSG_DUMMY_127 = ((P_log_api << 16) | 127),
	API_CAPI_API_CAPI_MSG_DUMMY_128 = ((P_log_api << 16) | 128),
	API_CAPI_API_CAPI_MSG_DUMMY_129 = ((P_log_api << 16) | 129),
	API_CAPI_API_CAPI_MSG_DUMMY_130 = ((P_log_api << 16) | 130),
	API_CAPI_API_CAPI_MSG_DUMMY_131 = ((P_log_api << 16) | 131),
	API_CAPI_API_CAPI_MSG_DUMMY_132 = ((P_log_api << 16) | 132),
	API_CAPI_API_CAPI_MSG_DUMMY_133 = ((P_log_api << 16) | 133),
	API_CAPI_MSG_SIM_PLMN_NUM_OF_ENTRY_REQ = ((P_log_api << 16) | 134),	///<Payload type {::T_SimApi_SendNumOfPLMNEntryReq}
	API_CAPI_MSG_SIM_PLMN_ENTRY_DATA_REQ = ((P_log_api << 16) | 135),	///<Payload type {::T_SimApi_SendReadPLMNEntryReq}
	API_CAPI_MSG_SIM_MUL_PLMN_ENTRY_UPDATE_REQ = ((P_log_api << 16) | 136),	///<Payload type {::T_SimApi_SendWriteMulPLMNEntryReq}
	API_CAPI_MSG_SYS_SET_REG_EVENT_MASK_REQ = ((P_log_api << 16) | 137),	///<Payload type {::T_SYS_SetRegisteredEventMask}
	API_CAPI_MSG_SYS_SET_REG_FILTER_MASK_REQ = ((P_log_api << 16) | 138),	///<Payload type {::T_SYS_SetFilteredEventMask}
	API_CAPI_MSG_SYS_SET_RSSI_THRESHOLD_REQ = ((P_log_api << 16) | 139),	///<Payload type {::T_PhoneCtrlApi_SetRssiThreshold}
	API_CAPI_API_CAPI_MSG_DUMMY_140 = ((P_log_api << 16) | 140),
	API_CAPI_API_CAPI_MSG_DUMMY_141 = ((P_log_api << 16) | 141),
	API_CAPI_API_CAPI_MSG_DUMMY_142 = ((P_log_api << 16) | 142),
	API_CAPI_MSG_SYS_SET_DARP_CFG_REQ = ((P_log_api << 16) | 143),	///<Payload type {::T_SYSPARM_SetDARPCfg}
	API_CAPI_MSG_SYSPARAM_SET_EGPRS_MSCLASS_REQ = ((P_log_api << 16) | 144),	///<Payload type {::T_SYSPARM_SetEGPRSMSClass}
	API_CAPI_MSG_SYSPARAM_SET_GPRS_MSCLASS_REQ = ((P_log_api << 16) | 145),	///<Payload type {::T_SYSPARM_SetGPRSMSClass}
	API_CAPI_MSG_TIMEZONE_DELETE_NW_NAME_REQ = ((P_log_api << 16) | 146),	///<Payload type {::T_NetRegApi_DeleteNetworkName}
	API_CAPI_MSG_CAPI2_TEST_REQ = ((P_log_api << 16) | 147),	///<Payload type {::T_TestCmds}
	API_CAPI_MSG_STK_SEND_PLAYTONE_RES_REQ = ((P_log_api << 16) | 148),	///<Payload type {::T_SatkApi_SendPlayToneRes}
	API_CAPI_MSG_STK_SETUP_CALL_RES_REQ = ((P_log_api << 16) | 149),	///<Payload type {::T_SATK_SendSetupCallRes}
	API_CAPI_MSG_PBK_SET_FDN_CHECK_REQ = ((P_log_api << 16) | 150),	///<Payload type {::T_PbkApi_SetFdnCheck}
	API_CAPI_MSG_PBK_GET_FDN_CHECK_REQ = ((P_log_api << 16) | 151),	///<Payload type {::T_PbkApi_GetFdnCheck}
	API_CAPI_MSG_PMU_BATT_LEVEL_REGISTER_REQ = ((P_log_api << 16) | 152),	///<Payload type {::T_PMU_Battery_Register}
	API_CAPI_API_CAPI_MSG_DUMMY_153 = ((P_log_api << 16) | 153),
	API_CAPI_MSG_SMS_SEND_MEM_AVAL_IND_REQ = ((P_log_api << 16) | 154),	///<Payload type {::T_SmsApi_SendMemAvailInd}
	API_CAPI_MSG_SMS_CONFIGUREMESTORAGE_REQ = ((P_log_api << 16) | 155),	///<Payload type {::T_SMS_ConfigureMEStorage}
	API_CAPI_API_CAPI_MSG_DUMMY_156 = ((P_log_api << 16) | 156),
	API_CAPI_API_CAPI_MSG_DUMMY_157 = ((P_log_api << 16) | 157),
	API_CAPI_MSG_USIM_IS_APP_SUPPORTED_REQ = ((P_log_api << 16) | 158),	///<Payload type {::T_USimApi_IsApplicationSupported}
	API_CAPI_MSG_USIM_IS_APN_ALLOWED_REQ = ((P_log_api << 16) | 159),	///<Payload type {::T_USimApi_IsAllowedAPN}
	API_CAPI_MSG_USIM_GET_NUM_APN_REQ = ((P_log_api << 16) | 160),	///<Payload type {::T_USimApi_GetNumOfAPN}
	API_CAPI_MSG_USIM_GET_APN_ENTRY_REQ = ((P_log_api << 16) | 161),	///<Payload type {::T_USimApi_GetAPNEntry}
	API_CAPI_MSG_USIM_IS_EST_SERV_ACTIVATED_REQ = ((P_log_api << 16) | 162),	///<Payload type {::T_USimApi_IsEstServActivated}
	API_CAPI_MSG_SIM_SET_EST_SERV_REQ = ((P_log_api << 16) | 163),	///<Payload type {::T_USimApi_SendSetEstServReq}
	API_CAPI_MSG_SIM_UPDATE_ONE_APN_REQ = ((P_log_api << 16) | 164),	///<Payload type {::T_USimApi_SendWriteAPNReq}
	API_CAPI_MSG_SIM_DELETE_ALL_APN_REQ = ((P_log_api << 16) | 165),	///<Payload type {::T_USimApi_SendDeleteAllAPNReq}
	API_CAPI_MSG_USIM_GET_RAT_MODE_REQ = ((P_log_api << 16) | 166),	///<Payload type {::T_USimApi_GetRatModeSetting}
	API_CAPI_API_CAPI_MSG_DUMMY_167 = ((P_log_api << 16) | 167),
	API_CAPI_API_CAPI_MSG_DUMMY_168 = ((P_log_api << 16) | 168),
	API_CAPI_API_CAPI_MSG_DUMMY_169 = ((P_log_api << 16) | 169),
	API_CAPI_MSG_MS_SETMEPOWER_CLASS_REQ = ((P_log_api << 16) | 170),	///<Payload type {::T_MS_SetMEPowerClass}
	API_CAPI_MSG_USIM_GET_SERVICE_STATUS_REQ = ((P_log_api << 16) | 171),	///<Payload type {::T_USimApi_GetServiceStatus}
	API_CAPI_MSG_SIM_IS_ALLOWED_APN_REQ = ((P_log_api << 16) | 172),	///<Payload type {::T_SimApi_IsAllowedAPN}
	API_CAPI_MSG_SMS_GETSMSMAXCAPACITY_REQ = ((P_log_api << 16) | 173),	///<Payload type {::T_SmsApi_GetSmsMaxCapacity}
	API_CAPI_MSG_SMS_RETRIEVEMAXCBCHNLLENGTH_REQ = ((P_log_api << 16) | 174),	///<Payload type {::T_SmsApi_RetrieveMaxCBChnlLength}
	API_CAPI_MSG_SIM_IS_BDN_RESTRICTED_REQ = ((P_log_api << 16) | 175),	///<Payload type {::T_SimApi_IsBdnOperationRestricted}
	API_CAPI_MSG_SIM_SEND_PLMN_UPDATE_IND_REQ = ((P_log_api << 16) | 176),	///<Payload type {::T_SimApi_SendPreferredPlmnUpdateInd}
	API_CAPI_API_CAPI_MSG_DUMMY_177 = ((P_log_api << 16) | 177),
	API_CAPI_MSG_SIM_SET_BDN_REQ = ((P_log_api << 16) | 178),	///<Payload type {::T_SimApi_SendSetBdnReq}
	API_CAPI_MSG_SIM_POWER_ON_OFF_CARD_REQ = ((P_log_api << 16) | 179),	///<Payload type {::T_SimApi_PowerOnOffCard}
	API_CAPI_MSG_SIM_GET_RAW_ATR_REQ = ((P_log_api << 16) | 180),	///<Payload type {::T_SimApi_GetRawAtr}
	API_CAPI_MSG_SIM_SET_PROTOCOL_REQ = ((P_log_api << 16) | 181),	///<Payload type {::T_SimApi_Set_Protocol}
	API_CAPI_MSG_SIM_GET_PROTOCOL_REQ = ((P_log_api << 16) | 182),	///<Payload type {::T_SimApi_Get_Protocol}
	API_CAPI_MSG_SIM_SEND_GENERIC_APDU_CMD_REQ = ((P_log_api << 16) | 183),	///<Payload type {::T_SimApi_SendGenericApduCmd}
	API_CAPI_MSG_SIM_TERMINATE_XFER_APDU_REQ = ((P_log_api << 16) | 184),	///<Payload type {::T_SimApi_TerminateXferApdu}
	API_CAPI_API_CAPI_MSG_DUMMY_185 = ((P_log_api << 16) | 185),
	API_CAPI_MSG_SET_PLMN_SELECT_RAT_REQ = ((P_log_api << 16) | 186),	///<Payload type {::T_NetRegApi_SetPlmnSelectRat}
	API_CAPI_API_CAPI_MSG_DUMMY_187 = ((P_log_api << 16) | 187),
	API_CAPI_API_CAPI_MSG_DUMMY_188 = ((P_log_api << 16) | 188),
	API_CAPI_MSG_READ_USIM_PBK_HDK_REQ = ((P_log_api << 16) | 189),	///<Payload type {::T_PbkApi_SendUsimHdkReadReq}
	API_CAPI_MSG_WRITE_USIM_PBK_HDK_REQ = ((P_log_api << 16) | 190),	///<Payload type {::T_PbkApi_SendUsimHdkUpdateReq}
	API_CAPI_MSG_READ_USIM_PBK_ALPHA_AAS_REQ = ((P_log_api << 16) | 191),	///<Payload type {::T_PbkApi_SendUsimAasReadReq}
	API_CAPI_MSG_WRITE_USIM_PBK_ALPHA_AAS_REQ = ((P_log_api << 16) | 192),	///<Payload type {::T_PbkApi_SendUsimAasUpdateReq}
	API_CAPI_MSG_READ_USIM_PBK_ALPHA_GAS_REQ = ((P_log_api << 16) | 193),	///<Payload type {::T_PbkApi_SendUsimGasReadReq}
	API_CAPI_MSG_WRITE_USIM_PBK_ALPHA_GAS_REQ = ((P_log_api << 16) | 194),	///<Payload type {::T_PbkApi_SendUsimGasUpdateReq}
	API_CAPI_MSG_GET_USIM_PBK_ALPHA_INFO_AAS_REQ = ((P_log_api << 16) | 195),	///<Payload type {::T_PbkApi_SendUsimAasInfoReq}
	API_CAPI_MSG_GET_USIM_PBK_ALPHA_INFO_GAS_REQ = ((P_log_api << 16) | 196),	///<Payload type {::T_PbkApi_SendUsimGasInfoReq}
	API_CAPI_API_CAPI_MSG_DUMMY_197 = ((P_log_api << 16) | 197),
	API_CAPI_MSG_DIAG_MEASURE_REPORT_REQ = ((P_log_api << 16) | 198),	///<Payload type {::T_DiagApi_MeasurmentReportReq}
	API_CAPI_MSG_PMU_BATT_CHARGING_NOTIFICATION_REQ = ((P_log_api << 16) | 199),	///<Payload type {::T_PMU_BattChargingNotification}
	API_CAPI_MSG_MS_INITCALLCFG_REQ = ((P_log_api << 16) | 200),	///<Payload type {::T_MsDbApi_InitCallCfg}
	API_CAPI_API_CAPI_MSG_DUMMY_201 = ((P_log_api << 16) | 201),
	API_CAPI_API_CAPI_MSG_DUMMY_202 = ((P_log_api << 16) | 202),
	API_CAPI_API_CAPI_MSG_DUMMY_203 = ((P_log_api << 16) | 203),
	API_CAPI_API_CAPI_MSG_DUMMY_204 = ((P_log_api << 16) | 204),
	API_CAPI_API_CAPI_MSG_DUMMY_205 = ((P_log_api << 16) | 205),
	API_CAPI_API_CAPI_MSG_DUMMY_206 = ((P_log_api << 16) | 206),
	API_CAPI_API_CAPI_MSG_DUMMY_207 = ((P_log_api << 16) | 207),
	API_CAPI_MSG_SMS_SEND_COMMAND_TXT_REQ = ((P_log_api << 16) | 208),	///<Payload type {::T_SmsApi_SendSMSCommandTxtReq}
	API_CAPI_MSG_SMS_SEND_COMMAND_PDU_REQ = ((P_log_api << 16) | 209),	///<Payload type {::T_SmsApi_SendSMSCommandPduReq}
	API_CAPI_MSG_SMS_SEND_ACKTONETWORK_PDU_REQ = ((P_log_api << 16) | 210),	///<Payload type {::T_SmsApi_SendPDUAckToNetwork}
	API_CAPI_MSG_SMS_CB_START_STOP_REQ = ((P_log_api << 16) | 211),	///<Payload type {::T_SmsApi_StartCellBroadcastWithChnlReq}
	API_CAPI_MSG_SMS_SET_TPMR_REQ = ((P_log_api << 16) | 212),	///<Payload type {::T_SmsApi_SetMoSmsTpMr}
	API_CAPI_MSG_SIMLOCK_SET_STATUS_REQ = ((P_log_api << 16) | 213),	///<Payload type {::T_SIMLOCKApi_SetStatus}
	API_CAPI_API_CAPI_MSG_DUMMY_214 = ((P_log_api << 16) | 214),
	API_CAPI_MSG_DIAG_CELLLOCK_REQ = ((P_log_api << 16) | 215),	///<Payload type {::T_DIAG_ApiCellLockReq}
	API_CAPI_API_CAPI_MSG_DUMMY_216 = ((P_log_api << 16) | 216),
	API_CAPI_MSG_MS_SET_RUA_READY_TIMER_REQ = ((P_log_api << 16) | 217),	///<Payload type {::T_MS_SetRuaReadyTimer}
	API_CAPI_MSG_LCS_REG_RRLP_HDL_REQ = ((P_log_api << 16) | 218),	///<Payload type {::T_LCS_RegisterRrlpDataHandler}
	API_CAPI_API_CAPI_MSG_DUMMY_219 = ((P_log_api << 16) | 219),
	API_CAPI_API_CAPI_MSG_DUMMY_220 = ((P_log_api << 16) | 220),
	API_CAPI_MSG_LCS_REG_RRC_HDL_REQ = ((P_log_api << 16) | 221),	///<Payload type {::T_LCS_RegisterRrcDataHandler}
	API_CAPI_API_CAPI_MSG_DUMMY_222 = ((P_log_api << 16) | 222),
	API_CAPI_API_CAPI_MSG_DUMMY_223 = ((P_log_api << 16) | 223),
	API_CAPI_API_CAPI_MSG_DUMMY_224 = ((P_log_api << 16) | 224),
	API_CAPI_API_CAPI_MSG_DUMMY_225 = ((P_log_api << 16) | 225),
	API_CAPI_API_CAPI_MSG_DUMMY_226 = ((P_log_api << 16) | 226),
	API_CAPI_API_CAPI_MSG_DUMMY_227 = ((P_log_api << 16) | 227),
	API_CAPI_MSG_CC_IS_THERE_EMERGENCY_CALL_REQ = ((P_log_api << 16) | 228),	///<Payload type {::T_CcApi_IsThereEmergencyCall}
	API_CAPI_API_CAPI_MSG_DUMMY_229 = ((P_log_api << 16) | 229),
	API_CAPI_MSG_SYS_ENABLE_CELL_INFO_REQ = ((P_log_api << 16) | 230),	///<Payload type {::T_SYS_EnableCellInfoMsg}
	API_CAPI_MSG_L1_BB_ISLOCKED_REQ = ((P_log_api << 16) | 231),	///<Payload type {::T_LCS_L1_bb_isLocked}
	API_CAPI_API_CAPI_MSG_DUMMY_232 = ((P_log_api << 16) | 232),
	API_CAPI_MSG_UTIL_DIAL_STR_PARSE_REQ = ((P_log_api << 16) | 233),	///<Payload type {::T_DIALSTR_ParseGetCallType}
	API_CAPI_MSG_LCS_FTT_DELTA_TIME_REQ = ((P_log_api << 16) | 234),	///<Payload type {::T_LCS_FttCalcDeltaTime}
	API_CAPI_API_CAPI_MSG_DUMMY_235 = ((P_log_api << 16) | 235),
	API_CAPI_MSG_MS_FORCEDREADYSTATE_REQ = ((P_log_api << 16) | 236),	///<Payload type {::T_NetRegApi_ForcedReadyStateReq}
	API_CAPI_API_CAPI_MSG_DUMMY_237 = ((P_log_api << 16) | 237),
	API_CAPI_MSG_SIMLOCK_GET_STATUS_REQ = ((P_log_api << 16) | 238),	///<Payload type {::T_SIMLOCK_GetStatus}
	API_CAPI_API_CAPI_MSG_DUMMY_239 = ((P_log_api << 16) | 239),
	API_CAPI_MSG_DIALSTR_IS_VALID_REQ = ((P_log_api << 16) | 240),	///<Payload type {::T_DIALSTR_IsValidString}
	API_CAPI_MSG_UTIL_CONVERT_NTWK_CAUSE_REQ = ((P_log_api << 16) | 241),	///<Payload type {::T_UTIL_Cause2NetworkCause}
	API_CAPI_MSG_UTIL_CONVERT_ERRCODE_NTWK_CAUSE_REQ = ((P_log_api << 16) | 242),	///<Payload type {::T_UTIL_ErrCodeToNetCause}
	API_CAPI_MSG_ISGPRS_DIAL_STR_REQ = ((P_log_api << 16) | 243),	///<Payload type {::T_IsGprsDialStr}
	API_CAPI_MSG_GET_NUM_SS_STR_REQ = ((P_log_api << 16) | 244),	///<Payload type {::T_UTIL_GetNumOffsetInSsStr}
	API_CAPI_API_CAPI_MSG_DUMMY_245 = ((P_log_api << 16) | 245),
	API_CAPI_API_CAPI_MSG_DUMMY_246 = ((P_log_api << 16) | 246),
	API_CAPI_MSG_DIALSTR_IS_PPPLOOPBACK_REQ = ((P_log_api << 16) | 247),	///<Payload type {::T_IsPppLoopbackDialStr}
	API_CAPI_API_CAPI_MSG_DUMMY_248 = ((P_log_api << 16) | 248),
	API_CAPI_MSG_SYSPARM_SET_HSDPA_PHY_CAT_REQ = ((P_log_api << 16) | 249),	///<Payload type {::T_SYSPARM_SetHSDPAPHYCategory}
	API_CAPI_API_CAPI_MSG_DUMMY_250 = ((P_log_api << 16) | 250),
	API_CAPI_MSG_SMS_CONVERT_MSGTYPE_REQ = ((P_log_api << 16) | 251),	///<Payload type {::T_SmsApi_ConvertSmsMSMsgType}
	API_CAPI_API_CAPI_MSG_DUMMY_252 = ((P_log_api << 16) | 252),
	API_CAPI_API_CAPI_MSG_DUMMY_253 = ((P_log_api << 16) | 253),
	API_CAPI_API_CAPI_MSG_DUMMY_254 = ((P_log_api << 16) | 254),
	API_CAPI_API_CAPI_MSG_DUMMY_255 = ((P_log_api << 16) | 255),
	API_CAPI_API_CAPI_MSG_DUMMY_256 = ((P_log_api << 16) | 256),
	API_CAPI_API_CAPI_MSG_DUMMY_257 = ((P_log_api << 16) | 257),
	API_CAPI_API_CAPI_MSG_DUMMY_258 = ((P_log_api << 16) | 258),
	API_CAPI_API_CAPI_MSG_DUMMY_259 = ((P_log_api << 16) | 259),
	API_CAPI_MSG_SYSPARM_SET_HSUPA_PHY_CAT_REQ = ((P_log_api << 16) | 260),	///<Payload type {::T_SYSPARM_SetHSUPAPHYCategory}
	API_CAPI_API_CAPI_MSG_DUMMY_261 = ((P_log_api << 16) | 261),
	API_CAPI_MSG_INTERTASK_MSG_TO_CP_REQ = ((P_log_api << 16) | 262),	///<Payload type {::T_InterTaskMsgToCP}
	API_CAPI_MSG_INTERTASK_MSG_TO_AP_REQ = ((P_log_api << 16) | 263),	///<Payload type {::T_InterTaskMsgToAP}
	API_CAPI_API_CAPI_MSG_DUMMY_264 = ((P_log_api << 16) | 264),
	API_CAPI_MSG_CC_GETCURRENTCALLINDEX_REQ = ((P_log_api << 16) | 265),	///<Payload type {::T_CcApi_GetCurrentCallIndex}
	API_CAPI_MSG_CC_GETNEXTACTIVECALLINDEX_REQ = ((P_log_api << 16) | 266),	///<Payload type {::T_CcApi_GetNextActiveCallIndex}
	API_CAPI_MSG_CC_GETNEXTHELDCALLINDEX_REQ = ((P_log_api << 16) | 267),	///<Payload type {::T_CcApi_GetNextHeldCallIndex}
	API_CAPI_MSG_CC_GETNEXTWAITCALLINDEX_REQ = ((P_log_api << 16) | 268),	///<Payload type {::T_CcApi_GetNextWaitCallIndex}
	API_CAPI_MSG_CC_GETMPTYCALLINDEX_REQ = ((P_log_api << 16) | 269),	///<Payload type {::T_CcApi_GetMPTYCallIndex}
	API_CAPI_MSG_CC_GETCALLSTATE_REQ = ((P_log_api << 16) | 270),	///<Payload type {::T_CcApi_GetCallState}
	API_CAPI_MSG_CC_GETCALLTYPE_REQ = ((P_log_api << 16) | 271),	///<Payload type {::T_CcApi_GetCallType}
	API_CAPI_MSG_CC_GETLASTCALLEXITCAUSE_REQ = ((P_log_api << 16) | 272),	///<Payload type {::T_CcApi_GetLastCallExitCause}
	API_CAPI_MSG_CC_GETCALLNUMBER_REQ = ((P_log_api << 16) | 273),	///<Payload type {::T_CcApi_GetCallNumber}
	API_CAPI_MSG_CC_GETCALLINGINFO_REQ = ((P_log_api << 16) | 274),	///<Payload type {::T_CcApi_GetCallingInfo}
	API_CAPI_MSG_CC_GETALLCALLSTATES_REQ = ((P_log_api << 16) | 275),	///<Payload type {::T_CcApi_GetAllCallStates}
	API_CAPI_MSG_CC_GETALLCALLINDEX_REQ = ((P_log_api << 16) | 276),	///<Payload type {::T_CcApi_GetAllCallIndex}
	API_CAPI_MSG_CC_GETALLHELDCALLINDEX_REQ = ((P_log_api << 16) | 277),	///<Payload type {::T_CcApi_GetAllHeldCallIndex}
	API_CAPI_MSG_CC_GETALLACTIVECALLINDEX_REQ = ((P_log_api << 16) | 278),	///<Payload type {::T_CcApi_GetAllActiveCallIndex}
	API_CAPI_MSG_CC_GETALLMPTYCALLINDEX_REQ = ((P_log_api << 16) | 279),	///<Payload type {::T_CcApi_GetAllMPTYCallIndex}
	API_CAPI_MSG_CC_GETNUMOFMPTYCALLS_REQ = ((P_log_api << 16) | 280),	///<Payload type {::T_CcApi_GetNumOfMPTYCalls}
	API_CAPI_MSG_CC_GETNUMOFACTIVECALLS_REQ = ((P_log_api << 16) | 281),	///<Payload type {::T_CcApi_GetNumofActiveCalls}
	API_CAPI_MSG_CC_GETNUMOFHELDCALLS_REQ = ((P_log_api << 16) | 282),	///<Payload type {::T_CcApi_GetNumofHeldCalls}
	API_CAPI_MSG_CC_ISTHEREWAITINGCALL_REQ = ((P_log_api << 16) | 283),	///<Payload type {::T_CcApi_IsThereWaitingCall}
	API_CAPI_MSG_CC_ISTHEREALERTINGCALL_REQ = ((P_log_api << 16) | 284),	///<Payload type {::T_CcApi_IsThereAlertingCall}
	API_CAPI_MSG_CC_GETCONNECTEDLINEID_REQ = ((P_log_api << 16) | 285),	///<Payload type {::T_CcApi_GetConnectedLineID}
	API_CAPI_MSG_CC_GET_CALL_PRESENT_REQ = ((P_log_api << 16) | 286),	///<Payload type {::T_CcApi_GetCallPresent}
	API_CAPI_MSG_CC_GET_INDEX_STATE_REQ = ((P_log_api << 16) | 287),	///<Payload type {::T_CcApi_GetCallIndexInThisState}
	API_CAPI_MSG_CC_ISMULTIPARTYCALL_REQ = ((P_log_api << 16) | 288),	///<Payload type {::T_CcApi_IsMultiPartyCall}
	API_CAPI_MSG_CC_ISTHEREVOICECALL_REQ = ((P_log_api << 16) | 289),	///<Payload type {::T_CcApi_IsThereVoiceCall}
	API_CAPI_MSG_CC_ISCONNECTEDLINEIDPRESENTALLOWED_REQ = ((P_log_api << 16) | 290),	///<Payload type {::T_CcApi_IsConnectedLineIDPresentAllowed}
	API_CAPI_MSG_CC_GETCURRENTCALLDURATIONINMILLISECONDS_REQ = ((P_log_api << 16) | 291),	///<Payload type {::T_CcApi_GetCurrentCallDurationInMilliSeconds}
	API_CAPI_MSG_CC_GETLASTCALLCCM_REQ = ((P_log_api << 16) | 292),	///<Payload type {::T_CcApi_GetLastCallCCM}
	API_CAPI_MSG_CC_GETLASTCALLDURATION_REQ = ((P_log_api << 16) | 293),	///<Payload type {::T_CcApi_GetLastCallDuration}
	API_CAPI_MSG_CC_GETLASTDATACALLRXBYTES_REQ = ((P_log_api << 16) | 294),	///<Payload type {::T_CcApi_GetLastDataCallRxBytes}
	API_CAPI_MSG_CC_GETLASTDATACALLTXBYTES_REQ = ((P_log_api << 16) | 295),	///<Payload type {::T_CcApi_GetLastDataCallTxBytes}
	API_CAPI_MSG_CC_GETDATACALLINDEX_REQ = ((P_log_api << 16) | 296),	///<Payload type {::T_CcApi_GetDataCallIndex}
	API_CAPI_MSG_CC_GETCALLCLIENT_INFO_REQ = ((P_log_api << 16) | 297),	///<Payload type {::T_CcApi_GetCallClientInfo}
	API_CAPI_MSG_CC_GETCALLCLIENTID_REQ = ((P_log_api << 16) | 298),	///<Payload type {::T_CcApi_GetCallClientID}
	API_CAPI_MSG_CC_GETTYPEADD_REQ = ((P_log_api << 16) | 299),	///<Payload type {::T_CcApi_GetTypeAdd}
	API_CAPI_MSG_CC_SETVOICECALLAUTOREJECT_REQ = ((P_log_api << 16) | 300),	///<Payload type {::T_CcApi_SetVoiceCallAutoReject}
	API_CAPI_MSG_CC_ISVOICECALLAUTOREJECT_REQ = ((P_log_api << 16) | 301),	///<Payload type {::T_CcApi_IsVoiceCallAutoReject}
	API_CAPI_MSG_CC_SETTTYCALL_REQ = ((P_log_api << 16) | 302),	///<Payload type {::T_CcApi_SetTTYCall}
	API_CAPI_MSG_CC_ISTTYENABLE_REQ = ((P_log_api << 16) | 303),	///<Payload type {::T_CcApi_IsTTYEnable}
	API_CAPI_MSG_CC_ISSIMORIGINEDCALL_REQ = ((P_log_api << 16) | 304),	///<Payload type {::T_CcApi_IsSimOriginedCall}
	API_CAPI_MSG_CC_SETVIDEOCALLPARAM_REQ = ((P_log_api << 16) | 305),	///<Payload type {::T_CcApi_SetVideoCallParam}
	API_CAPI_MSG_CC_GETVIDEOCALLPARAM_REQ = ((P_log_api << 16) | 306),	///<Payload type {::T_CcApi_GetVideoCallParam}
	API_CAPI_MSG_CC_GETCCM_REQ = ((P_log_api << 16) | 307),	///<Payload type {::T_CcApi_GetCCM}
	API_CAPI_MSG_CCAPI_SENDDTMF_REQ = ((P_log_api << 16) | 308),	///<Payload type {::T_CcApi_SendDtmfTone}
	API_CAPI_MSG_CCAPI_STOPDTMF_REQ = ((P_log_api << 16) | 309),	///<Payload type {::T_CcApi_StopDtmfTone}
	API_CAPI_MSG_CCAPI_ABORTDTMF_REQ = ((P_log_api << 16) | 310),	///<Payload type {::T_CcApi_AbortDtmf}
	API_CAPI_MSG_CCAPI_SETDTMFTIMER_REQ = ((P_log_api << 16) | 311),	///<Payload type {::T_CcApi_SetDtmfToneTimer}
	API_CAPI_MSG_CCAPI_RESETDTMFTIMER_REQ = ((P_log_api << 16) | 312),	///<Payload type {::T_CcApi_ResetDtmfToneTimer}
	API_CAPI_MSG_CCAPI_GETDTMFTIMER_REQ = ((P_log_api << 16) | 313),	///<Payload type {::T_CcApi_GetDtmfToneTimer}
	API_CAPI_MSG_CCAPI_GETTIFROMCALLINDEX_REQ = ((P_log_api << 16) | 314),	///<Payload type {::T_CcApi_GetTiFromCallIndex}
	API_CAPI_MSG_CCAPI_IS_SUPPORTEDBC_REQ = ((P_log_api << 16) | 315),	///<Payload type {::T_CcApi_IsSupportedBC}
	API_CAPI_MSG_CCAPI_IS_BEARER_CAPABILITY_REQ = ((P_log_api << 16) | 316),	///<Payload type {::T_CcApi_GetBearerCapability}
	API_CAPI_MSG_SMS_SENDSMSSRVCENTERNUMBERUPDATE_REQ = ((P_log_api << 16) | 317),	///<Payload type {::T_SmsApi_SendSMSSrvCenterNumberUpdateReq}
	API_CAPI_MSG_SMS_GETSMSSRVCENTERNUMBER_REQ = ((P_log_api << 16) | 318),	///<Payload type {::T_SmsApi_GetSMSrvCenterNumber}
	API_CAPI_MSG_SMS_GETSIMSMSCAPACITYEXCEEDEDFLAG_REQ = ((P_log_api << 16) | 319),	///<Payload type {::T_SmsApi_GetSIMSMSCapacityExceededFlag}
	API_CAPI_MSG_SMS_ISSMSSERVICEAVAIL_REQ = ((P_log_api << 16) | 320),	///<Payload type {::T_SmsApi_IsSmsServiceAvail}
	API_CAPI_MSG_SMS_GETSMSSTOREDSTATE_REQ = ((P_log_api << 16) | 321),	///<Payload type {::T_SmsApi_GetSmsStoredState}
	API_CAPI_MSG_SMS_WRITESMSPDU_REQ = ((P_log_api << 16) | 322),	///<Payload type {::T_SmsApi_WriteSMSPduReq}
	API_CAPI_MSG_SMS_WRITESMSREQ_REQ = ((P_log_api << 16) | 323),	///<Payload type {::T_SmsApi_WriteSMSReq}
	API_CAPI_MSG_SMS_SENDSMSREQ_REQ = ((P_log_api << 16) | 324),	///<Payload type {::T_SmsApi_SendSMSReq}
	API_CAPI_MSG_SMS_SENDSMSPDUREQ_REQ = ((P_log_api << 16) | 325),	///<Payload type {::T_SmsApi_SendSMSPduReq}
	API_CAPI_MSG_SMS_SENDSTOREDSMSREQ_REQ = ((P_log_api << 16) | 326),	///<Payload type {::T_SmsApi_SendStoredSmsReq}
	API_CAPI_MSG_SMS_WRITESMSPDUTOSIM_REQ = ((P_log_api << 16) | 327),	///<Payload type {::T_SmsApi_WriteSMSPduToSIMReq}
	API_CAPI_MSG_SMS_GETLASTTPMR_REQ = ((P_log_api << 16) | 328),	///<Payload type {::T_SmsApi_GetLastTpMr}
	API_CAPI_MSG_SMS_GETSMSTXPARAMS_REQ = ((P_log_api << 16) | 329),	///<Payload type {::T_SmsApi_GetSmsTxParams}
	API_CAPI_MSG_SMS_GETTXPARAMINTEXTMODE_REQ = ((P_log_api << 16) | 330),	///<Payload type {::T_SmsApi_GetTxParamInTextMode}
	API_CAPI_MSG_SMS_SETSMSTXPARAMPROCID_REQ = ((P_log_api << 16) | 331),	///<Payload type {::T_SmsApi_SetSmsTxParamProcId}
	API_CAPI_MSG_SMS_SETSMSTXPARAMCODINGTYPE_REQ = ((P_log_api << 16) | 332),	///<Payload type {::T_SmsApi_SetSmsTxParamCodingType}
	API_CAPI_MSG_SMS_SETSMSTXPARAMVALIDPERIOD_REQ = ((P_log_api << 16) | 333),	///<Payload type {::T_SmsApi_SetSmsTxParamValidPeriod}
	API_CAPI_MSG_SMS_SETSMSTXPARAMCOMPRESSION_REQ = ((P_log_api << 16) | 334),	///<Payload type {::T_SmsApi_SetSmsTxParamCompression}
	API_CAPI_MSG_SMS_SETSMSTXPARAMREPLYPATH_REQ = ((P_log_api << 16) | 335),	///<Payload type {::T_SmsApi_SetSmsTxParamReplyPath}
	API_CAPI_MSG_SMS_SETSMSTXPARAMUSERDATAHDRIND_REQ = ((P_log_api << 16) | 336),	///<Payload type {::T_SmsApi_SetSmsTxParamUserDataHdrInd}
	API_CAPI_MSG_SMS_SETSMSTXPARAMSTATUSRPTREQFLAG_REQ = ((P_log_api << 16) | 337),	///<Payload type {::T_SmsApi_SetSmsTxParamStatusRptReqFlag}
	API_CAPI_MSG_SMS_SETSMSTXPARAMREJDUPL_REQ = ((P_log_api << 16) | 338),	///<Payload type {::T_SmsApi_SetSmsTxParamRejDupl}
	API_CAPI_MSG_SMS_DELETESMSMSGBYINDEX_REQ = ((P_log_api << 16) | 339),	///<Payload type {::T_SmsApi_DeleteSmsMsgByIndexReq}
	API_CAPI_MSG_SMS_READSMSMSG_REQ = ((P_log_api << 16) | 340),	///<Payload type {::T_SmsApi_ReadSmsMsgReq}
	API_CAPI_MSG_SMS_LISTSMSMSG_REQ = ((P_log_api << 16) | 341),	///<Payload type {::T_SmsApi_ListSmsMsgReq}
	API_CAPI_MSG_SMS_SETNEWMSGDISPLAYPREF_REQ = ((P_log_api << 16) | 342),	///<Payload type {::T_SmsApi_SetNewMsgDisplayPref}
	API_CAPI_MSG_SMS_GETNEWMSGDISPLAYPREF_REQ = ((P_log_api << 16) | 343),	///<Payload type {::T_SmsApi_GetNewMsgDisplayPref}
	API_CAPI_MSG_SMS_SETSMSPREFSTORAGE_REQ = ((P_log_api << 16) | 344),	///<Payload type {::T_SmsApi_SetSMSPrefStorage}
	API_CAPI_MSG_SMS_GETSMSPREFSTORAGE_REQ = ((P_log_api << 16) | 345),	///<Payload type {::T_SmsApi_GetSMSPrefStorage}
	API_CAPI_MSG_SMS_GETSMSSTORAGESTATUS_REQ = ((P_log_api << 16) | 346),	///<Payload type {::T_SmsApi_GetSMSStorageStatus}
	API_CAPI_MSG_SMS_SAVESMSSERVICEPROFILE_REQ = ((P_log_api << 16) | 347),	///<Payload type {::T_SmsApi_SaveSmsServiceProfile}
	API_CAPI_MSG_SMS_RESTORESMSSERVICEPROFILE_REQ = ((P_log_api << 16) | 348),	///<Payload type {::T_SmsApi_RestoreSmsServiceProfile}
	API_CAPI_MSG_SMS_SETCELLBROADCASTMSGTYPE_REQ = ((P_log_api << 16) | 349),	///<Payload type {::T_SmsApi_SetCellBroadcastMsgTypeReq}
	API_CAPI_MSG_SMS_CBALLOWALLCHNLREQ_REQ = ((P_log_api << 16) | 350),	///<Payload type {::T_SmsApi_CBAllowAllChnlReq}
	API_CAPI_MSG_SMS_ADDCELLBROADCASTCHNLREQ_REQ = ((P_log_api << 16) | 351),	///<Payload type {::T_SmsApi_AddCellBroadcastChnlReq}
	API_CAPI_MSG_SMS_REMOVECELLBROADCASTCHNLREQ_REQ = ((P_log_api << 16) | 352),	///<Payload type {::T_SmsApi_RemoveCellBroadcastChnlReq}
	API_CAPI_MSG_SMS_REMOVEALLCBCHNLFROMSEARCHLIST_REQ = ((P_log_api << 16) | 353),	///<Payload type {::T_SmsApi_RemoveAllCBChnlFromSearchList}
	API_CAPI_MSG_SMS_GETCBMI_REQ = ((P_log_api << 16) | 354),	///<Payload type {::T_SmsApi_GetCBMI}
	API_CAPI_MSG_SMS_GETCBLANGUAGE_REQ = ((P_log_api << 16) | 355),	///<Payload type {::T_SmsApi_GetCbLanguage}
	API_CAPI_MSG_SMS_ADDCELLBROADCASTLANGREQ_REQ = ((P_log_api << 16) | 356),	///<Payload type {::T_SmsApi_AddCellBroadcastLangReq}
	API_CAPI_MSG_SMS_REMOVECELLBROADCASTLANGREQ_REQ = ((P_log_api << 16) | 357),	///<Payload type {::T_SmsApi_RemoveCellBroadcastLangReq}
	API_CAPI_MSG_SMS_STARTRECEIVINGCELLBROADCAST_REQ = ((P_log_api << 16) | 358),	///<Payload type {::T_SmsApi_StartReceivingCellBroadcastReq}
	API_CAPI_MSG_SMS_STOPRECEIVINGCELLBROADCAST_REQ = ((P_log_api << 16) | 359),	///<Payload type {::T_SmsApi_StopReceivingCellBroadcastReq}
	API_CAPI_MSG_SMS_SETCBIGNOREDUPLFLAG_REQ = ((P_log_api << 16) | 360),	///<Payload type {::T_SmsApi_SetCBIgnoreDuplFlag}
	API_CAPI_MSG_SMS_GETCBIGNOREDUPLFLAG_REQ = ((P_log_api << 16) | 361),	///<Payload type {::T_SmsApi_GetCBIgnoreDuplFlag}
	API_CAPI_MSG_SMS_SETVMINDONOFF_REQ = ((P_log_api << 16) | 362),	///<Payload type {::T_SmsApi_SetVMIndOnOff}
	API_CAPI_MSG_SMS_ISVMINDENABLED_REQ = ((P_log_api << 16) | 363),	///<Payload type {::T_SmsApi_IsVMIndEnabled}
	API_CAPI_MSG_SMS_GETVMWAITINGSTATUS_REQ = ((P_log_api << 16) | 364),	///<Payload type {::T_SmsApi_GetVMWaitingStatus}
	API_CAPI_MSG_SMS_GETNUMOFVMSCNUMBER_REQ = ((P_log_api << 16) | 365),	///<Payload type {::T_SmsApi_GetNumOfVmscNumber}
	API_CAPI_MSG_SMS_GETVMSCNUMBER_REQ = ((P_log_api << 16) | 366),	///<Payload type {::T_SmsApi_GetVmscNumber}
	API_CAPI_MSG_SMS_UPDATEVMSCNUMBERREQ_REQ = ((P_log_api << 16) | 367),	///<Payload type {::T_SmsApi_UpdateVmscNumberReq}
	API_CAPI_MSG_SMS_GETSMSBEARERPREFERENCE_REQ = ((P_log_api << 16) | 368),	///<Payload type {::T_SmsApi_GetSMSBearerPreference}
	API_CAPI_MSG_SMS_SETSMSBEARERPREFERENCE_REQ = ((P_log_api << 16) | 369),	///<Payload type {::T_SmsApi_SetSMSBearerPreference}
	API_CAPI_MSG_SMS_SETSMSREADSTATUSCHANGEMODE_REQ = ((P_log_api << 16) | 370),	///<Payload type {::T_SmsApi_SetSmsReadStatusChangeMode}
	API_CAPI_MSG_SMS_GETSMSREADSTATUSCHANGEMODE_REQ = ((P_log_api << 16) | 371),	///<Payload type {::T_SmsApi_GetSmsReadStatusChangeMode}
	API_CAPI_MSG_SMS_CHANGESTATUSREQ_REQ = ((P_log_api << 16) | 372),	///<Payload type {::T_SmsApi_ChangeSmsStatusReq}
	API_CAPI_MSG_SMS_SENDMESTOREDSTATUSIND_REQ = ((P_log_api << 16) | 373),	///<Payload type {::T_SmsApi_SendMEStoredStatusInd}
	API_CAPI_MSG_SMS_SENDMERETRIEVESMSDATAIND_REQ = ((P_log_api << 16) | 374),	///<Payload type {::T_SmsApi_SendMERetrieveSmsDataInd}
	API_CAPI_MSG_SMS_SENDMEREMOVEDSTATUSIND_REQ = ((P_log_api << 16) | 375),	///<Payload type {::T_SmsApi_SendMERemovedStatusInd}
	API_CAPI_MSG_SMS_SETSMSSTOREDSTATE_REQ = ((P_log_api << 16) | 376),	///<Payload type {::T_SmsApi_SetSmsStoredState}
	API_CAPI_MSG_SMS_ISCACHEDDATAREADY_REQ = ((P_log_api << 16) | 377),	///<Payload type {::T_SmsApi_IsCachedDataReady}
	API_CAPI_MSG_SMS_GETENHANCEDVMINFOIEI_REQ = ((P_log_api << 16) | 378),	///<Payload type {::T_SmsApi_GetEnhancedVMInfoIEI}
	API_CAPI_MSG_SMS_SETALLNEWMSGDISPLAYPREF_REQ = ((P_log_api << 16) | 379),	///<Payload type {::T_SmsApi_SetAllNewMsgDisplayPref}
	API_CAPI_MSG_SMS_ACKTONETWORK_REQ = ((P_log_api << 16) | 380),	///<Payload type {::T_SmsApi_SendAckToNetwork}
	API_CAPI_MSG_SMS_STARTMULTISMSTRANSFER_REQ = ((P_log_api << 16) | 381),	///<Payload type {::T_SmsApi_StartMultiSmsTransferReq}
	API_CAPI_MSG_SMS_STOPMULTISMSTRANSFER_REQ = ((P_log_api << 16) | 382),	///<Payload type {::T_SmsApi_StopMultiSmsTransferReq}
	API_CAPI_API_CAPI_MSG_DUMMY_383 = ((P_log_api << 16) | 383),
	API_CAPI_API_CAPI_MSG_DUMMY_384 = ((P_log_api << 16) | 384),
	API_CAPI_MSG_SMS_PDA_OVERFLOW_REQ = ((P_log_api << 16) | 385),	///<Payload type {::T_SMS_SetPDAStorageOverFlowFlag}
	API_CAPI_MSG_ISIM_SENDAUTHENAKAREQ_REQ = ((P_log_api << 16) | 386),	///<Payload type {::T_ISimApi_SendAuthenAkaReq}
	API_CAPI_MSG_ISIM_ISISIMSUPPORTED_REQ = ((P_log_api << 16) | 387),	///<Payload type {::T_ISimApi_IsIsimSupported}
	API_CAPI_MSG_ISIM_ISISIMACTIVATED_REQ = ((P_log_api << 16) | 388),	///<Payload type {::T_ISimApi_IsIsimActivated}
	API_CAPI_MSG_ISIM_ACTIVATEISIMAPPLI_REQ = ((P_log_api << 16) | 389),	///<Payload type {::T_ISimApi_ActivateIsimAppli}
	API_CAPI_MSG_ISIM_SENDAUTHENHTTPREQ_REQ = ((P_log_api << 16) | 390),	///<Payload type {::T_ISimApi_SendAuthenHttpReq}
	API_CAPI_MSG_ISIM_SENDAUTHENGBANAFREQ_REQ = ((P_log_api << 16) | 391),	///<Payload type {::T_ISimApi_SendAuthenGbaNafReq}
	API_CAPI_MSG_ISIM_SENDAUTHENGBABOOTREQ_REQ = ((P_log_api << 16) | 392),	///<Payload type {::T_ISimApi_SendAuthenGbaBootReq}
	API_CAPI_MSG_PBK_GETALPHA_REQ = ((P_log_api << 16) | 393),	///<Payload type {::T_PbkApi_GetAlpha}
	API_CAPI_MSG_PBK_ISEMERGENCYCALLNUMBER_REQ = ((P_log_api << 16) | 394),	///<Payload type {::T_PbkApi_IsEmergencyCallNumber}
	API_CAPI_MSG_PBK_ISPARTIALEMERGENCYCALLNUMBER_REQ = ((P_log_api << 16) | 395),	///<Payload type {::T_PbkApi_IsPartialEmergencyCallNumber}
	API_CAPI_MSG_PBK_SENDINFOREQ_REQ = ((P_log_api << 16) | 396),	///<Payload type {::T_PbkApi_SendInfoReq}
	API_CAPI_MSG_SENDFINDALPHAMATCHMULTIPLEREQ_REQ = ((P_log_api << 16) | 397),	///<Payload type {::T_PbkApi_SendFindAlphaMatchMultipleReq}
	API_CAPI_MSG_SENDFINDALPHAMATCHONEREQ_REQ = ((P_log_api << 16) | 398),	///<Payload type {::T_PbkApi_SendFindAlphaMatchOneReq}
	API_CAPI_MSG_PBK_ISREADY_REQ = ((P_log_api << 16) | 399),	///<Payload type {::T_PbkApi_IsReady}
	API_CAPI_MSG_SENDREADENTRYREQ_REQ = ((P_log_api << 16) | 400),	///<Payload type {::T_PbkApi_SendReadEntryReq}
	API_CAPI_MSG_PBK_SENDWRITEENTRYREQ_REQ = ((P_log_api << 16) | 401),	///<Payload type {::T_PbkApi_SendWriteEntryReq}
	API_CAPI_MSG_PBK_SENDUPDATEENTRYREQ = ((P_log_api << 16) | 402),	///<Payload type {::T_PbkApi_SendUpdateEntryReq}
	API_CAPI_MSG_PBK_SENDISNUMDIALLABLEREQ = ((P_log_api << 16) | 403),	///<Payload type {::T_PbkApi_SendIsNumDiallableReq}
	API_CAPI_MSG_PBK_ISNUMDIALLABLE_REQ = ((P_log_api << 16) | 404),	///<Payload type {::T_PbkApi_IsNumDiallable}
	API_CAPI_MSG_PBK_ISNUMBARRED_REQ = ((P_log_api << 16) | 405),	///<Payload type {::T_PbkApi_IsNumBarred}
	API_CAPI_MSG_PBK_ISUSSDDIALLABLE_REQ = ((P_log_api << 16) | 406),	///<Payload type {::T_PbkApi_IsUssdDiallable}
	API_CAPI_MSG_PDP_SETPDPCONTEXT_REQ = ((P_log_api << 16) | 407),	///<Payload type {::T_PdpApi_SetPDPContext}
	API_CAPI_MSG_PDP_SETSECPDPCONTEXT_REQ = ((P_log_api << 16) | 408),	///<Payload type {::T_PdpApi_SetSecPDPContext}
	API_CAPI_MSG_PDP_GETGPRSQOS_REQ = ((P_log_api << 16) | 409),	///<Payload type {::T_PdpApi_GetGPRSQoS}
	API_CAPI_MSG_PDP_SETGPRSQOS_REQ = ((P_log_api << 16) | 410),	///<Payload type {::T_PdpApi_SetGPRSQoS}
	API_CAPI_MSG_PDP_GETGPRSMINQOS_REQ = ((P_log_api << 16) | 411),	///<Payload type {::T_PdpApi_GetGPRSMinQoS}
	API_CAPI_MSG_PDP_SETGPRSMINQOS_REQ = ((P_log_api << 16) | 412),	///<Payload type {::T_PdpApi_SetGPRSMinQoS}
	API_CAPI_MSG_MS_SENDCOMBINEDATTACHREQ_REQ = ((P_log_api << 16) | 413),	///<Payload type {::T_NetRegApi_SendCombinedAttachReq}
	API_CAPI_MSG_MS_SENDDETACHREQ_REQ = ((P_log_api << 16) | 414),	///<Payload type {::T_NetRegApi_SendDetachReq}
	API_CAPI_API_CAPI_MSG_DUMMY_415 = ((P_log_api << 16) | 415),
	API_CAPI_MSG_PDP_ISSECONDARYPDPDEFINED_REQ = ((P_log_api << 16) | 416),	///<Payload type {::T_PdpApi_IsSecondaryPdpDefined}
	API_CAPI_MSG_PCHEX_SENDPDPACTIVATEREQ_REQ = ((P_log_api << 16) | 417),	///<Payload type {::T_PchExApi_SendPDPActivateReq}
	API_CAPI_MSG_PCHEX_SENDPDPMODIFYREQ_REQ = ((P_log_api << 16) | 418),	///<Payload type {::T_PchExApi_SendPDPModifyReq}
	API_CAPI_MSG_PCHEX_SENDPDPDEACTIVATEREQ_REQ = ((P_log_api << 16) | 419),	///<Payload type {::T_PchExApi_SendPDPDeactivateReq}
	API_CAPI_MSG_PCHEX_SENDPDPACTIVATESECREQ_REQ = ((P_log_api << 16) | 420),	///<Payload type {::T_PchExApi_SendPDPActivateSecReq}
	API_CAPI_MSG_PDP_GETGPRSACTIVATESTATUS_REQ = ((P_log_api << 16) | 421),	///<Payload type {::T_PdpApi_GetGPRSActivateStatus}
	API_CAPI_MSG_PDP_SETMSCLASS_REQ = ((P_log_api << 16) | 422),	///<Payload type {::T_NetRegApi_SetMSClass}
	API_CAPI_API_CAPI_MSG_DUMMY_423 = ((P_log_api << 16) | 423),
	API_CAPI_MSG_PDP_GETUMTSTFT_REQ = ((P_log_api << 16) | 424),	///<Payload type {::T_PdpApi_GetUMTSTft}
	API_CAPI_MSG_PDP_SETUMTSTFT_REQ = ((P_log_api << 16) | 425),	///<Payload type {::T_PdpApi_SetUMTSTft}
	API_CAPI_MSG_PDP_DELETEUMTSTFT_REQ = ((P_log_api << 16) | 426),	///<Payload type {::T_PdpApi_DeleteUMTSTft}
	API_CAPI_MSG_PDP_DEACTIVATESNDCPCONNECTION_REQ = ((P_log_api << 16) | 427),	///<Payload type {::T_PdpApi_DeactivateSNDCPConnection}
	API_CAPI_MSG_PDP_GETR99UMTSMINQOS_REQ = ((P_log_api << 16) | 428),	///<Payload type {::T_PdpApi_GetR99UMTSMinQoS}
	API_CAPI_MSG_PDP_GETR99UMTSQOS_REQ = ((P_log_api << 16) | 429),	///<Payload type {::T_PdpApi_GetR99UMTSQoS}
	API_CAPI_MSG_PDP_GETUMTSMINQOS_REQ = ((P_log_api << 16) | 430),	///<Payload type {::T_PdpApi_GetUMTSMinQoS}
	API_CAPI_MSG_PDP_GETUMTSQOS_REQ = ((P_log_api << 16) | 431),	///<Payload type {::T_PdpApi_GetUMTSQoS}
	API_CAPI_MSG_PDP_GETNEGQOS_REQ = ((P_log_api << 16) | 432),	///<Payload type {::T_PdpApi_GetNegQoS}
	API_CAPI_MSG_PDP_SETR99UMTSMINQOS_REQ = ((P_log_api << 16) | 433),	///<Payload type {::T_PdpApi_SetR99UMTSMinQoS}
	API_CAPI_MSG_PDP_SETR99UMTSQOS_REQ = ((P_log_api << 16) | 434),	///<Payload type {::T_PdpApi_SetR99UMTSQoS}
	API_CAPI_MSG_PDP_SETUMTSMINQOS_REQ = ((P_log_api << 16) | 435),	///<Payload type {::T_PdpApi_SetUMTSMinQoS}
	API_CAPI_MSG_PDP_SETUMTSQOS_REQ = ((P_log_api << 16) | 436),	///<Payload type {::T_PdpApi_SetUMTSQoS}
	API_CAPI_MSG_PDP_GETNEGOTIATEDPARMS_REQ = ((P_log_api << 16) | 437),	///<Payload type {::T_PdpApi_GetNegotiatedParms}
	API_CAPI_MSG_MS_ISGPRSCALLACTIVE_REQ = ((P_log_api << 16) | 438),	///<Payload type {::T_MS_IsGprsCallActive}
	API_CAPI_MSG_MS_SETCHANGPRSCALLACTIVE_REQ = ((P_log_api << 16) | 439),	///<Payload type {::T_MS_SetChanGprsCallActive}
	API_CAPI_MSG_MS_SETCIDFORGPRSACTIVECHAN_REQ = ((P_log_api << 16) | 440),	///<Payload type {::T_MS_SetCidForGprsActiveChan}
	API_CAPI_MSG_PDP_GETPPPMODEMCID_REQ = ((P_log_api << 16) | 441),	///<Payload type {::T_PdpApi_GetPPPModemCid}
	API_CAPI_MSG_MS_GETGPRSACTIVECHANFROMCID_REQ = ((P_log_api << 16) | 442),	///<Payload type {::T_MS_GetGprsActiveChanFromCid}
	API_CAPI_MSG_MS_GETCIDFROMGPRSACTIVECHAN_REQ = ((P_log_api << 16) | 443),	///<Payload type {::T_MS_GetCidFromGprsActiveChan}
	API_CAPI_MSG_PDP_GETPDPADDRESS_REQ = ((P_log_api << 16) | 444),	///<Payload type {::T_PdpApi_GetPDPAddress}
	API_CAPI_MSG_PDP_SENDTBFDATA_REQ = ((P_log_api << 16) | 445),	///<Payload type {::T_PdpApi_SendTBFData}
	API_CAPI_MSG_PDP_TFTADDFILTER_REQ = ((P_log_api << 16) | 446),	///<Payload type {::T_PdpApi_TftAddFilter}
	API_CAPI_MSG_PDP_SETPCHCONTEXTSTATE_REQ = ((P_log_api << 16) | 447),	///<Payload type {::T_PdpApi_SetPCHContextState}
	API_CAPI_MSG_PDP_SETDEFAULTPDPCONTEXT_REQ = ((P_log_api << 16) | 448),	///<Payload type {::T_PdpApi_SetDefaultPDPContext}
	API_CAPI_MSG_PCHEX_READDECODEDPROTCONFIG_REQ = ((P_log_api << 16) | 449),	///<Payload type {::T_PchExApi_GetDecodedProtConfig}
	API_CAPI_MSG_PCHEX_BUILDPROTCONFIGOPTIONS_REQ = ((P_log_api << 16) | 450),	///<Payload type {::T_PchExApi_BuildIpConfigOptions}
	API_CAPI_MSG_PCHEX_BUILDPROTCONFIGOPTIONS2_REQ = ((P_log_api << 16) | 451),	///<Payload type {::T_PchExApi_BuildIpConfigOptions2}
	API_CAPI_MSG_PCHEX_BUILDPROTCONFIGOPTION_CHAP_TYPE_REQ = ((P_log_api << 16) | 452),	///<Payload type {::T_PchExApi_BuildIpConfigOptionsWithChapAuthType}
	API_CAPI_MSG_PDP_GET_DEFAULT_QOS_REQ = ((P_log_api << 16) | 453),	///<Payload type {::T_PdpApi_GetDefaultQos}
	API_CAPI_MSG_PDP_ISCONTEXT_ACTIVE_REQ = ((P_log_api << 16) | 454),	///<Payload type {::T_PdpApi_IsPDPContextActive}
	API_CAPI_MSG_PDP_ACTIVATESNDCPCONNECTION_REQ = ((P_log_api << 16) | 455),	///<Payload type {::T_PdpApi_ActivateSNDCPConnection}
	API_CAPI_MSG_PDP_GETPDPCONTEXT_REQ = ((P_log_api << 16) | 456),	///<Payload type {::T_PdpApi_GetPDPContext}
	API_CAPI_MSG_PDP_GETPDPCONTEXT_CID_LIST_REQ = ((P_log_api << 16) | 457),	///<Payload type {::T_PdpApi_GetDefinedPDPContextCidList}
	API_CAPI_API_CAPI_MSG_DUMMY_458 = ((P_log_api << 16) | 458),
	API_CAPI_MSG_SYSPARAM_BOOTLOADER_VER_REQ = ((P_log_api << 16) | 459),	///<Payload type {::T_SYS_GetBootLoaderVersion}
	API_CAPI_MSG_SYSPARAM_DSF_VER_REQ = ((P_log_api << 16) | 460),	///<Payload type {::T_SYS_GetDSFVersion}
	API_CAPI_MSG_USIM_UST_DATA_REQ = ((P_log_api << 16) | 461),	///<Payload type {::T_USimApi_GetUstData}
	API_CAPI_API_CAPI_MSG_DUMMY_462 = ((P_log_api << 16) | 462),
	API_CAPI_MSG_SATK_GETCACHEDROOTMENUPTR_REQ = ((P_log_api << 16) | 463),	///<Payload type {::T_SatkApi_GetCachedRootMenuPtr}
	API_CAPI_MSG_SATK_SENDUSERACTIVITYEVENT_REQ = ((P_log_api << 16) | 464),	///<Payload type {::T_SatkApi_SendUserActivityEvent}
	API_CAPI_MSG_SATK_SENDIDLESCREENAVAIEVENT_REQ = ((P_log_api << 16) | 465),	///<Payload type {::T_SatkApi_SendIdleScreenAvaiEvent}
	API_CAPI_MSG_SATK_SENDLANGSELECTEVENT_REQ = ((P_log_api << 16) | 466),	///<Payload type {::T_SatkApi_SendLangSelectEvent}
	API_CAPI_MSG_SATK_SENDBROWSERTERMEVENT_REQ = ((P_log_api << 16) | 467),	///<Payload type {::T_SatkApi_SendBrowserTermEvent}
	API_CAPI_MSG_SATK_CMDRESP_REQ = ((P_log_api << 16) | 468),	///<Payload type {::T_SatkApi_CmdResp}
	API_CAPI_MSG_SATK_DATASERVCMDRESP_REQ = ((P_log_api << 16) | 469),	///<Payload type {::T_SatkApi_DataServCmdResp}
	API_CAPI_MSG_SATK_SENDDATASERVREQ_REQ = ((P_log_api << 16) | 470),	///<Payload type {::T_SatkApi_SendDataServReq}
	API_CAPI_MSG_SATK_SENDTERMINALRSP_REQ = ((P_log_api << 16) | 471),	///<Payload type {::T_SatkApi_SendTerminalRsp}
	API_CAPI_MSG_SATK_SETTERMPROFILE_REQ = ((P_log_api << 16) | 472),	///<Payload type {::T_SatkApi_SetTermProfile}
	API_CAPI_MSG_SATK_SEND_ENVELOPE_CMD_REQ = ((P_log_api << 16) | 473),	///<Payload type {::T_SatkApi_SendEnvelopeCmdReq}
	API_CAPI_MSG_STK_TERMINAL_RESPONSE_REQ = ((P_log_api << 16) | 474),	///<Payload type {::T_SatkApi_SendTerminalRspReq}
	API_CAPI_MSG_STK_SEND_BROWSING_STATUS_EVT_REQ = ((P_log_api << 16) | 475),	///<Payload type {::T_SATK_SendBrowsingStatusEvent}
	API_CAPI_MSG_SATK_SEND_CC_SETUP_REQ = ((P_log_api << 16) | 476),	///<Payload type {::T_SatkApi_SendCcSetupReq}
	API_CAPI_API_CAPI_MSG_DUMMY_477 = ((P_log_api << 16) | 477),
	API_CAPI_API_CAPI_MSG_DUMMY_478 = ((P_log_api << 16) | 478),
	API_CAPI_API_CAPI_MSG_DUMMY_479 = ((P_log_api << 16) | 479),
	API_CAPI_MSG_SATK_SEND_CC_SS_REQ = ((P_log_api << 16) | 480),	///<Payload type {::T_SatkApi_SendCcSsReq}
	API_CAPI_MSG_SATK_SEND_CC_USSD_REQ = ((P_log_api << 16) | 481),	///<Payload type {::T_SatkApi_SendCcUssdReq}
	API_CAPI_MSG_SATK_SEND_CC_SMS_REQ = ((P_log_api << 16) | 482),	///<Payload type {::T_SatkApi_SendCcSmsReq}
	API_CAPI_API_CAPI_MSG_DUMMY_483 = ((P_log_api << 16) | 483),
	API_CAPI_MSG_LCS_CPMOLRREQ_REQ = ((P_log_api << 16) | 484),	///<Payload type {::T_LCS_CpMoLrReq}
	API_CAPI_MSG_LCS_CPMOLRABORT_REQ = ((P_log_api << 16) | 485),	///<Payload type {::T_LCS_CpMoLrAbort}
	API_CAPI_MSG_LCS_CPMTLRVERIFICATIONRSP_REQ = ((P_log_api << 16) | 486),	///<Payload type {::T_LCS_CpMtLrVerificationRsp}
	API_CAPI_MSG_LCS_CPMTLRRSP_REQ = ((P_log_api << 16) | 487),	///<Payload type {::T_LCS_CpMtLrRsp}
	API_CAPI_MSG_LCS_CPLOCUPDATERSP_REQ = ((P_log_api << 16) | 488),	///<Payload type {::T_LCS_CpLocUpdateRsp}
	API_CAPI_MSG_LCS_DECODEPOSESTIMATE_REQ = ((P_log_api << 16) | 489),	///<Payload type {::T_LCS_DecodePosEstimate}
	API_CAPI_MSG_LCS_ENCODEASSISTANCEREQ_REQ = ((P_log_api << 16) | 490),	///<Payload type {::T_LCS_EncodeAssistanceReq}
	API_CAPI_MSG_LCS_SEND_RRLP_DATA_REQ = ((P_log_api << 16) | 491),	///<Payload type {::T_LCS_SendRrlpDataToNetwork}
	API_CAPI_MSG_LCS_RRC_MEAS_REPORT_REQ = ((P_log_api << 16) | 492),	///<Payload type {::T_LCS_RrcMeasurementReport}
	API_CAPI_MSG_LCS_RRC_MEAS_FAILURE_REQ = ((P_log_api << 16) | 493),	///<Payload type {::T_LCS_RrcMeasurementControlFailure}
	API_CAPI_MSG_LCS_RRC_STATUS_REQ = ((P_log_api << 16) | 494),	///<Payload type {::T_LCS_RrcStatus}
	API_CAPI_MSG_LCS_FTT_SYNC_REQ = ((P_log_api << 16) | 495),	///<Payload type {::T_LCS_FttSyncReq}
	API_CAPI_MSG_LCS_RRLP_SEND_DATA_REQ = ((P_log_api << 16) | 496),	///<Payload type {::T_LcsApi_RrlpSendDataToNetwork}
	API_CAPI_MSG_LCS_RRLP_REG_HDL_REQ = ((P_log_api << 16) | 497),	///<Payload type {::T_LcsApi_RrlpRegisterDataHandler}
	API_CAPI_MSG_CC_MAKEVOICECALL_REQ = ((P_log_api << 16) | 498),	///<Payload type {::T_CcApi_MakeVoiceCall}
	API_CAPI_MSG_CC_MAKEDATACALL_REQ = ((P_log_api << 16) | 499),	///<Payload type {::T_CcApi_MakeDataCall}
	API_CAPI_MSG_CC_MAKEFAXCALL_REQ = ((P_log_api << 16) | 500),	///<Payload type {::T_CcApi_MakeFaxCall}
	API_CAPI_MSG_CC_MAKEVIDEOCALL_REQ = ((P_log_api << 16) | 501),	///<Payload type {::T_CcApi_MakeVideoCall}
	API_CAPI_MSG_CC_ENDCALL_REQ = ((P_log_api << 16) | 502),	///<Payload type {::T_CcApi_EndCall}
	API_CAPI_MSG_CC_ENDALLCALLS_REQ = ((P_log_api << 16) | 503),	///<Payload type {::T_CcApi_EndAllCalls}
	API_CAPI_MSG_CCAPI_ENDCALL_IMMEDIATE_REQ = ((P_log_api << 16) | 504),	///<Payload type {::T_CcApi_EndCallImmediate}
	API_CAPI_MSG_CCAPI_ENDALLCALLS_IMMEDIATE_REQ = ((P_log_api << 16) | 505),	///<Payload type {::T_CcApi_EndAllCallsImmediate}
	API_CAPI_MSG_CC_ENDMPTYCALLS_REQ = ((P_log_api << 16) | 506),	///<Payload type {::T_CcApi_EndMPTYCalls}
	API_CAPI_MSG_CC_ENDHELDCALL_REQ = ((P_log_api << 16) | 507),	///<Payload type {::T_CcApi_EndHeldCall}
	API_CAPI_MSG_CC_ACCEPTVOICECALL_REQ = ((P_log_api << 16) | 508),	///<Payload type {::T_CcApi_AcceptVoiceCall}
	API_CAPI_MSG_CC_ACCEPTDATACALL_REQ = ((P_log_api << 16) | 509),	///<Payload type {::T_CcApi_AcceptDataCall}
	API_CAPI_MSG_CC_ACCEPTWAITINGCALL_REQ = ((P_log_api << 16) | 510),	///<Payload type {::T_CcApi_AcceptWaitingCall}
	API_CAPI_MSG_CC_ACCEPTVIDEOCALL_REQ = ((P_log_api << 16) | 511),	///<Payload type {::T_CcApi_AcceptVideoCall}
	API_CAPI_MSG_CC_HOLDCURRENTCALL_REQ = ((P_log_api << 16) | 512),	///<Payload type {::T_CcApi_HoldCurrentCall}
	API_CAPI_MSG_CC_HOLDCALL_REQ = ((P_log_api << 16) | 513),	///<Payload type {::T_CcApi_HoldCall}
	API_CAPI_MSG_CC_RETRIEVENEXTHELDCALL_REQ = ((P_log_api << 16) | 514),	///<Payload type {::T_CcApi_RetrieveNextHeldCall}
	API_CAPI_MSG_CC_RETRIEVECALL_REQ = ((P_log_api << 16) | 515),	///<Payload type {::T_CcApi_RetrieveCall}
	API_CAPI_MSG_CC_SWAPCALL_REQ = ((P_log_api << 16) | 516),	///<Payload type {::T_CcApi_SwapCall}
	API_CAPI_MSG_CC_SPLITCALL_REQ = ((P_log_api << 16) | 517),	///<Payload type {::T_CcApi_SplitCall}
	API_CAPI_MSG_CC_JOINCALL_REQ = ((P_log_api << 16) | 518),	///<Payload type {::T_CcApi_JoinCall}
	API_CAPI_MSG_CC_TRANSFERCALL_REQ = ((P_log_api << 16) | 519),	///<Payload type {::T_CcApi_TransferCall}
	API_CAPI_API_CAPI_MSG_DUMMY_520 = ((P_log_api << 16) | 520),
	API_CAPI_API_CAPI_MSG_DUMMY_521 = ((P_log_api << 16) | 521),
	API_CAPI_API_CAPI_MSG_DUMMY_522 = ((P_log_api << 16) | 522),
	API_CAPI_API_CAPI_MSG_DUMMY_523 = ((P_log_api << 16) | 523),
	API_CAPI_API_CAPI_MSG_DUMMY_524 = ((P_log_api << 16) | 524),
	API_CAPI_API_CAPI_MSG_DUMMY_525 = ((P_log_api << 16) | 525),
	API_CAPI_API_CAPI_MSG_DUMMY_526 = ((P_log_api << 16) | 526),
	API_CAPI_API_CAPI_MSG_DUMMY_527 = ((P_log_api << 16) | 527),
	API_CAPI_API_CAPI_MSG_DUMMY_528 = ((P_log_api << 16) | 528),
	API_CAPI_API_CAPI_MSG_DUMMY_529 = ((P_log_api << 16) | 529),
	API_CAPI_API_CAPI_MSG_DUMMY_530 = ((P_log_api << 16) | 530),
	API_CAPI_API_CAPI_MSG_DUMMY_531 = ((P_log_api << 16) | 531),
	API_CAPI_API_CAPI_MSG_DUMMY_532 = ((P_log_api << 16) | 532),
	API_CAPI_API_CAPI_MSG_DUMMY_533 = ((P_log_api << 16) | 533),
	API_CAPI_API_CAPI_MSG_DUMMY_534 = ((P_log_api << 16) | 534),
	API_CAPI_API_CAPI_MSG_DUMMY_535 = ((P_log_api << 16) | 535),
	API_CAPI_API_CAPI_MSG_DUMMY_536 = ((P_log_api << 16) | 536),
	API_CAPI_API_CAPI_MSG_DUMMY_537 = ((P_log_api << 16) | 537),
	API_CAPI_API_CAPI_MSG_DUMMY_538 = ((P_log_api << 16) | 538),
	API_CAPI_API_CAPI_MSG_DUMMY_539 = ((P_log_api << 16) | 539),
	API_CAPI_API_CAPI_MSG_DUMMY_540 = ((P_log_api << 16) | 540),
	API_CAPI_API_CAPI_MSG_DUMMY_541 = ((P_log_api << 16) | 541),
	API_CAPI_API_CAPI_MSG_DUMMY_542 = ((P_log_api << 16) | 542),
	API_CAPI_API_CAPI_MSG_DUMMY_543 = ((P_log_api << 16) | 543),
	API_CAPI_API_CAPI_MSG_DUMMY_544 = ((P_log_api << 16) | 544),
	API_CAPI_API_CAPI_MSG_DUMMY_545 = ((P_log_api << 16) | 545),
	API_CAPI_API_CAPI_MSG_DUMMY_546 = ((P_log_api << 16) | 546),
	API_CAPI_API_CAPI_MSG_DUMMY_547 = ((P_log_api << 16) | 547),
	API_CAPI_API_CAPI_MSG_DUMMY_548 = ((P_log_api << 16) | 548),
	API_CAPI_API_CAPI_MSG_DUMMY_549 = ((P_log_api << 16) | 549),
	API_CAPI_API_CAPI_MSG_DUMMY_550 = ((P_log_api << 16) | 550),
	API_CAPI_API_CAPI_MSG_DUMMY_551 = ((P_log_api << 16) | 551),
	API_CAPI_API_CAPI_MSG_DUMMY_552 = ((P_log_api << 16) | 552),
	API_CAPI_API_CAPI_MSG_DUMMY_553 = ((P_log_api << 16) | 553),
	API_CAPI_API_CAPI_MSG_DUMMY_554 = ((P_log_api << 16) | 554),
	API_CAPI_API_CAPI_MSG_DUMMY_555 = ((P_log_api << 16) | 555),
	API_CAPI_API_CAPI_MSG_DUMMY_556 = ((P_log_api << 16) | 556),
	API_CAPI_API_CAPI_MSG_DUMMY_557 = ((P_log_api << 16) | 557),
	API_CAPI_API_CAPI_MSG_DUMMY_558 = ((P_log_api << 16) | 558),
	API_CAPI_API_CAPI_MSG_DUMMY_559 = ((P_log_api << 16) | 559),
	API_CAPI_API_CAPI_MSG_DUMMY_560 = ((P_log_api << 16) | 560),
	API_CAPI_API_CAPI_MSG_DUMMY_561 = ((P_log_api << 16) | 561),
	API_CAPI_API_CAPI_MSG_DUMMY_562 = ((P_log_api << 16) | 562),
	API_CAPI_API_CAPI_MSG_DUMMY_563 = ((P_log_api << 16) | 563),
	API_CAPI_API_CAPI_MSG_DUMMY_564 = ((P_log_api << 16) | 564),
	API_CAPI_API_CAPI_MSG_DUMMY_565 = ((P_log_api << 16) | 565),
	API_CAPI_API_CAPI_MSG_DUMMY_566 = ((P_log_api << 16) | 566),
	API_CAPI_API_CAPI_MSG_DUMMY_567 = ((P_log_api << 16) | 567),
	API_CAPI_API_CAPI_MSG_DUMMY_568 = ((P_log_api << 16) | 568),
	API_CAPI_API_CAPI_MSG_DUMMY_569 = ((P_log_api << 16) | 569),
	API_CAPI_API_CAPI_MSG_DUMMY_570 = ((P_log_api << 16) | 570),
	API_CAPI_API_CAPI_MSG_DUMMY_571 = ((P_log_api << 16) | 571),
	API_CAPI_API_CAPI_MSG_DUMMY_572 = ((P_log_api << 16) | 572),
	API_CAPI_API_CAPI_MSG_DUMMY_573 = ((P_log_api << 16) | 573),
	API_CAPI_API_CAPI_MSG_DUMMY_574 = ((P_log_api << 16) | 574),
	API_CAPI_API_CAPI_MSG_DUMMY_575 = ((P_log_api << 16) | 575),
	API_CAPI_API_CAPI_MSG_DUMMY_576 = ((P_log_api << 16) | 576),
	API_CAPI_API_CAPI_MSG_DUMMY_577 = ((P_log_api << 16) | 577),
	API_CAPI_API_CAPI_MSG_DUMMY_578 = ((P_log_api << 16) | 578),
	API_CAPI_API_CAPI_MSG_DUMMY_579 = ((P_log_api << 16) | 579),
	API_CAPI_API_CAPI_MSG_DUMMY_580 = ((P_log_api << 16) | 580),
	API_CAPI_API_CAPI_MSG_DUMMY_581 = ((P_log_api << 16) | 581),
	API_CAPI_API_CAPI_MSG_DUMMY_582 = ((P_log_api << 16) | 582),
	API_CAPI_API_CAPI_MSG_DUMMY_583 = ((P_log_api << 16) | 583),
	API_CAPI_API_CAPI_MSG_DUMMY_584 = ((P_log_api << 16) | 584),
	API_CAPI_API_CAPI_MSG_DUMMY_585 = ((P_log_api << 16) | 585),
	API_CAPI_API_CAPI_MSG_DUMMY_586 = ((P_log_api << 16) | 586),
	API_CAPI_API_CAPI_MSG_DUMMY_587 = ((P_log_api << 16) | 587),
	API_CAPI_API_CAPI_MSG_DUMMY_588 = ((P_log_api << 16) | 588),
	API_CAPI_API_CAPI_MSG_DUMMY_589 = ((P_log_api << 16) | 589),
	API_CAPI_API_CAPI_MSG_DUMMY_590 = ((P_log_api << 16) | 590),
	API_CAPI_API_CAPI_MSG_DUMMY_591 = ((P_log_api << 16) | 591),
	API_CAPI_API_CAPI_MSG_DUMMY_592 = ((P_log_api << 16) | 592),
	API_CAPI_API_CAPI_MSG_DUMMY_593 = ((P_log_api << 16) | 593),
	API_CAPI_API_CAPI_MSG_DUMMY_594 = ((P_log_api << 16) | 594),
	API_CAPI_API_CAPI_MSG_DUMMY_595 = ((P_log_api << 16) | 595),
	API_CAPI_API_CAPI_MSG_DUMMY_596 = ((P_log_api << 16) | 596),
	API_CAPI_API_CAPI_MSG_DUMMY_597 = ((P_log_api << 16) | 597),
	API_CAPI_API_CAPI_MSG_DUMMY_598 = ((P_log_api << 16) | 598),
	API_CAPI_API_CAPI_MSG_DUMMY_599 = ((P_log_api << 16) | 599),
	API_CAPI_API_CAPI_MSG_DUMMY_600 = ((P_log_api << 16) | 600),
	API_CAPI_API_CAPI_MSG_DUMMY_601 = ((P_log_api << 16) | 601),
	API_CAPI_MSG_CC_GETCNAPNAME_REQ = ((P_log_api << 16) | 602),	///<Payload type {::T_CcApi_GetCNAPName}
	API_CAPI_API_CAPI_MSG_DUMMY_603 = ((P_log_api << 16) | 603),
	API_CAPI_API_CAPI_MSG_DUMMY_604 = ((P_log_api << 16) | 604),
	API_CAPI_API_CAPI_MSG_DUMMY_605 = ((P_log_api << 16) | 605),
	API_CAPI_MSG_MS_FORCE_PS_REL_REQ = ((P_log_api << 16) | 606),	///<Payload type {::T_NetRegApi_ForcePsReleaseReq}
	API_CAPI_MSG_CC_ISCURRENTSTATEMPTY_REQ = ((P_log_api << 16) | 607),	///<Payload type {::T_CcApi_IsCurrentStateMpty}
	API_CAPI_MSG_PDP_GETPCHCONTEXTSTATE_REQ = ((P_log_api << 16) | 608),	///<Payload type {::T_PdpApi_GetPCHContextState}
	API_CAPI_MSG_PDP_GETPCHCONTEXT_EX_REQ = ((P_log_api << 16) | 609),	///<Payload type {::T_PdpApi_GetPDPContextEx}
	API_CAPI_MSG_SIM_PIN_LOCK_TYPE_REQ = ((P_log_api << 16) | 610),	///<Payload type {::T_SimApi_GetCurrLockedSimlockType}
	API_CAPI_API_CAPI_MSG_DUMMY_611 = ((P_log_api << 16) | 611),
	API_CAPI_MSG_SIM_SEND_APDU_REQ = ((P_log_api << 16) | 612),	///<Payload type {::T_SimApi_SubmitSelectFileSendApduReq}
	API_CAPI_API_CAPI_MSG_DUMMY_613 = ((P_log_api << 16) | 613),
	API_CAPI_MSG_SIM_MUL_REC_DATA_REQ = ((P_log_api << 16) | 614),	///<Payload type {::T_SimApi_SubmitMulRecordEFileReq}
	API_CAPI_API_CAPI_MSG_DUMMY_615 = ((P_log_api << 16) | 615),
	API_CAPI_MSG_SIM_SELECT_APPLICATION_REQ = ((P_log_api << 16) | 616),	///<Payload type {::T_SimApi_SendSelectApplicationReq}
	API_CAPI_MSG_SIM_PEROFRM_STEERING_OF_ROAMING_REQ = ((P_log_api << 16) | 617),	///<Payload type {::T_SimApi_PerformSteeringOfRoaming}
	API_CAPI_API_CAPI_MSG_DUMMY_618 = ((P_log_api << 16) | 618),
	API_CAPI_MSG_STK_PROACTIVE_CMD_FETCHING_ON_OFF_REQ = ((P_log_api << 16) | 619),	///<Payload type {::T_SatkApi_SendProactiveCmdFetchingOnOffReq}
	API_CAPI_MSG_SATK_SEND_EXT_PROACTIVE_CMD_REQ = ((P_log_api << 16) | 620),	///<Payload type {::T_SatkApi_SendExtProactiveCmdReq}
	API_CAPI_MSG_SATK_SEND_TERMINAL_PROFILE_REQ = ((P_log_api << 16) | 621),	///<Payload type {::T_SatkApi_SendTerminalProfileReq}
	API_CAPI_MSG_STK_POLLING_INTERVAL_REQ = ((P_log_api << 16) | 622),	///<Payload type {::T_SatkApi_SendPollingIntervalReq}
	API_CAPI_API_CAPI_MSG_DUMMY_623 = ((P_log_api << 16) | 623),
	API_CAPI_MSG_PDP_SETPDPACTIVATIONCCFLAG_REQ = ((P_log_api << 16) | 624),	///<Payload type {::T_PdpApi_SetPDPActivationCallControlFlag}
	API_CAPI_MSG_PDP_GETPDPACTIVATIONCCFLAG_REQ = ((P_log_api << 16) | 625),	///<Payload type {::T_PdpApi_GetPDPActivationCallControlFlag}
	API_CAPI_MSG_PDP_SENDPDPACTIVATIONPDU_REQ = ((P_log_api << 16) | 626),	///<Payload type {::T_PdpApi_SendPDPActivateReq_PDU}
	API_CAPI_MSG_PDP_REJECTNWIACTIVATION_REQ = ((P_log_api << 16) | 627),	///<Payload type {::T_PdpApi_RejectNWIPDPActivation}
	API_CAPI_MSG_PDP_SETBEARERCTRLMODE_REQ = ((P_log_api << 16) | 628),	///<Payload type {::T_PdpApi_SetPDPBearerCtrlMode}
	API_CAPI_MSG_PDP_GETBEARERCTRLMODE_REQ = ((P_log_api << 16) | 629),	///<Payload type {::T_PdpApi_GetPDPBearerCtrlMode}
	API_CAPI_MSG_PDP_REJECTACTIVATESECIND_REQ = ((P_log_api << 16) | 630),	///<Payload type {::T_PdpApi_RejectActivateSecInd}
	API_CAPI_MSG_PDP_SETPDPNWICONTROLFLAG_REQ = ((P_log_api << 16) | 631),	///<Payload type {::T_PdpApi_SetPDPNWIControlFlag}
	API_CAPI_MSG_PDP_GETPDPNWICONTROLFLAG_REQ = ((P_log_api << 16) | 632),	///<Payload type {::T_PdpApi_GetPDPNWIControlFlag}
	API_CAPI_API_CAPI_MSG_DUMMY_633 = ((P_log_api << 16) | 633),
	API_CAPI_MSG_PDP_CHECKUMTSTFT_REQ = ((P_log_api << 16) | 634),	///<Payload type {::T_PdpApi_CheckUMTSTft}
	API_CAPI_MSG_PDP_ISANYPDPCONTEXTACTIVE_REQ = ((P_log_api << 16) | 635),	///<Payload type {::T_PdpApi_IsAnyPDPContextActive}
	API_CAPI_MSG_PDP_ISANYPDPCONTEXTPENDING_REQ = ((P_log_api << 16) | 636),	///<Payload type {::T_PdpApi_IsAnyPDPContextActivePending}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_AUTO_REJECT_SWITCH = ((P_log_api << 16) | 637),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_TEXT_TELEPHONY_SWITCH = ((P_log_api << 16) | 638),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_MT_CALL_HANDLING = ((P_log_api << 16) | 639),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_END_CALL_CAUSE = ((P_log_api << 16) | 640),	///<Payload type {::T_ELEM_PTR_CC_ELEM_END_CALL_CAUSE}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_SPEECH_CODEC = ((P_log_api << 16) | 641),	///<Payload type {::T_ELEM_PTR_CC_ELEM_SPEECH_CODEC}
	API_CAPI_MSG_ELEM_PTR_CC_ELEM_CHANNEL_MODE = ((P_log_api << 16) | 642),	///<Payload type {::T_ELEM_PTR_CC_ELEM_CHANNEL_MODE}
	API_CAPI_API_CAPI_MSG_DUMMY_643 = ((P_log_api << 16) | 643),
	API_CAPI_API_CAPI_MSG_DUMMY_644 = ((P_log_api << 16) | 644),
	API_CAPI_API_CAPI_MSG_DUMMY_645 = ((P_log_api << 16) | 645),
	API_CAPI_API_CAPI_MSG_DUMMY_646 = ((P_log_api << 16) | 646),
	API_CAPI_API_CAPI_MSG_DUMMY_647 = ((P_log_api << 16) | 647),
	API_CAPI_API_CAPI_MSG_DUMMY_648 = ((P_log_api << 16) | 648),
	API_CAPI_API_CAPI_MSG_DUMMY_649 = ((P_log_api << 16) | 649),
	API_CAPI_API_CAPI_MSG_DUMMY_650 = ((P_log_api << 16) | 650),
	API_CAPI_API_CAPI_MSG_DUMMY_651 = ((P_log_api << 16) | 651),
	API_CAPI_API_CAPI_MSG_DUMMY_652 = ((P_log_api << 16) | 652),
	API_CAPI_API_CAPI_MSG_DUMMY_653 = ((P_log_api << 16) | 653),
	API_CAPI_API_CAPI_MSG_DUMMY_654 = ((P_log_api << 16) | 654),
	API_CAPI_API_CAPI_MSG_DUMMY_655 = ((P_log_api << 16) | 655),
	API_CAPI_API_CAPI_MSG_DUMMY_656 = ((P_log_api << 16) | 656),
	API_CAPI_API_CAPI_MSG_DUMMY_657 = ((P_log_api << 16) | 657),
	API_CAPI_API_CAPI_MSG_DUMMY_658 = ((P_log_api << 16) | 658),
	API_CAPI_API_CAPI_MSG_DUMMY_659 = ((P_log_api << 16) | 659),
	API_CAPI_API_CAPI_MSG_DUMMY_660 = ((P_log_api << 16) | 660),
	API_CAPI_API_CAPI_MSG_DUMMY_661 = ((P_log_api << 16) | 661),
	API_CAPI_API_CAPI_MSG_DUMMY_662 = ((P_log_api << 16) | 662),
	API_CAPI_API_CAPI_MSG_DUMMY_663 = ((P_log_api << 16) | 663),
	API_CAPI_API_CAPI_MSG_DUMMY_664 = ((P_log_api << 16) | 664),
	API_CAPI_API_CAPI_MSG_DUMMY_665 = ((P_log_api << 16) | 665),
	API_CAPI_API_CAPI_MSG_DUMMY_666 = ((P_log_api << 16) | 666),
	API_CAPI_API_CAPI_MSG_DUMMY_667 = ((P_log_api << 16) | 667),
	API_CAPI_API_CAPI_MSG_DUMMY_668 = ((P_log_api << 16) | 668),
	API_CAPI_API_CAPI_MSG_DUMMY_669 = ((P_log_api << 16) | 669),
	API_CAPI_API_CAPI_MSG_DUMMY_670 = ((P_log_api << 16) | 670),
	API_CAPI_API_CAPI_MSG_DUMMY_671 = ((P_log_api << 16) | 671),
	API_CAPI_API_CAPI_MSG_DUMMY_672 = ((P_log_api << 16) | 672),
	API_CAPI_API_CAPI_MSG_DUMMY_673 = ((P_log_api << 16) | 673),
	API_CAPI_API_CAPI_MSG_DUMMY_674 = ((P_log_api << 16) | 674),
	API_CAPI_API_CAPI_MSG_DUMMY_675 = ((P_log_api << 16) | 675),
	API_CAPI_API_CAPI_MSG_DUMMY_676 = ((P_log_api << 16) | 676),
	API_CAPI_API_CAPI_MSG_DUMMY_677 = ((P_log_api << 16) | 677),
	API_CAPI_API_CAPI_MSG_DUMMY_678 = ((P_log_api << 16) | 678),
	API_CAPI_API_CAPI_MSG_DUMMY_679 = ((P_log_api << 16) | 679),
	API_CAPI_API_CAPI_MSG_DUMMY_680 = ((P_log_api << 16) | 680),
	API_CAPI_API_CAPI_MSG_DUMMY_681 = ((P_log_api << 16) | 681),
	API_CAPI_API_CAPI_MSG_DUMMY_682 = ((P_log_api << 16) | 682),
	API_CAPI_API_CAPI_MSG_DUMMY_683 = ((P_log_api << 16) | 683),
	API_CAPI_API_CAPI_MSG_DUMMY_684 = ((P_log_api << 16) | 684),
	API_CAPI_API_CAPI_MSG_DUMMY_685 = ((P_log_api << 16) | 685),
	API_CAPI_API_CAPI_MSG_DUMMY_686 = ((P_log_api << 16) | 686),
	API_CAPI_API_CAPI_MSG_DUMMY_687 = ((P_log_api << 16) | 687),
	API_CAPI_API_CAPI_MSG_DUMMY_688 = ((P_log_api << 16) | 688),
	API_CAPI_API_CAPI_MSG_DUMMY_689 = ((P_log_api << 16) | 689),
	API_CAPI_API_CAPI_MSG_DUMMY_690 = ((P_log_api << 16) | 690),
	API_CAPI_API_CAPI_MSG_DUMMY_691 = ((P_log_api << 16) | 691),
	API_CAPI_API_CAPI_MSG_DUMMY_692 = ((P_log_api << 16) | 692),
	API_CAPI_API_CAPI_MSG_DUMMY_693 = ((P_log_api << 16) | 693),
	API_CAPI_API_CAPI_MSG_DUMMY_694 = ((P_log_api << 16) | 694),
	API_CAPI_API_CAPI_MSG_DUMMY_695 = ((P_log_api << 16) | 695),
	API_CAPI_API_CAPI_MSG_DUMMY_696 = ((P_log_api << 16) | 696),
	API_CAPI_API_CAPI_MSG_DUMMY_697 = ((P_log_api << 16) | 697),
	API_CAPI_API_CAPI_MSG_DUMMY_698 = ((P_log_api << 16) | 698),
	API_CAPI_API_CAPI_MSG_DUMMY_699 = ((P_log_api << 16) | 699),
	API_CAPI_API_CAPI_MSG_DUMMY_700 = ((P_log_api << 16) | 700),
	API_CAPI_API_CAPI_MSG_DUMMY_701 = ((P_log_api << 16) | 701),
	API_CAPI_API_CAPI_MSG_DUMMY_702 = ((P_log_api << 16) | 702),
	API_CAPI_API_CAPI_MSG_DUMMY_703 = ((P_log_api << 16) | 703),
	API_CAPI_API_CAPI_MSG_DUMMY_704 = ((P_log_api << 16) | 704),
	API_CAPI_API_CAPI_MSG_DUMMY_705 = ((P_log_api << 16) | 705),
	API_CAPI_API_CAPI_MSG_DUMMY_706 = ((P_log_api << 16) | 706),
	API_CAPI_API_CAPI_MSG_DUMMY_707 = ((P_log_api << 16) | 707),
	API_CAPI_API_CAPI_MSG_DUMMY_708 = ((P_log_api << 16) | 708),
	API_CAPI_API_CAPI_MSG_DUMMY_709 = ((P_log_api << 16) | 709),
	API_CAPI_API_CAPI_MSG_DUMMY_710 = ((P_log_api << 16) | 710),
	API_CAPI_API_CAPI_MSG_DUMMY_711 = ((P_log_api << 16) | 711),
	API_CAPI_API_CAPI_MSG_DUMMY_712 = ((P_log_api << 16) | 712),
	API_CAPI_API_CAPI_MSG_DUMMY_713 = ((P_log_api << 16) | 713),
	API_CAPI_API_CAPI_MSG_DUMMY_714 = ((P_log_api << 16) | 714),
	API_CAPI_API_CAPI_MSG_DUMMY_715 = ((P_log_api << 16) | 715),
	API_CAPI_API_CAPI_MSG_DUMMY_716 = ((P_log_api << 16) | 716),
	API_CAPI_API_CAPI_MSG_DUMMY_717 = ((P_log_api << 16) | 717),
	API_CAPI_API_CAPI_MSG_DUMMY_718 = ((P_log_api << 16) | 718),
	API_CAPI_API_CAPI_MSG_DUMMY_719 = ((P_log_api << 16) | 719),
	API_CAPI_API_CAPI_MSG_DUMMY_720 = ((P_log_api << 16) | 720),
	API_CAPI_API_CAPI_MSG_DUMMY_721 = ((P_log_api << 16) | 721),
	API_CAPI_API_CAPI_MSG_DUMMY_722 = ((P_log_api << 16) | 722),
	API_CAPI_API_CAPI_MSG_DUMMY_723 = ((P_log_api << 16) | 723),
	API_CAPI_API_CAPI_MSG_DUMMY_724 = ((P_log_api << 16) | 724),
	API_CAPI_API_CAPI_MSG_DUMMY_725 = ((P_log_api << 16) | 725),
	API_CAPI_API_CAPI_MSG_DUMMY_726 = ((P_log_api << 16) | 726),
	API_CAPI_API_CAPI_MSG_DUMMY_727 = ((P_log_api << 16) | 727),
	API_CAPI_API_CAPI_MSG_DUMMY_728 = ((P_log_api << 16) | 728),
	API_CAPI_API_CAPI_MSG_DUMMY_729 = ((P_log_api << 16) | 729),
	API_CAPI_API_CAPI_MSG_DUMMY_730 = ((P_log_api << 16) | 730),
	API_CAPI_API_CAPI_MSG_DUMMY_731 = ((P_log_api << 16) | 731),
	API_CAPI_API_CAPI_MSG_DUMMY_732 = ((P_log_api << 16) | 732),
	API_CAPI_API_CAPI_MSG_DUMMY_733 = ((P_log_api << 16) | 733),
	API_CAPI_API_CAPI_MSG_DUMMY_734 = ((P_log_api << 16) | 734),
	API_CAPI_API_CAPI_MSG_DUMMY_735 = ((P_log_api << 16) | 735),
	API_CAPI_API_CAPI_MSG_DUMMY_736 = ((P_log_api << 16) | 736),
	API_CAPI_API_CAPI_MSG_DUMMY_737 = ((P_log_api << 16) | 737),
	API_CAPI_MSG_CMP_PTR_CC_ELEM_AUTO_REJECT_SWITCH = ((P_log_api << 16) | 738),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_CMP_PTR_CC_ELEM_END_CALL_CAUSE = ((P_log_api << 16) | 739),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_API_CAPI_MSG_DUMMY_740 = ((P_log_api << 16) | 740),
	API_CAPI_API_CAPI_MSG_DUMMY_741 = ((P_log_api << 16) | 741),
	API_CAPI_API_CAPI_MSG_DUMMY_742 = ((P_log_api << 16) | 742),
	API_CAPI_API_CAPI_MSG_DUMMY_743 = ((P_log_api << 16) | 743),
	API_CAPI_API_CAPI_MSG_DUMMY_744 = ((P_log_api << 16) | 744),
	API_CAPI_API_CAPI_MSG_DUMMY_745 = ((P_log_api << 16) | 745),
	API_CAPI_API_CAPI_MSG_DUMMY_746 = ((P_log_api << 16) | 746),
	API_CAPI_API_CAPI_MSG_DUMMY_747 = ((P_log_api << 16) | 747),
	API_CAPI_API_CAPI_MSG_DUMMY_748 = ((P_log_api << 16) | 748),
	API_CAPI_API_CAPI_MSG_DUMMY_749 = ((P_log_api << 16) | 749),
	API_CAPI_API_CAPI_MSG_DUMMY_750 = ((P_log_api << 16) | 750),
	API_CAPI_API_CAPI_MSG_DUMMY_751 = ((P_log_api << 16) | 751),
	API_CAPI_API_CAPI_MSG_DUMMY_752 = ((P_log_api << 16) | 752),
	API_CAPI_API_CAPI_MSG_DUMMY_753 = ((P_log_api << 16) | 753),
	API_CAPI_API_CAPI_MSG_DUMMY_754 = ((P_log_api << 16) | 754),
	API_CAPI_API_CAPI_MSG_DUMMY_755 = ((P_log_api << 16) | 755),
	API_CAPI_API_CAPI_MSG_DUMMY_756 = ((P_log_api << 16) | 756),
	API_CAPI_API_CAPI_MSG_DUMMY_757 = ((P_log_api << 16) | 757),
	API_CAPI_API_CAPI_MSG_DUMMY_758 = ((P_log_api << 16) | 758),
	API_CAPI_API_CAPI_MSG_DUMMY_759 = ((P_log_api << 16) | 759),
	API_CAPI_API_CAPI_MSG_DUMMY_760 = ((P_log_api << 16) | 760),
	API_CAPI_API_CAPI_MSG_DUMMY_761 = ((P_log_api << 16) | 761),
	API_CAPI_API_CAPI_MSG_DUMMY_762 = ((P_log_api << 16) | 762),
	API_CAPI_API_CAPI_MSG_DUMMY_763 = ((P_log_api << 16) | 763),
	API_CAPI_API_CAPI_MSG_DUMMY_764 = ((P_log_api << 16) | 764),
	API_CAPI_API_CAPI_MSG_DUMMY_765 = ((P_log_api << 16) | 765),
	API_CAPI_API_CAPI_MSG_DUMMY_766 = ((P_log_api << 16) | 766),
	API_CAPI_API_CAPI_MSG_DUMMY_767 = ((P_log_api << 16) | 767),
	API_CAPI_API_CAPI_MSG_DUMMY_768 = ((P_log_api << 16) | 768),
	API_CAPI_API_CAPI_MSG_DUMMY_769 = ((P_log_api << 16) | 769),
	API_CAPI_API_CAPI_MSG_DUMMY_770 = ((P_log_api << 16) | 770),
	API_CAPI_API_CAPI_MSG_DUMMY_771 = ((P_log_api << 16) | 771),
	API_CAPI_API_CAPI_MSG_DUMMY_772 = ((P_log_api << 16) | 772),
	API_CAPI_API_CAPI_MSG_DUMMY_773 = ((P_log_api << 16) | 773),
	API_CAPI_API_CAPI_MSG_DUMMY_774 = ((P_log_api << 16) | 774),
	API_CAPI_API_CAPI_MSG_DUMMY_775 = ((P_log_api << 16) | 775),
	API_CAPI_API_CAPI_MSG_DUMMY_776 = ((P_log_api << 16) | 776),
	API_CAPI_API_CAPI_MSG_DUMMY_777 = ((P_log_api << 16) | 777),
	API_CAPI_API_CAPI_MSG_DUMMY_778 = ((P_log_api << 16) | 778),
	API_CAPI_API_CAPI_MSG_DUMMY_779 = ((P_log_api << 16) | 779),
	API_CAPI_API_CAPI_MSG_DUMMY_780 = ((P_log_api << 16) | 780),
	API_CAPI_API_CAPI_MSG_DUMMY_781 = ((P_log_api << 16) | 781),
	API_CAPI_API_CAPI_MSG_DUMMY_782 = ((P_log_api << 16) | 782),
	API_CAPI_API_CAPI_MSG_DUMMY_783 = ((P_log_api << 16) | 783),
	API_CAPI_API_CAPI_MSG_DUMMY_784 = ((P_log_api << 16) | 784),
	API_CAPI_API_CAPI_MSG_DUMMY_785 = ((P_log_api << 16) | 785),
	API_CAPI_API_CAPI_MSG_DUMMY_786 = ((P_log_api << 16) | 786),
	API_CAPI_API_CAPI_MSG_DUMMY_787 = ((P_log_api << 16) | 787),
	API_CAPI_API_CAPI_MSG_DUMMY_788 = ((P_log_api << 16) | 788),
	API_CAPI_API_CAPI_MSG_DUMMY_789 = ((P_log_api << 16) | 789),
	API_CAPI_API_CAPI_MSG_DUMMY_790 = ((P_log_api << 16) | 790),
	API_CAPI_API_CAPI_MSG_DUMMY_791 = ((P_log_api << 16) | 791),
	API_CAPI_API_CAPI_MSG_DUMMY_792 = ((P_log_api << 16) | 792),
	API_CAPI_API_CAPI_MSG_DUMMY_793 = ((P_log_api << 16) | 793),
	API_CAPI_API_CAPI_MSG_DUMMY_794 = ((P_log_api << 16) | 794),
	API_CAPI_API_CAPI_MSG_DUMMY_795 = ((P_log_api << 16) | 795),
	API_CAPI_API_CAPI_MSG_DUMMY_796 = ((P_log_api << 16) | 796),
	API_CAPI_API_CAPI_MSG_DUMMY_797 = ((P_log_api << 16) | 797),
	API_CAPI_API_CAPI_MSG_DUMMY_798 = ((P_log_api << 16) | 798),
	API_CAPI_API_CAPI_MSG_DUMMY_799 = ((P_log_api << 16) | 799),
	API_CAPI_API_CAPI_MSG_DUMMY_800 = ((P_log_api << 16) | 800),
	API_CAPI_API_CAPI_MSG_DUMMY_801 = ((P_log_api << 16) | 801),
	API_CAPI_API_CAPI_MSG_DUMMY_802 = ((P_log_api << 16) | 802),
	API_CAPI_API_CAPI_MSG_DUMMY_803 = ((P_log_api << 16) | 803),
	API_CAPI_API_CAPI_MSG_DUMMY_804 = ((P_log_api << 16) | 804),
	API_CAPI_API_CAPI_MSG_DUMMY_805 = ((P_log_api << 16) | 805),
	API_CAPI_API_CAPI_MSG_DUMMY_806 = ((P_log_api << 16) | 806),
	API_CAPI_API_CAPI_MSG_DUMMY_807 = ((P_log_api << 16) | 807),
	API_CAPI_API_CAPI_MSG_DUMMY_808 = ((P_log_api << 16) | 808),
	API_CAPI_API_CAPI_MSG_DUMMY_809 = ((P_log_api << 16) | 809),
	API_CAPI_API_CAPI_MSG_DUMMY_810 = ((P_log_api << 16) | 810),
	API_CAPI_API_CAPI_MSG_DUMMY_811 = ((P_log_api << 16) | 811),
	API_CAPI_API_CAPI_MSG_DUMMY_812 = ((P_log_api << 16) | 812),
	API_CAPI_API_CAPI_MSG_DUMMY_813 = ((P_log_api << 16) | 813),
	API_CAPI_API_CAPI_MSG_DUMMY_814 = ((P_log_api << 16) | 814),
	API_CAPI_API_CAPI_MSG_DUMMY_815 = ((P_log_api << 16) | 815),
	API_CAPI_API_CAPI_MSG_DUMMY_816 = ((P_log_api << 16) | 816),
	API_CAPI_API_CAPI_MSG_DUMMY_817 = ((P_log_api << 16) | 817),
	API_CAPI_API_CAPI_MSG_DUMMY_818 = ((P_log_api << 16) | 818),
	API_CAPI_API_CAPI_MSG_DUMMY_819 = ((P_log_api << 16) | 819),
	API_CAPI_API_CAPI_MSG_DUMMY_820 = ((P_log_api << 16) | 820),
	API_CAPI_API_CAPI_MSG_DUMMY_821 = ((P_log_api << 16) | 821),
	API_CAPI_API_CAPI_MSG_DUMMY_822 = ((P_log_api << 16) | 822),
	API_CAPI_API_CAPI_MSG_DUMMY_823 = ((P_log_api << 16) | 823),
	API_CAPI_API_CAPI_MSG_DUMMY_824 = ((P_log_api << 16) | 824),
	API_CAPI_API_CAPI_MSG_DUMMY_825 = ((P_log_api << 16) | 825),
	API_CAPI_API_CAPI_MSG_DUMMY_826 = ((P_log_api << 16) | 826),
	API_CAPI_API_CAPI_MSG_DUMMY_827 = ((P_log_api << 16) | 827),
	API_CAPI_API_CAPI_MSG_DUMMY_828 = ((P_log_api << 16) | 828),
	API_CAPI_API_CAPI_MSG_DUMMY_829 = ((P_log_api << 16) | 829),
	API_CAPI_API_CAPI_MSG_DUMMY_830 = ((P_log_api << 16) | 830),
	API_CAPI_API_CAPI_MSG_DUMMY_831 = ((P_log_api << 16) | 831),
	API_CAPI_API_CAPI_MSG_DUMMY_832 = ((P_log_api << 16) | 832),
	API_CAPI_API_CAPI_MSG_DUMMY_833 = ((P_log_api << 16) | 833),
	API_CAPI_API_CAPI_MSG_DUMMY_834 = ((P_log_api << 16) | 834),
	API_CAPI_API_CAPI_MSG_DUMMY_835 = ((P_log_api << 16) | 835),
	API_CAPI_API_CAPI_MSG_DUMMY_836 = ((P_log_api << 16) | 836),
	API_CAPI_API_CAPI_MSG_DUMMY_837 = ((P_log_api << 16) | 837),
	API_CAPI_API_CAPI_MSG_DUMMY_838 = ((P_log_api << 16) | 838),
	API_CAPI_API_CAPI_MSG_DUMMY_839 = ((P_log_api << 16) | 839),
	API_CAPI_API_CAPI_MSG_DUMMY_840 = ((P_log_api << 16) | 840),
	API_CAPI_API_CAPI_MSG_DUMMY_841 = ((P_log_api << 16) | 841),
	API_CAPI_API_CAPI_MSG_DUMMY_842 = ((P_log_api << 16) | 842),
	API_CAPI_API_CAPI_MSG_DUMMY_843 = ((P_log_api << 16) | 843),
	API_CAPI_API_CAPI_MSG_DUMMY_844 = ((P_log_api << 16) | 844),
	API_CAPI_API_CAPI_MSG_DUMMY_845 = ((P_log_api << 16) | 845),
	API_CAPI_API_CAPI_MSG_DUMMY_846 = ((P_log_api << 16) | 846),
	API_CAPI_API_CAPI_MSG_DUMMY_847 = ((P_log_api << 16) | 847),
	API_CAPI_API_CAPI_MSG_DUMMY_848 = ((P_log_api << 16) | 848),
	API_CAPI_API_CAPI_MSG_DUMMY_849 = ((P_log_api << 16) | 849),
	API_CAPI_API_CAPI_MSG_DUMMY_850 = ((P_log_api << 16) | 850),
	API_CAPI_API_CAPI_MSG_DUMMY_851 = ((P_log_api << 16) | 851),
	API_CAPI_API_CAPI_MSG_DUMMY_852 = ((P_log_api << 16) | 852),
	API_CAPI_API_CAPI_MSG_DUMMY_853 = ((P_log_api << 16) | 853),
	API_CAPI_API_CAPI_MSG_DUMMY_854 = ((P_log_api << 16) | 854),
	API_CAPI_API_CAPI_MSG_DUMMY_855 = ((P_log_api << 16) | 855),
	API_CAPI_API_CAPI_MSG_DUMMY_856 = ((P_log_api << 16) | 856),
	API_CAPI_API_CAPI_MSG_DUMMY_857 = ((P_log_api << 16) | 857),
	API_CAPI_API_CAPI_MSG_DUMMY_858 = ((P_log_api << 16) | 858),
	API_CAPI_API_CAPI_MSG_DUMMY_859 = ((P_log_api << 16) | 859),
	API_CAPI_API_CAPI_MSG_DUMMY_860 = ((P_log_api << 16) | 860),
	API_CAPI_API_CAPI_MSG_DUMMY_861 = ((P_log_api << 16) | 861),
	API_CAPI_API_CAPI_MSG_DUMMY_862 = ((P_log_api << 16) | 862),
	API_CAPI_API_CAPI_MSG_DUMMY_863 = ((P_log_api << 16) | 863),
	API_CAPI_API_CAPI_MSG_DUMMY_864 = ((P_log_api << 16) | 864),
	API_CAPI_API_CAPI_MSG_DUMMY_865 = ((P_log_api << 16) | 865),
	API_CAPI_API_CAPI_MSG_DUMMY_866 = ((P_log_api << 16) | 866),
	API_CAPI_API_CAPI_MSG_DUMMY_867 = ((P_log_api << 16) | 867),
	API_CAPI_API_CAPI_MSG_DUMMY_868 = ((P_log_api << 16) | 868),
	API_CAPI_API_CAPI_MSG_DUMMY_869 = ((P_log_api << 16) | 869),
	API_CAPI_API_CAPI_MSG_DUMMY_870 = ((P_log_api << 16) | 870),
	API_CAPI_MSG_SS_SENDCALLFORWARDREQ_REQ = ((P_log_api << 16) | 871),	///<Payload type {::T_SS_SendCallForwardReq}
	API_CAPI_MSG_SS_QUERYCALLFORWARDSTATUS_REQ = ((P_log_api << 16) | 872),	///<Payload type {::T_SS_QueryCallForwardStatus}
	API_CAPI_MSG_SS_SENDCALLBARRINGREQ_REQ = ((P_log_api << 16) | 873),	///<Payload type {::T_SS_SendCallBarringReq}
	API_CAPI_MSG_SS_QUERYCALLBARRINGSTATUS_REQ = ((P_log_api << 16) | 874),	///<Payload type {::T_SS_QueryCallBarringStatus}
	API_CAPI_MSG_SS_SENDCALLBARRINGPWDCHANGEREQ_REQ = ((P_log_api << 16) | 875),	///<Payload type {::T_SS_SendCallBarringPWDChangeReq}
	API_CAPI_MSG_SS_SENDCALLWAITINGREQ_REQ = ((P_log_api << 16) | 876),	///<Payload type {::T_SS_SendCallWaitingReq}
	API_CAPI_MSG_SS_QUERYCALLWAITINGSTATUS_REQ = ((P_log_api << 16) | 877),	///<Payload type {::T_SS_QueryCallWaitingStatus}
	API_CAPI_API_CAPI_MSG_DUMMY_878 = ((P_log_api << 16) | 878),
	API_CAPI_API_CAPI_MSG_DUMMY_879 = ((P_log_api << 16) | 879),
	API_CAPI_API_CAPI_MSG_DUMMY_880 = ((P_log_api << 16) | 880),
	API_CAPI_API_CAPI_MSG_DUMMY_881 = ((P_log_api << 16) | 881),
	API_CAPI_API_CAPI_MSG_DUMMY_882 = ((P_log_api << 16) | 882),
	API_CAPI_MSG_SS_SETCALLINGLINEIDSTATUS_REQ = ((P_log_api << 16) | 883),	///<Payload type {::T_SS_SetCallingLineIDStatus}
	API_CAPI_MSG_SS_SETCALLINGLINERESTRICTIONSTATUS_REQ = ((P_log_api << 16) | 884),	///<Payload type {::T_SS_SetCallingLineRestrictionStatus}
	API_CAPI_MSG_SS_SETCONNECTEDLINEIDSTATUS_REQ = ((P_log_api << 16) | 885),	///<Payload type {::T_SS_SetConnectedLineIDStatus}
	API_CAPI_MSG_SS_SETCONNECTEDLINERESTRICTIONSTATUS_REQ = ((P_log_api << 16) | 886),	///<Payload type {::T_SS_SetConnectedLineRestrictionStatus}
	API_CAPI_MSG_SS_SENDUSSDCONNECTREQ_REQ = ((P_log_api << 16) | 887),	///<Payload type {::T_SS_SendUSSDConnectReq}
	API_CAPI_MSG_SS_SENDUSSDDATA_REQ = ((P_log_api << 16) | 888),	///<Payload type {::T_SS_SendUSSDData}
	API_CAPI_MSG_SSAPI_DIALSTRSRVREQ_REQ = ((P_log_api << 16) | 889),	///<Payload type {::T_SsApi_DialStrSrvReq}
	API_CAPI_MSG_SS_ENDUSSDCONNECTREQ_REQ = ((P_log_api << 16) | 890),	///<Payload type {::T_SS_EndUSSDConnectReq}
	API_CAPI_MSG_SSAPI_SSSRVREQ_REQ = ((P_log_api << 16) | 891),	///<Payload type {::T_SsApi_SsSrvReq}
	API_CAPI_MSG_SSAPI_USSDSRVREQ_REQ = ((P_log_api << 16) | 892),	///<Payload type {::T_SsApi_UssdSrvReq}
	API_CAPI_MSG_SSAPI_USSDDATAREQ_REQ = ((P_log_api << 16) | 893),	///<Payload type {::T_SsApi_UssdDataReq}
	API_CAPI_MSG_SSAPI_SSRELEASEREQ_REQ = ((P_log_api << 16) | 894),	///<Payload type {::T_SsApi_SsReleaseReq}
	API_CAPI_MSG_SSAPI_DATAREQ_REQ = ((P_log_api << 16) | 895),	///<Payload type {::T_SsApi_DataReq}
	API_CAPI_MSG_SSAPI_DISPATCH_REQ = ((P_log_api << 16) | 896),	///<Payload type {::T_SS_SsApiReqDispatcher}
	API_CAPI_MSG_SS_GET_STR_REQ = ((P_log_api << 16) | 897),	///<Payload type {::T_SS_GetStr}
	API_CAPI_API_CAPI_MSG_DUMMY_898 = ((P_log_api << 16) | 898),
	API_CAPI_API_CAPI_MSG_DUMMY_899 = ((P_log_api << 16) | 899),
	API_CAPI_API_CAPI_MSG_DUMMY_900 = ((P_log_api << 16) | 900),
	API_CAPI_API_CAPI_MSG_DUMMY_901 = ((P_log_api << 16) | 901),
	API_CAPI_API_CAPI_MSG_DUMMY_902 = ((P_log_api << 16) | 902),
	API_CAPI_API_CAPI_MSG_DUMMY_903 = ((P_log_api << 16) | 903),
	API_CAPI_API_CAPI_MSG_DUMMY_904 = ((P_log_api << 16) | 904),
	API_CAPI_API_CAPI_MSG_DUMMY_905 = ((P_log_api << 16) | 905),
	API_CAPI_API_CAPI_MSG_DUMMY_906 = ((P_log_api << 16) | 906),
	API_CAPI_API_CAPI_MSG_DUMMY_907 = ((P_log_api << 16) | 907),
	API_CAPI_API_CAPI_MSG_DUMMY_908 = ((P_log_api << 16) | 908),
	API_CAPI_API_CAPI_MSG_DUMMY_909 = ((P_log_api << 16) | 909),
	API_CAPI_API_CAPI_MSG_DUMMY_910 = ((P_log_api << 16) | 910),
	API_CAPI_API_CAPI_MSG_DUMMY_911 = ((P_log_api << 16) | 911),
	API_CAPI_API_CAPI_MSG_DUMMY_912 = ((P_log_api << 16) | 912),
	API_CAPI_API_CAPI_MSG_DUMMY_913 = ((P_log_api << 16) | 913),
	API_CAPI_API_CAPI_MSG_DUMMY_914 = ((P_log_api << 16) | 914),
	API_CAPI_API_CAPI_MSG_DUMMY_915 = ((P_log_api << 16) | 915),
	API_CAPI_API_CAPI_MSG_DUMMY_916 = ((P_log_api << 16) | 916),
	API_CAPI_API_CAPI_MSG_DUMMY_917 = ((P_log_api << 16) | 917),
	API_CAPI_API_CAPI_MSG_DUMMY_918 = ((P_log_api << 16) | 918),
	API_CAPI_API_CAPI_MSG_DUMMY_919 = ((P_log_api << 16) | 919),
	API_CAPI_API_CAPI_MSG_DUMMY_920 = ((P_log_api << 16) | 920),
	API_CAPI_API_CAPI_MSG_DUMMY_921 = ((P_log_api << 16) | 921),
	API_CAPI_API_CAPI_MSG_DUMMY_922 = ((P_log_api << 16) | 922),
	API_CAPI_API_CAPI_MSG_DUMMY_923 = ((P_log_api << 16) | 923),
	API_CAPI_API_CAPI_MSG_DUMMY_924 = ((P_log_api << 16) | 924),
	API_CAPI_API_CAPI_MSG_DUMMY_925 = ((P_log_api << 16) | 925),
	API_CAPI_API_CAPI_MSG_DUMMY_926 = ((P_log_api << 16) | 926),
	API_CAPI_API_CAPI_MSG_DUMMY_927 = ((P_log_api << 16) | 927),
	API_CAPI_API_CAPI_MSG_DUMMY_928 = ((P_log_api << 16) | 928),
	API_CAPI_API_CAPI_MSG_DUMMY_929 = ((P_log_api << 16) | 929),
	API_CAPI_API_CAPI_MSG_DUMMY_930 = ((P_log_api << 16) | 930),
	API_CAPI_API_CAPI_MSG_DUMMY_931 = ((P_log_api << 16) | 931),
	API_CAPI_API_CAPI_MSG_DUMMY_932 = ((P_log_api << 16) | 932),
	API_CAPI_API_CAPI_MSG_DUMMY_933 = ((P_log_api << 16) | 933),
	API_CAPI_API_CAPI_MSG_DUMMY_934 = ((P_log_api << 16) | 934),
	API_CAPI_API_CAPI_MSG_DUMMY_935 = ((P_log_api << 16) | 935),
	API_CAPI_API_CAPI_MSG_DUMMY_936 = ((P_log_api << 16) | 936),
	API_CAPI_API_CAPI_MSG_DUMMY_937 = ((P_log_api << 16) | 937),
	API_CAPI_API_CAPI_MSG_DUMMY_938 = ((P_log_api << 16) | 938),
	API_CAPI_API_CAPI_MSG_DUMMY_939 = ((P_log_api << 16) | 939),
	API_CAPI_API_CAPI_MSG_DUMMY_940 = ((P_log_api << 16) | 940),
	API_CAPI_API_CAPI_MSG_DUMMY_941 = ((P_log_api << 16) | 941),
	API_CAPI_API_CAPI_MSG_DUMMY_942 = ((P_log_api << 16) | 942),
	API_CAPI_API_CAPI_MSG_DUMMY_943 = ((P_log_api << 16) | 943),
	API_CAPI_API_CAPI_MSG_DUMMY_944 = ((P_log_api << 16) | 944),
	API_CAPI_API_CAPI_MSG_DUMMY_945 = ((P_log_api << 16) | 945),
	API_CAPI_API_CAPI_MSG_DUMMY_946 = ((P_log_api << 16) | 946),
	API_CAPI_API_CAPI_MSG_DUMMY_947 = ((P_log_api << 16) | 947),
	API_CAPI_API_CAPI_MSG_DUMMY_948 = ((P_log_api << 16) | 948),
	API_CAPI_API_CAPI_MSG_DUMMY_949 = ((P_log_api << 16) | 949),
	API_CAPI_API_CAPI_MSG_DUMMY_950 = ((P_log_api << 16) | 950),
	API_CAPI_API_CAPI_MSG_DUMMY_951 = ((P_log_api << 16) | 951),
	API_CAPI_API_CAPI_MSG_DUMMY_952 = ((P_log_api << 16) | 952),
	API_CAPI_API_CAPI_MSG_DUMMY_953 = ((P_log_api << 16) | 953),
	API_CAPI_API_CAPI_MSG_DUMMY_954 = ((P_log_api << 16) | 954),
	API_CAPI_API_CAPI_MSG_DUMMY_955 = ((P_log_api << 16) | 955),
	API_CAPI_API_CAPI_MSG_DUMMY_956 = ((P_log_api << 16) | 956),
	API_CAPI_API_CAPI_MSG_DUMMY_957 = ((P_log_api << 16) | 957),
	API_CAPI_API_CAPI_MSG_DUMMY_958 = ((P_log_api << 16) | 958),
	API_CAPI_API_CAPI_MSG_DUMMY_959 = ((P_log_api << 16) | 959),
	API_CAPI_API_CAPI_MSG_DUMMY_960 = ((P_log_api << 16) | 960),
	API_CAPI_API_CAPI_MSG_DUMMY_961 = ((P_log_api << 16) | 961),
	API_CAPI_API_CAPI_MSG_DUMMY_962 = ((P_log_api << 16) | 962),
	API_CAPI_API_CAPI_MSG_DUMMY_963 = ((P_log_api << 16) | 963),
	API_CAPI_API_CAPI_MSG_DUMMY_964 = ((P_log_api << 16) | 964),
	API_CAPI_API_CAPI_MSG_DUMMY_965 = ((P_log_api << 16) | 965),
	API_CAPI_API_CAPI_MSG_DUMMY_966 = ((P_log_api << 16) | 966),
	API_CAPI_API_CAPI_MSG_DUMMY_967 = ((P_log_api << 16) | 967),
	API_CAPI_API_CAPI_MSG_DUMMY_968 = ((P_log_api << 16) | 968),
	API_CAPI_API_CAPI_MSG_DUMMY_969 = ((P_log_api << 16) | 969),
	API_CAPI_API_CAPI_MSG_DUMMY_970 = ((P_log_api << 16) | 970),
	API_CAPI_API_CAPI_MSG_DUMMY_971 = ((P_log_api << 16) | 971),
	API_CAPI_API_CAPI_MSG_DUMMY_972 = ((P_log_api << 16) | 972),
	API_CAPI_API_CAPI_MSG_DUMMY_973 = ((P_log_api << 16) | 973),
	API_CAPI_API_CAPI_MSG_DUMMY_974 = ((P_log_api << 16) | 974),
	API_CAPI_API_CAPI_MSG_DUMMY_975 = ((P_log_api << 16) | 975),
	API_CAPI_API_CAPI_MSG_DUMMY_976 = ((P_log_api << 16) | 976),
	API_CAPI_API_CAPI_MSG_DUMMY_977 = ((P_log_api << 16) | 977),
	API_CAPI_API_CAPI_MSG_DUMMY_978 = ((P_log_api << 16) | 978),
	API_CAPI_API_CAPI_MSG_DUMMY_979 = ((P_log_api << 16) | 979),
	API_CAPI_API_CAPI_MSG_DUMMY_980 = ((P_log_api << 16) | 980),
	API_CAPI_API_CAPI_MSG_DUMMY_981 = ((P_log_api << 16) | 981),
	API_CAPI_API_CAPI_MSG_DUMMY_982 = ((P_log_api << 16) | 982),
	API_CAPI_API_CAPI_MSG_DUMMY_983 = ((P_log_api << 16) | 983),
	API_CAPI_API_CAPI_MSG_DUMMY_984 = ((P_log_api << 16) | 984),
	API_CAPI_API_CAPI_MSG_DUMMY_985 = ((P_log_api << 16) | 985),
	API_CAPI_API_CAPI_MSG_DUMMY_986 = ((P_log_api << 16) | 986),
	API_CAPI_API_CAPI_MSG_DUMMY_987 = ((P_log_api << 16) | 987),
	API_CAPI_API_CAPI_MSG_DUMMY_988 = ((P_log_api << 16) | 988),
	API_CAPI_API_CAPI_MSG_DUMMY_989 = ((P_log_api << 16) | 989),
	API_CAPI_API_CAPI_MSG_DUMMY_990 = ((P_log_api << 16) | 990),
	API_CAPI_API_CAPI_MSG_DUMMY_991 = ((P_log_api << 16) | 991),
	API_CAPI_API_CAPI_MSG_DUMMY_992 = ((P_log_api << 16) | 992),
	API_CAPI_API_CAPI_MSG_DUMMY_993 = ((P_log_api << 16) | 993),
	API_CAPI_API_CAPI_MSG_DUMMY_994 = ((P_log_api << 16) | 994),
	API_CAPI_API_CAPI_MSG_DUMMY_995 = ((P_log_api << 16) | 995),
	API_CAPI_API_CAPI_MSG_DUMMY_996 = ((P_log_api << 16) | 996),
	API_CAPI_API_CAPI_MSG_DUMMY_997 = ((P_log_api << 16) | 997),
	API_CAPI_API_CAPI_MSG_DUMMY_998 = ((P_log_api << 16) | 998),
	API_CAPI_API_CAPI_MSG_DUMMY_999 = ((P_log_api << 16) | 999),
	API_CAPI_API_CAPI_MSG_DUMMY_1000 = ((P_log_api << 16) | 1000),
	API_CAPI_API_CAPI_MSG_DUMMY_1001 = ((P_log_api << 16) | 1001),
	API_CAPI_API_CAPI_MSG_DUMMY_1002 = ((P_log_api << 16) | 1002),
	API_CAPI_API_CAPI_MSG_DUMMY_1003 = ((P_log_api << 16) | 1003),
	API_CAPI_API_CAPI_MSG_DUMMY_1004 = ((P_log_api << 16) | 1004),
	API_CAPI_API_CAPI_MSG_DUMMY_1005 = ((P_log_api << 16) | 1005),
	API_CAPI_API_CAPI_MSG_DUMMY_1006 = ((P_log_api << 16) | 1006),
	API_CAPI_API_CAPI_MSG_DUMMY_1007 = ((P_log_api << 16) | 1007),
	API_CAPI_API_CAPI_MSG_DUMMY_1008 = ((P_log_api << 16) | 1008),
	API_CAPI_API_CAPI_MSG_DUMMY_1009 = ((P_log_api << 16) | 1009),
	API_CAPI_API_CAPI_MSG_DUMMY_1010 = ((P_log_api << 16) | 1010),
	API_CAPI_API_CAPI_MSG_DUMMY_1011 = ((P_log_api << 16) | 1011),
	API_CAPI_API_CAPI_MSG_DUMMY_1012 = ((P_log_api << 16) | 1012),
	API_CAPI_API_CAPI_MSG_DUMMY_1013 = ((P_log_api << 16) | 1013),
	API_CAPI_API_CAPI_MSG_DUMMY_1014 = ((P_log_api << 16) | 1014),
	API_CAPI_API_CAPI_MSG_DUMMY_1015 = ((P_log_api << 16) | 1015),
	API_CAPI_API_CAPI_MSG_DUMMY_1016 = ((P_log_api << 16) | 1016),
	API_CAPI_API_CAPI_MSG_DUMMY_1017 = ((P_log_api << 16) | 1017),
	API_CAPI_API_CAPI_MSG_DUMMY_1018 = ((P_log_api << 16) | 1018),
	API_CAPI_API_CAPI_MSG_DUMMY_1019 = ((P_log_api << 16) | 1019),
	API_CAPI_API_CAPI_MSG_DUMMY_1020 = ((P_log_api << 16) | 1020),
	API_CAPI_API_CAPI_MSG_DUMMY_1021 = ((P_log_api << 16) | 1021),
	API_CAPI_API_CAPI_MSG_DUMMY_1022 = ((P_log_api << 16) | 1022),
	API_CAPI_API_CAPI_MSG_DUMMY_1023 = ((P_log_api << 16) | 1023),
	API_CAPI_API_CAPI_MSG_DUMMY_1024 = ((P_log_api << 16) | 1024),
	API_CAPI_API_CAPI_MSG_DUMMY_1025 = ((P_log_api << 16) | 1025),
	API_CAPI_API_CAPI_MSG_DUMMY_1026 = ((P_log_api << 16) | 1026),
	API_CAPI_API_CAPI_MSG_DUMMY_1027 = ((P_log_api << 16) | 1027),
	API_CAPI_API_CAPI_MSG_DUMMY_1028 = ((P_log_api << 16) | 1028),
	API_CAPI_API_CAPI_MSG_DUMMY_1029 = ((P_log_api << 16) | 1029),
	API_CAPI_API_CAPI_MSG_DUMMY_1030 = ((P_log_api << 16) | 1030),
	API_CAPI_API_CAPI_MSG_DUMMY_1031 = ((P_log_api << 16) | 1031),
	API_CAPI_API_CAPI_MSG_DUMMY_1032 = ((P_log_api << 16) | 1032),
	API_CAPI_API_CAPI_MSG_DUMMY_1033 = ((P_log_api << 16) | 1033),
	API_CAPI_API_CAPI_MSG_DUMMY_1034 = ((P_log_api << 16) | 1034),
	API_CAPI_API_CAPI_MSG_DUMMY_1035 = ((P_log_api << 16) | 1035),
	API_CAPI_API_CAPI_MSG_DUMMY_1036 = ((P_log_api << 16) | 1036),
	API_CAPI_API_CAPI_MSG_DUMMY_1037 = ((P_log_api << 16) | 1037),
	API_CAPI_API_CAPI_MSG_DUMMY_1038 = ((P_log_api << 16) | 1038),
	API_CAPI_API_CAPI_MSG_DUMMY_1039 = ((P_log_api << 16) | 1039),
	API_CAPI_API_CAPI_MSG_DUMMY_1040 = ((P_log_api << 16) | 1040),
	API_CAPI_API_CAPI_MSG_DUMMY_1041 = ((P_log_api << 16) | 1041),
	API_CAPI_API_CAPI_MSG_DUMMY_1042 = ((P_log_api << 16) | 1042),
	API_CAPI_API_CAPI_MSG_DUMMY_1043 = ((P_log_api << 16) | 1043),
	API_CAPI_API_CAPI_MSG_DUMMY_1044 = ((P_log_api << 16) | 1044),
	API_CAPI_API_CAPI_MSG_DUMMY_1045 = ((P_log_api << 16) | 1045),
	API_CAPI_API_CAPI_MSG_DUMMY_1046 = ((P_log_api << 16) | 1046),
	API_CAPI_API_CAPI_MSG_DUMMY_1047 = ((P_log_api << 16) | 1047),
	API_CAPI_API_CAPI_MSG_DUMMY_1048 = ((P_log_api << 16) | 1048),
	API_CAPI_API_CAPI_MSG_DUMMY_1049 = ((P_log_api << 16) | 1049),
	API_CAPI_API_CAPI_MSG_DUMMY_1050 = ((P_log_api << 16) | 1050),
	API_CAPI_API_CAPI_MSG_DUMMY_1051 = ((P_log_api << 16) | 1051),
	API_CAPI_API_CAPI_MSG_DUMMY_1052 = ((P_log_api << 16) | 1052),
	API_CAPI_API_CAPI_MSG_DUMMY_1053 = ((P_log_api << 16) | 1053),
	API_CAPI_API_CAPI_MSG_DUMMY_1054 = ((P_log_api << 16) | 1054),
	API_CAPI_API_CAPI_MSG_DUMMY_1055 = ((P_log_api << 16) | 1055),
	API_CAPI_API_CAPI_MSG_DUMMY_1056 = ((P_log_api << 16) | 1056),
	API_CAPI_API_CAPI_MSG_DUMMY_1057 = ((P_log_api << 16) | 1057),
	API_CAPI_API_CAPI_MSG_DUMMY_1058 = ((P_log_api << 16) | 1058),
	API_CAPI_API_CAPI_MSG_DUMMY_1059 = ((P_log_api << 16) | 1059),
	API_CAPI_API_CAPI_MSG_DUMMY_1060 = ((P_log_api << 16) | 1060),
	API_CAPI_API_CAPI_MSG_DUMMY_1061 = ((P_log_api << 16) | 1061),
	API_CAPI_API_CAPI_MSG_DUMMY_1062 = ((P_log_api << 16) | 1062),
	API_CAPI_API_CAPI_MSG_DUMMY_1063 = ((P_log_api << 16) | 1063),
	API_CAPI_API_CAPI_MSG_DUMMY_1064 = ((P_log_api << 16) | 1064),
	API_CAPI_API_CAPI_MSG_DUMMY_1065 = ((P_log_api << 16) | 1065),
	API_CAPI_API_CAPI_MSG_DUMMY_1066 = ((P_log_api << 16) | 1066),
	API_CAPI_API_CAPI_MSG_DUMMY_1067 = ((P_log_api << 16) | 1067),
	API_CAPI_API_CAPI_MSG_DUMMY_1068 = ((P_log_api << 16) | 1068),
	API_CAPI_API_CAPI_MSG_DUMMY_1069 = ((P_log_api << 16) | 1069),
	API_CAPI_API_CAPI_MSG_DUMMY_1070 = ((P_log_api << 16) | 1070),
	API_CAPI_API_CAPI_MSG_DUMMY_1071 = ((P_log_api << 16) | 1071),
	API_CAPI_API_CAPI_MSG_DUMMY_1072 = ((P_log_api << 16) | 1072),
	API_CAPI_API_CAPI_MSG_DUMMY_1073 = ((P_log_api << 16) | 1073),
	API_CAPI_API_CAPI_MSG_DUMMY_1074 = ((P_log_api << 16) | 1074),
	API_CAPI_API_CAPI_MSG_DUMMY_1075 = ((P_log_api << 16) | 1075),
	API_CAPI_API_CAPI_MSG_DUMMY_1076 = ((P_log_api << 16) | 1076),
	API_CAPI_API_CAPI_MSG_DUMMY_1077 = ((P_log_api << 16) | 1077),
	API_CAPI_API_CAPI_MSG_DUMMY_1078 = ((P_log_api << 16) | 1078),
	API_CAPI_API_CAPI_MSG_DUMMY_1079 = ((P_log_api << 16) | 1079),
	API_CAPI_API_CAPI_MSG_DUMMY_1080 = ((P_log_api << 16) | 1080),
	API_CAPI_API_CAPI_MSG_DUMMY_1081 = ((P_log_api << 16) | 1081),
	API_CAPI_API_CAPI_MSG_DUMMY_1082 = ((P_log_api << 16) | 1082),
	API_CAPI_API_CAPI_MSG_DUMMY_1083 = ((P_log_api << 16) | 1083),
	API_CAPI_API_CAPI_MSG_DUMMY_1084 = ((P_log_api << 16) | 1084),
	API_CAPI_API_CAPI_MSG_DUMMY_1085 = ((P_log_api << 16) | 1085),
	API_CAPI_API_CAPI_MSG_DUMMY_1086 = ((P_log_api << 16) | 1086),
	API_CAPI_API_CAPI_MSG_DUMMY_1087 = ((P_log_api << 16) | 1087),
	API_CAPI_API_CAPI_MSG_DUMMY_1088 = ((P_log_api << 16) | 1088),
	API_CAPI_API_CAPI_MSG_DUMMY_1089 = ((P_log_api << 16) | 1089),
	API_CAPI_API_CAPI_MSG_DUMMY_1090 = ((P_log_api << 16) | 1090),
	API_CAPI_API_CAPI_MSG_DUMMY_1091 = ((P_log_api << 16) | 1091),
	API_CAPI_API_CAPI_MSG_DUMMY_1092 = ((P_log_api << 16) | 1092),
	API_CAPI_API_CAPI_MSG_DUMMY_1093 = ((P_log_api << 16) | 1093),
	API_CAPI_API_CAPI_MSG_DUMMY_1094 = ((P_log_api << 16) | 1094),
	API_CAPI_API_CAPI_MSG_DUMMY_1095 = ((P_log_api << 16) | 1095),
	API_CAPI_API_CAPI_MSG_DUMMY_1096 = ((P_log_api << 16) | 1096),
	API_CAPI_API_CAPI_MSG_DUMMY_1097 = ((P_log_api << 16) | 1097),
	API_CAPI_API_CAPI_MSG_DUMMY_1098 = ((P_log_api << 16) | 1098),
	API_CAPI_API_CAPI_MSG_DUMMY_1099 = ((P_log_api << 16) | 1099),
	API_CAPI_API_CAPI_MSG_DUMMY_1100 = ((P_log_api << 16) | 1100),
	API_CAPI_API_CAPI_MSG_DUMMY_1101 = ((P_log_api << 16) | 1101),
	API_CAPI_API_CAPI_MSG_DUMMY_1102 = ((P_log_api << 16) | 1102),
	API_CAPI_API_CAPI_MSG_DUMMY_1103 = ((P_log_api << 16) | 1103),
	API_CAPI_API_CAPI_MSG_DUMMY_1104 = ((P_log_api << 16) | 1104),
	API_CAPI_API_CAPI_MSG_DUMMY_1105 = ((P_log_api << 16) | 1105),
	API_CAPI_API_CAPI_MSG_DUMMY_1106 = ((P_log_api << 16) | 1106),
	API_CAPI_API_CAPI_MSG_DUMMY_1107 = ((P_log_api << 16) | 1107),
	API_CAPI_API_CAPI_MSG_DUMMY_1108 = ((P_log_api << 16) | 1108),
	API_CAPI_API_CAPI_MSG_DUMMY_1109 = ((P_log_api << 16) | 1109),
	API_CAPI_API_CAPI_MSG_DUMMY_1110 = ((P_log_api << 16) | 1110),
	API_CAPI_API_CAPI_MSG_DUMMY_1111 = ((P_log_api << 16) | 1111),
	API_CAPI_API_CAPI_MSG_DUMMY_1112 = ((P_log_api << 16) | 1112),
	API_CAPI_API_CAPI_MSG_DUMMY_1113 = ((P_log_api << 16) | 1113),
	API_CAPI_API_CAPI_MSG_DUMMY_1114 = ((P_log_api << 16) | 1114),
	API_CAPI_API_CAPI_MSG_DUMMY_1115 = ((P_log_api << 16) | 1115),
	API_CAPI_API_CAPI_MSG_DUMMY_1116 = ((P_log_api << 16) | 1116),
	API_CAPI_API_CAPI_MSG_DUMMY_1117 = ((P_log_api << 16) | 1117),
	API_CAPI_API_CAPI_MSG_DUMMY_1118 = ((P_log_api << 16) | 1118),
	API_CAPI_API_CAPI_MSG_DUMMY_1119 = ((P_log_api << 16) | 1119),
	API_CAPI_API_CAPI_MSG_DUMMY_1120 = ((P_log_api << 16) | 1120),
	API_CAPI_API_CAPI_MSG_DUMMY_1121 = ((P_log_api << 16) | 1121),
	API_CAPI_API_CAPI_MSG_DUMMY_1122 = ((P_log_api << 16) | 1122),
	API_CAPI_API_CAPI_MSG_DUMMY_1123 = ((P_log_api << 16) | 1123),
	API_CAPI_API_CAPI_MSG_DUMMY_1124 = ((P_log_api << 16) | 1124),
	API_CAPI_API_CAPI_MSG_DUMMY_1125 = ((P_log_api << 16) | 1125),
	API_CAPI_API_CAPI_MSG_DUMMY_1126 = ((P_log_api << 16) | 1126),
	API_CAPI_API_CAPI_MSG_DUMMY_1127 = ((P_log_api << 16) | 1127),
	API_CAPI_API_CAPI_MSG_DUMMY_1128 = ((P_log_api << 16) | 1128),
	API_CAPI_API_CAPI_MSG_DUMMY_1129 = ((P_log_api << 16) | 1129),
	API_CAPI_API_CAPI_MSG_DUMMY_1130 = ((P_log_api << 16) | 1130),
	API_CAPI_API_CAPI_MSG_DUMMY_1131 = ((P_log_api << 16) | 1131),
	API_CAPI_API_CAPI_MSG_DUMMY_1132 = ((P_log_api << 16) | 1132),
	API_CAPI_API_CAPI_MSG_DUMMY_1133 = ((P_log_api << 16) | 1133),
	API_CAPI_API_CAPI_MSG_DUMMY_1134 = ((P_log_api << 16) | 1134),
	API_CAPI_API_CAPI_MSG_DUMMY_1135 = ((P_log_api << 16) | 1135),
	API_CAPI_API_CAPI_MSG_DUMMY_1136 = ((P_log_api << 16) | 1136),
	API_CAPI_API_CAPI_MSG_DUMMY_1137 = ((P_log_api << 16) | 1137),
	API_CAPI_API_CAPI_MSG_DUMMY_1138 = ((P_log_api << 16) | 1138),
	API_CAPI_API_CAPI_MSG_DUMMY_1139 = ((P_log_api << 16) | 1139),
	API_CAPI_API_CAPI_MSG_DUMMY_1140 = ((P_log_api << 16) | 1140),
	API_CAPI_API_CAPI_MSG_DUMMY_1141 = ((P_log_api << 16) | 1141),
	API_CAPI_API_CAPI_MSG_DUMMY_1142 = ((P_log_api << 16) | 1142),
	API_CAPI_API_CAPI_MSG_DUMMY_1143 = ((P_log_api << 16) | 1143),
	API_CAPI_API_CAPI_MSG_DUMMY_1144 = ((P_log_api << 16) | 1144),
	API_CAPI_API_CAPI_MSG_DUMMY_1145 = ((P_log_api << 16) | 1145),
	API_CAPI_API_CAPI_MSG_DUMMY_1146 = ((P_log_api << 16) | 1146),
	API_CAPI_API_CAPI_MSG_DUMMY_1147 = ((P_log_api << 16) | 1147),
	API_CAPI_API_CAPI_MSG_DUMMY_1148 = ((P_log_api << 16) | 1148),
	API_CAPI_API_CAPI_MSG_DUMMY_1149 = ((P_log_api << 16) | 1149),
	API_CAPI_API_CAPI_MSG_DUMMY_1150 = ((P_log_api << 16) | 1150),
	API_CAPI_API_CAPI_MSG_DUMMY_1151 = ((P_log_api << 16) | 1151),
	API_CAPI_API_CAPI_MSG_DUMMY_1152 = ((P_log_api << 16) | 1152),
	API_CAPI_API_CAPI_MSG_DUMMY_1153 = ((P_log_api << 16) | 1153),
	API_CAPI_API_CAPI_MSG_DUMMY_1154 = ((P_log_api << 16) | 1154),
	API_CAPI_API_CAPI_MSG_DUMMY_1155 = ((P_log_api << 16) | 1155),
	API_CAPI_API_CAPI_MSG_DUMMY_1156 = ((P_log_api << 16) | 1156),
	API_CAPI_API_CAPI_MSG_DUMMY_1157 = ((P_log_api << 16) | 1157),
	API_CAPI_API_CAPI_MSG_DUMMY_1158 = ((P_log_api << 16) | 1158),
	API_CAPI_API_CAPI_MSG_DUMMY_1159 = ((P_log_api << 16) | 1159),
	API_CAPI_API_CAPI_MSG_DUMMY_1160 = ((P_log_api << 16) | 1160),
	API_CAPI_API_CAPI_MSG_DUMMY_1161 = ((P_log_api << 16) | 1161),
	API_CAPI_API_CAPI_MSG_DUMMY_1162 = ((P_log_api << 16) | 1162),
	API_CAPI_API_CAPI_MSG_DUMMY_1163 = ((P_log_api << 16) | 1163),
	API_CAPI_API_CAPI_MSG_DUMMY_1164 = ((P_log_api << 16) | 1164),
	API_CAPI_API_CAPI_MSG_DUMMY_1165 = ((P_log_api << 16) | 1165),
	API_CAPI_API_CAPI_MSG_DUMMY_1166 = ((P_log_api << 16) | 1166),
	API_CAPI_API_CAPI_MSG_DUMMY_1167 = ((P_log_api << 16) | 1167),
	API_CAPI_API_CAPI_MSG_DUMMY_1168 = ((P_log_api << 16) | 1168),
	API_CAPI_API_CAPI_MSG_DUMMY_1169 = ((P_log_api << 16) | 1169),
	API_CAPI_API_CAPI_MSG_DUMMY_1170 = ((P_log_api << 16) | 1170),
	API_CAPI_API_CAPI_MSG_DUMMY_1171 = ((P_log_api << 16) | 1171),
	API_CAPI_API_CAPI_MSG_DUMMY_1172 = ((P_log_api << 16) | 1172),
	API_CAPI_API_CAPI_MSG_DUMMY_1173 = ((P_log_api << 16) | 1173),
	API_CAPI_API_CAPI_MSG_DUMMY_1174 = ((P_log_api << 16) | 1174),
	API_CAPI_API_CAPI_MSG_DUMMY_1175 = ((P_log_api << 16) | 1175),
	API_CAPI_API_CAPI_MSG_DUMMY_1176 = ((P_log_api << 16) | 1176),
	API_CAPI_API_CAPI_MSG_DUMMY_1177 = ((P_log_api << 16) | 1177),
	API_CAPI_API_CAPI_MSG_DUMMY_1178 = ((P_log_api << 16) | 1178),
	API_CAPI_API_CAPI_MSG_DUMMY_1179 = ((P_log_api << 16) | 1179),
	API_CAPI_API_CAPI_MSG_DUMMY_1180 = ((P_log_api << 16) | 1180),
	API_CAPI_API_CAPI_MSG_DUMMY_1181 = ((P_log_api << 16) | 1181),
	API_CAPI_API_CAPI_MSG_DUMMY_1182 = ((P_log_api << 16) | 1182),
	API_CAPI_API_CAPI_MSG_DUMMY_1183 = ((P_log_api << 16) | 1183),
	API_CAPI_API_CAPI_MSG_DUMMY_1184 = ((P_log_api << 16) | 1184),
	API_CAPI_API_CAPI_MSG_DUMMY_1185 = ((P_log_api << 16) | 1185),
	API_CAPI_API_CAPI_MSG_DUMMY_1186 = ((P_log_api << 16) | 1186),
	API_CAPI_API_CAPI_MSG_DUMMY_1187 = ((P_log_api << 16) | 1187),
	API_CAPI_API_CAPI_MSG_DUMMY_1188 = ((P_log_api << 16) | 1188),
	API_CAPI_API_CAPI_MSG_DUMMY_1189 = ((P_log_api << 16) | 1189),
	API_CAPI_API_CAPI_MSG_DUMMY_1190 = ((P_log_api << 16) | 1190),
	API_CAPI_API_CAPI_MSG_DUMMY_1191 = ((P_log_api << 16) | 1191),
	API_CAPI_API_CAPI_MSG_DUMMY_1192 = ((P_log_api << 16) | 1192),
	API_CAPI_API_CAPI_MSG_DUMMY_1193 = ((P_log_api << 16) | 1193),
	API_CAPI_API_CAPI_MSG_DUMMY_1194 = ((P_log_api << 16) | 1194),
	API_CAPI_API_CAPI_MSG_DUMMY_1195 = ((P_log_api << 16) | 1195),
	API_CAPI_API_CAPI_MSG_DUMMY_1196 = ((P_log_api << 16) | 1196),
	API_CAPI_API_CAPI_MSG_DUMMY_1197 = ((P_log_api << 16) | 1197),
	API_CAPI_API_CAPI_MSG_DUMMY_1198 = ((P_log_api << 16) | 1198),
	API_CAPI_API_CAPI_MSG_DUMMY_1199 = ((P_log_api << 16) | 1199),
	API_CAPI_API_CAPI_MSG_DUMMY_1200 = ((P_log_api << 16) | 1200),
	API_CAPI_API_CAPI_MSG_DUMMY_1201 = ((P_log_api << 16) | 1201),
	API_CAPI_API_CAPI_MSG_DUMMY_1202 = ((P_log_api << 16) | 1202),
	API_CAPI_API_CAPI_MSG_DUMMY_1203 = ((P_log_api << 16) | 1203),
	API_CAPI_API_CAPI_MSG_DUMMY_1204 = ((P_log_api << 16) | 1204),
	API_CAPI_API_CAPI_MSG_DUMMY_1205 = ((P_log_api << 16) | 1205),
	API_CAPI_API_CAPI_MSG_DUMMY_1206 = ((P_log_api << 16) | 1206),
	API_CAPI_API_CAPI_MSG_DUMMY_1207 = ((P_log_api << 16) | 1207),
	API_CAPI_API_CAPI_MSG_DUMMY_1208 = ((P_log_api << 16) | 1208),
	API_CAPI_API_CAPI_MSG_DUMMY_1209 = ((P_log_api << 16) | 1209),
	API_CAPI_API_CAPI_MSG_DUMMY_1210 = ((P_log_api << 16) | 1210),
	API_CAPI_API_CAPI_MSG_DUMMY_1211 = ((P_log_api << 16) | 1211),
	API_CAPI_API_CAPI_MSG_DUMMY_1212 = ((P_log_api << 16) | 1212),
	API_CAPI_API_CAPI_MSG_DUMMY_1213 = ((P_log_api << 16) | 1213),
	API_CAPI_API_CAPI_MSG_DUMMY_1214 = ((P_log_api << 16) | 1214),
	API_CAPI_API_CAPI_MSG_DUMMY_1215 = ((P_log_api << 16) | 1215),
	API_CAPI_API_CAPI_MSG_DUMMY_1216 = ((P_log_api << 16) | 1216),
	API_CAPI_API_CAPI_MSG_DUMMY_1217 = ((P_log_api << 16) | 1217),
	API_CAPI_API_CAPI_MSG_DUMMY_1218 = ((P_log_api << 16) | 1218),
	API_CAPI_API_CAPI_MSG_DUMMY_1219 = ((P_log_api << 16) | 1219),
	API_CAPI_API_CAPI_MSG_DUMMY_1220 = ((P_log_api << 16) | 1220),
	API_CAPI_API_CAPI_MSG_DUMMY_1221 = ((P_log_api << 16) | 1221),
	API_CAPI_API_CAPI_MSG_DUMMY_1222 = ((P_log_api << 16) | 1222),
	API_CAPI_API_CAPI_MSG_DUMMY_1223 = ((P_log_api << 16) | 1223),
	API_CAPI_API_CAPI_MSG_DUMMY_1224 = ((P_log_api << 16) | 1224),
	API_CAPI_API_CAPI_MSG_DUMMY_1225 = ((P_log_api << 16) | 1225),
	API_CAPI_API_CAPI_MSG_DUMMY_1226 = ((P_log_api << 16) | 1226),
	API_CAPI_API_CAPI_MSG_DUMMY_1227 = ((P_log_api << 16) | 1227),
	API_CAPI_API_CAPI_MSG_DUMMY_1228 = ((P_log_api << 16) | 1228),
	API_CAPI_API_CAPI_MSG_DUMMY_1229 = ((P_log_api << 16) | 1229),
	API_CAPI_API_CAPI_MSG_DUMMY_1230 = ((P_log_api << 16) | 1230),
	API_CAPI_API_CAPI_MSG_DUMMY_1231 = ((P_log_api << 16) | 1231),
	API_CAPI_API_CAPI_MSG_DUMMY_1232 = ((P_log_api << 16) | 1232),
	API_CAPI_API_CAPI_MSG_DUMMY_1233 = ((P_log_api << 16) | 1233),
	API_CAPI_API_CAPI_MSG_DUMMY_1234 = ((P_log_api << 16) | 1234),
	API_CAPI_API_CAPI_MSG_DUMMY_1235 = ((P_log_api << 16) | 1235),
	API_CAPI_API_CAPI_MSG_DUMMY_1236 = ((P_log_api << 16) | 1236),
	API_CAPI_API_CAPI_MSG_DUMMY_1237 = ((P_log_api << 16) | 1237),
	API_CAPI_API_CAPI_MSG_DUMMY_1238 = ((P_log_api << 16) | 1238),
	API_CAPI_API_CAPI_MSG_DUMMY_1239 = ((P_log_api << 16) | 1239),
	API_CAPI_API_CAPI_MSG_DUMMY_1240 = ((P_log_api << 16) | 1240),
	API_CAPI_API_CAPI_MSG_DUMMY_1241 = ((P_log_api << 16) | 1241),
	API_CAPI_API_CAPI_MSG_DUMMY_1242 = ((P_log_api << 16) | 1242),
	API_CAPI_API_CAPI_MSG_DUMMY_1243 = ((P_log_api << 16) | 1243),
	API_CAPI_API_CAPI_MSG_DUMMY_1244 = ((P_log_api << 16) | 1244),
	API_CAPI_API_CAPI_MSG_DUMMY_1245 = ((P_log_api << 16) | 1245),
	API_CAPI_API_CAPI_MSG_DUMMY_1246 = ((P_log_api << 16) | 1246),
	API_CAPI_API_CAPI_MSG_DUMMY_1247 = ((P_log_api << 16) | 1247),
	API_CAPI_API_CAPI_MSG_DUMMY_1248 = ((P_log_api << 16) | 1248),
	API_CAPI_API_CAPI_MSG_DUMMY_1249 = ((P_log_api << 16) | 1249),
	API_CAPI_API_CAPI_MSG_DUMMY_1250 = ((P_log_api << 16) | 1250),
	API_CAPI_API_CAPI_MSG_DUMMY_1251 = ((P_log_api << 16) | 1251),
	API_CAPI_API_CAPI_MSG_DUMMY_1252 = ((P_log_api << 16) | 1252),
	API_CAPI_API_CAPI_MSG_DUMMY_1253 = ((P_log_api << 16) | 1253),
	API_CAPI_API_CAPI_MSG_DUMMY_1254 = ((P_log_api << 16) | 1254),
	API_CAPI_API_CAPI_MSG_DUMMY_1255 = ((P_log_api << 16) | 1255),
	API_CAPI_API_CAPI_MSG_DUMMY_1256 = ((P_log_api << 16) | 1256),
	API_CAPI_API_CAPI_MSG_DUMMY_1257 = ((P_log_api << 16) | 1257),
	API_CAPI_API_CAPI_MSG_DUMMY_1258 = ((P_log_api << 16) | 1258),
	API_CAPI_API_CAPI_MSG_DUMMY_1259 = ((P_log_api << 16) | 1259),
	API_CAPI_API_CAPI_MSG_DUMMY_1260 = ((P_log_api << 16) | 1260),
	API_CAPI_API_CAPI_MSG_DUMMY_1261 = ((P_log_api << 16) | 1261),
	API_CAPI_API_CAPI_MSG_DUMMY_1262 = ((P_log_api << 16) | 1262),
	API_CAPI_API_CAPI_MSG_DUMMY_1263 = ((P_log_api << 16) | 1263),
	API_CAPI_API_CAPI_MSG_DUMMY_1264 = ((P_log_api << 16) | 1264),
	API_CAPI_API_CAPI_MSG_DUMMY_1265 = ((P_log_api << 16) | 1265),
	API_CAPI_API_CAPI_MSG_DUMMY_1266 = ((P_log_api << 16) | 1266),
	API_CAPI_API_CAPI_MSG_DUMMY_1267 = ((P_log_api << 16) | 1267),
	API_CAPI_API_CAPI_MSG_DUMMY_1268 = ((P_log_api << 16) | 1268),
	API_CAPI_API_CAPI_MSG_DUMMY_1269 = ((P_log_api << 16) | 1269),
	API_CAPI_API_CAPI_MSG_DUMMY_1270 = ((P_log_api << 16) | 1270),
	API_CAPI_API_CAPI_MSG_DUMMY_1271 = ((P_log_api << 16) | 1271),
	API_CAPI_API_CAPI_MSG_DUMMY_1272 = ((P_log_api << 16) | 1272),
	API_CAPI_API_CAPI_MSG_DUMMY_1273 = ((P_log_api << 16) | 1273),
	API_CAPI_API_CAPI_MSG_DUMMY_1274 = ((P_log_api << 16) | 1274),
	API_CAPI_API_CAPI_MSG_DUMMY_1275 = ((P_log_api << 16) | 1275),
	API_CAPI_API_CAPI_MSG_DUMMY_1276 = ((P_log_api << 16) | 1276),
	API_CAPI_API_CAPI_MSG_DUMMY_1277 = ((P_log_api << 16) | 1277),
	API_CAPI_API_CAPI_MSG_DUMMY_1278 = ((P_log_api << 16) | 1278),
	API_CAPI_API_CAPI_MSG_DUMMY_1279 = ((P_log_api << 16) | 1279),
	API_CAPI_API_CAPI_MSG_DUMMY_1280 = ((P_log_api << 16) | 1280),
	API_CAPI_API_CAPI_MSG_DUMMY_1281 = ((P_log_api << 16) | 1281),
	API_CAPI_API_CAPI_MSG_DUMMY_1282 = ((P_log_api << 16) | 1282),
	API_CAPI_API_CAPI_MSG_DUMMY_1283 = ((P_log_api << 16) | 1283),
	API_CAPI_API_CAPI_MSG_DUMMY_1284 = ((P_log_api << 16) | 1284),
	API_CAPI_API_CAPI_MSG_DUMMY_1285 = ((P_log_api << 16) | 1285),
	API_CAPI_API_CAPI_MSG_DUMMY_1286 = ((P_log_api << 16) | 1286),
	API_CAPI_API_CAPI_MSG_DUMMY_1287 = ((P_log_api << 16) | 1287),
	API_CAPI_API_CAPI_MSG_DUMMY_1288 = ((P_log_api << 16) | 1288),
	API_CAPI_API_CAPI_MSG_DUMMY_1289 = ((P_log_api << 16) | 1289),
	API_CAPI_API_CAPI_MSG_DUMMY_1290 = ((P_log_api << 16) | 1290),
	API_CAPI_API_CAPI_MSG_DUMMY_1291 = ((P_log_api << 16) | 1291),
	API_CAPI_API_CAPI_MSG_DUMMY_1292 = ((P_log_api << 16) | 1292),
	API_CAPI_API_CAPI_MSG_DUMMY_1293 = ((P_log_api << 16) | 1293),
	API_CAPI_API_CAPI_MSG_DUMMY_1294 = ((P_log_api << 16) | 1294),
	API_CAPI_API_CAPI_MSG_DUMMY_1295 = ((P_log_api << 16) | 1295),
	API_CAPI_API_CAPI_MSG_DUMMY_1296 = ((P_log_api << 16) | 1296),
	API_CAPI_API_CAPI_MSG_DUMMY_1297 = ((P_log_api << 16) | 1297),
	API_CAPI_API_CAPI_MSG_DUMMY_1298 = ((P_log_api << 16) | 1298),
	API_CAPI_API_CAPI_MSG_DUMMY_1299 = ((P_log_api << 16) | 1299),
	API_CAPI_API_CAPI_MSG_DUMMY_1300 = ((P_log_api << 16) | 1300),
	API_CAPI_API_CAPI_MSG_DUMMY_1301 = ((P_log_api << 16) | 1301),
	API_CAPI_API_CAPI_MSG_DUMMY_1302 = ((P_log_api << 16) | 1302),
	API_CAPI_API_CAPI_MSG_DUMMY_1303 = ((P_log_api << 16) | 1303),
	API_CAPI_API_CAPI_MSG_DUMMY_1304 = ((P_log_api << 16) | 1304),
	API_CAPI_API_CAPI_MSG_DUMMY_1305 = ((P_log_api << 16) | 1305),
	API_CAPI_API_CAPI_MSG_DUMMY_1306 = ((P_log_api << 16) | 1306),
	API_CAPI_API_CAPI_MSG_DUMMY_1307 = ((P_log_api << 16) | 1307),
	API_CAPI_API_CAPI_MSG_DUMMY_1308 = ((P_log_api << 16) | 1308),
	API_CAPI_API_CAPI_MSG_DUMMY_1309 = ((P_log_api << 16) | 1309),
	API_CAPI_API_CAPI_MSG_DUMMY_1310 = ((P_log_api << 16) | 1310),
	API_CAPI_API_CAPI_MSG_DUMMY_1311 = ((P_log_api << 16) | 1311),
	API_CAPI_API_CAPI_MSG_DUMMY_1312 = ((P_log_api << 16) | 1312),
	API_CAPI_API_CAPI_MSG_DUMMY_1313 = ((P_log_api << 16) | 1313),
	API_CAPI_API_CAPI_MSG_DUMMY_1314 = ((P_log_api << 16) | 1314),
	API_CAPI_API_CAPI_MSG_DUMMY_1315 = ((P_log_api << 16) | 1315),
	API_CAPI_API_CAPI_MSG_DUMMY_1316 = ((P_log_api << 16) | 1316),
	API_CAPI_API_CAPI_MSG_DUMMY_1317 = ((P_log_api << 16) | 1317),
	API_CAPI_API_CAPI_MSG_DUMMY_1318 = ((P_log_api << 16) | 1318),
	API_CAPI_API_CAPI_MSG_DUMMY_1319 = ((P_log_api << 16) | 1319),
	API_CAPI_API_CAPI_MSG_DUMMY_1320 = ((P_log_api << 16) | 1320),
	API_CAPI_API_CAPI_MSG_DUMMY_1321 = ((P_log_api << 16) | 1321),
	API_CAPI_API_CAPI_MSG_DUMMY_1322 = ((P_log_api << 16) | 1322),
	API_CAPI_API_CAPI_MSG_DUMMY_1323 = ((P_log_api << 16) | 1323),
	API_CAPI_API_CAPI_MSG_DUMMY_1324 = ((P_log_api << 16) | 1324),
	API_CAPI_API_CAPI_MSG_DUMMY_1325 = ((P_log_api << 16) | 1325),
	API_CAPI_API_CAPI_MSG_DUMMY_1326 = ((P_log_api << 16) | 1326),
	API_CAPI_API_CAPI_MSG_DUMMY_1327 = ((P_log_api << 16) | 1327),
	API_CAPI_API_CAPI_MSG_DUMMY_1328 = ((P_log_api << 16) | 1328),
	API_CAPI_API_CAPI_MSG_DUMMY_1329 = ((P_log_api << 16) | 1329),
	API_CAPI_API_CAPI_MSG_DUMMY_1330 = ((P_log_api << 16) | 1330),
	API_CAPI_API_CAPI_MSG_DUMMY_1331 = ((P_log_api << 16) | 1331),
	API_CAPI_API_CAPI_MSG_DUMMY_1332 = ((P_log_api << 16) | 1332),
	API_CAPI_API_CAPI_MSG_DUMMY_1333 = ((P_log_api << 16) | 1333),
	API_CAPI_API_CAPI_MSG_DUMMY_1334 = ((P_log_api << 16) | 1334),
	API_CAPI_API_CAPI_MSG_DUMMY_1335 = ((P_log_api << 16) | 1335),
	API_CAPI_API_CAPI_MSG_DUMMY_1336 = ((P_log_api << 16) | 1336),
	API_CAPI_API_CAPI_MSG_DUMMY_1337 = ((P_log_api << 16) | 1337),
	API_CAPI_API_CAPI_MSG_DUMMY_1338 = ((P_log_api << 16) | 1338),
	API_CAPI_API_CAPI_MSG_DUMMY_1339 = ((P_log_api << 16) | 1339),
	API_CAPI_API_CAPI_MSG_DUMMY_1340 = ((P_log_api << 16) | 1340),
	API_CAPI_API_CAPI_MSG_DUMMY_1341 = ((P_log_api << 16) | 1341),
	API_CAPI_API_CAPI_MSG_DUMMY_1342 = ((P_log_api << 16) | 1342),
	API_CAPI_API_CAPI_MSG_DUMMY_1343 = ((P_log_api << 16) | 1343),
	API_CAPI_API_CAPI_MSG_DUMMY_1344 = ((P_log_api << 16) | 1344),
	API_CAPI_API_CAPI_MSG_DUMMY_1345 = ((P_log_api << 16) | 1345),
	API_CAPI_API_CAPI_MSG_DUMMY_1346 = ((P_log_api << 16) | 1346),
	API_CAPI_API_CAPI_MSG_DUMMY_1347 = ((P_log_api << 16) | 1347),
	API_CAPI_API_CAPI_MSG_DUMMY_1348 = ((P_log_api << 16) | 1348),
	API_CAPI_API_CAPI_MSG_DUMMY_1349 = ((P_log_api << 16) | 1349),
	API_CAPI_API_CAPI_MSG_DUMMY_1350 = ((P_log_api << 16) | 1350),
	API_CAPI_API_CAPI_MSG_DUMMY_1351 = ((P_log_api << 16) | 1351),
	API_CAPI_API_CAPI_MSG_DUMMY_1352 = ((P_log_api << 16) | 1352),
	API_CAPI_API_CAPI_MSG_DUMMY_1353 = ((P_log_api << 16) | 1353),
	API_CAPI_API_CAPI_MSG_DUMMY_1354 = ((P_log_api << 16) | 1354),
	API_CAPI_API_CAPI_MSG_DUMMY_1355 = ((P_log_api << 16) | 1355),
	API_CAPI_API_CAPI_MSG_DUMMY_1356 = ((P_log_api << 16) | 1356),
	API_CAPI_API_CAPI_MSG_DUMMY_1357 = ((P_log_api << 16) | 1357),
	API_CAPI_API_CAPI_MSG_DUMMY_1358 = ((P_log_api << 16) | 1358),
	API_CAPI_API_CAPI_MSG_DUMMY_1359 = ((P_log_api << 16) | 1359),
	API_CAPI_API_CAPI_MSG_DUMMY_1360 = ((P_log_api << 16) | 1360),
	API_CAPI_API_CAPI_MSG_DUMMY_1361 = ((P_log_api << 16) | 1361),
	API_CAPI_API_CAPI_MSG_DUMMY_1362 = ((P_log_api << 16) | 1362),
	API_CAPI_API_CAPI_MSG_DUMMY_1363 = ((P_log_api << 16) | 1363),
	API_CAPI_API_CAPI_MSG_DUMMY_1364 = ((P_log_api << 16) | 1364),
	API_CAPI_API_CAPI_MSG_DUMMY_1365 = ((P_log_api << 16) | 1365),
	API_CAPI_API_CAPI_MSG_DUMMY_1366 = ((P_log_api << 16) | 1366),
	API_CAPI_API_CAPI_MSG_DUMMY_1367 = ((P_log_api << 16) | 1367),
	API_CAPI_API_CAPI_MSG_DUMMY_1368 = ((P_log_api << 16) | 1368),
	API_CAPI_API_CAPI_MSG_DUMMY_1369 = ((P_log_api << 16) | 1369),
	API_CAPI_API_CAPI_MSG_DUMMY_1370 = ((P_log_api << 16) | 1370),
	API_CAPI_API_CAPI_MSG_DUMMY_1371 = ((P_log_api << 16) | 1371),
	API_CAPI_API_CAPI_MSG_DUMMY_1372 = ((P_log_api << 16) | 1372),
	API_CAPI_API_CAPI_MSG_DUMMY_1373 = ((P_log_api << 16) | 1373),
	API_CAPI_API_CAPI_MSG_DUMMY_1374 = ((P_log_api << 16) | 1374),
	API_CAPI_API_CAPI_MSG_DUMMY_1375 = ((P_log_api << 16) | 1375),
	API_CAPI_API_CAPI_MSG_DUMMY_1376 = ((P_log_api << 16) | 1376),
	API_CAPI_API_CAPI_MSG_DUMMY_1377 = ((P_log_api << 16) | 1377),
	API_CAPI_API_CAPI_MSG_DUMMY_1378 = ((P_log_api << 16) | 1378),
	API_CAPI_API_CAPI_MSG_DUMMY_1379 = ((P_log_api << 16) | 1379),
	API_CAPI_API_CAPI_MSG_DUMMY_1380 = ((P_log_api << 16) | 1380),
	API_CAPI_API_CAPI_MSG_DUMMY_1381 = ((P_log_api << 16) | 1381),
	API_CAPI_API_CAPI_MSG_DUMMY_1382 = ((P_log_api << 16) | 1382),
	API_CAPI_API_CAPI_MSG_DUMMY_1383 = ((P_log_api << 16) | 1383),
	API_CAPI_API_CAPI_MSG_DUMMY_1384 = ((P_log_api << 16) | 1384),
	API_CAPI_API_CAPI_MSG_DUMMY_1385 = ((P_log_api << 16) | 1385),
	API_CAPI_API_CAPI_MSG_DUMMY_1386 = ((P_log_api << 16) | 1386),
	API_CAPI_API_CAPI_MSG_DUMMY_1387 = ((P_log_api << 16) | 1387),
	API_CAPI_API_CAPI_MSG_DUMMY_1388 = ((P_log_api << 16) | 1388),
	API_CAPI_API_CAPI_MSG_DUMMY_1389 = ((P_log_api << 16) | 1389),
	API_CAPI_API_CAPI_MSG_DUMMY_1390 = ((P_log_api << 16) | 1390),
	API_CAPI_API_CAPI_MSG_DUMMY_1391 = ((P_log_api << 16) | 1391),
	API_CAPI_API_CAPI_MSG_DUMMY_1392 = ((P_log_api << 16) | 1392),
	API_CAPI_API_CAPI_MSG_DUMMY_1393 = ((P_log_api << 16) | 1393),
	API_CAPI_API_CAPI_MSG_DUMMY_1394 = ((P_log_api << 16) | 1394),
	API_CAPI_API_CAPI_MSG_DUMMY_1395 = ((P_log_api << 16) | 1395),
	API_CAPI_API_CAPI_MSG_DUMMY_1396 = ((P_log_api << 16) | 1396),
	API_CAPI_API_CAPI_MSG_DUMMY_1397 = ((P_log_api << 16) | 1397),
	API_CAPI_API_CAPI_MSG_DUMMY_1398 = ((P_log_api << 16) | 1398),
	API_CAPI_API_CAPI_MSG_DUMMY_1399 = ((P_log_api << 16) | 1399),
	API_CAPI_API_CAPI_MSG_DUMMY_1400 = ((P_log_api << 16) | 1400),
	API_CAPI_API_CAPI_MSG_DUMMY_1401 = ((P_log_api << 16) | 1401),
	API_CAPI_API_CAPI_MSG_DUMMY_1402 = ((P_log_api << 16) | 1402),
	API_CAPI_API_CAPI_MSG_DUMMY_1403 = ((P_log_api << 16) | 1403),
	API_CAPI_API_CAPI_MSG_DUMMY_1404 = ((P_log_api << 16) | 1404),
	API_CAPI_API_CAPI_MSG_DUMMY_1405 = ((P_log_api << 16) | 1405),
	API_CAPI_API_CAPI_MSG_DUMMY_1406 = ((P_log_api << 16) | 1406),
	API_CAPI_API_CAPI_MSG_DUMMY_1407 = ((P_log_api << 16) | 1407),
	API_CAPI_API_CAPI_MSG_DUMMY_1408 = ((P_log_api << 16) | 1408),
	API_CAPI_API_CAPI_MSG_DUMMY_1409 = ((P_log_api << 16) | 1409),
	API_CAPI_API_CAPI_MSG_DUMMY_1410 = ((P_log_api << 16) | 1410),
	API_CAPI_API_CAPI_MSG_DUMMY_1411 = ((P_log_api << 16) | 1411),
	API_CAPI_API_CAPI_MSG_DUMMY_1412 = ((P_log_api << 16) | 1412),
	API_CAPI_API_CAPI_MSG_DUMMY_1413 = ((P_log_api << 16) | 1413),
	API_CAPI_API_CAPI_MSG_DUMMY_1414 = ((P_log_api << 16) | 1414),
	API_CAPI_API_CAPI_MSG_DUMMY_1415 = ((P_log_api << 16) | 1415),
	API_CAPI_API_CAPI_MSG_DUMMY_1416 = ((P_log_api << 16) | 1416),
	API_CAPI_API_CAPI_MSG_DUMMY_1417 = ((P_log_api << 16) | 1417),
	API_CAPI_API_CAPI_MSG_DUMMY_1418 = ((P_log_api << 16) | 1418),
	API_CAPI_API_CAPI_MSG_DUMMY_1419 = ((P_log_api << 16) | 1419),
	API_CAPI_API_CAPI_MSG_DUMMY_1420 = ((P_log_api << 16) | 1420),
	API_CAPI_API_CAPI_MSG_DUMMY_1421 = ((P_log_api << 16) | 1421),
	API_CAPI_API_CAPI_MSG_DUMMY_1422 = ((P_log_api << 16) | 1422),
	API_CAPI_API_CAPI_MSG_DUMMY_1423 = ((P_log_api << 16) | 1423),
	API_CAPI_API_CAPI_MSG_DUMMY_1424 = ((P_log_api << 16) | 1424),
	API_CAPI_API_CAPI_MSG_DUMMY_1425 = ((P_log_api << 16) | 1425),
	API_CAPI_API_CAPI_MSG_DUMMY_1426 = ((P_log_api << 16) | 1426),
	API_CAPI_API_CAPI_MSG_DUMMY_1427 = ((P_log_api << 16) | 1427),
	API_CAPI_API_CAPI_MSG_DUMMY_1428 = ((P_log_api << 16) | 1428),
	API_CAPI_API_CAPI_MSG_DUMMY_1429 = ((P_log_api << 16) | 1429),
	API_CAPI_API_CAPI_MSG_DUMMY_1430 = ((P_log_api << 16) | 1430),
	API_CAPI_API_CAPI_MSG_DUMMY_1431 = ((P_log_api << 16) | 1431),
	API_CAPI_API_CAPI_MSG_DUMMY_1432 = ((P_log_api << 16) | 1432),
	API_CAPI_API_CAPI_MSG_DUMMY_1433 = ((P_log_api << 16) | 1433),
	API_CAPI_API_CAPI_MSG_DUMMY_1434 = ((P_log_api << 16) | 1434),
	API_CAPI_API_CAPI_MSG_DUMMY_1435 = ((P_log_api << 16) | 1435),
	API_CAPI_API_CAPI_MSG_DUMMY_1436 = ((P_log_api << 16) | 1436),
	API_CAPI_API_CAPI_MSG_DUMMY_1437 = ((P_log_api << 16) | 1437),
	API_CAPI_API_CAPI_MSG_DUMMY_1438 = ((P_log_api << 16) | 1438),
	API_CAPI_API_CAPI_MSG_DUMMY_1439 = ((P_log_api << 16) | 1439),
	API_CAPI_API_CAPI_MSG_DUMMY_1440 = ((P_log_api << 16) | 1440),
	API_CAPI_API_CAPI_MSG_DUMMY_1441 = ((P_log_api << 16) | 1441),
	API_CAPI_API_CAPI_MSG_DUMMY_1442 = ((P_log_api << 16) | 1442),
	API_CAPI_API_CAPI_MSG_DUMMY_1443 = ((P_log_api << 16) | 1443),
	API_CAPI_API_CAPI_MSG_DUMMY_1444 = ((P_log_api << 16) | 1444),
	API_CAPI_API_CAPI_MSG_DUMMY_1445 = ((P_log_api << 16) | 1445),
	API_CAPI_API_CAPI_MSG_DUMMY_1446 = ((P_log_api << 16) | 1446),
	API_CAPI_API_CAPI_MSG_DUMMY_1447 = ((P_log_api << 16) | 1447),
	API_CAPI_API_CAPI_MSG_DUMMY_1448 = ((P_log_api << 16) | 1448),
	API_CAPI_API_CAPI_MSG_DUMMY_1449 = ((P_log_api << 16) | 1449),
	API_CAPI_API_CAPI_MSG_DUMMY_1450 = ((P_log_api << 16) | 1450),
	API_CAPI_API_CAPI_MSG_DUMMY_1451 = ((P_log_api << 16) | 1451),
	API_CAPI_API_CAPI_MSG_DUMMY_1452 = ((P_log_api << 16) | 1452),
	API_CAPI_API_CAPI_MSG_DUMMY_1453 = ((P_log_api << 16) | 1453),
	API_CAPI_API_CAPI_MSG_DUMMY_1454 = ((P_log_api << 16) | 1454),
	API_CAPI_API_CAPI_MSG_DUMMY_1455 = ((P_log_api << 16) | 1455),
	API_CAPI_API_CAPI_MSG_DUMMY_1456 = ((P_log_api << 16) | 1456),
	API_CAPI_API_CAPI_MSG_DUMMY_1457 = ((P_log_api << 16) | 1457),
	API_CAPI_API_CAPI_MSG_DUMMY_1458 = ((P_log_api << 16) | 1458),
	API_CAPI_API_CAPI_MSG_DUMMY_1459 = ((P_log_api << 16) | 1459),
	API_CAPI_API_CAPI_MSG_DUMMY_1460 = ((P_log_api << 16) | 1460),
	API_CAPI_API_CAPI_MSG_DUMMY_1461 = ((P_log_api << 16) | 1461),
	API_CAPI_API_CAPI_MSG_DUMMY_1462 = ((P_log_api << 16) | 1462),
	API_CAPI_API_CAPI_MSG_DUMMY_1463 = ((P_log_api << 16) | 1463),
	API_CAPI_API_CAPI_MSG_DUMMY_1464 = ((P_log_api << 16) | 1464),
	API_CAPI_API_CAPI_MSG_DUMMY_1465 = ((P_log_api << 16) | 1465),
	API_CAPI_API_CAPI_MSG_DUMMY_1466 = ((P_log_api << 16) | 1466),
	API_CAPI_API_CAPI_MSG_DUMMY_1467 = ((P_log_api << 16) | 1467),
	API_CAPI_API_CAPI_MSG_DUMMY_1468 = ((P_log_api << 16) | 1468),
	API_CAPI_API_CAPI_MSG_DUMMY_1469 = ((P_log_api << 16) | 1469),
	API_CAPI_API_CAPI_MSG_DUMMY_1470 = ((P_log_api << 16) | 1470),
	API_CAPI_API_CAPI_MSG_DUMMY_1471 = ((P_log_api << 16) | 1471),
	API_CAPI_API_CAPI_MSG_DUMMY_1472 = ((P_log_api << 16) | 1472),
	API_CAPI_API_CAPI_MSG_DUMMY_1473 = ((P_log_api << 16) | 1473),
	API_CAPI_API_CAPI_MSG_DUMMY_1474 = ((P_log_api << 16) | 1474),
	API_CAPI_API_CAPI_MSG_DUMMY_1475 = ((P_log_api << 16) | 1475),
	API_CAPI_API_CAPI_MSG_DUMMY_1476 = ((P_log_api << 16) | 1476),
	API_CAPI_API_CAPI_MSG_DUMMY_1477 = ((P_log_api << 16) | 1477),
	API_CAPI_API_CAPI_MSG_DUMMY_1478 = ((P_log_api << 16) | 1478),
	API_CAPI_API_CAPI_MSG_DUMMY_1479 = ((P_log_api << 16) | 1479),
	API_CAPI_API_CAPI_MSG_DUMMY_1480 = ((P_log_api << 16) | 1480),
	API_CAPI_API_CAPI_MSG_DUMMY_1481 = ((P_log_api << 16) | 1481),
	API_CAPI_API_CAPI_MSG_DUMMY_1482 = ((P_log_api << 16) | 1482),
	API_CAPI_API_CAPI_MSG_DUMMY_1483 = ((P_log_api << 16) | 1483),
	API_CAPI_API_CAPI_MSG_DUMMY_1484 = ((P_log_api << 16) | 1484),
	API_CAPI_API_CAPI_MSG_DUMMY_1485 = ((P_log_api << 16) | 1485),
	API_CAPI_API_CAPI_MSG_DUMMY_1486 = ((P_log_api << 16) | 1486),
	API_CAPI_API_CAPI_MSG_DUMMY_1487 = ((P_log_api << 16) | 1487),
	API_CAPI_API_CAPI_MSG_DUMMY_1488 = ((P_log_api << 16) | 1488),
	API_CAPI_API_CAPI_MSG_DUMMY_1489 = ((P_log_api << 16) | 1489),
	API_CAPI_API_CAPI_MSG_DUMMY_1490 = ((P_log_api << 16) | 1490),
	API_CAPI_API_CAPI_MSG_DUMMY_1491 = ((P_log_api << 16) | 1491),
	API_CAPI_API_CAPI_MSG_DUMMY_1492 = ((P_log_api << 16) | 1492),
	API_CAPI_API_CAPI_MSG_DUMMY_1493 = ((P_log_api << 16) | 1493),
	API_CAPI_API_CAPI_MSG_DUMMY_1494 = ((P_log_api << 16) | 1494),
	API_CAPI_API_CAPI_MSG_DUMMY_1495 = ((P_log_api << 16) | 1495),
	API_CAPI_API_CAPI_MSG_DUMMY_1496 = ((P_log_api << 16) | 1496),
	API_CAPI_API_CAPI_MSG_DUMMY_1497 = ((P_log_api << 16) | 1497),
	API_CAPI_API_CAPI_MSG_DUMMY_1498 = ((P_log_api << 16) | 1498),
	API_CAPI_API_CAPI_MSG_DUMMY_1499 = ((P_log_api << 16) | 1499),
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_CLIP = ((P_log_api << 16) | 1500),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_CLIR = ((P_log_api << 16) | 1501),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_COLP = ((P_log_api << 16) | 1502),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_CCWA = ((P_log_api << 16) | 1503),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_LONG_FTN_SUPPORTED = ((P_log_api << 16) | 1504),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_FDN_CHECK = ((P_log_api << 16) | 1505),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_TX_USSD_DATA_ENCODE = ((P_log_api << 16) | 1506),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_RX_USSD_DATA_ENCODE = ((P_log_api << 16) | 1507),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_INCLUDE_FACILITY_IE = ((P_log_api << 16) | 1508),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_INVOKE_ID = ((P_log_api << 16) | 1509),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_ENABLE_OLD_SS_MSG = ((P_log_api << 16) | 1510),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SS_ELEM_CLIENT_HANDLE_USSD_NOTIFY = ((P_log_api << 16) | 1511),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_TEST_ELEM_CHAN = ((P_log_api << 16) | 1512),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_TEST_ELEM_CHAN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_TEST_ELEM_PROD_MODE = ((P_log_api << 16) | 1513),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_AUDIO = ((P_log_api << 16) | 1514),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_VIDEO_CALL = ((P_log_api << 16) | 1515),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DEF_VOICE_BC = ((P_log_api << 16) | 1516),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DEF_VOICE_BC}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CLIENT_GEN_TONE = ((P_log_api << 16) | 1517),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SCUDIF = ((P_log_api << 16) | 1518),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURR_CALL_CFG = ((P_log_api << 16) | 1519),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURR_CALL_CFG}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_HSCSD_REPORT = ((P_log_api << 16) | 1520),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_HSCSD_REPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_IS_HSCSD_CALL = ((P_log_api << 16) | 1521),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_RX_SLOTS = ((P_log_api << 16) | 1522),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_TX_SLOTS = ((P_log_api << 16) | 1523),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_AIR_IF_USER_RATE = ((P_log_api << 16) | 1524),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURRENT_CODING = ((P_log_api << 16) | 1525),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURRENT_CONNECT_ELEMENT = ((P_log_api << 16) | 1526),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_TEL_NUMBER = ((P_log_api << 16) | 1527),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_TEL_NUMBER}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALLING_SUBADDR = ((P_log_api << 16) | 1528),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALLING_SUBADDR}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALLED_SUBADDR = ((P_log_api << 16) | 1529),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALLED_SUBADDR}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CONNECT_SUBADDR = ((P_log_api << 16) | 1530),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CONNECT_SUBADDR}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CBST = ((P_log_api << 16) | 1531),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CBST}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_NEG_DATACOMP = ((P_log_api << 16) | 1532),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_NEG_DATACOMP}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_REQ_DATACOMP = ((P_log_api << 16) | 1533),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_REQ_DATACOMP}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_REQ_SUCCESS_NEG = ((P_log_api << 16) | 1534),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_REQUESTED = ((P_log_api << 16) | 1535),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_REQUESTED}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_FALLBACK_NOT_NEGOTIABLE = ((P_log_api << 16) | 1536),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_FALLBACK_NOT_NEGOTIABLE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_FALLBACK_NO_ANSWER = ((P_log_api << 16) | 1537),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_FALLBACK_NO_ANSWER}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_PARAMS = ((P_log_api << 16) | 1538),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_EC_PARAMS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DC_PARAMS = ((P_log_api << 16) | 1539),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DC_PARAMS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_MCLASS = ((P_log_api << 16) | 1540),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_MAX_RX_SLOTS = ((P_log_api << 16) | 1541),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_MAX_TX_SLOTS = ((P_log_api << 16) | 1542),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SUM_SLOTS = ((P_log_api << 16) | 1543),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CODINGS = ((P_log_api << 16) | 1544),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_WANTED_RX_SLOTS = ((P_log_api << 16) | 1545),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_WANTED_AIR_USER_RATE = ((P_log_api << 16) | 1546),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_MAX_TX_SLOTS_SUPPORTED = ((P_log_api << 16) | 1547),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURR_TI_PD = ((P_log_api << 16) | 1548),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CURR_SERVICE_MODE = ((P_log_api << 16) | 1549),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_DS_DATACOMP_IND = ((P_log_api << 16) | 1550),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CUGI = ((P_log_api << 16) | 1551),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SPKR_LOUDNESS = ((P_log_api << 16) | 1552),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SPKR_MONITOR = ((P_log_api << 16) | 1553),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALL_METER = ((P_log_api << 16) | 1554),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CALL_METER}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_ENABLE_AUTO_REJECT = ((P_log_api << 16) | 1555),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_ETBM = ((P_log_api << 16) | 1556),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_ETBM}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CLIR = ((P_log_api << 16) | 1557),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CLIR}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_FAX_PARAMS = ((P_log_api << 16) | 1558),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_FAX_PARAMS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_FAX_FCLASS = ((P_log_api << 16) | 1559),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CSNS = ((P_log_api << 16) | 1560),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SMS_ELEM_DEF_REC_NUMBER = ((P_log_api << 16) | 1561),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SMS_ELEM_CLIENT_HANDLE_MT_SMS = ((P_log_api << 16) | 1562),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SMS_ELEM_CLIENT_HANDLE_MO_SMS_RETRY = ((P_log_api << 16) | 1563),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SMS_ELEM_MORE_MESSAGE_TO_SEND = ((P_log_api << 16) | 1564),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SETUP_CALL_CTR = ((P_log_api << 16) | 1565),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SEND_SS_CTR = ((P_log_api << 16) | 1566),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SEND_USSD_CTR = ((P_log_api << 16) | 1567),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SEND_SMS_CTR = ((P_log_api << 16) | 1568),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SEND_DTMF_CTR = ((P_log_api << 16) | 1569),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_CALL_CONTROL_CTR = ((P_log_api << 16) | 1570),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SS_CONTROL_CTR = ((P_log_api << 16) | 1571),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_USSD_CONTROL_CTR = ((P_log_api << 16) | 1572),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_SMS_CONTROL_CTR = ((P_log_api << 16) | 1573),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_GENERIC_INTERFACE_CTR = ((P_log_api << 16) | 1574),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_DYN_TERM_PROFILE = ((P_log_api << 16) | 1575),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_SATK_ELEM_CMD_FETCH_ENABLED_AT_STARTUP = ((P_log_api << 16) | 1576),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PBK_ELEM_BDN_CACHE_CTR = ((P_log_api << 16) | 1577),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PCH_ELEM_SECONDARY_PDP_CONTEXT = ((P_log_api << 16) | 1578),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_EBSE = ((P_log_api << 16) | 1579),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_SESSION = ((P_log_api << 16) | 1580),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_SESSION}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_MODE = ((P_log_api << 16) | 1581),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_MODE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREV_PLMN_MODE = ((P_log_api << 16) | 1582),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREV_PLMN_MODE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_SELECT_RAT = ((P_log_api << 16) | 1583),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREV_PLMN_SELECT_RAT = ((P_log_api << 16) | 1584),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_FORMAT = ((P_log_api << 16) | 1585),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMN_FORMAT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_ABORT_PLMN_SELECT = ((P_log_api << 16) | 1586),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PCM_MODE = ((P_log_api << 16) | 1587),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_IS_PWRDWN_REFRESH_SIM_RESET = ((P_log_api << 16) | 1588),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_IS_PWRDWN_SOFTRESET}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SELECTED_PLMN = ((P_log_api << 16) | 1589),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SELECTED_PLMN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREV_SEL_PLMN = ((P_log_api << 16) | 1590),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREV_SEL_PLMN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREF_USIM_PBK = ((P_log_api << 16) | 1591),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PREF_USIM_PBK}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PWRDWN_DETACH_TIMER = ((P_log_api << 16) | 1592),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PWRDWN_STORAGE_TIMER = ((P_log_api << 16) | 1593),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_ATTACH_MODE = ((P_log_api << 16) | 1594),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_ATTACH_MODE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_USER_NW_RESELECT = ((P_log_api << 16) | 1595),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_WAIT_BEFORE_DETACH_TIME = ((P_log_api << 16) | 1596),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_WAIT_BEFORE_DETACH_TIME}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SYSTEM_STATE = ((P_log_api << 16) | 1597),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SYSTEM_STATE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_POWER_DOWN_STATE = ((P_log_api << 16) | 1598),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_POWER_DOWN_STATE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_IMEI = ((P_log_api << 16) | 1599),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_FORCE_SIM_NOT_RDY = ((P_log_api << 16) | 1600),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_FAST_BAND_SELECT_DISABLE = ((P_log_api << 16) | 1601),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SMSME_STORAGE_ENABLE = ((P_log_api << 16) | 1602),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SMSME_MAX_CAPACITY = ((P_log_api << 16) | 1603),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_SMSME_MAX_CAPACITY}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_GAN_MODE = ((P_log_api << 16) | 1604),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_GAN_MODE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CELL_LOCK_PARAMS = ((P_log_api << 16) | 1605),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CELL_LOCK_PARAMS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_AT_ELEM_AUDIO_CTRL = ((P_log_api << 16) | 1606),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_MS_ELEM_ASYNC_RETURN_RESULT = ((P_log_api << 16) | 1607),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_MS_ELEM_DIALOG_ID = ((P_log_api << 16) | 1608),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_MS_ELEM_DIALOG_ID}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_NVRAM_CLASSMARK = ((P_log_api << 16) | 1609),	///<Payload type {::T_ELEM_MSDB_PTR_MS_STACK_ELEM_NVRAM_CLASSMARK}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_DTX_STATUS = ((P_log_api << 16) | 1610),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_CELL_BARR_STATUS = ((P_log_api << 16) | 1611),	///<Payload type {::T_ELEM_MSDB_PTR_MS_STACK_ELEM_CELL_BARR_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_IS_DEDICATED_MODE = ((P_log_api << 16) | 1612),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_CURR_TIMING_ADVANCE = ((P_log_api << 16) | 1613),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_SPEECHCODEC_BITMAP = ((P_log_api << 16) | 1614),	///<Payload type {::T_ELEM_MSDB_PTR_MS_STACK_ELEM_SPEECHCODEC_BITMAP}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_CHANNELMODE_SUPPORTED = ((P_log_api << 16) | 1615),	///<Payload type {::T_ELEM_MSDB_PTR_MS_STACK_ELEM_CHANNELMODE_SUPPORTED}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_ISGPRS_ALLOWED = ((P_log_api << 16) | 1616),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SYSTEM_RAT = ((P_log_api << 16) | 1617),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SYSTEM_RAT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SUPPORTED_RAT = ((P_log_api << 16) | 1618),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SUPPORTED_RAT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SYSTEM_BAND = ((P_log_api << 16) | 1619),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SYSTEM_BAND}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SUPPORTED_BAND = ((P_log_api << 16) | 1620),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_SUPPORTED_BAND}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_ISREGISTERED_GSM_OR_GPRS = ((P_log_api << 16) | 1621),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_GET_MS_CLASS = ((P_log_api << 16) | 1622),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_GET_MS_CLASS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_IS_REGISTER_IN_PROGRESS = ((P_log_api << 16) | 1623),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_IS_DEREGISTER_IN_PROGRESS = ((P_log_api << 16) | 1624),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_GPRSATTACH_STATUS = ((P_log_api << 16) | 1625),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_GPRSATTACH_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_NETREG_ELEM_MANUAL_PLMN_SELECT_RAT = ((P_log_api << 16) | 1626),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_DTM = ((P_log_api << 16) | 1627),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_DTM}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_HSDPA_SUPPORT = ((P_log_api << 16) | 1628),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_HSDPA_SUPPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_HSUPA_SUPPORT = ((P_log_api << 16) | 1629),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_HSUPA_SUPPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_SUPPORT = ((P_log_api << 16) | 1630),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_SUPPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_EGPRS_SUPPORT = ((P_log_api << 16) | 1631),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_EGPRS_SUPPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GAN_SUPPORT = ((P_log_api << 16) | 1632),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GAN_SUPPORT}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NOM = ((P_log_api << 16) | 1633),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NOM}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NW_TYPE = ((P_log_api << 16) | 1634),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NW_TYPE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_RAC = ((P_log_api << 16) | 1635),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_RNC = ((P_log_api << 16) | 1636),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_RNC}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GSMREG_STATE = ((P_log_api << 16) | 1637),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GSMREG_STATE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRSREG_STATE = ((P_log_api << 16) | 1638),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRSREG_STATE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GSM_CAUSE = ((P_log_api << 16) | 1639),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GSM_CAUSE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CAUSE = ((P_log_api << 16) | 1640),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CAUSE}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NET_INFO = ((P_log_api << 16) | 1641),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_NET_INFO}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GSM_NW_CAUSE = ((P_log_api << 16) | 1642),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_NW_CAUSE = ((P_log_api << 16) | 1643),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_REG_INFO = ((P_log_api << 16) | 1644),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_REG_INFO}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_REGSTATE_INFO = ((P_log_api << 16) | 1645),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_REGSTATE_INFO}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_ISGSM_REGISTERED = ((P_log_api << 16) | 1646),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_ISGPRS_REGISTERED = ((P_log_api << 16) | 1647),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_CSINVALID_SIM_MS_ME_STATUS = ((P_log_api << 16) | 1648),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_CSINVALID_SIM_MS_ME_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRSINVALID_SIM_MS_ME_STATUS = ((P_log_api << 16) | 1649),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRSINVALID_SIM_MS_ME_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_UAS_CONN_INFO = ((P_log_api << 16) | 1650),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_UAS_CONN_INFO}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_RADIO_STATUS = ((P_log_api << 16) | 1651),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_RADIO_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_CURRENT_CAMPED_PLMN = ((P_log_api << 16) | 1652),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_CURRENT_CAMPED_PLMN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_UARFCN_DL = ((P_log_api << 16) | 1653),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_UARFCN_DL}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_SPECIAL_CGSEND_MODE = ((P_log_api << 16) | 1654),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CALL_ACTIVE_BY_CHAN = ((P_log_api << 16) | 1655),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CALL_ACTIVE_BY_CHAN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CID_BY_CHAN = ((P_log_api << 16) | 1656),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CID_BY_CHAN}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CALL_ACTIVE_BY_CID = ((P_log_api << 16) | 1657),	///<Payload type {::T_ELEM_MSDB_PTR_MS_NETWORK_ELEM_GPRS_CALL_ACTIVE_BY_CID}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_PIN1_STATUS = ((P_log_api << 16) | 1658),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_PIN1_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_PIN2_STATUS = ((P_log_api << 16) | 1659),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_PIN2_STATUS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_SST = ((P_log_api << 16) | 1660),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_SST}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONS_LEN = ((P_log_api << 16) | 1661),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONSS_LEN = ((P_log_api << 16) | 1662),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_GID1_LEN = ((P_log_api << 16) | 1663),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_GID2_LEN = ((P_log_api << 16) | 1664),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_CFG_ELEM_SIM_LOCK_SUPPORTED = ((P_log_api << 16) | 1665),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_CFG_ELEM_SIM_CINGULAR_ENS_ENABLED = ((P_log_api << 16) | 1666),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_CFG_ELEM_SKT_AUTO_SEND_CONFIRMATION = ((P_log_api << 16) | 1667),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_CFG_ELEM_PLMN_NAME_FLAGS = ((P_log_api << 16) | 1668),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SPEECH_CODEC_LIST = ((P_log_api << 16) | 1669),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_SPEECH_CODEC_LIST}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CRLP = ((P_log_api << 16) | 1670),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_CC_ELEM_CRLP}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CGEREP = ((P_log_api << 16) | 1671),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CGEREP}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CSCS = ((P_log_api << 16) | 1672),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CSCS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CPWC = ((P_log_api << 16) | 1673),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_CPWC}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_PLMNSEL_REGISTERED_PLMN = ((P_log_api << 16) | 1674),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_FREQ_SEARCH_LIST = ((P_log_api << 16) | 1675),	///<Payload type {::T_ELEM_MSDB_PTR_MS_LOCAL_PHCTRL_ELEM_FREQ_SEARCH_LIST}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_STACK_ELEM_NW_MEAS_RESULT = ((P_log_api << 16) | 1676),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_DRIVER_ELEM_SPINNER_SLEEP_MODE = ((P_log_api << 16) | 1677),	///<Payload type {::T_ELEM_MSDB_UINT8}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_RAW_ICC_ID = ((P_log_api << 16) | 1678),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_RAW_ICC_ID}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONS = ((P_log_api << 16) | 1679),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONSS = ((P_log_api << 16) | 1680),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_ONSS}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_INFO = ((P_log_api << 16) | 1681),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_CPHS_INFO}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_ECC_REC_LIST = ((P_log_api << 16) | 1682),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_ECC_REC_LIST}
	API_CAPI_MSG_ELEM_MSDB_PTR_MS_SIM_ELEM_ICC_ID = ((P_log_api << 16) | 1683),	///<Payload type {::T_ELEM_MSDB_PTR_MS_SIM_ELEM_ICC_ID}
	/* End of log_api */

	/* Start of log_general */
#ifndef SDTENV
	MEMORY_DUMP = ((P_log_general << 16) | 1),
#else
	MEMORY_DUMP,
#endif
	IP_PACKET_DUMP,
	ETHERNET_FRAME_DUMP,
	/* End of log_general */

#if defined(STACK_wedge) && defined(UMTS)
	/* Start of umtsfw */
#ifndef SDTENV
	WTT_LOG_GROUP = ((P_umtsfw << 16) | 1),
#else
	WTT_LOG_GROUP,
#endif
	/* End of umtsfw */
#endif // #if defined(STACK_wedge) && defined(UMTS)

	/* Start of AUDIO */
	DSP_DATA = ((LOGID_AUDIO << 16) | 1),
	AUDIO_DATA = ((LOGID_AUDIO << 16) | 2),
	/* End of AUDIO */

	LAST_LOG_SIGNAL
} log_signal_id_t;

#endif /* LOG_SIG_CODE_H */
