    /**
 * @file   KRComputation.cpp
 * @author see AUTHORS
 * @brief  KRComputation definitions file.
 */

#include "KRComputation.hpp"

KRComputation::KRComputation(
    const Protein & protein,
    const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
    const double minimum_length,
    const double maximum_length,
    const bool use_slow_fitting,
    const double slow_minimum_length,
    const double slow_maximum_length,
    const double average_bin_length
) : protein(protein),
    protein_segments(protein_segments),
    minimum_length(minimum_length), maximum_length(maximum_length),
    use_slow_fitting(use_slow_fitting),
    slow_minimum_length(slow_minimum_length), slow_maximum_length(slow_maximum_length),
    average_bin_length(average_bin_length),
    alpha_12_kr_averager(THRESHOLD),
    alpha_13_kr_averager(),
    alpha_14_kr_averager(),
    alpha_15_kr_averager(),
    beta_12_kr_averager(THRESHOLD),
    beta_13_kr_averager(),
    beta_14_kr_averager(),
    beta_pair_kr_averager(),
    local_12_kr_averager(THRESHOLD),
    local_13_kr_averager(),
    local_14_kr_averager(),
    complete_protein_kr_averager(minimum_length,
                                 slow_maximum_length,
                                 average_bin_length) {
}

KRComputation::~KRComputation() {
}

ForceConstantSelector KRComputation::kk_selector(){
    fill_kr_averagers();
    do_nonlinear_fitting();
    ForceConstantSelector kk_selector(
            this->local_12_kr_averager.get_average_force_constant(),
            this->local_13_kr_averager.get_average_force_constant(),
            this->local_14_kr_averager.get_average_force_constant(),
        this->fast_fitter.get_a(),
        this->fast_fitter.get_b(),
        this->slow_fitter.get_a(),
        this->slow_fitter.get_b(),
            this->local_12_kr_averager.get_force_constant_cis(),
            this->alpha_12_kr_averager.get_average_force_constant(),
            this->alpha_13_kr_averager.get_average_force_constant(),
            this->alpha_14_kr_averager.get_average_force_constant(),
            this->alpha_15_kr_averager.get_average_force_constant(),
            this->beta_12_kr_averager.get_average_force_constant(),
            this->beta_13_kr_averager.get_average_force_constant(),
            this->beta_14_kr_averager.get_average_force_constant(),
            this->beta_pair_kr_averager.get_average_force_constant(),
        this->use_slow_fitting);
    return kk_selector;
}

void KRComputation::fill_kr_averagers() {
    for (size_t i = 0; i < this->protein_segments.size(); ++i){
        Eigen::MatrixXd force_constants = this->protein_segments[i]->force_constant();
        switch (this->protein_segments[i]->get_type()) {
        case ALPHA_HELIX:
            add_kr(this->alpha_12_kr_averager, force_constants, 1);
            add_kr(this->alpha_13_kr_averager, force_constants, 2);
            add_kr(this->alpha_14_kr_averager, force_constants, 3);
            add_kr(this->alpha_15_kr_averager, force_constants, 4);
            break;
        case BETA_STRAND:
            add_kr(this->beta_12_kr_averager, force_constants, 1);
            add_kr(this->beta_13_kr_averager, force_constants, 2);
            add_kr(this->beta_14_kr_averager, force_constants, 3);
            break;
        case LOCAL_INTERACTION:
            add_kr(this->local_12_kr_averager, force_constants, 1);
            add_kr(this->local_13_kr_averager, force_constants, 2);
            add_kr(this->local_14_kr_averager, force_constants, 3);
            break;
        case COMPLETE_PROTEIN:
            add_kr_complete(this->complete_protein_kr_averager, force_constants);
            add_kr_beta_pair(this->beta_pair_kr_averager, force_constants);
            break;
        }
    }


    this->average_force_constant_map["alpha_12_k"] = this->alpha_12_kr_averager.get_average_force_constant();
    this->average_force_constant_map["alpha_12_r"] = this->alpha_12_kr_averager.get_average_range();
    this->average_force_constant_map["alpha_12_k_error"] = this->alpha_12_kr_averager.get_error_1();
    this->average_force_constant_map["alpha_13_k"] = this->alpha_13_kr_averager.get_average_force_constant();
    this->average_force_constant_map["alpha_13_r"] = this->alpha_13_kr_averager.get_average_range();
    this->average_force_constant_map["alpha_13_k_error"] = this->alpha_13_kr_averager.get_error_1();
    this->average_force_constant_map["alpha_14_k"] = this->alpha_14_kr_averager.get_average_force_constant();
    this->average_force_constant_map["alpha_14_r"] = this->alpha_14_kr_averager.get_average_range();
    this->average_force_constant_map["alpha_14_k_error"] = this->alpha_14_kr_averager.get_error_1();
    this->average_force_constant_map["alpha_15_k"] = this->alpha_15_kr_averager.get_average_force_constant();
    this->average_force_constant_map["alpha_15_r"] = this->alpha_15_kr_averager.get_average_range();
    this->average_force_constant_map["alpha_15_k_error"] = this->alpha_15_kr_averager.get_error_1();

    this->average_force_constant_map["beta_12_k"] = this->beta_12_kr_averager.get_average_force_constant();
    this->average_force_constant_map["beta_12_r"] = this->beta_12_kr_averager.get_average_range();
    this->average_force_constant_map["beta_12_k_error"] = this->beta_12_kr_averager.get_error_1();
    this->average_force_constant_map["beta_13_k"] = this->beta_13_kr_averager.get_average_force_constant();
    this->average_force_constant_map["beta_13_r"] = this->beta_13_kr_averager.get_average_range();
    this->average_force_constant_map["beta_13_k_error"] = this->beta_13_kr_averager.get_error_1();
    this->average_force_constant_map["beta_14_k"] = this->beta_14_kr_averager.get_average_force_constant();
    this->average_force_constant_map["beta_14_r"] = this->beta_14_kr_averager.get_average_range();
    this->average_force_constant_map["beta_14_k_error"] = this->beta_14_kr_averager.get_error_1();

    this->average_force_constant_map["beta_pair_k"] = this->beta_pair_kr_averager.get_average_force_constant();
    this->average_force_constant_map["beta_pair_r"] = this->beta_pair_kr_averager.get_average_range();
    this->average_force_constant_map["beta_pair_k_error"] = this->beta_pair_kr_averager.get_error_1();

    this->average_force_constant_map["local_12_k"] = this->local_12_kr_averager.get_force_constant_cis();
    this->average_force_constant_map["local_12_r"] = this->local_12_kr_averager.get_range_cis();
    this->average_force_constant_map["local_12_k_error"] = this->local_12_kr_averager.get_error_1();
    this->average_force_constant_map["local_13_k"] = this->local_13_kr_averager.get_average_force_constant();
    this->average_force_constant_map["local_13_r"] = this->local_13_kr_averager.get_average_range();
    this->average_force_constant_map["local_13_k_error"] = this->local_13_kr_averager.get_error_1();
    this->average_force_constant_map["local_14_k"] = this->local_14_kr_averager.get_average_force_constant();
    this->average_force_constant_map["local_14_r"] = this->local_14_kr_averager.get_average_range();
    this->average_force_constant_map["local_14_k_error"] = this->local_14_kr_averager.get_error_1();

    this->kr_complete_protein_map["rs"] = this->complete_protein_kr_averager.get_rs();
    this->kr_complete_protein_map["ks"] = this->complete_protein_kr_averager.get_ks();
    this->kr_complete_protein_map["k_errors"] = this->complete_protein_kr_averager.get_error2s();

}

