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

#include <cstdint>

#include "gtest/gtest.h"
#include "sdk/vector/vector_common.h"

namespace dingodb {
namespace sdk {

TEST(VectorCommonTest, TestMetricType2InternalMetricTypePB) {
  EXPECT_EQ(MetricType2InternalMetricTypePB(MetricType::kL2), pb::common::MetricType::METRIC_TYPE_L2);

  EXPECT_EQ(MetricType2InternalMetricTypePB(MetricType::kInnerProduct),
            pb::common::MetricType::METRIC_TYPE_INNER_PRODUCT);

  EXPECT_EQ(MetricType2InternalMetricTypePB(MetricType::kCosine), pb::common::MetricType::METRIC_TYPE_COSINE);
}

TEST(VectorCommonTest, TestVectorIndexType2InternalVectorIndexTypePB) {
  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kNoneIndexType),
            pb::common::VECTOR_INDEX_TYPE_NONE);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kFlat), pb::common::VECTOR_INDEX_TYPE_FLAT);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kIvfFlat),
            pb::common::VECTOR_INDEX_TYPE_IVF_FLAT);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kIvfPq), pb::common::VECTOR_INDEX_TYPE_IVF_PQ);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kHnsw), pb::common::VECTOR_INDEX_TYPE_HNSW);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kDiskAnn),
            pb::common::VECTOR_INDEX_TYPE_DISKANN);

  EXPECT_EQ(VectorIndexType2InternalVectorIndexTypePB(VectorIndexType::kBruteForce),
            pb::common::VECTOR_INDEX_TYPE_BRUTEFORCE);
}

TEST(VectorCommonTest, TestInternalVectorIndexTypePB2VectorIndexType) {
  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_NONE),
            VectorIndexType::kNoneIndexType);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_FLAT), VectorIndexType::kFlat);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_IVF_FLAT),
            VectorIndexType::kIvfFlat);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_IVF_PQ), VectorIndexType::kIvfPq);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_HNSW), VectorIndexType::kHnsw);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_DISKANN),
            VectorIndexType::kDiskAnn);

  EXPECT_EQ(InternalVectorIndexTypePB2VectorIndexType(pb::common::VECTOR_INDEX_TYPE_BRUTEFORCE),
            VectorIndexType::kBruteForce);
}

TEST(VectorCommonTest, TestFillFlatParmeter) {
  pb::common::VectorIndexParameter parameter;
  FlatParam param{128, MetricType::kL2};

  FillFlatParmeter(&parameter, param);

  EXPECT_EQ(parameter.vector_index_type(), pb::common::VECTOR_INDEX_TYPE_FLAT);
  EXPECT_EQ(parameter.flat_parameter().dimension(), param.dimension);
  EXPECT_EQ(parameter.flat_parameter().metric_type(), MetricType2InternalMetricTypePB(param.metric_type));
}

TEST(VectorCommonTest, TestFillIvfFlatParmeter) {
  pb::common::VectorIndexParameter parameter;
  IvfFlatParam param{128, MetricType::kL2};

  FillIvfFlatParmeter(&parameter, param);

  EXPECT_EQ(parameter.vector_index_type(), pb::common::VECTOR_INDEX_TYPE_IVF_FLAT);
  EXPECT_EQ(parameter.ivf_flat_parameter().dimension(), param.dimension);
  EXPECT_EQ(parameter.ivf_flat_parameter().metric_type(), MetricType2InternalMetricTypePB(param.metric_type));
  EXPECT_EQ(parameter.ivf_flat_parameter().ncentroids(), param.ncentroids);
}

TEST(VectorCommonTest, TestFillIvfPqParmeter) {
  pb::common::VectorIndexParameter parameter;
  IvfPqParam param{128, MetricType::kL2};

  FillIvfPqParmeter(&parameter, param);

  EXPECT_EQ(parameter.vector_index_type(), pb::common::VECTOR_INDEX_TYPE_IVF_PQ);
  EXPECT_EQ(parameter.ivf_pq_parameter().dimension(), param.dimension);
  EXPECT_EQ(parameter.ivf_pq_parameter().metric_type(), MetricType2InternalMetricTypePB(param.metric_type));
  EXPECT_EQ(parameter.ivf_pq_parameter().ncentroids(), param.ncentroids);
  EXPECT_EQ(parameter.ivf_pq_parameter().nsubvector(), param.nsubvector);
}

