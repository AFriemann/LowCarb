/**
 * @file   NormalModeMeanSquareFluctuationCalculator.cpp
 * @author see AUTHORS
 * @brief  NormalModeMeanSquareFluctuationCalculator definitions file.
 */

#include "NormalModeMeanSquareFluctuationCalculator.hpp"

NormalModeMeanSquareFluctuationCalculator::NormalModeMeanSquareFluctuationCalculator(
        const std::vector<Residuum> & residues) :
    residue_count(residues.size()),
    hessian_matrix(Eigen::MatrixXd::Zero(residues.size()*3, residues.size()*3)),
    weighted_eigenvalues(residues.size() * 3),
    mean_square_fluctuation(Eigen::VectorXd::Zero(residues.size())),
    weighted_eigenvector(residues.size() * 3),
    eigenvalues(residues.size() * 3),
    eigenvectors(residues.size() * 3, residues.size() * 3),
    mass(residues.size()),
    done(false) {

    LOGD << "setting up NormalModeMeanSquareFluctuationCalculator";

    for (size_t i = 0; i < mass.size(); i++) {
        this->mass(i) = residues[i].get_mass();
    }
}

void NormalModeMeanSquareFluctuationCalculator::calculate(const Protein & protein,
                                                          const double & temperature,
                                                          const ForceConstantSelector & force_constant_selector,
                                                          const std::shared_ptr<ProteinSegment> & protein_segment,
                                                          const ModelReduction & model_reduction) {

    this->calculate_hessian_matrix(protein, force_constant_selector, protein_segment);

    this->calculate_model_reduction(model_reduction);

    this->calculate_eigenvalues_and_eigenvectors();
    this->calculate_mean_square_fluctuation(temperature);
    this->calculate_weighted_eigenvalues();
    this->calculate_weighted_eigenvector();
}

void NormalModeMeanSquareFluctuationCalculator::calculate(const Protein & protein,
                                                          const double & temperature,
                                                          const ForceConstantSelector & force_constant_selector,
                                                          const std::shared_ptr<ProteinSegment> &protein_segment) {

    this->calculate_hessian_matrix(protein, force_constant_selector, protein_segment);
    this->calculate_eigenvalues_and_eigenvectors();
    this->calculate_mean_square_fluctuation(temperature);
    this->calculate_weighted_eigenvalues();
    this->calculate_weighted_eigenvector();
}

void NormalModeMeanSquareFluctuationCalculator::calculate_model_reduction(const ModelReduction & model_reduction) {
    size_t reduction_selection_size = model_reduction.get_selection_size();

    if (reduction_selection_size > 0) {
        this->hessian_matrix = model_reduction.reduce_hessian_matrix(this->hessian_matrix);

        // adjust residuum count and masses
        Eigen::VectorXd new_masses(reduction_selection_size);

        for (size_t i = 0; i < reduction_selection_size; ++i) {
            new_masses(i) = this->mass(model_reduction.get_selection_at(i) - 1);
        }

        this->mass = new_masses;

        // resize space

        this->weighted_eigenvalues.resize(reduction_selection_size * 3);
        this->mean_square_fluctuation = Eigen::VectorXd::Zero(this->residue_count);
        this->weighted_eigenvector.resize(reduction_selection_size * 3);
        this->eigenvalues.resize(reduction_selection_size * 3);
        this->eigenvectors.resize(reduction_selection_size * 3, reduction_selection_size * 3);
    }
}

void NormalModeMeanSquareFluctuationCalculator::calculate_weighted_eigenvalues() {
    LOGD << "calculating eig";

    for (int i = 0; i < this->weighted_eigenvalues.rows(); ++i) {
        this->weighted_eigenvalues(i) = 53.0516 * sqrt(fabs(this->eigenvalues(i)));
    }

}

void NormalModeMeanSquareFluctuationCalculator::calculate_weighted_eigenvector() {
    LOGD << "calculating vec";

    for (int i = 0; i < mass.rows(); ++i){
        this->weighted_eigenvector(i*3 + 0) = this->eigenvectors(i*3 + 0, NMODE + 5) / sqrt(mass(i));
        this->weighted_eigenvector(i*3 + 1) = this->eigenvectors(i*3 + 1, NMODE + 5) / sqrt(mass(i));
        this->weighted_eigenvector(i*3 + 2) = this->eigenvectors(i*3 + 2, NMODE + 5) / sqrt(mass(i));
    }
}

