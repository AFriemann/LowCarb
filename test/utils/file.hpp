/* file.hpp
 * -*- coding: utf-8 -*-
 *
 *
 */

#ifndef FILE_HPP
#define FILE_HPP

// system includes =============================================================

#include <string>
#include <vector>
#include <sstream>
#include <glob.h>

#include <csv.h>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/Frame.hpp"
#include "../src/SecondaryStructureFile.hpp"
#include "../src/NormalModeMeanSquareFluctuationCalculator.hpp"

#include "string.hpp"
#include "type.hpp"

template <typename T>
std::vector<T> get_std_vector_from_csv(const std::string & csv_path,
                                       const std::string & column_name)
{
    io::CSVReader<1> csv_file(csv_path);
    csv_file.read_header(io::ignore_extra_column, column_name);
    std::vector<T> result;
    T value;

    while (csv_file.read_row(value)) {
        result.push_back(value);
    }
    return result;
}

Eigen::MatrixXd get_matrix_from_csv(std::string const & csv_filename);

Frame get_frame_from_csv(const std::string & csv_filename);

Frame get_frame_from_csv_not_hex(const std::string & csv_filename);

std::vector<Range> get_ranges_from_csv(const std::string & csv_path);

Eigen::VectorXd get_vector_from_csv(const std::string & csv_path, const std::string & column_name);

Eigen::MatrixXd get_sym_matrix_from_file(const std::string & matrix_file_path,
                                         int dim, char delim = ',', bool is_hex = false);

NormalModeMeanSquareFluctuationCalculator get_nmmsfc_for_testing(const std::string& pdb_path,
                                                                 const std::string& dssp_path,
                                                                 const std::string& test_prefix);

std::vector<std::string> glob(const std::string& pat);

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
