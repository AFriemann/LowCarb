/**
 * @file   NMAAnalyzer.hpp
 * @author see AUTHORS
 * @brief  NMAAnalyzer header file.
 */

#ifndef NONLINEARFITTER_HPP
#define NONLINEARFITTER_HPP

#include <math.h>

#include <Eigen/Dense>

/**
 * @class NonlinearFitter
 */
class NonlinearFitter {
public:
    /**
     * @param x
     * @param y
     * @param yerr2
     */
    void exponential_fit(Eigen::VectorXd & x, Eigen::VectorXd & y, Eigen::VectorXd & yerr2); //length == ndat/S

    /**
     * @return a
     */
    double get_a();

    /**
     * @return b
     */
    double get_b();

    /**
     * @return error for a
     */
    double get_err_a();

    /**
     * @return error for b
     */
    double get_err_b();
private:
    double a = 0;
    double b = 0;
    double err_a = 0;
    double err_b = 0;
    bool calculation_completed = false;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
