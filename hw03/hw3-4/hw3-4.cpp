#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
int num_threads = 0;
int t = 0;

thread threads[8];
int cnt[8] = {0};


void solve(int index, uint64_t current, int id) {
    if (index == m) {
        if (current == (one << n) - 1){
            cnt[id] ++;
        }
    } else {
        bool flag = false;
        if(num_threads == (1 << (index+1))){
            threads[t++] = thread(solve, index + 1, current, t);
            threads[t++] = thread(solve, index + 1, current | subsets[index], t);
        } else {
            solve(index + 1, current, id);
            solve(index + 1, current | subsets[index], id);
        }
    }
}

int main(int argc, char* argv[]) {
    num_threads = 1;
    // a parameter -t indicates the number of threads
    if(argc == 3 && string(argv[1]) == "-t"){
        num_threads = stoi(argv[2]);
    }
    // cout << num_threads << endl;
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

    solve(0, 0, -1);
    for(int i=0;i<t;i++){
        threads[i].join();
        global_count += cnt[i];
    }
    cout << global_count << endl;
    return 0;
}
