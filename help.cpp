#include "debugger.h"

void UI_help(char *parameter[], int n){
  //help can take either 0 parameters, or 1 parameter.
  //if 0:    list valid commands and a message stating how to get specific help on one at a time.
  //if 1:    determine if it is a valid command, then display the help specific to the command.

  if (n == 0){
    printf("List of valid pdp-11 commands:\n\n");
    printf("\tCLRBREAK\tHELP\t\tLIST\n");
    printf("\tLOADMEM\t\tMEMDUMP\t\tNEXT\n");
    printf("\tPCRESET\t\tREADMEM\t\tREADREG\n");
    printf("\tRUN\t\tSETBREAK\tSETFLG\n");
    printf("\tSETMEM\t\tSETREG\t\tTRACE\n\n");
    printf("For more information on a command listed above, type:\n\n");
    printf("\t\tHELP <command name>\tor\n\t\t? <command name>\n");
  }
  
  else if (n == 1){

    if ((!strcmp(parameter[0], "clrbreak")) || (!strcmp(parameter[0], "cb"))){
      printf("\nThe 'clrbreak' command clears a break point located at the memory addres\n");
      printf("specified by the parameters. There is only one (1) parameter for this command.\n");
      printf("The user may choose to clear one break point at a time, or all the break points\n");
      printf("at once. The command syntax is a follows:\n\n");
      printf("\t\tclrbreak <label/line number/memory address/all>\n\n");
      printf("<label> : label that corresponds to a memory address.\n");
      printf("<line number> : line number of in the code.\n");
      printf("<memory address> : memory address corresponding to an instruction in the code.\n");
      printf("<all> : clears all the break points currently set in the current program.\n");
      printf("\nAll break points are cleared upon loading a new file with the loadmem function.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "help")) || (!strcmp(parameter[0], "?"))){
      printf("\nThe 'help' command displays the a list of the valid pdp-11 debugger commands.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "list")) || (!strcmp(parameter[0], "li"))){
      printf("\nThe 'list' command displays a listing of the programmer's code, the memory\n");
      printf("address it is stored at, and the opcode for the instruction. ");
      printf("It takes up\nto two (2) parameters, both of which are optional.\n\n");
      printf("\t\tlist <from address>a <number of lines>n\n\n");
      printf("<from address> : location of the 'middle' line of code displayed.\n");
      printf("\tMust be followed immediately by an 'a'.\n");
      printf("<number of lines> : number of lines displayed before and after the middle line\n");
      printf("\tMust be followed immediately by a 'n'.\n\n");
      printf("The default values:\n");
      printf("\t<from address> = current location of the program counter,\n\t<number of lines> = 10.\n\n");
      printf("Parameters may be entered in any order, but each must be followed by the \ncorrect subscript -- either an 'a' or 'n'.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "loadmem")) || (!strcmp(parameter[0], "lm")) || (!strcmp(parameter[0], "load"))){
      printf("\nThe 'loadmem' command loads a program into memory. ");
      printf("It takes one optional\nparameter: <file name>\n\n");
      printf("\t\tloadmem <file name>\n\n");
      printf("If <file name> is not specified, the user will be prompted on the next line.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "memdump")) || (!strcmp(parameter[0], "md"))){
      printf("\nThe 'memdump' functiopn displays the contents of the specified block of memory.\n");
      printf("The contents are displayed as bytes, with 16 bytes to a line. The command\n");
      printf("takes two parameters: the starting address and the ending address. Both \n");
      printf("parameters are optional.\n\n");
      printf("\t\tmemdump <startaddr> <endaddr>\n\n");
      printf("<startaddr> : memory address to start displaying from. The address must be\n");
      printf("\tentered in hex format, unless it is a valid label.\n");
      printf("<endaddr> : memory address to stop displaying at. The address must be entered\n");
      printf("\tin hex format, unless it is a valid label.\n");
      printf("\nIf only one parameter is provided, it is used as the start address, and the\n");
      printf("default number of lines (10 lines) is displayed. If no paramters are provided,\n");
      printf("the defaut number of lines (10 lines) is displayed, starting from the current\n");
      printf("location of the program counter.\n");
  
      return;
    }
    else if ((!strcmp(parameter[0], "next")) || (!strcmp(parameter[0], "nx"))){
      printf("The 'next' command steps through the program one instruction at a time.\n");
      printf("The current instruction is executed, then the next line of code is displayed.\n");
      printf("If the instruction to be executed is a 'jsr', the program executes the\n");
      printf("subroutine and stops at the intruction that follows the 'jsr' in the\n");
      printf("programmer's code. There are no parameters for this command.\n\n");
      printf("\t\tnext\n");
      return;
    }
    else if ((!strcmp(parameter[0], "pcreset")) || (!strcmp(parameter[0], "pcr"))){
      printf("\nThe 'pcreset' command resets the program counter to the starting address of\n");
      printf("the program. This command takes no parameters.\n\n");
      printf("\t\tpcreset\n");
      return;
    }
    else if ((!strcmp(parameter[0], "readmem")) || (!strcmp(parameter[0], "rm"))){
      printf("\nThe 'readmem' command reads and displays a piece of memory specified by the\n");
      printf("parameters. There are two (2) parameters, both of which are manditory:\n\n");
      printf("\t\treadmem <memory address> <byte/word (or b/w)>\n\n");
      printf("<memory address> : the address of the memory to be read.\n");
      printf("\tIf reading a 'word', the address must be an even number.\n");
      printf("\tThe address must be entered in hex format, unless it is a valid label.\n");
      printf("<byte/word> : specifies whether reading one (1) byte or two (2) bytes - a word.\n");
      printf("\tCan enter byte or b, or word or w.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "readreg")) || (!strcmp(parameter[0], "rr"))){
      printf("\nThe 'readreg' command reads and displays the contents of the register\n");
      printf("specified by the one parameter. If no parameters are given, the contents\n");
      printf("of all the registers  \n\n");
      printf("\t\treadreg <register>\n\n");
      printf("<register> : the register to be read.\n");
      printf("\tValid register ids are: r0 - r7, pc (r7), sp (r6), psw.\n");
      return;
    }
    else if (!strcmp(parameter[0], "run")){
      printf("\nThe 'run' command runs the program in memory. There are three (3) parameters\n");
      printf("that are all optional:\n\n");
      printf("\trun <starting memory address>a <time>s <number of instructions>n\n\n");
      printf("<starting memory address> : memory address to start execution at.\n");
      printf("\tThe address must be entered in hex format, unless it is a valid label,\n");
      printf("\tand immediately followed by an 'a'.\n");
      printf("<time> : the maximum number of seconds that the program to execute for.\n");
      printf("\tThe time must be entered in decimal format, and immediately followed\n");
      printf("\tby a 's'.\n");
      printf("<number of instructions> : the maximun number of instructions to be executed\n");
      printf("\tbefore stopping.\n");
      printf("\tThe number of instructions must be entered in decimal format, followed\n");
      printf("\timmeditately by a 'n'\n");
      printf("\nThe parameters can be entered in any order, but must be followed by the\n");
      printf("proper subscript.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "setbreak")) || (!strcmp(parameter[0], "sb"))){
      printf("\nThe 'setbreak' command sets a break point at the memory address specified by\n");
      printf("the address given in the parameters. A maximum of four (4) break points can be\n");
      printf("set in a given program. Only one break point can be set at a time.\n\n");
      printf("\t\tsetbreak <label/line number/memory address>\n\n");
      printf("<label> : a label that corresponds to a memory address.\n");
      printf("<line number> : a line number of in the code.\n");
      printf("<memory address> : a memory address corresponding to an instruction in the code.\n");
      printf("\nAll break points are cleared upon loading a new file with the loadmem function.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "setflg")) || (!strcmp(parameter[0], "sf")) || (!strcmp(parameter[0], "setflag"))){
      printf("\nThe 'setflg' command sets or clears an ALU status flag(s) in the Process\n");
      printf("Status Word (psw). The flag(s) to be changed are specified by the parameters.\n");
      printf("The user can change at most three (3) flags at once.\n\n");
      printf("\t\tsetflag <+/-Fx> <+/-Fx> <+/-Fx>\n\n");
      printf("<+> : sets the specified flag (assigns value of 1.\n");
      printf("<-> : clears the specified flag (assigns value of 0.\n");
      printf("<Fx> : specifies the flag to be changed.\n");
      printf("\tValid flags are: FC, FV, FZ, FN, FT, F0 - F16.\n");
      printf("\nThe flags can be entered in any order, with a maximum of three (3) flags per\n");
      printf("command.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "setmem")) || (!strcmp(parameter[0], "sm"))){
      printf("\nThe 'setmem' command sets a piece of memory specified by the parameters.\n");
      printf("There are three (3) parameters, all of which are manditory:\n\n");
      printf("\t\tsetmem <memory address> <byte/word (or b/w)> <new value>\n\n");
      printf("<memory address> : the address of the memory to be set.\n");
      printf("\tIf setting a 'word', the address must be an even number.\n");
      printf("\tThe address must be entered in hex format, unless it is a valid label.\n");
      printf("<byte/word> : specifies whether setting one (1) byte or two (2) bytes - a word.\n");
      printf("\tCan enter byte or b, or word or w.\n");
      printf("<new value> : the 8 or 16 bit value to be written to the address.\n");
      printf("\tThe new value must be entered in hex format.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "setreg")) || (!strcmp(parameter[0], "sr"))){
      printf("\nThe 'setreg' command set the contents of the register specified by the\n");
      printf("the parameters.\n\n");
      printf("\t\tsetreg <register> <new value>\n\n");
      printf("<register> : the register to be read.\n");
      printf("\tValid register ids are: r0 - r7, pc (r7), sp (r6), psw.\n");
      printf("<new value> : the 16 bit value to be written to the specified register.\n");
      printf("\tThe value must be entered in hex format.\n");
      return;
    }
    else if ((!strcmp(parameter[0], "trace")) || (!strcmp(parameter[0], "tr"))){
      printf("The 'trace' command steps through the program one instruction at a time.\n");
      printf("The current instruction is executed, then the next line of code is displayed.\n");
      printf("There are no parameters for this command.\n\n");
      printf("\t\ttrace\n");
      return;
    }
    else{
      printf("Not a valid command. See the general help (type help) for a list of valid commands\n");
      return;
    }
  }
  else{
    printf("You entered the wrong number of parameters. Try again.\n");
    return;
  }  
}