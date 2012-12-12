#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINELENGTH 128
#define MAXLABLELENGTH 16
#define MAXOPERANDLEN 64
#define NUMSINGLEOPINS 27
#define NUMDOUBLEOPINS 12
#define NUMBRANCHINS 17
#define NUMJUMPINS 4
#define NUMCONDITIONINS 10
#define NUMNOOPINS 7
#define NUMWEIRDINS 6
#define BINARRAYSIZE 28672
#define MAPARRAYSIZE 28672
#define MAXNUMBRANCHES 256

struct instruction {
  char* mnemonic;
  char* opcode;
};

struct instruction singleopins[NUMSINGLEOPINS];
struct instruction doubleopins[NUMDOUBLEOPINS];
struct instruction branchins[NUMBRANCHINS];
struct instruction jumpins[NUMJUMPINS];
struct instruction conditionins[NUMCONDITIONINS];
struct instruction noopins[NUMNOOPINS];
struct instruction weirdins[NUMWEIRDINS];

char *binarray[BINARRAYSIZE];
int binarrayindex = 256;
char *maparray[MAPARRAYSIZE];
int mapoffset = 0;
int maparrayindex = 256;

struct cell {
  char *lable;
  int index;
};

struct cell listOne[MAXNUMBRANCHES];
struct cell listTwo[MAXNUMBRANCHES];
int listOnePtr = 0;
int listTwoPtr = 0;

/******************************************************************/

void initArrays(void) {
  int i;

  for(i=0; i<BINARRAYSIZE; i++) {
    binarray[i] = NULL;
    maparray[i] = NULL;
  }
}

/******************************************************************/

