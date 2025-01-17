#include "semaphore.h"

int create_semaphore(int key)
{
	int id;

	id = semget(key, 1, 0600 | IPC_CREAT);
	exit_on_error(id, "CREATE SEMAPHORE");

	return id;
}

int create_mutex(int key)
{
	int id;

	id = semget(key, 1, 0600 | IPC_CREAT | IPC_EXCL);
	if (id >= 0) {
		// there is no semaphore/mutex: guarantees that this has value 1
		up(id);
		return id;
	}
	else {
		// semaphore/mutex already exists: returns the id
		id = semget(key, 1, 0600);
		if (id >= 0) {
			return id;
		}
		perror("CREATE MUTEX");
		exit(1);
	}
}

void down(int id)
{
	struct sembuf s = {0, -1, 0};
	int status;

	status = semop(id, &s, 1);
	exit_on_error(status, "DOWN");
}

void up(int id)
{
	struct sembuf s = {0, 1, 0};
	int status;

	status = semop(id, &s, 1);
	exit_on_error(status, "UP");
}