#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
int num_threads = 0;

thread threads[8];
int t = 0;

mutex cnt_mutex;
mutex thread_mutex;

void solve(int index, uint64_t current) {
    if (index == m) {
        if (current == (one << n) - 1){
            lock_guard<mutex> lock(cnt_mutex);
            global_count++;
        }
    } else {
        bool flag = false;
        if(num_threads == (1 << (index+1))){
            // lock_guard<mutex> lock(thread_mutex);
            // cerr << "thread " << t << " is created" << endl;
            threads[t++] = thread(solve, index + 1, current);
            threads[t++] = thread(solve, index + 1, current | subsets[index]);
        } else {
            solve(index + 1, current);
            solve(index + 1, current | subsets[index]);
        }
    }
}

int main(int argc, char* argv[]) {
    num_threads = 1;
    // a parameter -t indicates the number of threads
    if(argc == 3 && string(argv[1]) == "-t"){
        num_threads = stoi(argv[2]);
    }
    cout << num_threads << endl;
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    solve(0, 0);
    // cout << t << endl;
    for(int i=0;i<t;i++){
        threads[i].join();
    }
    cout << global_count << endl;
    return 0;
}
