/**
 * @file   TRR.hpp
 * @author see AUTHORS
 * @brief  TRR header file.
 */

#ifndef LOW_CARB_TRR_H
#define LOW_CARB_TRR_H

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
#include <xdrfile_trr.h>

#include "TrajectoryFile.hpp"
#include "Frame.hpp"

/**
 * @class TRR
 * @brief used to read trajectories from .trr file
 */
class TRR : public TrajectoryFile {
public:
    /**
     * @param the path to the .trr file
     */
    explicit TRR(const std::string & path);

    ~TRR();

    /**
     * @return true if trajectory has a next frame
     */
    bool has_next();

    /**
     * @return the next frame of the trajectory
     */
    Frame get_next_frame();

    /**
     * @return the number of atoms in the protein
     */
    int get_atom_count() const;

    virtual TRAJECTORY_FILE_TYPE get_type() const;

private:
    int number_of_atoms, step;
    int return_code = exdrOK;
    float time, precision;
    rvec *atom_coordinates;
    XDRFILE * file;
    float *t;
    float *lambda;
    //extern int read_trr(XDRFILE *xd,int natoms,int *step,float *t,float *lambda,matrix box,rvec *x,rvec *v,rvec *f);

};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