int fillInstrArrays(void) {
  int i;

  for(i=0; i<NUMSINGLEOPINS; i++) {
    singleopins[i].mnemonic = (char *)malloc(5);
    singleopins[i].opcode = (char *)malloc(5);
    if((singleopins[i].mnemonic == NULL)||(singleopins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(singleopins[0].mnemonic, "CLR");
  strcpy(singleopins[0].opcode, "0050");
  strcpy(singleopins[1].mnemonic, "CLRB");
  strcpy(singleopins[1].opcode, "1050");
  strcpy(singleopins[2].mnemonic, "COM");
  strcpy(singleopins[2].opcode, "0051");
  strcpy(singleopins[3].mnemonic, "COMB");
  strcpy(singleopins[3].opcode, "1051");
  strcpy(singleopins[4].mnemonic, "INC");
  strcpy(singleopins[4].opcode, "0052");
  strcpy(singleopins[5].mnemonic, "INCB");
  strcpy(singleopins[5].opcode, "1052");
  strcpy(singleopins[6].mnemonic, "DEC");
  strcpy(singleopins[6].opcode, "0053");
  strcpy(singleopins[7].mnemonic, "DECB");
  strcpy(singleopins[7].opcode, "1053");
  strcpy(singleopins[8].mnemonic, "NEG");
  strcpy(singleopins[8].opcode, "0054");
  strcpy(singleopins[9].mnemonic, "NEGB");
  strcpy(singleopins[9].opcode, "1054");
  strcpy(singleopins[10].mnemonic, "ADC");
  strcpy(singleopins[10].opcode, "0055");
  strcpy(singleopins[11].mnemonic, "ADCB");
  strcpy(singleopins[11].opcode, "1055");
  strcpy(singleopins[12].mnemonic, "SBC");
  strcpy(singleopins[12].opcode, "0056");
  strcpy(singleopins[13].mnemonic, "SBCB");
  strcpy(singleopins[13].opcode, "1056");
  strcpy(singleopins[14].mnemonic, "TST");
  strcpy(singleopins[14].opcode, "0057");
  strcpy(singleopins[15].mnemonic, "TSTB");
  strcpy(singleopins[15].opcode, "1057");
  strcpy(singleopins[16].mnemonic, "ROR");
  strcpy(singleopins[16].opcode, "0060");
  strcpy(singleopins[17].mnemonic, "RORB");
  strcpy(singleopins[17].opcode, "1060");
  strcpy(singleopins[18].mnemonic, "ROL");
  strcpy(singleopins[18].opcode, "0061");
  strcpy(singleopins[19].mnemonic, "ROLB");
  strcpy(singleopins[19].opcode, "1061");
  strcpy(singleopins[20].mnemonic, "ASR");
  strcpy(singleopins[20].opcode, "0062");
  strcpy(singleopins[21].mnemonic, "ASRB");
  strcpy(singleopins[21].opcode, "1062");
  strcpy(singleopins[22].mnemonic, "ASL");
  strcpy(singleopins[22].opcode, "0063");
  strcpy(singleopins[23].mnemonic, "ASLB");
  strcpy(singleopins[23].opcode, "1063");
  strcpy(singleopins[24].mnemonic, "SWAB");
  strcpy(singleopins[24].opcode, "0003");
  strcpy(singleopins[25].mnemonic, "SXT");
  strcpy(singleopins[25].opcode, "0067");
  strcpy(singleopins[26].mnemonic, "JMP");
  strcpy(singleopins[26].opcode, "0001");


  for(i=0; i<NUMDOUBLEOPINS; i++) {
    doubleopins[i].mnemonic = (char *)malloc(5);
    doubleopins[i].opcode = (char *)malloc(4);
    if((doubleopins[i].mnemonic == NULL)||(doubleopins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(doubleopins[0].mnemonic,  "MOV");
  strcpy(doubleopins[0].opcode,  "01");
  strcpy(doubleopins[1].mnemonic,  "MOVB");
  strcpy(doubleopins[1].opcode,  "11");
  strcpy(doubleopins[2].mnemonic,  "CMP");
  strcpy(doubleopins[2].opcode,  "02");
  strcpy(doubleopins[3].mnemonic,  "CMPB");
  strcpy(doubleopins[3].opcode,  "12");
  strcpy(doubleopins[4].mnemonic,  "BIT");
  strcpy(doubleopins[4].opcode,  "03");
  strcpy(doubleopins[5].mnemonic,  "BITB");
  strcpy(doubleopins[5].opcode,  "13");
  strcpy(doubleopins[6].mnemonic,  "BIC");
  strcpy(doubleopins[6].opcode,  "40");
  strcpy(doubleopins[7].mnemonic,  "BICB");
  strcpy(doubleopins[7].opcode,  "14");
  strcpy(doubleopins[8].mnemonic,  "BIS");
  strcpy(doubleopins[8].opcode,  "05");
  strcpy(doubleopins[9].mnemonic,  "BISB");
  strcpy(doubleopins[9].opcode,  "15");
  strcpy(doubleopins[10].mnemonic,  "ADD");
  strcpy(doubleopins[10].opcode,  "06");
  strcpy(doubleopins[11].mnemonic,  "SUB");
  strcpy(doubleopins[11].opcode,  "16");

  for(i=0; i<NUMBRANCHINS; i++) {
    branchins[i].mnemonic = (char *)malloc(5);
    branchins[i].opcode = (char *)malloc(7);
    if((branchins[i].mnemonic == NULL)||(branchins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(branchins[0].mnemonic, "BR");
  strcpy(branchins[0].opcode, "000400");
  strcpy(branchins[1].mnemonic, "BNE");
  strcpy(branchins[1].opcode, "001000");
  strcpy(branchins[2].mnemonic, "BEQ");
  strcpy(branchins[2].opcode, "001400");
  strcpy(branchins[3].mnemonic, "BPL");
  strcpy(branchins[3].opcode, "100000");
  strcpy(branchins[4].mnemonic, "BMI");
  strcpy(branchins[4].opcode, "100400");
  strcpy(branchins[5].mnemonic, "BVC");
  strcpy(branchins[5].opcode, "102000");
  strcpy(branchins[6].mnemonic, "BVS");
  strcpy(branchins[6].opcode, "102400");
  strcpy(branchins[7].mnemonic, "BCC");
  strcpy(branchins[7].opcode, "103000");
  strcpy(branchins[8].mnemonic, "BCS");
  strcpy(branchins[8].opcode, "103400");
  strcpy(branchins[9].mnemonic, "BGE");
  strcpy(branchins[9].opcode, "002000");
  strcpy(branchins[10].mnemonic, "BLT");
  strcpy(branchins[10].opcode, "002400");
  strcpy(branchins[11].mnemonic, "BGT");
  strcpy(branchins[11].opcode, "003000");
  strcpy(branchins[12].mnemonic, "BLE");
  strcpy(branchins[12].opcode, "003400");
  strcpy(branchins[13].mnemonic, "BHI");
  strcpy(branchins[13].opcode, "101000");
  strcpy(branchins[14].mnemonic, "BLOS");
  strcpy(branchins[14].opcode, "101400");
  strcpy(branchins[15].mnemonic, "BHIS");
  strcpy(branchins[15].opcode, "103000");
  strcpy(branchins[16].mnemonic, "BLO");
  strcpy(branchins[16].opcode, "103400");

  for(i=0; i<NUMJUMPINS; i++) {
    jumpins[i].mnemonic = (char *)malloc(4);
    jumpins[i].opcode = (char *)malloc(7);
    if((jumpins[i].mnemonic == NULL)||(jumpins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(jumpins[0].mnemonic, "JSR");
  strcpy(jumpins[0].opcode, "004");
  strcpy(jumpins[1].mnemonic, "RTS");
  strcpy(jumpins[1].opcode, "00020");
  strcpy(jumpins[2].mnemonic, "SOB");
  strcpy(jumpins[2].opcode, "077");
  strcpy(jumpins[3].mnemonic,  "XOR");
  strcpy(jumpins[3].opcode,  "074");

  for(i=0; i<NUMCONDITIONINS; i++) {
    conditionins[i].mnemonic = (char *)malloc(4);
    conditionins[i].opcode = (char *)malloc(7);
    if((conditionins[i].mnemonic == NULL)||(conditionins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(conditionins[0].mnemonic, "CLC");
  strcpy(conditionins[0].opcode, "000241");
  strcpy(conditionins[1].mnemonic, "CLV");
  strcpy(conditionins[1].opcode, "000242");
  strcpy(conditionins[2].mnemonic, "CLZ");
  strcpy(conditionins[2].opcode, "000244");
  strcpy(conditionins[3].mnemonic, "CLN");
  strcpy(conditionins[3].opcode, "000250");
  strcpy(conditionins[4].mnemonic, "CCC");
  strcpy(conditionins[4].opcode, "000257");
  strcpy(conditionins[5].mnemonic, "SEC");
  strcpy(conditionins[5].opcode, "000261");
  strcpy(conditionins[6].mnemonic, "SEV");
  strcpy(conditionins[6].opcode, "000262");
  strcpy(conditionins[7].mnemonic, "SEZ");
  strcpy(conditionins[7].opcode, "000264");
  strcpy(conditionins[8].mnemonic, "SEN");
  strcpy(conditionins[8].opcode, "000270");
  strcpy(conditionins[9].mnemonic, "SCC");
  strcpy(conditionins[9].opcode, "000277");

  for(i=0; i<NUMNOOPINS; i++) {
    noopins[i].mnemonic = (char *)malloc(5);
    noopins[i].opcode = (char *)malloc(7);
    if((noopins[i].mnemonic == NULL)||(noopins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(noopins[0].mnemonic, "HALT");
  strcpy(noopins[0].opcode, "000000");
  strcpy(noopins[1].mnemonic, "WAIT");
  strcpy(noopins[1].opcode, "000001");
  strcpy(noopins[2].mnemonic, "RTI");
  strcpy(noopins[2].opcode, "000002");
  strcpy(noopins[3].mnemonic, "RTT");
  strcpy(noopins[3].opcode, "000006");
  strcpy(noopins[4].mnemonic, "BPT");
  strcpy(noopins[4].opcode, "000003");
  strcpy(noopins[5].mnemonic, "IOT");
  strcpy(noopins[5].opcode, "000004");
  strcpy(noopins[6].mnemonic, "NOP");
  strcpy(noopins[6].opcode, "000240");

  for(i=0; i<NUMWEIRDINS; i++) {
    weirdins[i].mnemonic = (char *)malloc(5);
    weirdins[i].opcode = (char *)malloc(7);
    if((weirdins[i].mnemonic == NULL)||(weirdins[i].opcode == NULL)) {
      return 0;
    }
  }
  strcpy(weirdins[0].mnemonic,  "MUL");
  strcpy(weirdins[0].opcode,  "070");
  strcpy(weirdins[1].mnemonic,  "DIV");
  strcpy(weirdins[1].opcode,  "071");
  strcpy(weirdins[2].mnemonic,  "ASH");
  strcpy(weirdins[2].opcode,  "072");
  strcpy(weirdins[3].mnemonic,  "ASHC");
  strcpy(weirdins[3].opcode,  "073");
  strcpy(weirdins[4].mnemonic, "EMT");
  strcpy(weirdins[4].opcode, "104");
  strcpy(weirdins[5].mnemonic, "TRAP");
  strcpy(weirdins[5].opcode, "104");

  return 1;
}

/******************************************************************/

int isAddress(char *address) {
  int i, len;

  len = strlen(address);
  if((len == 0)||(len > 6)) {
    return 0;
  }
  if((len == 6)&&(address[0] != '0')&&(address[0] != '1')) {
    return 0;
  }
  for(i=1; i<len; i++) {
    if((address[i] > '7')||(address[i] < '0')) {
      return 0;
    }
  }

  return 1;
}

/******************************************************************/

int isInNumberRange(char *number) {
  int i,len;

  len = strlen(number);
  if((len == 0)||(len > 7)) {
    return 0;
  }
  if(number[0] == '-') {
    if(len == 7) {
      if((number[1] != '0')&&(number[1] != '1')) {
        return 0;
      }
      for(i=2; i<len; i++) {
        if((number[i] > '7')||(number[i] < '0')) {
          return 0;
        }
      }
      return 1;
    }else {
      for(i=1; i<len; i++) {
        if((number[i] > '7')||(number[i] < '0')) {
          return 0;
        }
      }
    }
  }else {
    if(len == 6) {
      if((number[0] != '0')&&(number[0] != '1')) {
        return 0;
      }
      for(i=1; i<len; i++) {
        if((number[i] > '7')||(number[i] < '0')) {
          return 0;
        }
      }
    }else {
      for(i=0; i<len; i++) {
        if((number[i] > '7')||(number[i] < '0')) {
          return 0;
        }
      }
    }
  }
  return 1;
}

/******************************************************************/

int isMnemonic(char *mnemonic) {
  int i = 0;

  while(mnemonic[i] != '\0') {
    mnemonic[i] = toupper(mnemonic[i]);
    i++;
  }
  for(i=0; i<NUMNOOPINS; i++) {
    if(!strcmp(mnemonic,noopins[i].mnemonic)) {
      return 1;
    }
  }
  for(i=0; i<NUMSINGLEOPINS; i++) {
    if(!strcmp(mnemonic,singleopins[i].mnemonic)) {
      return 2;
    }
  }
  for(i=0; i<NUMDOUBLEOPINS; i++) {
    if(!strcmp(mnemonic,doubleopins[i].mnemonic)) {
      return 3;
    }
  }
  for(i=0; i<NUMBRANCHINS; i++) {
    if(!strcmp(mnemonic,branchins[i].mnemonic)) {
      return 4;
    }
  }
  for(i=0; i<NUMJUMPINS; i++) {
    if(!strcmp(mnemonic,jumpins[i].mnemonic)) {
      return 5;
    }
  }
  for(i=0; i<NUMCONDITIONINS; i++) {
    if(!strcmp(mnemonic,conditionins[i].mnemonic)) {
      return 6;
    }
  }
  for(i=0; i<NUMWEIRDINS; i++) {
    if(!strcmp(mnemonic,weirdins[i].mnemonic)) {
      return 7;
    }
  }
  return 0;
}

/******************************************************************/

char *mnemonicToOpcode(char *mnemonic) {
  int i;

  for(i=0; i<NUMNOOPINS; i++) {
    if(!strcmp(mnemonic,noopins[i].mnemonic)) {
      return noopins[i].opcode;
    }
  }
  for(i=0; i<NUMSINGLEOPINS; i++) {
    if(!strcmp(mnemonic,singleopins[i].mnemonic)) {
      return singleopins[i].opcode;
    }
  }
  for(i=0; i<NUMDOUBLEOPINS; i++) {
    if(!strcmp(mnemonic,doubleopins[i].mnemonic)) {
      return doubleopins[i].opcode;
    }
  }
  for(i=0; i<NUMBRANCHINS; i++) {
    if(!strcmp(mnemonic,branchins[i].mnemonic)) {
      return branchins[i].opcode;
    }
  }
  for(i=0; i<NUMJUMPINS; i++) {
    if(!strcmp(mnemonic,jumpins[i].mnemonic)) {
      return jumpins[i].opcode;
    }
  }
  for(i=0; i<NUMCONDITIONINS; i++) {
    if(!strcmp(mnemonic,conditionins[i].mnemonic)) {
      return conditionins[i].opcode;
    }
  }
  for(i=0; i<NUMWEIRDINS; i++) {
    if(!strcmp(mnemonic,weirdins[i].mnemonic)) {
      return weirdins[i].opcode;
    }
  }


  return NULL;
}

/******************************************************************/

char *isRegOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 2) {
    return NULL;
  }
  if(((string[0] != 'R')&&(string[0] != 'r'))||(string[1] < '0')||(string[1] > '7')) {
    if(((string[0] != 's')&&(string[0] != 'S'))||((string[1] != 'p')&&(string[1] != 'P'))) {
      if(((string[0] != 'p')&&(string[0] != 'P'))||((string[1] != 'C')&&(string[1] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[1] == 'c')||(string[1] == 'C')) {
    return "07";
  }else if((string[1] == 'p')||(string[1] == 'P')) {
    return "06";
  }else if(string[1] == '0') {
    return "00";
  }else if(string[1] == '1') {
    return "01";
  }else if(string[1] == '2') {
    return "02";
  }else if(string[1] == '3') {
    return "03";
  }else if(string[1] == '4') {
    return "04";
  }else if(string[1] == '5') {
    return "05";
  }else if(string[1] == '6') {
    return "06";
  }else if(string[1] == '7') {
    return "07";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isRegDefOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 4){
    return NULL;
  }
  if((string[0] != '(')||((string[1] != 'R')&&(string[1] != 'r'))||(string[2] < '0')||(string[2] > '7')||(string[3] != ')')) {
    if(((string[1] != 's')&&(string[1] != 'S'))||((string[2] != 'p')&&(string[2] != 'P'))) {
      if(((string[1] != 'p')&&(string[1] != 'P'))||((string[2] != 'C')&&(string[2] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[2] == 'c')||(string[2] == 'C')) {
    return "17";
  }else if((string[2] == 'p')||(string[2] == 'P')) {
    return "16";
  }else if(string[2] == '0') {
    return "10";
  }else if(string[2] == '1') {
    return "11";
  }else if(string[2] == '2') {
    return "12";
  }else if(string[2] == '3') {
    return "13";
  }else if(string[2] == '4') {
    return "14";
  }else if(string[2] == '5') {
    return "15";
  }else if(string[2] == '6') {
    return "16";
  }else if(string[2] == '7') {
    return "17";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isAutoIncOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 5) {
    return NULL;
  }
  if((string[0] != '(')||((string[1] != 'R')&&(string[1] != 'r'))||(string[2] < '0')||(string[2] > '7')||(string[3] != ')')||(string[4] != '+')) {
    if(((string[1] != 's')&&(string[1] != 'S'))||((string[2] != 'p')&&(string[2] != 'P'))) {
      if(((string[1] != 'p')&&(string[1] != 'P'))||((string[2] != 'C')&&(string[2] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[2] == 'c')||(string[2] == 'C')) {
    return "27";
  }else if((string[2] == 'p')||(string[2] == 'P')) {
    return "26";
  }else if(string[2] == '0') {
    return "20";
  }else if(string[2] == '1') {
    return "21";
  }else if(string[2] == '2') {
    return "22";
  }else if(string[2] == '3') {
    return "23";
  }else if(string[2] == '4') {
    return "24";
  }else if(string[2] == '5') {
    return "25";
  }else if(string[2] == '6') {
    return "26";
  }else if(string[2] == '7') {
    return "27";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isAutoIncDefOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 6) {
    return NULL;
  }
  if((string[0] != '@')||(string[1] != '(')||((string[2] != 'R')&&(string[2] != 'r'))||(string[3] < '0')||(string[3] > '7')||(string[4] != ')')||(string[5] != '+')) {
    if(((string[2] != 's')&&(string[2] != 'S'))||((string[3] != 'p')&&(string[3] != 'P'))) {
      if(((string[2] != 'p')&&(string[2] != 'P'))||((string[3] != 'C')&&(string[3] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[3] == 'c')||(string[3] == 'C')) {
    return "37";
  }else if((string[3] == 'p')||(string[3] == 'P')) {
    return "36";
  }else if(string[3] == '0') {
    return "30";
  }else if(string[3] == '1') {
    return "31";
  }else if(string[3] == '2') {
    return "32";
  }else if(string[3] == '3') {
    return "33";
  }else if(string[3] == '4') {
    return "34";
  }else if(string[3] == '5') {
    return "35";
  }else if(string[3] == '6') {
    return "36";
  }else if(string[3] == '7') {
    return "37";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isAutoDecOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 5) {
    return NULL;
  }
  if((string[0] != '-')||(string[1] != '(')||((string[2] != 'R')&&(string[2] != 'r'))||(string[3] < '0')||(string[3] > '7')||(string[4] != ')')) {
    if(((string[2] != 's')&&(string[2] != 'S'))||((string[3] != 'p')&&(string[3] != 'P'))) {
      if(((string[2] != 'p')&&(string[2] != 'P'))||((string[3] != 'C')&&(string[3] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[3] == 'c')||(string[3] == 'C')) {
    return "47";
  }else if((string[3] == 'p')||(string[3] == 'P')) {
    return "46";
  }else if(string[3] == '0') {
    return "40";
  }else if(string[3] == '1') {
    return "41";
  }else if(string[3] == '2') {
    return "42";
  }else if(string[3] == '3') {
    return "43";
  }else if(string[3] == '4') {
    return "44";
  }else if(string[3] == '5') {
    return "45";
  }else if(string[3] == '6') {
    return "46";
  }else if(string[3] == '7') {
    return "47";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isAutoDecDefOp(char *string) {
  int len;

  len = strlen(string);
  if(len != 6) {
    return NULL;
  }
  if((string[0] != '@')||(string[1] != '-')||(string[2] != '(')||((string[3] != 'R')&&(string[3] != 'r'))||(string[3] < '0')||(string[4] > '7')||(string[5] != ')')) {
    if(((string[3] != 's')&&(string[3] != 'S'))||((string[4] != 'p')&&(string[4] != 'P'))) {
      if(((string[3] != 'p')&&(string[3] != 'P'))||((string[4] != 'C')&&(string[4] != 'c'))) {
        return NULL;
      }
    }
  }
  if((string[4] == 'c')||(string[4] == 'C')) {
    return "57";
  }else if((string[4] == 'p')||(string[4] == 'P')) {
    return "56";
  }else if(string[4] == '0') {
    return "50";
  }else if(string[4] == '1') {
    return "51";
  }else if(string[4] == '2') {
    return "52";
  }else if(string[4] == '3') {
    return "53";
  }else if(string[4] == '4') {
    return "54";
  }else if(string[4] == '5') {
    return "55";
  }else if(string[4] == '6') {
    return "56";
  }else if(string[4] == '7') {
    return "57";
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isIndexOp(char *string) {
  int len;
  int i;
  int j;
  char number[7];
  char newnumber[7];
  char *opcode;

  len = strlen(string);
  if(len > 11) {
    return NULL;
  }
  i = 0;
  j = 0;
  while(((string[i] >= '0')&&(string[i] <= '7')) || (string[i] == '-')) {
    number[j] = string[i];
    j++;
    i++;
  }
  number[j] = '\0';
  if((isInNumberRange(number))== 0) {
    return NULL;
  }
  if((string[i] != '(')||((string[i+1] != 'R')&&(string[i+1] != 'r'))||(string[i+2] < '0')||(string[i+2] > '7')||(string[i+3] != ')')) {
    if(((string[i+1] != 's')&&(string[i+1] != 'S'))||((string[i+2] != 'p')&&(string[i+2] != 'P'))) {
      if(((string[i+1] != 'p')&&(string[i+1] != 'P'))||((string[i+2] != 'C')&&(string[i+2] != 'c'))) {
        return NULL;
      }
    }
  }
  if(string[i+4] != '\0') {
    return NULL;
  }
  for(j=0; j<6-strlen(number); j++) {
    newnumber[j] = '0';
  }
  newnumber[j] = '\0';
  len = strlen(newnumber);
  for(j=len; j<7; j++) {
    newnumber[j] = number[j-len];
  }
  newnumber[j] = '\0';
  opcode = (char *)malloc(sizeof(newnumber)+3);
  if((string[i+2] == 'c')||(string[i+2] == 'C')) {
    strcpy(opcode, "67");
    strcat(opcode, newnumber);
    return opcode;
  }else if((string[i+2] == 'p')||(string[i+2] == 'P')) {
    strcpy(opcode, "66");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '0') {
    strcpy(opcode, "60");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '1') {
    strcpy(opcode, "61");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '2') {
    strcpy(opcode, "62");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '3') {
    strcpy(opcode, "63");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '4') {
    strcpy(opcode, "64");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '5') {
    strcpy(opcode, "65");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '6') {
    strcpy(opcode, "66");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '7') {
    strcpy(opcode, "67");
    strcat(opcode, newnumber);
    return opcode;
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isIndexDefOp(char *string) {
  int len;
  int i,j;
  char number[7];
  char newnumber[7];
  char *opcode;

  len = strlen(string);
  if(len > 12) {
    return NULL;
  }
  if(string[0] != '@') {
    return NULL;
  }
  i = 1;
  j = 0;
  while(((string[i] >= '0')&&(string[i] <= '7')) || (string[i] == '-')) {
    number[j] = string[i];
    j++;
    i++;
  }
  number[j] = '\0';
  if(!isInNumberRange(number)) {
    return NULL;
  }
  if((string[i] != '(')||((string[i+1] != 'R')&&(string[i+1] != 'r'))||(string[i+2] < '0')||(string[i+2] > '7')||(string[i+3] != ')')) {
    if(((string[i+1] != 's')&&(string[i+1] != 'S'))||((string[i+2] != 'p')&&(string[i+2] != 'P'))) {
      if(((string[i+1] != 'p')&&(string[i+1] != 'P'))||((string[i+2] != 'C')&&(string[i+2] != 'c'))) {
        return NULL;
      }
    }
  }
  if(string[i+4] != '\0') {
    return NULL;
  }
  for(j=0; j<6-strlen(number); j++) {
    newnumber[j] = '0';
  }
  newnumber[j] = '\0';
  len = strlen(newnumber);
  for(j=len; j<7; j++) {
    newnumber[j] = number[j-len];
  }
  newnumber[j] = '\0';
  opcode = (char *)malloc(sizeof(newnumber)+3);
  if((string[i+2] == 'c')||(string[i+2] == 'C')) {
    strcpy(opcode, "77");
    strcat(opcode, newnumber);
    return opcode;
  }else if((string[i+2] == 'p')||(string[i+2] == 'P')) {
    strcpy(opcode, "76");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '0') {
    strcpy(opcode, "70");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '1') {
    strcpy(opcode, "71");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '2') {
    strcpy(opcode, "72");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '3') {
    strcpy(opcode, "73");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '4') {
    strcpy(opcode, "74");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '5') {
    strcpy(opcode, "75");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '6') {
    strcpy(opcode, "76");
    strcat(opcode, newnumber);
    return opcode;
  }else if(string[i+2] == '7') {
    strcpy(opcode, "77");
    strcat(opcode, newnumber);
    return opcode;
  }else {
    printf("flyers suck cock!\n");
    return NULL;
  }
}

/******************************************************************/

char *isImmediateOp(char *string) {
  int i,j,len;
  char number[7];
  char newnumber[7];
  char *opcode;

  len = strlen(string);
  if(string[0] != '#') {
    return NULL;
  }
  j = 0;
  for(i=1; i<len; i++) {
    number[j] = string[i];
    j++;
  }
  number[j] = '\0';
  if(isInNumberRange(number)) {
    for(i=0; i<6-strlen(number); i++) {
      newnumber[i] = '0';
    }
    newnumber[i] = '\0';
    len = strlen(newnumber);
    for(i=len; i<7; i++) {
      newnumber[i] = number[i-len];
    }
    newnumber[i] = '\0';
    opcode = (char *)malloc(sizeof(newnumber)+3);
    strcpy(opcode, "27");
    strcat(opcode, newnumber);
    return opcode;
  }else {
    return NULL;
  }
}

/******************************************************************/

char *isAbsoluteOp(char *string) {
  int len;
  int i,j;
  char address[7];
  char *opcode;

  len = strlen(string);
  if(len != 8) {
    return NULL;
  }
  if((string[0] != '@')||(string[1] != '#')) {
    return NULL;
  }
  j = 0;
  for(i=2; i<len; i++) {
    address[j] = string[i];
    j++;
  }
  address[j] = '\0';
  if(isAddress(address)) {
    opcode = (char *)malloc(sizeof(address) + 3);
    strcpy(opcode, "37");
    strcat(opcode, address);
    return opcode;
  }else {
    return NULL;
  }
}

/******************************************************************/

char *isRelativeOp(char *string) {
  int len;
  char *opcode;

  len = strlen(string);
  if(len != 6) {
    return NULL;
  }
  if(isAddress(string)) {
    opcode = (char *)malloc(sizeof(string) + 3);
    strcpy(opcode, "67");
    strcat(opcode, string);
    return opcode;
  }else {
    return NULL;
  }
}

/******************************************************************/

char *isRelDefOp(char *string) {
  int len;
  int i,j;
  char address[7];
  char *opcode;

  len = strlen(string);
  if(len != 7) {
    return NULL;
  }
  if(string[0] != '@') {
    return NULL;
  }
  j = 0;
  for(i=1; i<len; i++) {
    address[j] = string[i];
    j++;
  }
  address[j] = '\0';
  if(isAddress(address)) {
    opcode = (char *)malloc(sizeof(address) + 3);
    strcpy(opcode, "77");
    strcat(opcode, address);
    return opcode;
  }else {
    return NULL;
  }
}

/******************************************************************/

char *isOperand(char *operand) {
  int len;

  if((operand[0] == 'R')||(operand[0] == 'r')||(operand[0] == 's')||
    (operand[0] == 'S')||(operand[0] == 'P')||(operand[0] == 'p')) {
    return isRegOp(operand);
  }else if(operand[0] == '(') {
    len = strlen(operand);
    if(operand[len-1] == '+') {
      return isAutoIncOp(operand);
    }else {
      return isRegDefOp(operand);
    }
  }else if(operand[0] == '@') {
    if(operand[1] == '(') {
      return isAutoIncDefOp(operand);
    }else if(operand[1] == '-') {
      return isAutoDecDefOp(operand);
    }else if(operand[1] == '#') {
      return isAbsoluteOp(operand);
    }else if((operand[1] <= '7')&&(operand[1] >= '0')) {
      len = strlen(operand);
      if(operand[len-1] == ')') {
        return isIndexDefOp(operand);
      }else {
        return isRelDefOp(operand);
      }
    }else {
      return NULL;
    }
  }else if(operand[0] == '-') {
    if(operand[1] == '(') {
      return isAutoDecOp(operand);
    }else {
      return isIndexOp(operand);
    }
  }else if((operand[0] >= '0')&&(operand[0] <= '7')) {
    len = strlen(operand);
    if(operand[len-1] == ')') {
      return isIndexOp(operand);
    }else {
      return isRelativeOp(operand);
    }
  }else if(operand[0] == '#') {
    return isImmediateOp(operand);
  }else {
    return NULL;
  }
}

/******************************************************************/

int mnemonicTypeOne(char *mnemonic){
  char *mnemonicOpCode;

  mnemonicOpCode = mnemonicToOpcode(mnemonic);
  binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) +1);
  if(binarray[binarrayindex] == NULL) {
    printf("error allocating memory, pdpasm terminated.\n");
    return 0;
  }
  strcpy(binarray[binarrayindex], mnemonicOpCode);
  binarrayindex++;
  mapoffset++;
  return 1;
}

/******************************************************************/

int mnemonicTypeTwo(char *operand, char *mnemonic, int linenum){
  char newoperand[MAXOPERANDLEN];
  int i, j, len, state;
  char *operandOpCode, *mnemonicOpCode;
  char operandPartOne[3];
  char operandPartTwo[7];

  state = 0;
  j = 0;
  len = strlen(operand);

 /* this for loop goes through the operand passed to it and geats rid of any ws */
  for(i=0; i<len; i++) {
    if(state == 0) {
      if((operand[i] == ' ')||(operand[i] == '\t')){
        state = 1;
        continue;
      }else{
        state = 0;
        newoperand[j] = operand[i];
        j++;
        continue;
      }
    }
    if(state == 1) {
      if((operand[i] == ' ')||(operand[i] == '\t')){
        state = 1;
        continue;
      }else{
        printf("error in operand: [%s] for mnemonic: [%s] on line# %i\n",operand,mnemonic,linenum);
        return 0;
      }
    }
  }
  newoperand[j] = '\0';
  operandOpCode = isOperand(newoperand);
  if(operandOpCode != NULL) {
    mnemonicOpCode = mnemonicToOpcode(mnemonic);
/* if operandOpCode if >2 then we know we have an instruction that requires more than one word
  so we break it up into to parts and write the first part with the mnemonic in the binarray
  and the second part in the binarray in its own cell */
    if(strlen(operandOpCode) > 2) {
      operandPartOne[0] = operandOpCode[0];
      operandPartOne[1] = operandOpCode[1];
      operandPartOne[2] = '\0';
      for(i=2; i<8; i++) {
        operandPartTwo[i-2] = operandOpCode[i];
      }
      operandPartTwo[6] = '\0';
      binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandPartOne)+ 1);
      if(binarray[binarrayindex] == NULL) {
        printf("error allocating memory, pdpasm terminated\n");
        return 0;
      }
      strcpy(binarray[binarrayindex], mnemonicOpCode);
      strcat(binarray[binarrayindex], operandPartOne);
      binarrayindex++;
         mapoffset++;
      binarray[binarrayindex] = (char *)malloc(sizeof(operandPartTwo)+1);
      if(binarray[binarrayindex] == NULL) {
        printf("error allocating memory, pdpasm terminated\n");
        return 0;
      }
      strcpy(binarray[binarrayindex], operandPartTwo);
      binarrayindex++;
      mapoffset++;
      return 1;
    } else {
/* we only have one thing to write: the mnemonic concated with it's operand */
      binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandOpCode)+ 1);
      if(binarray[binarrayindex] == NULL) {
        printf("error allocating memory, pdpasm terminated\n");
        return 0;
      }
      strcpy(binarray[binarrayindex], mnemonicOpCode);
      strcat(binarray[binarrayindex], operandOpCode);
      binarrayindex++;
      mapoffset++;
      return 1;
    }
  }else {
    printf("error in operand: [%s] for mnemonic: [%s] on line# %i\n",newoperand,mnemonic,linenum);
    return 0;
  }
}

/******************************************************************/

int mnemonicTypeThree(char *operand, char *mnemonic, int linenum){
  char operandOne[MAXOPERANDLEN];
  char operandTwo[MAXOPERANDLEN];
  int i, j, k, len, state;
  char *operandOpCodeOne, *operandOpCodeTwo, *mnemonicOpCode;
  char sOperandPartOne[3];
  char sOperandPartTwo[7];
  char dOperandPartOne[3];
  char dOperandPartTwo[7];

  state = 0;
  j = 0;
  k = 0;
  len = strlen(operand);
/* this for loop goes through the operand passed to it and geats rid of any ws */
  for(i=0; i<len; i++) {
    if(state == 0){
      if(operand[i] == ',') {
        state = 2;
        continue;
      }else if((operand[i] == ' ')||(operand[i] == '\t')){
        state = 1;
        continue;
      }else {
        state = 0;
        operandOne[j] = operand[i];
        j++;
        continue;
      }
    }
    if(state == 1) {
      if((operand[i] == ' ')||(operand[i] == '\t')) {
        state = 1;
        continue;
      }else if(operand[i] == ',') {
        state = 2;
        continue;
      }else {
        printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
        return 0;
      }
    }
    if(state == 2) {
      if((operand[i] == ' ')||(operand[i] == '\t')) {
        state = 2;
        continue;
      }else {
        state = 3;
        operandTwo[k] = operand[i];
        k++;
        continue;
      }
    }
    if(state == 3) {
      if((operand[i] == ' ')||(operand[i] == '\t')) {
        state = 4;
        continue;
      }else {
        state = 3;
        operandTwo[k] = operand[i];
        k++;
        continue;
      }
    }
    if(state == 4) {
      if((operand[i] == ' ')||(operand[i] == '\t')) {
        state = 4;
        continue;
      }else {
        printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
        return 0;
      }
    }
  }
  operandOne[j] = '\0';
  operandTwo[k] = '\0';
  operandOpCodeOne = isOperand(operandOne);
  operandOpCodeTwo = isOperand(operandTwo);
  if(operandOpCodeOne == NULL) {
    printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
  }
  if(operandOpCodeTwo == NULL) {
    printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
  }
  if((operandOpCodeOne != NULL)&&(operandOpCodeTwo != NULL)) {
    mnemonicOpCode = mnemonicToOpcode(mnemonic);
    if(strlen(operandOpCodeOne) > 2) {
      if(strlen(operandOpCodeTwo) > 2) {
/*this is where both the source and the destination require another word for their adressing mode*/
        sOperandPartOne[0] = operandOpCodeOne[0];
        sOperandPartOne[1] = operandOpCodeOne[1];
        sOperandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          sOperandPartTwo[i-2] = operandOpCodeOne[i];
        }
        sOperandPartTwo[6] = '\0';
        dOperandPartOne[0] = operandOpCodeTwo[0];
        dOperandPartOne[1] = operandOpCodeTwo[1];
        dOperandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          dOperandPartTwo[i-2] = operandOpCodeTwo[i];
        }
        dOperandPartTwo[6] = '\0';
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(sOperandPartOne) + sizeof(dOperandPartOne) + 1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], sOperandPartOne);
        strcat(binarray[binarrayindex], dOperandPartOne);
        binarrayindex++;
            mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(sOperandPartTwo) +1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], sOperandPartTwo);
        binarrayindex++;
            mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(dOperandPartTwo) +1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], dOperandPartTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }else {
/*this is where only the source requires another word for its addressing mode*/
        sOperandPartOne[0] = operandOpCodeOne[0];
        sOperandPartOne[1] = operandOpCodeOne[1];
        sOperandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          sOperandPartTwo[i-2] = operandOpCodeOne[i];
        }
        sOperandPartTwo[6] = '\0';
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(sOperandPartOne) + sizeof(operandOpCodeTwo) + 1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], sOperandPartOne);
        strcat(binarray[binarrayindex], operandOpCodeTwo);
        binarrayindex++;
        mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(sOperandPartTwo) +1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], sOperandPartTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }
    }else {
      if(strlen(operandOpCodeTwo) > 2) {
/*this is where only the destination requires another word for its addressing mode*/
        dOperandPartOne[0] = operandOpCodeTwo[0];
        dOperandPartOne[1] = operandOpCodeTwo[1];
        dOperandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          dOperandPartTwo[i-2] = operandOpCodeTwo[i];
        }
        dOperandPartTwo[6] = '\0';
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(dOperandPartOne) + sizeof(operandOpCodeOne) + 1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], operandOpCodeOne);
        strcat(binarray[binarrayindex], dOperandPartOne);
        binarrayindex++;
            mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(dOperandPartTwo) +1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], dOperandPartTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }else {
/*this is where there is no need for any more words for the instruction*/
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandOpCodeOne) + sizeof(operandOpCodeTwo) + 1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], operandOpCodeOne);
        strcat(binarray[binarrayindex], operandOpCodeTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }
    }
  }else {
    return 0;
  }
}

/******************************************************************/

int mnemonicTypeFour(char *operand, char *mnemonic, int linenum){
  int i, len;
  char *mnemonicOpCode;

  len = strlen(operand);
  for(i=0; i<len; i++) {
    operand[i] = toupper(operand[i]);
  }
  for(i=0; i<len; i++) {
    if((operand[i] < 'A')||(operand[i] > 'Z')) {
      if(!isAddress(operand)) {
        printf("error in operand: [%s] for mnemonic: [%s] on line# %i\n",operand,mnemonic,linenum);
        return 0;
      }
    }
  }
  mnemonicOpCode = mnemonicToOpcode(mnemonic);
  binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + 1);
  if(binarray[binarrayindex] == NULL) {
    printf("error allocating memory, pdpasm terminated\n");
    return 0;
  }
  strcpy(binarray[binarrayindex], mnemonicOpCode);
  listTwo[listTwoPtr].lable = (char *)malloc(strlen(operand)+1);
  strcpy(listTwo[listTwoPtr].lable, operand);
  listTwo[listTwoPtr].index = binarrayindex;
  listTwoPtr++;
  binarrayindex++;
  mapoffset++;
  return 1;
}

/******************************************************************/

int mnemonicTypeFive(char *operand, char *mnemonic, int linenum){
  char operandOne[MAXOPERANDLEN];
  char operandTwo[MAXOPERANDLEN];
  int i, j, k, len;
  char *operandOpCode, *operandOpCodeOne, *operandOpCodeTwo;
  char *mnemonicOpCode;
  char operandPartOne[3];
  char operandPartTwo[7];
  char regNum[2];
  int state;

  if((strcmp(mnemonic,"JSR") == 0)||(strcmp(mnemonic,"XOR") == 0)) {
    state = 0;
    j = 0;
    k = 0;
    len = strlen(operand);
    for(i=0; i<len; i++) {
      if(state == 0){
        if(operand[i] == ',') {
          state = 2;
          continue;
        }else if((operand[i] == ' ')||(operand[i] == '\t')){
          state = 1;
          continue;
        }else {
          state = 0;
          operandOne[j] = operand[i];
          j++;
          continue;
        }
      }
      if(state == 1) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 1;
          continue;
        }else if(operand[i] == ',') {
          state = 2;
          continue;
        }else {
          printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
          return 0;
        }
      }
      if(state == 2) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 2;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 3) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 4) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
          return 0;
        }
      }
    }
    operandOne[j] = '\0';
    operandTwo[k] = '\0';
    operandOpCodeOne = isRegOp(operandOne);
    operandOpCodeTwo = isOperand(operandTwo);
    if(operandOpCodeOne == NULL) {
      printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
    }
    if(operandOpCodeTwo == NULL) {
      printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
    }
    if((operandOpCodeOne != NULL)&&(operandOpCodeTwo != NULL)) {
      mnemonicOpCode = mnemonicToOpcode(mnemonic);
      if(strlen(operandOpCodeTwo) > 2) {
        operandPartOne[0] = operandOpCodeTwo[0];
        operandPartOne[1] = operandOpCodeTwo[1];
        operandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          operandPartTwo[i-2] = operandOpCodeTwo[i];
        }
        operandPartTwo[6] = '\0';
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandPartOne)+ 2);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        regNum[0] = operandOpCodeOne[1];
        regNum[1] = '\0';
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], regNum);
        strcat(binarray[binarrayindex], operandPartOne);
        binarrayindex++;
            mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(operandPartTwo)+1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], operandPartTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }else {
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandOpCodeTwo) + 2);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        regNum[0] = operandOpCodeOne[1];
        regNum[1] = '\0';
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], regNum);
        strcat(binarray[binarrayindex], operandOpCodeTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }
    }else {
      return 0;
    }
  }else if(strcmp(mnemonic,"RTS") == 0) {
    state = 0;
    j = 0;
    len = strlen(operand);
    for(i=0; i<len; i++) {
      if(state == 0) {
        if((operand[i] == ' ')||(operand[i] == '\t')){
          state = 1;
          continue;
        }else{
          state = 0;
          operandOne[j] = operand[i];
          j++;
          continue;
        }
      }
      if(state == 1) {
        if((operand[i] == ' ')||(operand[i] == '\t')){
          state = 1;
          continue;
        }else{
          printf("error in operand: [%s] for mnemonic: [%s] on line# %i\n",operand,mnemonic,linenum);
          return 0;
        }
      }
    }
    operandOne[j] = '\0';
    operandOpCode = isRegOp(operandOne);
    if(operandOpCode != NULL){
      mnemonicOpCode = mnemonicToOpcode(mnemonic);
      binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + 2);
      if(binarray[binarrayindex] == NULL) {
        printf("error allocating memory, pdpasm terminated\n");
        return 0;
      }
      regNum[0] = operandOpCode[1];
      regNum[1] = '\0';
      strcpy(binarray[binarrayindex], mnemonicOpCode);
      strcat(binarray[binarrayindex], regNum);
      binarrayindex++;
      mapoffset++;
      return 1;
    }else {
      printf("error in operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
      return 0;
    }
  }else if(strcmp(mnemonic,"SOB") == 0) {
    state = 0;
    j = 0;
    k = 0;
    len = strlen(operand);
    for(i=0; i<len; i++) {
      if(state == 0){
        if(operand[i] == ',') {
          state = 2;
          continue;
        }else if((operand[i] == ' ')||(operand[i] == '\t')){
          state = 1;
          continue;
        }else {
          state = 0;
          operandOne[j] = operand[i];
          j++;
          continue;
        }
      }
      if(state == 1) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 1;
          continue;
        }else if(operand[i] == ',') {
          state = 2;
          continue;
        }else {
          printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
          return 0;
        }
      }
      if(state == 2) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 2;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 3) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 4) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
          return 0;
        }
      }
    }
    operandOne[j] = '\0';
    operandTwo[k] = '\0';
    operandOpCodeOne = isRegOp(operandOne);
    for(i=0; i<strlen(operandTwo); i++) {
      operandTwo[i] = toupper(operandTwo[i]);
    }
    for(i=0; i<strlen(operandTwo); i++) {
      if((operandTwo[i] < 'A')||(operandTwo[i] > 'Z')) {
        printf("error in second operand: [%s] for mnemonic: [%s] on line# %i, invalid lable name\n",operandTwo,mnemonic,linenum);
        return 0;
      }
    }
    listTwo[listTwoPtr].lable = (char *)malloc(strlen(operandTwo) +1);
    strcpy(listTwo[listTwoPtr].lable, operandTwo);
    listTwo[listTwoPtr].index = binarrayindex;
    listTwoPtr++;
    if(operandOpCodeOne == NULL) {
      printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
    }
    if((operandOpCodeOne != NULL)&&(operandOpCodeTwo != NULL)) {
      mnemonicOpCode = mnemonicToOpcode(mnemonic);
      binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandOpCodeTwo) + 2);
      if(binarray[binarrayindex] == NULL) {
        printf("error allocating memory, pdpasm terminated\n");
        return 0;
      }
      regNum[0] = operandOpCodeOne[1];
      regNum[1] = '\0';
      strcpy(binarray[binarrayindex], mnemonicOpCode);
      strcat(binarray[binarrayindex], regNum);
      strcat(binarray[binarrayindex], "00");
      binarrayindex++;
      mapoffset++;
      return 1;
    }else {
      return 0;
    }
  }else {
    printf("Flyers Suck\n");
    return 0;
  }
}

