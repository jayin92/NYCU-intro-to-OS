#include <bits/stdc++.h>

using namespace std;

int main(){
    int n, k, m;
    cin >> n >> k >> m;
    vector<pair<int, int>> c(n, {-1, -1});
    int d;
    int miss = 0;
    for(int i=0;i<m;i++){
        cin >> d;
        bool found = false;
        for(auto &j: c){
            if(j.first == d){
                found = true;
                j.second = i;
                break;
            }
        }
        if(!found){
            miss++;
            int s = i % (n / k);
            int min_idx = s*k;
            for(int j=s*k;j<(s+1)*k;j++){
                if(c[j].first == -1){
                    min_idx = j;
                    break;
                }
                if(c[j].second < c[min_idx].second){
                    min_idx = j;
                }
            }
            c[min_idx] = make_pair(d, i);
        }
    }
    cout << "Total Cache Misses:";
    cout << miss;
}