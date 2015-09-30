/**
 * @file   NonlinearFitter.hpp
 * @author see AUTHORS
 * @brief  NonlinearFitter definition file.
 */

#include "NonlinearFitter.hpp"

void NonlinearFitter::exponential_fit(Eigen::VectorXd & x, Eigen::VectorXd & y, Eigen::VectorXd & yerr2){
    double lambda = 0.01;
    double a0 = -log(fabs(y(1)/y(0)))/(x(1)-x(0));
    double b0 = y(0) * exp(a0 * x(0));

    int loop = 0;
    double score0 = 0.0;
    double score = 0.0;
    int i;

    for (i = 0; i < x.rows(); ++i){
        score0 += (y(i) - b0 * exp(-a0 * x(i))) * (y(i) - b0 * exp(-a0 * x(i))) / yerr2(i);
    }

    for(loop = 0; loop < 100; ++loop){
        double Q1 = 0.0;
        double Q2 = 0.0;
        double P11 = 0.0;
        double P12 = 0.0;
        double P21 = 0.0;
        double P22 = 0.0;

        for (i = 0; i < x.rows(); ++i){
            double tmp1 = -exp(-a0 * x(i)) * b0 * x(i);
            double tmp2 = exp(-a0 * x(i));
            double tmp0 = b0 * exp(-a0 * x(i));
            Q1 += tmp1 * (y(i) - tmp0) / yerr2(i);
            Q2 += tmp2 * (y(i) - tmp0) / yerr2(i);
            P11 += tmp1 * tmp1 * (1.0 + lambda) / yerr2(i);
            P22 += tmp2 * tmp2 * (1.0 + lambda) / yerr2(i);
            P12 += tmp1 * tmp2 / yerr2(i);
            P21 += tmp1 * tmp2 / yerr2(i);
        }
        double tmp = P11 * P22 - P12 * P12;

        this->a = a0 + (P22 * Q1 - P12 * Q2) / tmp;
        this->b = b0 + (P11 * Q2 - P12 * Q1) / tmp;

        score = 0.0;
        for (i = 0; i < x.rows(); ++i){
            score += ((y(i) - b* exp(-a * x(i))) * (y(i) - b* exp(-a * x(i)))) / yerr2(i);
        }

        if (score > score0){
            lambda *= 10.0;
        } else {
            if (!((score0 - score)/score0 < 0.000001)){
                lambda *= 0.1;
                a0 = this->a;
                b0 = this->b;
                score0 = score;
            } else {
                break;
            }
        }
    }

    double P11 = 0.0;
    double P12 = 0.0;
    double P21 = 0.0;
    double P22 = 0.0;
    for (i = 0; i < x.rows(); ++i){
        double tmp1 = -exp(-a*x(i))*b*x(i);
        double tmp2 = exp(-a*x(i));
        P11 = P11+tmp1*tmp1/yerr2(i);
        P22 = P22+tmp2*tmp2/yerr2(i);
        P12 = P12+tmp1*tmp2/yerr2(i);
        P21 = P21+tmp1*tmp2/yerr2(i);
    }
    double tmp = P11*P22-P12*P12;
    this->err_a = P22/tmp;
    this->err_b = P11/tmp;
    this->err_a = sqrt(this->err_a);
    this->err_b = sqrt(this->err_b);
}

double NonlinearFitter::get_a(){
    return this->a;
}
double NonlinearFitter::get_b(){
    return this->b;
}
double NonlinearFitter::get_err_a(){
    return this->err_a;
}
double NonlinearFitter::get_err_b(){
    return this->err_b;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
