/**
 * @file   DCD.cpp
 * @author see AUTHORS
 * @brief  DCD definitions file.
 */

#include "DCD.hpp"

DCD::DCD(std::string const & path, bool includes_crystal_information) : file(path, std::ios::in|std::ios::binary) {
    this->coordinate_char = new char[structure["coordinate"]];

    mlock(this->coordinate_char, sizeof(char) * 4);

    this->file.seekg(0, std::ios_base::end);
    this->file_length = this->file.tellg();
    this->file.seekg(0, std::ios_base::beg);
    read_header(path);
    this->includes_crystal_information = includes_crystal_information;
}

DCD::~DCD() {
    delete[] this->coordinate_char;
    munlock(this->coordinate_char, sizeof(char) * 4);
    this->file.close();
}

bool DCD::is_little_endian() {
    return this->little_endian;
}

void DCD::read_header(std::string const & path) {
    int i;
    int icntrl_counter = 0;
    int title_counter = 0;
    bool currently_in_header = true;

    this->header_length = 0;

    std::string current_header_part = "endian_indicator";

    if (this->file.is_open()) {
        while (currently_in_header) {
            this->header_length += structure[current_header_part];
            char * memory_block = new char[structure[current_header_part]];
            this->file.read(memory_block, structure[current_header_part]);
            if (current_header_part.compare("endian_indicator") == 0) {
                if (char_to_int(memory_block, this->little_endian) == 1409286144) {  // first 4 bytes
                    this->little_endian = false;
                } else {
                    this->little_endian = true;
                }

                current_header_part = "hdrr";
            } else if (current_header_part.compare("hdrr") == 0) {
                for (i = 0; i < structure[current_header_part]; ++i) {
                    this->HDR[i] = memory_block[i];
                }

                current_header_part = "icntrl";
            } else if (current_header_part.compare("icntrl") == 0) {
                this->ICNTRL[icntrl_counter++] =
                    char_to_int(memory_block, this->little_endian);
                if (icntrl_counter == 22) {
                    current_header_part = "ntitle";
                }
            } else if (current_header_part.compare("ntitle") == 0) {
                this->NTITLE = char_to_int(memory_block, this->little_endian);
                current_header_part = "title";
            } else if (current_header_part.compare("title") == 0) {
                for (i = 0; i < structure[current_header_part]; ++i) {
                    this->TITLE[title_counter * 80 + i] = memory_block[i];
                }
                if (++title_counter == this->NTITLE) {
                    current_header_part = "padding_before_natm";
                }
            } else if (
                current_header_part.compare("padding_before_natm") == 0) {
                current_header_part = "natm";
            } else if (current_header_part.compare("natm") == 0) {
                this->NATOM = char_to_int(memory_block, this->little_endian);
                currently_in_header = false;
            }

            delete[] memory_block;
        }
        this->file.seekg(structure["padding_before_coordinate_block"], std::ios_base::cur);

        this->NFRA = this->ICNTRL[0];
        if (this->ICNTRL[8] != 0) {
            this->NATOM -= this->ICNTRL[8];
        }
    } else {
        throw std::runtime_error("dcd file '" + path + "' not open");
    }
}

bool DCD::has_next() {
    return !((this->file.tellg()         >= this->file_length) ||
             (this->file.tellg()         <  0)                 ||
             (this->current_frame_number >= this->NFRA));
}

Frame DCD::get_next_frame() {
    this->current_frame_number++;
    int i;
    std::vector<double> x(this->NATOM);
    std::vector<double> y(this->NATOM);
    std::vector<double> z(this->NATOM);

        //////SKIP CRYSTAL INFORMATION
    if (this->includes_crystal_information){
        char * skip_crystal_memory_block = new char[structure["crystal_information"]];
         //padding?
        this->file.read(skip_crystal_memory_block, structure["crystal_information"]);
        delete skip_crystal_memory_block;
         /////END
    }

    for (i = 0; i < this->NATOM; ++i) {
        x[i] = read_next_single_coordinate();
    }

    this->file.seekg(structure["padding_before_coordinate_block"], std::ios_base::cur);

    for (i = 0; i < this->NATOM; ++i) {
        y[i] = read_next_single_coordinate();
    }

    this->file.seekg(structure["padding_before_coordinate_block"], std::ios_base::cur);

    for (i = 0; i < this->NATOM; ++i) {
        z[i] = read_next_single_coordinate();
    }

    this->file.seekg(structure["padding_before_coordinate_block"], std::ios_base::cur);

    Frame frame(x, y, z);
    return frame;
}

double DCD::read_next_single_coordinate() {
    this->file.read(coordinate_char, structure["coordinate"]);
    return char_to_double(coordinate_char, this->little_endian);
}

//LCOV_EXCL_START
int DCD::get_atom_count() const {
    return NATOM;
}

int DCD::get_frame_count() const {
    return NFRA;
}

TRAJECTORY_FILE_TYPE DCD::get_type() const {
    return DCD_TYPE;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
