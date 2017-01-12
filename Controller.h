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

//#define CTRL_MAX_DI                 2


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

#define CTRL_BOARD_SW1              0
#define CTRL_BOARD_SW2              1
#define CTRL_MKII_S1                3
#define CTRL_MKII_S2                2

#define CTRL_TARGET_RELAY0          0
#define CTRL_TARGET_RELAY1          1

#define CTRL_TARGET_STATUS_OUT0     2
#define CTRL_TARGET_STATUS_OUT1     3
#define CTRL_TARGET_STATUS_OUT2     4


#ifdef CTRL_GLOBALS
#define CTRL_EXT
#else
#define CTRL_EXT extern
#endif

typedef struct ctrl_cfg {
    uint32_t    CTRLParam0;             /* First parameter */
    uint32_t    CTRLParam1;             /* Second parameter */
    uint32_t    CTRLTargetRelay;        /* Target relays this function is going to change */
    uint32_t    CTRLTargetStatus;       /* Target status output this function is going to change */
    uint32_t    CTRLHoldDly;            /* Debounce time */
    uint32_t    CTRLOut;                /* Output of the function before the power filter */
    uint32_t    CTRLOutFilt;            /* Output of the function after the power filter */
    uint8_t     CTRLFunctSel;           /* Function selected */
    uint8_t     CTRLPullUp;             /*  */
    uint8_t     CTRLPowerEn;            /*  */
    uint8_t     CTRLPriority;           /* Priority of the input. Needs equal or higher priority to change an output */
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


//CTRL_EXT CTRL_CFG CTRLCfgTbl[CTRL_MAX_DI];




void CTRLInit(void);
void CTRLConfigureIO(void);


#ifdef __cplusplus
}
#endif



#endif /* CONTROLLER_H_ */
