
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "buffer_sem.h"

#ifdef __APPLE__
#include <pthread.h>

typedef struct sem_421 {
	pthread_mutex_t lock;
	pthread_cond_t cond;
	unsigned int value;
} sem_421_t;

static int sem421_init(sem_421_t *sem, unsigned int value)
{
	if(pthread_mutex_init(&sem->lock, NULL) != 0){
		return -1;
	}
	if(pthread_cond_init(&sem->cond, NULL) != 0){
		pthread_mutex_destroy(&sem->lock);
		return -1;
	}
	sem->value = value;
	return 0;
}

static int sem421_wait(sem_421_t *sem)
{
	pthread_mutex_lock(&sem->lock);
	while(sem->value == 0){
		pthread_cond_wait(&sem->cond, &sem->lock);
	}
	sem->value--;
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

static int sem421_post(sem_421_t *sem)
{
	pthread_mutex_lock(&sem->lock);
	sem->value++;
	pthread_cond_signal(&sem->cond);
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

static int sem421_getvalue(sem_421_t *sem, int *value)
{
	pthread_mutex_lock(&sem->lock);
	*value = (int)sem->value;
	pthread_mutex_unlock(&sem->lock);
	return 0;
}

static int sem421_destroy(sem_421_t *sem)
{
	pthread_cond_destroy(&sem->cond);
	pthread_mutex_destroy(&sem->lock);
	return 0;
}
#else
typedef sem_t sem_421_t;

static int sem421_init(sem_421_t *sem, unsigned int value)
{
	return sem_init(sem, 0, value);
}

static int sem421_wait(sem_421_t *sem)
{
	return sem_wait(sem);
}

static int sem421_post(sem_421_t *sem)
{
	return sem_post(sem);
}

static int sem421_getvalue(sem_421_t *sem, int *value)
{
	return sem_getvalue(sem, value);
}

static int sem421_destroy(sem_421_t *sem)
{
	return sem_destroy(sem);
}
#endif

static bb_buffer_421_t buffer;
static sem_421_t mutex;
static sem_421_t fill_count;
static sem_421_t empty_count;
static int waiting = 1;

long init_buffer_421(void) {
	// Write your code to initialize buffer

	if(buffer.read || buffer.write){
		printf("Buffer has already been initialized\n");
		return -1;
	}

	// Initialize your semaphores here.
	if(sem421_init(&mutex, 1) != 0 ||
	   sem421_init(&fill_count, 0) != 0 ||
	   sem421_init(&empty_count, SIZE_OF_BUFFER) != 0){
		printf("Semaphore initialization failed\n");
		return -1;
	}

	buffer.length = 0;

	struct bb_node_421* curr = (struct bb_node_421*)malloc(sizeof(struct bb_node_421));
	memset(curr->data, 0, DATA_LENGTH);

	struct bb_node_421* head = curr;

	for(int i = 0; i < SIZE_OF_BUFFER - 1; i++){

		curr->next = (struct bb_node_421*)malloc(sizeof(struct bb_node_421));

		memset(curr->next->data, 0, DATA_LENGTH);

		curr = curr->next;
	}

	curr->next = head;

	buffer.read = head;
	buffer.write = head;

	if(buffer.read != head && buffer.write != head){
		printf("Error upon initialization\n");
		return -1;
	}

	return 0;
}


long enqueue_buffer_421(const void *data) {
	// Write your code to enqueue data into the buffer
//	buffer.length = 0;
	if(!data){
		printf("Cannot enqueue null data\n");
		return -1;
	}

	if(!buffer.read || !buffer.write){
		printf("Buffer has not been initialized\n");
		return -1;
	}

	waiting = 0;
	sem421_wait(&empty_count);
	sem421_wait(&mutex);


	waiting = 1;

	buffer.length++;
	memcpy(buffer.write->data, data, DATA_LENGTH);
	buffer.write = buffer.write->next;

//	print_semaphores();

	const unsigned char *bytes = data;
	printf("ENQUEUING: %d bytes (first=0x%02x)\n", DATA_LENGTH, bytes[0]);

	sem421_post(&fill_count);

	print_semaphores();
	sem421_post(&mutex);
	return 0;
}

long dequeue_buffer_421(void *data) {

	// Write your code to dequeue data from the buffer

	if(!data){
		printf("Cannot dequeue into null data\n");
		return -1;
	}

	if(!buffer.read || !buffer.write){
		printf("Buffer has not been initialized\n");
		return -1;
	}

	waiting = 0;
	sem421_wait(&fill_count);
	sem421_wait(&mutex);
//	sem_wait(&fill_count);

	waiting = 1;

	memcpy(data, buffer.read->data, DATA_LENGTH);
	buffer.read = buffer.read->next;
	buffer.length--;

	const unsigned char *bytes = data;
	printf("DEQUEUING: %d bytes (first=0x%02x)\n", DATA_LENGTH, bytes[0]);

//	print_semaphores();

	sem421_post(&empty_count);
	print_semaphores();
	sem421_post(&mutex);
	return 0;
}


long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	// write your code to delete buffer and other unwanted components


	if(waiting == 0){
		printf("Cannot delete buffer because there is a thread waiting to be executed");
		return -1;
	}

	if(!buffer.read || !buffer.write){
		printf("Buffer has not been initialized\n");
		return -1;
	}


	sem421_wait(&mutex);

	struct bb_node_421* curr = buffer.read;

	for(int i = 0; i < SIZE_OF_BUFFER; i++){
		struct bb_node_421* head = curr;

		curr = curr->next;

		free(head);

	}

	buffer.length = 0;
	buffer.read = NULL;
	buffer.write = NULL;

	sem421_destroy(&mutex);
	sem421_destroy(&fill_count);
	sem421_destroy(&empty_count);

	return 0;
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int value;
	sem421_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem421_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem421_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	return;
}
