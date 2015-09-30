/**
 * @file   TrajectoryAnalyzer.cpp
 * @author see AUTHORS
 * @brief  TrajectoryAnalyzer definition file.
 */

#include "TrajectoryAnalyzer.hpp"

void TrajectoryAnalyzer::analyze(Trajectory & trajectory,
                                 std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
                                 const double & temperature,
                                 const int ensemble_size) {

    std::vector<ProteinSegmentEnsemble> protein_segment_ensembles;

    LOGD << "Fitting protein segments with trajectory frames and computing force constants.";
    while (trajectory.has_next()) {
        for (std::shared_ptr<ProteinSegment> const & protein_segment : protein_segments) {
            protein_segment_ensembles.push_back(ProteinSegmentEnsemble(protein_segment));
        }

        LOGD << "adding frames to protein segment ensembles";
        int frame_nr = 0;
        while (trajectory.has_next() && ++frame_nr <= ensemble_size) {
            Frame frame = trajectory.get_next_frame();

            #pragma omp parallel for
            for (size_t i = 0; i < protein_segment_ensembles.size(); i++) {
                protein_segment_ensembles[i].add_frame(frame);
            }

            LOGV_IF(frame_nr % 100 == 0) << "read " << frame_nr << " frames";
        }
        LOGD << "read total number of " << frame_nr << " frames";

        LOGD << "computing force constants for protein segment ensembles.";
        #pragma omp parallel for
        for (size_t i = 0; i < protein_segment_ensembles.size(); i++) {
            protein_segment_ensembles[i].compute_force_constant(temperature);
        }
    }
    LOGD << "Finished analyzing trajectory";
}

void TrajectoryAnalyzer::analyze(const Eigen::MatrixXd & nma_covariance,
                                 std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
                                 const double & temperature) {

    LOGD << "Fitting protein segments with NMA covariance matrix and computing force constants.";
    for (std::shared_ptr<ProteinSegment> const & protein_segment : protein_segments) {
        Eigen::MatrixXd covariance = nma_covariance.block((protein_segment->get_start_residuum_nr() - 1) * 3,
                                                                (protein_segment->get_start_residuum_nr() - 1) * 3,
                                                                protein_segment->get_size() * 3,
                                                                protein_segment->get_size() * 3);

        Eigen::VectorXd displacement_vector = covariance.diagonal().array().sqrt().matrix();

        Eigen::MatrixXd kk_matrix = Eigen::MatrixXd::Zero(protein_segment->get_size(), protein_segment->get_size());
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(covariance);

        for (int i = 0; i < protein_segment->get_size(); i++) {
            for (int j = 0; j < i; j++) {
                Eigen::Vector3d kk = Eigen::Vector3d::Zero();
                Eigen::Vector3d d = displacement_vector.segment<3>(3 * i)-displacement_vector.segment<3>(3 * j);

                for(int k = (protein_segment->get_size() > 2 ? 6 : 5); k < protein_segment->get_size() * 3; k++) {
                    kk += Eigen::Vector3d(
                        eig.eigenvectors()(3 * i + 0, k) * eig.eigenvectors()(3 * j + 0, k),
                        eig.eigenvectors()(3 * i + 1, k) * eig.eigenvectors()(3 * j + 1, k),
                        eig.eigenvectors()(3 * i + 2, k) * eig.eigenvectors()(3 * j + 2, k))
                        / eig.eigenvalues()[k];
                }
                kk_matrix(i, j) = -kk.sum() * 8.31 * temperature * 0.001;
                kk_matrix(j, i) = d.norm();
            }
        }

        protein_segment->add_force_constant(kk_matrix);
        protein_segment->add_displacement_vector(displacement_vector);
        protein_segment->add_mean_square_fluctuation(covariance);
    }
    LOGD << "Finished analyzing trajectory";
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
