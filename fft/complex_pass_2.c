#include <math.h>
#include <gsl_complex.h>
#include <gsl_fft_complex.h>

#include <fft_complex.h>

int
gsl_fft_complex_pass_2 (const gsl_complex from[],
			gsl_complex to[],
			const gsl_fft_direction sign,
			const size_t product,
			const size_t n,
			const gsl_complex twiddle[])
{
  size_t i = 0, j = 0;
  size_t k, k1;

  const size_t factor = 2;
  const size_t m = n / factor;
  const size_t q = n / product;
  const size_t product_1 = product / factor;
  const size_t jump = (factor - 1) * product_1;

  for (k = 0; k < q; k++)
    {
      double w_real, w_imag;

      if (k == 0)
	{
	  w_real = 1.0;
	  w_imag = 0.0;
	}
      else
	{
	  if (sign == forward)
	    {
	      /* forward tranform */
	      w_real = twiddle[k - 1].real;
	      w_imag = twiddle[k - 1].imag;
	    }
	  else
	    {
	      /* backward tranform: w -> conjugate(w) */
	      w_real = twiddle[k - 1].real;
	      w_imag = -twiddle[k - 1].imag;
	    }
	}

      for (k1 = 0; k1 < product_1; k1++)
	{
	  gsl_complex z0, z1;
	  double x0_real, x0_imag, x1_real, x1_imag;

	  {
	    const size_t from0 = i;
	    const size_t from1 = from0 + m;
	    z0 = from[from0];
	    z1 = from[from1];
	  }

	  /* compute x = W(2) z */

	  /* x0 = z0 + z1 */
	  x0_real = z0.real + z1.real;
	  x0_imag = z0.imag + z1.imag;

	  /* x1 = z0 - z1 */
	  x1_real = z0.real - z1.real;
	  x1_imag = z0.imag - z1.imag;

	  /* apply twiddle factors */
	  {
	    const size_t to0 = j;
	    const size_t to1 = product_1 + j;

	    /* to0 = 1 * x0 */
	    to[to0].real = x0_real;
	    to[to0].imag = x0_imag;

	    /* to1 = w * x1 */
	    to[to1].real = w_real * x1_real - w_imag * x1_imag;
	    to[to1].imag = w_real * x1_imag + w_imag * x1_real;
	  }

	  i++;
	  j++;
	}
      j += jump;
    }
  return 0;
}
