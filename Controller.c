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
Char CTRLTaskStk[CTRL_TASK_STK_SIZE];

static uint8_t     CTRLPowerMask;
//static bool         CTRLPullUp5or28;

/*
 *
 * TARGET RELAYS OUTPUT MAP
 *
 */

CTRL_DO g_CTRTargetRelaysTable[] =
{
 {CTRL_TARGET_RELAY0, 0},
 {CTRL_TARGET_RELAY1, 0}
};

#define CRTL_TARGET_RELAYS_TABLE_COUNT   (sizeof (g_CTRTargetRelaysTable) / sizeof (g_CTRTargetRelaysTable[0]))

/*
 *
 * TARGET STATUS OUTPUT MAP
 *
 */

CTRL_DO g_CTRTargetStatusOutpusTable[] =
{
 {CTRL_TARGET_STATUS_OUT0, 0},
 {CTRL_TARGET_STATUS_OUT1, 0}
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
 CTRLCfgDInHotCupAtIndex
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
        pcfg = &CTRLCfgTbl[i];
        pcfg->CTRLFunctSel = CTRL_FNCT_TOGLE;
        pcfg->CTRLTargetRelay = 1;

    }
}

void CTRLConfigureIO(void)
{
    uint8_t i;
    CTRL_CFG *pcfg;

    for (i = 0; i < CTRL_MAX_DI; i++){
        pcfg = &CTRLCfgTbl[i];
        if (pcfg->CTRLFunctSel < CRTL_CFG_DIN_TABLE_COUNT){
            g_CTRLCfgDInTable[pcfg->CTRLFunctSel](i);
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

    l_pcfg = &CTRLCfgTbl[index];
    l_din = DIGet(index);
    DIClr(index);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncMomentryAtIndex(uint8_t index)
{
    uint32_t l_din;
    CTRL_CFG *l_pcfg;

    l_pcfg = &CTRLCfgTbl[index];
    l_din = DIGet(index);
    l_pcfg->CTRLOut = l_din;
}

static void CTRLExecFuncSetOnAtIndex(uint8_t index)
{

}

static void CTRLExecFuncSetOffAtIndex(uint8_t index)
{

}

static void CTRLExecFuncSequenceAtIndex(uint8_t index)
{

}

static void CTRLExecFuncBinaryCountAtIndex(uint8_t index)
{

}

static void CTRLExecFuncOneShotAtIndex(uint8_t index)
{

}

static void CTRLExecFuncMasterOffResetAtIndex(uint8_t index)
{

}

static void CTRLExecFuncHotCupAtIndex(uint8_t index)
{

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
 CTRLExecFuncHotCupAtIndex
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

}

static void CTRLUpdateRelaySetOff(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelaySequence(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayBinaryCount(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayOneShot(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayMasterOffReset(uint32_t index, uint32_t val)
{

}

static void CTRLUpdateRelayHotCup(uint32_t index, uint32_t val)
{

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
 CTRLUpdateRelayHotCup
};

#define CRTL_UPDATE_RELAY_TABLE_COUNT   (sizeof (g_CTRLUpdateRelayTable) / sizeof (g_CTRLUpdateRelayTable[0]))


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
                g_CTRLUpdateRelayTable[pcfg->CTRLFunctSel](i, pcfg->CTRLOutFilt);
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

