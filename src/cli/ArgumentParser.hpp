/**
 * @file   ArgumentParser.hpp
 * @author see AUTHORS
 * @brief  ArgumentParser header file.
 */

#ifndef ARGUMENTPARSER_HPP
#define ARGUMENTPARSER_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

// local includes ==============================================================


namespace fs = boost::filesystem;
namespace po = boost::program_options;

struct Paths {
    const boost::filesystem::path config;
    const boost::filesystem::path output;
};

struct Arguments {
    const Paths paths;
    const bool debug;
    const bool verbose;
};

/**
 * @class ArgumentParser
 */
class ArgumentParser {
  public:
      /**
       * @brief Parse arguments.
       * @param argc The number of arguments
       * @param argv The argument list.
       * @return An Argument struct.
       */
      Arguments parse(const int & argc, const char * argv[]);

  private:
      /**
       * @brief Parse config and output path.
       *
       * Will parse given paths and check for existence. Output directory will be created
       * if it does not exist.
       *
       * @param config_path Path to a config file.
       * @param output_path Path to a directory.
       * @return A Paths struct.
       */
      Paths parse_paths(const std::string & config_path,
                        const std::string & output_path);
                        // const std::string & reduction_path);
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
