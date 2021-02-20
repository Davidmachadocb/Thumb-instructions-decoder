#include <stdio.h>
#include <stdlib.h>
#include "Decode.h"


// < LSL | LSR > Ld, Lm, #<immed5>
void DecodeLSL_LSR_LD_LM_IMM5(int number, FILE *fileX){
    int op = (number & (mask1 << 0xB)) >> 0xB;                                   // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t LSL r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
    else
        fprintf(fileX,"%x\t LSR r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
}

// < ASR > Ld, Lm, #<immed5>
void DecodeASR_LD_LM_IMM5(int number, FILE *fileX){
    fprintf(fileX,"%x\t ASR r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),immed5(number));
}

// < ADD | SUB > Ld, Ln, Lm
void DecodeADD_SUB_LD_LN_LM(int number, FILE *fileX){
    int op = (number & (mask1 << 0x9)) >> 0x9;                                  // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d, r%d \n",number,first3Bits(number),second3Bits(number),third3Bits(number));
    else
        fprintf(fileX,"%x\t SUB r%d, r%d, r%d \n",number,first3Bits(number),second3Bits(number),third3Bits(number));
}

// < ADD | SUB > com Ld, Ln, #<immed3>
void DecodeADD_SUB_LD_LN_IMM3(int number, FILE *fileX){
    int Immed3 = (((mask7 << 0x6) & number) >> 0x6);                            // Bits [6-8]
    int op = (number & (mask1 << 0x9)) >> 0x9;                                  // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),Immed3);
    else
        fprintf(fileX,"%x\t SUB r%d, r%d, #%d \n",number,first3Bits(number),second3Bits(number),Immed3);
}

// < MOV | CMP > Ld, #<immed8>
void DecodeMOV_CMP_LD_IMM8(int number, FILE *fileX){
    int Ld_Ln = (number & (mask7 << 0x8)) >> 0x8;                               // Bits [8-10]
    int op = (number & (mask1 << 0xB)) >> 0xB;                                  // Bit [11]

    if(op == 0)
        fprintf(fileX,"%x\t MOV r%d, #%d \n",number,Ld_Ln,immed8(number));
    else
        fprintf(fileX,"%x\t CMP r%d, #%d \n",number,Ld_Ln,immed8(number));
}

// < ADD | SUB > Ld, #<immed8>
void DecodeADD_SUB_LD_IMM8(int number, FILE *fileX){
    int Ld = (number & (mask7 << 0x8)) >> 0x8;                                  // Bits [8-10]
    int op = (number & (mask1 << 0xB)) >> 0xB;                                  // Bit [11]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, #%d \n",number,Ld,immed8(number));
    else
        fprintf(fileX,"%x\t SUB r%d, #%d \n",number,Ld,immed8(number));
}

// < AND | EOR | LSL | LSR >  Ld, Lm
void DecodeAND_EOR_LSL_LSR_LD_LM(int number, FILE *fileX){
    int op = (number & 0xC0) >> 6;                                              // Bits [6,7]

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
void DecodeASR_ADC_SBC_ROR_LD_LM_LS(int number, FILE *fileX){
    int op = (number & 0xC0) >> 6;                                              // Bits [6,7]

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
void DecodeTST_NEG_CMP_CMN_LD_LN_LM(int number, FILE *fileX){
    int op = (number & 0xC0) >> 6;                                              // Bits [6,7]

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
void DecodeORR_MUL_BIC_MVN_LD_LM(int number, FILE *fileX){
    int op = (number & 0xC0) >> 6;                                              // Bits [6,7]

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
void DecodeCPY_LD_LM(int number, FILE *fileX){
    fprintf(fileX,"%x\t CPY r%d, r%d \n",number,first3Bits(number),second3Bits(number));
}

// < ADD | MOV > Ld, Hm
void DecodeADD_MOV_LD_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    int op = (number & (mask1 << 0x9)) >> 0x9;                                  // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d \n",number,first3Bits(number),Hm);
    else
        fprintf(fileX,"%x\t MOV r%d, r%d \n",number,first3Bits(number),Hm);
}

// < ADD | MOV > Hd, Lm
void DecodeADD_MOV_HD_LM(int number, FILE *fileX){
    int Hd = first3Bits(number) | 0x8;                                          // Transformando em um high-register
    int op = (number & (mask1 << 0x9)) >> 0x9;                                  // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d \n",number,Hd,second3Bits(number));
    else
        fprintf(fileX,"%x\t MOV r%d, r%d \n",number,Hd,second3Bits(number));
}

// < ADD | MOV > Hd, Hm
void DecodeADD_MOV_HD_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    int Hd = first3Bits(number) | 0x8;                                          // Transformando em um high-register
    int op = (number & (mask1 << 0x9)) >> 0x9;                                  // Bit [9]

    if(op == 0)
        fprintf(fileX,"%x\t ADD r%d, r%d \n",number,Hd,Hm);                    
    else
        fprintf(fileX,"%x\t MOV r%d, r%d \n",number,Hd,Hm);                     

}

// < CMP > Ln, Hm
void DecodeCMP_LN_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register

    fprintf(fileX,"%x\t CMP r%d, r%d \n",number,first3Bits(number),Hm);
}

// < CMP > Hn, Lm
void DecodeCMP_HN_LM(int number, FILE *fileX){
    int Hn = first3Bits(number) | 0x8;                                          // Transformando em um high-register

    fprintf(fileX,"%x\t CMP r%d, r%d \n",number,Hn,second3Bits(number));       
}

// < CMP > Hn, Hm
void DecodeCMP_HN_HM(int number, FILE *fileX){
    int Hm = second3Bits(number) | 0x8;                                         // Transformando em um high-register
    int Hn = first3Bits(number) | 0x8;                                          // Transformando em um high-register

    fprintf(fileX,"%x\t CMP r%d, r%d \n",number,Hn,Hm);                        

}

//BX_BLX Rm
void DecodeBX_BLX_RM(int number, FILE *fileX){
    int maskR = 0xf;

    int op = ((number & (mask1 << 7)) >> 7);
    int Rm = ((number & (maskR << 3)) >> 3);

    if(op == 0 ){
        if(Rm == 13){
            fprintf(fileX,"%x BX sp\n", number);
        }else if(Rm == 14){
            fprintf(fileX,"%x BX lr\n", number);
        }else if(Rm == 15){
            fprintf(fileX,"%x BX pc\n", number);
        }else{
            fprintf(fileX,"%x BX r%d\n", number, Rm);
        }
    }else{
        if(Rm == 13){
            fprintf(fileX,"%x BLX sp\n", number);
        }else if(Rm == 14){
            fprintf(fileX,"%x BLX lr\n", number);
        }else if(Rm == 15){
            fprintf(fileX,"%x BLX pc\n", number);
        }else{
            fprintf(fileX,"%x BLX r%d\n", number, Rm);
        }
    }
}

//BKPT #imm8
void DecodeBKPT_IMM8(int number, FILE *fileX){

    int imm = immed8(number);

    fprintf(fileX,"%x\t BKPT #%d\n", number, imm);
}

//B<cond> #offset*2+4
void DecodeBcond_OFFSET(int number, FILE *fileX){
    int maskC = 0xf;

    int cond = ((number & (maskC << 8)) >> 8);
    int imm = immed8(number);

    switch(cond){
        case 0:
            fprintf(fileX,"%x\t BEQ #%d\n", number, (imm*2)+4);
        break;

        case 1:
            fprintf(fileX,"%x\t BNE #%d\n", number, (imm*2)+4);
        break;

        case 2:
            fprintf(fileX,"%x\t BCS #%d\n", number, (imm*2)+4);
        break;

        case 3:
            fprintf(fileX,"%x\t BCC #%d\n", number, (imm*2)+4);
        break;

        case 4:
            fprintf(fileX,"%x\t BMI #%d\n", number, (imm*2)+4);
        break;

        case 5:
            fprintf(fileX,"%x\t BPL #%d\n", number, (imm*2)+4);
        break;

        case 6:
            fprintf(fileX,"%x\t BVS #%d\n", number, (imm*2)+4);
        break;

        case 7:
            fprintf(fileX,"%x\t BVC #%d\n", number, (imm*2)+4);
        break;

        case 8:
            fprintf(fileX,"%x\t BHI #%d\n", number, (imm*2)+4);
        break;

        case 9:
            fprintf(fileX,"%x\t BLS #%d\n", number, (imm*2)+4);
        break;

        case 10:
            fprintf(fileX,"%x\t BGE #%d\n", number, (imm*2)+4);
        break;

        case 11:
            fprintf(fileX,"%x\t BLT #%d\n", number, (imm*2)+4);
        break;

        case 12:
            fprintf(fileX,"%x\t BGT #%d\n", number, (imm*2)+4);
        break;
        
        case 13:
            fprintf(fileX,"%x\t BLE #%d\n", number, (imm*2)+4);
        break;

        default:
            fprintf(fileX,"Valor inválido!\n");
    }
}

//B #offset*2+4
void DecodeB_OFFSET(int number, FILE *fileX){

    int imm = immed11(number);

    fprintf(fileX,"%x\t B #%d\n", number, (imm*2)+4);
}

//SWI #imm8
void DecodeSWI_IMM8(int number, FILE *fileX){

    int imm = immed8(number);

    fprintf(fileX,"%x\t SWI #%d\n", number, imm);
}

//BLX #offset*2+4
void DecodeBLX_OFFSET(int number, FILE *fileX){
    int maskIMM = 0x3ff;

    int imm = ((number & (maskIMM << 1)) >> 1);

    fprintf(fileX,"%x\t BLX #%d\n", number, (imm*2)+4);
}

//BL #offset*2+4
void DecodeBL_OFFSET(int number, FILE *fileX){

    int imm = immed11(number);

    fprintf(fileX,"%x\t BL #%d\n", number, (imm*2)+4);
}

//LDR Ld, [pc, #imm8*4]
void DecodeLDR_LD_PC_IMM8X4(int number, FILE *fileX){
    int Ld = ((number & (mask7 << 8)) >> 8);

    fprintf(fileX,"%x\t LDR r%d, [pc, #%d]\n", number, Ld, first3Bits(number));
}

//STR|STRH|STRB|LDRSB Ld, [Ln, Lm]
void DecodeSTR_STRH_STRB_LDRSB_LD_LN_LM(int number, FILE *fileX){
    int maskOP = 0x2;
    int Lm = ((number & (mask7 << 6)) >> 6);
    int Ln = ((number & (mask7 << 3)) >> 3);
    int Ld = (number & mask7);
    int op = ((number & (maskOP) << 9) >> 9);

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
    int maskOP = 0x2;
    int Lm = ((number & (mask7 << 6)) >> 6);
    int Ln = ((number & (mask7 << 3)) >> 3);
    int Ld = (number & mask7);
    int op = ((number & (maskOP << 9)) >> 9);

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

    int op = ((number & (mask1 << 11)) >> 11);
    int imm = immed5(number);
    int Ln = ((number & (mask7 << 3)) >> 3);
    int Ld = (number & mask7);

    if(op == 0){
        fprintf(fileX,"%x\t STR r%d, [r%d, #%d]\n", number, Ld, Ln, imm*4);
    }else{
        fprintf(fileX,"%x\t LDR r%d, [r%d, #%d]\n", number, Ld, Ln, imm*4);
    }
}

//STRB_LDRB Ld, [Ln, #imm5]
void DecodeSTRB_LDRB_LD_LN_IMM5(int number, FILE *fileX){

    int op = ((number & (mask1 << 11)) >> 11);
    int imm = immed5(number);
    int Ln = ((number & (mask7 << 3)) >> 3);
    int Ld = (number & mask7);

    if(op == 0){
        fprintf(fileX,"%x\t STRB r%d, [r%d, #%d]\n", number, Ld, Ln, imm);
    }else{
        fprintf(fileX,"%x\t LDRB r%d, [r%d, #%d]\n", number, Ld, Ln, imm);
    }
}

//STRH_LDRH Ld, [Ln, imm5*2]
void DecodeSTRH_LDRH_LD_LN_IMM5(int number, FILE *fileX){

    int op = ((number & (mask1 << 11)) >> 11);
    int imm = immed5(number);
    int Ln = ((number & (mask7 << 3)) >> 3);
    int Ld = (number & mask7);

    if(op == 0){
        fprintf(fileX,"%x\t STRH r%d, [r%d, #%d]\n", number, Ld, Ln, imm*2);
    }
    else{
        fprintf(fileX,"%x\t LDRH r%d, [r%d, #%d]\n", number, Ld, Ln, imm*2);
    }
}

//STR_LDR Ld, [sp, #imm8*4]
void DecodeSTR_LDR_SP_IMM8(int number, FILE *fileX){

    int op = ((number & (mask1 << 11)) >> 11);
    int imm = immed8(number);
    int Ld = ((number & (mask7 << 8)) >> 8);

    if(op == 0){
        fprintf(fileX,"%x\t STR r%d, [sp, #%d]\n", number, Ld, imm*4);
    }else{
        fprintf(fileX,"%x\t LDR r%d, [sp, #%d]\n", number, Ld, imm*4);
    }
}

// <PUSH | POP> {register_list, R}
void DecodePUSH_POP(int number, FILE *fileX){
    if((number & 0xFF) > 0){
        int op = (number & (mask1 << 0xB)) >> 0xB;
        int R  = (number & (mask1 << 0x8)) >> 0x8;

        if(op == 0){
            fprintf(fileX,"%x\t PUSH {",number); 
        }
        else{
            fprintf(fileX,"%x\t POP {",number);
        }

        int c = 0, botaVirg = 0;
        int register_list[8];
        int value = number & 0xFF;

        for(int i = 0; i < 9; i++)
        {
            if((value & 0x1) == 1){ 
                register_list[c] = i;
                c++;
                
            }
            else if(c > 0){
                if(botaVirg > 0){
                    fprintf(fileX, ",");
                }

                if(c == 1){
                    fprintf(fileX, "r%d", register_list[0]);
                }
                else if(c == 2){
                    fprintf(fileX, "r%d, r%d", register_list[0],register_list[1]);
                }
                else
                    fprintf(fileX, "r%d-r%d", register_list[0],register_list[c-1]);
                
                c = 0;
                botaVirg = 1;
            }
            value = value >> 0x1;
        }
        

        if(R == 1){
            if(op == 0) fprintf(fileX, ",LR}\n");
            else fprintf(fileX, ",PC}\n");
        }
        else
            fprintf(fileX, "}\n");
    }
    else
        fprintf(fileX, "%d\t UNPREDICTABLE", number);        
}

// <STMIA | LDMIA> Ln!, {register_list}
void DecodeSTMIA_LDMIA(int number, FILE *fileX){
    if((number & 0xFF) > 0){
        int op = (number & (mask1 << 0xB)) >> 0xB;
        int Ln  = (number & (mask7 << 0x8)) >> 0x8;


        if(op == 0){
            fprintf(fileX,"%x\t STMIA r%d!, {",number,Ln); 
        }
        else{
            fprintf(fileX,"%x\t LDMIA r%d!, {",number,Ln);
        }

        int c = 0, botaVirg = 0;
        int register_list[8];
        int value = number & 0xFF;

        for(int i = 0; i < 9; i++)
        {
            if((value & 0x1) == 1){ 
                register_list[c] = i;
                c++;  
            }
            else if(c > 0){
                if(botaVirg > 0){
                    fprintf(fileX,",");
                }

                if(c == 1){
                    fprintf(fileX,"r%d", register_list[0]);
                }
                else if(c == 2){
                    fprintf(fileX,"r%d, r%d", register_list[0],register_list[1]);
                }
                else
                    fprintf(fileX,"r%d-r%d", register_list[0],register_list[c-1]);
                
                c = 0;
                botaVirg = 1;
            }
            value = value >> 0x1;
        }
        
        fprintf(fileX, "}\n");
    }
    else
        fprintf(fileX, "%d\t UNPREDICTABLE", number);      
}

// ADD Ld, pc, #immed*4 | ADD Ld, sp, #immed*4
void DecodeADDLdpc_ADDLdsp(int number, FILE *fileX){
    int op = (number & (mask1 << 0xb)) >> 0xb;                                  
    int Ld = (number & (mask7 << 0x8)) >> 0x8;

    if(op == 0){
        fprintf(fileX,"%x\t ADD r%d, pc, #%d \n",number,Ld,immed8(number)*4);
    }
    else{
        fprintf(fileX,"%x\t ADD r%d, sp, #%d \n",number,Ld,immed8(number)*4);
    }
}

// ADD sp, #immed*4 | SUB sp, #immed*4
void DecodeADDsp_SUBsp(int number, FILE *fileX){
    int op = (number & (mask1 << 0x7)) >> 0x7;                                  

    if(op == 0){
        fprintf(fileX,"%x\t ADD  sp, #%d \n", number,(immed7(number))*4);
    }
    else{
        fprintf(fileX,"%x\t SUB sp, #%d \n", number ,(immed7(number))*4);
    }
}

// SETEND LE | SETEND BE
void DecodeSETENDLE_SETENDBE(int number, FILE *fileX){
    int op = (number & (mask1 << 0x3)) >> 0x3;                                 

    if(op == 0){
        fprintf(fileX,"%x\t SETEND LE \n", number);
    }
    else{
        fprintf(fileX,"%x\t SETEND BE \n", number);
    }
}

//SXTH | SXTB | UXTH | UXTB
void DecodeSXTH_SXTB_UXTH_UXTB(int number, FILE *fileX){
    int op = (number & (mask3 << 0x6)) >> 0x6;
    int Ld = number & maskF;
    int Lm = (number & (mask7 << 0x3)) >> 0x3;

    if(op == 0 ){
        fprintf(fileX,"%x\t SXTH r%d, r%d \n",number, Lm, Ld);
    }
    else if(op == 1){
        fprintf(fileX,"%x\t SXTB r%d, r%d \n",number, Lm, Ld);
    }
    else if(op == 2){
        fprintf(fileX,"%x\t UXTH r%d, r%d \n",number, Lm, Ld);
    }
    else{
        fprintf(fileX,"%x\t UXTB r%d, r%d \n",number, Lm, Ld);
    }
}

//REV | REV16 | | REVSH
void DecodeREV_REV16_REVSH(int number, FILE *fileX){
    int op = (number & (mask3 << 0x6)) >> 0x6;
    int Ld = number & maskF;
    int Lm = (number & (mask7 << 0x3)) >> 0x3;

    if(op == 0 ){
        fprintf(fileX,"%x\t REV r%d, r%d \n",number, Lm, Ld);
    }
    else if(op == 1){
        fprintf(fileX,"%x\t REV16 r%d, r%d \n",number, Lm, Ld);
    }
    else if(op == 2){
        fprintf(fileX,"%x\t", number);
    }
    else{
        fprintf(fileX,"%x\t REVSH r%d, r%d \n",number, Lm, Ld);
    }
}

//CPSIE | CPSID
void DecodeCPSIE_CPSID(int number, FILE *fileX){
    int op = (number & (mask1 << 0x4)) >> 0x4;


    if(op == 0){
        fprintf(fileX,"%x\t CPSIE ", number);
        if((number & (mask1 << 2)) >> 2 == 1) fprintf(fileX,"a");
        if((number & (mask1 << 1)) >> 1 == 1) fprintf(fileX,"i");
        if((number & mask1) == 1)     fprintf(fileX,"f");
        fprintf(fileX," \n");
    }
    else{
        fprintf(fileX,"%x\t CPSID ", number);
        if((number & (mask1 << 2)) >> 2 == 1) fprintf(fileX,"a");
        if((number & (mask1 << 1)) >> 1 == 1) fprintf(fileX,"i");
        if((number & mask1) == 1)     fprintf(fileX,"f");;
        fprintf(fileX," \n");
    }
}

//Undefined and expected to remain so
void DecodeUndefined(int number, FILE *fileX){
    fprintf(fileX, "%x\t  Undefined and expected to remain so\n", number);
}


