/**
 * @file   ProteinSegmentEnsemble.cpp
 * @author see AUTHORS
 * @brief  ProteinSegmentEnsemble definitions file.
 */

#include "ProteinSegmentEnsemble.hpp"

ProteinSegmentEnsemble::ProteinSegmentEnsemble(const std::shared_ptr<ProteinSegment> & protein_segment)
    : protein_segment(protein_segment),
    force_constant(protein_segment->get_size() * 3, protein_segment->get_size() * 3),
    covariance_averager(protein_segment->get_size() * 3, protein_segment->get_size() * 3),
    displacement_vector_averager(protein_segment->get_size() * 3),
    frame_segment(protein_segment) {
}

ProteinSegmentEnsemble::~ProteinSegmentEnsemble() {
}

void ProteinSegmentEnsemble::add_frame(const Frame & frame) {
    this->frame_segment.set_frame(frame);
    Eigen::VectorXd displacement_vector = this->frame_segment.fit_to_reference();

    this->displacement_vector_averager.add(displacement_vector);
    Eigen::MatrixXd cov = displacement_vector * displacement_vector.transpose();
    this->covariance_averager.add(cov);
}

void ProteinSegmentEnsemble::compute_force_constant(double temperature) {
    Eigen::VectorXd displacement_vector = this->displacement_vector_averager.get();
    Eigen::MatrixXd covariance = this->covariance_averager.get()
        - displacement_vector * displacement_vector.transpose();

    Eigen::MatrixXd kk_matrix = Eigen::MatrixXd::Zero(protein_segment->get_size(), protein_segment->get_size());
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(covariance);

    for (int i = 0; i < protein_segment->get_size(); i++) {
        for (int j = 0; j < i; j++) {
            Eigen::Vector3d kk = Eigen::Vector3d::Zero();
            Eigen::Vector3d d = displacement_vector.segment<3>(3*i)-displacement_vector.segment<3>(3*j);
            for(int k = (protein_segment->get_size() > 2 ? 6 : 5); k < protein_segment->get_size()*3; k++) {
                kk += Eigen::Vector3d(
                    eig.eigenvectors()(3*i+0,k)*eig.eigenvectors()(3*j+0,k),
                    eig.eigenvectors()(3*i+1,k)*eig.eigenvectors()(3*j+1,k),
                    eig.eigenvectors()(3*i+2,k)*eig.eigenvectors()(3*j+2,k))
                    / eig.eigenvalues()[k];
            }
            kk_matrix(i,j) = -kk.sum()* 8.31 * temperature * 0.001;
            kk_matrix(j,i) = d.norm();
        }
    }

    this->protein_segment->add_force_constant(kk_matrix);
    this->protein_segment->add_displacement_vector(displacement_vector);
    this->protein_segment->add_mean_square_fluctuation(covariance);
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
