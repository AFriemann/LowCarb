/**
 * @file   FrameSegment.cpp
 * @author see AUTHORS
 * @brief  FrameSegment definitions file.
 */

#include "FrameSegment.hpp"

FrameSegment::FrameSegment(const std::shared_ptr<ProteinSegment> & protein_segment)
    : protein_segment(protein_segment) {

    this->atoms = protein_segment->get_atoms();
}

void FrameSegment::set_frame(const Frame & frame) {
    for (Atom & atom : this->atoms) {
        atom.set_position(frame.get_atom_positions()(atom.get_atom_number()));
    }
}

Eigen::VectorXd FrameSegment::fit_to_reference() {
    Eigen::Vector3d cen_com = get_center_of_mass();

    Eigen:: Matrix3Xd ref = this->protein_segment->get_relative_positions();
    Eigen:: Matrix3Xd com = get_relative_positions();

    Eigen::MatrixXd cov = ref * com.transpose();
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(cov, Eigen::ComputeThinU | Eigen::ComputeThinV);

    // Find the rotation
    double d = (svd.matrixV() * svd.matrixU().transpose()).determinant();

    if (d > 0) {
      d = 1.0;
    } else {
      d = -1.0;
    }

    Eigen::Matrix3d I = Eigen::Matrix3d::Identity(3, 3);
    I(2, 2) = d;
    Eigen::Matrix3d R = svd.matrixV() * I * svd.matrixU().transpose();

    Eigen::VectorXd displacement_vector(this->atoms.size()*3);

    for (size_t i = 0; i < this->atoms.size(); i++) {
        Eigen::Vector3d com_crd = this->atoms[i].get_position();
        Eigen::Vector3d dc = com_crd - cen_com;
        Eigen::Vector3d dp = R.transpose() * dc;
        displacement_vector(3 * i + 0) = dp(0);
        displacement_vector(3 * i + 1) = dp(1);
        displacement_vector(3 * i + 2) = dp(2);
    }
    return displacement_vector;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
