#include <bits/stdc++.h>
using namespace std;
#define loop(i,a,b) for(int i=a;i<b;i++)
#define int long long
#define fastio ios::sync_with_stdio(false); cin.tie(nullptr)
#define endl '\n'

class Parser{
    public:
    string time;
    string level;
    string user;
    string latency;
    string message;

    Parser(string s){
        stringstream g(s);
        getline(g,time,' ');
        getline(g,level,' ');
        getline(g,user,' ');
        getline(g,latency,' ');
        getline(g,message,' ');
    }
};
void countLogLevel(const vector<Parser> &v){
    unordered_map<string,int> mpp;
    for(auto i:v){
        mpp[i.level]++;
    }
    for(auto i:mpp){
        cout<<"Level "<<i.first<<" - count: "<<i.second<<endl;
    }
}
void AverageLatency(const vector<Parser> &v){
    double sum=0;
    for(auto i:v){
        sum +=stod(i.latency);
    }
    cout<<"Avg Latency: "<<sum/v.size()<<endl;
}
void TopUsers(const vector<Parser> &v){
    unordered_map<string,int> mpp;
    for(auto i:v){
        mpp[i.user]++;
    }
    vector<pair<int,string>> count;
    for(auto i:mpp){
        count.push_back({i.second,i.first});
    }
    sort(count.rbegin(),count.rend());
    for(int i=0;i<10;i++){
        cout<<i<<"."<<count[i].second<<endl;
    }
}
void PercentileLatency(const vector<Parser> &v){
    vector<double> latencylist;
    for(auto i:v){
        latencylist.push_back(stod(i.latency));
    }
    sort(latencylist.begin(),latencylist.end());
    int index = floor(0.95*v.size());
    cout<<"95th percentile latency is: "<<latencylist[index]<<endl;
}
int32_t main() {
    fastio;
    int t = 1;
    // cin >> t;
    while(t--) {
        // your code
    }
    vector<Parser> Parservec;
    fstream file("/home/siddharth/fastlog_sid/log generator/tc1.log");
    string line;
    auto starttime = chrono::steady_clock::now();
    while(getline(file,line)){
        Parser*p = new Parser(line);
        Parservec.push_back(*p);
        //cout<<"time: "<<p->time<<" ;"<<"level: "<<p->level<<" ;"<<"user: "<<p->user<<" ;"<<"latency: "<<p->latency<<" ;"<<"msg: "<<p->message<<endl;
        delete p;
        p = nullptr;
    }
    auto endtime = chrono::steady_clock::now();
    chrono::duration<double> duration = endtime - starttime;
    cout<<"Reading time: "<<duration.count()<<"seconds"<<endl;
    starttime = chrono::steady_clock::now();
     countLogLevel(Parservec);
        AverageLatency(Parservec);
        TopUsers(Parservec);
        PercentileLatency(Parservec);
    endtime = chrono::steady_clock::now();
    duration = endtime-starttime;
    cout<<"Processing Time: "<<duration.count()<<"seconds"<<endl;
    return 0;
}
