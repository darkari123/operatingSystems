
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>


int student[3];
int totalMoney;	// Intitalize Total = 4000


void *producer(int id);
void *consumer(void* id);

pthread_mutex_t mutex;



int
main()
{
	totalMoney = 4000;
	student[0] = student[1] = student[2] = 0;
	
	pthread_t tid[3]; // 3 threads for 3 students 


	pthread_setconcurrency(3);

	pthread_create(&tid[0], NULL, (void *(*)(void *))consumer, (void*)0);
	pthread_create(&tid[1], NULL, (void *(*)(void *))consumer, (void*)1);
	pthread_create(&tid[2], NULL, (void *(*)(void *))consumer, (void*)2);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);

	printf("Student A received $%d in total\n", student[0]);
	printf("Student B received $%d in total \n", student[1]);
	printf("Student C received $%d in total\n", student[2]);
	printf("This adds up to $%d", student[0] + student[1] + student[2]);

	return 0;

}

void *producer(int id)
{
	int rMoney = totalMoney;
	int taken = ceil(rMoney * 0.25);
	rMoney -= taken;
	student[id] += taken;
	printf("Student %c received $%d\n", 'A' + id, taken);
	totalMoney = rMoney;
}

void *consumer(void* id)
{
	while (totalMoney > 0)
	{
		sleep(1);
		producer((int)id);
	}
	pthread_exit(0);


}




