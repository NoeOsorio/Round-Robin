#include<stdio.h> 
#include <string.h>
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include <signal.h>

int readQT(){
	int qt;
	printf("Ingrese Quantum:\n");
	scanf("%d", &qt);
	return qt;
}

int process_fork(int nproc) {
	int i;
	for(i=1; i<=nproc-1;i++) if(fork()==0) return(i);
	return(0);
}

void getProcesos(int *p){
	int *tmp = p;
	int buf; FILE *fp;
	if((fp=fopen("procesos.txt","r"))==NULL){
		printf("error al abri el archivo");
		exit(0);
	}
	else {
		system("sort procesos.txt -o procesos.txt");
		//printf("buscando archivo \n");
		while(!feof(fp)){
			fscanf(fp,"%d",&buf);
				*p = buf;
				//printf("%d\n", *p);
				p++;
				
			}
			
		fclose(fp);
	}
	p = tmp;
	return;
}

int getAProcess(int n, int *procesos, int exception){
	int *tmp = procesos;
	int something = 0;
	int first = 0;
	int band = 0;
	for (int i = 0; i < n; i++){
		if(*procesos != 0 && *procesos != exception){
			something = *procesos;
			//printf("Something: %d \n", something);
			if(something < exception && band == 0){
				first = something;
				band = 1;
			}
			else if (something > exception){
				procesos = tmp;
				break;
			}
			
		}
		procesos++;
	}
	if (first > something || something < exception){
		something = first;
	}

	procesos = tmp;
	//printf("Next: %d \n", something);
	return something;
	
}

void deleteProcess(int n, int *procesos, int delete){
	int *tmp = procesos;
	char command[50];
	system("rm procesos.txt");
	for (int i = 0; i < n-1; ++i)
	{
		if (*procesos != delete){
			sprintf(command, "echo %d >> procesos.txt", *procesos);
			system(command);
		}
		else{
			system("echo 0 >> procesos.txt");
			//printf("Se elimino proceso %d\n\n", *procesos);
		}
		procesos++;
	}
	procesos = tmp;
}


void RR(int qt, int n, int *proc) 
{ 
	char command[50];
	int turn = 1;
	int i, j, c, stat, pid, buf;
	int burstTime;
	int bt[10];

	//Asigna de forma aleatoria los BurstTime
	for (j = 0; j<n ; j++){
		bt[j] = rand()%10;
	}

	//Crea los procesos
	pid = process_fork(n);
	printf("Soy Proceso %d PID: %d\n",pid, getpid() );

	//Se les asigna un BurstTime
	burstTime = bt[pid];

	//Se guardan los PID`s

	
	if (pid != 0){
		//Crea el archivo
		sprintf(command, "echo %d >> procesos.txt", getpid());
		//printf("%s\n", command);
		system(command);
		sleep(1);
		
		if(pid != 1){
			kill(getpid(), SIGSTOP);
		}
	}
	
	
	


	//Aqui es donde esta la magia
	if (pid == 0){
		//Padre
		sleep(3);
		//Comunicacion entre procesos
		getProcesos(proc);

		for(i = 0; i<n; i++){
			wait(&stat);
		}

		printf("Procesos terminados \n");
		exit(0);
	}

	//Hijos
	else{
		//printf("Soy Proceso %d PID: %d\n",pid, getpid() );
		
		
		while(burstTime > 0){
				//Comunicacion entre procesos
				getProcesos(proc);
				printf("\nProceso %d burstTime: %d\n", pid, burstTime);
			if (burstTime > qt){
				burstTime -= qt;
				//Simula el proceso
				sleep(qt);

				if ((turn = getAProcess(n,proc,getpid())) != 0){
						//printf("Proceso %d se despierta\n", turn);
						kill(turn, SIGCONT);
						//printf("Proceso %d se va a dormir\n", getpid());
						kill(getpid(), SIGSTOP);
				}
			}
			else{
				sleep(burstTime);
				burstTime = 0;
				//printf("Proceso %d burstTime: %d\n", pid, burstTime);
				printf("Proceso %d terminado\n\n", pid);
				
				if ((turn = getAProcess(n,proc,getpid())) != 0){
						//printf("Proceso %d se despierta\n", turn);
						deleteProcess(n, proc, getpid());
						kill(turn, SIGCONT);
						
				}
				else{
					deleteProcess(n, proc, getpid());
				}

				exit(0);
			}

		}
		
	}

} 


int main() 
{ 
	int proc[10], *p;
	int qt;

	//Aqui se van a guardar los id de procesos que se encuentran en procesos.txt
	p = proc;

	//Limpia el archivo
	system("rm procesos.txt");

	//Lee el quantum
	qt = readQT();

	//Magia
	RR(qt, 4, p); 

	return 0; 
} 