/**
 * @file   OutputWriter.cpp
 * @author see AUTHORS
 * @brief  OutputWriter definition file.
 */

#include "OutputWriter.hpp"

void OutputWriter::write_force_constants(const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments) {
    LOGD << "writing force constants";

    for (std::shared_ptr<ProteinSegment> protein_segment : protein_segments) {
        boost::filesystem::path csv_name("force_constant_"+ protein_segment->get_type_as_string()+".csv");

        CSVWriter csv;
        csv.open(this->path / csv_name);

        try {
            csv.write(protein_segment->force_constant());

            csv.close();
        } catch (...) {
            csv.close();
        }
    }
}

void OutputWriter::write_mean_square_fluctuation(const Eigen::VectorXd & mean_square_fluctuation,
                                                 const Eigen::VectorXd & xx_com) {
    LOGD << "writing mean square fluctuation";

    assert(mean_square_fluctuation.rows() == xx_com.rows());

    boost::filesystem::path csv_name("mean_square_fluctuation.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        std::vector<std::string> headers = {"x2_MD","x2_NMA"};
        csv.write_line(headers);

        Eigen::MatrixXd content(xx_com.rows(), 2);
        content.col(0) << mean_square_fluctuation;
        content.col(1) << xx_com;
        csv.write(content, false);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_eigenvectors(const Eigen::MatrixXd & eigenvectors) {
    LOGD << "writing eigenvectors";

    boost::filesystem::path csv_name("eigenvectors.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        csv.write(eigenvectors);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_eigenvalues(const Eigen::VectorXd & eigenvalues) {
    LOGD << "writing eigenvalues";

    boost::filesystem::path csv_name("eigenvalues.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        csv.write(eigenvalues);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_eig_and_vec(const Eigen::VectorXd & eig,
                                     const Eigen::VectorXd & vec) {
    LOGD << "writing vectors eig and vec";

    boost::filesystem::path csv_name("eig_and_vec.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        std::vector<std::string> headers = {"eig", "vec"};
        csv.write_line(headers);
        csv.write(eig, false);
        csv.write(vec, false);
        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}


void OutputWriter::write_averaged_force_constants(const std::map<std::string, double> & averaged_force_constants) {
    LOGD << "writing vectors averaged force constants";

    boost::filesystem::path csv_name("avg_force_constants.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        std::vector<std::string> header = {
            "alpha_12_k", "alpha_12_r", "alpha_12_k_error",
            "alpha_13_k", "alpha_13_r", "alpha_13_k_error",
            "alpha_14_k", "alpha_14_r", "alpha_14_k_error",
            "alpha_15_k", "alpha_15_r", "alpha_15_k_error",

            "beta_12_k", "beta_12_r", "beta_12_k_error",
            "beta_13_k", "beta_13_r", "beta_13_k_error",
            "beta_14_k", "beta_14_r", "beta_14_k_error",

            "beta_pair_k", "beta_pair_r", "beta_pair_k_error",

            "local_12_k", "local_12_r", "local_12_k_error",
            "local_13_k", "local_13_r", "local_13_k_error",
            "local_14_k", "local_14_r", "local_14_k_error",
        };

        csv.write_line(header);

        std::vector<double> line(header.size());

        for (int i = 0; i < header.size(); ++i) {
            line[i] = averaged_force_constants.find(header[i])->second;
        }

        csv.write_line(line);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_average_force_constants_for_complete_protein(
        const std::map<std::string, Eigen::VectorXd> & averaged_force_constants) {
    LOGD << "writing average force constants for complete protein";

    boost::filesystem::path csv_name("avg_force_constants_complete_protein.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);
    try {
        std::vector<std::string> headers = { "ks", "rs", "k_errors" };
        csv.write_line(headers);

        for (int row = 0; row < averaged_force_constants.find("ks")->second.size(); ++row) {
            std::vector<double> line(headers.size());

            for (int col = 0; col < headers.size(); ++col) {
                line[col] = averaged_force_constants.find(headers[col])->second(row);
            }
            csv.write_line(line);
        }

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_hessian_matrix(const Eigen::MatrixXd & hessian_matrix) {
    LOGD << "writing hessian matrix";

    boost::filesystem::path csv_name("hessian_matrix.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        csv.write(hessian_matrix);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

void OutputWriter::write_covariance_matrix(const Eigen::MatrixXd & covariance_matrix) {
    LOGD << "writing covariance matrix";

    boost::filesystem::path csv_name("covariance_matrix.csv");

    CSVWriter csv;
    csv.open(this->path / csv_name);

    try {
        csv.write(covariance_matrix, false);

        csv.close();
    } catch (...) {
        csv.close();
        throw;
    }
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