void KRComputation::do_nonlinear_fitting() {
    double head = this->maximum_length - this->minimum_length;

    Eigen::VectorXd fast_rs = complete_protein_kr_averager.get_rs().head(head);
    Eigen::VectorXd fast_ks = complete_protein_kr_averager.get_ks().head(head);
    Eigen::VectorXd fast_error2s = complete_protein_kr_averager.get_error2s().head(head);

    this->fast_fitter.exponential_fit(
        fast_rs,
        fast_ks,
        fast_error2s
    );

    if (this->use_slow_fitting) {
        double tail = this->slow_maximum_length - this->slow_minimum_length;

        Eigen::VectorXd slow_rs = this->complete_protein_kr_averager.get_rs().tail(tail);
        Eigen::VectorXd slow_ks = this->complete_protein_kr_averager.get_ks().tail(tail);
        Eigen::VectorXd slow_error2s = this->complete_protein_kr_averager.get_error2s().tail(tail);

        this->slow_fitter.exponential_fit(
          slow_rs,
          slow_ks,
          slow_error2s
        );
    }
}

void KRComputation::add_kr(KRAverager & averager, const Eigen::MatrixXd & force_constants, int offset){
    Eigen::VectorXd ks = force_constants.diagonal(-offset);
    Eigen::VectorXd rs = force_constants.diagonal(offset);
    averager.add_force_constant_vector(ks, rs);
}

void KRComputation::add_kr_complete(KRBinAverager & kr_averager, const Eigen::MatrixXd & force_constants) {
    for (int i = 0; i < force_constants.rows(); i++) {
        for (int j = 0; j <= i-(this->protein.has_secondary_structure_information() ? 2 : 4); j++) {
            if (protein.has_secondary_structure_interaction_in_atom_pair(i+1, j+1))
                continue;
            kr_averager.add_force_constant_tuple(force_constants(i,j),force_constants(j,i));
        }
    }
}

void KRComputation::add_kr_beta_pair(KRAverager & averager, const Eigen::MatrixXd force_constants){
    std::vector<std::pair<int,int>> beta_pairs = this->protein.get_beta_pairs();
    for (size_t i = 0; i < beta_pairs.size(); ++i) {
        averager.add_force_constant_tuple(
            force_constants(beta_pairs[i].second, beta_pairs[i].first),
            force_constants(beta_pairs[i].first, beta_pairs[i].second)
        );
    }
}

std::map<std::string, double> KRComputation::get_average_force_constant_map() const {
    return this->average_force_constant_map;
}

std::map<std::string, Eigen::VectorXd> KRComputation::get_kr_complete_protein_map() const {
    return this->kr_complete_protein_map;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
