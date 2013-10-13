/*****************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef _VC_FB_DEFS_H_
#define _VC_FB_DEFS_H_

/* FourCC code used for VCHI connection */
#define VC_FB_SERVER_NAME  MAKE_FOURCC("FBUF")

/* Maximum message length */
#define VC_FB_MAX_MSG_LEN  (sizeof(VC_FB_MSG_UNION_T) + \
	sizeof(VC_FB_MSG_HDR_T))

/* Allocation type preference */
#define VC_FB_ALLOC_INTERNAL_HEAP       (1<<0)
#define VC_FB_ALLOC_RELOC_HEAP          (1<<1)
#define VC_FB_ALLOC_ALLOW_FALLBACK      (1<<2)
#define VC_FB_ALLOC_WISH_GRANTED        (1<<3)

/* List of screens that are currently supported */
typedef enum {
	VC_FB_SCRN_LCD = 0,
	VC_FB_SCRN_HDMI,
	VC_FB_SCRN_MAX
} VC_FB_SCRN;

/* All message types supported for HOST->VC direction */
typedef enum {
	VC_FB_MSG_TYPE_GET_SCRN_INFO = 0,	/* Get screen information */
	VC_FB_MSG_TYPE_ALLOC,	/* Allocate framebuffer memory */
	VC_FB_MSG_TYPE_FREE,	/* Free framebuffer memory */
	VC_FB_MSG_TYPE_PAN,	/* Pan framebuffer */
	VC_FB_MSG_TYPE_SWAP,	/* Swap active frames in framebuffer */
	VC_FB_MSG_TYPE_CFG,	/* Configure framebuffer */
	VC_FB_MSG_TYPE_READ,	/* Read framebuffer to  */
	VC_FB_MSG_TYPE_MAX
} VC_FB_MSG_TYPE;

/* Message header for all messages in HOST->VC direction */
typedef struct {
	int32_t type;		/* Message type (VC_FB_MSG_TYPE) */
	uint8_t body[0];	/* Pointer to message body (if exists) */

} VC_FB_MSG_HDR_T;

/* Request for information for specified screen (HOST->VC) */
typedef struct {
	uint32_t scrn;		/* Screen number (VC_FB_SCRN) */

} VC_FB_GET_SCRN_INFO_T;

/* Requested information for a screen (VC->HOST) */
typedef struct {
	uint32_t width;		/* Screen width (pixels) */
	uint32_t height;	/* Screen height (pixels) */
	uint32_t bits_per_pixel;	/* Bits per pixel */

} VC_FB_SCRN_INFO_T;

/* Request to allocate memory (HOST->VC) */
typedef struct {
	/* Screen number (VC_FB_SCRN) */
	uint32_t scrn;
	/* Screen width (pixels) */
	uint32_t width;
	/* Screen height (pixels) */
	uint32_t height;
	/* 16 or 32 */
	uint32_t bits_per_pixel;
	/* Number of frames, i.e., 2 for double buffering */
	uint32_t num_frames;
	/* Layer which will be used in dispmanx */
	uint32_t layer;
	/* 0=use default_alpha for whole frame,
	 * 1=use per-pixel alpha channel */
	uint32_t alpha_per_pixel;
	/* Alpha value to use when
	 * alpha_per_pixel is 1. */
	uint32_t default_alpha;
	/* 0 = center framebuffer on screen.
	 * 1 = scale framebuffer to screen */
	uint32_t scale;
	/* 0 = (default) pads unaligned pitch.
	 * 1 = no pad added. */
	uint32_t nopad;
	/* Prefered heap allocation policy. */
	uint32_t alloc;

} VC_FB_ALLOC_T;

/* Result of a requested memory allocation (VC->HOST) */
typedef struct {
	/* Resource handle */
	uint32_t res_handle;
	/* Pointer to resource buffer */
	void *res_mem;
	/* Number of bytes to advance
	 * from one line to another */
	uint32_t line_bytes;
	/* Number of bytes to advance
	 * from one frame to another */
	uint32_t frame_bytes;

} VC_FB_ALLOC_RESULT_T;

/* Request to free a previously allocated memory (HOST->VC) */
typedef struct {
	/* Resource handle (returned from alloc) */
	uint32_t res_handle;

} VC_FB_FREE_T;

/* Request to pan a framebuffer (HOST->VC) */
typedef struct {
	/* Resource handle (returned from alloc) */
	uint32_t res_handle;
	/* Amount of y-offset in pixels
	 * starting from 0 (top) */
	uint32_t y_offset;
} VC_FB_PAN_T;

/* Request to swap active frames in a framebuffer (HOST->VC) */
typedef struct {
	/* Resource handle (returned from alloc) */
	uint32_t res_handle;
	/* Active frame to display (0-indexed) */
	uint32_t active_frame;

} VC_FB_SWAP_T;

/* Request to configure the framebuffer (HOST->VC) */
typedef struct {
	/* Resource handle (returned from alloc) */
	uint32_t res_handle;
	/* Bits per pixel (leave as 0 for unchanged) */
	uint32_t bits_per_pixel;
	/* 0=use default_alpha for whole frame,
	 * 1=use per-pixel alpha channel
	 * (needed when using 32 bpp) */
	uint32_t alpha_per_pixel;
	/* Alpha value to use when
	 * alpha_per_pixel is 1 */
	uint32_t default_alpha;

} VC_FB_CFG_T;

/* Result of a requested configuration of framebuffer (VC->HOST) */
typedef struct {
	int32_t success;
	/* Number of bytes to advance
	 * from one line to another */
	uint32_t line_bytes;
	/* Number of bytes to advance
	 * from one frame to another */
	uint32_t frame_bytes;

} VC_FB_CFG_RESULT_T;

/* Request to read screen in a framebuffer (HOST->VC)*/
typedef struct {
	uint32_t res_handle;	/* Resource handle (returned from alloc) */
	uint32_t size;		/* frame size  */

} VC_FB_READ_T;

/* Generic result for a request (VC->HOST)*/
typedef struct {
	int32_t success;	/* Success value */

} VC_FB_RESULT_T;

/* Union of ALL messages */
typedef union {
	VC_FB_GET_SCRN_INFO_T get_scrn_info;
	VC_FB_SCRN_INFO_T scrn_info;
	VC_FB_ALLOC_T alloc;
	VC_FB_ALLOC_RESULT_T alloc_result;
	VC_FB_FREE_T free;
	VC_FB_PAN_T pan;
	VC_FB_SWAP_T swap;
	VC_FB_READ_T read;
	VC_FB_RESULT_T result;
} VC_FB_MSG_UNION_T;

#endif /* _VC_FB_DEFS_H_ */
