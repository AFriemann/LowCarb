/**
 * @file   Trajectory.cpp
 * @author see AUTHORS
 * @brief  Trajectory definitions file.
 */

#ifndef TRAJECTORY_CPP
#define TRAJECTORY_CPP

#include "Trajectory.hpp"

Trajectory::Trajectory() : files() {} //LCOV_EXCL_LINE

Trajectory::Trajectory(const std::vector<std::shared_ptr<TrajectoryFile>> & files) : files(files) {}

bool Trajectory::has_next() {
    return (this->files[this->current_file_position]->has_next() ||
            this->current_file_position < this->files.size() - 1);
}

Frame Trajectory::get_next_frame() {
    if (this->files[this->current_file_position]->has_next()) {
        this->current_position_in_file += 1;
    } else {
        this->current_file_position += 1;
        this->current_position_in_file = 0;
    }

    return this->files[this->current_file_position]->get_next_frame();
}

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
