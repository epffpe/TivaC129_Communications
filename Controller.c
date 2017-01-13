/*
 * Controller.c
 *
 *  Created on: Jan 11, 2017
 *      Author: epffpe
 */

/*
 *
 * INCLUDE FILES
 *
 */

#define CTRL_GLOBALS
#include "includes.h"


/*
 *
 * GLOBAL VARIABLES
 *
 */


Task_Struct CTRLTaskStruct;
Char        CTRLTaskStk[CTRL_TASK_STK_SIZE];

static uint8_t     CTRLPowerMask;
//static bool         CTRLPullUp5or28;

/*
 *
 * INPUT MAPPING
 *
 */

const uint8_t g_CTRLInputTable[] =
{
 CTRL_BOARD_SW1,
 CTRL_BOARD_SW2,
 CTRL_MKII_S2,
 CTRL_MKII_S1
};

#define CTRL_INPUT_TABLE_COUNT   (sizeof (g_CTRLInputTable) / sizeof (g_CTRLInputTable[0]))
#define CTRL_MAX_DI             CTRL_INPUT_TABLE_COUNT

CTRL_CFG CTRLCfgTbl[CTRL_INPUT_TABLE_COUNT];

/*
 *
 * TARGET RELAYS OUTPUT MAP
 *
 */

CTRL_DO g_CTRTargetRelaysTable[] =
{
 {CTRL_TARGET_RELAY0, 0, 0, 0xFF, CTRL_ONESHOT_IDLE},
 {CTRL_TARGET_RELAY1, 0, 0, 0xFF, CTRL_ONESHOT_IDLE}
};

#define CRTL_TARGET_RELAYS_TABLE_COUNT   (sizeof (g_CTRTargetRelaysTable) / sizeof (g_CTRTargetRelaysTable[0]))

/*
 *
 * TARGET STATUS OUTPUT MAP
 *
 */

CTRL_DO g_CTRTargetStatusOutpusTable[] =
{
 {CTRL_TARGET_STATUS_OUT0, 0, 0, 0xFF, CTRL_ONESHOT_IDLE},
 {CTRL_TARGET_STATUS_OUT1, 0, 0, 0xFF, CTRL_ONESHOT_IDLE},
 {CTRL_TARGET_STATUS_OUT2, 0, 0, 0xFF, CTRL_ONESHOT_IDLE}
};

#define CRTL_TARGET_STATUS_OUT_TABLE_COUNT   (sizeof (g_CTRTargetStatusOutpusTable) / sizeof (g_CTRTargetStatusOutpusTable[0]))



/*
 *
 *  CONFIGURE INPUTS
 *
 */
static void CTRLCfgDInNoneAtIndex(uint8_t index)
{
    DICfgMode(index, DI_MODE_LOW);
    DISetBypassEn(index, false);

}

static void CTRLCfgDInTogleAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_EDGE_LOW_GOING);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInMomentaryAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInSetOnAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInSetOffAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInSequenceAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInBinaryCountAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInOneShotAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_EDGE_LOW_GOING);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInMasterOffResetAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInHotCupAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_INV);
    DISetBypassEn(index, false);
}

static void CTRLCfgDInSetPriorityAtIndex(uint8_t index)
{
    DICfgDebounce (index, 50, 500, 200);
    DISetDebounceEn(index, true);
    DICfgMode(index, DI_MODE_TOGGLE_LOW_GOING);
    DISetBypassEn(index, false);
}

/*
 *
 * TABLE FOR INPUT CONFIGURATION
 *
 */
const pfnCTRLCfgIn g_CTRLCfgDInTable[] =
{
 CTRLCfgDInNoneAtIndex,
 CTRLCfgDInTogleAtIndex,
 CTRLCfgDInMomentaryAtIndex,
 CTRLCfgDInSetOnAtIndex,
 CTRLCfgDInSetOffAtIndex,
 CTRLCfgDInSequenceAtIndex,
 CTRLCfgDInBinaryCountAtIndex,
 CTRLCfgDInOneShotAtIndex,
 CTRLCfgDInMasterOffResetAtIndex,
 CTRLCfgDInHotCupAtIndex,
 CTRLCfgDInSetPriorityAtIndex
};

#define CRTL_CFG_DIN_TABLE_COUNT   (sizeof (g_CTRLCfgDInTable) / sizeof (g_CTRLCfgDInTable[0]))

