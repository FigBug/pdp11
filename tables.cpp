#include "debugger.h"


#define MAX_STRINGS  28672

//STRING TABLE:
  //datastructure is an array. 
  //To find the string for given memory address, take the addr passed in and divide it by 2.
  //This gives the array index of the location of the pointer to the desired string.
  //This works since there are never any lines of code stored at odd memory address.
  //The array of strings is half as big as the array of memory.
  //The program gets an address from the user, divides it by 2, and uses it as an array index 
  //for the stringTable array to get the string associated with the memory address.
  //There are 28672 elements because there is 64K bytes in total memory, therefore 32K words,
  //but the 4K words are reserved for i/o. Therefore there are 32-4=28K words of usable space...
  //->28(1024) = 28672 elements.

char *stringTable[MAX_STRINGS];

//LABEL TABLE:
  //datastructure is a linked list of cells.
  //List is sorted alpahbetically, so that it is easier to determine if a label is already in the list.
  //User refers to a label, the program looks up the label in the label table, 
  //retrieves the address associated with the label.
  //screw the alphabetical stuff, we've got resources to spare!

list *head;

int ST_isLabel(char *string){
  //string is a string containing everything after the memory address 
  //  --the memory address and the leading space have already been cut off.
  //a label is between 1 and 8 characters long, and followed by a colon.
  //if I come accross a space or I count 9 elements before I find a colon, then 
  //the string does not start with a label.
  //  --return -1.
  //if I find a colon before I get to nineth element or a space, then the preceding is a label.
  //  --return the number of elements past the start of the string.
  //what happens when the colon is the first element of the string????
  //  --that is the assemblers problem. It shouldn't ever happen.
  //  --just make sure it doesnt crash the debugger -> return -1 for something to do.
  //    --undefined behaviour.

  int n = 0;
  
  if ((n == 0) && (string[0] == ':')){  //that is a big problem from assembler!!
    return -1;
  }

  while ((string[n] != '\0') && (n < 9)){
    if (string[n] == ':'){   
      return (n);          //location of the end of the label string (ie location of the :)
    }
    else if (string[n] == ' '){
      return -1;          //found space before checked 9 elements (no label)
    }
    n++;
  }
  return -1;              //didn't find a space or colon in the first 9 elements (no label)
}

void ST_addLabel(char *label, u16 addr){
  //create a list item, and add the label and address to the label table list.
  //list is not sorted alphabetically, so just put the label on the front of the list

  list *newlabel;

  newlabel = (list *)malloc(sizeof(struct list));

  strcpy(newlabel->label, label);
  newlabel->addr = addr;
  newlabel->next = head;
  
  head = newlabel;
}

bool ST_lookupLabel(char *findlabel, u16 &findaddr){
  //takes a string to compare to those in the label table, 
  //passes back a u16 address corresponding to the label if it is found.
  //returns true if the label was found, false if the label wasn't found.
 
  list *current;
  int length;
  bool retval;

  retval = false;               //default value-> if label not found, or no labels, return false.

  if (head != NULL){
    //there are labels in the label table, so look for the right one!
    current = head;
    length = strlen(findlabel);

    while (current != NULL){
      
      //if they are the same, we have a winner!
      if (!strncmp(current->label, findlabel, length)){    
        findaddr = current->addr;
        return true;
      }
      //if they are not the same, try the next one.
      else{
        current = current->next;
      }
    }
  }
  return retval;
}

void ST_stringTableInit(void){

  int i;

  for (i = 0; i < MAX_STRINGS; i ++){
    stringTable[i] = NULL;
  }
}

void ST_addString(char *string, u16 addr){
  //there shouldn't ever be anything already stored at that address 
  //unless you are writing self-modifying code...

  int n;

  n = addr / 2;
  stringTable[n] = strdup(string);
}

char *ST_lookupString(u16 addr){
  //send it the address and return the string associated with that addess 
  //(ie the one at stringTable[addr/2])
  
  int n;
  char *string;

  n = addr / 2;
  string = stringTable[n];

  return string;
}

void ST_freeLabelTable(void){
  //free all strings in the label tabel and initialize the head of the list to NULL.
  list *current;

  while (head != NULL){
    current = head;
    head = current->next;
    free(current);
  }
}

void ST_freeStringTable(void){
  //free all strings in the string table and initialize each possible location to NULL.

  int i;

  for (i = 0; i < MAX_STRINGS; i++){

    if (stringTable[i]){      //if there is a string at the location in the string table
      free (stringTable[i]);
      stringTable[i] = NULL;
    }
  }
}

void ST_makeSymbolTables(FILE *fp){
  
  u16 memaddr;
  int tempaddr;
  int offset;
  int fulllinelen;
  char fullline[1024];
  char labelline[1024];
  char *label;
  char *restofline;
  char *memtok;
  char delim[] = " \t";

  while (fgets(fullline, 1023, fp) != NULL){    //get a full line from the file until end of file.

    //remove the carridge return from the end of the string.
    fullline[strlen(fullline)-1] = '\0';

    if (fullline[0] == '\0'){
      return;
    }

    //get rid of any spaces or tabs ar the end of the string.
    fulllinelen = strlen(fullline);
    while(((fullline[fulllinelen-1] == ' ') || (fullline[fulllinelen-1] == '\t')) && (fulllinelen > 0)){
      fullline[fulllinelen-1] = '\0';
      fulllinelen--;
    }

    //cut the memory address off the front of the full line. It will always be the first 4 characters of the line.
    memtok = strtok(fullline, delim);    //get the address off the front of the string
    sscanf(memtok, "%x", &tempaddr);      
    memaddr = tempaddr;                  //put the address in a the u16 variable 

    //check if there is a label at the beginning of the string.
    //memtok always going to be 4 elements, since there is only a 16 bit address space.
    offset = ST_isLabel(fullline + 5);

    //if there was a label, restofline is a pointer to the non-label part of the string.
    if (offset != -1){
      strcpy(labelline, &fullline[5]);
      label = labelline;                //set label to start after the space after the memaddr.
      label[offset] = '\0';              //make the last element of the offset a \0 instead of a :
      MS_stringtolower(label);          //make the label all lower case.
      ST_addLabel(label, memaddr);      //put the label in the label table. If assembler is proper, there will be no duplicates.
    }

    restofline = fullline + 5;          //set restofline to start after the space after the memaddr.
    ST_addString(restofline, memaddr);  //put the line of code in the string table, at corresponding addr.
  }
}
