/* $Id: rvgs.h 55 2005-09-13 22:29:52Z asminer $ */
/* -------------------------------------------------------------
 * Name            : rvgs.h (header file for the library rvgs.c)
 * Author          : Steve Park & Dave Geyer
 * Language        : ANSI C
 * Latest Revision : 11-03-96
 * --------------------------------------------------------------
 */

#if !defined( _RVGS_ )
#define _RVGS_

long Bernoulli(long double p);
long Binomial(long n, long double p);
long Equilikely(long a, long b);
long Geometric(long double p);
long Pascal(long n, long double p);
long Poisson(long double m);

long double Uniform(long double a, long double b);
long double Exponential(long double m);
long double Erlang(long n, long double b);
long double Normal(long double m, long double s);
long double Lognormal(long double a, long double b);
long double Chisquare(long n);
long double Student(long n);

#endif

