/**
 * @file   ReductionFile.hpp
 * @author see AUTHORS
 * @brief  ReductionFile header file.
 */

#ifndef REDUCTIONFILE_HPP
#define REDUCTIONFILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <sstream>

#include <plog/Log.h>

#include "utils/StringUtils.hpp"

/**
 * @class ReductionFile
 * @brief file used for reducing
 */
class ReductionFile {
  public:
    ReductionFile();

    /**
     * @param path the path to the reduction file.
     * @param residuum_count maximum number of residuums
     */
    ReductionFile(const std::string & path, const size_t & residuum_count);

    /**
     * @return std::vector of integers for residues requested for reduction.
     */
    std::vector<int> get_selection() const;

  private:
    std::vector<int> selection;

    /**
     * @param path the path to a reduction file.
     * @return the file content as std::string.
     */
    std::string get_selection_string_from_file(const std::string & path);

    /**
     * @brief parse the reductions string and produce the required reduction vector.
     * @param input the reduction string.
     * @param max_num maximum number of residues.
     * @return std::vector of integers for residues requested for reduction.
     */
    std::vector<int> parse_selection_input(const std::string & input, const size_t & max_num);
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
