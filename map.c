#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "Decode.h"

int flagPoff = 0;                                                                                                       // Diz se poff foi alterado na instrução anterior, com 0 para não e 1 para sim
int indice = 0;
int test = 0;
int higher4Bits;

//============================================================================================================================================================
// PONTEIROS:

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bits[15-12]
void (* pCamarda1[])(int number,FILE *fileX) =
{                              
  DecodeLSL_LSR_LD_LM_IMM5,                                                                                             // Bits[15-12] == 0000
  fgrup1,                                                                                                              // Bits[15-12] == 0001                                                        
  DecodeMOV_CMP_LD_IMM8,                                                                                                // Bits[15-12] == 0010
  DecodeADD_SUB_LD_IMM8,                                                                                                // Bits[15-12] == 0011
  fgrup4,                                                                                                              // Bits[15-12] == 0100
  fgrup5,                                                                                                              // Bits[15-12] == 0101
  DecodeSTR_LDR_LD_LN_IMM5,                                                                                             // Bits[15-12] == 0110
  DecodeSTRB_LDRB_LD_LN_IMM5,                                                                                           // Bits[15-12] == 0111
  DecodeSTRH_LDRH_LD_LN_IMM5,                                                                                           // Bits[15-12] == 1000
  DecodeSTR_LDR_SP_IMM8,                                                                                                // Bits[15-12] == 1001
  DecodeADDLdpc_ADDLdsp,                                                                                                // Bits[15-12] == 1010
  fgrup11,                                                                                                             // Bits[15-12] == 1011
  DecodeSTMIA_LDMIA,                                                                                                    // Bits[15-12] == 1100
  fgrup13,                                                                                                             // Bits[15-12] == 1101
  fgrup14,                                                                                                             // Bits[15-12] == 1110
  DecodeBL_OFFSET                                                                                                       // Bits[15-12] == 1111
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bits [11,10] das instruções com os bits [15-12] sendo igual a 0001
void (* pDecodegrup0001[])(int number,FILE *fileX) =
{                              
  DecodeASR_LD_LM_IMM5,                                                                                                 // bits [11,10] sendo 00
  DecodeASR_LD_LM_IMM5,                                                                                                 // bits [11,10] sendo 00
  DecodeADD_SUB_LD_LN_LM,                                                                                               // bits [11,10] sendo 10
  DecodeADD_SUB_LD_LN_IMM3,                                                                                             // bits [11,10] sendo 11
};


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bit [10] das instruções com os bits [15-12] sendo igual a 0100
void (* pDecodegrup0100[])(int number,FILE *fileX) =
{                              
  fsubgrup4_1,                                                                                                         // bit [10] sendo 0
  fsubgrup4_2                                                                                                          // bit [10] sendo 1
};

// Bits [9,8] das instruções com os bits [15-12] sendo igual a 0100
void (* pDecodeSubgrup4_1[])(int number,FILE *fileX) =
{                              
  DecodeAND_EOR_LSL_LSR_LD_LM,                                                                                          // bits [9,8] sendo 00
  DecodeASR_ADC_SBC_ROR_LD_LM_LS,                                                                                       // bits [9,8] sendo 00
  DecodeTST_NEG_CMP_CMN_LD_LN_LM,                                                                                       // bits [9,8] sendo 10
  DecodeORR_MUL_BIC_MVN_LD_LM                                                                                           // bits [9,8] sendo 11
};

// Bits [8-6] das instruções com os bits superiores [15-12] sendo igual a 0100
void (* pDecodeSubgrup4_2[])(int number,FILE *fileX) =
{ 
  DecodeCPY_LD_LM,                                                                                                      // Bits [8-6] sendo 000
  DecodeADD_MOV_LD_HM,                                                                                                  // Bits [8-6] sendo 001
  DecodeADD_MOV_HD_LM,                                                                                                  // Bits [8-6] sendo 010
  DecodeADD_MOV_HD_HM,                                                                                                  // Bits [8-6] sendo 011
  underfined,                                                                                                           // Bits [8-6] sendo 100
  DecodeCMP_LN_HM,                                                                                                      // Bits [8-6] sendo 101
  DecodeCMP_HN_LM,                                                                                                      // Bits [8-6] sendo 110
  DecodeCMP_HN_HM,                                                                                                      // Bits [8-6] sendo 111
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 0101
void (* pDecodegrup0101[])(int number,FILE *fileX) =
{                              
  DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM,                                                                                   // bit [11] sendo 0
  DecodeLDR_LDRH_LDRB_LDRSH_LD_LN_LM                                                                                    // bit [11] sendo 1
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bits [11-8]. Nas instruções com os bits superiores [15-12] sendo igual a 1011
void (* pDecodegrup1011[])(int number,FILE *fileX) =
{                              
  DecodeADDsp_SUBsp,                                                                                                    // Bits[11-8] sendo 0000
  underfined,                                                                                                           // Bits[11-8] sendo 0001
  DecodeSXTH_SXTB_UXTH_UXTB,                                                                                            // Bits[11-8] sendo 0010
  underfined,                                                                                                           // Bits[11-8] sendo 0011
  DecodePUSH_POP,                                                                                                       // Bits[11-8] sendo 0100
  DecodePUSH_POP,                                                                                                       // Bits[11-8] sendo 0101
  fsubgrup11,                                                                                                           // Bits[11-8] sendo 0110
  underfined,                                                                                                           // Bits[11-8] sendo 0111
  underfined,                                                                                                           // Bits[11-8] sendo 1000
  underfined,                                                                                                           // Bits[11-8] sendo 1001
  DecodeREV_REV16_REVSH,                                                                                                // Bits[11-8] sendo 1010
  underfined,                                                                                                           // Bits[11-8] sendo 1011
  DecodePUSH_POP,                                                                                                       // Bits[11-8] sendo 1100
  DecodePUSH_POP,                                                                                                       // Bits[11-8] sendo 1101
  DecodeBKPT_IMM8,                                                                                                      // Bits[11-8] sendo 1110
  underfined                                                                                                            // Bits[11-8] sendo 1111
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bit [8] das instruções com os bits superiores [15-12] sendo igual a 1101
void (* pDecodegrup1101[])(int number,FILE *fileX) =
{                              
  DecodeUndefined,                                                                                                      // bit [8] sendo 0
  DecodeSWI_IMM8                                                                                                        // bit [8] sendo 0
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Bit [11] das instruções com os bits superiores [15-12] sendo igual a 1110
void (* pDecodegrup1110[])(int number,FILE *fileX) =
{                              
  DecodeB_OFFSET,                                                                                                       // bit [11] sendo 0
  DecodeBLX_OFFSET                                                                                                      // bit [11] sendo 1
};


//============================================================================================================================================================
// FUNÇÕES:

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0001
void fgrup1(int number,FILE *fileX){
  indice = (number & 0xC00) >> 0xA;                                                                                 // Pegando bits [11,10]
  (* pDecodegrup0001[indice])(number,fileX);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0100
void fgrup4(int number,FILE *fileX){
  if(((number & 0x800) >> 0xB) == 1)                                                                                    // Se bit [11] é 1, então só pode ser uma função
    DecodeLDR_LD_PC_IMM8X4(number,fileX);
  else{
    indice = (number & 0x400) >> 0xA;                                                                               // Pegando bit [10] como índice
    (* pDecodegrup0100[indice])(number,fileX);
  }
}

// Instruções com os bits superiores [15-12] sendo igual a 0100 e bit 10 sendo 0
void fsubgrup4_1(int number,FILE *fileX){
  indice = (number & 0x300) >> 0x8;                                                                                 // Pegando bits [9-8] como índice
  (* pDecodeSubgrup4_1[indice])(number,fileX);  
}

// Instruções com os bits superiores [15-12] sendo igual a 0100 e bit 10 sendo 1
void fsubgrup4_2(int number,FILE *fileX){
  test = (number & 0x3C0) >> 0x6;

  if(test == 0x0 || test == 0x4)
    underfined(number,fileX);
  else if(((number & 0x3C0) >> 0x6) >= 0xC)                                                                             // Se os bits [9-6] são maiores ou iguais a 1100    
    DecodeBX_BLX_RM(number,fileX);                                          
  else{
    indice = (number & 0x3C0) >> 0x6;                                                                               // Pegando bits [9-6] como índice
    (* pDecodeSubgrup4_2[indice])(number,fileX);  
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 0101
void fgrup5(int number,FILE *fileX){
  indice = (number & 0x800) >> 0xB;                                                                                 // Pegando bit [11] como índice
  (* pDecodegrup0101[indice])(number,fileX);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1011
void fgrup11(int number,FILE *fileX){
  indice = (number & 0xF00) >> 0x8;                                                                                 // Pegando bits [8-11] como índice
  (* pDecodegrup1011[indice])(number,fileX);
}

// Instruções com os bits superiores [15-12] sendo igual a 1011, bits [8-11] sendo igual a 0110
void fsubgrup11(int number,FILE *fileX){
  if((((number & 0xF0) >> 0x4) == 0x5) && (first3Bits(number) == 0x0)){                                                 // number com bits [4-7] sendo 0101 e bits [0-2] sendo 000 
    DecodeSETENDLE_SETENDBE(number,fileX);
  }
  else if((((number & 0xE0) >> 0x5) == 0x3) && (((number & (mask1 << 0x3)) >> 0x3) == 0x0)){                            // number com bits [5-7] sendo 011 e bit [3] sendo 0
    DecodeCPSIE_CPSID(number,fileX);
  }
  else
    underfined(number,fileX);                                                                                           // Não há uma função    
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1101
void fgrup13(int number,FILE *fileX){
  if(((number & (0xF00)) >> 0X8) < 0xE)
    DecodeBcond_OFFSET(number, fileX);                                                                                  // cond < 1110
  else{
    indice = (number & 0x100) >> 0x8;                                                                               // Pegando bit [8] como índice
    (* pDecodegrup1101[indice])(number,fileX);
  }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// Instruções com os bits superiores [15-12] sendo igual a 1110
void fgrup14(int number,FILE *fileX){
  indice = (number & 0x800) >> 0xB;                                                                                 // Pegando bits [11] como índice
  if((indice == 1) && ((number & mask1) == 1))                                                                          // Se bit [11] for 1 e bit [0] de number for 1,então não há uma função
    underfined(number,fileX);
  else
    (* pDecodegrup1110[indice])(number,fileX);
}

int Poff(){
  return flagPoff;
}

//============================================================================================================================================================
// FUNÇÃO MAP:

void mapFunction(int number,FILE *fileX){
  test = (number & 0xF800) >> 0xB;                                                                                  // Teste para instrução que altera o valor de poff

  if(flagPoff == 0 && test == 0x1E){                                                                                    // poff no seu estado original e com a instrução que altera poff
    fPoff(number,fileX,0);                                                                                              // novo valor de poff
    flagPoff = 1;                                                                                                       
  }
  else if(flagPoff == 1 && test == 0x1E){                                                                               // poff alterado na instrução anterior, mas instrução atual é outra que altera poff
    fPoff(number,fileX,1);                                                                                              // Error para instrução que alterou o poff antes
    fPoff(number,fileX,0);                                                                                              // novo valor de poff
  }
  else if((flagPoff == 1 && test != 0x1D) && (flagPoff == 1 && test != 0x1F)){                                          // poff alterado na instrução anterior, mas instrução atual não é BLX ou BL que usam valor de poff
    fPoff(number,fileX,1);                                                                                              // Error para instrução que alterou o poff antes
    fPoff(0,fileX,0);                                                                                                   // poff em seu estado original
    flagPoff = 0;                                                                                                       
    
    higher4Bits = (number & (0xF << 0xC)) >> 0xC;                                                                   // mapear instrução atual
    (* pCamarda1[higher4Bits])(number,fileX);
  }
  else if((flagPoff == 1 && test == 0x1D) || (flagPoff == 1 && test == 0x1F)){                                          // poff alterado na instrução anterior e instrução atual é BLX ou BL que usam valor de poff
    fPoff(number,fileX,2);                                                                                              // poff sem Error
    flagPoff = 0;                                                                                                       
    
    higher4Bits = (number & (0xF << 0xC)) >> 0xC;                                                                   // mapear instrução atual
    (* pCamarda1[higher4Bits])(number,fileX);

    fPoff(0,fileX,0);                                                                                                   // poff estado original
  }
  else{                                                                                                                 //poff no seu estado original e instrução atual não altera poff
    higher4Bits = (number & (0xF << 0xC)) >> 0xC;
    (* pCamarda1[higher4Bits])(number,fileX);
  }
}
