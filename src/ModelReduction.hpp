/**
 * @file   ModelReduction.hpp
 * @author see AUTHORS
 * @brief  ModelReduction header file.
 */

#ifndef REDUCTION_HPP
#define REDUCTION_HPP

#include <vector>
#include <memory>
#include <stdexcept>

#include <Eigen/Dense>
#include <plog/Log.h>

#include "ReductionFile.hpp"

/**
 * @class ModelReduction
 * @brief used to reduce matixes and vectors to a selection of values
 */
class ModelReduction {
public:
    ModelReduction();

    /**
     * @param reduction_file a shared_ptr to a ReductionFile object.
     */
    ModelReduction(const std::shared_ptr<ReductionFile> & reduction_file);

    /**
     * @brief reduce the given matrix with the stored ReductionFile.
     * @param hessian_matrix the hessian matrix to be reduced.
     * @return the reduced matrix.
     */
    Eigen::MatrixXd reduce_hessian_matrix(const Eigen::MatrixXd & hessian_matrix) const;

    /**
     * @return the selection size.
     */
    size_t get_selection_size() const;

    /**
     * @param i value index.
     * @return the selection value at index i.
     */
    int get_selection_at(int i) const;

private:
    std::shared_ptr<ReductionFile> reduction;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
