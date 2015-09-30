/**
 * @file   ArgumentParser.cpp
 * @author see AUTHORS
 * @brief  ArgumentParser definitions file.
 */

#include "ArgumentParser.hpp"

// parse paths =================================================================

Paths ArgumentParser::parse_paths(const std::string & config_path,
                                  const std::string & output_path)
                                  // const std::string & reduction_path)
{

    fs::path fs_config_path(config_path);
    fs::path fs_output_path(output_path);
    // fs::path fs_reduction_path(reduction_path);

    if (!fs::exists(fs_config_path)) {
        throw po::error("config file does not exist: " + fs_config_path.string());
    }

    if (!fs::exists(fs_output_path)) {
        fs::create_directories(fs_output_path);
    } else if (!fs::is_directory(fs_output_path)) {
        throw po::error("output path exists but is not a directory: " + fs_output_path.string());
    }

    // if (fs_reduction_path != "" && !fs::exists(fs_reduction_path)) {
    //     throw std::runtime_error("reduction file does not exist: " + fs_reduction_path.string());
    // }
    //
    // return {fs_config_path, fs_output_path, fs_reduction_path};
    return {fs_config_path, fs_output_path};
}

//==============================================================================

Arguments ArgumentParser::parse(const int & argc, const char * argv[]){
    po::variables_map variables_map;

    // buffers
    std::string config_path;
    std::string output_path;

    // Options
    auto config_path_option = new po::typed_value<std::string>(&config_path);
    auto output_path_option = new po::typed_value<std::string>(&output_path);

    // further option configuration
    config_path_option->value_name("PATH")->required();
    output_path_option->value_name("PATH")->required();

    // help description
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h",                             "produce help message")
        ("config,c",    config_path_option,    "config file path")
        ("output,o",    output_path_option,    "output file path")
        ("debug,d",                            "print debug output")
        ("verbose,v",                          "print verbose output");

    // can throw
    po::store(po::parse_command_line(argc, argv, desc), variables_map);

    // --help option
    if (variables_map.count("help")) {
        std::cout << argv[0] << std::endl << desc << std::endl;
        std::exit(0);
    }

    // throws on error, so do after help in case there are any problems
    po::notify(variables_map);

    // parse paths and other options
    // Paths paths = this->parse_paths(config_path, output_path, reduction_path);
    Paths paths = this->parse_paths(config_path, output_path);

    return {paths, variables_map.count("debug") > 0,
                   variables_map.count("verbose") > 0};
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
