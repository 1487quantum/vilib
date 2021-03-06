/*
 * Blender functionalities and frame storage
 * blender.h
 *
 * Copyright (C) 2019-2020 Balazs Nagy,
 * Robotics and Perception Group, University of Zurich
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <assert.h>
#include <Eigen/Dense>

class Blender {
public:
  enum class Scenery {
    /* simple sequence for Sparse Image Alignment, and Feature Alignment */
    Hut=0,
    /* longer sequence for Feature Tracking */
    HutLong,
    /* simple sequence for Sparse Stereo matching */
    HutStereo
  };
  struct BlenderFrame {
    std::string image_2d_path_;
    std::string image_depth_path_;
    Eigen::Matrix4d T_W_C_;
    Eigen::Matrix4d T_C_W_;
  };

  Blender(Scenery scenery);
  ~Blender(void) = default;

  inline const struct BlenderFrame & getFrame(std::size_t idx) const {
    assert(frames_.size() > idx);
    return frames_[idx];
  }

  inline std::size_t getFrameCount(void) const {
    return frames_.size();
  }

  inline const Eigen::Vector4d & getIntrinsicParameters(void) const {
    return camera_intr_;
  }

  inline const Eigen::Matrix4d & get_T_B_C(void) const {
    return T_B_C_;
  }

  inline const Eigen::Matrix4d & get_T_C_B(void) const {
    return T_C_B_;
  }

  inline const Eigen::MatrixXd get_M(void) const {
    /*
     * Note to future self:
     * M = K * P
     */
    Eigen::MatrixXd P(3,4);
    Eigen::Matrix3d K;
    P << 1,0,0,0,
         0,1,0,0,
         0,0,1,0;
    K << camera_intr_(0), 0.0, camera_intr_(2),
         0.0, camera_intr_(1), camera_intr_(3),
         0.0, 0.0, 1.0;
    return (K*P);
  }

private:
  std::vector<struct BlenderFrame,
              Eigen::aligned_allocator<struct BlenderFrame>> frames_;
  Eigen::Vector4d camera_intr_;
  Eigen::Matrix4d T_B_C_;
  Eigen::Matrix4d T_C_B_;
};
