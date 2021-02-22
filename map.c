#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "Decode.h"

//==============================================================================
// PONTEIROS:

//------------------------------------------------------------------------------
// Camada 0
void (* pCamarda1[])(int number,FILE *fileX) =
{                              
  DecodeLSL_LSR_LD_LM_IMM5,
  fgrupo1,
  DecodeMOV_CMP_LD_IMM8,
  DecodeADD_SUB_LD_IMM8,
  fgrupo4,
  fgrupo5,
  DecodeSTR_LDR_LD_LN_IMM5,
  DecodeSTRB_LDRB_LD_LN_IMM5,
  DecodeSTRH_LDRH_LD_LN_IMM5,
  DecodeSTR_LDR_SP_IMM8,
  DecodeADDLdpc_ADDLdsp,
  fgrupo11,
  DecodeSTMIA_LDMIA,
  fgrupo13,
  fgrupo14,
  DecodeBL_OFFSET
};

//-------------------------------------------------------------------------------
// Bits [11,10] das instruções com os bits superiores [15-12] sendo igual a 0001
void (* pDecodeGrupo0001[])(int number,FILE *fileX) =
{                              
  DecodeASR_LD_LM_IMM5,   // Repete que o bit 10 pode ser 0 ou 1, logo os bits [11,10] podendo ser 00 ou 01
  DecodeASR_LD_LM_IMM5,
  DecodeADD_SUB_LD_LN_LM,
  DecodeADD_SUB_LD_LN_IMM3,
};


//-------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 0100
void (* pDecodeGrupo0100[])(int number,FILE *fileX) =
{                              
  fsubGrupo4_1,
  fsubGrupo4_2
};

// Bits [10-8] das instruções com os bits superiores [15-12] sendo igual a 0100
void (* pDecodeSubGrupo4_1[])(int number,FILE *fileX) =
{                              
  DecodeAND_EOR_LSL_LSR_LD_LM,
  DecodeASR_ADC_SBC_ROR_LD_LM_LS,
  DecodeTST_NEG_CMP_CMN_LD_LN_LM,
  DecodeORR_MUL_BIC_MVN_LD_LM
};

// Bits [8-6] das instruções com os bits superiores [15-12] sendo igual a 0100
void (* pDecodeSubGrupo4_2[])(int number,FILE *fileX) =
{                              
  DecodeCPY_LD_LM,
  DecodeADD_MOV_LD_HM,
  DecodeADD_MOV_HD_LM,
  DecodeADD_MOV_HD_HM,
  underfinedGrup04,                                                             // bits : 0100 0101 00 [bits 5-0 sendo qualquer coisa]
  DecodeCMP_LN_HM,
  DecodeCMP_HN_LM,
  DecodeCMP_HN_HM
};

//-------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 0101
void (* pDecodeGrupo0101[])(int number,FILE *fileX) =
{                              
  DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM,
  DecodeLDR_LDRH_LDRB_LDRSH_LD_LN_LM
};

//-------------------------------------------------------------------------------
// Bits [11-9] . Nas instruções com os bits superiores [15-12] sendo igual a 1011
void (* pDecodeGrupo1011[])(int number,FILE *fileX) =
{                              
  DecodeADDsp_SUBsp,
  DecodeSXTH_SXTB_UXTH_UXTB,
  DecodePUSH_POP,
  fsubGrupo11,
  DecodePUSH_POP,
  DecodeREV_REV16_REVSH,
  DecodePUSH_POP,
  DecodeBKPT_IMM8
};

// Bit [5] das instruções com os bits superiores [15-12] sendo igual a 1011 e bits [11-9] sendo igual a 011
void (* pDecodeSubGrupo11[])(int number,FILE *fileX) =
{                              
  DecodeSETENDLE_SETENDBE,
  DecodeCPSIE_CPSID
};

