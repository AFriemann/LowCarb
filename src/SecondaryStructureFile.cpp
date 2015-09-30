/**
 * @file   SecondaryStructureFile.cpp
 * @author see AUTHORS
 * @brief  SecondaryStructureFile definitions file.
 */

#include "SecondaryStructureFile.hpp"

SecondaryStructureFile::~SecondaryStructureFile() {}

DSSP::~DSSP() {
}

DSSP::DSSP(const std::string & path)
    : alpha_helix_ranges(0), beta_strand_ranges(0) {

    std::ifstream file(path);
    std::vector<std::string> lines;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    int residue_count = 0;
    char last_structure = NO_STRUCTURE;
    int current_structure_start = 0;
    int current_structure_length = 0;
    int current_part_of_file = HEADER;
    int beta_pair;
    int header_length = 0;

    for (size_t i = 0; i < lines.size(); ++i) {
        if (current_part_of_file == HEADER) {
            if(lines[i].find("  #  RESIDUE") != std::string::npos) {
                current_part_of_file = MAIN;
            }
            header_length += 1;

        } else if (current_part_of_file == MAIN) {
            ++residue_count;

            if (lines[i][16] == HELIX) {
                if (last_structure != HELIX) {
                    current_structure_start = residue_count;
                    this->alpha_helix_count += 1;
                    current_structure_length = 0;
                }

                current_structure_length += 1;
                last_structure = HELIX;
                this->alpha_residue_count += 1;

                if (i == lines.size() - 1 || lines[i + 1][16] != HELIX) {
                    Range range = {current_structure_start, current_structure_start + current_structure_length - 1, current_structure_length - 1};
                    this->alpha_helix_ranges.push_back(range);
                }

            } else if (lines[i][16] == STRAND) {
                if (last_structure != STRAND) {
                    current_structure_start = residue_count;
                    this->beta_strand_count += 1;
                    current_structure_length = 0;
                }

                current_structure_length += 1;
                last_structure = STRAND;
                this->beta_residue_count += 1;

                if (i == lines.size() - 1 || lines[i + 1][16] != STRAND) {
                    Range range = {current_structure_start, current_structure_start + current_structure_length - 1, current_structure_length - 1};
                    this->beta_strand_ranges.push_back(range);
                }

                beta_pair = atoi(lines[i].substr(25, 4).c_str());
                if (beta_pair > residue_count) {
                    this->inter_beta_interactions += 1;
                    std::pair<int, int> beta_pair_tmp(i - header_length + 1, beta_pair);
                    this->beta_pairs.push_back(beta_pair_tmp);
                }

                beta_pair = atoi(lines[i].substr(29, 4).c_str());
                if(beta_pair > residue_count) {
                    this->inter_beta_interactions += 1;
                    std::pair<int, int> beta_pair_tmp(i - header_length + 1, beta_pair);
                    this->beta_pairs.push_back(beta_pair_tmp);
                }

            } else {
                last_structure = NO_STRUCTURE;
            }
        }
    }
}

DSSP::DSSP() :
    alpha_helix_ranges(0),
    beta_strand_ranges(0){

}

int DSSP::get_alpha_helix_count(){
    return this->alpha_helix_count;
}

int DSSP::get_beta_strand_count(){
    return this->beta_strand_count;
}

std::vector<Range> DSSP::get_alpha_helix_ranges(){
    return this->alpha_helix_ranges;
}

std::vector<Range> DSSP::get_beta_strand_ranges(){
    return this->beta_strand_ranges;
}

int DSSP::get_inter_beta_interactions(){
    return this->inter_beta_interactions;
}

int DSSP::get_alpha_residue_count(){
    return this->alpha_residue_count;
}

int DSSP::get_beta_residue_count(){
    return this->beta_residue_count;
}

std::vector<std::pair<int,int>> DSSP::get_beta_pairs(){
    return this->beta_pairs;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
