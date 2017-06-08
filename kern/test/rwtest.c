/*
 * All the contents of this file are overwritten during automated
 * testing. Please consider this before changing anything in this file.
 */

#include <types.h>
#include <lib.h>
#include <clock.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <kern/test161.h>
#include <spinlock.h>

#define NTHREADS	32

/*
 * Use these stubs to test your reader-writer locks.
 */

struct rwlock *rwlock = NULL;

/* rwtest
In this test we first acquire two read locks.
Then release them both and then acquire a write lock and then release it.
All this is done on a single thread.
*/

int rwtest(int nargs, char **args) { 
	(void)nargs;
	(void)args;

	kprintf_n("Starting rwt1...\n");
	rwlock = rwlock_create("rwlock");

	if(rwlock == NULL){
		panic("rwt1: rwlock_create failed\n");
	}

	rwlock_acquire_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	rwlock_acquire_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	rwlock_release_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	rwlock_release_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	rwlock_acquire_write(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	rwlock_release_write(rwlock);
	kprintf_n("[Final] Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);

	success(TEST161_SUCCESS, SECRET, "rwt1");

	rwlock_destroy(rwlock);
	rwlock = NULL;	

	return 0;
}

/*
Now I am going to create two threads readlocks and writelocks
*/

static
void
readlocks(void *junk1, unsigned long junk2) {
	(void)junk1;
	(void)junk2;

	kprintf_n("Read 1\n");
	random_yielder(4);
	rwlock_acquire_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	kprintf_n("Read 2\n");
	random_yielder(4);
	rwlock_acquire_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	kprintf_n("Read 3\n");
	random_yielder(4);
	rwlock_release_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	kprintf_n("Read 4\n");
	random_yielder(4);
	rwlock_release_read(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);

	success(TEST161_SUCCESS, SECRET, "rwt2");
}

static
void
writelocks(void *junk1, unsigned long junk2) {
	(void)junk1;
	(void)junk2;

	kprintf_n("Write 1\n");
	random_yielder(4);
	rwlock_acquire_write(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	kprintf_n("Write 2\n");
	random_yielder(4);
	rwlock_release_write(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);
	
	kprintf_n("Write 3\n");
	random_yielder(4);
	rwlock_acquire_write(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);

	kprintf_n("Write 4\n");
	random_yielder(4);
	rwlock_release_write(rwlock);
	kprintf_n("Count: %d Mode: %c\n", rwlock->rwlock_count, rwlock->rwlock_mode);

	success(TEST161_SUCCESS, SECRET, "rwt2");
}


int rwtest2(int nargs, char **args) {  
	(void)nargs;
	(void)args;

	int result;

	kprintf_n("Starting rwt2...\n");
	rwlock = rwlock_create("rwlock");

	if(rwlock == NULL){
		panic("rwt2: rwlock_create failed\n");
	}

	result = thread_fork("rwt2", NULL, writelocks, NULL, 0);
	if (result) {
		panic("rwt2: thread_fork failed\n");
	}
	result = thread_fork("rwt2", NULL, readlocks, NULL, 0);

	if (result) {
		panic("rwt2: thread_fork failed\n");
	}

	success(TEST161_SUCCESS, SECRET, "rwt2");

	return 0;
}


int rwtest3(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("Starting rwt3...\n");
	rwlock = rwlock_create("rwlock");

	if(rwlock == NULL){
		panic("rwt3: rwlock_create failed\n");
	}

	secprintf(SECRET, "Should panic...", "rwt3");
	rwlock_release_write(rwlock);

	//Should panic on success and not get to this point

	success(TEST161_SUCCESS, SECRET, "rwt3");
	return 0;
}


int rwtest4(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("Starting rwt4...\n");
	rwlock = rwlock_create("rwlock");

	if(rwlock == NULL){
		panic("rwt4: rwlock_create failed\n");
	}

	secprintf(SECRET, "Should panic...", "rwt4");
	rwlock_acquire_read(rwlock);
	rwlock_release_write(rwlock);

	//Should panic on success and not get to this point

	success(TEST161_SUCCESS, SECRET, "rwt4");
	return 0;
}

int rwtest5(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf_n("Starting rwt5...\n");
	rwlock = rwlock_create("rwlock");

	if(rwlock == NULL){
		panic("rwt5: rwlock_create failed\n");
	}

	secprintf(SECRET, "Should panic...", "rwt5");
	rwlock_acquire_write(rwlock);
	rwlock_release_read(rwlock);

	//Should panic on success and not get to this point

	success(TEST161_SUCCESS, SECRET, "rwt5");

	return 0;
}