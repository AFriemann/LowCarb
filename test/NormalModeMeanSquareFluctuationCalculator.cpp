/* NormalModeMeanSquareFluctuationCalculator.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/Reach.hpp"
#include "../src/FileFactory.hpp"
#include "../src/TrajectoryFile.hpp"
#include "../src/ProteinFile.hpp"
#include "../src/SecondaryStructureFile.hpp"
#include "../src/cli/ConfigParser.hpp"

#include "utils/file.hpp"
#include "utils/log.hpp"
#include "utils/definitions.hpp"

BOOST_AUTO_TEST_SUITE(calnmodemsfss)

    BOOST_AUTO_TEST_CASE(nmodem_test_all) {
        TEST_MESSAGE("nmodem_test_all");

        std::string test_dir = "test/resources/calnmodemsfss/";
        std::string test_prefix = "mb_ca";

        NormalModeMeanSquareFluctuationCalculator nmmsfc
                = get_nmmsfc_for_testing("tools/REACH/test/mb_ca.pdb","tools/REACH/test/1a6g.dssp","mb_ca");

        Eigen::MatrixXd hessian = nmmsfc.get_hessian_matrix();
        Eigen::VectorXd xxcom = nmmsfc.get_mean_square_fluctuation();
        Eigen::VectorXd eig = nmmsfc.get_weighted_eigenvalues();
        Eigen::VectorXd vec = nmmsfc.get_weighted_eigenvector();

        // read reach result
        Eigen::MatrixXd reach_hessian = get_sym_matrix_from_file(test_dir + test_prefix + "_calnmodemsfss_hessian.csv",453, ',', true);
        Eigen::VectorXd xxcom_reach = get_vector_from_csv(test_dir + test_prefix + "_calnmodemsfss_xxcom.csv","xxcom");
        Eigen::VectorXd eig_reach = get_vector_from_csv(test_dir + test_prefix + "_calnmodemsfss_eigvec.csv","eig");
        Eigen::VectorXd vec_reach = get_vector_from_csv(test_dir + test_prefix + "_calnmodemsfss_eigvec.csv","vec");

        // compare hessian, eig, vec, xxcom
        for (size_t i=0; i<453; ++i) {
            for (size_t j=0; j<453; ++j) {
                BOOST_REQUIRE_CLOSE_FRACTION(hessian(i,j), reach_hessian(i,j), PRECISION);
            }
            BOOST_REQUIRE_CLOSE_FRACTION(vec(i), vec_reach(i), PRECISION);
        }
        // check eig, ignoring the first six trivial eigenvalues
        for (size_t i=6; i<453; ++i) {
            BOOST_REQUIRE_CLOSE_FRACTION(eig(i), eig_reach(i), PRECISION);
        }

        for (size_t i=0; i<151; ++i) {
            BOOST_REQUIRE_CLOSE_FRACTION(xxcom(i), xxcom_reach(i), PRECISION);
        }
}

BOOST_AUTO_TEST_SUITE_END()


// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
