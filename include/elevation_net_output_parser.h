// Copyright (c) 2022，Horizon Robotics.
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

#ifndef DNN_ELEVATION_NET_OUTPUT_PARSER_H
#define DNN_ELEVATION_NET_OUTPUT_PARSER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "dnn/hb_dnn_ext.h"

#include "dnn_node/dnn_node.h"

using hobot::dnn_node::DNNTensor;
using hobot::dnn_node::Model;

/**
 * \~Chinese @brief 单精度浮点数组，可用于存储特征值、质量结果等
 */
struct FloatArray {
  std::vector<float> values;
  /// \~Chinese 置信度
  float score = 0.0;
};

class ElevationNetResult {
 public:
  FloatArray depth_result;
  FloatArray height_result;
};

class ElevationNetOutputParser {
 public:
  int32_t Parse(
      std::shared_ptr<ElevationNetResult> &output,
      std::shared_ptr<DNNTensor> &output_tensor);

  int32_t PostProcess(std::shared_ptr<DNNTensor> &output_tensor,
                      std::shared_ptr<ElevationNetResult> &det_result);

 private:
  struct CameraParameter {
    float fx_inv_ = 0.0006045482782860858f, fy_inv_ = 0.0006044447633990631f;
    float cx_inv_ = -0.6043894145811973f, cy_inv_ = -0.3181318317406348f;
    float nx_ = 0.f, ny_ = 0.f, nz_ = 1.f;
    float cam_H_ = 1.f;
  };
  CameraParameter desc_;
  std::vector<float> points_;
  int model_output_height_, model_output_width_;

  void GetFrameOutPut_NEON(uint32_t shift, uint32_t src_w_stride,
                          void *depth_ptr, void *height_ptr, void *gamma_ptr);

  void GetFrameOutPut(uint32_t shift, uint32_t src_w_stride, void *depth_ptr,
                     void *height_ptr, void *gamma_ptr);
  void GenerateFeaturePoints(int width, int height);
};

#endif  // DNN_ELEVATION_NET_OUTPUT_PARSER_H
