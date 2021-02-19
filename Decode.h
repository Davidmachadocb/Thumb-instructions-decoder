#ifndef DECODE_H_INCLUDED
#define DECODE_H_INCLUDED

#define mask7 0x7
#define mask1 0x1

#define immed8(number) ((number) & 0xFF)
#define immed5(number) ((number) & 0x7C0)

#define first3Bits(number) ((number) & 0x7)
#define second3Bits(number) ((number) & 0x38)
#define third3Bits(number) ((number) & 0x1C0)

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

void DecodeLDR_LD_PC_IMM8X4(int number, FILE *fileX);

void DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM(int number, FILE *fileX);

void DecodeLDR_LDRH_LDRB_LDRSH_LD_LN_LM(int number, FILE *fileX);

void DecodeSTR_LDR_LD_LN_IMM5(int number, FILE *fileX);

void DecodeSTRB_LDRB_LD_LN_IMM5(int number, FILE *fileX);

void decodeSTRH_LDRH_LD_LN_IMM5(int number, FILE *fileX);

void DecodeSTR_LDR_SP_IMM8(int number, FILE *fileX);

void DecodeBX_BLX_RM(int number, FILE *fileX);

#endif // DECODE_H_INCLUDED
