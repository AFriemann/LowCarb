/**
 * @file   CSVWriter.cpp
 * @author see AUTHORS
 * @brief  CSVWriter definitions file.
 */

#include "CSVWriter.hpp"

CSVWriter::CSVWriter(const char delimiter, const char quote_char)
    : delimiter(delimiter), quote(quote_char), csv_file() {}


CSVWriter::~CSVWriter() {
    if (this->csv_file.is_open()) {
        this->csv_file.close();
    }
}

void CSVWriter::open(const boost::filesystem::path & path) {
    this->csv_file.open(path.string());
    if (!this->csv_file.is_open()) {
        throw std::runtime_error("File does not exist: " + path.string());
    }
}

void CSVWriter::open(const std::string & path) {
    this->open(boost::filesystem::path(path));
}

void CSVWriter::close() {
    this->csv_file.close();
}

void CSVWriter::check_open() {
    if (!this->csv_file.is_open()) {
        throw std::runtime_error("attempted to write to closed file handle");
    }
}

void CSVWriter::write_line(const std::vector<std::string> & values) {
    this->check_open();

    int counter = 0;
    for (auto const & value : values) {
        this->csv_file << this->quote << value << this->quote;
        if (counter < values.size() - 1) {
            this->csv_file << this->delimiter;
            ++counter;
        }
    }

    this->csv_file << std::endl;
}

void CSVWriter::write_line(const std::vector<double> & values) {
    this->check_open();

    int counter = 0;
    for (auto const & value : values) {
        this->csv_file << value;
        if (counter < values.size() - 1) {
            this->csv_file << this->delimiter;
            ++counter;
        }
    }

    this->csv_file << std::endl;
}

void CSVWriter::write(const Eigen::MatrixXd & matrix, const bool write_headers) {
    if (write_headers) {
        std::vector<std::string> header;
        for (int i = 0; i < matrix.cols(); ++i){
            header.push_back("x"+std::to_string(i+1));
        }

        this->write_line(header);
    }

    for (int row_number = 0; row_number < matrix.rows(); ++row_number){
        std::vector<double> values;

        for (int col_number = 0; col_number < matrix.cols(); ++col_number){
            values.push_back(matrix(row_number, col_number));
        }

        this->write_line(values);
    }
}

void CSVWriter::write(const Eigen::VectorXd & vector, const bool write_headers) {
    if (write_headers) {
        this->write_line({ "row1" });
    }

    for (int row_number = 0; row_number < vector.rows(); ++row_number) {
        std::vector<double> row = { vector(row_number) };
        this->write_line(row);
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
