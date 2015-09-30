/**
 * @file   main.hpp
 * @author see AUTHORS
 * @brief  Header file for LowCarb main.
 */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <exception>
#include <memory>
#include <stdexcept>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

#include <boost/filesystem.hpp>

// local includes ==============================================================

#include "FileFactory.hpp"
#include "ProteinFile.hpp"
#include "TrajectoryFile.hpp"
#include "SecondaryStructureFile.hpp"
#include "ReductionFile.hpp"
#include "Trajectory.hpp"
#include "Protein.hpp"
#include "ModelReduction.hpp"
#include "Reach.hpp"
#include "ProteinSegmentFactory.hpp"
#include "ProteinSegment.hpp"
#include "OutputWriter.hpp"

#include "cli/ArgumentParser.hpp"
#include "cli/ConfigParser.hpp"

#include "utils/CSVReader.hpp"

namespace {
  const size_t SUCCESS = 0;
  const size_t ERROR_IN_STARTUP = 1;
  const size_t ERROR_IN_RUNTIME = 2;
  const size_t ERROR_UNHANDLED_EXCEPTION = 3;
}

/*
 * @brief creates a protein object
 * @param protein_path
 * @param secondary_structure_path
 * @return a new Protein
 */
Protein load_protein(const boost::filesystem::path & protein_path,
                     const boost::optional<boost::filesystem::path> & secondary_structure_path);

/*
 * @brief sets up the reduction string that is used to reduce the hessian
 * matrix that is calculated by reach
 * @param reduction_config_path
 * @return the string indication how to ruduce the hessian
 */
ModelReduction load_reduction(const boost::optional<boost::filesystem::path> & reduction_config_path,
                           const size_t & residuum_count);

/*
 * @brief setup REACH
 * @param config
 * @return a fully configuered instance of REACH
 */
Reach setup_reach(const Config & config, const Protein & protein);

/*
 * @brief parses the parameters into arguments
 * @param argc
 * @param argv
 * @return the parameters as arguments object
 */
Arguments parse_args(const int argc, char * argv[]);

/*
 * @brief parses the config file
 * @param path the path to the config file
 * @return a config object
 */
Config parse_config(const std::string path);

/*
 * @brief initializes the logging depending on the specified loglevel
 * @param debug
 * @param log_level
 */
void setup_logging(const bool debug, const bool verbose, const int log_level);

/*
 * @brief initialize multithreaded execution
 * @param threads
 * @param dynamic
 */
void setup_threading(const int threads, const boost::optional<int> eigen_threads, const bool dynamic);

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
