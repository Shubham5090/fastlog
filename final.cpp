#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#include <string_view>

vector<int> level_count(4);
unordered_map<string, int> user_count;
vector<int> latencies;
long long total_latency = 0;

inline void parse_line(const char* start, const char* end) {

    const char* p = start;

    // skip timestamp
    while (p<end && *p != ' ') p++;
    p++;

    // level
    const char* lvl_start = p;
    while (p<end && *p != ' ') p++;
    string_view level = string_view(lvl_start, p - lvl_start);
    p++;

    // user
    const char* userstart = p;
    while (p<end && *p != ' ') p++;
    string_view user = string_view(userstart, p - userstart);
    p++;

    // latency
    int latency = 0;
    while (p<end && *p >= '0' && *p <= '9') {
        latency = latency * 10 + (*p - '0');
        p++;
    }

    if(level == "INFO") level_count[0]++;
    if(level == "WARN") level_count[1]++;
    if(level == "ERROR") level_count[2]++;
    if(level == "DEBUG") level_count[3]++;
    user_count[string(user)]++;
    total_latency += latency;
    latencies.push_back(latency);

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

    user_count.reserve(5000000);
    latencies.reserve(5000000);

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
                parse_line(leftover.data(), leftover.data() + leftover.size());
                leftover.clear();
            } else {
                parse_line(p, nl);
            }
            p = nl + 1;
        }
    }

    if (!leftover.empty()) {
        parse_line(leftover.data(), leftover.data() + leftover.size());
        leftover.clear();
    }

    auto read_end = steady_clock::now();

    // -------- PROCESS --------

    auto proc_start = steady_clock::now();

     // Average

    double avg_latency = (double)total_latency / latencies.size();

    // Top 10 users
    priority_queue<pair<int, string_view>, vector<pair<int, string_view>>, greater<pair<int, string_view>>> min_heap;
    for(const auto& [user, freq] : user_count){
        min_heap.push({freq, user});
        if(min_heap.size() > 10){
            min_heap.pop();
        }
    }
    std::vector<pair<int, string_view>> reverse_heap;
    while(!min_heap.empty()){
        reverse_heap.push_back(min_heap.top());
        min_heap.pop();
    }
    reverse(reverse_heap.begin(), reverse_heap.end());

    // 95th percentile
    size_t idx = 0.95 * latencies.size();
    nth_element(latencies.begin(), latencies.begin() + idx, latencies.end());
    int p95 = latencies[idx];

    auto proc_end = steady_clock::now();

    // -------- OUTPUT --------
    cout << "Log Level Counts:\n";
    cout << "INFO: " << level_count[0] << "\n";
    cout << "WARN: " << level_count[1] << "\n";
    cout << "ERROR: " << level_count[2] << "\n";
    cout << "DEBUG: " << level_count[3] << "\n";

    cout << "\nAverage Latency: " << avg_latency << " ms\n";

    cout << "\nTop 10 Users:\n";
    for (int i = 0; i < 10 && i < reverse_heap.size(); i++) {
        cout << reverse_heap[i].second << " : " << reverse_heap[i].first << "\n";
    }

    cout << "\n95th Percentile Latency: " << p95 << " ms\n";

    auto read_time = duration_cast<milliseconds>(read_end - read_start).count();
    auto proc_time = duration_cast<milliseconds>(proc_end - proc_start).count();

    cout << "\nRead Time: " << read_time << " ms\n";
    cout << "Processing Time: " << proc_time << " ms\n";

    return 0;
}
