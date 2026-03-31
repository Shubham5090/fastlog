#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#include <string_view>

struct Log {
    std::string_view level;
    std::string_view user;
    int latency;
};

inline Log parse_line(const char* start, const char* end) {
    Log log;

    const char* p = start;

    // skip timestamp
    while (p<end && *p != ' ') p++;
    p++;

    // level
    const char* lvl_start = p;
    while (p<end && *p != ' ') p++;
    log.level = string_view(lvl_start, p - lvl_start);
    p++;

    // user
    const char* userstart = p;
    while (p<end && *p != ' ') p++;
    log.user = string_view(userstart, p - userstart);
    p++;

    // latency
    int latency = 0;
    while (p<end && *p >= '0' && *p <= '9') {
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
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cout << "Error opening file\n";
        return 1;
    }
    vector<Log> logs;

    logs.reserve(5000000);
    const int CHUNK_SIZE = 8 * 1024 * 1024; // 8 MB
    vector<char> buffer(CHUNK_SIZE);
    string leftover;

    // -------- READ--------

    auto read_start = steady_clock::now();
    while (file) {
        file.read(buffer.data(), CHUNK_SIZE);
        streamsize bytes_read = file.gcount();
        if (bytes_read == 0) break;
        const char* chunk_start = buffer.data();
        const char* chunk_end   = chunk_start + bytes_read;
        const char* p           = chunk_start;
        while (p < chunk_end) {
            const char* nl = (const char*)memchr(p, '\n', chunk_end - p);
            if (!nl) {
                leftover.append(p, chunk_end - p);
                break;
            }
            if (!leftover.empty()) {
                leftover.append(p, nl - p);
                logs.push_back(parse_line(leftover.data(), leftover.data() + leftover.size()));
                leftover.clear();
            } else {
                logs.push_back(parse_line(p, nl));
            }
            p = nl + 1;
        }
    }

    if (!leftover.empty()) {
        logs.push_back(parse_line(leftover.data(), leftover.data() + leftover.size()));
        leftover.clear();
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
        level_count[string(log.level)]++;
        user_count[string(log.user)]++;
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
