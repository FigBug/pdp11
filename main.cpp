#include "debugger.h"

#define MAX_MEMORY  0x7000

//array for storing the addresses of the break points
//int because then use -1 as the invalid value, since 0 is a valid address.
int bpoints[4];

//global variable to keep track of number of breakpoints currently set. must be <=4.
int bpcount;       

extern list *head;

void UI_next(int n){
  //there should be no parameters entered by the user, so
  // n = 0     and *parameters is empty.

  u16 pcaddr;
  u16 memaddr;
  u16 bytecode;
  int cpuerror;

  if (n != 0){
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  //get the current location of the program counter.
   cpuerror = CPUreadreg(7, pcaddr);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }

  //before executing the instruction, test if it is a 'jsr'
  cpuerror = CPUreadword(pcaddr, bytecode);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
  
  //test if the instruction is a 'jsr'
  if ((bytecode & JSR_MSK) == 0x0800){
    
    //see if need to set break point at pcaddr+2 or pcaddr+4
    if (((bytecode & ADDRMODE_MSK) == 0x0017) || 
        ((bytecode & ADDRMODE_MSK) == 0x001F) ||
        ((bytecode & ADDRMODE_MSK) == 0x0037) ||
        ((bytecode & ADDRMODE_MSK) == 0x003F)){

      memaddr = pcaddr + 4;
    }
    else{
      memaddr = pcaddr + 2;
    }

    cpuerror = CPUsetbreak(memaddr);
    if (cpuerror == 1){                      //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
    }

    CPUgo(-1, -1);
    CPUclrbreak(memaddr);
  }

  //if it is not a jump, just step on man!
  else{
    //execute current line of code.
    CPUstep();  
  }
  
  //everybody needs to print the next line;

  //get value of PC current counter (points to next instruction to be executed
  cpuerror = CPUreadreg(7, pcaddr);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
  
  MS_printfCodeLine(pcaddr);

}

void UI_trace(int n){
  //trace doesn't take any parameters, so n=0 otherwise there was an input error...
  u16 pcaddr;
  int cpuerror;

  if (n != 0){
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  //execute current line of code.
  CPUstep();  
  
  //get value of PC current counter (points to next instruction to be executed
  cpuerror = CPUreadreg(7, pcaddr);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }

  MS_printfCodeLine(pcaddr);
}