TEST(VectorCommonTest, TestFillHnswParmeter) {
  pb::common::VectorIndexParameter parameter;
  HnswParam param{128, MetricType::kL2, 200};

  FillHnswParmeter(&parameter, param);

  EXPECT_EQ(parameter.vector_index_type(), pb::common::VECTOR_INDEX_TYPE_HNSW);
  EXPECT_EQ(parameter.hnsw_parameter().dimension(), param.dimension);
  EXPECT_EQ(parameter.hnsw_parameter().metric_type(), MetricType2InternalMetricTypePB(param.metric_type));
  EXPECT_EQ(parameter.hnsw_parameter().efconstruction(), param.ef_construction);
  EXPECT_EQ(parameter.hnsw_parameter().nlinks(), param.nlinks);
  EXPECT_EQ(parameter.hnsw_parameter().max_elements(), param.max_elements);
}

TEST(VectorCommonTest, TestFillButeForceParmeter) {
  pb::common::VectorIndexParameter parameter;
  BruteForceParam param{128, MetricType::kL2};

  FillButeForceParmeter(&parameter, param);

  EXPECT_EQ(parameter.vector_index_type(), pb::common::VECTOR_INDEX_TYPE_BRUTEFORCE);
  EXPECT_EQ(parameter.bruteforce_parameter().dimension(), param.dimension);
  EXPECT_EQ(parameter.bruteforce_parameter().metric_type(), MetricType2InternalMetricTypePB(param.metric_type));
}

TEST(VectorCommonTest, TestFillRangePartitionRule) {
  pb::meta::PartitionRule partition_rule;
  std::vector<int64_t> seperator_ids = {10, 20, 30};
  std::vector<int64_t> index_and_part_ids = {1, 2, 3, 4, 5};

  FillRangePartitionRule(&partition_rule, seperator_ids, index_and_part_ids);

  EXPECT_EQ(partition_rule.partitions_size(), seperator_ids.size() + 1);

  for (int i = 0; i < partition_rule.partitions_size(); i++) {
    const auto& part = partition_rule.partitions(i);
    EXPECT_EQ(part.id().entity_id(), index_and_part_ids[i + 1]);
    EXPECT_EQ(part.id().parent_entity_id(), index_and_part_ids[0]);
  }

  for (int i = 0; i < partition_rule.partitions_size(); i++) {
    const auto& part = partition_rule.partitions(i);

    int64_t start_id = VectorCodec::DecodeVectorId(part.range().start_key());
    if (i == 0) {
      EXPECT_EQ(start_id, 0);
    } else {
      EXPECT_EQ(start_id, seperator_ids[i - 1]);
    }

    int64_t end_id = VectorCodec::DecodeVectorId(part.range().end_key());
    EXPECT_EQ(end_id, 0);
  }

  for (int i = 0; i < partition_rule.partitions_size(); i++) {
    const auto& part = partition_rule.partitions(i);
    int64_t start_key_part_id = VectorCodec::DecodePartitionId(part.range().start_key());
    int64_t end_key_part_id = VectorCodec::DecodePartitionId(part.range().end_key());
    EXPECT_EQ(start_key_part_id, index_and_part_ids[i + 1]);
    EXPECT_EQ(end_key_part_id, index_and_part_ids[i + 1] + 1);
  }
}

TEST(VectorCommonTest, EncodeDecodeVectorIndexCacheKey) {
  int64_t schema_id = 123;
  std::string index_name = "test_index";

  VectorIndexCacheKey key = GetVectorIndexCacheKey(schema_id, index_name);

  int64_t decoded_schema_id;
  std::string decoded_index_name;
  DecodeVectorIndexCacheKey(key, decoded_schema_id, decoded_index_name);

  EXPECT_EQ(decoded_schema_id, schema_id);
  EXPECT_EQ(decoded_index_name, index_name);
}

}  // namespace sdk
}  // namespace dingodb