void NormalModeMeanSquareFluctuationCalculator::calculate_mean_square_fluctuation(const double &temperature) {
    LOGD << "calculating xxcom";

    for (int i=0; i<this->residue_count
            ; ++i){
        for (int j=6; j <3*this->residue_count
                ; ++j) {

            double tmp = this->eigenvectors(i*3,j)*this->eigenvectors(i*3,j)
                    + this->eigenvectors(i*3+1,j)*this->eigenvectors(i*3+1,j)
                    + this->eigenvectors(i*3+2,j)*this->eigenvectors(i*3+2,j);

            this->mean_square_fluctuation(i) += tmp / this->eigenvalues(j) / this->mass(i) * 8.31 * temperature * 0.001;
        }
    }
}

void NormalModeMeanSquareFluctuationCalculator::calculate_eigenvalues_and_eigenvectors() {
    LOGD << "calculating eigenvalues and eigenvectors";

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigen_solver(this->hessian_matrix);
    this->eigenvalues = eigen_solver.eigenvalues();
    this->eigenvectors = eigen_solver.eigenvectors();
}

void NormalModeMeanSquareFluctuationCalculator::calculate_hessian_matrix(
        const Protein& protein,
        const ForceConstantSelector & kk_selector,
        const std::shared_ptr<ProteinSegment> &protein_segment) {
    LOGD << "calculating hessian matrix";
        Eigen::VectorXd ave = protein_segment->displacement_vector();

    for (size_t ires=0; ires < this->residue_count; ++ires) {
        for (size_t jres=0; jres < ires; ++jres) {
            Eigen::Vector3d dr = ave.segment<3>(3 * ires) - ave.segment<3>(3 * jres);

            double range1_2 = dr.squaredNorm();

            double kk = kk_selector.select_force_constant(
                    protein.get_secondary_structure_type_of_atom_pair(ires + 1, jres + 1),
                    ires - jres, range1_2);

            // calnmodemfs.f:72
            for (size_t ixyz = 0; ixyz < 3; ixyz++) {
                for (size_t jxyz = 0; jxyz < 3; jxyz++) {
                    size_t i = 3 * ires + ixyz;
                    size_t j = 3 * ires + jxyz;

                    this->hessian_matrix(i, j) += kk * dr(ixyz) * dr(jxyz) / range1_2;
                    i = 3 * ires + ixyz;
                    j = 3 * jres + jxyz;
                    this->hessian_matrix(i, j) -= kk * dr(ixyz) * dr(jxyz) / range1_2;
                    i = 3 * jres + ixyz;
                    j = 3 * ires + jxyz;
                    this->hessian_matrix(i, j) -= kk * dr(ixyz) * dr(jxyz) / range1_2;
                    i = 3 * jres + ixyz;
                    j = 3 * jres + jxyz;
                    this->hessian_matrix(i, j) += kk * dr(ixyz) * dr(jxyz) / range1_2;
                }
            }
        }
    }

    // mass weighted
    // calmodemsfss.f:200
    for (size_t ires = 0; ires < this->residue_count; ++ires) {
        for (size_t ixyz = 0; ixyz < 3; ++ixyz) {
            size_t i = 3 * ires+ixyz;
            for (size_t jres = 0; jres < this->residue_count; ++jres) {
                for (size_t jxyz = 0; jxyz < 3; ++jxyz) {
                    size_t j = 3 * jres + jxyz;
                    this->hessian_matrix(i,j) /= sqrt(mass(ires) * mass(jres));
                }
            }
        }
    }
}

//LCOV_EXCL_START
Eigen::VectorXd NormalModeMeanSquareFluctuationCalculator::get_mean_square_fluctuation() const {
    return this->mean_square_fluctuation;
}

Eigen::VectorXd NormalModeMeanSquareFluctuationCalculator::get_weighted_eigenvalues() const {
    return this->weighted_eigenvalues;
}

Eigen::VectorXd NormalModeMeanSquareFluctuationCalculator::get_weighted_eigenvector() const {
    return this->weighted_eigenvector;
}

Eigen::VectorXd NormalModeMeanSquareFluctuationCalculator::get_eigenvalues() const {
    return this->eigenvalues;
}

Eigen::MatrixXd NormalModeMeanSquareFluctuationCalculator::get_eigenvectors() const {
    return this->eigenvectors;
}

Eigen::MatrixXd NormalModeMeanSquareFluctuationCalculator::get_hessian_matrix() const {
    return this->hessian_matrix;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
