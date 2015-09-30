/**
 * @file   NormalModeMeanSquareFluctuationCalculator.hpp
 * @author see AUTHORS
 * @brief  NormalModeMeanSquareFluctuationCalculator header file.
 */

#ifndef NORMALMODEMEANSQUAREFLUCTUATION_HPP
#define NORMALMODEMEANSQUAREFLUCTUATION_HPP

#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

#include <plog/Log.h>

#include "Residuum.hpp"
#include "Protein.hpp"
#include "ProteinSegment.hpp"
#include "StructureType.hpp"
#include "ForceConstantSelector.hpp"
#include "ModelReduction.hpp"

#define NMODE 1

/**
 * @class NormalModeMeanSquareFluctuationCalculator
 * @brief used for calculation of NMA mean square fluctuation
 */
class NormalModeMeanSquareFluctuationCalculator {
    public:
        /**
         * @param residues
         */
        NormalModeMeanSquareFluctuationCalculator(const std::vector<Residuum> & residues);

        Eigen::VectorXd get_mean_square_fluctuation() const;
        Eigen::VectorXd get_weighted_eigenvalues() const;
        Eigen::VectorXd get_weighted_eigenvector() const;
        Eigen::VectorXd get_eigenvalues() const;

        /**
         * @return the eigenvectors
         */
        Eigen::MatrixXd get_eigenvectors() const;

        /**
         * @return the hessian matrix
         */
        Eigen::MatrixXd get_hessian_matrix() const;

        /**
         * @param protein
         * @param temperature
         * @param force_constant_selector
         * @param protein_segment
         */
        void calculate(const Protein & protein,
                       const double & temperature,
                       const ForceConstantSelector & force_constant_selector,
                       const std::shared_ptr<ProteinSegment> & protein_segment,
                       const ModelReduction & model_reduction);

        /**
         * @param protein
         * @param temperature
         * @param force_constant_selector
         * @param complete_protein_segment
         */
        void calculate(const Protein & protein,
                       const double & temperature,
                       const ForceConstantSelector & force_constant_selector,
                       const std::shared_ptr<ProteinSegment> &protein_segment);

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    protected:
        /**
         * @brief reduces the model according to model reduction input
         * @aram model_reduction
         */
        void calculate_model_reduction(const ModelReduction & model_reduction);

        /**
         * @brief calculates the eigenvalues with the magic number 53.0516 which was also
         * used in the original REACH programm for unknown reasons
         */
        void calculate_weighted_eigenvalues();

        /**
         * @brief calculates the weighted eigenvector depending on NMODE and the mass
         */
        void calculate_weighted_eigenvector();

        /**
         * @brief calculates the mean square fluctuation
         */
        void calculate_mean_square_fluctuation(const double &temperature);

        /*
         * @brief calculate the hessian matrix for the protein segment
         */
        void calculate_hessian_matrix(const Protein & protein,
                                      const ForceConstantSelector & kk_selector,
                                      const std::shared_ptr<ProteinSegment> & protein_segment);

        /**
         * @brief calculate both, the eigenvalues and the eigenvectors
         */
        void calculate_eigenvalues_and_eigenvectors();

        bool done;
        size_t residue_count;

        Eigen::VectorXd mass;
        Eigen::MatrixXd hessian_matrix;
        Eigen::VectorXd mean_square_fluctuation;
        Eigen::VectorXd weighted_eigenvalues;
        Eigen::VectorXd weighted_eigenvector;
        Eigen::VectorXd eigenvalues;
        Eigen::MatrixXd eigenvectors;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
