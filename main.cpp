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

int work(int t, int id) {
	printf("%d start \n", id);

	std::this_thread::sleep_for(std::chrono::seconds(t));

	printf("%d end after %ds\n", id, t);

	return t + id;
}

//It is ThreadPool function but lnk 2019 error occurred when it build in ThreadPool.cpp file
template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::enqueue_task(
	F&& f, Args&&... args) {
	if (_stop_threads) {
		throw std::runtime_error("ThreadPool Stopped");
	}

	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	std::future<return_type> task_result_future = task->get_future();
	{
		_m_job_queue.lock();
		task_queue.push([task]() { (*task)(); });
		_m_job_queue.unlock();
	}

	return task_result_future;
}

int main() {
	initialize();
	
	std::vector<std::future<int>> futures;
	for (int i = 0; i < 10; i++)
	{
		futures.push_back(tp->enqueue_task(work, i % 3 + 1, i));
		//tp->enqueue_task(work, i % 3 + 1, i);	
		//getTP()->enqueue_task([i]() {work(i % 3 + 1, i);  }); //lambda expression
	}

	for (auto& f : futures) {
		printf("result : %d \n", f.get());
	}

	finalize();
	
	return 0;
}