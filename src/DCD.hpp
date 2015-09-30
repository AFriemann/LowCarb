/**
 * @file   DCD.hpp
 * @author see AUTHORS
 * @brief  DCD header file.
 */

#ifndef DCD_HPP
#define DCD_HPP

#include <sys/mman.h>
#include <math.h>
#include <string.h>  // memcpy
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>
#include <map>

#include "TrajectoryFile.hpp"
#include "Frame.hpp"

#include "utils/TypeUtils.hpp"

/**
 * @class DCD
 */
class DCD : public TrajectoryFile {
 public:
    bool has_next();
    Frame get_next_frame();
    int get_atom_count() const;
    int get_frame_count() const;
    virtual TRAJECTORY_FILE_TYPE get_type() const;

    /**
     * @brief Prepares the DCD file for reading.
     * Opens the filestream and calls read_header.
     * @param path the path to the DCD file
     */
    explicit DCD(const std::string & path, bool includes_crystal_information = false);

    /**
     * @brief Deletes the frame matrix;
     */
    ~DCD();

    bool is_little_endian();

  private:
    /**
     * @brief Reads the information from the header of the DCD file.
     * Writes NATOM and NFRA.
     */
    void read_header(const std::string & path);

    /**
     * @brief Reads the next coordinate from the DCD file.
     * @return the next coordinate(x, y or z).
     */
    double read_next_single_coordinate();

    std::ifstream file;

    /**
     * @brief Tells weather the file is bigendian formatted or not.
     */
    bool little_endian = true;

    /**
     * @brief Consits of the different parts of the DCD file and there size in bits.
     */
    std::map<std::string, int> structure = {
        {"endian_indicator", 4},
        {"hdrr", 4},
        {"icntrl", 4},
        {"ntitle", 4},
        {"title", 80},
        {"natm", 4},
        {"padding_before_natm", 8},
        {"padding_before_coordinate_block", 8},
        {"coordinate", 4},
        {"crystal_information", 7*8}
    };

    int    header_length = 0;
    char   HDR[4];
    int    NSET;
    int    ISTRT;
    int    NSAVC;
    int    PADDING_1[5];
    int    NATOM_NFREAT;
    int    ICNTRL[22];
    double DELTA;
    int    PADDING_2[9];
    int    NTITLE;
    char   TITLE[32*80];
    int    NATOM;
    int    NFRA;

    int current_frame_number = 0;
    char * coordinate_char;

    long file_length;
    bool   includes_crystal_information;
};

#endif
