/**
 * @file   ProteinFile.hpp
 * @author see AUTHORS
 * @brief  ProteinFile header file.
 */

#ifndef PROTEINFILE_HPP
#define PROTEINFILE_HPP

#include <string>
#include <vector>

#include "Atom.hpp"

/**
 * @class ProteinFile
 */
class ProteinFile {
public:
    /**
     * @return the atoms
     */
    std::vector<Atom> get_atoms() const;

protected:
    ProteinFile() {}

    std::vector<Atom> atoms;
};

class PDB : public ProteinFile {
 public:
    /**
     * @param path
     */
    explicit PDB(const std::string & path);
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