/******************************************************************/

int mnemonicTypeSix(char *mnemonic){
  char *mnemonicopcode;

  mnemonicopcode = mnemonicToOpcode(mnemonic);
  binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicopcode) +1);
  if(binarray[binarrayindex] == NULL) {
    printf("error allocating memory, pdpasm terminated\n");
    return 0;
  }
  strcpy(binarray[binarrayindex], mnemonicopcode);
  binarrayindex++;
   mapoffset++;
  return 1;
}

/******************************************************************/

int mnemonicTypeSeven(char *operand, char *mnemonic, int linenum){
  char operandOne[MAXOPERANDLEN];
  char operandTwo[MAXOPERANDLEN];
  int i, j, k, len, state;
  char *operandOpCodeOne, *operandOpCodeTwo, *mnemonicOpCode;
  char operandPartOne[3];
  char operandPartTwo[7];
  char regNum[2];

  if((strcmp(mnemonic, "EMT") == 0)||(strcmp(mnemonic, "TRAP") == 0)) {
    return 1;
  }else{
    state = 0;
    j = 0;
    k = 0;
    len = strlen(operand);
/* this for loop goes through the operand passed to it and geats rid of any ws */
    for(i=0; i<len; i++) {
      if(state == 0){
        if(operand[i] == ',') {
          state = 2;
          continue;
        }else if((operand[i] == ' ')||(operand[i] == '\t')){
          state = 1;
          continue;
        }else {
          state = 0;
          operandOne[j] = operand[i];
          j++;
          continue;
        }
      }
      if(state == 1) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 1;
          continue;
        }else if(operand[i] == ',') {
          state = 2;
          continue;
        }else {
          printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
          return 0;
        }
      }
      if(state == 2) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 2;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 3) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          state = 3;
          operandTwo[k] = operand[i];
          k++;
          continue;
        }
      }
      if(state == 4) {
        if((operand[i] == ' ')||(operand[i] == '\t')) {
          state = 4;
          continue;
        }else {
          printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
          return 0;
        }
      }
    }
    operandOne[j] = '\0';
    operandTwo[k] = '\0';
    operandOpCodeOne = isOperand(operandOne);
    operandOpCodeTwo = isRegOp(operandTwo);
    if(operandOpCodeOne == NULL) {
      printf("error in first operand: [%s] for mnemonic: [%s] on line# %i\n",operandOne,mnemonic,linenum);
    }
    if(operandOpCodeTwo == NULL) {
      printf("error in second operand: [%s] for mnemonic: [%s] on line# %i\n",operandTwo,mnemonic,linenum);
    }
    if((operandOpCodeOne != NULL)&&(operandOpCodeTwo != NULL)) {
      mnemonicOpCode = mnemonicToOpcode(mnemonic);
      if(strlen(operandOpCodeOne) > 2) {
        operandPartOne[0] = operandOpCodeOne[0];
        operandPartOne[1] = operandOpCodeOne[1];
        operandPartOne[2] = '\0';
        for(i=2; i<8; i++) {
          operandPartTwo[i-2] = operandOpCodeOne[i];
        }
        operandPartTwo[6] = '\0';
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + sizeof(operandPartOne)+ 2);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        regNum[0] = operandOpCodeTwo[1];
        regNum[1] = '\0';
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], regNum);
        strcat(binarray[binarrayindex], operandPartOne);
        binarrayindex++;
        mapoffset++;
        binarray[binarrayindex] = (char *)malloc(sizeof(operandPartTwo)+1);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        strcpy(binarray[binarrayindex], operandPartTwo);
        binarrayindex++;
        mapoffset++;
        return 1;
      }else {
        binarray[binarrayindex] = (char *)malloc(sizeof(mnemonicOpCode) + 2);
        if(binarray[binarrayindex] == NULL) {
          printf("error allocating memory, pdpasm terminated\n");
          return 0;
        }
        regNum[0] = operandOpCodeTwo[1];
        regNum[1] = '\0';
        strcpy(binarray[binarrayindex], mnemonicOpCode);
        strcat(binarray[binarrayindex], regNum);
        strcat(binarray[binarrayindex], operandOpCodeOne);
        binarrayindex++;
        mapoffset++;
        return 1;
      }
    }else {
      return 0;
    }
  }
}