/*
 *
 * CONFIGURE OUTPUTS
 *
 */
void CTRLCfgDOut()
{
    uint8_t i;
    for (i = 0; i < DIO_MAX_DO; i++){
        DOCfgMode(i, DO_MODE_DIRECT, false);
    }
}

/*
 *
 * INITIALIZATION FUNCTIONS
 *
 */
static void CTRLLoadNVParam(void)
{
    uint8_t i;
    CTRL_CFG *pcfg;
    for (i = 0; i < CTRL_MAX_DI; i++){
        pcfg                    = &CTRLCfgTbl[i];
        pcfg->CTRLFunctSel      = CTRL_FNCT_TOGLE;
        pcfg->CTRLTargetRelay   = (1 << 0)| (1 << 0);
        pcfg->CTRLTargetStatus   = (1 << 2);

    }
    pcfg = &CTRLCfgTbl[1];
    pcfg->CTRLFunctSel      = CTRL_FNCT_ONE_SHOT;
    pcfg->CTRLTargetRelay   = 1;
    pcfg->CTRLPriority      = 0xFE;
    pcfg->CTRLParam0        = 6000;

    pcfg                    = &CTRLCfgTbl[2];
    pcfg->CTRLFunctSel      = CTRL_FNCT_SETON;
    pcfg->CTRLTargetRelay   = (1 << 0);
    pcfg->CTRLTargetStatus   = (1 << 0);

    pcfg                    = &CTRLCfgTbl[3];
    pcfg->CTRLFunctSel      = CTRL_FNCT_MASTER_OFF_RESET;
    pcfg->CTRLTargetRelay   = (1 << 0);
    pcfg->CTRLTargetStatus   = (1 << 0);
}

void CTRLConfigureIO(void)
{
    uint8_t i, index;
    CTRL_CFG *pcfg;

    for (i = 0; i < CTRL_MAX_DI; i++){
        pcfg = &CTRLCfgTbl[i];
        if (pcfg->CTRLFunctSel < CRTL_CFG_DIN_TABLE_COUNT){
            index = g_CTRLInputTable[i];
            g_CTRLCfgDInTable[pcfg->CTRLFunctSel](index);
        }

    }
    CTRLCfgDOut();
}

/*
 *
 * FUNCTIONS CALLED IN THE LOOP
 *
 */

static void CTRLReadPower(void)
{

}

/*
 *
 * FUNTIONS EXECUTE
 *
 */


static void CTRLExecFuncNoneAtIndex(uint8_t index)
{

}

static void CTRLExecFuncTogleAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    DIClr(index);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncMomentryAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncSetOnAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncSetOffAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncSequenceAtIndex(uint8_t index)
{

}

static void CTRLExecFuncBinaryCountAtIndex(uint8_t index)
{

}

static void CTRLExecFuncOneShotAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    DIClr(index);
    l_pcfg->CTRLOut = l_din ? l_pcfg->CTRLParam0 : 0;
}

static void CTRLExecFuncMasterOffResetAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;
    uint8_t i;

    l_pcfg = &CTRLCfgTbl[index];
    i = g_CTRLInputTable[index];
    l_din = DIGet(i);
    l_pcfg->CTRLOut = l_din;
    l_pcfg->CTRLTargetRelay = 0xFFFFFFFF;
    l_pcfg->CTRLTargetStatus = 0xFFFFFFFF;
}

static void CTRLExecFuncHotCupAtIndex(uint8_t index)
{

}

static void CTRLExecFuncSetPriorityAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;

    l_pcfg = &CTRLCfgTbl[index];
    l_din = DIGet(index);
    l_pcfg->CTRLOut = l_din ? l_pcfg->CTRLPriority : l_pcfg->CTRLParam0;
}


/*
 *
 * TABLE FOR INPUT CONFIGURATION
 *
 */
const pfnCTRLCfgIn g_CTRLExecFuncTable[] =
{
 CTRLExecFuncNoneAtIndex,
 CTRLExecFuncTogleAtIndex,
 CTRLExecFuncMomentryAtIndex,
 CTRLExecFuncSetOnAtIndex,
 CTRLExecFuncSetOffAtIndex,
 CTRLExecFuncSequenceAtIndex,
 CTRLExecFuncBinaryCountAtIndex,
 CTRLExecFuncOneShotAtIndex,
 CTRLExecFuncMasterOffResetAtIndex,
 CTRLExecFuncHotCupAtIndex,
 CTRLExecFuncSetPriorityAtIndex
};

