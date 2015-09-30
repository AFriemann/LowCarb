/**
 * @file   Trajectory.hpp
 * @author see AUTHORS
 * @brief  Trajectory header file.
 */

#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include <iterator>
#include <string>
#include <vector>
#include <memory>

#include <plog/Log.h>

#include "TrajectoryFile.hpp"
#include "Frame.hpp"

/**
 * @class Trajectory
 */
class Trajectory {
 public:
    Trajectory();

    /**
     * @param files
     */
    Trajectory(const std::vector<std::shared_ptr<TrajectoryFile>> & files);

    /**
     * @brief checks wether the trajectory has another frame after the current file position.
     */
    bool has_next();

    /**
     * @return the next frame of the trajectory
     */
    Frame get_next_frame();

 private:
    std::vector<std::shared_ptr<TrajectoryFile>> files;
    int current_file_position = 0;
    int current_position_in_file = 0;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
