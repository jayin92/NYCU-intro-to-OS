#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int n, global_count = 0;

mutex cnt_mutex;

void is_prime(int l, int r) {
    int count = 0;
    if(l > r) return;
    if(l <= 2 && 2 <= r) count++;
    if(l % 2 == 0) l ++;
    for(int num=l;num<=r;num+=2){
        if(num == 1) continue;
        bool flag = true;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                flag = false;
                break;
            }
        }
        if(flag) {count++; }

    }
    cnt_mutex.lock();
    global_count += count;
    cnt_mutex.unlock();
}

int main(int argc, char* argv[]) {
    int num_threads = 1;
    // a parameter -t indicates the number of threads
    if(argc == 3 && string(argv[1]) == "-t"){
        num_threads = stoi(argv[2]);
    }

    cin >> n;
    thread t[num_threads];
    for(int i=0;i<num_threads;i++){
        // cout << "l: " << i*n/num_threads+1 << " r: " << (i+1)*n/num_threads << endl;
        t[i] = thread(is_prime, i*n/num_threads+1, (i+1)*n/num_threads);
    }
    for(int i=0;i<num_threads;i++){
        t[i].join();
    }

    cout << global_count << endl;
    return 0;
}
