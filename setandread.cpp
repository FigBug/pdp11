#include "debugger.h"

//import global variables for keeping track of break points from the main file.
extern int bpcount;
extern int bpoints[4];
extern list *head;

void UI_setMem(char *parameter[], int n){
  //*parameter[] contains user input: <label/address> <byte/word> newvalue
  //address = hex, newvalue = hex
  //order matters.

  char *BorW;
  int cpuerror;
  int validaddr;
  int validnewnum;
  int validlabel;
  u8 newnum8;
  u16 newnum16;
  u16 addr;

  //there should always be only 3 parameters: addr, size, newvalue.
  if (n != 3){  
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }

  //check if the first parameter is a label.
  validlabel = ST_lookupLabel(parameter[0], addr);
  
  //If it is not in the label table, it SHOULD be a hex address...
  if (validlabel == false){

    //make sure the address is a valid 16 unsigned value
    validaddr = MS_isValidU16(parameter[0], "hex");
  
    //if validaddr is less than minimum value of a U16, there was an error or incorrect input
    if (validaddr < MIN_U16){
      printf("Invalid address or label. Try again.\n");
      return;
    }

    //if valid, continue
    addr = validaddr;
  }
  //now determine whether to call CPUwriteword or CPUwritebyte
  BorW = parameter[1];

  if ((!strcmp(BorW, "byte")) || (!strcmp(BorW, "b"))){
    
    //make sure the address is a valid 16 unsigned value
    validnewnum = MS_isValidU8(parameter[2], "hex");

    //if not less than valid u8, error or incorrect input.
    if (validnewnum < MIN_U8){      
      printf("Invalid new value. Try again.\n");
      return;
    }
    
    //if valid, continue
    newnum8 = validnewnum;
    cpuerror = CPUwritebyte(addr, newnum8);
  }
  else if ((!strcmp(BorW, "word")) || (!strcmp(BorW, "w"))){
    
    //can only write words to even memory addresses
    if ((addr % 2) != 0){
      printf("Words can only be written to even memory addresses. Try again.\n");
      return;
    }

    //make sure the new value is a valid 16 unsigned value
    validnewnum = MS_isValidU16(parameter[2], "hex");

    //if not less than valid u16, error or incorrect input.
    if (validnewnum < MIN_U16){
      printf("Invalid new value. Try again.\n");
      return;
    }

    //if valid, continue
    newnum16 = validnewnum;
    cpuerror = CPUwriteword(addr, newnum16);
  }
  else{
    printf("Please enter whether the new value is a word or a byte. Try again.\n");
    return;
  }

  if (cpuerror == 1){          //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_readMem(char *parameter[], int n){
  //*parameter[] contains user input: <label/address> <byte/word>
  //address = hex
  //oder matters

  u16 addr;
  u16 result;
  u8 temp;
  int cpuerror;
  int validaddr;
  int validlabel;
  char *BorW;

  //there should always be only 2 parameters: addr, size.
  if (n != 2){  
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  //check if the first parameter is a label.
  validlabel = ST_lookupLabel(parameter[0], addr);
  
  //If it is not in the label table, it SHOULD be a hex address...
  if (validlabel == false){

    //make sure the address is a valid 16 unsigned value
    validaddr = MS_isValidU16(parameter[0], "hex");
  
    //if validaddr is less than minimum value of a U16, there was an error or incorrect input
    if (validaddr < MIN_U16){
      printf("Invalid address or label. Try again.\n");
      return;
    }

    //if valid, continue
    addr = validaddr;
  }

  //now determine whether to call CPUwriteword or CPUwritebyte
  BorW = parameter[1];

  if ((!strcmp(BorW, "byte")) || (!strcmp(BorW, "b"))){
    
    //call readbyte.
    cpuerror = CPUreadbyte(addr, temp);
    result = temp;          //8 bit -> 16 bit conversion. no biggie.

    if (cpuerror == 1){          //something has gone wrong in CPU
      printf("Something had gone wrong. oops.\n");
      return;
    }
    printf("Addr %4.4x = %2.2X\n", addr, result);

  }
  else if ((!strcmp(BorW, "word")) || (!strcmp(BorW, "w"))){

    //can only read words from even memory addresses
    if ((addr % 2) != 0){
      printf("Words can only be read from even memory addresses. Try again.\n");
      return;
    }

    //call readword.
    cpuerror = CPUreadword(addr, result);

    if (cpuerror == 1){          //something has gone wrong in CPU
      printf("Something had gone wrong. oops.\n");
      return;
    }
    printf("Addr %4.4x = %4.4X\n", addr, result);  
  }
  else{
    printf("Please enter whether the new value is a word or a byte. Try again.\n");
    return;
  }
}

void UI_setReg(char *parameter[], int n){
  //*parameter[] contains user input: registerid, newvalue.
  //newvalue = hex    regid = 0-8, pc, sp, psw
  //order matters

  int validnewnum;
  int regid;
  int cpuerror;
  u16 newnum;
  char regstr[10];

  //there should always be only 2 parameters: registerid, newvalue.
  if (n != 2){  
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  //make sure new value is valid u16
  validnewnum = MS_isValidU16(parameter[1], "hex");

  //if not less than valid u16, error or incorrect input.
  if (validnewnum < MIN_U16){
    printf("Invalid new value. Try again.\n");
    return;
  }
  newnum = validnewnum;

  //now determine the registerid
  strcpy(regstr, parameter[0]);

  //all registers can be set using register values... r0 - r7
  if ((regstr[0] == 'r') && (regstr[1] >= '0') && (regstr[2] < '8')){
    if (regstr[1] == '0'){
      regid = 0;
    }
    else if (regstr[1] == '1'){
      regid = 1;
    }
    else if (regstr[1] == '2'){
      regid = 2;
    }
    else if (regstr[1] == '3'){
      regid = 3;
    }
    else if (regstr[1] == '4'){
      regid = 4;
    }
    else if (regstr[1] == '5'){
      regid = 5;
    }
    else if (regstr[1] == '6'){
      regid = 6;
    }
    else if (regstr[1] == '7'){
      regid = 7;
    }
    else{
      printf("Invalid register id. Try again.\n");
      return;
    }
  }
  //sp -> Stack Pointer (=r6)
  else if ((regstr[0] == 's') && (regstr[1] == 'p') && (regstr[2] == '\0')){      
    regid = 6;  
  }
  //pc -> Program Counter (=r7)
  else if ((regstr[0] == 'p') && (regstr[1] == 'c') && (regstr[2] == '\0')){      
    regid = 7;
  }
  //psw ->program status word
  else if ((regstr[0] == 'p') && (regstr[1] == 's') && (regstr[2] == 'w') && (regstr[3] == '\0')){
    regid = PSW;                    
  }
  else{
    printf("Invalid register id. Try again.\n");
    return;
  }

  cpuerror = CPUwritereg(regid, newnum);
  
  if (cpuerror == 1){  //something has gone wrong in CPU
    printf("Something has gone wrong. oops.\n");
    return;
  }
}

void UI_readReg(char *parameter[], int n){
  //*parameter[] contains user input: registerid
  //if no parameters (n=0) that means nore registers specified, so display the contents of all of them.

  u16 regid;
  u16 result;
  char regstr [10];    //to make comparing say the 3rd element easier
  int cpuerror;
  
  //there should be either no parameters or 1 parameter: registerid. 
  if (n == 0){
    //display all registers
    regid = 0;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R0 = %4.4X\t", result);

    regid = 1;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R1 = %4.4X\t", result);

    regid = 2;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R2 = %4.4X\t", result);

    regid = 3;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R3 = %4.4X\t", result);

    regid = 4;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R4 = %4.4X\n", result);

    regid = 5;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R5 = %4.4X\t", result);

    regid = 6;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R6 = %4.4X\t", result);

    regid = 7;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("R7 = %4.4X\t\n", result);

    regid = 8;
    cpuerror = CPUreadreg(regid, result);
    if (cpuerror == 1){
      printf("Something has gone wrong. oops.\n");
      return;
    }
    printf("PSW = ");
    MS_printBinaryString(result);
    printf("\n");

  }
  //if a register id was given:
  else if (n > 1){  
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  //otherwise determine which register it is, and call CPUreadreg. 
  //valid regids: r0 - r7, sp, pc, psw.
  else{

    strcpy(regstr, parameter[0]);
    
    //its a register (r0 - r7)
    if ((regstr[0] == 'r') && ((regstr[1] >= '0') && (regstr[2] < '8'))){        
      if (regstr[1] == '0'){
        regid = 0;
        printf("R0 = ");
      }
      else if (regstr[1] == '1'){
        regid = 1;
        printf("R1 = ");
      }
      else if (regstr[1] == '2'){
        regid = 2;
        printf("R2 = ");
      }
      else if (regstr[1] == '3'){
        regid = 3;
        printf("R3 = ");
      }
      else if (regstr[1] == '4'){
        regid = 4;
        printf("R4 = ");
      }
      else if (regstr[1] == '5'){
        regid = 5;
        printf("R5 = ");
      }
      else if (regstr[1] == '6'){
        regid = 6;
        printf("R6 = ");
      }
      else if (regstr[1] == '7'){
        regid = 7;
        printf("R7 = ");
      }
      else{
        printf("Invalid register id. Try again.\n");
        return;
      }
    }
    //sp -> Stack Pointer (=r6)
    else if ((regstr[0] == 's') && (regstr[1] == 'p') && (regstr[2] == '\0')){      
      regid = 6;
      printf("SP = ");
    }
    //pc -> Program Counter (=r7)
    else if ((regstr[0] == 'p') && (regstr[1] == 'c') && (regstr[2] == '\0')){      
      regid = 7;
      printf("PC = ");
    }
    //psw ->program status word
    else if ((regstr[0] == 'p') && (regstr[1] == 's')&&(regstr[2] == 'w') && (regstr[3] == '\0')){
      regid = PSW;
      printf("PSW = ");
    }
    //not register or sp or pc or psw -> not valid at all.
    else{
      printf("Invalid register id. Try again.\n");
      return;
    }
    //now regid has a value, unless input was invalid, in which case it will not get this far.

    cpuerror = CPUreadreg(regid, result);

    if (cpuerror == 1){  //something has gone wrong in CPU
      printf("Something has gone wrong. oops.\n");
      return;
    }
  
    if (regid == PSW){
       MS_printBinaryString(result);
      printf("\n");
    }else{
      printf("%4.4X\n", result);
    }
  }
}

void UI_pcReset(int n){
  //there should never be any parameters for this command. 
  //It always sets the program counter back to the starting value of 0x200
  
  int cpuerror;

  if (n != 0){
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  //otherwise, just set the program counter to 0x200
  cpuerror = CPUwritereg(7, PCSTARTADDR);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_setFlg(char *parameter[], int n){
  //*parameter[] contains user input: list of flags to set/reset.
  //can only change 3 flags with one command -> limitation of main program... might change later.
  //+FT sets flag t to a 1.
  //-FT sets flag t to a 0.
  //flags FT, FN, FZ, FC refer to flags F4-F0
  //flags F5-F7 are priority flags.
  //flags F8-F16 are unused, but the user can set them anyways if that would make them happy.

  int value;
  u8 flag;
  int cpuerror;
  int i;

  if (n == 0){
    printf("You must specify which flag(s) to set. Please try again.\n");
    return;
  }
  
  for (i = 0; i < n; i++){
    value = MS_lookupFlag(parameter[i], flag);  

    if (value >= 0){                          //MS_lookupFlag returns -1 for invalid user input.
      cpuerror = CPUsetflag(flag, (bool)value);

      if (cpuerror == 1){                      //something has gone wrong in CPU
        printf("Something had gone wrong. oops.\n");
        return;
      }
    }
  }
  return;
}

void UI_setBreak(char *parameter[], int n){
  //*parameter[] contains user input: <label> or <memaddr>
  //there should only ever be one parameter.

  int validaddr;
  int i;
  int empty;
  int cpuerror;
  int validlabel;
  u16 addr;

  if (n != 1){
    printf("You entered the wrong number of parameters. Please try again.\n");
    return;
  }

  if (bpcount >= 4){
    printf("You already have 4 break points set. You must clear one if you want to set another.\n");
    return;
  }

  //check if the first parameter is a label.
  validlabel = ST_lookupLabel(parameter[0], addr);
  
  //If it is not in the label table, it SHOULD be a hex address...
  if (validlabel == false){

    //right now, the command only works with valid memory addresses. Line numbers and labels not implemented yet.
    validaddr = MS_isValidU16(parameter[0], "hex");   
    //returns a valid u16 or a negative number if something went wrong.
  
    //if validaddr is less than minimum value of a U16, there was an error or incorrect input
    if (validaddr < MIN_U16){
      printf("Invalid address or label. Try again.\n");
      return;
    }
    //if valid, continue
    addr = validaddr;
  }
  //now we have a valid memory address, so keep going 

  //make sure that the break points are unique 
  //-- don't allow user to enter 2 break points at same address
  //check all the elements to find empty spot and make sure not duplicated.
  for (i = 0; i < 4; i++){            

    //if the element is not empty, test if duplicate, otherwise keep going...
    if ( bpoints[i] != -1){

      //if the element is equal to the specified address, print message and quit.
      if (bpoints[i] == addr){
        printf("Specified break point already set.\n");
        return;
      }
    }
    //if the element is empty, keep track of it, and test next element.
    else{
      empty = i;  //will find last empty element in the array.
    }
  }
  //store the break point address in the empty element.
  bpoints[empty] = addr;
  //increment the breakpoint counter.
  bpcount ++;       

  cpuerror = CPUsetbreak(addr);
  if (cpuerror == 1){                      //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_clrBreak(char *parameter[], int n){
  //*parameter[] contains user input: <label, memory address> or <all>
  //there should only ever be one parameter.

  int cpuerror;
  int validlabel;
  int validaddr;
  int i;
  u16 addr;

  if (n != 1){
    printf("You entered the wrong number of parameters. Please try again.\n");
    return;
  }

  if (bpcount < 1){     //can't clear a breakpoint if there aren't any set!
    printf("There are no break points currently set.\n");
    return;
  }

  if (!strcmp(parameter[0], "all")){
    //remove all the break points from the array, and remove them from the CPU, and set the elements to -1.
    for (i = 0; i < 4; i ++){
      if (bpoints[i] != -1){
        CPUclrbreak(bpoints[i]);
        bpoints[i] = -1;
      }
    }
  }
  else{
    //check if the first parameter is a label.
    validlabel = ST_lookupLabel(parameter[0], addr);
  
    //If it is not in the label table, it SHOULD be a hex address...
    if (validlabel == false){

      //right now, the command only works with valid memory addresses. Line numbers and labels not implemented yet.
      validaddr = MS_isValidU16(parameter[0], "hex");   
      //returns a valid u16 or a negative number if something went wrong.
  
      //if validaddr is less than minimum value of a U16, there was an error or incorrect input
      if (validaddr < MIN_U16){
        printf("Invalid address or label. Try again.\n");
        return;
      }
      //if valid, continue
      addr = validaddr;
    }

    //determine if the break point address is in the array
    i = 0;
    while (bpoints[i] != addr){
      if (i >= 4){
        printf("Not a valid break point.\n");
        return;
      }
      i++;
    }
    
    //if it gets this far, the break point was found in the array, and it is at index i.
    bpoints[i] = -1;
    bpcount --;
    
    cpuerror = CPUclrbreak(addr);
    if (cpuerror == 1){                      //something has gone wrong in CPU
      printf("Something had gone wrong. oops.\n");
      return;
    }
  }
}
