#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <gsl_complex.h>
#include <gsl_errno.h>
#include <gsl_dft_complex.h>
#include <gsl_fft_complex.h>
#include <gsl_fft_real.h>
#include <gsl_fft_halfcomplex.h>
#include <gsl_fft_signals.h>

#include <gsl_test.h>
#include <compare.h>

int verbose = 0;

size_t tests = 0;
size_t passed = 0;
size_t failed = 0;

int
main (int argc, char *argv[])
{
  int status, factor_sum;
  size_t i, start, end, n;
  gsl_complex *complex_data, *complex_tmp;
  double rms, total;

  gsl_fft_complex_wavetable complex_wavetable;

  if (argc == 2)
    {
      start = strtol (argv[1], NULL, 0);
      end = start + 1;
    }
  else
    {
      start = 1 ;
      end = 1000 ;
    }

  for (n = start; n < end; n++)
    {

      complex_data = malloc (n * sizeof (gsl_complex));
      complex_tmp = malloc (n * sizeof (gsl_complex));
      /*    fft_complex_data = malloc (n * sizeof (gsl_complex));
         fft_complex_tmp = malloc (n * sizeof (gsl_complex)); */

      status = gsl_fft_complex_wavetable_alloc (n, &complex_wavetable);
      status = gsl_fft_complex_init (n, &complex_wavetable);
      status = gsl_fft_complex_generate_wavetable (n, &complex_wavetable);

      for (i = 0; i < n; i++)
	{
	  complex_data[i].real = ((double) rand ()) / RAND_MAX;
	  complex_data[i].imag = ((double) rand ()) / RAND_MAX;
	}

      memcpy (complex_tmp, complex_data, n * sizeof (gsl_complex));
      gsl_fft_complex_forward (complex_data, n, &complex_wavetable);
      gsl_fft_complex_inverse (complex_data, n, &complex_wavetable);

      total = 0.0;
      for (i = 0; i < n; i++)
	{
	  double dr = complex_data[i].real - complex_tmp[i].real;
	  double di = complex_data[i].imag - complex_tmp[i].imag;
	  total += dr * dr + di * di;
	}

      rms = sqrt (total / n);

      factor_sum = 0;
      for (i = 0; i < complex_wavetable.nf; i++)
	{
	  int j = complex_wavetable.factor[i];
	  factor_sum += j;
	}

      printf ("n = %d factor_sum = %d rms = %e\n", n, factor_sum, rms);

      free (complex_data);
      free (complex_tmp);

    }

  return 0;
}


