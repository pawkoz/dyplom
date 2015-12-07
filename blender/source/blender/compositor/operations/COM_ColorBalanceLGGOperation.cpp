/*
 * Copyright 2011, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor: 
 *		Jeroen Bakker 
 *		Monique Dewanchand
 */

#include "COM_ColorBalanceLGGOperation.h"
#include "BLI_math.h"


inline float colorbalance_lgg(float in, float lift_lgg, float gamma_inv, float gain)
{
	/* 1:1 match with the sequencer with linear/srgb conversions, the conversion isnt pretty
	 * but best keep it this way, sice testing for durian shows a similar calculation
	 * without lin/srgb conversions gives bad results (over-saturated shadows) with colors
	 * slightly below 1.0. some correction can be done but it ends up looking bad for shadows or lighter tones - campbell */
	float x = (((linearrgb_to_srgb(in) - 1.0f) * lift_lgg) + 1.0f) * gain;

	/* prevent NaN */
	if (x < 0.f) x = 0.f;

	return powf(srgb_to_linearrgb(x), gamma_inv);
}

ColorBalanceLGGOperation::ColorBalanceLGGOperation() : NodeOperation()
{
	this->addInputSocket(COM_DT_VALUE);
	this->addInputSocket(COM_DT_COLOR);
	this->addOutputSocket(COM_DT_COLOR);
	this->m_inputValueOperation = NULL;
	this->m_inputColorOperation = NULL;
	this->setResolutionInputSocketIndex(1);
}

void ColorBalanceLGGOperation::initExecution()
{
	this->m_inputValueOperation = this->getInputSocketReader(0);
	this->m_inputColorOperation = this->getInputSocketReader(1);
}

void ColorBalanceLGGOperation::executePixelSampled(float output[4], float x, float y, PixelSampler sampler)
{
	float inColor[4];
	float inKey[4];

	const float hue = 0.01f;;
	const float sat = 0.1f;
	const float val = 0.1f;

	float h_wrap;

	this->m_inputColorOperation->readSampled(inColor, x, y, sampler);
	rgb_to_hsv_v(this->m_gain, inKey);
	printf("R %f G %f B %f \n",inKey[0],inKey[1],inKey[2]);

	/* store matte(alpha) value in [0] to go with
	 * COM_SetAlphaOperation and the Value output
	 */

	if (
	    /* do hue last because it needs to wrap, and does some more checks  */

	    /* sat */ (fabsf(inColor[1] - inKey[1]) < sat) &&
	    /* val */ (fabsf(inColor[2] - inKey[2]) < val) &&

	    /* multiply by 2 because it wraps on both sides of the hue,
	     * otherwise 0.5 would key all hue's */

	    /* hue */ ((h_wrap = 2.f * fabsf(inColor[0] - inKey[0])) < hue || (2.f - h_wrap) < hue)
	    )
	{
		output[0] = 0.0f; /* make transparent */
	}

	else { /*pixel is outside key color */
		output[0] = inColor[3]; /* make pixel just as transparent as it was before */
	}
	
}

void ColorBalanceLGGOperation::deinitExecution()
{
	this->m_inputValueOperation = NULL;
	this->m_inputColorOperation = NULL;
}
