/**
 * @file   FileUtils.cpp
 * @author see AUTHORS
 * @brief  FileUtils definitions file.
 */

#include "FileUtils.hpp"

std::string get_extension(const boost::filesystem::path & path) {
    std::string format = path.extension().string();
    boost::algorithm::to_lower(format);

    return format;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
