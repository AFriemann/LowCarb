
#include <boost/test/unit_test.hpp>
#include <Eigen/Dense>

#include "../src/KRAverager.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(kr_averager_tests)
    BOOST_AUTO_TEST_CASE(cis_averager) {
        TEST_MESSAGE("cis_averager");

        KRAveragerCis averager(3.0);
        Eigen::VectorXd ks(3);
        Eigen::VectorXd rs(3);

        ks << 1.0, 1.5, 4.0;
        rs << 2.0, 2.5, 5.0;

        averager.add_force_constant_vector(ks, rs);
        double r_cis = averager.get_range_cis();
        double k_cis = averager.get_force_constant_cis();

        BOOST_CHECK_EQUAL(r_cis, 2.25);
        BOOST_CHECK_EQUAL(k_cis, 1.25);



    }
BOOST_AUTO_TEST_SUITE_END()
