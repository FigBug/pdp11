#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

int pid;

void sighndlr(int signo)
{
	printf("Clock Shutting down\n");
	kill(pid, SIGUSR2);
}

int main(void)
{
	struct timespec sleeptm;

	printf("PDP-11 Clock Emulator\n\n");
	printf("The followind PDP-11 emulators are running:\n");

  system("ps | grep pdp11 | grep -v grep");

	printf("Enter PID of emulator to clock -> ");

	scanf("%d", &pid);
  if (pid < 1) return 0;

	printf("PDP-11 Clock Enabled: CTRL+C to kill\n");

	sleeptm.tv_sec = 0;
	sleeptm.tv_nsec = 100000000L;

	signal(SIGINT, sighndlr);

	while (1)
	{
	  kill(pid, SIGUSR1);

		if (nanosleep(&sleeptm, NULL) == -1)
		{
			return 0;
	  }
	}
	
	return 0;
}
