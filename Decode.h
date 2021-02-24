#include<stdio.h>

#ifndef DECODE_H_INCLUDED
#define DECODE_H_INCLUDED

#define maskF 0xF
#define mask7 0x7
#define mask3 0x3
#define mask1 0x1

#define immed8(number) ((number) & 0xFF)                                        // bits [0-8] de number
#define immed5(number) (((number) & 0x7C0) >> 0x6)                              // bits [6-10] de number
#define immed7(number) ((number) & 0x007F)                                      // bits [0-6] de number
#define immed11(number) ((number) & 0x7ff)                                      // bits [0-10] de number

#define first3Bits(number) ((number) & 0x7)                                     // bits [0-2] de number
#define second3Bits(number) (((number) & 0x38) >> 0x3)                          // bits [3-5] de number
#define third3Bits(number) (((number) & 0x1C0) >> 0x6)                          // bits [6-8]

void highregister(int Hm, FILE *fileX);

void DecodeLSL_LSR_LD_LM_IMM5(int number, FILE *fileX);

void DecodeASR_LD_LM_IMM5(int number, FILE *fileX);

void DecodeADD_SUB_LD_LN_LM(int number, FILE *fileX);

void DecodeADD_SUB_LD_LN_IMM3(int number, FILE *fileX);

void DecodeMOV_CMP_LD_IMM8(int number, FILE *fileX);

void DecodeADD_SUB_LD_IMM8(int number, FILE *fileX);

void DecodeAND_EOR_LSL_LSR_LD_LM(int number, FILE *fileX);

void DecodeASR_ADC_SBC_ROR_LD_LM_LS(int number, FILE *fileX);

void DecodeTST_NEG_CMP_CMN_LD_LN_LM(int number, FILE *fileX);

void DecodeORR_MUL_BIC_MVN_LD_LM(int number, FILE *fileX);

void DecodeCPY_LD_LM(int number, FILE *fileX);

void DecodeADD_MOV_LD_HM(int number, FILE *fileX);

void DecodeADD_MOV_HD_LM(int number, FILE *fileX);

void DecodeADD_MOV_HD_HM(int number, FILE *fileX);

void DecodeCMP_LN_HM(int number, FILE *fileX);

void DecodeCMP_HN_LM(int number, FILE *fileX);

void DecodeCMP_HN_HM(int number, FILE *fileX);

void DecodeBX_BLX_RM(int number, FILE *fileX);

void DecodeLDR_LD_PC_IMM8X4(int number, FILE *fileX);

void DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM(int number, FILE *fileX);

void DecodeLDR_LDRH_LDRB_LDRSH_LD_LN_LM(int number, FILE *fileX);

void DecodeSTR_LDR_LD_LN_IMM5(int number, FILE *fileX);

void DecodeSTRB_LDRB_LD_LN_IMM5(int number, FILE *fileX);

void DecodeSTRH_LDRH_LD_LN_IMM5(int number, FILE *fileX);

void DecodeSTR_LDR_SP_IMM8(int number, FILE *fileX);

void DecodeADDLdpc_ADDLdsp(int number, FILE *fileX);

void DecodeADDsp_SUBsp(int number, FILE *fileX);

void DecodeSXTH_SXTB_UXTH_UXTB(int number, FILE *fileX);

void DecodeREV_REV16_REVSH(int number, FILE *fileX);

void DecodePUSH_POP(int number, FILE *fileX);

void DecodeSETENDLE_SETENDBE(int number, FILE *fileX);

void DecodeCPSIE_CPSID(int number, FILE *fileX);

void DecodeBKPT_IMM8(int number, FILE *fileX);

void DecodeSTMIA_LDMIA(int number, FILE *fileX);

void DecodeBcond_OFFSET(int number, FILE *fileX);

void DecodeUndefined(int number, FILE *fileX);

void DecodeSWI_IMM8(int number, FILE *fileX);

void DecodeB_OFFSET(int number, FILE *fileX);

void DecodeBLX_OFFSET(int number, FILE *fileX);

void DecodeBL_OFFSET(int number, FILE *fileX);

// obs
void underfinedGrup04(int number,FILE *fileX);

void fPoff(int number,FILE *fileX,int flagPoff);
#endif // DECODE_H_INCLUDED
