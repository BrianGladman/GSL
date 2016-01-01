#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>

#define NEW 1

int
func_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  double x1 = gsl_vector_get(x, 0);
  double x2 = gsl_vector_get(x, 1);

#if NEW
  gsl_vector_set(f, 0, 100.0 * (x2 - x1*x1));
  gsl_vector_set(f, 1, x1);
#else
  gsl_vector_set(f, 0, 10.0 * (x2 - x1*x1));
  gsl_vector_set(f, 1, 1.0 - x1);
#endif

  return GSL_SUCCESS;
}

int
func_df (const gsl_vector * x, void *params, gsl_matrix * J)
{
  double x1 = gsl_vector_get(x, 0);

#if NEW
  gsl_matrix_set(J, 0, 0, -200.0*x1);
  gsl_matrix_set(J, 0, 1, 100.0);
  gsl_matrix_set(J, 1, 0, 1.0);
  gsl_matrix_set(J, 1, 1, 0.0);
#else
  gsl_matrix_set(J, 0, 0, -20.0*x1);
  gsl_matrix_set(J, 0, 1, 10.0);
  gsl_matrix_set(J, 1, 0, -1.0);
  gsl_matrix_set(J, 1, 1, 0.0);
#endif

  return GSL_SUCCESS;
}

int
func_fvv (const gsl_vector * x, const gsl_vector * v,
          void *params, gsl_vector * fvv)
{
  double v1 = gsl_vector_get(v, 0);

#if NEW
  gsl_vector_set(fvv, 0, -200.0 * v1 * v1);
  gsl_vector_set(fvv, 1, 0.0);
#else
  gsl_vector_set(fvv, 0, -20.0 * v1 * v1);
  gsl_vector_set(fvv, 1, 0.0);
#endif

  return GSL_SUCCESS;
}

void
callback(const size_t iter, void *params,
         const gsl_multifit_nlinear_workspace *w)
{
  gsl_vector * x = gsl_multifit_nlinear_position(w);

  /* print out current location */
  printf("%f %f\n",
         gsl_vector_get(x, 0),
         gsl_vector_get(x, 1));
}

void
solve_system(gsl_vector *x, gsl_multifit_nlinear_fdf *fdf,
             gsl_multifit_nlinear_parameters *params)
{
  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_lm;
  const size_t max_iter = 200;
  const double xtol = 1.0e-8;
  const double gtol = 1.0e-8;
  const double ftol = 1.0e-8;
  const size_t n = fdf->n;
  const size_t p = fdf->p;
  gsl_multifit_nlinear_workspace *work =
    gsl_multifit_nlinear_alloc(T, params, n, p);
  gsl_vector * f = gsl_multifit_nlinear_residual(work);
  int info;
  double chisq0, chisq;

  /* initialize solver */
  gsl_multifit_nlinear_init(fdf, x, work);

  /* store initial cost */
  gsl_blas_ddot(f, f, &chisq0);

  /* iterate until convergence */
  gsl_multifit_nlinear_driver(max_iter, xtol, gtol, ftol,
                              callback, &info, work);

  /* store final cost */
  gsl_blas_ddot(f, f, &chisq);

  /* print summary */

  fprintf(stderr, "NITER        = %zu\n", gsl_multifit_nlinear_niter(work));
  fprintf(stderr, "NFEV         = %zu\n", fdf->nevalf);
  fprintf(stderr, "NJEV         = %zu\n", fdf->nevaldf);
  fprintf(stderr, "NAEV         = %zu\n", fdf->nevalfvv);
  fprintf(stderr, "initial cost = %.12e\n", chisq0);
  fprintf(stderr, "final cost   = %.12e\n", chisq);

  printf("\n\n");

  gsl_multifit_nlinear_free(work);
}

int
main (void)
{
  const size_t n = 2;
  const size_t p = 2;
  gsl_vector *f = gsl_vector_alloc(n);
  gsl_vector *x = gsl_vector_alloc(p);
  gsl_multifit_nlinear_fdf fdf;
  gsl_multifit_nlinear_parameters fdf_params =
    gsl_multifit_nlinear_default_parameters();
  
  {
    double x1, x2, chisq;
    double *f1 = gsl_vector_ptr(f, 0);
    double *f2 = gsl_vector_ptr(f, 1);

    for (x1 = -1.2; x1 < 0.1; x1 += 0.1)
      {
        for (x2 = -0.5; x2 < 2.1; x2 += 0.1)
          {
            gsl_vector_set(x, 0, x1);
            gsl_vector_set(x, 1, x2);
            func_f(x, NULL, f);

            chisq = (*f1) * (*f1) + (*f2) * (*f2);
            printf("%f %f %f\n", x1, x2, chisq);
          }
        printf("\n");
      }
    printf("\n\n");
  }

  /* define function to be minimized */
  fdf.f = func_f;
  fdf.df = func_df;
  fdf.fvv = func_fvv;
  fdf.n = n;
  fdf.p = p;
  fdf.params = NULL;

  fdf_params.solver = GSL_MULTIFIT_NLINEAR_SOLVER_NORMAL;

  /* starting point */
#if NEW
  gsl_vector_set(x, 0, -1.0);
  gsl_vector_set(x, 1, 1.0);
#else
  gsl_vector_set(x, 0, -1.2);
  gsl_vector_set(x, 1, 1.0);
#endif

  fprintf(stderr, "=== Solving system without acceleration ===\n");
  solve_system(x, &fdf, &fdf_params);

  fprintf(stderr, "=== Solving system with acceleration ===\n");
  fdf_params.accel = 1;
  solve_system(x, &fdf, &fdf_params);

  gsl_vector_free(f);
  gsl_vector_free(x);

  return 0;
}
