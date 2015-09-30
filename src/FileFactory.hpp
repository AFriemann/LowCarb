/**
 * @file   FileFactory.hpp
 * @author see AUTHORS
 * @brief  FileFactory header file.
 */

#ifndef FILEFACTORY_HPP
#define FILEFACTORY_HPP

#include <string>
#include <memory>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include "SecondaryStructureFile.hpp"
#include "ProteinFile.hpp"
#include "TrajectoryFile.hpp"
#include "ReductionFile.hpp"
#include "DCD.hpp"
#include "XTC.hpp"
#include "TRR.hpp"

#include "utils/FileUtils.hpp"
//LCOV_EXCL_START

/**
 * @class FileFactory
 */
template <class T>
class FileFactory {
 public:
    virtual ~FileFactory();

    /**
     * @param path the file path.
     */
    virtual std::shared_ptr<T> create(const boost::filesystem::path & path) = 0;
};

template <class T>
FileFactory<T>::~FileFactory() {
}

/**
 * @class TrajectoryFileFactory
 */
class TrajectoryFileFactory : public FileFactory<TrajectoryFile> {
 public:
    /**
     * @param contains_crystal_information if the trajectory file contains crystal information.
     */
    std::shared_ptr<TrajectoryFile> create(const boost::filesystem::path & path,
                                           const bool contains_crystal_information);

    std::shared_ptr<TrajectoryFile> create(const boost::filesystem::path & path);
};

/**
 * @class ProteinFileFactory
 */
class ProteinFileFactory : public FileFactory<ProteinFile> {
 public:
    std::shared_ptr<ProteinFile> create(const boost::filesystem::path & path);
};

/**
 * @class SecondaryStructureFileFactory
 */
class SecondaryStructureFileFactory : public FileFactory<SecondaryStructureFile> {
 public:
     std::shared_ptr<SecondaryStructureFile> create(const boost::filesystem::path & path);
};

/**
 * @class ReductionFileFactory
 */
class ReductionFileFactory : public FileFactory<ReductionFile> {
 public:
     std::shared_ptr<ReductionFile> create(const boost::filesystem::path & path);

    /**
     * @param residuum_count the protein residue count.
     */
     std::shared_ptr<ReductionFile> create(const boost::filesystem::path & path,
                                           const size_t & residue_count);
};
//LCOV_EXCL_STOP
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
