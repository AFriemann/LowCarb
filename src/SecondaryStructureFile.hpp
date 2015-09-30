/**
 * @file   SecondaryStructureFile.hpp
 * @author see AUTHORS
 * @brief  SecondaryStructureFile header file.
 */

#ifndef SECONDARY_STRUCTURE_FILE_H
#define SECONDARY_STRUCTURE_FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#define HEADER 0
#define MAIN 1

#define NO_STRUCTURE ' '
#define HELIX 'H'
#define STRAND 'E'

struct Range {
    int start;
    int end;
    int length;
};

/**
 * @class SecondaryStructureFile
 */
class SecondaryStructureFile {
 public:
      // has to be virtual i.o. to call child destructor
      virtual ~SecondaryStructureFile();

      /**
       * @brief get count of alpha helices in protein.
       * @return an integer.
       */
      virtual int get_alpha_helix_count() = 0;

      /**
       * @brief get count of beta strands in protein.
       * @return an integer.
       */
      virtual int get_beta_strand_count() = 0;

      /**
       * @brief get count of alpha residues in protein.
       * @return an integer.
       */
      virtual int get_alpha_residue_count() = 0;

      /**
       * @brief get count of beta residues in protein.
       * @return an integer.
       */
      virtual int get_beta_residue_count() = 0;

      /**
       * @brief get inter beta strand interactions in protein.
       * @return an integer.
       */
      virtual int get_inter_beta_interactions() = 0;

      /**
       * @brief get alpha helix ranges for protein.
       * @return std::vector of Ranges.
       */
      virtual std::vector<Range> get_alpha_helix_ranges() = 0;

      /**
       * @brief get beta strand ranges.
       * @return std::vector of Ranges.
       */
      virtual std::vector<Range> get_beta_strand_ranges() = 0;

      /**
       * @brief get beta pairs from secondary structure file.
       * @return std::vector of std::pairs with integers.
       */
      virtual std::vector<std::pair<int,int>> get_beta_pairs() = 0;

};

class DSSP : public SecondaryStructureFile {
  public:
      explicit DSSP(const std::string & path);

      DSSP();

      ~DSSP();

      int get_alpha_helix_count();

      int get_beta_strand_count();

      int get_alpha_residue_count();

      int get_beta_residue_count();

      int get_inter_beta_interactions();

      std::vector<Range> get_alpha_helix_ranges();

      std::vector<Range> get_beta_strand_ranges();

      std::vector<std::pair<int,int>> get_beta_pairs();

  private:
      int alpha_helix_count = 0;
      int beta_strand_count = 0;

      std::vector<Range> alpha_helix_ranges;
      std::vector<Range> beta_strand_ranges;

      int inter_beta_interactions = 0;

      int alpha_residue_count = 0;
      int beta_residue_count = 0;

      std::vector<std::pair<int,int>> beta_pairs;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
