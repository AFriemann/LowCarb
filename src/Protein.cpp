/**
 * @file   Protein.cpp
 * @author see AUTHORS
 * @brief  Protein definitions file.
 */

#include "Protein.hpp"

Protein::Protein(const std::shared_ptr<ProteinFile> & file) :
     atoms(file->get_atoms()),
     secondary_structure_file() {

    this->init();
}

Protein::Protein(const std::shared_ptr<ProteinFile> & protein_file,
                 const std::shared_ptr<SecondaryStructureFile> & secondary_structure_file)
    : atoms(protein_file->get_atoms()), secondary_structure_file(secondary_structure_file) {

    this->init();
}

void Protein::init(){
    for (Atom const & atom : this->atoms) {
        if (atom.is_c_alpha()) {
            std::vector<Atom> residuum_atoms;

            for(Atom const & residuum_atom : this->atoms) {
                if (residuum_atom.get_residuum_number() == atom.get_atom_number())
                    residuum_atoms.push_back(residuum_atom);
            }

            Residuum residuum(atom, residuum_atoms);
            this->residues.push_back(residuum);
        }
    }

    LOGD << "initialized Protein with " << this->atoms.size() << " atoms and " << this->residues.size() << " CA atoms.";
}


bool Protein::has_secondary_structure_information() const {
    std::vector<Range> alpha_helices = this->secondary_structure_file->get_alpha_helix_ranges();
    std::vector<Range> beta_strands = this->secondary_structure_file->get_alpha_helix_ranges();
    std::vector<std::pair<int,int>> beta_pairs = this->secondary_structure_file->get_beta_pairs();
    return alpha_helices.size() != 0 || beta_strands.size() != 0 || beta_pairs.size() != 0;
}

bool Protein::has_secondary_structure_interaction_in_atom_pair(int atom_nr1, int atom_nr2) const {
    return get_secondary_structure_type_of_atom_pair(atom_nr1, atom_nr2) != NONE;
}

StructureType Protein::get_secondary_structure_type_of_atom_pair(int atom_nr1, int atom_nr2) const {
    //check alpha helices, beta strands and beta pairs
    size_t i;

    std::vector<std::pair<int,int>> beta_pairs = this->secondary_structure_file->get_beta_pairs();
    for (i = 0; i < beta_pairs.size(); ++i){
        if ((beta_pairs[i].first == atom_nr1 || beta_pairs[i].first == atom_nr2) &&
            (beta_pairs[i].second == atom_nr1 || beta_pairs[i].second == atom_nr2)){
            return BETA_PAIR;
        }
    }

    std::vector<Range> beta_strands = this->secondary_structure_file->get_beta_strand_ranges();
    if (abs(atom_nr1 - atom_nr2) <= 3) {
        for (i = 0; i < beta_strands.size(); ++i){
            if (beta_strands[i].start <= atom_nr1 && beta_strands[i].end >= atom_nr1 &&
                beta_strands[i].start <= atom_nr2 && beta_strands[i].end >= atom_nr2){
                return BETA_STRAND;
            }
        }
    }

    std::vector<Range> alpha_helices = this->secondary_structure_file->get_alpha_helix_ranges();
    if (abs(atom_nr1 - atom_nr2) <= 4) {
        for (i = 0; i < alpha_helices.size(); ++i){
            if (alpha_helices[i].start <= atom_nr1 && alpha_helices[i].end >= atom_nr1 &&
                alpha_helices[i].start <= atom_nr2 && alpha_helices[i].end >= atom_nr2){
                return ALPHA_HELIX;
            }
        }
    }
    return NONE;
}
//LCOV_EXCL_START
std::vector<Atom> Protein::get_atoms() const {
    return this->atoms;
}

std::vector<Residuum> Protein::get_residues() const {
    return this->residues;
}

std::vector<std::pair<int,int>> Protein::get_beta_pairs() {
    return this->secondary_structure_file->get_beta_pairs();
}

size_t Protein::residue_count() const {
    return this->residues.size();
}

std::vector<Range> Protein::get_alpha_helix_ranges() const {
    return this->secondary_structure_file->get_alpha_helix_ranges();
}
std::vector<Range> Protein::get_beta_strand_ranges() const {
    return this->secondary_structure_file->get_beta_strand_ranges();
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
