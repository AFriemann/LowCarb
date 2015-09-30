/**
 * @file   CSVReader.hpp
 * @author see AUTHORS
 * @brief  CSVReader header file.
 */

#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <Eigen/Dense>

class CSVReader{
    public:
        /**
         * @brief reads a matrix from a .csv file
         * @param csv_path
         */
        Eigen::MatrixXd read_matrix(const boost::filesystem::path & matrix_csv_path,
                                    const int dimension) const;

        /**
        * @brief reads a vector of 3d vectors from a .csv file
        * @param csv_path
        */
        std::vector<Eigen::Vector3d> read_as_vector(const boost::filesystem::path &matrix_csv_path) const;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
