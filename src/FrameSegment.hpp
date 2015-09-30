/* Copyright 2015 see AUTHORS
 *
 *
 */

#ifndef FRAMESEGMENT_HPP
#define FRAMESEGMENT_HPP

#include <vector>
#include <memory>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Frame.hpp"
#include "AtomGroup.hpp"
#include "ProteinSegment.hpp"

/**
 * @class FrameSegment
 * @brief a frame segment is a list of atoms from a frame
 */
class FrameSegment : public AtomGroup {
public:
    /**
     * @param protein_segment
     */
    FrameSegment(const std::shared_ptr<ProteinSegment> & protein_segment);

    /**
     * @param frame
     */
    void set_frame(const Frame & frame);

    /**
     * @return displacement vector
     */
    Eigen::VectorXd fit_to_reference();

private:
    std::shared_ptr<ProteinSegment> protein_segment;
};

#endif
