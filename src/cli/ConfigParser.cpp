/**
 * @file   ConfigParser.cpp
 * @author see AUTHORS
 * @brief  ConfigParser definitions file.
 */

#include "ConfigParser.hpp"

boost::optional<fs::path> ConfigParser::absolute_existing(const boost::optional<fs::path> & path) {
    if (path) {
        return boost::optional<fs::path>(this->absolute_existing(*path));
    }

    return path;
}

fs::path ConfigParser::absolute_existing(const fs::path & path) {
    fs::path absolute_path = fs::absolute(path, this->root);

    if (!exists(absolute_path)) {
        throw std::runtime_error("file does not exist: " + absolute_path.string());
    }

    return absolute_path;
}

General ConfigParser::parse_general() {
    General general;
    general.temperature = this->pt.get<double>(TEMPERATURE);

    general.ensemble_size = this->check_greater_equal("ENSEMBLE_SIZE", 0,
                                    this->pt.get<int>(ENSEMBLE_SIZE, DEFAULT_ENSEMBLE_SIZE));

    general.segment_size = this->check_greater_equal("SEGMENT_SIZE", 0,
                                    this->pt.get<int>(SEGMENT_SIZE, DEFAULT_SEGMENT_SIZE));

    general.crystal_information = this->pt.get<bool>(CRYSTAL_INFORMATION, DEFAULT_CRYSTAL_INFORMATION);
    return general;
}

Fitting ConfigParser::parse_fitting() {
    Fitting fitting;

    // read fitting options
    fitting.average_bin_length = this->check_greater_equal("AVERAGE_BIN_LENGTH", 0.0,
                                    this->pt.get<double>(AVERAGE_BIN_LENGTH, DEFAULT_AVERAGE_BIN_LENGTH));

    fitting.minimum_length = this->check_greater_equal("MINIMUM_LENGTH", 0.0,
                                this->pt.get<double>(MINIMUM_LENGTH, DEFAULT_MINIMUM_LENGTH));

    fitting.maximum_length = this->check_greater_equal("MAXIMUM_LENGTH", 0.0,
                                this->pt.get<double>(MAXIMUM_LENGTH, DEFAULT_MAXIMUM_LENGTH));

    // read slow fitting options
    fitting.use_slow_fitting = this->pt.get<bool>(USE_SLOW_FITTING, DEFAULT_USE_SLOW_FITTING);

    fitting.slow_minimum_length = this->check_greater_equal("SLOW_MINIMUM_LENGTH", 0.0,
                                    this->pt.get<double>(SLOW_MINIMUM_LENGTH, DEFAULT_SLOW_MINIMUM_LENGTH));

    fitting.slow_maximum_length = this->check_greater_equal("SLOW_MAXIMUM_LENGTH", 0.0,
                                    this->pt.get<double>(SLOW_MAXIMUM_LENGTH, DEFAULT_SLOW_MAXIMUM_LENGTH));
    return fitting;
}

Files ConfigParser::parse_files() {
    Files files;

    files.protein = this->absolute_existing(
                     this->pt.get<fs::path>(PROTEIN));
    LOGD << "using protein file: " << files.protein;

    files.secondary_structure = this->absolute_existing(
                                 this->pt.get_optional<fs::path>(SECONDARY_STRUCTURE));
    LOGD_IF(files.secondary_structure) << "using secondary structure file: " << files.secondary_structure;

    files.reduction = this->absolute_existing(
                       this->pt.get_optional<fs::path>(REDUCTION));
    LOGD_IF(files.reduction) << "using reduction file: " << files.reduction;

    if (files.nma_covariance = this->absolute_existing(
                                this->pt.get_optional<fs::path>(NMA_COVARIANCE))) {
        LOGD << "using nma covariance input: " << files.nma_covariance;
    } else {
        for (std::string t_path : split(this->pt.get<std::string>(TRAJECTORIES), ',')) {
            files.trajectories.push_back(this->absolute_existing(t_path));

            LOGD << "using trajectory file: " << t_path;
        }
    }
    return files;
}

Output ConfigParser::parse_output() {
    Output output;
    output.force_constants              = this->pt.get<bool>(OUTPUT_FORCE_CONSTANTS,
                                                             DEFAULT_OUTPUT_FORCE_CONSTANTS);

    output.mean_square_fluctuation      = this->pt.get<bool>(OUTPUT_MEAN_SQUARE_FLUCTUATION,
                                                             DEFAULT_OUTPUT_MEAN_SQUARE_FLUCTUATION);

    output.eigenvalues_and_eigenvectors = this->pt.get<bool>(OUTPUT_EIGENVALUES_AND_EIGENVECTORS,
                                                             DEFAULT_OUTPUT_EIGENVALUES_AND_EIGENVECTORS);

    output.average_force_constants      = this->pt.get<bool>(OUTPUT_AVG_FORCE_CONSTANTS,
                                                             DEFAULT_OUTPUT_AVG_FORCE_CONSTANTS);

    output.hessian_matrix               = this->pt.get<bool>(OUTPUT_HESSIAN_MATRIX,
                                                             DEFAULT_OUTPUT_HESSIAN_MATRIX);

    output.covariance_matrix            = this->pt.get<bool>(OUTPUT_COVARIANCE_MATRIX,
                                                             DEFAULT_COVARIANCE_MATRIX);

    return output;
}

Threading ConfigParser::parse_threading() {
    Threading threading;
    threading.threads = this->pt.get<int>(THREADING_THREADS, DEFAULT_THREADING_THREADS);

    threading.dynamic = this->pt.get<bool>(THREADING_DYNAMIC, DEFAULT_THREADING_DYNAMIC);

    threading.eigen_threads = this->pt.get_optional<int>(THREADING_EIGEN_THREADS);

    return threading;
}

Logging ConfigParser::parse_logging() {
    Logging logging;
    logging.level = this->pt.get<int>(LOGGING_LEVEL, DEFAULT_LOGGING_LEVEL);

    if (logging.level < LOG_LEVEL_MIN || logging.level > LOG_LEVEL_MAX) {
        throw std::runtime_error("invalid log level: " + logging.level);
    }
    return logging;
}

Config ConfigParser::parse(const fs::path & path) {
    boost::property_tree::ini_parser::read_ini(path.string(), this->pt);
    this->root = path.parent_path();
    return Config(this->parse_general(),
                  this->parse_fitting(),
                  this->parse_files(),
                  this->parse_output(),
                  this->parse_threading(),
                  this->parse_logging());
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
