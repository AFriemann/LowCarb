/**
 * @file   ConfigParser.hpp
 * @author see AUTHORS
 * @brief  ConfigParser header file.
 */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <iostream>
#include <functional>

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

// https://github.com/SergiusTheBest/plog
#include <plog/Log.h>
#define LOG_LEVEL_MIN 0
#define LOG_LEVEL_MAX 6

// local includes ==============================================================

#include "../utils/StringUtils.hpp"

//==============================================================================
// config paths
//==============================================================================

// General =====================================================================
#define ENSEMBLE_SIZE "General.ENSEMBLE_SIZE"
#define SEGMENT_SIZE "General.SEGMENT_SIZE"
#define TEMPERATURE "General.TEMPERATURE"
#define CRYSTAL_INFORMATION "General.CRYSTAL_INFORMATION"

#define DEFAULT_ENSEMBLE_SIZE 2000
#define DEFAULT_SEGMENT_SIZE 20
#define DEFAULT_CRYSTAL_INFORMATION false

// Files =======================================================================
#define PROTEIN "Files.PROTEIN"
#define TRAJECTORIES "Files.TRAJECTORIES"
#define NMA_COVARIANCE "Files.NMA_COVARIANCE"
#define SECONDARY_STRUCTURE "Files.SECONDARY_STRUCTURE"
#define REDUCTION "Files.REDUCTION"

// Fitting =====================================================================
#define AVERAGE_BIN_LENGTH "Fitting.AVERAGE_BIN_LENGTH"
#define MINIMUM_LENGTH "Fitting.MINIMUM_LENGTH"
#define MAXIMUM_LENGTH "Fitting.MAXIMUM_LENGTH"
#define USE_SLOW_FITTING "Fitting.USE_SLOW_FITTING"
#define SLOW_MINIMUM_LENGTH "Fitting.SLOW_MINIMUM_LENGTH"
#define SLOW_MAXIMUM_LENGTH "Fitting.SLOW_MAXIMUM_LENGTH"

#define DEFAULT_AVERAGE_BIN_LENGTH 1.0
#define DEFAULT_MINIMUM_LENGTH 6.0
#define DEFAULT_MAXIMUM_LENGTH 12.0
#define DEFAULT_USE_SLOW_FITTING true
#define DEFAULT_SLOW_MINIMUM_LENGTH 11.0
#define DEFAULT_SLOW_MAXIMUM_LENGTH 20.0

// Output ======================================================================
#define OUTPUT_HESSIAN_MATRIX "Output.HESSIAN_MATRIX"
#define OUTPUT_FORCE_CONSTANTS "Output.FORCE_CONSTANTS"
#define OUTPUT_MEAN_SQUARE_FLUCTUATION "Output.MEAN_SQUARE_FLUCTUATION"
#define OUTPUT_EIGENVALUES_AND_EIGENVECTORS "Output.EIGENVALUES_AND_EIGENVECTORS"
#define OUTPUT_AVG_FORCE_CONSTANTS "Output.AVG_FORCE_CONSTANTS"
#define OUTPUT_COVARIANCE_MATRIX "Output.COVARIANCE_MATRIX"

#define DEFAULT_OUTPUT_HESSIAN_MATRIX true
#define DEFAULT_OUTPUT_FORCE_CONSTANTS true
#define DEFAULT_OUTPUT_MEAN_SQUARE_FLUCTUATION true
#define DEFAULT_OUTPUT_EIGENVALUES_AND_EIGENVECTORS true
#define DEFAULT_OUTPUT_AVG_FORCE_CONSTANTS true
#define DEFAULT_COVARIANCE_MATRIX true

// Threading ===================================================================
#define THREADING_THREADS "Threading.THREADS"
#define THREADING_DYNAMIC "Threading.DYNAMIC"
#define THREADING_EIGEN_THREADS "Threading.EIGEN_THREADS"

#define DEFAULT_THREADING_THREADS 0
#define DEFAULT_THREADING_DYNAMIC true

// Logging =====================================================================

#define LOGGING_LEVEL "Logging.LEVEL"

#define DEFAULT_LOGGING_LEVEL 4

//==============================================================================
// Declarations
//==============================================================================

namespace fs = boost::filesystem;

struct Files {
    fs::path protein;
    std::vector<fs::path> trajectories;
    boost::optional<fs::path> secondary_structure;
    boost::optional<fs::path> nma_covariance;
    boost::optional<fs::path> reduction;
};

struct Fitting {
    double average_bin_length;
    double minimum_length;
    double maximum_length;
    bool   use_slow_fitting;
    double slow_minimum_length;
    double slow_maximum_length;
};

struct General {
    int    ensemble_size;
    int    segment_size;
    double temperature;
    bool   crystal_information;
    int    threads;
};

struct Output {
    bool force_constants;
    bool mean_square_fluctuation;
    bool eigenvalues_and_eigenvectors;
    bool hessian_matrix;
    bool average_force_constants;
    bool covariance_matrix;
};

struct Threading {
    int threads;
    boost::optional<int> eigen_threads;
    bool dynamic;
};

struct Logging {
    int level;
};

struct Config {
    General general;
    Fitting fitting;
    Files files;
    Output output;
    Threading threading;
    Logging logging;

    Config(General general, Fitting fitting, Files files, Output output, Threading threading, Logging logging)
        : general(general),
          fitting(fitting),
          files(files),
          output(output),
          threading(threading),
          logging(logging) {}
};

/**
 * @class ConfigParser
 */
class ConfigParser {
  public:
    ConfigParser() : pt() {}

    /**
     * @brief Parse config file.
     * @param path The path to a config file.
     * @return A Config struct.
     */
    Config parse(const fs::path & path);

  private:
    fs::path root;

    boost::property_tree::ptree pt;

    /**
     * @brief Parse output section.
     * @return A Output struct.
     */
    Output parse_output();

    /**
     * @brief Parse general section.
     * @return A General struct.
     */
    General parse_general();

    /**
     * @brief Parse fitting section.
     * @return A Fitting struct.
     */
    Fitting parse_fitting();

    /**
     * @brief Parse files section.
     * @return A Files struct.
     */
    Files parse_files();

    /**
     * @brief Parse threading section.
     * @return A Threading struct.
     */
    Threading parse_threading();

    /**
     * @brief Parse logging section
     * @return A Logging struct.
     */
    Logging parse_logging();

    /**
     * @brief checks if value1 is greater than value2.
     * @param name the config value name, used for error output.
     * @param against the value to check against.
     * @param value the value to be checked.
     * @return returns value if the condition is met.
     */
    template <typename T>
    T check_greater_equal(const std::string name, const T & against,
                          const T & value);

    /**
      * @brief checks if given path exists.
      *
      * Checks the given path for existence in relation to root if path is relative.
      *
      * @param path the relative or absolute path to the file
      * @return The absolute path to path.
     */
    fs::path absolute_existing(const fs::path & path);

    /**
      * @brief Checks if given path exists.
      *
      * Checks the given path for existence in relation to root if path is relative.
      *
      * @param path The relative or absolute path to the file
      * @return The absolute path as optional if path is initialized, else path.
     */
    boost::optional<fs::path> absolute_existing(const boost::optional<fs::path> & path);
};

template <typename T>
T ConfigParser::check_greater_equal(const std::string name, const T & against, const T & value) {
    if (! (value >= against)) {
        throw std::runtime_error("failed check for config value "+ name +": "
                                 + std::to_string(value) +" >= "+ std::to_string(against));
    }
    return value;
}

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
