#include "Functions.h"


int main()
{
	int start, finish = 0;
	Intialize_Htable();

	start = clock();
	std::thread myThreads[MAX_THREAD];

	for (int i = 0; i < MAX_THREAD; i++) {

		myThreads[i] = std::thread(thread_function);
		while (flag[i + 1] != 1);//waiting for each thread processing to be completed.
	}
	for (int i = 0; i < MAX_THREAD; i++)
		myThreads[i].join();
	
	finish = clock();
	cout << endl;
	//cout << "-----------Total Time to Perform Execution: " << finish - start << "----------------" << endl << endl;
	return 0;
}

