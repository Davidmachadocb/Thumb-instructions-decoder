#include <stdio.h>
#include <stdlib.h>
#include "Decode.h"

int poff = 0;                                                                   // poff em seu estado original
int op = 0;

void highregister(int H, FILE *fileX){                                          // Imprime como deve ser chamado cada high-register (r8-r15) 
    if(H == 13)
        fprintf(fileX,"sp");
    else if(H == 14)
        fprintf(fileX,"lr");
    else if(H == 15)
        fprintf(fileX,"pc");
    else
        fprintf(fileX,"r%d",H);
}

// < LSL | LSR > Ld, Lm, #<immed5>
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
// immed5 = immed5(number). Logo os bits [6,7,8,9,10]
void DecodeLSL_LSR_LD_LM_IMM5(int number, FILE *fileX){
    op = (number & (mask1 << 0xB)) >> 0xB; // pega o Bit [11] para escolher a instrução

    if(op == 0)
        fprintf(fileX,"0%x\t LSL r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
    else
        fprintf(fileX,"0%x\t LSR r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
}

// < ASR > Ld, Lm, #<immed5>
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
// immed5 = immed5(number). Logo os bits [6,7,8,9,10]
void DecodeASR_LD_LM_IMM5(int number, FILE *fileX){
    fprintf(fileX,"%x\t ASR r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
}

// < ADD | SUB > Ld, Ln, Lm
void DecodeADD_SUB_LD_LN_LM(int number, FILE *fileX){
    op = (number & (mask1 << 0x9)) >> 0x9;                                  // pega o Bit [9] para escolher a instrução

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d, r%d \n",number,first3Bits(number),second3Bits(number),third3Bits(number));
    else
        fprintf(fileX,"%x\t SUB r%d, r%d, r%d \n",number,first3Bits(number),second3Bits(number),third3Bits(number));
}

// < ADD | SUB > com Ld, Ln, #<immed3>
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Ln = second3Bits(number). Logo os bits [3,4,5]
// immed3 será os bits [6,7,8]
void DecodeADD_SUB_LD_LN_IMM3(int number, FILE *fileX){
    int Immed3 = (((mask7 << 0x6) & number) >> 0x6);                            // pega os bits 6-8 de number, define um imediato de 3 bits
    op = (number & (mask1 << 0x9)) >> 0x9;                                  // pega o Bit [9] para escolher a instrução

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),Immed3);
    else
        fprintf(fileX,"%x\t SUB r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),Immed3);
}

// < MOV | CMP > Ld, #<immed8>
// Ld_Ln será os bits [8,9,10]
// immed8 = immed8(number). Logo os bits [0,1,2,3,4,5,6,7]
void DecodeMOV_CMP_LD_IMM8(int number, FILE *fileX){
    int Ld_Ln = (number & (mask7 << 0x8)) >> 0x8;                               // pega os bits 8-10 de number, define registrador Ld_Ln
    op = (number & (mask1 << 0xB)) >> 0xB;                                  // pega o Bit [11] para escolher a instrução

    if(op == 0)
        fprintf(fileX,"%x\t MOV r%d, #%d \n",number,Ld_Ln,immed8(number));
    else
        fprintf(fileX,"%x\t CMP r%d, #%d \n",number,Ld_Ln,immed8(number));
}

// < ADD | SUB > Ld, #<immed8>
// Ld será os bits [8,9,10]
// immed8 = immed8(number). Logo os bits [0,1,2,3,4,5,6,7]
void DecodeADD_SUB_LD_IMM8(int number, FILE *fileX){
    int Ld = (number & (mask7 << 0x8)) >> 0x8;                                  // pega os bits 8-10 de number, define registrador Ld
    op = (number & (mask1 << 0xB)) >> 0xB;                                  // pega o Bit [11] para escolher a instrução

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, #%d \n",number,Ld,immed8(number));
    else
        fprintf(fileX,"%x\t SUB r%d, #%d \n",number,Ld,immed8(number));
}

// < AND | EOR | LSL | LSR >  Ld, Lm
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeAND_EOR_LSL_LSR_LD_LM(int number, FILE *fileX){
    op = (number & 0xC0) >> 6;                                              // pega os Bits 6 e 7 para escolher uma das 4 instruções

    if(op == 0)
        fprintf(fileX,"%x\t AND r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 1)
        fprintf(fileX,"%x\t EOR r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 2)
        fprintf(fileX,"%x\t LSL r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else
        fprintf(fileX,"%x\t LSR r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < ASR | ADC | SBC | ROR > Ld, Lm/Ls
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm/Ls = second3Bits(number). Logo os bits [3,4,5]
void DecodeASR_ADC_SBC_ROR_LD_LM_LS(int number, FILE *fileX){
    op = (number & 0xC0) >> 6;                                              // pega os Bits 6 e 7 para escolher uma das 4 instruções

    if(op == 0)
        fprintf(fileX,"%x\t ASR r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 1)
        fprintf(fileX,"%x\t ADC r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 2)
        fprintf(fileX,"%x\t SBC r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else
        fprintf(fileX,"%x\t ROR r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < TST | NEG | CMP | CMN > Ld/Ln, Lm
// Ld/Ln = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeTST_NEG_CMP_CMN_LD_LN_LM(int number, FILE *fileX){
    op = (number & 0xC0) >> 6;                                              // pega os Bits 6 e 7 para escolher uma das 4 instruções

    if(op == 0)
        fprintf(fileX,"%x\t TST r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 1)
        fprintf(fileX,"%x\t NEG r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 2)
        fprintf(fileX,"%x\t CMP r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else
        fprintf(fileX,"%x\t CMN r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < ORR | MUL | BIC | MVN >  Ld, Lm
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeORR_MUL_BIC_MVN_LD_LM(int number, FILE *fileX){
    op = (number & 0xC0) >> 6;                                              // pega os Bits 6 e 7 para escolher uma das 4 instruções

    if(op == 0)
        fprintf(fileX,"%x\t ORR r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 1)
        fprintf(fileX,"%x\t MUL r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else if(op == 2)
        fprintf(fileX,"%x\t BIC r%d, r%d \n",number,first3Bits(number),second3Bits(number));
    else
        fprintf(fileX,"%x\t MVN r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < CPY > Ld, Lm
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeCPY_LD_LM(int number, FILE *fileX){
    fprintf(fileX,"%x\t CPY r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < ADD | MOV > Ld, Hm
// Ld = first3Bits(number). Logo os bits [0,1,2]
// Hm = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [3,4,5] de number
void DecodeADD_MOV_LD_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    op = (number & (mask1 << 0x9)) >> 0x9;                                  // pega o bit 9 para escolher uma das 2 instruções

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d,",number,first3Bits(number));
    else
        fprintf(fileX,"%x\t MOV r%d,",number,first3Bits(number));
    highregister(Hm,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,"\n");
}

// < ADD | MOV > Hd, Lm
// < ADD | MOV > Ld, Hm
// Hd = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [0,1,2] de number 
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeADD_MOV_HD_LM(int number, FILE *fileX){
    int Hd = first3Bits(number) | 0x8;                                          // Transformando em um high-register
    op = (number & (mask1 << 0x9)) >> 0x9;                                  // pega o bit 9 para escolher uma das 2 instruções

    if(op == 0)
        fprintf(fileX,"%x\t ADD ",number);
    else
        fprintf(fileX,"%x\t MOV ",number);
    highregister(Hd,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,", r%d\n", second3Bits(number));                              // Imprimindo Lm
}

// < ADD | MOV > Hd, Hm
// Hd = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [0,1,2] de number
// Hm = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [3,4,5] de number
void DecodeADD_MOV_HD_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    int Hd = first3Bits(number) | 0x8;                                          // Transformando em um high-register
    op = (number & (mask1 << 0x9)) >> 0x9;                                  // pega o bit 9 para escolher uma das 2 instruções

    if(op == 0)
        fprintf(fileX,"%x\t ADD ",number);                    
    else
        fprintf(fileX,"%x\t MOV ",number);                     
    
    highregister(Hd,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,", ");
    highregister(Hm,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,"\n");                     

}

// < CMP > Ln, Hm
// Ln = first3Bits(number). Logo os bits [0,1,2]
// Hm = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [3,4,5] de number
void DecodeCMP_LN_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register

    fprintf(fileX,"%x\t CMP r%d, ",number,first3Bits(number));
    highregister(Hm,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,"\n");
}

// < CMP > Hn, Lm
// Hn = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [0,1,2] de number 
// Lm = second3Bits(number). Logo os bits [3,4,5]
void DecodeCMP_HN_LM(int number, FILE *fileX){
    int Hn = first3Bits(number) | 0x8;                                          // Transformando em um high-register

    fprintf(fileX,"%x\t CMP ",number);
    highregister(Hn,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,", r%d\n", second3Bits(number));         
}

// < CMP > Hn, Hm
// Hn = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [0,1,2] de number
// Hm = bit 3 sendo 1 e bits 0,1 e 2 sendo formado pelos bits [3,4,5] de number
void DecodeCMP_HN_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    int Hn = first3Bits(number) | 0x8;                                          // Transformando em um high-register

    fprintf(fileX,"%x\t CMP ",number);                        
    
    highregister(Hn,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,", ");                                                        
    highregister(Hm,fileX);                                                     // Imprime na forma correta de chamar o registrador
    fprintf(fileX,"\n");                        

}

//BX_BLX Rm
void DecodeBX_BLX_RM(int number, FILE *fileX){
    int maskR = 0xf;

    op = ((number & (mask1 << 7)) >> 7);               //pegar o bit 7 de number, define se será BX OU BLX
    int Rm = ((number & (maskR << 3)) >> 3);               //pegar os bits 3-6 de number, define o Rm

    if(op == 0 ){
        if(Rm == 13){                                      //se Rm for maior igual a 13, Rm será chamado de sp, lr ou pc respectivamente
            fprintf(fileX,"%x\t BX sp\n", number);
        }else if(Rm == 14){
            fprintf(fileX,"%x\t BX lr\n", number);
        }else if(Rm == 15){
            fprintf(fileX,"%x\t BX pc\n", number);
        }else{
            fprintf(fileX,"%x\t BX r%d\n", number, Rm);
        }
    }else{
        if(Rm == 13){
            fprintf(fileX,"%x\t BLX sp\n", number);
        }else if(Rm == 14){
            fprintf(fileX,"%x\t BLX lr\n", number);
        }else if(Rm == 15){
            fprintf(fileX,"%x\t BLX pc\n", number);
        }else{
            fprintf(fileX,"%x\t BLX r%d\n", number, Rm);
        }
    }
}

//BKPT #imm8
void DecodeBKPT_IMM8(int number, FILE *fileX){

    int imm = immed8(number);                              //pega os bits 0-7 de number, define um imediato de 8 bits.

    fprintf(fileX,"%x\t BKPT #%d\n", number, imm);
}

//B<cond> #offset*2+4
void DecodeBcond_OFFSET(int number, FILE *fileX){

    int cond = ((number & (maskF << 8)) >> 8);                         //pega os bits 8-11 de number, define o tipo de condição e precisa ser 
    int off = immed8(number);                                          //pega os bits 0-7 de number, define um offset de 8 bits

    switch(cond){
        case 0:                                                        //as condicionais vão de 0 a 13
            fprintf(fileX,"%x\t BEQ #%d\n", number, (off*2)+4);
        break;

        case 1:
            fprintf(fileX,"%x\t BNE #%d\n", number, (off*2)+4);
        break;

        case 2:
            fprintf(fileX,"%x\t BCS #%d\n", number, (off*2)+4);
        break;

        case 3:
            fprintf(fileX,"%x\t BCC #%d\n", number, (off*2)+4);
        break;

        case 4:
            fprintf(fileX,"%x\t BMI #%d\n", number, (off*2)+4);
        break;

        case 5:
            fprintf(fileX,"%x\t BPL #%d\n", number, (off*2)+4);
        break;

        case 6:
            fprintf(fileX,"%x\t BVS #%d\n", number, (off*2)+4);
        break;

        case 7:
            fprintf(fileX,"%x\t BVC #%d\n", number, (off*2)+4);
        break;

        case 8:
            fprintf(fileX,"%x\t BHI #%d\n", number, (off*2)+4);
        break;

        case 9:
            fprintf(fileX,"%x\t BLS #%d\n", number, (off*2)+4);
        break;

        case 10:
            fprintf(fileX,"%x\t BGE #%d\n", number, (off*2)+4);
        break;

        case 11:
            fprintf(fileX,"%x\t BLT #%d\n", number, (off*2)+4);
        break;

        case 12:
            fprintf(fileX,"%x\t BGT #%d\n", number, (off*2)+4);
        break;
        
        case 13:
            fprintf(fileX,"%x\t BLE #%d\n", number, (off*2)+4);
        break;

        default:
            fprintf(fileX,"Valor inválido!\n");
    }
}

//B #offset*2+4
void DecodeB_OFFSET(int number, FILE *fileX){

    int off = immed11(number);                               //pega os bits de 0-10 de number, define um offset de 11 bits

    fprintf(fileX,"%x\t B #%d\n", number, (off*2)+4);
}

//SWI #imm8
void DecodeSWI_IMM8(int number, FILE *fileX){

    int imm = immed8(number);                               //pega os bits de 0-7 de number, define um imediato de 8 bits

    fprintf(fileX,"%x\t SWI #%d\n", number, imm);
}

//BLX #((offset*2+4)+(poff << 12)) &~ 3
void DecodeBLX_OFFSET(int number, FILE *fileX){
    
    int maskOFF = 0x3ff;

    int off = ((number & (maskOFF << 1)) >> 1);             //pega os bits 1-10 de number, define um offset de 10 bits

    fprintf(fileX,"%x\t BLX #%d\n", number, ((((poff << 12) & 0xffff)+(off*4)+4) &~ 3));
}

//BL #(offset*2+4)+(poff << 12)
void DecodeBL_OFFSET(int number, FILE *fileX){

    int off = immed11(number);                             //pega os bits de 0-10 de number, define um offset de 11 bits

    fprintf(fileX,"%x\t BL #%d\n", number, (((poff << 12) & 0xffff)+(off*2)+4));
}

//LDR Ld, [pc, #imm8*4]
void DecodeLDR_LD_PC_IMM8X4(int number, FILE *fileX){
    int Ld = ((number & (mask7 << 8)) >> 8);                                   //pega os bits 8-10 de number, define Ld

    fprintf(fileX,"%x\t LDR r%d, [pc, #%d]\n", number, Ld, immed8(number));    //immed8(number) pega os bits 0-7 de number, define um imediato de 8 bits
}

//STR|STRH|STRB|LDRSB Ld, [Ln, Lm]
void DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM(int number, FILE *fileX){
    int Lm = ((number & (mask7 << 6)) >> 6);                                   //pega os bits 6-8 de number, define Lm
    int Ln = ((number & (mask7 << 3)) >> 3);                                   //pega os bits 3-5 de number, define Ln
    int Ld = (number & mask7);                                                 //pega os bits 0-2 de number, define Ld
    op = ((number & (mask3) << 9) >> 9);                                  //pega os bits 9-10 de number, define qual instrução será usada
                                                                               //STR=0, STRH=1, STRB=2, LDRSB=3
    if(op == 0){
        fprintf(fileX,"%x\t STR r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else if(op == 1){
        fprintf(fileX,"%x\t STRH r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else if(op == 2){
        fprintf(fileX,"%x\t STRB r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else{
        fprintf(fileX,"%x\t LDRSB r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }

}

//LDR_LDRH_LDRB_LDRSH Ld, [Ln, Lm]
void DecodeLDR_LDRH_LDRB_LDRSH_LD_LN_LM(int number, FILE *fileX){
    int Lm = ((number & (mask7 << 6)) >> 6);                                  //pega os bits 6-8 de number, define Lm
    int Ln = ((number & (mask7 << 3)) >> 3);                                  //pega os bits 3-5 de number, define Ln
    int Ld = (number & mask7);                                                //pega os bits 0-2 de number, define Ld
    op = ((number & (mask3 << 9)) >> 9);                                 //pega os bits 9-10 de number, define qual instrução será usada
                                                                              //LDR=0, LDRH=1, LDRSH=2, LDRSH= 3
    if(op == 0){
        fprintf(fileX,"%x\t LDR r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else if(op == 1){
        fprintf(fileX,"%x\t LDRH r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else if(op == 2){
        fprintf(fileX,"%x\t LDRB r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }else{
        fprintf(fileX,"%x\t LDRSH r%d, [r%d, r%d]\n", number, Ld, Ln, Lm);
    }

}

//STR_LDR Ld, [Ln, #imm5*4]
void DecodeSTR_LDR_LD_LN_IMM5(int number, FILE *fileX){

    op = ((number & (mask1 << 11)) >> 11);                                  //pega o bit 11 de number, define a instrução usada. STR=0, LDR=1
    int imm = immed5(number);                                                   //pega os bits 6-10 de number, define um imediato de 5 bits
    int Ln = ((number & (mask7 << 3)) >> 3);                                    //pega os bits 3-5 de number, define Ln
    int Ld = (number & mask7);                                                  //pega os bits 0-2 de number, define Ld

    if(op == 0){
        fprintf(fileX,"%x\t STR r%d, [r%d, #%d]\n", number, Ld, Ln, imm*4);
    }else{
        fprintf(fileX,"%x\t LDR r%d, [r%d, #%d]\n", number, Ld, Ln, imm*4);
    }
}

//STRB_LDRB Ld, [Ln, #imm5]
void DecodeSTRB_LDRB_LD_LN_IMM5(int number, FILE *fileX){

    op = ((number & (mask1 << 11)) >> 11);                                 //pega o bit 11 de number, define a instrução usada. STRB=0, LDRB=1
    int imm = immed5(number);                                                  //pega os bits 6-10 de number, define um imediato de 5 bits
    int Ln = ((number & (mask7 << 3)) >> 3);                                   //pega os bits 3-5 de number, define Ln
    int Ld = (number & mask7);                                                 //pega os bits 0-2 de number, define Ld

    if(op == 0){
        fprintf(fileX,"%x\t STRB r%d, [r%d, #%d]\n", number, Ld, Ln, imm);
    }else{
        fprintf(fileX,"%x\t LDRB r%d, [r%d, #%d]\n", number, Ld, Ln, imm);
    }
}

//STRH_LDRH Ld, [Ln, imm5*2]
void DecodeSTRH_LDRH_LD_LN_IMM5(int number, FILE *fileX){

    op = ((number & (mask1 << 11)) >> 11);                                 //pega o bit 11 de number, define a instrução usada. STRH=0, LDRH=1
    int imm = immed5(number);                                                  //pega os bits 6-10 de number, define um imediato de 5 bits
    int Ln = ((number & (mask7 << 3)) >> 3);                                   //pega os bits 3-5 de number, define Ln
    int Ld = (number & mask7);                                                 //pega os bits 0-2 de number, define Ld

    if(op == 0){
        fprintf(fileX,"%x\t STRH r%d, [r%d, #%d]\n", number, Ld, Ln, imm*2);
    }
    else{
        fprintf(fileX,"%x\t LDRH r%d, [r%d, #%d]\n", number, Ld, Ln, imm*2);
    }
}

//STR_LDR Ld, [sp, #imm8*4]
void DecodeSTR_LDR_SP_IMM8(int number, FILE *fileX){

    op = ((number & (mask1 << 11)) >> 11);                                //pega o bit 11 de number, define a instrução usada. STR=0, LDR=1 
    int imm = immed8(number);                                                 //pega os bits 0-7 de number, define um imediato de 8 bits
    int Ld = ((number & (mask7 << 8)) >> 8);                                  //pega os bits 8-10 de number, define Ld

    if(op == 0){
        fprintf(fileX,"%x\t STR r%d, [sp, #%d]\n", number, Ld, imm*4);
    }else{
        fprintf(fileX,"%x\t LDR r%d, [sp, #%d]\n", number, Ld, imm*4);
    }
}

// <PUSH | POP> {register_list, R}
void DecodePUSH_POP(int number, FILE *fileX){
    if((number & 0xFF) > 0){
        op = (number & (mask1 << 0xB)) >> 0xB;                              // pega o Bit 11 para escolher uma das 2 instruções
        int R  = (number & (mask1 << 0x8)) >> 0x8;                              // pega o Bit 8 para saber se coloca ou não lr para push e pc para pop

        if(op == 0){                                                            // escolhe se é push ou pop e já imprime no arquivo
            fprintf(fileX,"%x\t PUSH {",number); 
        }
        else{
            fprintf(fileX,"%x\t POP {",number);
        }

                                                                                // Formar saída da lista de registradores ativos
                                                                                // Obs: Assim que um registrador estiver apto para ser impresso, ele será
                                                                                // botaVirg para sinalizar quando deve por uma vírgula após a último registrador impresso
                                                                                // c dirá a quantidade de bits 1 sequenciais
                                                                                // register_list vai armazenar os índices de 0 até 7 que estão ativos (logo igual a 1) sequencialmente (sem 0 entre eles)
        int c = 0, botaVirg = 0;
        int register_list[8];
        int value = number & 0xFF;                                              // Pegando somente os bits [0-7]

        for(int i = 0; i < 9; i++)                                              // i até 8, pois caso o bit 7 esteja ativo será preciso imprimir-lo
        {
            if((value & 0x1) == 1){                                             // Verifica se o bit em que estou é 1         
                register_list[c] = i;                                           // Se for, então armazeno seu índice em register_list e incrementa c
                c++;
                
            }
            else if(c > 0){                                                     // Se o bit em que estou foi 0 e meu c é maior que 0, então posso ter ao menos um registrador para ser impresso
                if(botaVirg > 0){
                    fprintf(fileX, ",");
                }
                                                                                // Haverá somente uma das 3 opções de impressão:            
                if(c == 1)                                                      // 1) Imprimindo apenas um registrador;
                {                                                               // ex = in: 0000 0001 | out: r0 sendo impresso
                    fprintf(fileX, "r%d", register_list[0]);
                }
                else if(c == 2)                                                 // 2) Imprimindo dois registradores;
                {                                                               // ex = in: 0000 0011 | out: r0,r1 sendo impresso
                    fprintf(fileX, "r%d, r%d", register_list[0],register_list[1]);
                }
                else                                                            // 3) Imprimindo um conjunto de registradores, mas apenas será preciso <primeiro registrador> e o <último registrador> salvo em register_list;                                                            
                    fprintf(fileX, "r%d-r%d", register_list[0],register_list[c-1]);// ex = in: 0111 1000 | out: r3-r6 sendo impresso
                
                c = 0;                                                          // Limpa c, pois o que já foi impresso não interessa mais
                botaVirg = 1;                                                   // Ativa botaVirg, para sinalizar que uma vírgula precisa ser colocada antes de imprimir o novo registrador ou conjunto de registradores
            }

            value = value >> 0x1;                                               // Desloca value tendo assim bit por bit        
        }
        

        if(R == 1){                                                             // Imprime no arquivo de saída caso R esteja ativo
            if(op == 0) fprintf(fileX, ",LR}\n");
            else fprintf(fileX, ",PC}\n");
        }
        else
            fprintf(fileX, "}\n");
    }
    else                                                                        // Caso não haja nenhum registrador, logo se do bit 0 ao 7 tiver apenas 0
        fprintf(fileX, "%d\t UNPREDICTABLE", number);     
}

// <STMIA | LDMIA> Ln!, {register_list}
void DecodeSTMIA_LDMIA(int number, FILE *fileX){
    if((number & 0xFF) > 0){
        op = (number & (mask1 << 0xB)) >> 0xB;                              // pega o Bit 11 para escolher uma das 2 instruções
        int Ln  = (number & (mask7 << 0x8)) >> 0x8;                             // pega os bits 8-10 de number, define registrador Ln


        if(op == 0){                                                            // escolhe se é STMIA ou LDMIA e já imprime no arquivo
            fprintf(fileX,"%x\t STMIA r%d!, {",number,Ln); 
        }
        else{
            fprintf(fileX,"%x\t LDMIA r%d!, {",number,Ln);
        }

                                                                                // Formar saída da lista de registradores ativos
                                                                                // Obs: Assim que um registrador estiver apto para ser impresso, ele será
                                                                                // botaVirg para sinalizar quando deve por uma vírgula após a último registrador impresso
                                                                                // c dirá a quantidade de bits 1 sequenciais
                                                                                // register_list vai armazenar os índices de 0 até 7 que estão ativos (logo igual a 1) sequencialmente (sem 0 entre eles)
        int c = 0, botaVirg = 0;
        int register_list[8];
        int value = number & 0xFF;                                              // Pegando somente os bits [0-7]

        for(int i = 0; i < 9; i++)                                              // i até 8, pois caso o bit 7 esteja ativo será preciso imprimir-lo
        {
            if((value & 0x1) == 1){                                             // Verifica se o bit em que estou é 1
                register_list[c] = i;                                           // Se for, então armazeno seu índice em register_list e incrementa c
                c++;  
            }
            else if(c > 0){                                                     // Se o bit em que estou foi 0 e meu c é maior que 0, então posso ter ao menos um registrador para ser impresso
                if(botaVirg > 0){
                    fprintf(fileX, ",");
                }
                                                                                // Haverá somente uma das 3 opções de impressão:            
                if(c == 1)                                                      // 1) Imprimindo apenas um registrador;
                {                                                               // ex = in: 0000 0001 | out: r0 sendo impresso
                    fprintf(fileX, "r%d", register_list[0]);
                }
                else if(c == 2)                                                 // 2) Imprimindo dois registradores;
                {                                                               // ex = in: 0000 0011 | out: r0,r1 sendo impresso
                    fprintf(fileX, "r%d, r%d", register_list[0],register_list[1]);
                }
                else                                                            // 3) Imprimindo um conjunto de registradores, mas apenas será preciso <primeiro registrador> e o <último registrador> salvo em register_list;                                                            
                    fprintf(fileX, "r%d-r%d", register_list[0],register_list[c-1]);// ex = in: 0111 1000 | out: r3-r6 sendo impresso
                
                c = 0;                                                          // Limpa c, pois o que já foi impresso não interessa mais
                botaVirg = 1;                                                   // Ativa botaVirg, para sinalizar que uma vírgula precisa ser colocada antes de imprimir o novo registrador ou conjunto de registradores
            }

            value = value >> 0x1;                                               // Desloca value tendo assim bit por bit        
        }
        
        fprintf(fileX, "}\n");
    }
    else
        fprintf(fileX, "%d\t UNPREDICTABLE", number);                           // Caso não haja nenhum registrador, logo se do bit 0 ao 7 tiver apenas 0      
}
// ADD Ld, pc, #immed*4 | ADD Ld, sp, #immed*4
void DecodeADDLdpc_ADDLdsp(int number, FILE *fileX){
    op = (number & (mask1 << 0xb)) >> 0xb;	//recebe o valor do operando que está no bit 11.                                  
    int Ld = (number & (mask7 << 0x8)) >> 0x8;  //recebe o valor que representa o registrador Ld

	// Se op = 0, então ADD Ld, pc, #immed*4
    if(op == 0){
        fprintf(fileX,"%x\t ADD r%d, pc, #%d \n",number,Ld,immed8(number)*4);
    }
    //Se op = 1, então ADD Ld, sp, #immed*4
    else{
        fprintf(fileX,"%x\t ADD r%d, sp, #%d \n",number,Ld,immed8(number)*4);
    }
}

// ADD sp, #immed*4 | SUB sp, #immed*4
void DecodeADDsp_SUBsp(int number, FILE *fileX){
    op = (number & (mask1 << 0x7)) >> 0x7; //recebe o valor do operando que está no bit 7.                                  

	// Se op = 0, então ADD sp, #immed*4
    if(op == 0){
        fprintf(fileX,"%x\t ADD  sp, #%d \n", number,(immed7(number))*4);
    }
    //Se op = 1, então SUB sp, #immed*4
    else{
        fprintf(fileX,"%x\t SUB sp, #%d \n", number ,(immed7(number))*4);
    }
}

// SETEND LE | SETEND BE
void DecodeSETENDLE_SETENDBE(int number, FILE *fileX){
    op = (number & (mask1 << 0x3)) >> 0x3;  //recebe o valor do op que está no bit 3.                              
	
	// Se op = 0, então SETEND LE
    if(op == 0){
        fprintf(fileX,"%x\t SETEND LE \n", number);
    }
    // Se op = 1, então SETEND BE
    else{
        fprintf(fileX,"%x\t SETEND BE \n", number);
    }
}

//SXTH | SXTB | UXTH | UXTB
void DecodeSXTH_SXTB_UXTH_UXTB(int number, FILE *fileX){
    op = (number & 0xC0) >> 6; //salva o valor de op que se encontra nos bits 6 e 7.
    int Ld = first3Bits(number);			   //recebe o valor que representa o registrador nos 3 primeiros bits em Ld.
    int Lm = second3Bits(number);              //recebe o valor de três bits que representa o registrador e salva em Lm.

	// se op = 0, então SXTH.
    if(op == 0 ){
        fprintf(fileX,"%x\t SXTH r%d, r%d \n",number, Ld, Lm);
    }
    // se op = 1, então SXTB. 
    else if(op == 1){
        fprintf(fileX,"%x\t SXTB r%d, r%d \n",number, Ld, Lm);
    }
    // se op = 2, então UXTH.
    else if(op == 2){
        fprintf(fileX,"%x\t UXTH r%d, r%d \n",number, Ld, Lm);
    }
    // Se op = 3, então UXTB.
    else{
        fprintf(fileX,"%x\t UXTB r%d, r%d \n",number, Ld, Lm);
    }
}

//REV | REV16 | nada | REVSH
void DecodeREV_REV16_REVSH(int number, FILE *fileX){
    op = (number & 0xC0) >> 6; //salva o valor de op que se encontra nos bits 6 e 7.
    int Ld = first3Bits(number);			   //recebe o valor que representa o registrador nos 3 primeiros bits em Ld.
    int Lm = second3Bits(number);              //recebe o valor de três bits que representa o registrador e salva em Lm.

	// se op = 0, então REV.
    if(op == 0 ){
        fprintf(fileX,"%x\t REV r%d, r%d \n",number, Ld, Lm);
    }
    
    //se op = 1, então REV.
    else if(op == 1){
        fprintf(fileX,"%x\t REV16 r%d, r%d \n",number, Ld, Lm);
    } 
    //  se op = 2, então nada.
    else if(op == 2){
        fprintf(fileX,"%x\t Undefined \n", number);
    }
    // se op = 3, então REVSH. 
    else{
        fprintf(fileX,"%x\t REVSH r%d, r%d \n",number, Ld, Lm);
    }
}

//CPSIE | CPSID
void DecodeCPSIE_CPSID(int number, FILE *fileX){
    op = (number & (mask1 << 0x4)) >> 0x4; // recebe o valor que está no bit 4 que define se é CPSIE ou CPSID.
	int none = 0; // variavel para ver se algum bit de flag foi ativado.

    if(op == 0){
        fprintf(fileX,"%x\t CPSIE ", number); //imprimir a mensagem parcialmente.
        
        //verficar se o bit a está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & (mask1 << 2)) >> 2 == 1){ 
			fprintf(fileX,"a");
			none = 1;
		}
        
        //verficar se o bit i está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & (mask1 << 1)) >> 1 == 1){ 
			fprintf(fileX,"i");
			none = 1;
		}
        
        //verficar se o bit f está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & mask1) == 1){     
			fprintf(fileX,"f");
			none = 1;
		}
        
        //verficar se none é zero, e se for imprimir none.
        if(none == 0){	fprintf(fileX,"none\n");}
        
        fprintf(fileX," \n");
    }
    
    else{
        fprintf(fileX,"%x\t CPSIE ", number);//imprimir a mensagem parcialmente.
        
        //verficar se o bit a está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & (mask1 << 2)) >> 2 == 1){ 
			fprintf(fileX,"a");
			none = 1;
		}
        
        //verficar se o bit i está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & (mask1 << 1)) >> 1 == 1){ 
			fprintf(fileX,"i");
			none = 1;
		}
        
        //verficar se o bit f está ativado e se estiver imprimir ele e colocar 1 em none.
        if((number & mask1) == 1){     
			fprintf(fileX,"f");
			none = 1;
		}
        
        //verficar se none é zero, e se for imprimir none.
        if(none == 0){	fprintf(fileX,"none\n");}
        
        fprintf(fileX," \n");
    }
}

//Undefined and expected to remain so
void DecodeUndefined(int number, FILE *fileX){
    fprintf(fileX, "%x\t  Undefined and expected to remain so\n", number);
}

//-------------------------------------------------------------------------------
// Instrução não definida
void underfinedGrup04(int number,FILE *fileX){
  fprintf(fileX, "%x\t Sem instrução correpondente\n",number);
}

//-------------------------------------------------------------------------------
// Instrução não definida
void underfined(int number,FILE *fileX){
  fprintf(fileX, "%x\t Undefined\n",number);
}

void fPoff(int number, FILE *fileX,int flagPoff){
    if(flagPoff == 0){                                                          // Atribuindo novo valor a poff
        poff = number;
    }
    else if(flagPoff == 1){                                                     // Error na instrução que alterou poff anteriormente
        fprintf(fileX, "%x\t Error\n",poff);
        poff = 0;                                                               // poff em seu estado natural
    }
    else{
        fprintf(fileX, "%x\t\n",poff);                                          // instrução que alterou poff anteriormente foi sucedida por uma BLX ou BL que usa o poff
    }
}


