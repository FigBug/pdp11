#include "debugger.h"

void MS_printfCodeLine(u16 pcaddr){
  //pass in the pcaddr or address of whatever line you want to display. 
  //function displays the line of code as written by the programmer, located at the specified address.
  
  int cpuerror;
  u16 bytecode;
  char *codeline;
  char text[20];

	//get byte code for the instruction
	cpuerror = CPUreadword(pcaddr, bytecode);
	if (cpuerror == 1){		//something has gone wrong in CPU
		printf("Something had gone wrong. oops.\n");
		return;
	}
  
	//get string corresponding to the instruction pointed to by the pc
	codeline = ST_lookupString(pcaddr);	
	if (codeline == NULL)		                    //there was no string, so decompile it:
	{
    DECdecompile(pcaddr, text);
    printf("%4.4X %4.4X %s\n", pcaddr, bytecode, text);
 	}
	else
	{
		//print to screen:
		printf("%4.4X %4.4X %s\n",pcaddr, bytecode, codeline);
	}

}

int MS_lookupFlag(char *string, u8 &flag){
  //takes a string from the user, and the address of the flag value that will be passed back.
  //returns 1 if the string starts with a +, and 0 if string starts with a -.
  //returns -1 if the string is not valid.

  int value;

  if (string[0] == '+'){
    value = 1;
  }
  else if (string[0] == '-'){
    value = 0;
  }
  else{
    printf("You must specify whether to set or clear the flag. Please try again.\n");
    value = -1;
  }

  if (value >= 0){            //only continue if the value is valid.

    if (string[1] != 'f'){    //if the flag does not start with f, it is not a valid flag name.
      printf("Not a valid flag. Flags must start with the letter F. Please try again.\n");
      value = -1;
    }
    else if (value >= 0){                    //flag started with a valid letter.
      if ((string[2] == '0') || (string[2] == 'c')){
        flag = F0;
      }
      else if (((string[2] == '1') && (string[3] == 0)) || (string[2] == 'v')){
        flag = F1;
      }
      else if ((string[2] == '2') || (string[2] == 'z')){
        flag = F2;
      }
      else if ((string[2] == '3') || (string[2] == 'n')){
        flag = F3;
      }
      else if ((string[2] == '4') || (string[2] == 't')){
        flag = F4;
      }
      else if (string[2] == '5'){
        flag = F5;
      }
      else if (string[2] == '6'){
        flag = F6;
      }
      else if (string[2] == '7'){
        flag = F7;
      }
      else if (string[2] == '8'){
        flag = F8;
      }
      else if (string[2] == '9'){
        flag = F9;
      }
      else if ((string[2] == '1') && (string[3] != 0)){
        if (string[3] == '0'){
          flag = F10;
        }
        else if (string[3] == '1'){
          flag = F11;
        }
        else if (string[3] == '2'){
          flag = F12;
        }
        else if (string[3] == '3'){
          flag = F13;
        }
        else if (string[3] == '4'){
          flag = F14;
        }
        else if (string[3] == '5'){
          flag = F15;
        }
      }
      else{
        printf("Not a valid flag. Please try again.\n");
        value = -1;
      }
    }
  }
  return value;
}

void MS_printBinaryString(u16 value){
	int i;
	int mask;

	mask = 0x8000;
	for (i = 0; i < 16; i++){
		if (mask & value){
			printf("1");
		}
		else{
			printf("0");
		}
		mask = mask >> 1;
	}
}


/* convert a string to lower case */
void MS_stringtolower(char *string){

	while (*string != '\0'){
		*string = tolower(*string);
		string++;
	}
}

int MS_stringishex(char *string){
	//returns true if hex value
	//returns false if not hex value

	while (*string != '\0'){
		if ((isdigit(*string)) == 0){	//if its not (0-9)
			if ((*string != 'a') && (*string != 'b') && (*string != 'c') &&		//if it not a hex character
				(*string != 'd') && (*string != 'e') && (*string != 'f')){
				return false;			//not a valid hex value
			}
		}
		string++;					//is valid hex value -> test next element
	}
	return true;
}

int MS_stringisdigit(char *string){

	while (*string != '\0'){
		if ((isdigit(*string)) == 0){   //not a digit (0-9)
			return false;
		}
		string++;
	}
	return true;
}

