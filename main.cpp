/*Origin From Windows*/
#include <iostream>

/*Origin From My Code*/
#include "ThreadPool.h"

ThreadPool* tp;

void initialize() {
	tp = NULL;
	tp = new ThreadPool(3);
}

void finalize() {
	if (tp != NULL)
		delete tp; //it will call destructor
}

ThreadPool* getTP() {
	return tp;
}

void work(int t, int id) {
	printf("%d start \n", id);

	std::this_thread::sleep_for(std::chrono::seconds(t));

	printf("%d end after %ds\n", id, t);
}

int main(int argc, char** argv) {
	initialize();

	for (int i = 0; i < 10; i++)
	{
		getTP()->enqueue_task([i]() {work(i % 3 + 1, i);  }); //lambda expression
	}

	finalize();

	return 0;
}