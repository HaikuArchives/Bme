/*****************************************************************
 * Copyright (c) 2005 Tim de Jong								 *
 *							       								 *
 * All rights reserved.											 *
 * Distributed under the terms of the MIT License.               *
 *****************************************************************/
#ifndef LOCALE_MACRO_H
#define LOCALE_MACRO_H

/**	This macro uses Zeta locale kit when the source is compiled for Zeta, 
	returns its parameter in R5. This parameter is the key, or english version
	of the string, which will be used to lookup the translation in another 
	language, when in Zeta.
 */
#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(str) (str)
#endif

#endif
