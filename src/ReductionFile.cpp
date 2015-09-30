/**
 * @file   ReductionFile.cpp
 * @author see AUTHORS
 * @brief  ReductionFile definitions file.
 */

#include "ReductionFile.hpp"

ReductionFile::ReductionFile() : selection() {/* no reduction file */}

ReductionFile::ReductionFile(const std::string & path, const size_t & residuum_count) : selection() {
    std::string input_string = this->get_selection_string_from_file(path);
    this->selection = this->parse_selection_input(input_string,
                                                  residuum_count);
}

std::string ReductionFile::get_selection_string_from_file(const std::string & path) {
    std::string result = "";
    std::ifstream file(path);

    if (file.is_open() == false)
        return result;

    std::getline(file, result);

    return result;
}

std::vector<int> ReductionFile::parse_selection_input(const std::string & input, const size_t & max_num) {
    LOGD << "parsing selection input";

    std::vector<int> result;

    std::string parse_string = replace(input, "end", std::to_string(max_num));

    for (std::string & reduction : split(parse_string, ',')) {
        std::vector<std::string> reduction_tuple = split(reduction, '-');

        try {
            if (reduction_tuple.size() == 1) {
                result.push_back(std::stoi(reduction_tuple.at(0)));
            } else if (reduction_tuple.size() == 2) {
                int first = std::stoi(reduction_tuple.at(0));
                int second = std::stoi(reduction_tuple.at(1));

                for (size_t i=first; i<=second; ++i) {
                    result.push_back(i);
                }
            } else {
                throw std::invalid_argument(reduction);
            }
        } catch(std::invalid_argument & e) {
            throw std::runtime_error("failed to parse reduction file contents: " + *e.what());
        }
    }

    // sort result and delete multiple entries
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

//LCOV_EXCL_START
std::vector<int> ReductionFile::get_selection() const {
    return this->selection;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
