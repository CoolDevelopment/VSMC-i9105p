/*
 * Copyright (c) 2010-2012 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef VCHIQ_KONA_H
#define VCHIQ_KONA_H

#include "vchiq_bi_ipc_shared_mem.h"

#if defined(VCHIQ_SM_ALLOC_VCDDR)

#define VCHIQ_IPC_SHARED_MEM_SIZE            0x1E000  /* ARM and VC channels. */
#define VCHIQ_IPC_SHARED_MEM_EXTRA           0x1000   /* Misc pointers: clock
							 debug, gpio. */

#define VCHIQ_IPC_SHARED_MEM_SYMBOL          "vchiq_ipc_shared_mem"
#define VCHIQ_IPC_SHARED_MEM_SIZE_SYMBOL     "vchiq_ipc_shared_mem_size"

#else

#define IPC_SHARED_MEM_SLOTS_VIRT IPC_SHARED_MEM_SLOTS

#endif

#define VCHIQ_VC_SUSPEND_ITER_SYMBOL         "g_suspend_iter"
#define VCHIQ_VC_SUSP_RES_CALLBACK_SYMBOL    "g_curr_suspend_callback_fn"


#endif /* VCHIQ_KONA_H */