void UI_run0parms(char *parameter[]){
  s16 seconds;
  s16 instructions;
  int cpuerror;

  seconds = -1;
  instructions = -1;

  cpuerror = CPUgo(seconds, instructions);
  
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_run1parms(char *parameter[]){

  int value;
  int validlabel;
  int length;
  s16 seconds;
  s16 instructions;
  u16 addr;
  int cpuerror;
  char type;
  char temp[1024];

  //default values
  instructions = -1;
  seconds = -1;
  value = -1;

   
  //if the parameter is an address, check if it is a label 
  strcpy(temp, parameter[0]);
  length = strlen(temp);

  if ((temp[length-1] == 'a') && (head != NULL)){
    type = 'a';
    temp[length-1] = '\0';
    validlabel = ST_lookupLabel(temp, addr);
    if (validlabel == true){        //label found
      value = addr;
    }
  }
  if (value == -1){            //if value still = -1, then the thing wasn't a label!!
    //if it's not a label, get the value and the type.
    type = MS_valueAndType(parameter[0], value);
  }

  //parameter type (a, i, s) returned to "type"; the valid value returned in "value"
  //now just test it type is >= 0, and if value >= 0;
  
  if (type < 0){
    printf("Invalid parameter subscript. Try again.\n");
    return;
  }
  if (value < 0){
    printf("Invalid input parameter.\n");
    return;
  }
  
  if (type == 'a'){
    CPUwritereg(7, value);      //change the pc to the starting location.
  }
  else if (type == 'i'){
    instructions = (s16)value;
  }
  else if (type == 's'){                //type = s
    seconds = (s16)value;
  }
  else{
    printf("Invalid paramter subscript. Try again.\n");
    return;
  }

  cpuerror = CPUgo(seconds, instructions);  

  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_run2parms(char *parameter[]){

  char type[2];
  int value[2];
  int i;
  s16 seconds;
  s16 instructions;
  u16 addr;
  int cpuerror;
  int validlabel;
  char temp[1024];
  int length;

  //default values
  instructions = -1;
  seconds = -1;
  for (i = 0; i < 2; i++){
    value[i] = -1;
  }

  //check all the parameters to see if they are labels.
  //will only be labels if they are addresses.
  for (i = 0; i < 2; i++){
    strcpy(temp, parameter[i]);
    length = strlen(temp);
    if ((temp[length-1] == 'a') && (head != NULL)){
      type[i] = 'a';
      temp[length-1] = '\0';
      validlabel = ST_lookupLabel(temp, addr);
      if (validlabel == true){
        value[i] = addr;
      }
    }
    if (value[i] == -1){
      type[i] = MS_valueAndType(parameter[i], value[i]);
    }
  }

  //parameter type (a, i, s) returned to "type"; the valid value returned in "value"
  //now just test it type is >= 0, and if value >= 0;

  if ((type[0] < 0) || (type[1] < 0)){
    printf("Invalid parameter subscript. Try again.\n");
    return;
  }
  if ((value[0] < 0) || (value[1] < 0)){
    printf("Invalid input parameter.\n");
    return;
  }
  if (type[0] == type[1]){
    printf("Both parameters are of the same type.\n");
    return;
  }

  //Now because we know that the parameters aren't both the same type, 
  //we can test whether each type of each variable is set...
  //This works because order of parameters doesn't matter.

  if (type[0] == 'a'){
    CPUwritereg(7, value[0]);      //change the pc to the starting location.
  }
  if (type[1] == 'a'){
    CPUwritereg(7, value[1]);
  }
  if (type[0] == 'i'){
    instructions = (s16)value[0];
  }
  if (type[1] == 'i'){
    instructions = (s16)value[1];
  }
  if (type[0] == 's'){
    seconds = (s16)value[0];
  }
  if (type[1] == 's'){
    seconds = (s16)value[1];
  }

  cpuerror = CPUgo(seconds, instructions);  

  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_run3parms(char *parameter[]){

  char type[3];
  char temp[1024];
  int validlabel;
  int length;
  int i;
  int value[3];
  s16 seconds;
  s16 instructions;
  u16 addr;
  int cpuerror;

  //default values
  instructions = -1;
  seconds = -1;
  for (i = 0; i < 3; i++){
    value[i] = -1;
  }

  //check all the parameters to see if they are labels.
  //will only be labels if they are addresses.
  for (i = 0; i < 3; i++){
    strcpy(temp, parameter[i]);
    length = strlen(temp);
    if ((temp[length-1] == 'a') && (head != NULL)){
      type[i] = 'a';
      temp[length-1] = '\0';
      validlabel = ST_lookupLabel(temp, addr);
      if (validlabel == true){
        value[i] = addr;
      }
    }
    if (value[i] == -1){
      type[i] = MS_valueAndType(parameter[i], value[i]);
    }
  }

  //parameter type (a, i, s) returned to "type"; the valid value returned in "value"
  //now just test it type is >= 0, and if value >= 0;

  if ((type[0] < 0) || (type[1] < 0) || (type[2] < 0)){
    printf("Invalid parameter subscript. Try again.\n");
    return;
  }
  if ((value[0] < 0) || (value[1] < 0) || (value[2] < 0)){
    printf("Invalid input parameter.\n");
    return;
  }
  if ((type[0] == type[1]) || (type[0]== type[2]) || (type[1] == type[2])){
    printf("Two or more parameters are of the same type.\n");
    return;
  }

  //Now because we know that the parameters aren't all of the same type, 
  //we can test whether each type of each variable is set...
  //This works because order of parameters doesn't matter.

  //if the type is 'a' make sure the value is actually a u16 instead of a s16.
  if (type[0] == 'a'){
    CPUwritereg(7, value[0]);      //change the pc to the starting location.
  }
  if (type[1] == 'a'){
    CPUwritereg(7, value[1]);
  }
  if (type[2] == 'a'){
    CPUwritereg(7, value[2]);
  }
  if (type[0] == 'i'){
    instructions = (s16)value[0];
  }
  if (type[1] == 'i'){
    instructions = (s16)value[1];
  }
  if (type[2] == 'i'){
    instructions = (s16)value[2];
  }
  if (type[0] == 's'){
    seconds = (s16)value[0];
  }
  if (type[1] == 's'){
    seconds = (s16)value[1];
  }
  if (type[2] == 's'){
    seconds = (s16)value[2];
  }

  cpuerror = CPUgo(seconds, instructions);  

  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
}

void UI_run(char *parameter[], int n){
  //*parameter[] contains user input: <startingAddress> <time> <NumberOfInstructions> (none,1,2,or all three)

  //Because I Want To, #1:
  //  user must subscript the input parameters so that they are idenitfiable to me the supreme ruler of  
  //  of run function. If the user only enters one parameters, say instructions, I have no way of knowing 
  //  whether the user was referring to seconds or instructions or address. So I therefore decree that 
  //  all users of the run function must enter seconds followed by an s, and instructions followed by an i,
  //  and address followed by an a.
  //    eg. run 167s  or    run 243i  or run 167s 243i 343a
  // Order of the input parameters does not matter.
  // Seconds and Instructions must be entered in decimal format, but address must be in hex.

  u16 pcaddr;
  int cpuerror;
  int i;

  if (n > 3){

    printf("You entered the wrong number of parameters. Try again.\n");
    return;

  }
  //no parmeters --> call go with -1 and -1.
  else if (n == 0){  

    UI_run0parms(parameter);

  }
  //1 parameter --> has to be either seconds or instructions or address.
  else if (n == 1){

    UI_run1parms(parameter);

  }
  //2 parameters -> two of seconds, instructions, or address. Will stop execution when first condition is met.
  else if (n ==2){

    UI_run2parms(parameter);

  }
  //3 parameters -> all three parameters...
  else {
    UI_run3parms(parameter);
  }

  //check if program counter is equal to one of the break points. 
  //if it si then tell the user, and print the next line of code to be exectuted.
    
  cpuerror = CPUreadreg(7, pcaddr);
  if (cpuerror == 1){    //something has gone wrong in CPU
    printf("Something had gone wrong. oops.\n");
    return;
  }
  //check if the pcaddr is a break point.
  for (i = 0; i < 4; i++){
    if (pcaddr == bpoints[i]){
      //if it is, tell the user, 
      printf("Stopped at break point %4.4x.\n", pcaddr);
      //and print the next line of code
      MS_printfCodeLine(pcaddr);
    }
  }
}


int UI_list2parms (char *parameter[], u16 &lines, u16 &centeraddr){

  int value[2];
  int validlabel;
  int i;
  int length;
  char type[2];
  char temp[1024];
  u16 addr;

  
  for (i = 0; i < 2; i++){
    value[i] = -1;
  }

  //check all the parameters to see if they are labels.
  //will only be labels if they are addresses.
  for (i = 0; i < 2; i++){
    strcpy(temp, parameter[i]);
    length = strlen(temp);
    if ((temp[length-1] == 'a') && (head != NULL)){
      type[i] = 'a';
      temp[length-1] = '\0';
      validlabel = ST_lookupLabel(temp, addr);
      if (validlabel == true){
        value[i] = addr;
      }
    }
    if (value[i] == -1){
      type[i] = MS_valueAndType(parameter[i], value[i]);
    }
  }

  //parameter type (a, n) returned to "type"; the valid value returned in "value"
  //now just test it type is >= 0, and if value >= 0;

  if ((type[0] < 0) || (type[1] < 0)){
    printf("Invalid parameter subscript. Try again.\n");
    return -1;
  }
  if ((value[0] < 0) || (value[1] < 0)){
    printf("Invalid input parameter.\n");
    return -1;
  }
  if (type[0] == type[1]){
    printf("Both parameters are of the same type.\n");
    return -1;
  }

  //Now because we know that the parameters aren't both the same type, 
  //we can test whether each type of each variable is set...
  //This works because order of parameters doesn't matter.
    
  if (type[0] == 'a'){
    //change the pc to the starting location.
    centeraddr = value[0];
  }
  if (type[1] == 'a'){
    //change the pc to the starting location.
    centeraddr = value[1];
  }
  if (type[0] == 'n'){
    //change the number of lines to display to the input value.
    lines = (s16)value[0];
  }
  if (type[1] == 'n'){
    //change the number of lines to display to the input value.
    lines = (s16)value[1];
  }
  return 0;
}

int UI_list1parms (char *parameter[], u16 &lines, u16 &centeraddr){

  int type;
  int value;
  int length;
  int validlabel;
  u16 addr;
  char temp[1024];

  value = -1;

  //if the parameter is an address, check if it is a label 
  strcpy(temp, parameter[0]);
  length = strlen(temp);

  if ((temp[length-1] == 'a') && (head != NULL)){
    type = 'a';
    temp[length-1] = '\0';
    validlabel = ST_lookupLabel(temp, addr);
    if (validlabel == true){        //label found
      value = addr;
    }
  }
  if (value == -1){            //if value still = -1, then the thing wasn't a label!!
    //if it's not a label, get the value and the type.
    type = MS_valueAndType(parameter[0], value);
  }
  
  //parameter type (a, n) returned to "type"; the valid value returned in "value"
  //now just test it type is >= 0, and if value >= 0;
  
  if (type < 0){
    printf("Invalid parameter subscript. Try again.\n");
    return -1;
  }
  if (value < 0){
    printf("Invalid input parameter.\n");
    return -1;
  }
  
  if (type == 'a'){
    //change the pc to the starting location.
    centeraddr = value;        
  }
  else if (type == 'n'){
    //change the number of lines to display to the input value.
    lines = (u16)value;
  }
  else{
    printf("Invalid paramter subscript. Try again.\n");
    return -1;
  }
  return 0;
}

void UI_list(char *parameter[], int n){
  
  u16 pcaddr;
  u16 centeraddr;
  u16 displayaddr;
  u16 bytecode;
  char *codeline;
  char text[20];
  int cpuerror;
  int reterror;
  int i;
  u16 lines;

  if (n > 2){
    printf("You entered the wrong number of parameters yet. Please try again.\n");
    return;
  }
  
  //DEFAULT VALUES:
  CPUreadreg(7, pcaddr);      //pc = current (ie don't change)
  centeraddr = pcaddr;
  lines = 10;            //default for lines is 10

  //for n==0, don't need to change the default values so 
  //skip the if/else thing completely -> go straight to the for loop.

  if (n == 1){

    reterror = UI_list1parms(parameter, lines, centeraddr);
    if (reterror < 0){
      return;
    }
  }
  else if (n == 2){
  
    reterror = UI_list2parms(parameter, lines, centeraddr);
    if (reterror < 0){
      return;
    }
  }

  if ((centeraddr + 2*(lines + 1)) > MAX_MEMORY){
    printf("List will exceed uppermost memory address. Please try again.\n");
    return;
  }

  for (i = lines; i > -(lines+1); i--){      //display from ca-x to ca-0 to ca+x

    displayaddr = centeraddr - 2*i;    //top line displayed is 5 (times 2 bytes) lines back.
    
    //get byte code for the instruction
    cpuerror = CPUreadword(displayaddr, bytecode);
    if (cpuerror == 1){    //something has gone wrong in CPU
      printf("Something had gone wrong. oops.\n");
      return;
    }

    //get string corresponding to the instruction pointed to by the pc
    codeline = ST_lookupString(displayaddr);  

    if (codeline == NULL)                    //there are no labels, so decompile it!!
    {
      DECdecompile(displayaddr, text);
      if (i == 0)
      {
        printf(">%4.4X %4.4X %s\n", displayaddr, bytecode, text);
      }
      else
      {
        printf(" %4.4X %4.4X %s\n", displayaddr, bytecode, text);
      }
    }
    else
    {
      //print to screen:
      if (i == 0)
      {
        printf(">%4.4X %4.4X %s\n", displayaddr, bytecode, codeline);
      }
      else
      {
        printf(" %4.4X %4.4X %s\n", displayaddr, bytecode, codeline);
      }
    }
  }
}

void UI_memDump(char *parameter[], int n){
  //*parameter[] contains user input: 2 addresses -> starting and ending. 
  //if both parameters are not given, it defaults to the default values.

  int validstart;
  int validstartlab;
  int validend;
  int validendlab;
  int j;
  int cpuerror;
  u16 i;
  u16 pcaddr;
  u16 startaddr;
  u16 endaddr;
  u8 byte;
  
  if (n > 2){
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  
  else if (n == 0){
     //default for start address is the current location of the program counter.
    CPUreadreg(7, pcaddr);      //pc = current (ie don't change)
    startaddr = pcaddr;

    //default for the end address is the start address + 10d(16d)-1 = startaddress + 159
    //    10 lines of 16 bytes
    endaddr = startaddr + 159;
  }
  else if (n == 1){     
    //if only one parameter was entered, it must be the start address.
  
    //check if the first parameter is a label.
    validstartlab = ST_lookupLabel(parameter[0], startaddr);
  
    //If it is not in the label table, it SHOULD be a hex address...
    if (validstartlab == false){

      //make sure the address is a valid 16 unsigned value
      validstart = MS_isValidU16(parameter[0], "hex");
  
      //if validaddr is less than minimum value of a U16, there was an error or incorrect input
      if (validstart < MIN_U16){
        printf("Invalid start address or label. Try again.\n");
        return;
      }
      //if valid, continue
      startaddr = validstart;
    }
 
    //default for the end address is the start address + 10d(16d)-1 = startaddress + 159
    //    10 lines of 16 bytes
    endaddr = startaddr + 159;
  }
  else if (n == 2){
    //Start and End addresses supplied

    //check if the first parameter is a label.
    validstartlab = ST_lookupLabel(parameter[0], startaddr);
  
    //If it is not in the label table, it SHOULD be a hex address...
    if (validstartlab == false){

      //make sure the address is a valid 16 unsigned value
      validstart = MS_isValidU16(parameter[0], "hex");
  
      //if validaddr is less than minimum value of a U16, there was an error or incorrect input
      if (validstart < MIN_U16){
        printf("Invalid start address or label. Try again.\n");
        return;
      }
      //if valid, continue
      startaddr = validstart;
    }
    //if get this far, we have a valid start address.

    //check if the second parameter is a label.
    validendlab = ST_lookupLabel(parameter[1], endaddr);
  
    //If it is not in the label table, it SHOULD be a hex address...
    if (validendlab == false){

      //make sure the address is a valid 16 unsigned value
      validend = MS_isValidU16(parameter[1], "hex");
  
      //if validaddr is less than (or equal to) the starting address, then that makes no sense.
      if (validend <= startaddr){
        printf("Invalid end address or label. Try again.\n");
        return;
      }
      //if valid, continue
      endaddr = validend;
    }
    //if we get here we have a valid start and end address, 
    //and the end address is greater than the start address.
  }
  //everyone does this part!! (unless the are stupid and have already returned.)
  j = 0;
  for (i = startaddr; i <= endaddr; i++){
      
    cpuerror = CPUreadbyte(i, byte);
    if (cpuerror == 1){          //something has gone wrong in CPU
      printf("Something had gone wrong. oops.\n");
      return;
    }
    
    if (j % 16 == 0){             //print lines of 16 bytes (each separated by 1 space)
      printf("\n%4.4X  ", i);     //start each line with the memory address (followed by 2 spaces)
    }
    printf("%2.2X ", byte);
    j++;                          //increment byte counter
  }
  printf("\n");
}

void UI_loadMem(char *parameter[], int n){
  //*parameter[] contains user input: either filename or nothing.
  //bin file is require, but map file is optional. If can't find bin file, user screwed up, but if can't find
  //map file, that is okey dokey.

  u16 pcaddr;
  u16 addr = 0;
  FILE *fp;
  int inputlength;
  int ch;
  int i;
  char *filetok;
  char *emptytok;
  char binstr[256];
  char mapstr[256];
  char input[1024];
  char delim[] = " \t";


  if (n > 1){
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }
  else if (n == 1){
    filetok = parameter[0];
    emptytok = NULL;    //just so that the next "if" statement works
  }
  else{
    //prompt user for a filename to load into memory.

    printf("Enter file name: ");
  
    fgets(input, 1023, stdin);

    //remove the carridge return from the end of the string.
    input[strlen(input)-1] = '\0';

    //get rid of any spaces or tabs ar the end of the string.
    inputlength = strlen(input);
    while(((input[inputlength-1] == ' ') || (input[inputlength-1] == '\t')) && (inputlength > 0)){
      input[inputlength-1] = '\0';
      inputlength--;
    }

    //there will should be one file name entered, Therefore strtok twice, and the first one should contain the
    //filename and the second one should be empty.  If not, the user screwed up, so say so and exit.
    filetok = strtok(input, delim);      //specifies file name
    emptytok = strtok(NULL, delim);      //should be empty or else user screwed up
  }
  //valid input only it emptytok is empty, and filetok is not empty.
  if((emptytok == NULL) && (filetok != NULL)){
    strcpy(binstr, filetok);
    strcat (binstr, ".bin");      //make the .bin filename

    strcpy(mapstr, filetok);
    strcat (mapstr, ".map");      //make the .map filename

    fp = fopen(binstr, "rb");      //returns NULL if fails

    if (fp){
      while ((ch = getc(fp)) != EOF){  //get each byte from the file until the end of file is reached.
        CPUwritebyte(addr, (u8)ch);  //write the byte into the memory array starting at the first element
        addr ++;
      }
      fclose(fp);
    }
    else{
      printf("File %s not found. Please try again.\n", binstr);  //the file wasn't found. so quit.
      return;
    }
    
    //free any existing string table and label table entries
    ST_freeStringTable();
    ST_freeLabelTable();
    //reset the program counter in case something has been run already
    UI_pcReset(0);
    pcaddr = PCSTARTADDR;
    
    //initialize the number of current break points to zero 
    bpcount = 0;
    //initialize the list of break point addresses to -1 since 0 is a valid address.
    for (i = 0; i < 4; i ++){
      if (bpoints[i] != -1){
        CPUclrbreak(bpoints[i]);
        bpoints[i] = -1;
      }
    }
    
    fp = fopen(mapstr, "r");      //returns NULL if fails

    if (fp){                      //if there is a map file...          

      //generate the symbol table
      ST_makeSymbolTables(fp);
      //close the file.
      fclose(fp);

     }    
    else{          //map file not manditory. don't quit. can't use symbols later though.
      printf("File %s not found. You don't deserve symbols.\n", mapstr);
    }
    //print the first line of code wether there is a map file or not.
    MS_printfCodeLine(pcaddr);
  }
}

int main(void){
  
  int inputlength;
  int tokcount;
  int i;
  char input[1024];
  char previnput[1024];
  char *functok;
  char *emptytok;
  char *parameter[4];      //should never have more than 3 elements used, but what the hey - let's make it 4!
  char delim[] = " \t";

  CPUinit();
  ST_stringTableInit();

  //initialize the array of break points first off 
  for (i = 0; i < 4; i++){
    bpoints[i] = -1;
  }

  printf("\n--> ");

  while (fgets(input, 1023, stdin) ){

    if (input[0] == '\n'){
      //if user enters blank line, re-execute the previous command.
      //make the previous command also equal to the previous command (ie don't change it)
      strcpy(input, previnput);
    }
    else{  //if the user entered input, fix it up:

      //remove the carridge return from the end of the string.
      input[strlen(input)-1] = '\0';

      //get rid of any spaces or tabs at the end of the string.
      inputlength = strlen(input);
      while(((input[inputlength-1] == ' ') || (input[inputlength-1] == '\t')) && (inputlength > 0)){
        input[inputlength-1] = '\0';
        inputlength--;
      }

      strcpy(previnput, input);    //copy the current command into the previous command 
                                  //for possible use next time.
    }
      
    //there will be at most 4 input parameters entered by the user: setmem <addr> <b/w> <data>
    //so strtok will be used 5 times on the input string. The first 4 tokens created will/may
    //contain parameters, but the 5th one should always be empty. If the 5th one isn't empty, 
    //the user has entered too many parameters, and should be shot. Depending on the function
    //being called, the user may not enter all 4 parameters -- some functions only require 2 
    //or 3 tokens. The parameters may be separated by tabs or spaces.

    functok = strtok(input, delim);        //specifies which function to call
    parameter[0] = strtok(NULL, delim);    //parameter #1  ie address
    parameter[1] = strtok(NULL, delim);    //parameter #2  ie byte/word
    parameter[2] = strtok(NULL, delim);    //parameter #3  ie newvalue
    emptytok = strtok(NULL, delim);        //should always be empty.  if not, error!!

    //valid input only it emptytok is empty, and functok is not empty.
    //is possible that only functok has value if it is a "help" call...
    if ((emptytok == NULL) && (functok !=NULL)){
      
      //change all gathered input to all small letters
      MS_stringtolower(functok);

      tokcount = 0;
      if (parameter[0] != NULL){
        MS_stringtolower(parameter[0]);
        tokcount ++;
      }
      if (parameter[1] != NULL){
        MS_stringtolower(parameter[1]);
        tokcount ++;
      }
      if (parameter[2] != NULL){
        MS_stringtolower(parameter[2]);
        tokcount ++;
      }
      
      // determine which function to call
      if ((!strcmp(functok, "setmem")) || (!strcmp(functok, "sm"))){
        UI_setMem(parameter, tokcount);
      }
      else if ((!strcmp(functok, "readmem")) || (!strcmp(functok, "rm"))){
        UI_readMem(parameter, tokcount);
      }
      else if ((!strcmp(functok, "setreg")) || (!strcmp(functok, "sr"))){
        UI_setReg(parameter, tokcount);
      }
      else if ((!strcmp(functok, "readreg")) || (!strcmp(functok, "rr"))){
        UI_readReg(parameter, tokcount);
      }
      else if (!strcmp(functok, "run")){      //if they are too lazy to type run, then they shouldn't be using our simulator
        UI_run(parameter, tokcount);
      }
      else if ((!strcmp(functok, "loadmem")) || (!strcmp(functok, "load")) || (!strcmp(functok, "lm"))){
        UI_loadMem(parameter, tokcount);
      }
      else if ((!strcmp(functok, "trace")) || (!strcmp(functok, "tr"))){
        UI_trace(tokcount);
      }
      else if ((!strcmp(functok, "list")) || (!strcmp(functok, "li"))){
        UI_list(parameter, tokcount);
      }
      else if ((!strcmp(functok, "pcreset")) || (!strcmp(functok, "pcr"))){
        UI_pcReset(tokcount);
      }
      else if ((!strcmp(functok, "next")) || (!strcmp(functok, "nx"))){
        UI_next(tokcount);
      }
      else if ((!strcmp(functok, "setbreak")) || (!strcmp(functok, "sb"))){
        UI_setBreak(parameter, tokcount);
      }
      else if ((!strcmp(functok, "clrbreak")) || (!strcmp(functok, "cb"))){
        UI_clrBreak(parameter, tokcount);
      }
      else if ((!strcmp(functok, "setflag")) || (!strcmp(functok, "setflg")) || (!strcmp(functok, "sf"))){
        UI_setFlg(parameter, tokcount);
      }
      else if ((!strcmp(functok, "memdump")) || (!strcmp(functok, "md"))){
        UI_memDump(parameter, tokcount);
      }
      else if ((!strcmp(functok, "help")) || (!strcmp(functok, "?"))){
        UI_help(parameter, tokcount);
      }
      else if ((!strcmp(functok, "quit")) || (!strcmp(functok, "exit"))){
        CPUhalt();
        printf("Exiting.\n");
        return 0;
      }
      else{
        printf("Not a valid command name.\n");
      }
    }
    else if (emptytok != NULL){
      printf("Too many parameters buddy. Try again.\n");
    }
    printf("\n--> ");

  }//end outermost while

  printf("Exiting.\n");
  return 0;
}
