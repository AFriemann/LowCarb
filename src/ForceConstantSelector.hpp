/**
 * @file   ForceConstantSelector.hpp
 * @author see AUTHORS
 * @brief  ForceConstantSelector header file.
 */

#ifndef KKSELECTOR_HPP
#define KKSELECTOR_HPP

#include "Protein.hpp"

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

#include <stdexcept>

/**
 * threshold for cutoff distance
 */
#define THRESHOLD 3.5

/*
 * @class ForceConstantSelector
 * @brief used to select the most accurate force constant
 */
class ForceConstantSelector {
 public:
    /**
     * @param force_constant_to_next
     * @param force_constant_to_2nd_next
     * @param force_constant_to_3nd_next
     * @param anb
     * @param anb2
     * @param bnb
     * @param bnb2
     * @param force_constant_to_next_cis
     * @param force_constant_to_next_in_alpha_helix
     * @param force_constant_to_2nd_next_in_alpha_helix
     * @param force_constant_to_3rd_next_in_alpha_helix
     * @param force_constant_to_4th_next_in_alpha_helix
     * @param force_constant_to_next_beta_strand
     * @param force_constant_to_2nd_next_beta_strand
     * @param force_constant_to_3rd_next_beta_strand
     * @param force_constant_beta_pair
     * @param use_slow_fitting
    */
    ForceConstantSelector(
        double force_constant_to_next, double force_constant_to_2nd_next, double force_constant_to_3nd_next, double anb, double bnb, double anb2, double bnb2,
        double force_constant_to_next_cis, double force_constant_to_next_in_alpha_helix, double force_constant_to_2nd_next_in_alpha_helix,
        double force_constant_to_3rd_next_in_alpha_helix, double force_constant_to_4th_next_in_alpha_helix,
        double force_constant_to_next_beta_strand, double force_constant_to_2nd_next_beta_strand, double force_constant_to_3rd_next_beta_strand, double force_constant_beta_pair,
        bool use_slow_fitting);

    /*
     * @brief Tries to find the most accurate force constant.
     * If none is found it defaults to exp(-anb * sqrt(r12)) * bnb and adds exp(-anb2 * sqrt(r12)) * bnb2 if slow fitting is used
     * @param structure_type
     * @param atom_distance
     * @param range1_2
     * @return most accurate force constant
     */
    double select_force_constant(StructureType structure_type, int atom_distance, double range1_2) const;

 private:
    double force_constant_to_next; double force_constant_to_2nd_next; double force_constant_to_3nd_next;
    double anb; double bnb; double anb2; double bnb2;
    double force_constant_to_next_cis;
    double force_constant_to_next_in_alpha_helix; double force_constant_to_2nd_next_in_alpha_helix; double force_constant_to_3rd_next_in_alpha_helix; double force_constant_to_4th_next_in_alpha_helix;
    double force_constant_to_next_beta_strand; double force_constant_to_2nd_next_beta_strand; double force_constant_to_3rd_next_beta_strand; double force_constant_beta_pair;
    bool use_slow_fitting;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
