#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <poll.h>

static struct termios stored;

int in;
int out;

char outPipeName[64];
char inPipeName[64];

void sighndlr(int signo)
{
	signal(SIGINT, sighndlr);
}

void input_process(int fpid)
{
	char buffer[1];
	char last;

	while (1)
	{
		read(0, buffer, 1);
		write(in, buffer, 1);

		if (last == '~' && buffer[0] == '.')
		{
		  kill(fpid, SIGKILL);
			close(in);
			close(out);

      remove(outPipeName);
      remove(inPipeName);
			printf("\n\n");
			
			_exit(0);
		}
		last = buffer[0];
	}
}

void output_process(void)
{
  char buffer[1];

	while (1)
	{
		if (read(out, buffer, 1) == 0)
    {
      _exit(0);
    }
		printf("%c", buffer[0]);
		fflush(stdout);
	}
}

void setterm(void)
{
	struct termios newtrm;
	tcgetattr(0,&stored);
	memcpy(&newtrm, &stored, sizeof(struct termios));
	newtrm.c_lflag &= (~ECHO);
	newtrm.c_lflag &= (~ICANON);
	newtrm.c_cc[VTIME] = 0;
	newtrm.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&newtrm);
}

void resetterm(void)
{
	tcsetattr(0,TCSANOW,&stored);
}

int main(void)
{
	int pid;
  int fpid;

	printf("PDP-11 Terminal Emulator\n\n");
	printf("The following PDP-11 emulators are running:\n");

	system("ps | grep pdp11 | grep -v grep");

	printf("Enter PID of emulator to connect to -> ");

	scanf("%d", &pid);
	if (pid < 1) return 0;

	printf("Syncing with output\n");
  sprintf(outPipeName, "pdp%d_out", pid);
	do
	{
	  out = open(outPipeName, O_RDONLY);
		sleep(1);
	} while (out < 0);

	printf("Syncing with input\n");
	sprintf(inPipeName, "pdp%d_in", pid);
	do 
	{
		in = open(inPipeName, O_WRONLY);
		sleep(1);
	} while (in < 0);

  printf("\n\nTerminal up and running\n");
	printf("~. kills terminal (do not use CTRL+C)\n");

	setterm();

	fpid = fork();
  
  signal(SIGINT, sighndlr);

	if (fpid < 0)
	{
		printf("forking error.\n");
		return 0;
	}
	else if (fpid == 0)
	{
		output_process();	
	}
	else
	{
		input_process(fpid);
	}
	
	return 0;
}

