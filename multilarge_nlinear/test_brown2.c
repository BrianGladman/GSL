#define brown2_N        5
#define brown2_P        5

static double brown2_x0[brown2_P] = { 0.5, 0.5, 0.5, 0.5, 0.5 };
static double brown2_epsrel = 1.0e-12;

static double brown2_J[brown2_N * brown2_P];

static void
brown2_checksol(const double x[], const double sumsq,
              const double epsrel, const char *sname,
              const char *pname)
{
  size_t i;
  double sumsq_exact;
  double alpha;
  const double p = (double) brown2_P;
  double alpha1mp, lhs, lastel;

  if (sumsq < 0.5)
    {
      /* sumsq = 0 case */

      sumsq_exact = 0.0;
      alpha = x[0];
      alpha1mp = pow(alpha, 1.0 - p);
      lhs = p*pow(alpha, p) - (p + 1)/alpha1mp;
      lastel = alpha1mp;

      gsl_test_rel(lhs, -1.0, epsrel, "%s/%s alpha lhs",
                   sname, pname);
    }
  else
    {
      /* sumsq = 1 case */

      sumsq_exact = 1.0;
      alpha = 0.0;
      lastel = p + 1.0;
    }

  gsl_test_rel(sumsq, sumsq_exact, epsrel, "%s/%s sumsq",
               sname, pname);

  for (i = 1; i < brown2_P - 1; ++i)
    {
      gsl_test_rel(x[i], alpha, epsrel, "%s/%s i=%zu",
                   sname, pname, i);
    }

  gsl_test_rel(x[brown2_P - 1], lastel, epsrel, "%s/%s last element",
               sname, pname);
}

static int
brown2_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  size_t i;
  double sum = -(brown2_N + 1.0);
  double prod = 1.0;

  for (i = 0; i < brown2_N; ++i)
    {
      double xi = gsl_vector_get(x, i);
      sum += xi;
      prod *= xi;
    }

  for (i = 0; i < brown2_N - 1; ++i)
    {
      double xi = gsl_vector_get(x, i);
      gsl_vector_set(f, i, xi + sum);
    }

  gsl_vector_set(f, brown2_N - 1, prod - 1.0);

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static int
brown2_df (CBLAS_TRANSPOSE_t TransJ, const gsl_vector * x,
           const gsl_vector * u, void * params, gsl_vector * v,
           gsl_matrix * JTJ)
{
  gsl_matrix_view J = gsl_matrix_view_array(brown2_J, brown2_N, brown2_P);
  size_t i, j;

  for (j = 0; j < brown2_P; ++j)
    {
      double prod = 1.0;

      for (i = 0; i < brown2_N - 1; i++)
        {
          double Jij = (i == j) ? 2.0 : 1.0;
          gsl_matrix_set(&J.matrix, i, j, Jij);
        }

      for (i = 0; i < brown2_N; i++)
        {
          if (i != j)
            prod *= gsl_vector_get(x, i);
        }

      gsl_matrix_set(&J.matrix, brown2_N - 1, j, prod);
    }

  if (v)
    gsl_blas_dgemv(TransJ, 1.0, &J.matrix, u, 0.0, v);

  if (JTJ)
    gsl_blas_dsyrk(CblasLower, CblasTrans, 1.0, &J.matrix, 0.0, JTJ);

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static int
brown2_fvv (const gsl_vector * x, const gsl_vector * v,
            void *params, gsl_vector * fvv)
{
  size_t i, j, k;
  double sum = 0.0;

  gsl_vector_set_zero(fvv);

  for (k = 0; k < brown2_P; ++k)
    {
      double vk = gsl_vector_get(v, k);

      for (i = 0; i < brown2_P; ++i)
        {
          double vi = gsl_vector_get(v, i);
          double delta = (i == k) ? 1.0 : 0.0;
          double prod = 1.0;

          for (j = 0; j < brown2_N; ++j)
            {
              if (j != i && j != k)
                {
                  double xj = gsl_vector_get(x, j);
                  prod *= xj;
                }
            }

          sum += vk * vi * (1.0 - delta) * prod;
        }
    }

  gsl_vector_set(fvv, brown2_N - 1, sum);

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static gsl_multilarge_nlinear_fdf brown2_func =
{
  brown2_f,
  brown2_df,
  brown2_fvv,
  brown2_N,
  brown2_P,
  NULL,
  0,
  0,
  0,
  0
};

static test_fdf_problem brown2_problem =
{
  "brown_almost_linear",
  brown2_x0,
  NULL,
  &brown2_epsrel,
  &brown2_checksol,
  &brown2_func
};
