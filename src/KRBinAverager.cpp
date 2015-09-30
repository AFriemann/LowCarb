/**
 * @file   KRBinAverager.cpp
 * @author see AUTHORS
 * @brief  KRBinAverager definitions file.
 */

#include "KRBinAverager.hpp"

KRBinAverager::KRBinAverager(const double min, const double max, const double bin)
    :min(min), max(max), bin(bin) {
    this->number_of_bins = int((max-min)/bin);
    this->rs = Eigen::VectorXd::Zero(this->number_of_bins);
    for (size_t i = 0; i < this->number_of_bins; i++) {
        KRAverager kr_averager;
        this->kr_averagers.push_back(kr_averager);
        this->rs(i) = min + bin*(double(i)+0.5);
    }
}

void KRBinAverager::add_force_constant_tuple(const double k, const double r){
    int n = int((r-this->min)/this->bin);
    if (n >= 0 && n < this->number_of_bins) {
        kr_averagers[n].add_force_constant_tuple(k, r);
    }
}

Eigen::VectorXd KRBinAverager::get_rs() const {
    return this->rs;
}

Eigen::VectorXd KRBinAverager::get_ks() const {
    Eigen::VectorXd ks = Eigen::VectorXd::Zero(this->number_of_bins);
    for (size_t i = 0; i < this->number_of_bins; i++) {
        ks(i)= this->kr_averagers[i].get_average_force_constant();
    }
    return ks;
}

Eigen::VectorXd KRBinAverager::get_error2s() const {
    Eigen::VectorXd error2s(this->number_of_bins);
    for (size_t i = 0; i < this->number_of_bins; i++) {
        error2s(i)= this->kr_averagers[i].get_error_2();
    }
    return error2s;
}

Eigen::VectorXd KRBinAverager::get_errors() const {
    Eigen::VectorXd errors(this->number_of_bins);
    for (size_t i = 0; i < this->number_of_bins; i++) {
        errors(i)= this->kr_averagers[i].get_error_1();
    }
    return errors;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
