/**
 * @file   ModelReduction.cpp
 * @author see AUTHORS
 * @brief  ModelReduction definitions file.
 */

#include "ModelReduction.hpp"

ModelReduction::ModelReduction() {/* no reduction file */}

ModelReduction::ModelReduction(const std::shared_ptr<ReductionFile> & reduction_file)
    : reduction(reduction_file) {}

size_t ModelReduction::get_selection_size() const {
    if (this->reduction) {
        return this->reduction->get_selection().size();
    }

    return 0;
}

int ModelReduction::get_selection_at(int i) const {
    if (this->reduction) {
        return this->reduction->get_selection().at(i);
    }

    throw std::runtime_error("called ModelReduction::get_selection_at with uninitialized reduction.");
}

Eigen::MatrixXd ModelReduction::reduce_hessian_matrix(const Eigen::MatrixXd & hessian_matrix) const {
    if (this->reduction) {
        std::vector<int> selection = this->reduction->get_selection();

        if (selection.size() > 0) {
            LOGI << "reducing hessian matrix";

            assert(hessian_matrix.rows() == hessian_matrix.cols());
            assert(hessian_matrix.rows() >= selection.size());

            // create not_selected
            std::vector<int> not_selected;

            size_t num_sel = selection.size();
            size_t it_sel = 0;

            for (size_t i=0; i<hessian_matrix.rows() / 3; ++i)
            {
                if ( it_sel >= num_sel || selection[it_sel] != i + 1)
                    not_selected.push_back(i+1);
                else
                    ++it_sel;
            }

            size_t num_not_sel = not_selected.size();

            assert(hessian_matrix.rows() / 3 == num_sel + num_not_sel);

            // allocate space for submatrices
            Eigen::MatrixXd sub_aa(3 * num_sel, 3 * num_sel);
            Eigen::MatrixXd sub_ab(3 * num_sel, 3 * num_not_sel);
            Eigen::MatrixXd sub_bb(3 * num_not_sel, 3 * num_not_sel);
            Eigen::MatrixXd sub_ba(3 * num_not_sel, 3 * num_sel);

            // fill aa
            for (size_t i=0; i<num_sel; ++i)
                for (size_t j=0; j<num_sel; ++j)
                    sub_aa.block<3,3>(3 * i, 3 * j) = hessian_matrix.block<3,3>(3 * (selection[i]-1),
                                                                                3 * (selection[j]-1));

            // fill bb
            for (size_t i=0; i<num_not_sel; ++i)
                for (size_t j=0; j<num_not_sel; ++j)
                    sub_bb.block<3,3>(3 * i, 3 * j) = hessian_matrix.block<3,3>(3 * (not_selected[i]-1),
                                                                                3 * (not_selected[j]-1));

            // fill ab
            for (size_t i=0; i<num_sel; ++i)
                for (size_t j=0; j<num_not_sel; ++j)
                    sub_ab.block<3,3>(3 * i, 3 * j) = hessian_matrix.block<3,3>(3 * (selection[i]-1),
                                                                           3 * (not_selected[j]-1));

            // fill ba
            for (size_t i=0; i<num_not_sel; ++i)
                for (size_t j=0; j<num_sel; ++j)
                    sub_ba.block<3,3>(3 * i, 3 * j) = hessian_matrix.block<3,3>(3 * (not_selected[i]-1),
                                                                                3 * (selection[j]-1));

            // calculate the reduced hessian
            return (sub_aa - sub_ab * sub_bb.inverse() * sub_ba);
        }
    }

    return hessian_matrix;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
