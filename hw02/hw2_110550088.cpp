#include <bits/stdc++.h>

using namespace std;

int cur_time = 0;

class Process{
public:
    Process(int arrival_, int bust_): arraival(arrival_), bust(bust_) {
        wait = -arrival_;
        turnaround = 0;
    }
    int arraival;
    int bust;
    int wait;
    int turnaround;
};

class Scheduler{
public:
    Scheduler() { }
    virtual void run() = 0;
    virtual void push(Process* proc) = 0;
    virtual void preempt() = 0;
    virtual bool empty() = 0;
    Scheduler* nxt;
};

Process* last_exec_proc = nullptr;
Scheduler* last_exec_scheduler = nullptr;

class FCFS: public Scheduler{
public:
    FCFS(){
        queue = vector<Process*>();
        nxt = nullptr;
    }

    FCFS(Scheduler* nxt_) {
        queue = vector<Process*>();
        nxt = nxt_;
    }
    
    bool empty(){
        return queue.empty();
    }

    void run(){
        auto proc = queue[0];
        proc -> bust --;
        proc -> wait --;
        if(last_exec_proc != nullptr && last_exec_proc != proc){
            last_exec_scheduler -> preempt();
        }
        if(proc -> bust == 0){
            queue.erase(queue.begin());
            proc -> turnaround = cur_time + 1 - proc -> arraival;
            last_exec_proc = nullptr;
            last_exec_scheduler = nullptr;
            return;
        } else {
            last_exec_proc = proc;
            last_exec_scheduler = this;
        }
    }

    void push(Process* proc){
        queue.push_back(proc);
    }

    void pop(Process* proc){
        int sz = queue.size();
        for(int i=0;i<sz;i++){
            if(queue[i] == proc){
                queue.erase(queue.begin() + i);
                break;
            }
        }
    }

    void preempt(){
        if(last_exec_proc != nullptr){
            if(nxt != nullptr){
                nxt -> push(last_exec_proc);
                pop(last_exec_proc);
            } else {
                pop(last_exec_proc);
                push(last_exec_proc);
            }
        }
    }

    vector<Process*> queue;
};

class SRTF_cmp {
public:
    bool operator() (Process* a, Process* b) {
        if(a -> bust == b -> bust){
            return a -> arraival > b -> arraival;
        }
        return a -> bust > b -> bust;
    }
};

class SRTF: public Scheduler{
public:
    SRTF(){
        queue = priority_queue<Process*, vector<Process*>, SRTF_cmp>();
        nxt = nullptr;
    }

    SRTF(Scheduler* nxt_) {
        queue = priority_queue<Process*, vector<Process*>, SRTF_cmp>();
        nxt = nxt_;
    }

    bool empty(){
        return queue.empty();
    }

    void run(){
        auto proc = queue.top();
        queue.pop();
        proc -> bust --;
        proc -> wait --;
        if(last_exec_proc != nullptr && last_exec_proc != proc){
            last_exec_scheduler -> preempt();
        }   
        if(proc -> bust == 0){
            proc -> turnaround = cur_time + 1 - proc -> arraival;
            last_exec_proc = nullptr;
            return;
        } else{
            queue.push(proc);
        }
        last_exec_proc = proc;
        last_exec_scheduler = this;
    }

    void push(Process* proc){
        queue.push(proc);
    }

    void pop(Process* proc){
        while(!queue.empty()){
            auto cur = queue.top();
            queue.pop();
            if(cur == proc){
                break;
            }
            queue.push(cur);
        }
    }

    void preempt(){
        if(last_exec_proc != nullptr){
            if(nxt != nullptr){
                nxt -> push(last_exec_proc);
                pop(last_exec_proc);
            } else {
                pop(last_exec_proc);
                push(last_exec_proc);
            }
        }
    }

    priority_queue<Process*, vector<Process*>, SRTF_cmp> queue;
};

