/**
 * @file   FileFactory.cpp
 * @author see AUTHORS
 * @brief  FileFactory definitions file.
 */

#include "FileFactory.hpp"

std::shared_ptr<TrajectoryFile> TrajectoryFileFactory::create(
    const boost::filesystem::path & path, const bool contains_crystal_information) {

    std::string format = get_extension(path);
    if (format == ".dcd") {
        return std::shared_ptr<TrajectoryFile>(new DCD(path.string(), contains_crystal_information));
    }
    else if (format == ".xtc") {
        return std::shared_ptr<TrajectoryFile>(new XTC(path.string()));
    }
    else if (format == ".trr") {
        return std::shared_ptr<TrajectoryFile>(new TRR(path.string()));
    }

    throw std::runtime_error("unavailable format (" + format + ") for TrajectoryFile " + path.string());
}

std::shared_ptr<TrajectoryFile> TrajectoryFileFactory::create(
    const boost::filesystem::path & path) {

    return this->create(path, false);
}

std::shared_ptr<ProteinFile> ProteinFileFactory::create(
    const boost::filesystem::path & path) {

    std::string format = get_extension(path);

    if (format == ".pdb") {
        // ProteinFile pdb_file(PDB(tmp_path));
        return std::shared_ptr<ProteinFile>(new PDB(path.string()));
    } else {
        throw std::runtime_error("unavailable format (" + format + ") for ProteinFile " + path.string());
    }
}

std::shared_ptr<SecondaryStructureFile> SecondaryStructureFileFactory::create(
    const boost::filesystem::path & path) {

    std::string format = get_extension(path);

    if (format == ".dssp") {
        // SecondaryStructureFile dssp_file(DSSP(tmp_path));
        return std::shared_ptr<SecondaryStructureFile>(new DSSP(path.string()));
    }

    throw std::runtime_error("unavailable format (" + format + ") for SecondaryStructureFile " + path.string());
}


 std::shared_ptr<ReductionFile> ReductionFileFactory::create(
    const boost::filesystem::path & path, const size_t & residue_count) {

    return std::shared_ptr<ReductionFile>(new ReductionFile(path.string(), residue_count));
 }

 std::shared_ptr<ReductionFile> ReductionFileFactory::create(
    const boost::filesystem::path & path) {
    return this->create(path, 999999);
 }

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
