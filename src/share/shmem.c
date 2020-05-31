/**
 * @file
 * @author eikendev, https://eiken.dev/
 * @date 2018-01-04
 * @brief This module contains function definitions for shared memory setup and cleanup.
 * @details Shared memory is used to transfer data between the clients and the server.
 */

#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shmem.h"

/**
 * @details Shared memory to setup or clean up.
 */
extern void *shmem;

int create_shared_memory(char *name, size_t len, bool master)
{
	int fd = shm_open(name, O_RDWR | O_CREAT, 0640);
	if (fd == -1)
		return -1;

	if (master) {
		int errind = ftruncate(fd, len);
		if (errind == -1)
			return -2;
	}

	shmem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shmem == MAP_FAILED) {
		return -3;
	}

	return fd;
}

int close_shared_memory(char *name, size_t len, int fd, bool master)
{
	int errind;

	if (fd >= 0) {
		errind = close(fd);
		if (errind == -1)
			return -1;
	}

	if (shmem != NULL) {
		errind = munmap(shmem, len);
		if (errind == -1)
			return -2;
	}

	if (master) {
		errind = shm_unlink(name);
		if (errind == -1)
			return -3;
	}

	return 0;
}