int MS_isValidU8(char *string, char *base){
//returns a valid u16 or a negative number if something went wrong.

	int temp;
	int test;
	u8 value;

	if (!strcmp(base, "hex")){
		test = MS_stringishex(string);
	}
	else if (!strcmp(base, "dec")){
		test = MS_stringisdigit(string);
	}
	else{
		printf("Please specify the base.\n");	//shouldn't ever happen -- programmers a fool
		return (MIN_U8 - 1);
	}
	if (test == 1){								//string is valid format
		
		if (!strcmp(base, "dec")){				//scan string in in decimal form
			sscanf(string, "%d", &temp);
		}
		else if (!strcmp(base, "hex")){			//scan string in in hex form
			sscanf(string, "%x", &temp);
		}

		if ((temp < MAX_U8) && (temp >= MIN_U8)){	//a valid u8 value
			value = temp;
		}
		else{
			printf("Value is larger than 8 bits.\n");
			return (MIN_U8 - 1);
		}
	}
	else{
		printf("Not a valid value.\n");
		return (MIN_U8 - 1);
	}
	return value;
}

int MS_isValidU16(char *string, char *base){
//returns a valid u16 or a negative number if something went wrong.

	int temp;
	int test;
	u16 value;

	if (!strcmp(base, "hex")){
		test = MS_stringishex(string);
	}
	else if (!strcmp(base, "dec")){
		test = MS_stringisdigit(string);
	}
	else{
		printf("Please specify the base.\n");	//shouldn't ever happen -- programmers a fool
		return (MIN_U16 - 1);
	}
	if (test == 1){								//string is valid format
		
		if (!strcmp(base, "dec")){				//scan string in in decimal form
			sscanf(string, "%d", &temp);
		}
		else if (!strcmp(base, "hex")){			//scan string in in hex form
			sscanf(string, "%x", &temp);
		}

		if ((temp <= MAX_U16) && (temp >= MIN_U16)){	//a valid u16 value
			value = temp;
		}
		else{
			printf("Value is larger than 16 bits.\n");
			return (MIN_U16 - 1);
		}
	}
	else{
		printf("Not a valid value.\n");
		return (MIN_U16 - 1);
	}
	return value;
}


int MS_isValidS16(char *string, char *base){

	//returns a valid s16 (inside an int) if everything goes as planned.
	//returns MIN_S16-1 (inside an int) if ;
	//		-the base specified wasn't correct  -->  programmer is a fool.
	//		-if the value isn't a valid hex/dec value
	//		-value is not int he s16 range

	int temp;
	int test;
	s16 value;

	if (!strcmp(base, "hex")){
		test = MS_stringishex(string);
	}
	else if (!strcmp(base, "dec")){
		test = MS_stringisdigit(string);
	}
	else{
		printf("Please specify the base.\n");	//only happen in programmer is a fool.
		return (MIN_S16 - 1);
	}
	if (test == 1){								//string is valid format
		
		if (!strcmp(base, "dec")){				//scan string in in decimal form
			sscanf(string, "%d", &temp);
		}
		else if (!strcmp(base, "hex")){			//scan string in in hex form
			sscanf(string, "%x", &temp);
		}

		if ((temp < MAX_S16) && (temp >= MIN_S16)){	//a valid s16 value
			value = temp;
		}
		else{									//larger/smaller than a 16 signed value.
			return (MIN_S16 - 1);
		}
	}
	else{
		printf("Not a valid value.\n");
		return (MIN_S16 - 1);
	}
	return value;
}

char MS_valueAndType(char *string, int &value){
	int length;
	char lastchar;

	length = strlen(string);
	lastchar = string[length-1];

	if (lastchar == 'a'){					//address is always a u16 value - not a s16.
		string[length-1] = '\0';
		value = MS_isValidU16(string, "hex");
		if (value < MIN_U16){				//if return value isn't a valid u16 value, 
			return lastchar;			
		}
		//if it is a valid u16, keep going
		if ((value % 2) != 0){				//can ony run from even valued addresses
			value = MIN_U16 - 1;			//value not an even value ->return an invalid u16.
			printf("Not an even address location.\n");
			return lastchar;
		}
	}
	else if (lastchar == 'n'){				//number of lines is always a u16
		string[length-1] = '\0';
		value = MS_isValidU16(string, "dec");
		if (value < MIN_U16){
			return lastchar;
		}
	}
	else if (lastchar == 'i'){
		string[length-1] = '\0';
		value = MS_isValidS16(string, "dec");
		if (value < MIN_S16){				//if return value isn't a valid s16 value, 
			return lastchar;			
		}
	//if it is a valid s16, keep going	
	}
	else if (lastchar == 's'){
		string[length-1] = '\0';
		value = MS_isValidS16(string, "dec");
		if (value < MIN_S16){				//if return value isn't a valid s16 value, 
			return lastchar;			
		}
	//if it is a valid s16, keep going	
	}
	else{
		printf("Not a valid input value.\n");
		return -1;							//not a, i, or s.
	}
	return lastchar;
}