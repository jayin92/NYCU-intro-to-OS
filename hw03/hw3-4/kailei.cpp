#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>

using namespace std;

int n, m, num_threads;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);
vector<uint64_t> cnt(8, 0);
vector<thread> threads;

void solve(int index, uint64_t current, int thread_id) {
    if (index == m) {
        if (current == (one << n) - 1) {
            // cout << "thread " << thread_id << " found a solution\n";
            cnt[thread_id]++;
        }
    } else if ((1 << (index + 1)) == num_threads) {
        threads.push_back(thread(solve, index + 1, current, threads.size()));
        cout << "thread " << threads.size() << " created\n";
        threads.push_back(thread(solve, index + 1, current | subsets[index], threads.size()));
    } else {
        solve(index + 1, current, thread_id);
        solve(index + 1, current | subsets[index], thread_id);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3 || string(argv[1]) != "-t") {
        cout << "Usage: " << argv[0] << " -t <num_threads>\n";
        return 1;
    }

    num_threads = stoi(argv[2]);
    cin >> n >> m;
    cout << num_threads << endl;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    solve(0, 0, 0);

    for (auto& t : threads) {
        t.join();
    }

    uint64_t global_count = 0;
    for (int i = 0; i < num_threads; i++) {
        global_count += cnt[i];
    }

    cout << global_count << endl;
    return 0;
}
