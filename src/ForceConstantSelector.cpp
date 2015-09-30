/**
 * @file   ForceConstantSelector.cpp
 * @author see AUTHORS
 * @brief  ForceConstantSelector definitions file.
 */

#include "ForceConstantSelector.hpp"

ForceConstantSelector::ForceConstantSelector(
    double force_constant_to_next, double force_constant_to_2nd_next, double force_constant_to_3nd_next,
    double anb, double bnb, double anb2, double bnb2,
    double force_constant_to_next_cis,
    double force_constant_to_next_in_alpha_helix, double force_constant_to_2nd_next_in_alpha_helix, double force_constant_to_3rd_next_in_alpha_helix, double force_constant_to_4th_next_in_alpha_helix,
    double force_constant_to_next_beta_strand, double force_constant_to_2nd_next_beta_strand, double force_constant_to_3rd_next_beta_strand, double force_constant_beta_pair,
    bool use_slow_fitting
) :
    force_constant_to_next(force_constant_to_next), force_constant_to_2nd_next(force_constant_to_2nd_next), force_constant_to_3nd_next(force_constant_to_3nd_next),
    anb(anb), bnb(bnb), anb2(anb2), bnb2(bnb2), force_constant_to_next_cis(force_constant_to_next_cis),
    force_constant_to_next_in_alpha_helix(force_constant_to_next_in_alpha_helix), force_constant_to_2nd_next_in_alpha_helix(force_constant_to_2nd_next_in_alpha_helix),
    force_constant_to_3rd_next_in_alpha_helix(force_constant_to_3rd_next_in_alpha_helix), force_constant_to_4th_next_in_alpha_helix(force_constant_to_4th_next_in_alpha_helix),
    force_constant_to_next_beta_strand(force_constant_to_next_beta_strand), force_constant_to_2nd_next_beta_strand(force_constant_to_2nd_next_beta_strand),
    force_constant_to_3rd_next_beta_strand(force_constant_to_3rd_next_beta_strand), force_constant_beta_pair(force_constant_beta_pair), use_slow_fitting(use_slow_fitting)
{
}

double ForceConstantSelector::select_force_constant(StructureType structure_type, int atom_distance, double range1_2) const {
    switch (structure_type) {
        case StructureType::BETA_STRAND:
            switch (atom_distance) {
                case 1:
                    return force_constant_to_next_beta_strand;
                case 2:
                    return force_constant_to_2nd_next_beta_strand;
                case 3:
                    return force_constant_to_3rd_next_beta_strand;
                default:
                    break;
            }
        case StructureType::ALPHA_HELIX:
            switch (atom_distance) {
                case 1:
                    return force_constant_to_next_in_alpha_helix;
                case 2:
                    return force_constant_to_2nd_next_in_alpha_helix;
                case 3:
                    return force_constant_to_3rd_next_in_alpha_helix;
                case 4:
                    return force_constant_to_4th_next_in_alpha_helix;
                default:
                    break;
            }
        case StructureType::BETA_PAIR:
            return force_constant_beta_pair;
        default:
            switch (atom_distance) {
                case 1:
                    if (sqrt(range1_2) > THRESHOLD)
                        return force_constant_to_next_cis;
                    else
                        return force_constant_to_next;
                case 2:
                    return force_constant_to_2nd_next;
                case 3:
                    return force_constant_to_3nd_next;
            }
    }
    double force_constant = exp(-anb * sqrt(range1_2)) * bnb;
    if (use_slow_fitting)
        force_constant += exp(-anb2 * sqrt(range1_2)) * bnb2;
    return force_constant;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
