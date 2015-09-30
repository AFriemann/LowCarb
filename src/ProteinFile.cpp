/**
 * @file   ProteinFile.cpp
 * @author see AUTHORS
 * @brief  ProteinFile definition file.
 */

#include "ProteinFile.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::vector<Atom> ProteinFile::get_atoms() const {
    return this->atoms;
}

PDB::PDB(std::string const &path) {
    std::ifstream infile;
    std::string line;
    int atom_nr = 1;

    infile.open(path);
    while (std::getline(infile, line)) {
        if (line.substr(0, 4) == "ATOM") {
            //std:: cout << line << std::endl;
            //std::cout << "x: " << line.substr(31, 8) << " y: " << line.substr(39,8) << " z: " << line.substr(47, 8) << " number: " << line.substr(22,6) << std::endl;
            double x = stod(line.substr(31, 8));
            double y = stod(line.substr(39, 8));
            double z = stod(line.substr(47, 8));
            std::string long_type = line.substr(13, 4);
            unsigned res_nr = stoul(line.substr(22, 6));

            Atom atom(x, y, z, long_type, atom_nr++, res_nr);
            this->atoms.push_back(atom);
         }
    }
    infile.close();
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
