#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
bool signal_flag = false;

void do_packet_loop(char* data[])
{
	int fd;
	int fd2;
	char buf[] = "\nGot signal at ";
	char *argv2[] = { "thereez","/",NULL };
	char *buffer[] = {};
	while (1)
	{
		pause();
		if (signal_flag)
		{
			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			fd = open("dmn.txt", O_CREAT | O_RDWR, S_IRWXU);
			fd2 = open(data[1], O_CREAT | O_RDWR, S_IRWXU);
			read(fd2, buffer[0], 7);
			close(1);
			dup2(fd2, 1);
			execve(buffer[0], argv2, NULL);
			lseek(fd, 0, SEEK_END);
			write(fd, buf, sizeof(buf) - 1);
			write(fd, asctime(timeinfo), 25);
			close(fd);
			signal_flag = false;
		}
	}
}

void fsignal(int sig)
{
	switch (sig)
	{
	case SIGINT:
		signal_flag = true;
		break;

	case SIGUSR1:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{

	if (fork())
		exit(0);
	setsid();
	signal(SIGINT, fsignal);
	signal(SIGUSR1, fsignal);
	printf("daemon started\n");
	int fd;
	char buf[] = "\nDaemon started at ";
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fd = open("dmn.txt", O_CREAT | O_RDWR, S_IRWXU);
	lseek(fd, 0, SEEK_END);
	write(fd, buf, sizeof(buf) - 1);
	write(fd, asctime(timeinfo), 25);
	close(fd);
	do_packet_loop(argv);
}
