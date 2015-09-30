/**
 * @file   OutputWriter.hpp
 * @author see AUTHORS
 * @brief  OutputWriter header file.
 */

#ifndef OUTPUTWRITER_HPP
#define OUTPUTWRITER_HPP

#include <cassert>
#include <utility>
#include <vector>
#include <map>

#include <plog/Log.h>
#include <Eigen/Dense>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

// local includes ==============================================================

#include "ProteinSegment.hpp"
#include "utils/CSVWriter.hpp"

/**
 * @class OutputWriter
 */
class OutputWriter {
public:
    /**
     * @param path the output directory path.
     */
    OutputWriter(const boost::filesystem::path & path) : path(path) {}

    /**
     * @brief write protein segment force constants.
     * @param protein_segments a vector of shared pointes to ProteinSegment objects.
     */
    void write_force_constants(const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments);

    /**
     * @brief write mean square fluctuation and vector and xx_com vector.
     * @param mean_square_fluctuation An Eigen Vector containing mean square fluctuation data.
     * @param xx_com An Eigen Vector containing nma data.
     */
    void write_mean_square_fluctuation(const Eigen::VectorXd & mean_square_fluctuation,
                                       const Eigen::VectorXd & xx_com);

    /**
     * @brief write eigenvectors to file.
     * @param eigenvectors An Eigen Matrix containing eigenvectors.
     */
    void write_eigenvectors(const Eigen::MatrixXd & eigenvectors);

    /**
     * @brief write eigenvalues to file.
     * @param eigenvalues An Eigen Vector containing eigenvalues.
     */
    void write_eigenvalues(const Eigen::VectorXd & eigenvalues);

    /**
     * @brief write vectors eig and vec to file.
     * @param eig An Eigen vector.
     * @param vec An Eigen vector.
     */
    void write_eig_and_vec(const Eigen::VectorXd & eig, const Eigen::VectorXd & vec);

    /**
     * @brief write averaged force constants, ranges and erros to file.
     * @param averaged_force_constants A map of string and double values.
     */
    void write_averaged_force_constants(const std::map<std::string, double> & averaged_force_constants);

    /**
     * @brief write average force constants, ranges and errors for complete protein to file.
     * @param averaged_force_constants A map of string and double values.
     */
    void write_average_force_constants_for_complete_protein(
            const std::map<std::string, Eigen::VectorXd> & averaged_force_constants);

    /**
     * @brief write hessian matrix to file.
     * @param hessian_matrix An eigen Matrix.
     */
    void write_hessian_matrix(const Eigen::MatrixXd & hessian_matrix);

    /**
     * @brief write covariance matrix to file.
     * @param hessian_matrix An eigen Matrix.
     */
    void write_covariance_matrix(const Eigen::MatrixXd & covariance_matrix);

private:
    boost::filesystem::path path;
};

#endif
