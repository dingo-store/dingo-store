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

#ifndef DINGODB_CLUSTER_STAT_SERVICE_H_
#define DINGODB_CLUSTER_STAT_SERVICE_H_

#include <memory>
#include <ostream>
#include <string>

#include "brpc/builtin/tabbed.h"
#include "coordinator/auto_increment_control.h"
#include "coordinator/coordinator_control.h"
#include "coordinator/kv_control.h"
#include "coordinator/tso_control.h"
#include "proto/cluster_stat.pb.h"

namespace dingodb {

class ClusterStatImpl : public pb::cluster::dingo, public brpc::Tabbed {
 public:
  ClusterStatImpl() = default;
  void default_method(::google::protobuf::RpcController* controller, const pb::cluster::ClusterStatRequest* request,
                      pb::cluster::ClusterStatResponse* response, ::google::protobuf::Closure* done) override;
  void GetTabInfo(brpc::TabInfoList*) const override;
  void SetControl(std::shared_ptr<CoordinatorControl> coordinator_controller, std::shared_ptr<KvControl> kv_controller,
                  std::shared_ptr<TsoControl> tso_controller,
                  std::shared_ptr<AutoIncrementControl> auto_increment_controller);

 private:
  std::shared_ptr<CoordinatorControl> coordinator_controller_;
  std::shared_ptr<KvControl> kv_controller_;
  std::shared_ptr<TsoControl> tso_controller_;
  std::shared_ptr<AutoIncrementControl> auto_increment_controller_;

  static void PrintHtmlTable(std::ostream& os, bool use_html, const std::vector<std::string>& table_header,
                             const std::vector<int32_t>& min_widths,
                             const std::vector<std::vector<std::string>>& table_contents,
                             const std::vector<std::vector<std::string>>& table_urls);

  void PrintStores(std::ostream& os, bool use_html);
  void PrintExecutors(std::ostream& os, bool use_html);
  void PrintRegions(std::ostream& os, bool use_html);
  void PrintSchemaTables(std::ostream& os, bool use_html);
};

}  // namespace dingodb

#endif  // DINGODB_CLUSTER_STAT_SERVICE_H_
