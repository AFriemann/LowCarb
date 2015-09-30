/* ModelReduction.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

#include <string>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/FileFactory.hpp"
#include "../src/ModelReduction.hpp"
#include "../src/ReductionFile.hpp"
#include "../src/NormalModeMeanSquareFluctuationCalculator.hpp"
#include "../src/Atom.hpp"
#include "../src/Residuum.hpp"

#include "utils/test.hpp"
#include "utils/file.hpp"
#include "utils/definitions.hpp"
#include "utils/log.hpp"

class MockedNormalModeMeanSquareFluctuationCalculator : public NormalModeMeanSquareFluctuationCalculator {
public:
    MockedNormalModeMeanSquareFluctuationCalculator(const std::vector<Residuum> & residues) : NormalModeMeanSquareFluctuationCalculator(residues) {

    };
    void reduce_model(const ModelReduction & model_reduction, const Eigen::MatrixXd & hessian_matrix) {
        this->hessian_matrix = hessian_matrix;
        calculate_model_reduction(model_reduction);
    };

    double get_mass() const {
        double sum_of_mass = 0;
        for(int i = 0; i < this->mass.size(); ++i) {
            sum_of_mass += this->mass(i);
        }
        return sum_of_mass;
    };
};

BOOST_AUTO_TEST_SUITE(reduce_matrix)

    struct Setup {
        Setup() { factory = ReductionFileFactory(); }

        ReductionFileFactory factory;

        std::string test_file_dir = "test/resources/mat_reduce/";
    };

    int get_dimension_from_path(std::string path) {
        std::string filename = boost::filesystem::path(path).filename().string();

        size_t first_  = filename.find('_') + 1;
        size_t second_ = filename.find('_', first_);

        try {
            return std::stoi(filename.substr(first_, second_ - first_)) * 3;
        } catch (std::invalid_argument & e) {
            TEST_MESSAGE("ERROR: failed to get dimension from filename: " + filename);
            BOOST_REQUIRE(false);
        }
    }

    BOOST_AUTO_TEST_CASE(calculate_model_reduction_test) {
        TEST_MESSAGE("calculate_model_reduction_test");

        Setup s;

        std::vector<Residuum> residues(0);
        std::vector<Atom> atoms_in_residuum_1;
        atoms_in_residuum_1.push_back(Atom(0, 0, 0, "C", 0, 0));
        atoms_in_residuum_1.push_back(Atom(0, 0, 0, "O", 0, 0));
         // mass = 28.009999999999998
        Atom c_alpha_residuum_1(0, 0, 0, "C", 0, 0);
        residues.push_back(Residuum(c_alpha_residuum_1, atoms_in_residuum_1));

        std::vector<Atom> atoms_in_residuum_2;
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "C", 0, 0));
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "O", 0, 0));
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "N", 0, 0));
        //mass = 42.086999999999996
        Atom c_alpha_residuum_2(0, 0, 0, "C", 0, 0);
        residues.push_back(Residuum(c_alpha_residuum_2, atoms_in_residuum_2));

        std::vector<Atom> atoms_in_residuum_3;
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "C", 0, 0));
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "O", 0, 0));
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "N", 0, 0));
        atoms_in_residuum_2.push_back(Atom(0, 0, 0, "S", 0, 0));
        //mass = 74.14699999999999
        Atom c_alpha_residuum_3(0, 0, 0, "C", 0, 0);
        residues.push_back(Residuum(c_alpha_residuum_3, atoms_in_residuum_3));

        Eigen::MatrixXd hessian9 = get_sym_matrix_from_file(s.test_file_dir + "hessian_input_9.mat", 9, ' ');

        std::string path(s.test_file_dir + "selection_2_9");
        MockedNormalModeMeanSquareFluctuationCalculator nmsf_calculator(residues);
        ModelReduction reduction(s.factory.create(path, 3));
        nmsf_calculator.reduce_model(reduction, hessian9);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_mass(), 28.009999999999998 + 42.086999999999996);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_mean_square_fluctuation().size(), 3);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_weighted_eigenvalues().size(), 6);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_hessian_matrix().size(), 6*6);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_eigenvalues().size(), 6);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_weighted_eigenvector().size(), 6);
        BOOST_CHECK_EQUAL(nmsf_calculator.get_eigenvectors().size(), 6*6);

    }

    BOOST_AUTO_TEST_CASE(reduce_matrix_test) {
        TEST_MESSAGE("reduce_matrix_test");

        Setup s;

        // test 90x90 matrix
        Eigen::MatrixXd hessian90 = get_sym_matrix_from_file(s.test_file_dir + "hessian_input_90.mat", 90, ' ');

        for (std::string & path : glob(s.test_file_dir + "selection_*_90")) {
            ModelReduction reduction(s.factory.create(path, 30));

            // Eigen::MatrixXd expected = get_sym_matrix_from_file(path + ".hessian", n * 3, ' ');
            Eigen::MatrixXd expected = get_sym_matrix_from_file(path + ".hessian",
                                                                get_dimension_from_path(path),
                                                                ' ');

            Eigen::MatrixXd actual = reduction.reduce_hessian_matrix(hessian90);

            compare_two_sym_matrices(actual, expected, PRECISION);
        }
    }

    BOOST_AUTO_TEST_CASE(empty_reductionfile_should_leave_matrix_untouched) {
        TEST_MESSAGE("empty_reductionfile_should_leave_matrix_untouched");

        Setup s;

        Eigen::MatrixXd hessian90 = get_sym_matrix_from_file(s.test_file_dir + "hessian_input_90.mat", 90, ' ');

        ModelReduction reduction;

        Eigen::MatrixXd reduced_matrix = reduction.reduce_hessian_matrix(hessian90);

        BOOST_REQUIRE_EQUAL(reduced_matrix, hessian90);
    }

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
