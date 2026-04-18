#include <bits/stdc++.h>
#include <thread>
#include <string_view>
using namespace std;
using namespace std::chrono;

// Per-thread local result

struct t_result {
    vector<int> level_count{4, 0};
    unordered_map<string, int> user_count;
    vector<int> latencies;
    long long total_latency = 0;
};

// Parse one line into t_result

inline void parse_line(const char* start, const char* end, t_result& res) {
    const char* p = start;

    // skip timestamp
    while (p < end && *p != ' ') p++;
    p++;

    // level
    const char* lvl_start = p;
    while (p < end && *p != ' ') p++;
    string_view level(lvl_start, p - lvl_start);
    p++;

    // user
    const char* userstart = p;
    while (p < end && *p != ' ') p++;
    string_view user(userstart, p - userstart);
    p++;

    // latency
    int latency = 0;
    while (p < end && *p >= '0' && *p <= '9') {
        latency = latency * 10 + (*p - '0');
        p++;
    }

    if (level == "INFO")  res.level_count[0]++;
    if (level == "WARN")  res.level_count[1]++;
    if (level == "ERROR") res.level_count[2]++;
    if (level == "DEBUG") res.level_count[3]++;

    res.user_count[string(user)]++;
    res.total_latency += latency;
    res.latencies.push_back(latency);
}

// Worker: reads and parses from file

void worker(const string& filename, long long byte_start, long long byte_end, t_result& t_res) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    file.seekg(byte_start);
    long long pos = byte_start;

    t_res.user_count.reserve(2500000);
    t_res.latencies.reserve(2500000);

    const int CHUNK_SIZE = 8 * 1024 * 1024;
    vector<char> buffer(CHUNK_SIZE);
    string leftover;

    while (file && pos < byte_end) {
        long long remaining = byte_end - pos;
        long long to_read = min((long long)CHUNK_SIZE, remaining);

        file.read(buffer.data(), to_read);
        streamsize bytes_read = file.gcount();
        if (bytes_read == 0) break;

        const char* chunk_start = buffer.data();
        const char* chunk_end = chunk_start + bytes_read;
        const char* p = chunk_start;

        while (p < chunk_end) {
            const char* nl = (const char*)memchr(p, '\n', chunk_end - p);
            if (!nl) {
                leftover.append(p, chunk_end - p);
                break;
            }
            if (!leftover.empty()) {
                leftover.append(p, nl - p);
                parse_line(leftover.data(), leftover.data() + leftover.size(), t_res);
                leftover.clear();
            } else {
                parse_line(p, nl, t_res);
            }
            p = nl + 1;
        }

        pos += bytes_read;
    }

    if (!leftover.empty()) {
        parse_line(leftover.data(), leftover.data() + leftover.size(), t_res);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./final <file>\n";
        return 1;
    }

    string filename = argv[1];

    ifstream tmp(filename, ios::binary | ios::ate);
    if (!tmp.is_open()) {
        cout << "Error opening file\n";
        return 1;
    }
    long long file_size = tmp.tellg();

    if (file_size == 0) {
        cout << "Empty file.\n";
        return 0;
    }

    tmp.seekg(file_size / 2);
    string skip;
    getline(tmp, skip);
    long long split = tmp.tellg();
    tmp.close();

    long long mid = split;

    // -------- READ + PARSE --------

    t_result t0_res, t1_res;

    auto read_parse_start = steady_clock::now();

    thread t0(worker, filename, 0LL, mid, ref(t0_res));
    thread t1(worker, filename, mid, file_size, ref(t1_res));

    t0.join();
    t1.join();

    auto read_parse_end = steady_clock::now();

    // -------- MERGE --------

    auto proc_start = steady_clock::now();

    // Level counts
    vector<int> level_count(4);
    for (int i = 0; i < 4; i++)
        level_count[i] = t0_res.level_count[i] + t1_res.level_count[i];

    // Latencies
    vector<int>& latencies = t0_res.latencies;
    latencies.insert(latencies.end(), make_move_iterator(t1_res.latencies.begin()), make_move_iterator(t1_res.latencies.end()));
    t1_res.latencies.clear();
    t1_res.latencies.shrink_to_fit();

    long long total_latency = t0_res.total_latency + t1_res.total_latency;

    // User counts (merge smaller map into larger for speed)
    auto& big   = (t0_res.user_count.size() >= t1_res.user_count.size()) ? t0_res.user_count : t1_res.user_count;
    auto& small = (t0_res.user_count.size() >= t1_res.user_count.size()) ? t1_res.user_count : t0_res.user_count;
    for (auto& [user, cnt] : small)
        big[user] += cnt;

    // -------- PROCESS --------

    // Average latency
    double avg_latency = (double)total_latency / latencies.size();

    // Top 10 users
    priority_queue<pair<int,string_view>, vector<pair<int,string_view>>, greater<pair<int,string_view>>> min_heap;

    for (const auto& [user, freq] : big) {
        min_heap.push({freq, user});
        if (min_heap.size() > 10) min_heap.pop();
    }
    vector<pair<int,string_view>> top10;
    while (!min_heap.empty()) {
        top10.push_back(min_heap.top());
        min_heap.pop();
    }
    reverse(top10.begin(), top10.end());

    // 95th percentile
    size_t idx = (size_t)(0.95 * latencies.size());
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
    for (int i = 0; i < (int)top10.size(); i++)
        cout << top10[i].second << " : " << top10[i].first << "\n";

    cout << "\n95th Percentile Latency: " << p95 << " ms\n";

    auto read_time = duration_cast<milliseconds>(read_parse_end - read_parse_start).count();
    auto proc_time = duration_cast<milliseconds>(proc_end - proc_start).count();

    cout << "\nRead and Parse Time: " << read_time << " ms\n";
    cout << "Processing Time: " << proc_time << " ms\n";

    return 0;
}
