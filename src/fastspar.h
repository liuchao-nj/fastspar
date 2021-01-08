#ifndef __FASTSPAR_H__
#define __FASTSPAR_H__


#include <cmath>
#include <time.h>


#include "common.h"
#if defined(FASTSPAR_CPACKAGE)
    #include "fastspar_opts.h"
#endif


struct FastSpar {
    // Some algorithm parameters
    unsigned int iterations;
    unsigned int exclusion_iterations;
    float exclusion_threshold;

    // RNG
    int seed;
    std::vector<gsl_rng *> p_rngs;

    // Threads to use during iteration
    unsigned int threads;

    // Filenames
    std::string otu_filename;
    std::string correlation_output_filename;
    std::string covariance_output_filename;

    // OTU count matrix
    const OtuTable *otu_table;

    // List of each correlation and covariance matrix calculated during iterations
    std::vector<arma::Mat<float>> correlation_vector;
    std::vector<arma::Col<float>> covariance_vector;

    // The median correlation covariance of all iterations
    arma::Mat<float> median_correlation;
    arma::Mat<float> median_covariance;


    // Construct FastSpar with a given otu_table and other parameters
    FastSpar(const OtuTable *_otu_table, unsigned int _iterations, unsigned int _exclusion_iterations, float _exclusion_threshold, unsigned int _threads, unsigned int _seed);

    // Infer correlation and covariance for n iterations
    void infer_correlation_and_covariance();

    // Calculate the final correlation and covariance for OTUs
    void calculate_median_correlation_and_covariance();
};


struct FastSparIteration {
    // Some variables
    const OtuTable *otu_table;
    unsigned int exclusion_iterations;
    float exclusion_threshold;

    // Estimated fractions of OTUs
    arma::Mat<float> fraction_estimates;
    // Variance of estimated OTU fractions
    arma::Mat<float> fraction_variance;

    // List of highly OTU pairs excluded in this iteration and number of components remaining
    std::vector<arma::uword> excluded_pairs;
    unsigned int components_remaining;

    // Modifier matrix (lhs in dgesv)
    arma::Mat<float> mod;
    // Basis variance vector
    arma::Col<float> basis_variance;

    // Correlation and covariance for this iteration
    arma::Mat<float> basis_correlation;
    arma::Mat<float> basis_covariance;


    // Construct FastSparIterations with a given otu_table and other parameters
    FastSparIteration(const OtuTable *_otu_table, unsigned int _exclusion_iterations, float _exclusion_threshold);

    // Estimates component fraction of all OTUs across samples using a dirichlet distribution
    void estimate_component_fractions(gsl_rng *p_rng);

    // Calculate the log ratio variance of the estimated fractions
    void calculate_fraction_log_ratio_variance();

    // Calcaulte the basis variance
    void calculate_basis_variance();

    // Calculate the basis correlation and covariance
    void calculate_correlation_and_covariance();

    // Find and exclude highly correlation OTU pairs
    void find_and_exclude_pairs(float threshold);
};


// Continue or exit prompt
bool continue_exit_prompt();


#endif
