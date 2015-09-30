/**
 * @file   TrajectoryFile.hpp
 * @author see AUTHORS
 * @brief  TrajectoryFile header file.
 */

#ifndef TRAJECTORYFILE_HPP
#define TRAJECTORYFILE_HPP

#include "Frame.hpp"

enum TRAJECTORY_FILE_TYPE {
    DCD_TYPE,
    XTC_TYPE,
    TRR_TYPE
};

//LCOV_EXCL_START
/**
 * @class TrajectoryFile
 * @brief base class for classes that read Trajectories from files
 */
class TrajectoryFile {
  public:
    // has to be virtual i.o. to call child destructor
    virtual ~TrajectoryFile() {}

    /**
     * @return true if trajectory has a next frame
     */
    virtual bool has_next() = 0;

    /**
     * @return the next frame
     */
    virtual Frame get_next_frame() = 0;

    /**
     * @return the number of atoms in the protein
     */
    virtual int get_atom_count() const = 0;

    /**
     * @brief only necessary for tests
     */
    virtual TRAJECTORY_FILE_TYPE get_type() const = 0;
};
//LCOV_EXCL_STOP

#endif
// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
