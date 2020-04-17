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
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#define N 32
#define MAXCHAR 50

sem_t sem;

struct
{
    pid_t pid;
}
g_proc[N];

int flag;

void do_packet_loop(char* data[], int size)
{
		
int fd;
int fd2;
int fd3;

while(1)
{pause();
switch(flag)
{
case 2:
{
int i=0;
int j=0;
int p_number = 0;
char buffer[1024];
char *buf;
char *pch[N]; for(i=0;i<N;i++) pch[i]=NULL;
char *args[N][MAXCHAR]; for(i=0;i<N;i++) for(j=0;j<MAXCHAR;j++) args[i][j]=NULL;
char *comms[N];
char *buffer1[MAXCHAR*N]; for(int i=0;i<MAXCHAR*N;i++) buffer1[i]=NULL;
sem_init(&sem,0,1);
fd = open("log.txt", O_CREAT|O_RDWR, S_IRWXU);
fd2 = open(data[1], O_CREAT|O_RDWR, S_IRWXU);
fd3 = open("results.txt", O_CREAT|O_RDWR, S_IRWXU);
read(fd2,buffer,1024);
close(fd2);
i=0;
buf = strtok(buffer,"#");
pch[i] = strtok(buf, "\n");
while(pch[i] != NULL)
{
	i=i+1;
	pch[i] = strtok(NULL,"\n");
}
for(i=0;i<N;i++) if(pch[i]!=NULL) p_number++;

for(i = 0; i<p_number;i++)
{
	buffer1[j] = strtok(pch[i]," ");
	comms[i]=buffer1[j];
	int counter = 0;
	while(buffer1[j]!=NULL)
	{
		counter++;
		j=j+1;
		buffer1[j] = strtok(NULL," ");
	}
	for(int k=0;k<counter;k++) args[i][k]=buffer1[j-counter+k+1];
}
char buf1[] = "Executed successfully\n";
char buf2[] = "\nFailure at command ";
char buf3[10];

pid_t pid2;
    for (i = 0; i < p_number; ++i)
    {
        g_proc[i].pid = fork();
 
        if (g_proc[i].pid == 0)
        {
			pid2 = fork();
			if (pid2==0)
			{
				close(1);
				dup2(fd3,1);
			if(execv(comms[i],args[i])<0)
				exit(1);
			}
			else
			{
			int status;
			wait(&status);
			sem_wait(&sem);
			if(WEXITSTATUS(status)!=1){
			write(fd,buf1,22);
			}
			else{
			write(fd,buf2,19);
			sprintf(buf3,"%d",i);
			write(fd,buf3,1);
			}
			sem_post(&sem);}
			
			exit(0);
		}else
		{
			int status2;
			wait(&status2);
		}
		
    }
sem_destroy(&sem);
flag = 0;
close(fd);
close(fd3);
break;
}
case 1:
{
exit(0);
}
}
}
}

void fsignal(int sig)
{
        switch(sig)
        {
                case SIGINT:
			flag = 2;
			break;
		
		case SIGUSR1:
			flag = 1;
			break; 
                default:
                        break;
        }
}

int main(int argc,char* argv[])
{
	
        if(fork())
                exit(0);
        setsid();
        signal(SIGINT,fsignal);
		signal(SIGUSR1,fsignal);
        do_packet_loop(argv,argc-1);
}
