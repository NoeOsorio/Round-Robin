#include<stdio.h> 
#include<string.h>
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include<signal.h>

int isFisrtTime = 1;

struct proceso{
	int id;
	int yaPaso;
	int prioridad;
	int bt;
}procA[10], *p;

//Declaracion de funciones
void getPrioridad(int n);

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
void sort(int n){
	struct proceso t;
	//Sort
	int d;
	for (int c = 1 ; c < n - 1; c++) {
	    d = c;
	 
	    while ( d > 0 && procA[d-1].bt > procA[d].bt) {
	      t          = procA[d];
	      procA[d]   = procA[d-1];
	      procA[d-1] = t;
	 
	      d--;
	    }
	}
	//return procA;
}
void sortRR(int n){
	struct proceso t;
	//Sort
	int d;
	for (int c = 1 ; c < n - 1; c++) {
	    d = c;
	 
	    while ( d > 0 && procA[d-1].id > procA[d].id) {
	      t          = procA[d];
	      procA[d]   = procA[d-1];
	      procA[d-1] = t;
	 
	      d--;
	    }
	}
	//return procA;
}
void setProcesos(int n){
	char command[50];
	system("rm procesos.txt");
	for (int i = 0; i < n-1; ++i)
	{
		sprintf(command, "echo %d %d %d %d >> procesos.txt", (procA[i].prioridad) , (procA[i].id), (procA[i].bt), (procA[i].yaPaso));
		system(command);
	}
}
void getProcesos(int n){
	int cont = 0, i = 0;
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
				(procA[i].prioridad) = buf;
				//printf("%d\n", procA[i].id);
			fscanf(fp,"%d",&buf);
				(procA[i].id) = buf;
				//printf("%d\n", procA[i].prioridad);
			fscanf(fp,"%d",&buf);
				(procA[i].bt) = buf;
			fscanf(fp,"%d",&buf);
				(procA[i].yaPaso) = buf;	
				//printf("%d\n", procA[i].id);	
			i++;
			cont++;	
			if (cont >= n)
				break;			
		}
			
		fclose(fp);
	}
	return;
}
int getAProcess(int n, struct proceso exception){
	struct proceso something, anterior, sig, same;
	int band = 0, isEmpty = 1;

	something.id = 0;
	something.prioridad = 0;
	something.bt = 0;

	for (int i = 0; i < n; ++i)
	{
		if(procA[i].yaPaso == 0 && procA[i].id != 0){
			isEmpty = 0;
			
			break;
		}
	}
	//printf("isEmpty = %d \n", isEmpty);
	if(isEmpty == 1){

		system("echo 0 > ft.txt");
		getPrioridad(n);
	}


	for (int i = 0; i < n; i++){
		if ((procA[i].id) == exception.id){
			//Encuentra el mismo
			same = exception;
		}
		if(((procA[i].id) != 0) && ((procA[i].id) != exception.id) && ((procA[i].id) > 100) && (procA[i].yaPaso == 0)){
			isEmpty = 0;
			something = procA[i];
			//printf("Something: %d con prioridad %d \n", something.id, something.prioridad);
			if(something.prioridad < exception.prioridad ){
				anterior = something;
				break;
			}
			else if (something.prioridad > exception.prioridad && band == 0){
				isEmpty = 0;
				sig = something;
				band = 1;
			}
			
		}
	}
	if (something.prioridad >= n){
		for (int i = 0; i < n; ++i)
		{
		 	if(procA[i].bt > 0){
		 		something = procA[i];
		 		break;
		 	}
		} 
	}

	else if (anterior.prioridad > 0 && anterior.prioridad < something.prioridad){
		something = anterior;
		//printf("Anterior: %d con prioridad %d \n", something.id, something.prioridad);
	}
	else if (something.prioridad > exception.prioridad)	{
		something = sig;
		//printf("Siguiente: %d con prioridad %d \n", something.id, something.prioridad);
	}
	else if (isEmpty == 1){
		something = same;
		//printf("El mismo: %d con prioridad %d \n", something.id, something.prioridad);
		for (int i = 0; i < n; i++){
			if ((procA[i].id) == exception.id){
				//Encuentra el mismo
				same = exception;
			}
			if(((procA[i].id) != 0) && ((procA[i].id) != exception.id) && ((procA[i].id) > 100) && (procA[i].yaPaso == 0)){
				isEmpty = 0;
				something = procA[i];
				//printf("Something: %d con prioridad %d \n", something.id, something.prioridad);
				if(something.prioridad < exception.prioridad ){
					anterior = something;
					break;
				}
				else if (something.prioridad > exception.prioridad && band == 0){
					isEmpty = 0;
					sig = something;
					band = 1;
				}
				
			}
		}
		if (something.prioridad >= n){
			for (int i = 0; i < n; ++i)
			{
			 	if(procA[i].bt > 0){
			 		something = procA[i];
			 		break;
			 	}
			} 
		}

		else if (anterior.prioridad > 0 && anterior.prioridad < something.prioridad){
			something = anterior;
			//printf("Anterior: %d con prioridad %d \n", something.id, something.prioridad);
		}
		else if (something.prioridad > exception.prioridad)	{
			something = sig;
			//printf("Siguiente: %d con prioridad %d \n", something.id, something.prioridad);
		}
		else if (isEmpty == 1){
			something = same;
			//printf("El mismo: %d con prioridad %d \n", something.id, something.prioridad);
		}
	}

	//printf("Next: %d \n", something.id);
	return something.id;	
}
struct proceso updateSelf(int n, struct proceso self){

