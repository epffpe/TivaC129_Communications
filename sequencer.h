/*
 * secuencer.h
 *
 *  Created on: Jan 13, 2017
 *      Author: epffpe
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#ifdef SEQUENCER_GLOBALS
#define SEQ_EXT
#else
#define SEQ_EXT extern
#endif

#define SEQ_SEQUENCE1               0
#define SEQ_SEQUENCE2               1
#define SEQ_SEQUENCE3               2
#define SEQ_SEQUENCE4               3

#define SEQ_SEQUENCE1_SIZE          32
#define SEQ_SEQUENCE2_SIZE          16
#define SEQ_SEQUENCE3_SIZE          8
#define SEQ_SEQUENCE4_SIZE          8

#define SEQ_CTL_END                 0x80000000


typedef struct seq_param {
    uint32_t    SEQElement;
    uint32_t    SEQDly;
}SEQ_PARAM;


#ifdef __cplusplus
extern "C"
{
#endif



void SEQInit(void);
void SEQSequenceStepConfigure(uint32_t ui32SequenceNum, uint32_t ui32Step, uint32_t ui32Config);
int SEQGetValue(uint32_t ui32SequenceNum, uint32_t *ui32Step, uint32_t *index);
int SEQGetValueUpdateStep(uint32_t ui32SequenceNum, uint32_t *ui32Step, uint32_t *index);


#ifdef __cplusplus
}
#endif



#endif /* SEQUENCER_H_ */
