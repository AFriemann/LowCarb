/**
 * @file   FileUtils.hpp
 * @author see AUTHORS
 * @brief  FileUtils header file.
 */

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

std::string get_extension(const boost::filesystem::path & path);

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
