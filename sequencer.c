/*
 * secuencer.c
 *
 *  Created on: Jan 13, 2017
 *      Author: epffpe
 */

#define SEQUENCER_GLOBALS
#include "includes.h"

static SEQ_PARAM g_SEQSequence1Table[SEQ_SEQUENCE1_SIZE];
static SEQ_PARAM g_SEQSequence2Table[SEQ_SEQUENCE2_SIZE];
static SEQ_PARAM g_SEQSequence3Table[SEQ_SEQUENCE3_SIZE];
static SEQ_PARAM g_SEQSequence4Table[SEQ_SEQUENCE4_SIZE];


void SEQInit(void)
{
    uint8_t i;

    for (i = 0; i < SEQ_SEQUENCE1_SIZE; i++){
        g_SEQSequence1Table[i].SEQElement = i;
        g_SEQSequence1Table[i].SEQDly = 0;
    }
    g_SEQSequence1Table[i-1].SEQElement |= SEQ_CTL_END;
    for (i = 0; i < SEQ_SEQUENCE2_SIZE; i++){
        g_SEQSequence2Table[i].SEQElement = i;
        g_SEQSequence2Table[i].SEQDly = 0;
    }
    g_SEQSequence2Table[i-1].SEQElement |= SEQ_CTL_END;
    for (i = 0; i < SEQ_SEQUENCE3_SIZE; i++){
        g_SEQSequence2Table[i].SEQElement = i;
        g_SEQSequence2Table[i].SEQDly = 0;
    }
    g_SEQSequence2Table[i-1].SEQElement |= SEQ_CTL_END;
    for (i = 0; i < SEQ_SEQUENCE4_SIZE; i++){
        g_SEQSequence2Table[i].SEQElement = i;
        g_SEQSequence2Table[i].SEQDly = 0;
    }
    g_SEQSequence2Table[i-1].SEQElement |= SEQ_CTL_END;
}

void SEQSequenceStepConfigure(uint32_t ui32SequenceNum, uint32_t ui32Step, uint32_t ui32Config)
{
    switch(ui32SequenceNum){
    case SEQ_SEQUENCE1:
        if (ui32Step < SEQ_SEQUENCE1_SIZE){
            g_SEQSequence1Table[ui32Step].SEQElement = ui32Config;
        }
        break;
    case SEQ_SEQUENCE2:
        if (ui32Step < SEQ_SEQUENCE2_SIZE){
            g_SEQSequence2Table[ui32Step].SEQElement = ui32Config;
        }
        break;
    case SEQ_SEQUENCE3:
        if (ui32Step < SEQ_SEQUENCE3_SIZE){
            g_SEQSequence3Table[ui32Step].SEQElement = ui32Config;
        }
        break;
    case SEQ_SEQUENCE4:
        if (ui32Step < SEQ_SEQUENCE4_SIZE){
            g_SEQSequence4Table[ui32Step].SEQElement = ui32Config;
        }
        break;
    default:
        break;
    }
}

int SEQGetValue(uint32_t ui32SequenceNum, uint32_t *ui32Step, uint32_t *index)
{
    int retVal;

    retVal = 0;
    switch(ui32SequenceNum){
    case SEQ_SEQUENCE1:
        if (*ui32Step < SEQ_SEQUENCE1_SIZE){
            *index = g_SEQSequence1Table[*ui32Step].SEQElement;
        }
        break;
    case SEQ_SEQUENCE2:
        if (*ui32Step < SEQ_SEQUENCE2_SIZE){
            *index = g_SEQSequence2Table[*ui32Step].SEQElement;
        }
        break;
    case SEQ_SEQUENCE3:
        if (*ui32Step < SEQ_SEQUENCE3_SIZE){
            *index = g_SEQSequence3Table[*ui32Step].SEQElement;
        }
        break;
    case SEQ_SEQUENCE4:
        if (*ui32Step < SEQ_SEQUENCE4_SIZE){
            *index = g_SEQSequence4Table[*ui32Step].SEQElement;
        }
        break;
    default:
        retVal = -1;
        break;
    }
    if (*index & SEQ_CTL_END) {
        retVal = 1;
    }
    *index &= 0x0FFFFFFF;

    return retVal;
}

int SEQGetValueUpdateStep(uint32_t ui32SequenceNum, uint32_t *ui32Step, uint32_t *index)
{
    int retVal;
    bool isEnd;

    retVal = 0;
    isEnd = false;
    switch(ui32SequenceNum){
    case SEQ_SEQUENCE1:
        if (*ui32Step < SEQ_SEQUENCE1_SIZE){
            *index = g_SEQSequence1Table[*ui32Step].SEQElement;
            (*ui32Step)++;
            if ((*ui32Step >= SEQ_SEQUENCE1_SIZE) || (*index & SEQ_CTL_END)){
                isEnd = true;
            }
        }else {
            *ui32Step = 0;
        }
        break;
    case SEQ_SEQUENCE2:
        if (*ui32Step < SEQ_SEQUENCE2_SIZE){
            *index = g_SEQSequence2Table[*ui32Step].SEQElement;
            (*ui32Step)++;
            if ((*ui32Step >= SEQ_SEQUENCE1_SIZE) || (*index & SEQ_CTL_END)){
                isEnd = true;
            }
        }else {
            *ui32Step = 0;
        }
        break;
    case SEQ_SEQUENCE3:
        if (*ui32Step < SEQ_SEQUENCE3_SIZE){
            *index = g_SEQSequence3Table[*ui32Step].SEQElement;
            (*ui32Step)++;
            if ((*ui32Step >= SEQ_SEQUENCE1_SIZE) || (*index & SEQ_CTL_END)){
                isEnd = true;
            }
        }else {
            *ui32Step = 0;
        }
        break;
    case SEQ_SEQUENCE4:
        if (*ui32Step < SEQ_SEQUENCE4_SIZE){
            *index = g_SEQSequence4Table[*ui32Step].SEQElement;
            (*ui32Step)++;
            if ((*ui32Step >= SEQ_SEQUENCE1_SIZE) || (*index & SEQ_CTL_END)){
                isEnd = true;
            }
        }else {
            *ui32Step = 0;
        }
        break;
    default:
        break;
    }
    if (isEnd) {
        retVal = 1;
        *ui32Step = 0;
    }
    *index &= 0x0FFFFFFF;

    return retVal;
}
