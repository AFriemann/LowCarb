/**
 * @file   CSVReader.cpp
 * @author see AUTHORS
 * @brief  CSVReader definitions file.
 */

#include "CSVReader.hpp"

Eigen::MatrixXd CSVReader::read_matrix(
    const boost::filesystem::path & matrix_csv_path,
    const int dimension)
    const {

    if (!exists(matrix_csv_path)) {
        throw std::runtime_error("file does not exist: " + matrix_csv_path.string());
    }

    std::ifstream matrix_file;
    matrix_file.open(matrix_csv_path.string());

    std::string row;
    size_t columns = 0;
    size_t rows = 0;

    Eigen::MatrixXd matrix(dimension, dimension);

    while (std::getline(matrix_file, row)){
        columns = 0;
        size_t begin = 0;
        size_t delimiter_position;
        std::string entry;

        do {
            delimiter_position = row.find(',', begin);
            entry = row.substr(begin, delimiter_position - begin);
            matrix(rows, columns) = std::atof(entry.c_str());
            begin = delimiter_position + 1;
            columns++;
        } while (delimiter_position != std::string::npos);
        rows++;
    }

    return matrix;
}

std::vector<Eigen::Vector3d> CSVReader::read_as_vector(
    const boost::filesystem::path &matrix_csv_path)
    const {

    if (!exists(matrix_csv_path)) {
        throw std::runtime_error("file does not exist: " + matrix_csv_path.string());
    }

    std::ifstream file;
    file.open(matrix_csv_path.string());
    std::string row;
    size_t columns = 0;
    size_t rows = 0;

    std::vector<Eigen::Vector3d> vector;

    while (std::getline(file, row)){
        size_t begin = 0;
        size_t delimiter_position;
        std::string x, y, z;
        delimiter_position = row.find(',', begin);
        x = row.substr(begin, delimiter_position - begin);
        begin = delimiter_position + 1;
        delimiter_position = row.find(',', begin);
        y = row.substr(begin, delimiter_position - begin);
        begin = delimiter_position + 1;
        delimiter_position = row.find(',', begin);
        z = row.substr(begin, delimiter_position - begin);

        Eigen::Vector3d coordinate  = Eigen::Vector3d(std::stod(x), std::stod(y), std::stod(z));
        vector.push_back(coordinate);
    }
    return vector;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