/******************************************************************/

int dispatchOperand(char *operand, char *mnemonic, int mnemonicType, int linenum) {

  if(mnemonicType == 1) {
    return mnemonicTypeOne(mnemonic);
  }
  else if(mnemonicType == 2) {
    return mnemonicTypeTwo(operand, mnemonic, linenum);
  }
  else if(mnemonicType == 3) {
    return mnemonicTypeThree(operand, mnemonic, linenum);
  }
  else if(mnemonicType == 4) {
    return mnemonicTypeFour(operand, mnemonic, linenum);
  }
  else if(mnemonicType == 5) {
    return mnemonicTypeFive(operand, mnemonic, linenum);
  }
  else if(mnemonicType == 6) {
    return mnemonicTypeSix(mnemonic);
  }
  else if(mnemonicType == 7) {
    return mnemonicTypeSeven(operand, mnemonic, linenum);
  }
  else {
    printf("flyer's suck!\n");
    return 0;
  }
}

/******************************************************************/

int newLable(char *string) {
  int i;

  for(i=0; i<listOnePtr; i++) {
    if(strcmp(listOne[i].lable, string) == 0) {
      return 0;
    }
  }
  return 1;
}

/******************************************************************/

char *fixLine(char *string) {
  int state, ptr,i;
  char newstring[MAXLINELENGTH];
  char *returnString;

  state = 0;
  ptr = 0;
  for(i=0; i<strlen(string); i++) {
    if(state == 0) {
      if((string[i] == ' ')||(string[i] == '\t')) {
        state = 0;
        continue;
      }else {
        newstring[ptr] = string[i];
        ptr ++;
        state = 1;
        continue;
      }
    }
    else if(state == 1) {
      if(string[i] == '\n') {
        break;
      }else if(string[i] == '\0') {
        break;
      }else {
        state = 1;
        newstring[ptr] = string[i];
        ptr++;
        continue;
      }
    }
  }
  newstring[ptr] = '\0';
  returnString = (char *)malloc(strlen(newstring)+1);
  strcpy(returnString,newstring);
  return returnString;
}

