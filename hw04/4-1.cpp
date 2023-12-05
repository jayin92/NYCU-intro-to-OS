#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> c(n, -1);
    int d;
    int miss = 0;
    for(int i=0;i<m;i++){
        cin >> d;
        // cout << d << " " << d % n << endl;
        bool found = false;
        for(auto j: c){
            if(j == d){
                found = true;
                break;
            }
        }
        if(!found){
            miss++;
            c[i % n] = d;
        }

    }
    cout << "Total Cache Misses:";
    cout << miss;
}