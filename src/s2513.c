/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* (c) Copyright 1995 by                                                     */
/*     SINTEF, Oslo, Norway                                                  */
/*     All rights reserved. See the copyright.h for more details.            */
/*                                                                           */
/*****************************************************************************/

#include "copyright.h"

/*
 *
 * $Id: s2513.c,v 1.6 1995-08-01 10:54:45 jka Exp $
 *
 */


#define S2513

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
   s2513(SISLSurf *surf, int ider, int normalized, double derive[], 
	 double normal[], double fundform[], int *stat)
#else
   void s2513(surf, ider, normalized, derive, normal, fundform, stat)
      SISLSurf *surf;
      int ider;
      int normalized;
      double derive[];
      double normal[];
      double fundform[];
      int *stat;
#endif
/*
***************************************************************************
*
***************************************************************************
*  PURPOSE      :  To compute the coefficients of the first and second
*                  fundamental form. 
*  INPUT        :
*     surf         - Pointer to the surface to evaluate.
*     ider         - Only implemented for ider=0 (derivative order).
*     normalized   - Flag telling if the second fundamental form
*                    coefficients shall be calculated from normalized or
*                    not normalized normals:
*                    = 0 : not normalized,
*                    = 1 : normalized,
*     derive       - Array containing derivatives from routine s1421().
*                    Size = idim*6. The sequence is:
*                    S, Su, Sv, Suu, Suv, Svv. 
*     normal       - Array containing the normal from routine s1421().
*                    Size = 3, only used for dim = 3.
*
*  OUTPUT       :
*     fundform     - Array for the fundamental form. The dimension is
*                    6*(ider + 1)*(ider + 2)/2, and the sequence is
*                    (E,F,G,e,f,g,Eu,...,Ev,...,Euu,...,Euv,...,Evv,...).
*     stat         - Status messages
*
*                         = 0 : Ok.
*                         < 0 : Error.
*
*  METHOD       :  
*
*  REFERENCES   :  Differential Geometry of Curves and Surfaces,
*                    (Manfredo P. Do Carmo, Prentice Hall,
*                      ISBN: 0-13-212589-7).
*-
*  CALLS        :
*
*  LIMITATIONS  :
*               (i) The dimension of the space in which the surface lies must
*                   be 1 or 3, if not, stat = -105 is returned.
*
*
* WRITTEN BY   :  Johannes Kaasa, SINTEF, Oslo, Norway.            Date: 1995-6
*****************************************************************************
*/
{
   double norm_scale; /* Scalation of the surface normal. */
      
   if (ider != 0) goto err178;
      
   if (surf->idim == 1) 
   {
	 
      /* 1D surface. */
	 
      if (normalized == 0)
         norm_scale = 1.;
      else
	 norm_scale = sqrt(1. + derive[1]*derive[1] + derive[2]*derive[2]);
	 
      fundform[0] = 1. + derive[1]*derive[1];
      fundform[1] = derive[1]*derive[2];
      fundform[2] = 1. + derive[2]*derive[2];
	 
      fundform[3] = derive[3]/norm_scale;
      fundform[4] = derive[4]/norm_scale;
      fundform[5] = derive[5]/norm_scale;
   }
      
   else if (surf->idim == 3) 
   {
	 
      /* 3D surface */
	 
      if (normalized == 0)
         norm_scale = 1.;
      else
	 norm_scale = sqrt(normal[0]*normal[0] + normal[1]*normal[1] +
			   normal[2]*normal[2]);
	 
      fundform[0] = s6scpr(&derive[3], &derive[3], 3);
      fundform[1] = s6scpr(&derive[3], &derive[6], 3);
      fundform[2] = s6scpr(&derive[6], &derive[6], 3);
	 
      fundform[3] = (s6scpr(normal, &derive[9], 3))/norm_scale;
      fundform[4] = (s6scpr(normal, &derive[12], 3))/norm_scale;
      fundform[5] = (s6scpr(normal, &derive[15], 3))/norm_scale;
   }
   else 
   {
      /* Not 1D or 3D surface */
	 
      goto err105;
   }
      
            
   /* Successful computations  */
      
   *stat = 0;
   goto out;
      
      
   /* Error in input, surf->idim != 1 or 3 */
err105:
   *stat = -105;
   s6err("s2513",*stat,0);
   goto out;
      
   /* Illegal derivative requested. */
err178:
   *stat = -178;
   s6err("s2513",*stat,0);
   goto out;
      
out:	 
   return;
      
}
