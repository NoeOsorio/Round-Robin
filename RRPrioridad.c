#include<stdio.h> 
#include<string.h>
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include<signal.h>

struct proceso{
	int id;
	int yaPaso;
	int prioridad;
	int bt;
}proc[10], *p;



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

void sort(int n, struct proceso *p){
	struct proceso array[10], *tmp = p, t;
	for (int i = 0; i < n-1; ++i)
	{
		array[i] = *p;
		p++;	
	}
	p = tmp;


	//Sort
	int d;

	
	for (int c = 1 ; c < n - 1; c++) {
    d = c;
 
    while ( d > 0 && array[d-1].bt > array[d].bt) {
      t          = array[d];
      array[d]   = array[d-1];
      array[d-1] = t;
 
      d--;
    }

    //Pointer

    for (int i = 0; i < n-1; ++i)
	{
		*p = array[i];
		p++;	
	}
	p = tmp;
  }
}

void setProcesos(int n, struct proceso *procesos){
	struct proceso *tmp = procesos;
	char command[50];
	system("rm procesos.txt");
	for (int i = 0; i < n-1; ++i)
	{
		sprintf(command, "echo %d %d %d %d >> procesos.txt", (procesos->prioridad) , (procesos->id), (procesos->bt), (procesos->yaPaso));
		system(command);
		procesos++;
	}
	procesos = tmp;
}

void getProcesos(int n, struct proceso *p){
	struct proceso *tmp = p;
	int cont = 0;
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
				(p->prioridad) = buf;
				//printf("%d\n", p->id);
			fscanf(fp,"%d",&buf);
				(p->id) = buf;
				//printf("%d\n", p->prioridad);
			fscanf(fp,"%d",&buf);
				(p->bt) = buf;
			fscanf(fp,"%d",&buf);
				(p->yaPaso) = buf;	
				//printf("%d\n", p->id);	
				p++;
			cont++;	
			if (cont >= n)
				break;			
			}
			
		fclose(fp);
	}
	p = tmp;
	return;
}

