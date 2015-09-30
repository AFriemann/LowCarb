/**
* @file   Protein.hpp
* @author see AUTHORS
* @brief  Protein header file.
*/

#ifndef PROTEIN_HPP
#define PROTEIN_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <math.h>

#include <plog/Log.h>

#include "ProteinFile.hpp"
#include "Frame.hpp"
#include "Atom.hpp"
#include "Residuum.hpp"
#include "SecondaryStructureFile.hpp"
#include "StructureType.hpp"

/**
 * @class Protein
 * @brief represents the structure of a protein
 */
class Protein {
public:
    /**
     * @brief constructor for proteins containing crystal information
     * @param protein_file
     * @param secondary_structure_file
     */
    explicit Protein(const std::shared_ptr<ProteinFile> & protein_file,
                     const std::shared_ptr<SecondaryStructureFile> & secondary_structure_file);

    /**
     * @brief constructor for proteins without crystal information
     * @param protein_file
     */
    explicit Protein(const std::shared_ptr<ProteinFile> & protein_file);

    /**
     * @return all the atoms
     */
    std::vector<Atom> get_atoms() const;
    /**
     * @return all the residues
     */
    virtual std::vector<Residuum> get_residues() const;

    /**
     * @return the number of residues in the protein
     */
    virtual size_t residue_count() const;

    /**
     * @return the alpha helix range for the protein
     */
    std::vector<Range> get_alpha_helix_ranges() const;

    /**
     * @return the beta strand range for the protein
     */
    std::vector<Range> get_beta_strand_ranges() const;

    /**
     * @return true if the protein contains secondary structure information, else false
     */
    bool has_secondary_structure_information() const;

    /**
     * @param atom_nr1
     * @param atom_nr2
     * @return true if atompair has secondary structure interaction
     */
    bool has_secondary_structure_interaction_in_atom_pair(const int atom_nr1, const int atom_nr2) const;

    /**
     * @brief Protein::get_secondary_structure_type_of_atom_pair
     * @param atom_nr1
     * @param atom_nr2
     * @return secondary structure type of atompair
     */
    StructureType get_secondary_structure_type_of_atom_pair(int atom_nr1, int atom_nr2) const;


    /**
     * @return the beta pairs
     */
    std::vector<std::pair<int,int>> get_beta_pairs();
protected:
    Protein() {}

private:
    void init();
    std::vector<Atom> atoms;
    std::vector<Residuum> residues;
    std::shared_ptr<SecondaryStructureFile> secondary_structure_file;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
