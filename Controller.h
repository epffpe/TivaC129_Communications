/*
 * Controller.h
 *
 *  Created on: Jan 11, 2017
 *      Author: epffpe
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define CTRL_TASK_PRIO              9
#define CTRL_TASK_DLY_TICKS         10
#define CTRL_TASK_STK_SIZE          512

#define CTRL_MAX_DI                 2


#define CTRL_FNCT_NONE              0
#define CTRL_FNCT_TOGLE             1
#define CTRL_FNCT_MOMENTARY         2
#define CTRL_FNCT_SETON             3
#define CTRL_FNCT_SETOFF            4
#define CTRL_FNCT_SEQUENCE          5
#define CTRL_FNCT_BINARY_COUNT      6
#define CTRL_FNCT_ONE_SHOT          7
#define CTRL_FNCT_MASTER_OFF_RESET  8
#define CTRL_FNCT_HOTCUP            9
#define CTRL_FNCT_SET_PRIORITY      10


#define CTRL_TARGET_RELAY0          0
#define CTRL_TARGET_RELAY1          1

#define CTRL_TARGET_STATUS_OUT0     2
#define CTRL_TARGET_STATUS_OUT1     3


#ifdef CTRL_GLOBALS
#define CTRL_EXT
#else
#define CTRL_EXT extern
#endif

typedef struct ctrl_cfg {
    uint32_t    CTRLParam0;
    uint32_t    CTRLParam1;
    uint32_t    CTRLTargetRelay;
    uint32_t    CTRLTargetStatus;
    uint32_t    CTRLHoldDly;
    uint32_t    CTRLOut;
    uint32_t    CTRLOutFilt;
    uint8_t     CTRLFunctSel;
    uint8_t     CTRLPullUp;
    uint8_t     CTRLPowerEn;
    uint8_t     CTRLPriority;
}CTRL_CFG;

typedef struct ctrl_do {
    const uint32_t      CTRLDout;
    uint32_t            CTRLVal;
    uint8_t             CTRLPriority;
}CTRL_DO;


typedef void (*pfnCTRLCfgIn)(uint8_t);
typedef void (*pfnCTRLUpdateRelay)(uint32_t, uint32_t);

#ifdef __cplusplus
extern "C"
{
#endif


CTRL_EXT CTRL_CFG CTRLCfgTbl[CTRL_MAX_DI];




void CTRLInit(void);
void CTRLConfigureIO(void);


#ifdef __cplusplus
}
#endif



#endif /* CONTROLLER_H_ */
