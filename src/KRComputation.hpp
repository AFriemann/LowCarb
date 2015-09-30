/**
 * @file   KRComputation.hpp
 * @author see AUTHORS
 * @brief  KRComputation header file.
 */

#ifndef KRCOMPUTATION_HPP
#define KRCOMPUTATION_HPP

#include <memory>
#include <map>

#include "ProteinSegment.hpp"
#include "KRAverager.hpp"
#include "KRBinAverager.hpp"
#include "NonlinearFitter.hpp"
#include "StructureType.hpp"
#include "ForceConstantSelector.hpp"

#define THRESHOLD 3.5

/**
 * @class KRComputation
 */
class KRComputation {
public:
    /**
     * @param protein
     * @param protein_segment
     * @param minimum_length
     * @param maximum_length
     * @param use_slow_fittung
     * @param average_bin_length
    */
    KRComputation(
        const Protein & protein,
        const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
        const double minimum_length,
        const double maximum_length,
        const bool use_slow_fitting,
        const double slow_minimum_length,
        const double slow_maximum_length,
        const double average_bin_length);

    ~KRComputation();

    /**
     * @return selector for force constants
     */
    ForceConstantSelector kk_selector();

    /**
     * @return average force constant
     */
    std::map<std::string, double> get_average_force_constant_map() const;

    /**
     * @return complete kr protein
     */
    std::map<std::string, Eigen::VectorXd> get_kr_complete_protein_map() const;
private:
    void fill_kr_averagers();

    void do_nonlinear_fitting();

    void add_kr(KRAverager & averager, const Eigen::MatrixXd & force_constants, int offset);

    void add_kr_complete(KRBinAverager & kr_averager, const Eigen::MatrixXd & force_constants);

    void add_kr_beta_pair(KRAverager & averager, const Eigen::MatrixXd force_constants);

    Protein protein;
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments;
    double minimum_length;
    double maximum_length;
    bool use_slow_fitting;
    double slow_minimum_length;
    double slow_maximum_length;
    double average_bin_length;
    double temperature;
 
    KRAverager alpha_12_kr_averager;
    KRAverager alpha_13_kr_averager;
    KRAverager alpha_14_kr_averager;
    KRAverager alpha_15_kr_averager;
    KRAverager beta_12_kr_averager;
    KRAverager beta_13_kr_averager;
    KRAverager beta_14_kr_averager;
    KRAverager beta_pair_kr_averager;
    KRAveragerCis local_12_kr_averager;
    KRAverager local_13_kr_averager;
    KRAverager local_14_kr_averager;
    KRBinAverager complete_protein_kr_averager;
 
    NonlinearFitter fast_fitter;
    NonlinearFitter slow_fitter;
 
    std::map<std::string, double> average_force_constant_map;
    std::map<std::string, Eigen::VectorXd> kr_complete_protein_map;

};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
