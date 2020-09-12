#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>


int main(){
	int i,shmid;
	int pid;
	shmid = shmget(IPC_PRIVATE,5*sizeof(int),IPC_CREAT | 0666);
	if(shmid<0){
		printf("\n error in shmget");
		exit(0);
	}
	pid = fork();
	if(pid==0){
		//this is father
		int *myarray;
		myarray= shmat(shmid,NULL,0);
		if(myarray<0){
			printf("\n error in shmat in father");
			exit(0);
		}
		printf("\nwriting 0-4 in shared memory\n");
		for(i=0;i<5;i++){
			myarray[i]=i;
		}
		if(shmdt(myarray) == -1){
			printf("\n error in shmdt in father");
			exit(0);
		}
	}
	else{
		int *myarray;
		myarray= shmat(shmid,NULL,0);
		if(myarray<0){
			printf("\n error in shmat in child");
			exit(0);
		}
		printf("reading sharedmemory in child\n");
		while(1){
			if(myarray[4]== 4){
				printf("shared memory updated to new values\n");
				for(i=0;i<5;i++){
					printf("%d ",myarray[i]);
				}
				printf("\n");
				break;
			}
			else{
				for(i=0;i<5;i++){
					printf("%d ",myarray[i]);
				}
				printf("\n");
			}
		}
		if(shmdt(myarray) == -1){
			printf("\n error in shmdt in child");
			exit(0);
		}
		if(shmctl(shmid,IPC_RMID,NULL) == -1){  //will remove shmid and segment after child reading
			printf("\n error in shmctl in child");
			exit(0);
		}
	}
	
}