//-------------------------------------------------------------------------------
// Bit [8] das instruções com os bits superiores [15-12] sendo igual a 1101
void (* pDecodeGrupo1101[])(int number,FILE *fileX) =
{                              
  DecodeUndefined,
  DecodeSWI_IMM8
};

//-------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 1110
void (* pDecodeGrupo1110[])(int number,FILE *fileX) =
{                              
  DecodeB_OFFSET,
  DecodeBLX_OFFSET
};

//-------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 1111
void (* pDecodeGrupo1111[])(int number,FILE *fileX) =
{                              
  // função que falta
  DecodeBL_OFFSET
};


//==============================================================================
// FUNÇÕES:

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0001
void fgrupo1(int number,FILE *fileX){
  int indice = (number & 0xC00) >> 0xA;                                          // Pegando bits [11,10]
  (* pDecodeGrupo0001[indice])(number,fileX);
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0100
void fgrupo4(int number,FILE *fileX){
  if(((number & 0x800) >> 0xB) == 1)                                            // se bit [11] é 1, então só pode ser uma função
    DecodeLDR_LD_PC_IMM8X4(number,fileX);
  else{
    int indice = (number & 0x400) >> 0xA;                                        // Pegando bit [10]
    (* pDecodeGrupo0100[indice])(number,fileX);
  }
}

// Instruções com os bits superiores [15-12] sendo igual a 0100 e bit 10 sendo 0
void fsubGrupo4_1(int number,FILE *fileX){
  int indice = (number & 0x700) >> 0x8;                                          // Pegando bits [10-8]
  (* pDecodeSubGrupo4_1[indice])(number,fileX);  
}

// Instruções com os bits superiores [15-12] sendo igual a 0100 e bit 10 sendo 1
void fsubGrupo4_2(int number,FILE *fileX){
  if(((number & 0x700) >> 0x8) == 0x7)
    DecodeBX_BLX_RM(number,fileX);
  else{
    int indice = (number & 0x1C0) >> 0x6;                                        // Pegando bits [8-6]
    (* pDecodeSubGrupo4_2[indice])(number,fileX);  
  }
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0101
void fgrupo5(int number,FILE *fileX){
  int indice = (number & 0x800) >> 0xB;                                          // Pegando bit [11]
  (* pDecodeGrupo0101[indice])(number,fileX);
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1011
void fgrupo11(int number,FILE *fileX){
  int indice = (number & 0xE00) >> 0x9;                                          // Pegando bits [11,10]
  (* pDecodeGrupo1011[indice])(number,fileX);
}

// Instruções com os bits superiores [15-12] sendo igual a 1011 e bit 5 sendo 0 ou 1
void fsubGrupo11(int number,FILE *fileX){
  int indice = (number & 0x20) >> 0x5;                                          // Pegando bits [10-8]
  (* pDecodeSubGrupo11[indice])(number,fileX);  
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1101
void fgrupo13(int number,FILE *fileX){
  if(((number & (0xF00)) >> 0X8) < 0xE)
    DecodeBcond_OFFSET(number, fileX);                                          // cond < 1110
  else{
    int indice = (number & 0x100) >> 0x8;                                       // Pegando bit [8]
    (* pDecodeGrupo1101[indice])(number,fileX);
  }
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1110
void fgrupo14(int number,FILE *fileX){
  int indice = (number & 0x800) >> 0xB;                                          // Pegando bits [11]
  (* pDecodeGrupo1110[indice])(number,fileX);
}

//-------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1111
void fgrupo15(int number,FILE *fileX){
  int indice = (number & 0x800) >> 0xB;                                          // Pegando bits [11]
  (* pDecodeGrupo1111[indice])(number,fileX);
}


//==============================================================================
// FUNÇÃO MAP:

void mapFunction(int number,FILE *fileX){
  int higher4Bits = (number & (0xF << 0xC)) >> 0xC;

  (* pCamarda1[higher4Bits])(number,fileX);
}
