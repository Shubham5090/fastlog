/*
-- run this code to generate test cases
-- change the TARGET_SIZE to generate different sizes of log files
-- change the filename to generate different test cases
*/
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

const vector<string> LEVELS = {"INFO", "WARN", "ERROR", "DEBUG"};
const vector<string> MESSAGES = {
    "Request completed", "Database connection failed", "User authenticated",
    "Cache miss",        "Timeout occurred",           "File uploaded",
    "Permission denied"};

string random_timestamp(mt19937 &rng) {
  uniform_int_distribution<int> year(2023, 2026);
  uniform_int_distribution<int> month(1, 12);
  uniform_int_distribution<int> day(1, 28);
  uniform_int_distribution<int> hour(0, 23);
  uniform_int_distribution<int> minute(0, 59);
  uniform_int_distribution<int> second(0, 59);

  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02dZ", year(rng),
           month(rng), day(rng), hour(rng), minute(rng), second(rng));

  return string(buffer);
}

int main() {
  const string filename =
      "tc2.log"; // change this to generate different test cases
  const uint64_t TARGET_SIZE =
      1ULL * 1024 * 1024 *
      1024; // 1 GB change this to generate different sizes of log files

  ofstream out(filename, ios::out | ios::binary);
  if (!out) {
    cerr << "Error opening file\n";
    return 1;
  }

  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  uniform_int_distribution<int> level_dist(0, LEVELS.size() - 1);
  uniform_int_distribution<int> user_dist(1, 10000);
  uniform_int_distribution<int> latency_dist(1, 500);
  uniform_int_distribution<int> msg_dist(0, MESSAGES.size() - 1);

  uint64_t current_size = 0;
  string line;

  while (current_size < TARGET_SIZE) {
    line = random_timestamp(rng) + " " + LEVELS[level_dist(rng)] + " " +
           "user" + to_string(user_dist(rng)) + " " +
           to_string(latency_dist(rng)) + "ms " + "\"" +
           MESSAGES[msg_dist(rng)] + "\"\n";

    out.write(line.c_str(), line.size());
    current_size += line.size();
  }

  out.close();
  cout << "Generated ~1GB log file: " << filename << endl;
  return 0;
}