bool RR_cmp(pair<Process*, int> a, pair<Process*, int> b){
    if(a.second != b.second){
        return a.second < b.second;
    }
    return a.first -> arraival > b.first -> arraival;
}

class RR: public Scheduler{
public:
    RR(int tq){
        queue = vector<pair<Process*,int>>();
        nxt = nullptr;
        time_q = tq;
        cur_q = tq;
    }

    RR(int tq, Scheduler* nxt_) {
        queue = vector<pair<Process*,int>>();
        nxt = nxt_;
        time_q = tq;
        cur_q = tq;
    }

    bool empty(){
        return queue.empty();
    }

    void run(){
        auto proc = queue[0].first;
        if(last_exec_proc != nullptr && last_exec_proc != proc){
            last_exec_scheduler -> preempt();
        }
        proc -> bust --;
        proc -> wait --;
        cur_q --;

        if(proc -> bust == 0){
            proc -> turnaround = cur_time + 1 - proc -> arraival;
            queue.erase(queue.begin());
            last_exec_proc = nullptr;
            cur_q = time_q;
            return;
        } else if(cur_q == 0){
            cur_q = time_q;
            queue.erase(queue.begin());
            cur_time ++;
            if(nxt == nullptr){
                push(proc);
            } else {
                nxt -> push(proc);
            }
            cur_time --;
            last_exec_proc = nullptr;
            return;
        }
        last_exec_proc = proc;
        last_exec_scheduler = this;
    }



    void push(Process* proc){
        queue.push_back({proc, cur_time});
        sort(queue.begin(), queue.end(), RR_cmp);
    }

    void pop(Process* proc){
        int sz = queue.size();
        for(int i=0;i<sz;i++){
            if(queue[i].first == proc){
                queue.erase(queue.begin() + i);
                break;
            }
        }
    }

    void preempt(){
        cur_q = time_q;
        if(last_exec_proc != nullptr){
            if(nxt != nullptr){
                pop(last_exec_proc);
                nxt -> push(last_exec_proc);
            } else {
                pop(last_exec_proc);
                push(last_exec_proc);
            }
        }
    }

    int time_q;
    int cur_q;
    vector<pair<Process*, int>> queue;
};

void wait(vector<Process*>& procs){
    for(auto i: procs){
        if(i -> bust > 0){
            i -> wait ++;
        }
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    vector<Process*> procs;
    vector<Scheduler*> schedulers;
    int mode, time_q;
    for(int i=0;i<n;i++){
        cin >> mode >> time_q;
        if(mode == 0){
            schedulers.push_back(new FCFS());
        } else if(mode == 1){
            schedulers.push_back(new SRTF());
        } else if(mode == 2){
            schedulers.push_back(new RR(time_q));
        }
    }
    for(int i=0;i<n-1;i++){
        schedulers[i] -> nxt = schedulers[i+1];
    }
    int arrv, bust;
    for(int i=0;i<m;i++){
        cin >> arrv >> bust;
        Process* proc = new Process(arrv, bust);
        procs.push_back(proc);
    }
    int idx = 0;
    while(true){
        while(idx != m && procs[idx] -> arraival <= cur_time){
            schedulers[0] -> push(procs[idx]);
            idx ++;
        }
        Scheduler* cur_sc = nullptr;
        for(int i=0;i<n;i++){
            if(schedulers[i] -> empty() == false){
                cur_sc = schedulers[i];
                break;
            }
        }
        if(cur_sc == nullptr && idx == m){
            break;
        }
        wait(procs);
        if(cur_sc == nullptr){
            cur_time ++;
            continue;
        }
        cur_sc -> run();
        cur_time ++;
        
    }
    int total_wait, total_turnaround;
    total_wait = total_turnaround = 0;
    for(int i=0;i<m;i++){
        cout << procs[i]->wait << " " << procs[i]->turnaround << endl;
        total_wait += procs[i]->wait;
        total_turnaround += procs[i]->turnaround;
    }
    cout << total_wait << endl;
    cout << total_turnaround << endl;
}