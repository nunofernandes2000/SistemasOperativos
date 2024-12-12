#include <stdio.h>
#include <windows.h>

#define N_PRODUCERS		5
#define N_CONSUMERS		5

int capacity = 5;
int items = 0;

HANDLE mutex;

DWORD WINAPI producer(LPVOID T)
{
	int *period;
	period = (int *) T;
	
	while (1) {
		WaitForSingleObject(mutex, INFINITE);
		if (items < capacity) {
			items++;
			printf("\n Stored box: They became %d boxes", items);
		}
		else
			printf("\n FULL"); 
		ReleaseMutex(mutex);
		Sleep(*period);
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID T)
{
	int *period;
	period = (int *) T;
	
	while (1) {
		WaitForSingleObject(mutex, INFINITE);
		if (items > 0) {
			Sleep(100);
			items--;
			printf("\n Box removed: %d boxes left", items);
		}
		else 
			printf("\n EMPTY");
		ReleaseMutex(mutex);
		
		Sleep(*period);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int producer_period = 1500;
	int consumer_period = 1000;
		
	DWORD threadID_producer[N_PRODUCERS], threadID_consumer[N_CONSUMERS];
	HANDLE threadH_producer[N_PRODUCERS], threadH_consumer[N_CONSUMERS];
	
	int i;
	
	// create mutex
	mutex = CreateMutex(NULL, FALSE, NULL);
		
	// start producers
	for (i = 0; i < N_PRODUCERS; i++) 
		threadH_producer[i] = CreateThread(NULL, 0, producer, &producer_period, 0, &threadID_producer[i]);
	
	// start consumer
	for (i = 0; i < N_CONSUMERS; i++) 
		threadH_consumer[i] = CreateThread(NULL, 0, consumer, &consumer_period, 0, &threadID_consumer[i]);
	
	// wait for the end of threads
	for (i = 0; i < N_PRODUCERS; i++) {
		WaitForSingleObject(threadH_producer[i], INFINITE);
		CloseHandle(threadH_producer[i]);
	}
		
	for (i = 0; i < N_CONSUMERS; i++) {
		WaitForSingleObject(threadH_consumer[i], INFINITE);
		CloseHandle(threadH_consumer[i]);
	}
	
	return 0;
}
