/* Author: G. Jungman
 * RCS:    $Id$
 */
#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_errno.h>
#include "gsl_qrng.h"


gsl_qrng *
gsl_qrng_alloc (const gsl_qrng_type * T, unsigned int dimension)
{

  gsl_qrng * r = (gsl_qrng *) malloc (sizeof (gsl_qrng));

  if (r == 0)
    {
      GSL_ERROR_RETURN ("allocation failed for qrng struct",
			GSL_ENOMEM, 0);
    };

  r->dimension = dimension;
  r->state_size = T->state_size(dimension);
  r->state = malloc (r->state_size);

  if (r->state == 0)
    {
      free (r);
      GSL_ERROR_RETURN ("allocation failed for qrng state",
			GSL_ENOMEM, 0);
    };

  r->type = T;

  gsl_qrng_init (r);

  return r;
}


int
gsl_qrng_memcpy (gsl_qrng * dest, const gsl_qrng * src)
{
  if (dest->type != src->type)
    {
      GSL_ERROR ("generators must be of the same type", GSL_EINVAL);
    }

  dest->dimension = src->dimension;
  dest->state_size = src->state_size;
  memcpy (dest->state, src->state, src->state_size);

  return GSL_SUCCESS;
}


/* WHAT THE HELL ? WHAT ABOUT THE OTHER STATE INFO ??? */
gsl_qrng *
gsl_qrng_clone (const gsl_qrng * q)
{
  gsl_qrng * r = (gsl_qrng *) malloc (sizeof (gsl_qrng));

  if (r == 0)
    {
      GSL_ERROR_RETURN ("failed to allocate space for rng struct",
			GSL_ENOMEM, 0);
    };

  r->dimension = q->dimension;
  r->state_size = q->state_size;
  r->state = malloc (r->state_size);

  if (r->state == 0)
    {
      free (r);
      GSL_ERROR_RETURN ("failed to allocate space for rng state",
			GSL_ENOMEM, 0);
    };

  r->type = q->type;

  memcpy (r->state, q->state, q->state_size);

  return r;
}


int
gsl_qrng_get (const gsl_qrng * r, double * v)
{
  return (r->type->get) (r->state, v);
}


const char *
gsl_qrng_name (const gsl_qrng * r)
{
  return r->type->name;
}


size_t
gsl_qrng_size (const gsl_qrng * r)
{
  return r->state_size;
}


void *
gsl_qrng_state (const gsl_qrng * r)
{
  return r->state;
}


void
gsl_qrng_free (gsl_qrng * r)
{
  if(r != 0) {
    if(r->state != 0) free (r->state);
    free (r);
  }
}
