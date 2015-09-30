/**
 * @file   KRAverager.cpp
 * @author see AUTHORS
 * @brief  KRAverager definitions file.
 */

#include "KRAverager.hpp"

KRAverager::KRAverager() {}

KRAverager::KRAverager(int threshold){
    this->threshold = threshold;
    this->use_threshold = true;
}

void KRAverager::add_force_constant_tuple(const double force_constant, const double range){
    if (!(this->use_threshold) || range > this->threshold){
        this->k_sum += force_constant;
        this->r_sum += range;
        this->error_sum += force_constant * force_constant;
        this->count += 1;
    }
}


void KRAverager::add_force_constant_vector(const Eigen::VectorXd &force_constants, const Eigen::VectorXd &ranges) {
    for (int i = 0; i < force_constants.rows(); ++i) {
        add_force_constant_tuple(force_constants(i), ranges(i));
    }
}

double KRAverager::get_average_range() const {
    if (this->count > 0){
        return this->r_sum / this->count;
    } else {
        return 0;
    }
}

double KRAverager::get_average_force_constant() const {
    if (this->count > 0) {
        return this->k_sum / this->count;
    } else {
        return 0;
    }
}

double KRAverager::get_error_1() const {
    return sqrt(get_error_2());
}

double KRAverager::get_error_2() const {
    if (this->count > 0) {
        return (
            (this->error_sum / this->count)
                - get_average_force_constant() * get_average_force_constant()
            ) / (this->count);
    } else {
        return 0;
    }
}

void KRAveragerCis::add_force_constant_tuple(double k, double r) {
    if (r > this->threshold) {
        this->count += 1;
        this->k_sum += k;
        this->r_sum += r;
        this->error_sum += k * k;
    } else {
        this->cis_count += 1;
        this->k_cis_sum += k;
        this->r_cis_sum += r;
    }
}

void KRAveragerCis::add_force_constant_vector(const Eigen::VectorXd & ks, const Eigen::VectorXd & rs) {
    for (int i = 0; i < ks.rows(); ++i) {
        add_force_constant_tuple(ks(i), rs(i));
    }
}

double KRAveragerCis::get_range_cis() const {
    if (this->cis_count > 0) {
        return this->r_cis_sum / this->cis_count;
    } else {
        return 0;
    }
}

double KRAveragerCis::get_force_constant_cis() const {
    if (this->cis_count > 0) {
        return this->k_cis_sum / this->cis_count;
    } else {
        return 0;
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
