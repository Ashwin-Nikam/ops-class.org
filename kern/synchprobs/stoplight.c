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
 * Driver code is in kern/tests/synchprobs.c We will replace that file. This
 * file is yours to modify as you see fit.
 *
 * You should implement your solution to the stoplight problem below. The
 * quadrant and direction mappings for reference: (although the problem is, of
 * course, stable under rotation)
 *
 *   |0 |
 * -     --
 *    01  1
 * 3  32
 * --    --
 *   | 2|
 *
 * As way to think about it, assuming cars drive on the right: a car entering
 * the intersection from direction X will enter intersection quadrant X first.
 * The semantics of the problem are that once a car enters any quadrant it has
 * to be somewhere in the intersection until it call leaveIntersection(),
 * which it should call while in the final quadrant.
 *
 * As an example, let's say a car approaches the intersection and needs to
 * pass through quadrants 0, 3 and 2. Once you call inQuadrant(0), the car is
 * considered in quadrant 0 until you call inQuadrant(3). After you call
 * inQuadrant(2), the car is considered in quadrant 2 until you call
 * leaveIntersection().
 *
 * You will probably want to write some helper functions to assist with the
 * mappings. Modular arithmetic can help, e.g. a car passing straight through
 * the intersection entering from direction X will leave to direction (X + 2)
 * % 4 and pass through quadrants X and (X + 3) % 4.  Boo-yah.
 *
 * Your solutions below should call the inQuadrant() and leaveIntersection()
 * functions in synchprobs.c to record their progress.
 */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

static struct lock *lockArray[4]; 

/*
 * Called by the driver during initialization.
 */

void
stoplight_init() {
	lockArray[0] = lock_create("0_lock");
	lockArray[1] = lock_create("1_lock");
	lockArray[2] = lock_create("2_lock");
	lockArray[3] = lock_create("3_lock");
	return;
}

/*
 * Called by the driver during teardown.
 */

void stoplight_cleanup() {
	lock_destroy(lockArray[0]);
	lock_destroy(lockArray[1]);
	lock_destroy(lockArray[2]);
	lock_destroy(lockArray[3]);
	return;
}

void
turnright(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	/*
	 * Implement this function.
	 */

	kprintf_n("[turnright] direction: %d index: %d\n", direction, index);
	lock_acquire(lockArray[direction]);
	inQuadrant(direction, index);
	leaveIntersection(index);
	lock_release(lockArray[direction]);
	return;
}
void
gostraight(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	/*
	 * Implement this function.
	 */

	uint32_t destination = (direction + 3)%4;	
	kprintf_n("[gostraight] direction: %d index: %d destination:%d\n", direction, index, destination);

	if(destination < direction){

		lock_acquire(lockArray[destination]);
		lock_acquire(lockArray[direction]);
		inQuadrant(direction, index);
		inQuadrant(destination, index);
		leaveIntersection(index);
		lock_release(lockArray[direction]);
		lock_release(lockArray[destination]);

	}else{

		lock_acquire(lockArray[direction]);
		lock_acquire(lockArray[destination]);
		inQuadrant(direction, index);
		inQuadrant(destination, index);
		leaveIntersection(index);
		lock_release(lockArray[destination]);
		lock_release(lockArray[direction]);	
	}
	

	return;
}
void
turnleft(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	/*
	 * Implement this function.
	 */

	uint32_t mid = (direction + 3)%4;
	uint32_t destination = (mid + 3)%4;
	kprintf_n("[turnleft] direction: %d index: %d mid:%d destination:%d\n", direction, index, mid, destination);

	uint32_t array[3];
	array[0] = direction;
	array[1] = mid;
	array[2] = destination;

	kprintf_n("BS\n");
	for(int i=0;i<3;i++){
		kprintf_n("%d", array[i]);
	}

	for(int i=0;i<3-1;i++){
		for(int j=i+1;j<3;j++){
			if(array[i]>array[j]){
				uint32_t temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}

	kprintf_n("\nAS\n");
	for(int i=0;i<3;i++){
		kprintf_n("%d", array[i]);
	}
	
 	lock_acquire(lockArray[array[0]]);
	lock_acquire(lockArray[array[1]]);
	lock_acquire(lockArray[array[2]]);
	inQuadrant(direction, index);
	inQuadrant(mid, index);
	inQuadrant(destination, index);
	leaveIntersection(index);
	lock_release(lockArray[array[2]]);
	lock_release(lockArray[array[1]]);
	lock_release(lockArray[array[0]]);

	return;
}