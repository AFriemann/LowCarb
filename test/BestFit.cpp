/* BestFit.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <vector>
#include <memory>
#include <csv.h>

// local includes ==============================================================

#include "../src/DCD.hpp"
#include "../src/ProteinFile.hpp"
#include "../src/SecondaryStructureFile.hpp"
#include "../src/Frame.hpp"
#include "../src/Protein.hpp"
#include "../src/ProteinSegment.hpp"
#include "../src/FrameSegment.hpp"

#include "utils/type.hpp"
#include "utils/log.hpp"
#include "utils/definitions.hpp"

BOOST_AUTO_TEST_SUITE(best_fit)

BOOST_AUTO_TEST_CASE(bstft_complete_protein) {
    TEST_MESSAGE("bstft_complete_protein");

    DCD trajectory("test/resources/mb_ca.dcd");
    std::shared_ptr<PDB> pdb(new PDB("test/resources/integration/mb_ca.pdb"));
    Protein protein(pdb);
    std::shared_ptr<ProteinSegment> protein_segment(new ProteinSegment(protein.get_residues(), 1, 151, COMPLETE_PROTEIN));
    FrameSegment frame_segment(protein_segment);

    for(int i = 1; i <= 2000; i++) {
        Frame frame = trajectory.get_next_frame();
        frame_segment.set_frame(frame);
        Eigen::VectorXd displacement_vector = frame_segment.fit_to_reference();

        std::stringstream ss;
        ss << "test/resources/bstft/mb_ca_bstft_frame";
        ss << std::setfill('0') << std::setw(4) << i;
        ss << "_fit_hex.csv";
        io::CSVReader<3> reach_fit_data(ss.str());
        reach_fit_data.read_header(io::ignore_extra_column,"x1_hex","x2_hex","x3_hex");
        std::string x1,x2,x3;


        for (size_t k=0; reach_fit_data.read_row(x1,x2,x3); ++k) {
            BOOST_REQUIRE_CLOSE_FRACTION(cast_hex_to_double(x1), displacement_vector(3*k), PRECISION);
            BOOST_REQUIRE_CLOSE_FRACTION(cast_hex_to_double(x2), displacement_vector(3*k+1), PRECISION);
            BOOST_REQUIRE_CLOSE_FRACTION(cast_hex_to_double(x3), displacement_vector(3*k+2), PRECISION);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
