#include <config.h>
#include <gsl_math.h>
#include <gsl_errno.h>
#include <gsl_multiroots.h>

int
gsl_multiroot_test_delta (const gsl_vector * dx, const gsl_vector * x, 
                     double epsabs, double epsrel)
{
  size_t i;
  int ok = 1;
  const size_t n = x->size ;

  if (epsrel < 0.0)
    {
      GSL_ERROR ("relative tolerance is negative", GSL_EBADTOL);
    }

  for (i = 0 ; i < n ; i++)
    {
      double xi = gsl_vector_get(x,i);
      double dxi = gsl_vector_get(dx,i);
      double tolerance = epsabs + epsrel * fabs(xi)  ;

      if (fabs(dxi) < tolerance)
        {
          ok = 1;
        }
      else
        {
          ok = 0;
          break;
        }
    }

  if (ok)
    return GSL_SUCCESS ;

  return GSL_CONTINUE;
}

int
gsl_multiroot_test_residual (const gsl_vector * f, double epsabs)
{
  size_t i;

  double residual = 0;

  const size_t n = f->size;

  if (epsabs < 0.0)
    {
      GSL_ERROR ("absolute tolerance is negative", GSL_EBADTOL);
    }
 
  for (i = 0 ; i < n ; i++)
    {
      double fi = gsl_vector_get(f, i);
      
      residual += fabs(fi);
    }


  if (residual < epsabs)
    {
      return GSL_SUCCESS;
    }
  
  return GSL_CONTINUE ;
}

