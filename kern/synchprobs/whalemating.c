/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code is in kern/tests/synchprobs.c We will
 * replace that file. This file is yours to modify as you see fit.
 *
 * You should implement your solution to the whalemating problem below.
 */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>


static struct semaphore *sem1;
static struct semaphore *sem2;

/*
 * Called by the driver during initialization.
 */

void whalemating_init() {
	sem1 = sem_create("male_sem", 0);
	if(sem1 == NULL){
		panic("whalemating: male_sem creation failed...\n");
	}
	sem2 = sem_create("female_sem", 0);
	if(sem2 == NULL){
		panic("whalemating: female_sem creation failed...\n");
	}
	
	return;
}

/*
 * Called by the driver during teardown.
 */

void
whalemating_cleanup() {

	sem_destroy(sem1);
	sem_destroy(sem2);

	return;
}

void
male(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling male_start and male_end when
	 * appropriate.
	 */

	
	male_start(index);
	P(sem1);
	male_end(index);
	return;
}

void
female(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling female_start and female_end when
	 * appropriate.
	 */

	female_start(index);
	P(sem2);
	female_end(index);
	return;
}

void
matchmaker(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling matchmaker_start and matchmaker_end
	 * when appropriate.
	 */
	
	matchmaker_start(index);
	V(sem1);
	V(sem2);
	matchmaker_end(index);	
	return;
}
