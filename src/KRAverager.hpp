#ifndef KRAVERAGER_H
#define KRAVERAGER_H

#include <iostream>

#include <Eigen/Dense>
#include <math.h>

/**
 * @class KRAverager
 */
class KRAverager {
    public:
        KRAverager();

        /**
         * @param threshold
         */
        KRAverager(int threshold);

        /**
         * @param force_constant
         * @param range
         */
        virtual void add_force_constant_tuple(double force_constant, double range);

        /**
         * @param force_constants
         * @param ranges
         */
        virtual void add_force_constant_vector(const Eigen::VectorXd &force_constants, const Eigen::VectorXd &ranges);

        /**
         * @return the average range
         */
        double get_average_range() const;

        /**
         * @return the average force constant
         */
        double get_average_force_constant() const;

        /**
         * @return first error
         */
        double get_error_1() const;

        /**
         * @return second error
         */
        double get_error_2() const;
    protected:
        int threshold;
        bool use_threshold = false;
        double r_sum = 0;
        double k_sum = 0;
        double error_sum = 0;
        long count = 0;
};

/**
 * @class KRAveragerCis
 */
class KRAveragerCis : public KRAverager {
    public:
        KRAveragerCis() : KRAverager() {};
        KRAveragerCis(int threshhold) : KRAverager(threshhold) {};

        /**
         * @param force_constant
         * @param range
         */
        void add_force_constant_tuple(double k, double r);

        /**
         * @param force_constants
         * @param ranges
         */
        void add_force_constant_vector(const Eigen::VectorXd & ks, const Eigen::VectorXd & rs);

        /**
         * @return range cis
         */
        double get_range_cis() const;

        /**
         * @return force constant cis
         */
        double get_force_constant_cis() const;
    private:
        double cis_count = 0;
        double r_cis_sum = 0;
        double k_cis_sum = 0;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
