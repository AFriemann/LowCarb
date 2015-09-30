/**
 * @file   TRR.cpp
 * @author see AUTHORS
 * @brief  TRR definitions file.
 */

#include "XTC.hpp"

XTC::XTC(std::string const & path) {
    const char * path_char_const = path.c_str();
    char * path_char = new char[path.length()+1];
    strcpy(path_char,path_char_const);
    return_code = read_xtc_natoms(path_char, &this->number_of_atoms);
    this->file = xdrfile_open(path_char_const, "r");
    if (this->file == NULL){
        throw new std::runtime_error("the xtc file was not opened correctly");
    }
}

XTC::~XTC(){
    xdrfile_close(this->file);
}

Frame XTC::get_next_frame() {
    matrix frameMatrix;
    int i;
    atom_coordinates = new rvec[this->number_of_atoms];
    std::vector<double> x_coordinate(this->number_of_atoms), y_coordinate(this->number_of_atoms), z_coordinate(this->number_of_atoms);
    return_code = read_xtc(this->file, this->number_of_atoms, &step, &time, frameMatrix, atom_coordinates, &precision);
    if (return_code == exdrOK) {
        for (i = 0; i < this->number_of_atoms; i++) {
            x_coordinate[i] = (double) atom_coordinates[i][0];
            y_coordinate[i] = (double) atom_coordinates[i][1];
            z_coordinate[i] = (double) atom_coordinates[i][2];
        }
    } else {
        throw new std::runtime_error("the library for reading xtc had return code " + return_code);
    }
    Frame frame (x_coordinate,y_coordinate,z_coordinate);
    return frame;
}

bool XTC::has_next(){
    return this->return_code != exdrENDOFFILE;
}

int XTC::get_atom_count() const {
    return this->number_of_atoms;
}


TRAJECTORY_FILE_TYPE XTC::get_type() const {
    return XTC_TYPE;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
