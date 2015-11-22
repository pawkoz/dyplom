
#include "COM_ChannelMatteOperation.h"
#include "BLI_math.h"

ChannelMatteOperation::ChannelMatteOperation() : NodeOperation()
{
	addInputSocket(COM_DT_COLOR);
	addInputSocket(COM_DT_COLOR);
	//addInputSocket(COM_DT_COLOR);
	//addInputSocket(COM_DT_COLOR);
	//addInputSocket(COM_DT_COLOR);
	addOutputSocket(COM_DT_VALUE);

	this->m_inputImageProgram = NULL;
	this->m_inputKeyProgram = NULL;
	//this->m_inputKeyProgram_2 = NULL;
	//this->m_inputKeyProgram_3 = NULL;
	//this->m_inputKeyProgram_4 = NULL;
}

void ChannelMatteOperation::initExecution()
{
	this->m_inputImageProgram = this->getInputSocketReader(0);
	this->m_inputKeyProgram = this->getInputSocketReader(1);
	//this->m_inputKeyProgram_2 = this->getInputSocketReader(2);
	//this->m_inputKeyProgram_3 = this->getInputSocketReader(3);
	//this->m_inputKeyProgram_4 = this->getInputSocketReader(4);
}

void ChannelMatteOperation::deinitExecution()
{
	this->m_inputImageProgram = NULL;
	this->m_inputKeyProgram = NULL;
	//this->m_inputKeyProgram_2 = NULL;
	//this->m_inputKeyProgram_3 = NULL;
	//this->m_inputKeyProgram_4 = NULL;
}

void ChannelMatteOperation::executePixelSampled(float output[4], float x, float y, PixelSampler sampler)
{
	float inColor[4];
	float inKey[4];
	//float inKey_1[4], inKey_2[4], inKey_3[4], inKey_4[4];


	const float hue = this->m_settings->t1;
	const float sat = this->m_settings->t2;
	const float val = this->m_settings->t3;

	float h_wrap;

	this->m_inputImageProgram->readSampled(inColor, x, y, sampler);
	this->m_inputKeyProgram->readSampled(inKey, x, y, sampler);



	if (
	     (fabsf(inColor[1] - inKey[1]) < sat) &&
	     (fabsf(inColor[2] - inKey[2]) < val) &&
	     ((h_wrap = 2.f * fabsf(inColor[0] - inKey[0])) < hue || (2.f - h_wrap) < hue)
	    )
	{
		output[0] = 0.0f;
	}

	else { 
		output[0] = inColor[3]; 
	}
}