/******************************************************************/

int doBranches(void) {
  int i,j;
  int from, to, offset;
  int returncode;
  int branchFrom, branchTo;
  char secondBit;
  char string[7];

  returncode = 1;
  for(i=0; i<listTwoPtr; i++) {
    branchFrom = -1;
    branchTo = -1;
    for(j=0; j<listOnePtr; j++) {
      if(strcmp(listTwo[i].lable,listOne[j].lable) == 0) {
        from = listTwo[i].index;
        to = listOne[j].index;
        break;
      }
    }
    if((from == -1)||(to == -1)) {
      printf("branch lable: [%s] does not correspond to any lables to branch to\n",listTwo[i].index);
      returncode = 0;
    }else {
    offset = to - from - 1; 
    if((offset < -128)||(offset > 127)) {
      printf("There is a branch that is out of range (greater than 127 words)\n");
      returncode = 0;
    }else {
      sscanf(binarray[from], "%o", &branchFrom);
      if (offset < 0) {
        strcpy(string,binarray[from]);
        secondBit = string[1];
        if(secondBit == '0') {
          offset += 256;
        }else {
          offset += 64;
        }
      }
      branchFrom = branchFrom + offset;
      sprintf(binarray[from], "%6.6o", branchFrom);
    }
   }
  }
  return returncode;
}

