#include<stdio.h> 
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 

int process_fork(int nproc) {
	int i;
	for(i=1; i<=nproc-1;i++) if(fork()==0) return(i);
	return(0);
}

void waitexample(int qt) 
{ 

	int n = 4;
	int cont = n;
	int i, j, c, stat, pid;
	int burstTime;
	int bt[4];

	for (j = 0; j<n ; j++){
		bt[j] = rand()%10;
	}

	pid = process_fork(n);
	burstTime = bt[pid];
	if (pid == 0){
		//Padre
		for(i = 0; i<n; i++){
			wait(&stat);
		}

		printf("Procesos terminados \n");
		exit(0);
	}

	else{
		while(burstTime > 0){
			for(c = 0; c < cont; c++){
					wait(&stat);
				}	
				printf("Proceso %d burstTime: %d\n", pid, burstTime);
			if (burstTime > qt){
				burstTime -= qt;
				sleep(qt);
							
			}
			else{
				sleep(burstTime);
				burstTime = 0;
				printf("Proceso %d burstTime: %d\n", pid, burstTime);
				printf("\nProceso %d terminado\n\n", pid);
				cont--;
				exit(0);
			}

		}
		
	}

} 
int cont = 4;
// Driver code 
int main() 
{ 
	//waitexample(4); 
	int n = 4;
	int qt;
	int i, j, c, stat, pid;
	int burstTime;
	int bt[4];

	printf("Ingrese Quantum:\n");
	scanf("%d", &qt);

	for (j = 0; j<n ; j++){
		bt[j] = rand()%10;

	}

	pid = process_fork(n);
	burstTime = bt[pid];
	if (pid == 0){
		//Padre
		for(i = 0; i<n; i++){
			wait(&stat);
		}

		printf("Procesos terminados \n");
		exit(0);
	}

	else{
		while(burstTime > 0){
				printf("Proceso %d burstTime: %d\n", pid, burstTime);
			if (burstTime > qt){
				burstTime -= qt;
				sleep(qt);
							
			}
			else{
				sleep(burstTime);
				burstTime = 0;
				printf("Proceso %d burstTime: %d\n", pid, burstTime);
				printf("\nProceso %d terminado\n\n", pid);
				cont--;
				exit(0);
			}
		}	
	}
	return 0; 
} 