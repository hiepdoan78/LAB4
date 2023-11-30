/*
 * scheduler.c
 *
 *  Created on: Nov 22, 2023
 *      Author: Hiepm
 */

#include "main.h"
#include "task.h"
#include "scheduler.h"

void SCH_Init() {
	SCH_TASK_SCHEDULER.numOfTask = 0;
	for(int i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_TASK_SCHEDULER.TASK_LIST[i].Delay = 0;
		SCH_TASK_SCHEDULER.TASK_LIST[i].Period = 0;
		SCH_TASK_SCHEDULER.TASK_LIST[i].RunMe = 0;
		SCH_TASK_SCHEDULER.TASK_LIST[i].pTask = 0;
	}
}

void insertNewTask(sTask task) {
	// check if list is empty
	if (SCH_TASK_SCHEDULER.numOfTask == 0) {
		SCH_TASK_SCHEDULER.TASK_LIST[0] = task; // insert new task to the first position
		SCH_TASK_SCHEDULER.numOfTask += 1;
		return;
	} else {
		int pos = 0;
		int pre_sum = SCH_TASK_SCHEDULER.TASK_LIST[0].Delay;
		int sum = SCH_TASK_SCHEDULER.TASK_LIST[0].Delay;
		// find the suitable position
		while (pos < SCH_TASK_SCHEDULER.numOfTask && task.Delay > sum) {
			pos++;
			if (pos < SCH_TASK_SCHEDULER.numOfTask) {
				pre_sum = sum;
				sum += SCH_TASK_SCHEDULER.TASK_LIST[pos].Delay;
			}
		}
		//check if add to front or middle
		if (pos == SCH_TASK_SCHEDULER.numOfTask) { //add to last position
			task.Delay -= sum;
			SCH_TASK_SCHEDULER.TASK_LIST[pos] = task;
			SCH_TASK_SCHEDULER.numOfTask += 1;
			return;
		}
		else {
			for (int i = SCH_TASK_SCHEDULER.numOfTask; i > pos; i--) {
				SCH_TASK_SCHEDULER.TASK_LIST[i] = SCH_TASK_SCHEDULER.TASK_LIST[i - 1];
			}
			// add to front
			if (pos == 0) {
				SCH_TASK_SCHEDULER.TASK_LIST[pos] = task;
				sum = 0;
				pre_sum = task.Delay;
			}
			// add to middle
			else {
				task.Delay -= pre_sum;
				sum = pre_sum;
				pre_sum += task.Delay;
				SCH_TASK_SCHEDULER.TASK_LIST[pos] = task;
			}
			// update delay of index[POS+1]
			sum += SCH_TASK_SCHEDULER.TASK_LIST[pos + 1].Delay;
			SCH_TASK_SCHEDULER.TASK_LIST[pos + 1].Delay = sum - pre_sum;
			SCH_TASK_SCHEDULER.numOfTask += 1;
		}
	}
}
void SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
	if (SCH_TASK_SCHEDULER.numOfTask <= SCH_MAX_TASKS) {
	// create new task and insert to queue
	sTask newTask;
	newTask.pTask = pFunction;
	newTask.Delay = delay / TICK;
	newTask.Period = period / TICK;
	newTask.RunMe = 0;
	insertNewTask(newTask);
	}
}

void SCH_Update() {
	// check if the list is empty
	if (!SCH_TASK_SCHEDULER.TASK_LIST[0].pTask) {
		return;
	}
	else {
		if (SCH_TASK_SCHEDULER.TASK_LIST[0].Delay == 0) {
			SCH_TASK_SCHEDULER.TASK_LIST[0].RunMe++;
			if (SCH_TASK_SCHEDULER.TASK_LIST[0].Period) {
				SCH_TASK_SCHEDULER.TASK_LIST[0].Delay = SCH_TASK_SCHEDULER.TASK_LIST[0].Period;
			}
		}
		else SCH_TASK_SCHEDULER.TASK_LIST[0].Delay--;
	}
}

void SCH_Delete_Task() {
	int index = 0;
	int add_back_flag = 0;
	sTask temp;
	// check if this is one shot task
	if (SCH_TASK_SCHEDULER.TASK_LIST[index].Period) {
		add_back_flag = 1;
		temp = SCH_TASK_SCHEDULER.TASK_LIST[index];
	}
	// shift left all tasks
	for (; index < SCH_TASK_SCHEDULER.numOfTask - 1; index++) {
		SCH_TASK_SCHEDULER.TASK_LIST[index] = SCH_TASK_SCHEDULER.TASK_LIST[index + 1];
	}
	// delete task rear after shift left
	SCH_TASK_SCHEDULER.TASK_LIST[index].pTask = 0x0000;
	SCH_TASK_SCHEDULER.TASK_LIST[index].Delay = 0;
	SCH_TASK_SCHEDULER.TASK_LIST[index].Period = 0;
	SCH_TASK_SCHEDULER.TASK_LIST[index].RunMe = 0;
	SCH_TASK_SCHEDULER.numOfTask -= 1;
	// check flag to add back into queue
	if (add_back_flag == 1) {
		insertNewTask(temp);
	}
}

void SCH_Dispatch_Tasks() {
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_TASK_SCHEDULER.TASK_LIST[index].RunMe > 0) {
			// Run the task
			(*SCH_TASK_SCHEDULER.TASK_LIST[index].pTask)();
			// Reset / reduce RunMe flag
			SCH_TASK_SCHEDULER.TASK_LIST[index].RunMe -= 1;
			// schedule to delete task
			SCH_Delete_Task();
		}
	}
}