/******************************************************************/

int parseFilePassOne(FILE *fin) {
  char line[MAXLINELENGTH];
  char lable[MAXLABLELENGTH];
  int i, linenum;
  int state;
  int mnemonicptr;
  int lableptr;
  int mnereturn;
  char mnemonic[5];
  int opreturn, operandptr;
  char operand[MAXOPERANDLEN];
  int returncode;
  int somethingtowrite;

  returncode = 1;
  linenum = 0;
  state = 0;
  somethingtowrite = 0;
  while(fgets(line, MAXLINELENGTH, fin)) {
    linenum++;
    mnemonicptr = 0;
    operandptr = 0;
    somethingtowrite = 0;
    lableptr = 0;
    for(i=0; i<MAXLINELENGTH; i++) {
      if(state == 0) {
        if(line[i] == ';') {
          state = 1;
          continue;
        }else if(((line[i] >= 'a')&&(line[i] <= 'z'))||((line[i] >= 'A')&&(line[i] <= 'Z'))) {
          lable[lableptr] = toupper(line[i]);
          lableptr++;
          state = 2;
          continue;
        }else if((line[i] == ' ')||(line[i] == '\t')) {
          state = 3;
          continue;
        }else if((line[i] == '\n')||(line[i] == '\0')) {
          state = 0;
          break;
        }else {
          printf("error on line# %i\n",linenum);
          returncode = 0;
          state = 0;
          break;
        }
      }
      if(state == 1) {
        if((line[i] != '\n')&&(line[i] != '\0')) {
          state = 1;
          continue;
        }else {
          state = 0;
          break;
        }
      }
      if(state == 2) {
        if(((line[i] >= 'a')&&(line[i] <= 'z'))||((line[i] >= 'A')&&(line[i] <= 'Z'))) {
          lable[lableptr] = toupper(line[i]);
          lableptr++;
          state = 2;
          continue;
        }else if(line[i] == ':') {
          lable[lableptr] = '\0';
          if(newLable(lable)) {
            listOne[listOnePtr].lable = (char *)malloc(strlen(lable) +1);
            strcpy(listOne[listOnePtr].lable, lable);
            listOne[listOnePtr].index = binarrayindex;
            listOnePtr++;
            state = 4;
            continue;
          }else {
            printf("lable: [%s] used more than once, (line #%i)\n",lable,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else {
          printf("error on line# %i\n",linenum);
          returncode = 0;
          state = 0;
          break;
        }
      }
      if(state == 3) {
        if(((line[i] >= 'a')&&(line[i] <= 'z'))||((line[i] >= 'A')&&(line[i] <= 'Z'))) {
          state = 5;
          mnemonic[mnemonicptr] = line[i];
          mnemonicptr++;
          continue;
        }else if((line[i] == '\n')||(line[i] == '\0')) {
          state = 0;
          break;
        }else if((line[i] == ' ')||(line[i] == '\t')) {
          state = 3;
          continue;
        }else if(line[i] == ';') {
          state = 1;
          continue;
        }else {
          printf("error on line# %i\n",linenum);
          returncode = 0;
          state = 0;
          break;
        }
      }
      if(state == 4) {
        if((line[i] == ' ')||(line[i] == '\t')) {
          state = 4;
          continue;
        }else if(((line[i] >= 'a')&&(line[i] <= 'z'))||((line[i] >= 'A')&&(line[i] <= 'Z'))) {
          state = 5;
          mnemonic[mnemonicptr] = line[i];
          mnemonicptr++;
          continue;
        }else {
          printf("error on line# %i\n",linenum);
          returncode = 0;
          state = 0;
          break;
        }
      }
      if(state == 5) {
        if(((line[i] >= 'a')&&(line[i] <= 'z'))||((line[i] >= 'A')&&(line[i] <= 'Z'))) {
          state = 5;
          mnemonic[mnemonicptr] = line[i];
          mnemonicptr++;
          continue;
        }else if((line[i] == ' ')||(line[i] == '\t')) {
          mnemonic[mnemonicptr] = '\0';
          mnereturn = isMnemonic(mnemonic);
          if(mnereturn) {
            state = 6;
            continue;
          }else {
            printf("invalid mnemonic [%s] on line# %i\n",mnemonic,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else if((line[i] == '\n')||(line[i] == '\0')) {
          mnemonic[mnemonicptr] = '\0';
          mnereturn = isMnemonic(mnemonic);
          if((mnereturn == 1)||(mnereturn == 6)) {
            dispatchOperand("", mnemonic, mnereturn, linenum);
            somethingtowrite = 1;
            state = 0;
            break;
          }else {
            printf("invalid operand for mnemonic [%s] on line# %i\n",mnemonic,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else if(line[i] == ';') {
          mnemonic[mnemonicptr] = '\0';
          mnereturn = isMnemonic(mnemonic);
          if((mnereturn == 1)||(mnereturn == 6)) {
            dispatchOperand("", mnemonic, mnereturn, linenum);
            somethingtowrite = 1;
            state = 1;
            break;
          }else {
            printf("invalid operand for mnemonic [%s] on line# %i\n",mnemonic,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else {
          printf("error on line# %i\n",linenum);
          returncode = 0;
          state = 0;
          break;
        }
      }
      if(state == 6) {
        if((line[i] == ' ')||(line[i] == '\t')) {
          state = 6;
          continue;
        }else if((line[i] == '\n')||(line[i] == '\0')) {
          mnemonic[mnemonicptr] = '\0';
          mnereturn = isMnemonic(mnemonic);
          if((mnereturn == 1)||(mnereturn == 6)) {
            dispatchOperand("", mnemonic, mnereturn, linenum);
            somethingtowrite = 1;
            state = 0;
            break;
          }else {
            printf("invalid operand for mnemonic [%s] on line# %i\n",mnemonic,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else if(line[i] == ';') {
          mnemonic[mnemonicptr] = '\0';
          mnereturn = isMnemonic(mnemonic);
          if((mnereturn == 1)||(mnereturn == 6)) {
            dispatchOperand("", mnemonic, mnereturn, linenum);
            somethingtowrite = 1;
            state = 1;
            break;
          }else {
            printf("invalid operand for mnemonic [%s] on line# %i\n",mnemonic,linenum);
            returncode = 0;
            state = 0;
            break;
          }
        }else {
          state = 7;
          operand[operandptr] = line[i];
          operandptr++;
          continue;
        }
      }
      if(state == 7) {
        if((line[i] == '\n')||(line[i] == '\0')) {
          operand[operandptr] = '\0';
          opreturn = dispatchOperand(operand, mnemonic, mnereturn, linenum);
          if(opreturn) {
            somethingtowrite = 1;
            state = 0;
            break;
          }else {
            returncode = 0;
            state = 0;
            break;
          }
        }else if(line[i] == ';') {
          operand[operandptr] = '\0';
          opreturn = dispatchOperand(operand, mnemonic, mnereturn, linenum);
          if(opreturn) {
            somethingtowrite = 1;
            state = 1;
            continue;
          }else {
            returncode = 0;
            state = 0;
            break;
          }
        }else {
          state = 7;
          operand[operandptr] = line[i];
          operandptr++;
        }
      }
    }
    if(somethingtowrite) {
      maparray[maparrayindex] = (char *)malloc(strlen(fixLine(line)) + 1);
      strcpy(maparray[maparrayindex], fixLine(line));
      maparrayindex = maparrayindex + mapoffset;
      mapoffset = 0;
    }
  }
  if(!doBranches()) {
    returncode = 0;
  }

  return returncode;
}

/******************************************************************/

int writeToFileBin(FILE *foutbin) {
  char hi, lo;
  int num, i;

  for(i=0; i<BINARRAYSIZE; i++) {
   if(binarray[i] == NULL) {
    fprintf(foutbin, "%c%c",0,0);
   }else {
    sscanf(binarray[i], "%o", &num);
    lo = num & 0xFF;
    hi = num / 0x100;
    fprintf(foutbin,"%c%c",hi,lo);
   }
  }
  return 1;
}

/******************************************************************/

int writeToFileMap(FILE *foutmap) {
  int i;

  for(i=0; i<MAPARRAYSIZE; i++) {
    if(maparray[i] == NULL) {
      continue;
    }else {
      fprintf(foutmap,"%4.4x %s\n",i*2,maparray[i]);
    }
  }
  return 1;
}

/******************************************************************/

int openFilesToWrite(char *filenamein, char *filenameout, int numargs) {
  FILE *foutmap;
  FILE *foutbin;
  char filenamemap[20];
  char filenamebin[20];
  int i, len;

  if(numargs == 3) {
    len = strlen(filenameout);
    for(i=0; i<len; i++) {
      if((toupper(filenameout[i]) > 'Z')||(toupper(filenameout[i] < 'A'))) {
        printf("incorrect format for argument: <out>\n");
        return 0;
      }
    }
    strcpy(filenamemap, filenameout);
    strcat(filenamemap, ".map\0");
    strcpy(filenamebin, filenameout);
    strcat(filenamebin, ".bin\0");
  }else {
    len = strlen(filenamein);
    for(i=0; i<len; i++) {
      if(filenamein[i] == '.') {
        break;
      }
      filenamemap[i] = filenamein[i];
      filenamebin[i] = filenamein[i];
    }
    filenamemap[i] = '\0';
    filenamebin[i] = '\0';
    strcat(filenamemap, ".map\0");
    strcat(filenamebin, ".bin\0");
  }
  foutmap = fopen(filenamemap, "w");
  foutbin = fopen(filenamebin, "wb");
  if(foutmap == NULL) {
    printf("cannot open [%s]\n",filenamemap);
    return 0;
  }
  if(foutbin == NULL) {
    printf("cannot open [%s]\n",filenamebin);
    return 0;
  }
  writeToFileBin(foutbin);
  writeToFileMap(foutmap);
  printf("wrote files: [%s] and [%s]\n",filenamemap,filenamebin);
  fclose(foutmap);
  fclose(foutbin);
  return 1;
}

/******************************************************************/

FILE *openFileToRead(int argc, char *argv[]) {
  FILE *fin;
  int len, start, i, j;
  char filename[20];
  char filenameext[20];

  if((argc != 2)&&(argc != 3)) {
    printf("usage: pdpasm <filename.pdp> <out>\n");
    return NULL;
  }
  len = strlen(argv[1]);
  start = 0;
  j = 0;
  strcpy(filename,argv[1]);
  for(i=0; i<len; i++) {
    if(filename[i] == '.') {
      start = 1;
    }
    if(start) {
      filenameext[j] = filename[i];
      j++;
    }
  }
  filenameext[j] = '\0';
  if(strcmp(filenameext,".pdp") != 0) {
    printf("incorrect file extension [%s]\n",filenameext);
    return NULL;
  }
  fin = fopen(argv[1], "r");
  if(fin == NULL) {
    printf("cannot open file [%s]\n",argv[1]);
    return NULL;
  }
  return fin;
}

/******************************************************************/

int main (int argc, char *argv[]) {
  FILE *fin,*q;
  int i;

  printf("\n");
  printf("***************************************\n");
  printf("*   KHAQUEZENGASHEZ PDP-11 Assembler  *\n");
  printf("* -our shit don't stink               *\n");
  printf("***************************************\n\n");

  fin = openFileToRead(argc, argv);
  if(fin == NULL) {
    return 0;
  }
  initArrays();
  if(!fillInstrArrays()) {
    printf("error allocating memory, pdpasm terminated\n");
    return 0;
  }
  if (parseFilePassOne(fin)) {
    openFilesToWrite(argv[1], argv[2], argc);
    fclose(fin);
  }else {
    printf("There are errors in [%s]\n",argv[1]);
    fclose(fin);
  }
  q = fopen("debug.txt", "w");
  for(i=0; i<MAXNUMBRANCHES; i++) {
    if(listOne[i].lable == NULL) {
      continue;
    }else {
      fprintf(q,"%i: lable:[%s] index:%i *** ",i,listOne[i].lable,listOne[i].index);
      fprintf(q,"lable:[%s] index:%i\n",listTwo[i].lable,listTwo[i].index);
    }
  }
  for(i=0; i<BINARRAYSIZE; i++) {
    if(binarray[i] == NULL) {
      if(maparray[i] == NULL) {
        continue;
      }else {
        fprintf(q,"%i: [NULL]--[%s]\n",i,maparray[i]);
      }
    }else {
      if(maparray[i] == NULL) {
        fprintf(q,"%i: [%s]--[NULL]\n",i,binarray[i]);
      }else{
        fprintf(q,"%i: [%s]--[%s]\n",i,binarray[i],maparray[i]);
      }
    }
  }
  fclose(q);
  return 0;
}
