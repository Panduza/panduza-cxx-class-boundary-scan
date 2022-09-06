/*
 * JTAG Core library
 * Copyright (c) 2008 - 2021 Viveris Technologies
 *
 * JTAG Core library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * JTAG Core library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with JTAG Core library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @file   drv_loader.c
 *  driver loader
 * @author Jean-François DEL NERO <Jean-Francois.DELNERO@viveris.fr>
 */

#include <stdio.h>
#include <string.h>

#include "drv_loader.hxx"

#include "../jtag_core/jtag_core_internal.hxx"
#include "../jtag_core/jtag_core.hxx"

#include "../bsdl_parser/bsdl_loader.hxx"

#include "./ftdi_jtag/ftdi_jtag_drv.hxx"


const drv_entry staticdrvs[] =
{
#if defined(__linux__)
	{(DRV_GETMODULEINFOS)drv_FTDI_libGetDrv,0},
#endif
	{(DRV_GETMODULEINFOS)-1,0}
};



int GetDrvInfo(void * jc_ctx,unsigned long infotype,void * returnvalue,const char * drv_id,const char * drv_desc,drv_ptr * drv_func)
{
	if(jc_ctx)
	{
		if(returnvalue)
		{
			switch(infotype)
			{
				case GET_DRV_ID:
					*(char**)(returnvalue)=(char*)drv_id;
					break;

				case GET_DRV_DESCRIPTION:
					strcpy(returnvalue, (char*)drv_desc);
					break;

				case GET_DRV_FUNCPTR:
					memcpy(returnvalue,drv_func,sizeof(drv_ptr));
					break;

				case GET_DRV_DETECT:
					*((int*)(returnvalue)) =  drv_func->drv_Detect(jc_ctx);
					break;

				default:
					return JTAG_CORE_BAD_PARAMETER;
					break;
			}

			return JTAG_CORE_NO_ERROR;

		}
	}
	return JTAG_CORE_BAD_PARAMETER;
}


int jtagcore_loaddriver(jtag_core * jc, int id, char * parameters)
{
	int i,ret;
	i = 0;

	while (staticdrvs[i].getinfosfunc != (DRV_GETMODULEINFOS)-1 )
	{
		i++;
	}

	if ( (id >> 8) < i)
	{
		if (jc->io_functions.drv_DeInit)
		{
			jc->io_functions.drv_DeInit(jc);
			memset(&jc->io_functions, 0, sizeof(drv_ptr));
		}

		staticdrvs[id>>8].getinfosfunc(jc, id & 0xFF, GET_DRV_FUNCPTR, &jc->io_functions);

		if( jc->io_functions.drv_Init )
		{
			ret = jc->io_functions.drv_Init(jc, id & 0xFF,0);
			if (ret < 0)
			{
				memset(&jc->io_functions, 0, sizeof(drv_ptr));
			}
			else
			{
			}
		}
		else
			goto fail;

		return ret;
	}

fail:

	return JTAG_CORE_NOT_FOUND;
};

