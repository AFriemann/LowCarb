/* test.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include "test.hpp"

void check_atoms_for_equality(const Atom& atom1, const Atom& atom2) {
    BOOST_CHECK_EQUAL(atom1.get_x(), atom2.get_x());
    BOOST_CHECK_EQUAL(atom1.get_y(), atom2.get_y());
    BOOST_CHECK_EQUAL(atom1.get_z(), atom2.get_z());
}

void check_atoms_for_equality(
    const Atom& atom1, const Atom& atom2, float error) {
    BOOST_CHECK_CLOSE(atom1.get_x(), atom2.get_x(), error);
    BOOST_CHECK_CLOSE(atom1.get_y(), atom2.get_y(), error);
    BOOST_CHECK_CLOSE(atom1.get_z(), atom2.get_z(), error);
}

void check_frames_for_equality(
    const Frame& frame1, const Frame& frame2, float error) {
    AtomPositions atoms_from_frame1 = frame1.get_atom_positions();
    AtomPositions atoms_from_frame2 = frame2.get_atom_positions();
    BOOST_CHECK_EQUAL(atoms_from_frame1.get_x().size(),
                      atoms_from_frame2.get_x().size());
    for (int i = 0; i < atoms_from_frame1.get_x().size(); ++i) {
        BOOST_REQUIRE_CLOSE(atoms_from_frame1.get_x()[i], atoms_from_frame2.get_x()[i], error);
        BOOST_REQUIRE_CLOSE(atoms_from_frame1.get_y()[i], atoms_from_frame2.get_y()[i], error);
        BOOST_REQUIRE_CLOSE(atoms_from_frame1.get_z()[i], atoms_from_frame2.get_z()[i], error);
    }
}

void compare_two_sym_matrices(const Eigen::MatrixXd & mat_a, const Eigen::MatrixXd & mat_b, double precision)
{

    if (mat_a.rows() != mat_a.cols() || mat_b.rows() != mat_b.cols() || mat_a.rows() != mat_b.rows())
    {
        std::cerr << "matrices malformed" << std::endl;
        return;
    }

    size_t a_len = mat_a.rows();

    for (size_t i=0; i<a_len; ++i)
        for (size_t j=0; j<a_len; ++j)
            BOOST_REQUIRE_CLOSE_FRACTION (mat_a(i,j), mat_b(i,j), precision);

}

void compare_ranges(std::vector<Range> ranges1, std::vector<Range> & ranges2) {
    BOOST_CHECK_EQUAL(ranges1.size(), ranges2.size());

    for (int i = 0; i < ranges1.size(); ++i){
        BOOST_CHECK_EQUAL(ranges1[i].start, ranges2[i].start);
        BOOST_CHECK_EQUAL(ranges1[i].end, ranges2[i].end);
        BOOST_CHECK_EQUAL(ranges1[i].length, ranges2[i].length);
    }
}

void compare_force_constant_matrix(
        Eigen::MatrixXd & low_carb_result,
        const std::string & filename_reach_results,
        double precision,
        bool is_hex,
        bool toggle_row_and_col) {

    if (!exists(boost::filesystem::path(filename_reach_results))) {
        throw std::runtime_error("file does not exist: " + filename_reach_results);
    }

    std::ifstream file_reach_results;
    file_reach_results.open(filename_reach_results);

    size_t rows=0;
    size_t cols=0;

    std::string row;

    while (std::getline(file_reach_results,row)) {
        cols=0;
        size_t begin=0;
        size_t delimiter_pos;
        std::string entry;


        do {
            delimiter_pos = row.find(',',begin);
            entry = row.substr(begin, delimiter_pos-begin);
            double low_carb_result_entry;
            if(toggle_row_and_col) {
                low_carb_result_entry = low_carb_result(cols,rows);
            } else {
                low_carb_result_entry = low_carb_result(rows, cols);
            }
            if (is_hex)
            {
                BOOST_REQUIRE_CLOSE_FRACTION(cast_hex_to_double(entry), low_carb_result_entry, precision);
            }
            else
                BOOST_REQUIRE_CLOSE_FRACTION(std::atof(entry.c_str()), low_carb_result_entry, precision);

            begin=delimiter_pos+1;
            cols++;
        } while (delimiter_pos != std::string::npos);

        rows++;
    }

    BOOST_CHECK_EQUAL(rows, cols);
    BOOST_CHECK_EQUAL(rows, low_carb_result.rows());
    BOOST_CHECK_EQUAL(cols, low_carb_result.cols());
    //std::cout << "average difference :" << diff_sum/(cols*rows) << std::endl;
}

bool close_to_zero(double a, double eps)
{
    return (fabs(a) < eps);
}

double get_precision(double a, double b)
{
    double eps = 10-16;
    if ( close_to_zero(b,eps) == false)
        return fabs(a/b-1);
    else
        return 0;

}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
