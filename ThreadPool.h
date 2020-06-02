#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>

//threadpool class.
//Pattern : producer / consumer pattern
//consumer : threads in thread_pool
//pipe : task_queue
//producer : in main thread...
class ThreadPool {
private:
	bool _stop_threads;
	size_t _num_threads;

	std::mutex _m_job_queue;

	std::vector<std::thread> thread_pool;
	std::queue < std::function<void()> > task_queue;

	void worker();
public:
	ThreadPool(size_t num_threads);
	~ThreadPool();
	void enqueue_task(std::function<void()>task);
};