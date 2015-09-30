/**
 * @file   ProteinSegmentFactory.hpp
 * @author see AUTHORS
 * @brief  ProteinSegment factory header file.
 */

#ifndef PROTEINSEGMENTFACTORY_HPP
#define PROTEINSEGMENTFACTORY_HPP

#include <plog/Log.h>

#include "Protein.hpp"
#include "ProteinSegment.hpp"
#include "StructureType.hpp"

#define ATOMS_PER_LOCAL_INTERACTION_SEGMENT 20
#define LOCAL_INTERACTION_SEGMENT_OVERLAP 3

/**
 * @class ProteinSegmentFactory
 * @brief used to create protein segments.
 *
 * This class can be used to construct protein segment vectors for Protein objects.
 */
class ProteinSegmentFactory {
public:
    /**
     * @param protein the protein to create segments from.
     * @return an std::vector of shared pointers to ProteinSegments.
     */
    std::vector<std::shared_ptr<ProteinSegment>> generate_protein_segments_for_analysis(const Protein & protein);

private:
    /**
     * @param protein
     * @param protein_segment
     */
    void add_complete_protein_segment(const Protein & protein, std::vector<std::shared_ptr<ProteinSegment>> & protein_segments);
 
    /**
     * @param protein
     * @param protein_segment
     */
    void add_local_interaction_segments(const Protein & protein, std::vector<std::shared_ptr<ProteinSegment>> & protein_segments);
 
    /**
     * @param protein
     * @param protein_segment
     */
    void add_alpha_helix_segments(const Protein & protein, std::vector<std::shared_ptr<ProteinSegment>> & protein_segments);
 
    /**
     * @param protein
     * @param protein_segment
     */
    void add_beta_strand_segments(const Protein & protein, std::vector<std::shared_ptr<ProteinSegment>> & protein_segments);
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
