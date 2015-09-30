/**
 * @file   ProteinSegment.cpp
 * @author see AUTHORS
 * @brief  ProteinSegment definitions file.
 */

#include "ProteinSegment.hpp"

ProteinSegment::ProteinSegment(const std::vector<Residuum> & residues,
                               const int start_residuum_nr,
                               const int end_residuum_nr,
                               StructureType type)
    : start_residuum_nr(start_residuum_nr), end_residuum_nr(end_residuum_nr),
      type(type), force_constant_averager(residues.size(), residues.size()) {

    for (int residuum_nr = start_residuum_nr; residuum_nr <= end_residuum_nr; residuum_nr++) {
        this->atoms.push_back(residues[residuum_nr-1].get_c_alpha());
    }

    LOGV << "initialized ProteinSegment " << this->get_type_as_string();
}

int ProteinSegment::get_start_residuum_nr() const {
    return this->start_residuum_nr;
}

Eigen::MatrixXd ProteinSegment::force_constant() const {
    return this->force_constant_averager.get();
}

StructureType ProteinSegment::get_type() const {
    return this->type;
}

//LCOV_EXCL_START
Eigen::VectorXd ProteinSegment::displacement_vector() const {
    return this->displacement_vector_averager.get();
}

Eigen::VectorXd ProteinSegment::mean_square_fluctuation() const {
    return this->mean_square_fluctuation_averager.get();
}
//LCOV_EXCL_STOP

void ProteinSegment::add_force_constant(const Eigen::MatrixXd & force_constant) {
    this->force_constant_averager.add(force_constant);
}

void ProteinSegment::add_displacement_vector(const Eigen::VectorXd & displacement_vector) {
    this->displacement_vector_averager.add(displacement_vector);
}

void ProteinSegment::add_mean_square_fluctuation(const Eigen::MatrixXd & covariance) {
    Eigen::VectorXd mean_square_fluctuation(this->get_size());
    for (size_t i = 0; i < this->get_size(); i++) {
        mean_square_fluctuation(i) = covariance.diagonal().segment<3>(3*i).sum();
    }
    this->mean_square_fluctuation_averager.add(mean_square_fluctuation);
    this->covariance_averager.add(covariance);
}

std::string ProteinSegment::get_type_as_string() const {
    std::stringstream num_string;
    num_string << std::setw(4) << std::setfill('0') << start_residuum_nr;
    num_string << "_";
    num_string << std::setw(4) << std::setfill('0') << end_residuum_nr;

    switch (this->type) {
        case ALPHA_HELIX:
            return "alpha_helix_" + num_string.str();
        case BETA_STRAND:
            return "beta_strand_" + num_string.str();
        case LOCAL_INTERACTION:
            return "local_interaction_" + num_string.str();
        case COMPLETE_PROTEIN:
            return "complete_protein";
    }
}

Eigen::MatrixXd ProteinSegment::get_average_covariance_matrix() const {
    return this->covariance_averager.get();
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
