/**
 * @file   ProteinSegment.hpp
 * @author see AUTHORS
 * @brief  ProteinSegment header file.
 */

#ifndef PROTEINSEGMENT_HPP
#define PROTEINSEGMENT_HPP

#include <Eigen/Dense>
#include <sstream>
#include <iomanip>
#include <plog/Log.h>

#include "AtomGroup.hpp"
#include "Averager.hpp"
#include "StructureType.hpp"
#include "Residuum.hpp"

/**
 * @class ProteinSegment
 */
class ProteinSegment : public AtomGroup {
 public:
     /**
      * @param residues
      * @param start_residuum_nr
      * @param end_residuum_nr
      * @param type
      */
    ProteinSegment(const std::vector<Residuum> & residues,
                   const int start_residuum_nr,
                   const int end_residuum_nr,
                   StructureType type
                   );

    virtual ~ProteinSegment() {}

    /**
     * @return number of first residuum
     */
    int get_start_residuum_nr() const;

    /**
     * @return force constant matrix
     */
    Eigen::MatrixXd force_constant() const;

    /**
     * @return displacement vector
     */
    virtual Eigen::VectorXd displacement_vector() const;

    /**
     * @return mean square fluctuation
     */
    virtual Eigen::VectorXd mean_square_fluctuation() const;

    /**
     * @param force_constant
     */
    void add_force_constant(const Eigen::MatrixXd & force_constant);

    /**
     * @param displacement_vector
     */
    virtual void add_displacement_vector(const Eigen::VectorXd & displacement_vector);

    /**
     * @ param mean square fluctuation
     */
    virtual void add_mean_square_fluctuation(const Eigen::MatrixXd & mean_square_fluctuation);

    /**
     * @return the type of the segment
     */
    std::string get_type_as_string() const;

    /**
     * @return the type of the segment
     */
    StructureType get_type() const;

    /**
    * @return the average covariance matrix
    */
    virtual Eigen::MatrixXd get_average_covariance_matrix() const;

 private:
    Averager<Eigen::VectorXd> displacement_vector_averager;
    Averager<Eigen::MatrixXd> force_constant_averager;
    Averager<Eigen::VectorXd> mean_square_fluctuation_averager;
    Averager<Eigen::MatrixXd> covariance_averager;
    int start_residuum_nr;
    int end_residuum_nr;
    StructureType type;
};


#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
