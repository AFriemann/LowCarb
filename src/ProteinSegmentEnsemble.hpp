/**
 * @file   ProteinSegmentEnsemble.hpp
 * @author see AUTHORS
 * @brief  ProteinSegmentEnsemble header file.
 */

#ifndef PROTEINSEGMENTENSEMBLE_HPP
#define PROTEINSEGMENTENSEMBLE_HPP

#include <vector>

#include <Eigen/Dense>

#include "Averager.hpp"
#include "ProteinSegment.hpp"
#include "Frame.hpp"
#include "FrameSegment.hpp"

/**
 * @class ProteinSegmentEnsemble
 */
class ProteinSegmentEnsemble {
 public:
    static int add_frame_counter;

    /**
     * @param protein_segment
     * @sideeffect will change the protein segments upon creation.
     */
    explicit ProteinSegmentEnsemble(const std::shared_ptr<ProteinSegment> & protein_segment);

    ~ProteinSegmentEnsemble();

    /**
     * @param frame
     */
    void add_frame(const Frame & frame);

    /**
     * @param temparature
     */
    void compute_force_constant(double temperature);

 private:
    std::shared_ptr<ProteinSegment> protein_segment;
    Averager<Eigen::MatrixXd> covariance_averager;
    Averager<Eigen::VectorXd> displacement_vector_averager;
    Eigen::MatrixXd force_constant;
    FrameSegment frame_segment;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
