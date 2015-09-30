/**
 * @file   main.cpp
 * @author see AUTHORS
 * @brief  LowCarb main file.
 */

#include "main.hpp"

//==============================================================================
// main method
//==============================================================================

//LCOV_EXCL_START
int main(int argc, char * argv[]) {
    try {
        static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::info, &consoleAppender);

        Arguments args = parse_args(argc, argv);
        Config config = parse_config(args.paths.config.string());

        setup_logging(args.debug, args.verbose, config.logging.level);
        setup_threading(config.threading.threads, config.threading.eigen_threads, config.threading.dynamic);

        Protein protein = load_protein(config.files.protein, config.files.secondary_structure);

        Reach reach = setup_reach(config, protein);

        reach.compute_mean_square_fluctuation(protein);

        //// OUTPUT
        LOGI << "writing output";
        OutputWriter output_writer(args.paths.output);

        if (config.output.force_constants) {
            output_writer.write_force_constants(reach.get_protein_segments());
        }

        if (config.output.mean_square_fluctuation) {
            output_writer.write_mean_square_fluctuation(reach.first_segments_mean_square_fluctuation(),
                                                        reach.get_mean_square_fluctuation());
        }

        if (config.output.eigenvalues_and_eigenvectors) {
            output_writer.write_eigenvectors(reach.get_eigenvectors());
            output_writer.write_eigenvalues(reach.get_eigenvalues());
            output_writer.write_eig_and_vec(reach.get_eigenvalues(), reach.get_weighted_eigenvector());
        }

        if (config.output.average_force_constants) {
            output_writer.write_averaged_force_constants(reach.get_average_force_constant_map());
            output_writer.write_average_force_constants_for_complete_protein(reach.get_kr_complete_protein_map());
        }

        if (config.output.hessian_matrix) {
            output_writer.write_hessian_matrix(reach.get_hessian_matrix());
        }

        if (config.output.covariance_matrix) {
            output_writer.write_covariance_matrix(reach.get_covariance_matrix());
        }
    } catch (std::logic_error & e) {
        LOGF << e.what();

        return ERROR_IN_STARTUP;
    } catch (std::runtime_error & e) {
        LOGF << e.what();

        return ERROR_IN_RUNTIME;
    } catch (std::exception & e) {
        LOGF << "Unhandled exception reached the top of main: " << e.what();

        return ERROR_UNHANDLED_EXCEPTION;
    }

    return SUCCESS;
}

Arguments parse_args(const int argc, char * argv[]){
    ArgumentParser ap;
    return ap.parse(argc, const_cast<const char **>(argv));
}

Config parse_config(const std::string path) {
    ConfigParser cp;
    return cp.parse(path);
}

void setup_logging(const bool debug, const bool verbose, const int log_level) {
    if (verbose) {
        plog::get()->setMaxSeverity(plog::verbose);
    } else if (debug) {
        plog::get()->setMaxSeverity(plog::debug);
    } else {
        plog::get()->setMaxSeverity(plog::Severity(log_level));
    }
}

void setup_threading(const int threads, const boost::optional<int> eigen_threads, const bool dynamic) {
    #ifdef _OPENMP
    Eigen::initParallel();

    if (threads != 0) {
        omp_set_num_threads(threads);
    }

    if (eigen_threads) {
        Eigen::setNbThreads(*eigen_threads);
    }

    omp_set_dynamic(dynamic);
    LOGD << "OpenMP initialized with dynamic teams set to " << omp_get_dynamic();
    LOGD << "Eigen is using " << Eigen::nbThreads() << " threads";
    #endif
}

Reach setup_reach(const Config & config, const Protein & protein) {
    ModelReduction reduction = load_reduction(config.files.reduction, protein.residue_count());

    ProteinSegmentFactory protein_segment_factory;
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments = protein_segment_factory.generate_protein_segments_for_analysis(protein);

    TrajectoryAnalyzer trajectory_analyzer;

    if (config.files.nma_covariance) {
        LOGI << "setting up Reach with NMA input";

        CSVReader csv_reader;
        Eigen::MatrixXd nma_covariance = csv_reader.read_matrix(
                *config.files.nma_covariance,
                protein.residue_count() * 3);

        trajectory_analyzer.analyze(nma_covariance, protein_segments, config.general.temperature);

    } else {
        LOGI << "setting up Reach with Trajectory";
        LOGI << "loading trajectory files";

        TrajectoryFileFactory tf;
        std::vector<std::shared_ptr<TrajectoryFile>> trajectory_files;

        for (auto const & path : config.files.trajectories) {
            trajectory_files.push_back(tf.create(path, config.general.crystal_information));
        }

        Trajectory trajectory(trajectory_files);

        trajectory_analyzer.analyze(trajectory,
                                    protein_segments,
                                    config.general.temperature,
                                    config.general.ensemble_size);
    }

    Reach reach(protein_segments,
                config.general.temperature,
                config.general.ensemble_size,
                config.fitting.average_bin_length,
                config.fitting.minimum_length,
                config.fitting.maximum_length,
                config.fitting.use_slow_fitting,
                config.fitting.slow_minimum_length,
                config.fitting.slow_maximum_length,
                reduction);

    return reach;
}

Protein load_protein(const boost::filesystem::path & protein_path,
                     const boost::optional<boost::filesystem::path> & secondary_structure_path) {
    LOGI << "loading protein file";

    ProteinFileFactory pf;
    std::shared_ptr<ProteinFile> protein_file(pf.create(protein_path));
    SecondaryStructureFileFactory ssf;

    if (secondary_structure_path) {
        LOGI << "loading secondary structure file";

        std::shared_ptr<SecondaryStructureFile> secondary_structure_file(
                ssf.create(*secondary_structure_path));
        return Protein(protein_file, secondary_structure_file);
    } else {
        return Protein(protein_file);
    }
}

ModelReduction load_reduction(const boost::optional<boost::filesystem::path> & reduction_config_path,
                              const size_t & residuum_count) {
    if (reduction_config_path) {
        LOGI << "setting up reduction";

        ReductionFileFactory rf;
        return ModelReduction(rf.create(*reduction_config_path, residuum_count));
    }

    return ModelReduction();
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
