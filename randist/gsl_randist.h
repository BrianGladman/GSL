#ifndef GSL_RANDIST_H
#define GSL_RANDIST_H
#include <gsl_rng.h>

unsigned int gsl_ran_bernoulli (const gsl_rng * r, double p);
double gsl_ran_bernoulli_pdf (unsigned int n, double p);

double gsl_ran_beta (const gsl_rng * r, double a, double b);
double gsl_ran_beta_pdf (double x, double a, double b);

unsigned int gsl_ran_binomial (const gsl_rng * r, double p, unsigned int t);
double gsl_ran_binomial_pdf (unsigned int n, double p, unsigned int t);

double gsl_ran_exponential (const gsl_rng * r, double mu);
double gsl_ran_exponential_pdf (double x, double mu);

double gsl_ran_exppow (const gsl_rng * r, double mu, double a);
double gsl_ran_exppow_pdf (double x, double mu, double a);

double gsl_ran_cauchy (const gsl_rng * r, double mu);
double gsl_ran_cauchy_pdf (double x, double mu);

double gsl_ran_chisq (const gsl_rng * r, double nu);
double gsl_ran_chisq_pdf (double x, double nu);

double gsl_ran_erlang (const gsl_rng * r, double mu, double n);
double gsl_ran_erlang_pdf (double x, double mu, double n);

double gsl_ran_fdist (const gsl_rng * r, double nu1, double nu2);
double gsl_ran_fdist_pdf (double x, double nu1, double nu2);

double gsl_ran_flat (const gsl_rng * r, double a, double b);
double gsl_ran_flat_pdf (double x, double a, double b);

double gsl_ran_gamma (const gsl_rng * r, double a, double b);
double gsl_ran_gamma_int (const gsl_rng * r, unsigned int a);
double gsl_ran_gamma_pdf (double x, double a, double b);

double gsl_ran_gaussian (const gsl_rng * r, double sigma);
double gsl_ran_gaussian_pdf (double x, double sigma);

double gsl_ran_ugaussian (const gsl_rng * r);
double gsl_ran_ugaussian_pdf (double x);

void gsl_ran_bivariate_gaussian (const gsl_rng * r, double sigma_x, double sigma_y, double rho, double *x, double *y);
double gsl_ran_bivariate_gaussian_pdf (double x, double y, double sigma_x, double sigma_y, double rho);

unsigned int gsl_ran_geometric (const gsl_rng * r, double p);
double gsl_ran_geometric_pdf (unsigned int n, double p);

unsigned int gsl_ran_hypergeometric (const gsl_rng * r, unsigned int n1, unsigned int n2, unsigned int t);
double gsl_ran_hypergeometric_pdf (unsigned int n, unsigned int n1, unsigned int n2, unsigned int t);

double gsl_ran_gumbel1 (const gsl_rng * r, double a, double b);
double gsl_ran_gumbel1_pdf (double x, double a, double b);

double gsl_ran_gumbel2 (const gsl_rng * r, double a, double b);
double gsl_ran_gumbel2_pdf (double x, double a, double b);

double gsl_ran_logistic (const gsl_rng * r, double mu);
double gsl_ran_logistic_pdf (double x, double mu);

double gsl_ran_lognormal (const gsl_rng * r);
double gsl_ran_lognormal_pdf (double x);

unsigned int gsl_ran_negative_binomial (const gsl_rng * r, double p, double t);
double gsl_ran_negative_binomial_pdf (unsigned int n, double p, double t);

unsigned int gsl_ran_pascal (const gsl_rng * r, double p, unsigned int n);
double gsl_ran_pascal_pdf (unsigned int k, double p, unsigned int n);

double gsl_ran_pareto (const gsl_rng * r, double mu, double a);
double gsl_ran_pareto_pdf (double x, double mu, double a);

unsigned int gsl_ran_poisson (const gsl_rng * r, double mu);
void gsl_ran_poisson_array (const gsl_rng * r, size_t n, unsigned int array[],
			    double mu);
double gsl_ran_poisson_pdf (unsigned int n, double mu);

double gsl_ran_tdist (const gsl_rng * r, double nu);
double gsl_ran_tdist_pdf (double x, double nu);

double gsl_ran_laplace (const gsl_rng * r, double mu);
double gsl_ran_laplace_pdf (double x, double mu);

double gsl_ran_levy (const gsl_rng * r, double mu, double a);

double gsl_ran_weibull (const gsl_rng * r, double mu, double a);
double gsl_ran_weibull_pdf (double x, double mu, double a);

void gsl_ran_dir_2d (const gsl_rng * r, double * x, double * y);
void gsl_ran_dir_3d (const gsl_rng * r, double * x, double * y, double * z);

void gsl_ran_shuffle (const gsl_rng * r, void * base, size_t nmembm, size_t size);
void * gsl_ran_choose (const gsl_rng * r, void * dest, size_t k, void * src, size_t n, size_t size) ;

#endif /* GSL_RANDIST_H */
