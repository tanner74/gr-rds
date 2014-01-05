/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rds/freq_divider.h>
#include <gnuradio/io_signature.h>

using namespace gr::rds;

/*
 * Create a new instance of freq_divider and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
freq_divider::sptr
freq_divider::make (int divider)
{
	return gnuradio::get_initial_sptr(new freq_divider(divider));
}

freq_divider::freq_divider (int divider)
	: gr::sync_block ("gr_rds_freq_divider",
		gr::io_signature::make(1, 1, sizeof (float)),
		gr::io_signature::make(1, 1, sizeof (float)))
{
	d_divider = 0;
	DIVIDER = divider;
	d_sign_last = d_sign_current = false;
	d_out = 1;
}

int 
freq_divider::work (int noutput_items,
		gr_vector_const_void_star &input_items,
	        gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	float *out = (float *) output_items[0];

	for (int i = 0; i < noutput_items; i++){
		d_sign_current = (in[i] > 0 ? true : false);
		if(d_sign_current != d_sign_last) {		// A zero cross
			if(++d_divider ==  DIVIDER) {
				d_out *= -1;
				d_divider = 0;
			}
		}
		out[i] = d_out;
		d_sign_last = d_sign_current;
	}

	return noutput_items;
}
