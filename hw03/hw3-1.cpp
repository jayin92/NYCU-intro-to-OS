#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore.h>

using namespace std;

sem_t sems[100];

void count1(int index) {
  static mutex io_mutex;
  int num = 1000000;
  sem_wait(&sems[index]);
  while (num--) {}
  {
    lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
    sem_post(&sems[index+1]);
  }
}

int main(void) {
  thread t[100];
  sem_init(&sems[0], 0, 1);
  for(int i=1;i<100;i++)
    sem_init(&sems[i], 0, 0);

  for (int i = 0; i < 100; i++)
    t[i] = thread(count1, i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
