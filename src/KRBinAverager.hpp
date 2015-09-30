#ifndef KRBINAVERAGER_H
#define KRBINAVERAGER_H

#include <Eigen/Dense>
#include <math.h>

#include "KRAverager.hpp"

/**
 * @class KRBinAverager
 */
class KRBinAverager {
    public:
        /**
         * @param max
         * @param min
         * @param bin
         */
        KRBinAverager(const double max, const double min, const double bin);

        /**
         * @param k
         * @param r
         */
        void add_force_constant_tuple(double k, double r);

        /**
         * @return rs
         */
        Eigen::VectorXd get_rs() const;

        /**
         * @return ks
         */
        Eigen::VectorXd get_ks() const;

        /**
         * @return error 2s
         */
        Eigen::VectorXd get_error2s() const;

        /**
         * @return errors
         */
        Eigen::VectorXd get_errors() const;
    private:
        std::vector<KRAverager> kr_averagers;
        Eigen::VectorXd rs;

        int number_of_bins;
        double min;
        double max;
        double bin;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
