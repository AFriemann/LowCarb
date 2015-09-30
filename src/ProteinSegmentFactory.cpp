/**
 * @file   ProteinSegmentFactory.cpp
 * @author see AUTHORS
 * @brief  ProteinSegmentFactory definitions file.
 */

#include "ProteinSegmentFactory.hpp"

std::vector<std::shared_ptr<ProteinSegment>> ProteinSegmentFactory::generate_protein_segments_for_analysis(const Protein & protein) {
    if (protein.residue_count() < ATOMS_PER_LOCAL_INTERACTION_SEGMENT) {
        throw std::runtime_error("the protein does not have enough residuums");
    }

    LOGD << "generating protein segments from protein";

    std::vector<std::shared_ptr<ProteinSegment>> protein_segments;

    add_complete_protein_segment(protein, protein_segments);
    add_local_interaction_segments(protein, protein_segments);
    add_alpha_helix_segments(protein, protein_segments);
    add_beta_strand_segments(protein, protein_segments);

    return protein_segments;
}

void ProteinSegmentFactory::add_complete_protein_segment(const Protein & protein,
                                                         std::vector<std::shared_ptr<ProteinSegment>> & protein_segments) {

    LOGV << "adding complete protein segment";

    std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(protein.get_residues(),
                                                                               1,
                                                                               protein.residue_count(),
                                                                               COMPLETE_PROTEIN));
    protein_segments.push_back(protein_segment);
}

void ProteinSegmentFactory::add_local_interaction_segments(const Protein & protein,
                                                           std::vector<std::shared_ptr<ProteinSegment>> & protein_segments) {
    LOGV << "adding local interaction segments";

    int next_start_atom_nr = 1;
    int segment_shift = ATOMS_PER_LOCAL_INTERACTION_SEGMENT - LOCAL_INTERACTION_SEGMENT_OVERLAP;

    while (next_start_atom_nr < protein.residue_count() - ATOMS_PER_LOCAL_INTERACTION_SEGMENT) {
        std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(protein.get_residues(),
                                                                           next_start_atom_nr,
                                                                           next_start_atom_nr + ATOMS_PER_LOCAL_INTERACTION_SEGMENT - 1,
                                                                           LOCAL_INTERACTION
        ));

        protein_segments.push_back(protein_segment);
        next_start_atom_nr += segment_shift;
    }
    std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(
        protein.get_residues(),
        protein.residue_count() - ATOMS_PER_LOCAL_INTERACTION_SEGMENT + 1,
        protein.residue_count(),
        LOCAL_INTERACTION
    ));

    protein_segments.push_back(protein_segment);
}

void ProteinSegmentFactory::add_alpha_helix_segments(const Protein & protein,
                                                     std::vector<std::shared_ptr<ProteinSegment>> & protein_segments) {

    LOGV << "adding alpha helix segments";

    std::vector<Range> alpha_helix_ranges = protein.get_alpha_helix_ranges();

    for (Range const & alpha_helix_range : alpha_helix_ranges){
        std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(protein.get_residues(),
                                                                           alpha_helix_range.start,
                                                                           alpha_helix_range.end,
                                                                           ALPHA_HELIX));
        protein_segments.push_back(protein_segment);
    }
}

void ProteinSegmentFactory::add_beta_strand_segments(const Protein & protein,
                                                     std::vector<std::shared_ptr<ProteinSegment>> & protein_segments) {

    LOGV << "adding beta strand segments";

    std::vector<Range> beta_strand_ranges = protein.get_beta_strand_ranges();

    for (Range const & beta_strand_range : beta_strand_ranges){
        std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(protein.get_residues(),
                                                                           beta_strand_range.start,
                                                                           beta_strand_range.end,
                                                                           BETA_STRAND));
        protein_segments.push_back(protein_segment);
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
