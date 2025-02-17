// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DINGODB_COMMON_RUNNABLE_H_
#define DINGODB_COMMON_RUNNABLE_H_

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bthread/execution_queue.h"

namespace dingodb {

class TaskRunnable {
 public:
  TaskRunnable();
  virtual ~TaskRunnable();

  uint64_t Id() const;
  static uint64_t GenId();

  virtual std::string Type() = 0;

  virtual void Run() = 0;

  virtual std::string Trace() { return ""; }

 private:
  uint64_t id_;
};

using TaskRunnablePtr = std::shared_ptr<TaskRunnable>;

int ExecuteRoutine(void*, bthread::TaskIterator<TaskRunnablePtr>& iter);

// Run task worker
class Worker {
 public:
  enum class EventType {
    kAddTask = 0,
    kFinishTask = 1,
  };
  using NotifyFuncer = std::function<void(EventType)>;

  Worker(NotifyFuncer notify_func);
  ~Worker();

  static std::shared_ptr<Worker> New() { return std::make_shared<Worker>(nullptr); }
  static std::shared_ptr<Worker> New(NotifyFuncer notify_func) { return std::make_shared<Worker>(notify_func); }

  bool Init();
  void Destroy();

  bool Execute(TaskRunnablePtr task);

  uint64_t TotalTaskCount();
  void IncTotalTaskCount();

  uint64_t PendingTaskCount();
  void IncPendingTaskCount();
  void DecPendingTaskCount();

  void Nodify(EventType type);

  void AppendPendingTaskTrace(uint64_t task_id, const std::string& trace);
  void PopPendingTaskTrace(uint64_t task_id);
  std::vector<std::string> GetPendingTaskTrace();

 private:
  // Execution queue is available.
  std::atomic<bool> is_available_;
  bthread::ExecutionQueueId<TaskRunnablePtr> queue_id_;  // NOLINT

  // Metrics
  std::atomic<uint64_t> total_task_count_{0};
  std::atomic<int64_t> pending_task_count_{0};

  // Notify
  NotifyFuncer notify_func_;

  // trace
  bool is_use_trace_;
  bthread_mutex_t trace_mutex_;
  std::map<uint64_t, std::string> pending_task_traces_;
};

using WorkerPtr = std::shared_ptr<Worker>;

class WorkerSet {
 public:
  WorkerSet(std::string name, uint32_t worker_num, int64_t max_pending_task_count);
  ~WorkerSet();

  static std::shared_ptr<WorkerSet> New(std::string name, uint32_t worker_num, uint32_t max_pending_task_count) {
    return std::make_shared<WorkerSet>(name, worker_num, max_pending_task_count);
  }

  bool Init();
  void Destroy();

  bool ExecuteRR(TaskRunnablePtr task);
  bool ExecuteHashByRegionId(int64_t region_id, TaskRunnablePtr task);

  void WatchWorker(Worker::EventType type);

  uint64_t TotalTaskCount();
  void IncTotalTaskCount();

  uint64_t PendingTaskCount();
  void IncPendingTaskCount();
  void DecPendingTaskCount();

  std::vector<std::vector<std::string>> GetPendingTaskTrace();

 private:
  const std::string name_;
  int64_t max_pending_task_count_;
  uint32_t worker_num_;
  std::vector<WorkerPtr> workers_;
  std::atomic<uint64_t> active_worker_id_;

  std::atomic<int64_t> pending_task_count_{0};

  // Metrics
  bvar::Adder<uint64_t> total_task_count_metrics_;
  bvar::Adder<int64_t> pending_task_count_metrics_;
};

using WorkerSetPtr = std::shared_ptr<WorkerSet>;

}  // namespace dingodb

#endif  // DINGODB_COMMON_RUNNABLE_H_