int getAProcess(int n, struct proceso *procesos, struct proceso exception){
	struct proceso *tmp = procesos;
	struct proceso something, anterior, sig, same;
	int band = 0, isEmpty = 0;

	something.id = 0;
	something.prioridad = 0;
	something.bt = 0;


	for (int i = 0; i < n; i++){
		if ((procesos->id) == exception.id){
			same = exception;
		}
		if(((procesos->id) != 0) && ((procesos->id) != exception.id) && ((procesos->id) > 100) && (procesos->yaPaso == 0)){
			isEmpty = 0;
			something = *procesos;
			//printf("Something: %d con prioridad %d \n", something.id, something.prioridad);
			if(something.prioridad < exception.prioridad ){
				anterior = something;
				break;
			}
			else if (something.prioridad > exception.prioridad && band == 0){
				isEmpty = 0;
				sig = something;
				procesos = tmp;
				band = 1;
			}
			
		}
		procesos++;
	}
	if (something.prioridad >= n){
		procesos = tmp;
		for (int i = 0; i < n; ++i)
		 {
		 	if(procesos->bt > 0){
		 		something = *procesos;
		 		break;
		 	}
		 	procesos++;
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


	procesos = tmp;
	//printf("Next: %d \n", something.id);
	return something.id;
	
}
struct proceso updateSelf(int n, struct proceso *p, struct proceso self){
	struct proceso *tmp = p;

	for (int i = 0; i < n-1; ++i)
	{
		if(p->id == self.id){
			self = *p;
			break;
		}
		p++;
	}
	p = tmp;
	return self;
}
struct proceso updateBT(int n, struct proceso *p, struct proceso self){
	struct proceso *tmp = p;

	for (int i = 0; i < n-1; ++i)
	{
		if(p->id == self.id){
			p->bt = self.bt;
			self = *p;
			break;
		}
		p++;
	}
	p = tmp;
	return self;
}
struct proceso updateYaPaso(int n, struct proceso *p, struct proceso self){
	struct proceso *tmp = p;

	for (int i = 0; i < n-1; ++i)
	{
		if(p->id == self.id){
			p->yaPaso = self.yaPaso;
			self = *p;
			break;
		}
		p++;
	}
	p = tmp;
	return self;
}

void setPrioridad(int n, struct proceso *p){
	/*char command[50];
	struct proceso *tmp = procesos;
	system("rm prioridad.txt");
	for (int i = 0; i < n-1; ++i)
	{
		sprintf(command, "echo %d %d >> prioridad.txt", (procesos->bt) , (procesos->id));
		system(command);
		procesos++;
	}
	system("sort prioridad.txt -o prioridad.txt");
	procesos = tmp;*/

	sort(n,p);
	struct proceso array[10], *tmp = p;
	int prio = 1, band = 0;
	for (int i = 0; i < n-1; ++i)
	{
		if (p->yaPaso == 1){
			prio++;
		}
		if (p->bt != 0 && p->yaPaso == 0){
			band = 1;
			p->prioridad = prio;
			prio++;
		}
		p++;	
	}
	//printf("Prioridad maxima %d, %d\n", prio, band);
	p = tmp;

	if (band == 0){
			for (int i = 0; i < n-1; ++i)
			{
				p->yaPaso = 0;
				p++;	
			}
		p = tmp;
	}
}
void getPrioridad(int n, struct proceso *p){
	struct proceso *tmp = p;

	getProcesos(n,p);
	setPrioridad(n,p);
	setProcesos(n,p);
	
	p = tmp;
	//setProcesos(n, p);
	return;
}
void deleteProcess(int n, struct proceso *procesos, int delete){
	struct proceso *tmp = procesos;
	char command[50];
	system("rm procesos.txt");
	for (int i = 0; i < n-1; ++i)
	{
		if ((procesos->id) == delete){
			(procesos->prioridad) = 0;
			(procesos->id) = 0;
			(procesos->bt) = 0;

			//printf("Se elimino proceso %d\n\n", *procesos);
		}
		sprintf(command, "echo %d %d %d %d >> procesos.txt", (procesos->prioridad) , (procesos->id), (procesos->bt), (procesos->yaPaso));
		system(command);

		procesos++;
	}
	procesos = tmp;
}
void RR(int qt, int n, struct proceso *proc) 
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
		bt[j] = rand()%10+1;
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
		getPrioridad(n,proc);
		self = updateSelf(n, proc, self);
		while(self.bt >= 0){
				//Comunicacion entre procesos
				getPrioridad(n,proc);
				self = updateBT(n, proc, self);
				self.yaPaso = 1;
				self = updateYaPaso(n, proc, self);
				setProcesos(n, p);
				printf("\nProceso %d burstTime: %d prioridad: %d\n", pid, self.bt, self.prioridad);
			if (self.bt > qt){
				self.bt -= qt;
				//Simula el proceso
				sleep(qt);

				if ((turn = getAProcess(n,proc,self)) != 0){
						getPrioridad(n,proc);
						//printf("Proceso %d se despierta\n", turn);
						kill(turn, SIGCONT);
						//printf("Proceso %d se va a dormir\n", getpid());

						kill(getpid(), SIGSTOP);
				}
			}
			else{
				sleep(self.bt);
				self.bt = 1;
				//printf("Proceso %d burstTime: %d\n", pid, burstTime);
				printf("Proceso %d terminado\n", pid);
				
				if ((turn = getAProcess(n,proc,self)) != 0){
						//printf("Proceso %d se despierta\n", turn);
						deleteProcess(n, proc, getpid());
						getPrioridad(n,proc);
						//setProcesos(n, p);
						kill(turn, SIGCONT);
						
				}
				else{
					deleteProcess(n, proc, getpid());
					setProcesos(n, p);
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
	p = proc;

	//Limpia el archivo
	system("clear");
	system("rm procesos.txt");
	//system("rm prioridad.txt");

	//Lee el quantum
	qt = readQT();

	//Magia
	RR(qt, 6, p); 

	return 0; 
} 