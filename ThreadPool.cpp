#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) : _num_threads(num_threads), _stop_threads(false) {

	thread_pool.reserve(_num_threads);
	
	//[] : introducer. some parameters send into lambda function.
	//lambda function catches this keyword, and use in function
	for (int i = 0; i < num_threads; i++) {
		thread_pool.push_back(std::thread([this]() {
			this->worker();
		}));
	}
}

ThreadPool::~ThreadPool() {
	_stop_threads  = true;
	
	for (auto& t : thread_pool)
	{
		t.join();
	}

	printf("end Threads!!\n");
}

//worker - thread model...
//it should have mutex lock
/*
void ThreadPool::enqueue_task(std::function<void()>task) {
	if (_stop_threads)
	{
		//if you have logger. 
		return;
	}

	_m_job_queue.lock();
	task_queue.push(task);
	_m_job_queue.unlock();
}
*/

void ThreadPool::worker() {
	while (true) {
		_m_job_queue.lock();
		
		if (task_queue.size() == 0) {
			_m_job_queue.unlock();
			if (_stop_threads)
				break;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}

		std::function<void()> task = std::move(task_queue.front());
		task_queue.pop();
		_m_job_queue.unlock();

		task();
	}
}