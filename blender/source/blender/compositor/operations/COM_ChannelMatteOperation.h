
#ifndef _COM_ChannelMatteOperation_h
#define _COM_ChannelMatteOperation_h
#include "COM_MixOperation.h"
#include "COM_NodeOperation.h"

/**
 * this program converts an input color to an output value.
 * it assumes we are in sRGB color space.
 */
class ChannelMatteOperation : public NodeOperation {
private:
	NodeChroma *m_settings;
	SocketReader *m_inputImageProgram;
	SocketReader *m_inputKeyProgram;

public:
	/**
	* Default constructor
	*/
	ChannelMatteOperation();

	/**
	* the inner loop of this program
	*/
	void executePixelSampled(float output[4], float x, float y, PixelSampler sampler);

	void initExecution();
	void deinitExecution();

	//void setKey(const float lift[4]) { copy_v3_v3(this->m_lift, lift); }
	void setSettings(NodeChroma *nodeChroma) { this->m_settings = nodeChroma; }
};
#endif

