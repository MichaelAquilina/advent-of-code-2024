#include "lib.h"
#include "absl/strings/str_split.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <tuple>

std::tuple<std::map<int, std::vector<int>>, std::vector<std::vector<int>>>
read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to read file");
  }

  std::map<int, std::vector<int>> queue;
  std::vector<std::vector<int>> jobs;
  std::string line;

  while (getline(file, line)) {
    if (line == "") {
      break;
    }
    std::vector<std::string> tokens = absl::StrSplit(line, "|");
    queue[stoi(tokens.at(1))].push_back(stoi(tokens.at(0)));
  }

  while (getline(file, line)) {
    std::vector<int> job;
    std::vector<std::string> tokens = absl::StrSplit(line, ",");
    for (auto &page : tokens) {
      job.push_back(stoi(page));
    }
    jobs.push_back(job);
  }

  return {queue, jobs};
}

bool validate_job(std::map<int, std::vector<int>> &queue,
                  std::vector<int> job) {
  std::set<int> finished_jobs;
  for (auto &page : job) {
    for (auto &dependency : queue.at(page)) {
      // ignore dependencies for pages not in the job
      if (std::find(job.begin(), job.end(), dependency) == job.end()) {
        continue;
      }

      if (!finished_jobs.contains(dependency)) {
        return false;
      }
    }
    finished_jobs.insert(page);
  }
  return true;
}

void traverse(int page, std::map<int, std::vector<int>> &queue,
              std::vector<int> &job, std::vector<int> &result) {
  if (queue.contains(page)) {
    for (auto &item : queue.at(page)) {
      // do not traverse if it is not in the job
      if (std::find(job.begin(), job.end(), item) == job.end()) {
        continue;
      }

      // dont traverse if we already added it in
      if (std::find(result.begin(), result.end(), item) == result.end()) {
        traverse(item, queue, job, result);
        result.push_back(item);
      }
    }
  }
}

std::vector<int> fix_job(std::map<int, std::vector<int>> &queue,
                         std::vector<int> job) {
  std::vector<int> result;

  // perform depth first search on all items
  for (auto &page : job) {
    traverse(page, queue, job, result);
  }

  return result;
}

std::tuple<uint, uint> get_parts(std::map<int, std::vector<int>> &queue,
                                 const std::vector<std::vector<int>> &jobs) {
  uint part1 = 0;
  uint part2 = 0;

  for (auto &job : jobs) {
    if (validate_job(queue, job)) {
      part1 += job[job.size() / 2];
    } else {
      auto fixed_job = fix_job(queue, job);
      part2 += fixed_job.at(fixed_job.size() / 2);
    }
  }

  return {part1, part2};
}