	for (int i = 0; i < n-1; ++i)
	{
		if(procA[i].id == self.id){
			self = procA[i];
			break;
		}
	}
	return self;
}
struct proceso updateBT(int n, struct proceso self){
	for (int i = 0; i < n-1; ++i)
	{
		if(procA[i].id == self.id){
			procA[i].bt = self.bt;
			self = procA[i];
			break;
		}
		
	}
	return self;
}
struct proceso updateYaPaso(int n, struct proceso self){

	for (int i = 0; i < n-1; ++i)
	{
		if(procA[i].id == self.id){
			procA[i].yaPaso = self.yaPaso;
			self = procA[i];
			break;
		}
	}
	return self;
}
void setPrioridad(int n){
	if(isFisrtTime == 0)
		sort(n);
	else
		sortRR(n);


	int prio = 1, band = 0;
	for (int i = 0; i < n-1; ++i)
	{
		if (procA[i].yaPaso == 1){
			prio++;
		}
		if (procA[i].bt != 0 && procA[i].yaPaso == 0){
			band = 1;
			procA[i].prioridad = prio;
			prio++;
		}
	}
	//printf("Prioridad maxima %d, %d\n", prio, band);

	if (band == 0){
			for (int i = 0; i < n-1; ++i)
			{
				procA[i].yaPaso = 0;
			}
	}
}
void getPrioridad(int n){
	getProcesos(n);
	setPrioridad(n);
	setProcesos(n);
	//setProcesos(n, p);
	return;
}
void deleteProcess(int n, int delete){
	char command[50];
	system("rm procesos.txt");
	for (int i = 0; i < n-1; ++i)
	{
		if ((procA[i].id) == delete){
			(procA[i].prioridad) = 0;
			(procA[i].id) = 0;
			(procA[i].bt) = 0;

			//printf("Se elimino proceso %d\n\n", *procesos);
		}
		sprintf(command, "echo %d %d %d %d >> procesos.txt", (procA[i].prioridad) , (procA[i].id), (procA[i].bt), (procA[i].yaPaso));
		system(command);

	}
}
int getFT(){
	int ft;
	int buf; FILE *fp;
	if((fp=fopen("ft.txt","r"))==NULL){
		printf("error al abri el archivo");
		exit(0);
	}
	else {
		//printf("buscando archivo \n");
		while(!feof(fp)){
			fscanf(fp,"%d",&buf);
			ft = buf;
			break;			
		}
			
		fclose(fp);
	}
	return ft;
}

void RR(int qt, int n) 
{ 
	
	char command[50];
	int turn = 1;
	int i, j, c, stat, pid, buf;
	int burstTime, prio;
	int bt[10];
	int Prio[10];
	struct proceso self;	
  

	//Asigna de forma aleatoria los BurstTime
	for (j = 0; j<n ; j++){
		bt[j] = rand()%20+1;
		Prio[j] = rand()%5+1;
	}

	//Crea los procesos
	pid = process_fork(n);

	//Se les asigna un BurstTime
	burstTime = bt[pid];
	prio = Prio[pid];

	self.id = getpid();
	self.prioridad = 0;
	self.bt = burstTime;
	self.yaPaso = 0;

	printf("Soy Proceso %d PID: %d BT: %d\n",pid, getpid(), burstTime);
	//Se crea una prioridad al azar
	//prio = rand()%10;

	//Se guardan los PID`s
	if (pid != 0){
		//Crea el archivo
		sprintf(command, "echo %d %d %d %d >> procesos.txt", 0, self.id, self.bt, self.yaPaso);
		//printf("%s\n", command);
		system(command);
		sleep(2);

		if(pid != 1){
			kill(getpid(), SIGSTOP);
		}
	}
	


	//Aqui es donde esta la magia
	//Padre
	if (pid == 0){
		
		//Comunicacion entre procesos 

		for(i = 0; i<n; i++){
			wait(&stat);
		}

		printf("Procesos terminados \n");
		exit(0);
	}

	//Hijos
	else{
		//printf("Soy Proceso %d PID: %d\n",pid, getpid() );
		getPrioridad(n);
		self = updateSelf(n,self);
		while(self.bt >= 0){
				isFisrtTime = getFT();
				//Comunicacion entre procesos
				getPrioridad(n);
				self = updateBT(n, self);
				self.yaPaso = 1;
				self = updateYaPaso(n, self);
				setProcesos(n);
				printf("\nProceso %d burstTime: %d prioridad: %d\n", pid, self.bt, self.prioridad);
			if (self.bt > qt){
				self.bt -= qt;
				//Simula el proceso
				sleep(qt);

				if ((turn = getAProcess(n, self)) != 0){
						getPrioridad(n);
						//printf("Proceso %d se despierta\n", turn);
						kill(turn, SIGCONT);
						//printf("Proceso %d se va a dormir\n", getpid());
						if(turn != self.id)
							kill(getpid(), SIGSTOP);
				}
			}
			else{
				sleep(self.bt);
				self.bt = 1;
				//printf("Proceso %d burstTime: %d\n", pid, burstTime);
				printf("Proceso %d terminado\n", pid);
				
				if ((turn = getAProcess(n, self)) != 0){
						//printf("Proceso %d se despierta\n", turn);
						deleteProcess(n, getpid());
						getPrioridad(n);
						//setProcesos(n, p);
						kill(turn, SIGCONT);
						
				}
				else{
					deleteProcess(n, getpid());
					setProcesos(n);
				}

				exit(0);
			}

		}
		
	}

} 

int main() 
{ 
	int qt;
	time_t t;

	//Rand
	srand((unsigned) time(&t));

	//Aqui se van a guardar los id de procesos que se encuentran en procesos.txt
	p = procA;

	//Limpia el archivo
	system("clear");
	system("rm procesos.txt");
	//system("rm prioridad.txt");
	system("echo 1 > ft.txt");
	//Lee el quantum
	qt = readQT();

	//Magia
	RR(qt, 6); 

	return 0; 
} 