#define CRTL_EXEC_FUNC_TABLE_COUNT   (sizeof (g_CTRLExecFuncTable) / sizeof (g_CTRLExecFuncTable[0]))


/*
 *
 * READ INPUTS AND EXECUTE THE FUNCTION ASIGNED
 *
 */

static void CTRLExec(void)
{
    uint8_t i;
    CTRL_CFG *pcfg;

    for (i = 0; i < CTRL_MAX_DI; i++){
        pcfg = &CTRLCfgTbl[i];
        if (pcfg->CTRLFunctSel < CRTL_EXEC_FUNC_TABLE_COUNT){
            g_CTRLExecFuncTable[pcfg->CTRLFunctSel](i);
        }

    }
}

static void CTRLPowerFilter(void)
{
    uint8_t i;
    CTRL_CFG *pcfg;

    for (i = 0; i < CTRL_MAX_DI; i++){
        pcfg = &CTRLCfgTbl[i];
        pcfg->CTRLOutFilt = pcfg->CTRLOut;

    }
}

/*
 *
 *
 *
 */

static void CTRLUpdateRelayNone(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayTogle(uint32_t index, uint32_t val)
{
    if (val) {
        g_CTRTargetRelaysTable[index].CTRLVal = g_CTRTargetRelaysTable[index].CTRLVal ? 0 : 1;
    }
}

static void CTRLUpdateRelayMomentary(uint32_t index, uint32_t val)
{
    g_CTRTargetRelaysTable[index].CTRLVal = val;
}

static void CTRLUpdateRelaySetOn(uint32_t index, uint32_t val)
{
    if (val) {
        g_CTRTargetRelaysTable[index].CTRLVal = 1;
    }
}

static void CTRLUpdateRelaySetOff(uint32_t index, uint32_t val)
{
    if (val) {
        g_CTRTargetRelaysTable[index].CTRLVal = 0;
    }
}

static void CTRLUpdateRelaySequence(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayBinaryCount(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayOneShot(uint32_t index, uint32_t val)
{
    switch(g_CTRTargetRelaysTable[index].CTRLState){
    case CTRL_ONESHOT_IDLE:
        if (val){
            g_CTRTargetRelaysTable[index].CTRLVal = 1;
            g_CTRTargetRelaysTable[index].CTRLState = CTRL_ONESHOT_WAIT;
            g_CTRTargetRelaysTable[index].CTRLCtr = val;
        }
        break;
    case CTRL_ONESHOT_WAIT:
        if (g_CTRTargetRelaysTable[index].CTRLCtr > 0){
            g_CTRTargetRelaysTable[index].CTRLCtr--;
            if (g_CTRTargetRelaysTable[index].CTRLCtr == 0){
                g_CTRTargetRelaysTable[index].CTRLVal = 0;
                g_CTRTargetRelaysTable[index].CTRLState = CTRL_ONESHOT_IDLE;
            }
        }
        break;
    default:
        g_CTRTargetRelaysTable[index].CTRLState = CTRL_ONESHOT_IDLE;
        g_CTRTargetRelaysTable[index].CTRLVal = 0;
        break;
    }
}

static void CTRLUpdateRelayMasterOffReset(uint32_t index, uint32_t val)
{
    if (val) {
        g_CTRTargetRelaysTable[index].CTRLVal = 0;
        g_CTRTargetRelaysTable[index].CTRLCtr = 0;
        g_CTRTargetRelaysTable[index].CTRLState = CTRL_ONESHOT_IDLE;
    }
}

static void CTRLUpdateRelayHotCup(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelaySetPriority(uint32_t index, uint32_t val)
{
    g_CTRTargetRelaysTable[index].CTRLPriority = val;
}

/*
 *
 * TABLE FOR UPDATE RELAY
 *
 */
const pfnCTRLUpdateRelay g_CTRLUpdateRelayTable[] =
{
 CTRLUpdateRelayNone,
 CTRLUpdateRelayTogle,
 CTRLUpdateRelayMomentary,
 CTRLUpdateRelaySetOn,
 CTRLUpdateRelaySetOff,
 CTRLUpdateRelaySequence,
 CTRLUpdateRelayBinaryCount,
 CTRLUpdateRelayOneShot,
 CTRLUpdateRelayMasterOffReset,
 CTRLUpdateRelayHotCup,
 CTRLUpdateRelaySetPriority
};

#define CRTL_UPDATE_RELAY_TABLE_COUNT   (sizeof (g_CTRLUpdateRelayTable) / sizeof (g_CTRLUpdateRelayTable[0]))


/*
 *
 *
 *
 */

static void CTRLUpdateStatusNone(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateStatusTogle(uint32_t index, uint32_t val)
{
    if (val) {
        g_CTRTargetStatusOutpusTable[index].CTRLVal = g_CTRTargetStatusOutpusTable[index].CTRLVal ? 0 : 1;
    }
}

static void CTRLUpdateStatusMomentary(uint32_t index, uint32_t val)
{
    g_CTRTargetStatusOutpusTable[index].CTRLVal = val;
}

static void CTRLUpdateStatusSetOn(uint32_t index, uint32_t val)
{
    if (val){
        g_CTRTargetStatusOutpusTable[index].CTRLVal = 1;
    }
}

static void CTRLUpdateStatusSetOff(uint32_t index, uint32_t val)
{
    if (val){
        g_CTRTargetStatusOutpusTable[index].CTRLVal = 0;
    }
}

static void CTRLUpdateStatusSequence(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateStatusBinaryCount(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateStatusOneShot(uint32_t index, uint32_t val)
{
    switch(g_CTRTargetStatusOutpusTable[index].CTRLState){
    case CTRL_ONESHOT_IDLE:
        if (val){
            g_CTRTargetStatusOutpusTable[index].CTRLVal = 1;
            g_CTRTargetStatusOutpusTable[index].CTRLState = CTRL_ONESHOT_WAIT;
            g_CTRTargetStatusOutpusTable[index].CTRLCtr = val;
        }
        break;
    case CTRL_ONESHOT_WAIT:
        if (g_CTRTargetStatusOutpusTable[index].CTRLCtr > 0){
            g_CTRTargetStatusOutpusTable[index].CTRLCtr--;
            if (g_CTRTargetStatusOutpusTable[index].CTRLCtr == 0){
                g_CTRTargetStatusOutpusTable[index].CTRLVal = 0;
                g_CTRTargetStatusOutpusTable[index].CTRLState = CTRL_ONESHOT_IDLE;
            }
        }
        break;
    default:
        g_CTRTargetStatusOutpusTable[index].CTRLState = CTRL_ONESHOT_IDLE;
        g_CTRTargetStatusOutpusTable[index].CTRLVal = 0;
        break;
    }
}

static void CTRLUpdateStatusMasterOffReset(uint32_t index, uint32_t val)
{
    if (val){
        g_CTRTargetStatusOutpusTable[index].CTRLVal = 0;
        g_CTRTargetStatusOutpusTable[index].CTRLCtr = 0;
        g_CTRTargetStatusOutpusTable[index].CTRLState = CTRL_ONESHOT_IDLE;
    }
}

static void CTRLUpdateStatusHotCup(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateStatusSetPriority(uint32_t index, uint32_t val)
{
    g_CTRTargetStatusOutpusTable[index].CTRLPriority = val;
}

/*
 *
 * TABLE FOR UPDATE RELAY
 *
 */
const pfnCTRLUpdateRelay g_CTRLUpdateStatusTable[] =
{
 CTRLUpdateStatusNone,
 CTRLUpdateStatusTogle,
 CTRLUpdateStatusMomentary,
 CTRLUpdateStatusSetOn,
 CTRLUpdateStatusSetOff,
 CTRLUpdateStatusSequence,
 CTRLUpdateStatusBinaryCount,
 CTRLUpdateStatusOneShot,
 CTRLUpdateStatusMasterOffReset,
 CTRLUpdateStatusHotCup,
 CTRLUpdateStatusSetPriority
};

#define CRTL_UPDATE_STATUS_TABLE_COUNT   (sizeof (g_CTRLUpdateStatusTable) / sizeof (g_CTRLUpdateStatusTable[0]))

/*
 *
 *
 *
 */

static void CTRLUpdateTargetRelaysAtIndex(uint32_t index)
{
    uint8_t i;
    uint32_t mask;
    uint32_t val;
    CTRL_CFG *pcfg;

    pcfg = &CTRLCfgTbl[index];
    for (i = 0; i < CRTL_TARGET_RELAYS_TABLE_COUNT; i++){
        mask = (1 << i);
        val = pcfg->CTRLTargetRelay & mask;
        if (val) {
            if (pcfg->CTRLFunctSel < CRTL_UPDATE_RELAY_TABLE_COUNT){
                if (pcfg->CTRLPriority <= g_CTRTargetRelaysTable[i].CTRLPriority) {
                    g_CTRLUpdateRelayTable[pcfg->CTRLFunctSel](i, pcfg->CTRLOutFilt);
                }
            }
        }
    }

}

static void CTRLUpdateTargetStatusOutAtIndex(uint32_t index)
{
    uint8_t i;
    uint32_t mask;
    uint32_t val;
    CTRL_CFG *pcfg;

    pcfg = &CTRLCfgTbl[index];
    for (i = 0; i < CRTL_TARGET_STATUS_OUT_TABLE_COUNT; i++){
        mask = (1 << i);
        val = pcfg->CTRLTargetStatus & mask;
        if (val) {
//            DOSet(g_CTRTargetStatusOutpusTable[i], pcfg->CTRLOutFilt ? 1 : 0);
            if (pcfg->CTRLFunctSel < CRTL_UPDATE_STATUS_TABLE_COUNT){
                if (pcfg->CTRLPriority <= g_CTRTargetStatusOutpusTable[i].CTRLPriority) {
                    g_CTRLUpdateStatusTable[pcfg->CTRLFunctSel](i, pcfg->CTRLOutFilt);
                }
            }
        }
    }
}


static void CTRLUpdateOutputs(void)
{
    uint8_t i;

    for (i = 0; i < CTRL_MAX_DI; i++){
        CTRLUpdateTargetRelaysAtIndex(i);
        CTRLUpdateTargetStatusOutAtIndex(i);
    }
    for (i = 0; i < CRTL_TARGET_RELAYS_TABLE_COUNT; i++){
        DOSet(g_CTRTargetRelaysTable[i].CTRLDout, g_CTRTargetRelaysTable[i].CTRLVal ? 1 : 0);
    }

    for (i = 0; i < CRTL_TARGET_STATUS_OUT_TABLE_COUNT; i++){
        DOSet(g_CTRTargetStatusOutpusTable[i].CTRLDout, g_CTRTargetStatusOutpusTable[i].CTRLVal ? 1 : 0);
    }
}

/*
 *
 * MAIN TASK
 *
 */

Void CTRLTask(UArg arg0, UArg arg1)
{
    arg0 = arg0;
    arg1 = arg1;


    CTRLLoadNVParam();
    CTRLConfigureIO();

    for(;;){
        Task_sleep(CTRL_TASK_DLY_TICKS);
        CTRLReadPower();
        CTRLExec();
        CTRLPowerFilter();
        CTRLUpdateOutputs();
    }
}

/*
 *
 * CONTROL MODULE INITIALIZATION
 *
 */

void CTRLInit(void)
{
    CTRL_CFG *pcfg;
    uint32_t i;
    Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;
    /* Make sure Error_Block is initialized */
    Error_init(&eb);

    CTRLPowerMask = 0x00;
    pcfg = &CTRLCfgTbl[0];
    for (i = 0; i < CTRL_MAX_DI; i++) {
        pcfg->CTRLFunctSel      = CTRL_FNCT_NONE;
        pcfg->CTRLParam0        = 0;
        pcfg->CTRLParam1        = 0;
        pcfg->CTRLTargetRelay   = 0;
        pcfg->CTRLTargetStatus  = 0;
        pcfg->CTRLHoldDly       = 50;
        pcfg->CTRLOut           = 0;
        pcfg->CTRLOutFilt       = 0;
        pcfg->CTRLPullUp        = 0;
        pcfg->CTRLPowerEn       = 0;
        pcfg->CTRLPriority      = 0xFF;

        pcfg++;
    }

    /*
     *  Create the Task that farms out incoming TCP connections.
     *  arg0 will be the port that this task listens to.
     */
    Task_Params_init(&taskParams);
    taskParams.stackSize = CTRL_TASK_STK_SIZE;
    taskParams.priority = CTRL_TASK_PRIO;
    taskParams.arg0 = (UArg)0;
    taskParams.stack = CTRLTaskStk;
    taskHandle = Task_create((Task_FuncPtr)CTRLTask, &taskParams, &eb);
    if (taskHandle == NULL) {
        System_printf("Failed to create CTRLTask Task\n");
    }

    System_flush();
}

