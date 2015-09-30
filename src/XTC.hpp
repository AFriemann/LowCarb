/**
 * @file   XTC.hpp
 * @author see AUTHORS
 * @brief  XTC header file.
 */

#ifndef XTC_HPP
#define XTC_HPP

#include <math.h>
#include <string.h>  // memcpy
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>
#include <map>

#include <xdrfile.h>
#include <xdrfile_xtc.h>

#include "TrajectoryFile.hpp"
#include "Frame.hpp"

/**
 * @class XTC
 * @brief used to read trajectories from .xtc file
 */
class XTC : public TrajectoryFile {
 public:

    /**
     * @param path to .xtc file.
     */
    explicit XTC(const std::string & path);

    /**
     * @brief closes the file handle.
     */
    ~XTC();

    /**
     * @return true if trajectory has a next frame
     */
    bool has_next();

    /**
     * @return the next frame
     */
    Frame get_next_frame();
    /**
     * @return the number of atoms in the protein
     */
    int get_atom_count() const;

    TRAJECTORY_FILE_TYPE get_type() const;

  private:
    int number_of_atoms, step;
    int return_code = exdrOK;
    float time, precision;
    rvec *atom_coordinates;
    XDRFILE * file;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
