/*
 * DIO.h
 *
 *  Created on: Jan 10, 2017
 *      Author: epffpe
 */

#ifndef DIO_H_
#define DIO_H_

/*
 *  CONFIGURATION CONSTANTS
 *
 */

#ifndef CFG_H

#define DIO_TASK_PRIO           10
#define DIO_TASK_DLY_TICKS      10
#define DIO_TASK_STK_SIZE       1024

#define DIO_MAX_DI              2
#define DIO_MAX_DO              2

#define DI_EDGE_EN              1

#define DO_BLINK_MODE_EN        1

#endif

#ifdef DIO_GLOBALS
#define DIO_EXT
#else
#define DIO_EXT extern
#endif

/*
 *
 * DISCRETE INPUT CONSTANT
 *
 */

                                            /* DI MODE SELECTOR VALUES */
#define DI_MODE_LOW                 0       /* Input is forced low */
#define DI_MODE_HIGH                1       /* Input is forced high */
#define DI_MODE_DIRECT              2       /* Input is based on state of physical input */
#define DI_MODE_INV                 3       /* Input is based on the complement of the physical input */
#define DI_MODE_EDGE_LOW_GOING      4       /* Low going edge detection of input */
#define DI_MODE_EDGE_HIGH_GOING     5       /* High going edge detection of input */
#define DI_MODE_EDGE_BOTH           6       /* Both low and high going edge detection of input */
#define DI_MODE_TOGGLE_LOW_GOING    7       /* Low going edge detection of input */
#define DI_MODE_TOGGLE_HIGH_GOING   8       /* High going edge detection of input */

                                            /* DI EDGE TRIGGERING MODE SELECTOR VALUES */
#define DI_EDGE_LOW_GOING           0       /* Negative going edge */
#define DI_EDGE_HIGH_GOING          1       /* Positive going edge */
#define DI_EDGE_BOTH                2       /* Both positive and negative going */

/*
 *
 * DISCRETE OUTPUT CONSTANTS
 *
 */
                                           /* DO MODE SELECTOR VALUES */
#define DO_MODE_LOW                 0      /* Output will be low */
#define DO_MODE_HIGH                1      /* Output will be high */
#define DO_MODE_DIRECT              2      /* Output is based on state of user supplied state */
#define DO_MODE_BLINK_SYNC          3      /* Sync. Blink mode */
#define DO_MODE_BLINK_ASYNC         4      /* Async. Blink mode */

                                           /* DO BLINK MODE ENABLE SELECTOR VALUES */
#define DO_BLINK_EN                 0      /* Blink is always enabled */
#define DO_BLINK_EN_NORMAL          1      /* Blink depends on user request's state */
#define DO_BLINK_EN_INV             2      /* Blink depends on the complemented user request's state */



/*
 *
 * DATA TYPES
 *
 */
typedef enum {
    KEY_STATE_UP,
    KEY_STATE_DEBOUNCE,
    KEY_STATE_RPT_START_DLY,
    KEY_STATE_RPT_DLY
} DIO_DEBOUNCE_STATE;


typedef struct dio_di {                   /* DISCRETE INPUT CHANNEL DATA STRUCTURE */
    bool        DIIn;                     /* Current state of sensor input */
    uint32_t    DIVal;                    /* State of discrete input channel (or # of transitions) */
    bool        DIPrev;                   /* Previous state of DIIn for edge detection */
    bool        DIBypassEn;               /* Bypass enable switch (Bypass when TRUE) */
    uint8_t     DIModeSel;                /* Discrete input channel mode selector */
#if DI_EDGE_EN
    void        (*DITrigFnct)(void *);    /* Function to execute if edge triggered */
    void        *DITrigFnctArg;           /* arguments passed to function when edge detected */
#endif
                                          /* DEBOUNCE PARAMETERS */
    bool        DIDebounceEn;
    uint32_t    DIDownTmr;
    uint32_t    DIDebounceDlyCtr;
    uint32_t    DIDebounceDly;
    uint32_t    DIRptStartDlyCtr;
    uint32_t    DIRptStartDly;
    uint32_t    DIRptDlyCtr;
    uint32_t    DIRptDly;
    uint32_t    DIScanState;
    void        (*DIDebFnct)(void *);    /* Function to execute if edge triggered */
    void        *DIDebFnctArg;           /* arguments passed to function when edge detected */


}DIO_DI;

typedef struct dio_do {
    bool        DOOut;
    bool        DOCtrl;
    bool        DOBypass;
    bool        DOBypassEn;
    uint8_t     DOModeSel;
    uint8_t     DOBlinkEnSel;
    bool        DOInv;
#if DO_BLINK_MODE_EN
    uint32_t    DOA;
    uint32_t    DOB;
    uint32_t    DOBCtr;
#endif
}DIO_DO;

typedef struct dio_di_map {
    unsigned int     DIOPinMap;
}DIO_MAP;




/*
 *
 * GLOBAL VARIABLES
 *
 */

DIO_EXT DIO_DI      DITbl[DIO_MAX_DI];
DIO_EXT DIO_DO      DOTbl[DIO_MAX_DO];



/*
 *
 * FUNCTION PROTOTYPES API
 *
 */

void DIOInit(void);

void        DICfgMode(uint8_t n, uint8_t mode);
uint32_t    DIGet(uint8_t n);
void        DISetBypassEn (uint8_t n, bool state);
void        DISetBypass(uint8_t n, uint32_t val);

#if         DI_EDGE_EN
void        DIClr(uint8_t n);
void        DICfgEdgeDetectFnct(uint8_t n, void (*fnct)(void *), void *arg);
#endif

void        DOCfgMode (uint8_t n, uint8_t mode, bool inv);
bool        DOGet (uint8_t n);
void        DOSet (uint8_t n, bool state);
void        DOSetBypassEn (uint8_t n, bool state);
void        DOSetBypass (uint8_t n, bool state);

#if         DO_BLINK_MODE_EN
void        DOCfgBlink (uint8_t n, uint8_t mode, uint32_t a, uint32_t b);
void        DOSetSyncCtrMax(uint32_t val);
#endif


#endif /* DIO_H_ */


void DIOInitIO(void);
void DIRd(void);
void DOWr(void);


