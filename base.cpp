#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

struct Log {
    string level;
    string user;
    int latency;
};

inline Log parse_line(const string& line) {
    Log log;

    const char* p = line.c_str();

    // skip timestamp
    while (*p && *p != ' ') p++;
    p++;

    // level
    const char* start = p;
    while (*p && *p != ' ') p++;
    log.level = string(start, p - start);
    p++;

    // user
    start = p;
    while (*p && *p != ' ') p++;
    log.user = string(start, p - start);
    p++;

    // latency
    int latency = 0;
    while (*p >= '0' && *p <= '9') {
        latency = latency * 10 + (*p - '0');
        p++;
    }
    log.latency = latency;

    return log;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./fastlog <file>\n";
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file\n";
        return 1;
    }

    vector<Log> logs;
    logs.reserve(5000000);

    string line;

    // -------- READ --------
    auto read_start = steady_clock::now();

    while (getline(file, line)) {
        logs.push_back(parse_line(line));
    }

    auto read_end = steady_clock::now();

    // -------- PROCESS --------
    auto proc_start = steady_clock::now();

    unordered_map<string, int> level_count;
    unordered_map<string, int> user_count;
    vector<int> latencies;
    latencies.reserve(logs.size());

    long long total_latency = 0;

    for (const auto& log : logs) {
        level_count[log.level]++;
        user_count[log.user]++;
        total_latency += log.latency;
        latencies.push_back(log.latency);
    }

    // Average
    double avg_latency = (double)total_latency / logs.size();

    // Top 10 users
    vector<pair<string, int>> users(user_count.begin(), user_count.end());
    sort(users.begin(), users.end(),
         [](auto& a, auto& b) { return a.second > b.second; });

    // 95th percentile
    size_t idx = 0.95 * latencies.size();
    nth_element(latencies.begin(), latencies.begin() + idx, latencies.end());
    int p95 = latencies[idx];

    auto proc_end = steady_clock::now();

    // -------- OUTPUT --------
    cout << "Log Level Counts:\n";
    for (auto& p : level_count) {
        cout << p.first << ": " << p.second << "\n";
    }

    cout << "\nAverage Latency: " << avg_latency << " ms\n";

    cout << "\nTop 10 Users:\n";
    for (int i = 0; i < 10 && i < users.size(); i++) {
        cout << users[i].first << " : " << users[i].second << "\n";
    }

    cout << "\n95th Percentile Latency: " << p95 << " ms\n";

    auto read_time = duration_cast<milliseconds>(read_end - read_start).count();
    auto proc_time = duration_cast<milliseconds>(proc_end - proc_start).count();

    cout << "\nRead Time: " << read_time << " ms\n";
    cout << "Processing Time: " << proc_time << " ms\n";

    return 0;
}