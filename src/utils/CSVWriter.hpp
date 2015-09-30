/**
 * @file   CSVReader.hpp
 * @author see AUTHORS
 * @brief  CSVReader header file.
 */

#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <Eigen/Dense>

class CSVWriter{
 public:
    /**
     * @param delimiter the CSV delimiter. By default , is chosen.
     * @param quote_char the CSV quote_char. By default " is chosen.
     */
    CSVWriter(const char delimiter = ',', const char quote_char = '"');

    /**
     * @brief destructor closes the file handle if it is still open.
     */
    ~CSVWriter();

    /**
     * @brief open the file handle to given path.
     * @param path the path to a file.
     */
    void open(const boost::filesystem::path & path);

    /**
     * @brief open the file handle to given path.
     * @param path the path to a file.
     */
    void open(const std::string & path);

    /**
     * @brief close the open file handle.
     */
    void close();

    /**
     * @param values a set of strings to be written to the opened file.
     */
    void write_line(const std::vector<std::string> & values);

    /**
     * @param values a set of doubles to be written to the opened file.
     */
    void write_line(const std::vector<double> & values);

    /**
     * @param values An Eigen MatrixXd object to be written to the opened file.
     * @param write_headers if headers should be written automatically.
     */
    void write(const Eigen::MatrixXd & values, const bool write_headers = true);

    /**
     * @param values An Eigen VectorXd object to be written to the opened file.
     * @param write_headers if headers should be written automatically.
     */
    void write(const Eigen::VectorXd & values, const bool write_headers = true);

 private:
    std::ofstream csv_file;

    char delimiter;
    char quote;

    /**
     * @brief check if the file handle is open and throw a runtime error if not.
     */
    void check_open();
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
