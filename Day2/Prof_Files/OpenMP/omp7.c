#include <iostream>
#include <omp.h>

void task_function(int task_id) {
	    std::cout << "Task " << task_id << " executed by Thread " << omp_get_thread_num() << std::endl;
}

int main() {
	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task
			task_function(1);

			#pragma omp task
			task_function(2);

			#pragma omp task
			task_function(3);

			#pragma omp taskwait
									        
		}
			        
	}

	        return 0;
}

