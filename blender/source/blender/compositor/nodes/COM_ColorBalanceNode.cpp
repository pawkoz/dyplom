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

#include "COM_ColorBalanceNode.h"
#include "COM_ColorBalanceLGGOperation.h"
#include "COM_ColorBalanceASCCDLOperation.h"
#include "COM_ExecutionSystem.h"
#include "BKE_node.h"
#include "COM_MixOperation.h"
#include "COM_ConvertOperation.h"
#include "COM_SetAlphaOperation.h"

ColorBalanceNode::ColorBalanceNode(bNode *editorNode) : Node(editorNode)
{
	/* pass */
}

void ColorBalanceNode::convertToOperations(NodeConverter &converter, const CompositorContext &/*context*/) const
{
	

	
	

	bNode *editorsnode = getbNode();
	NodeColorBalance *n = (NodeColorBalance *)editorsnode->storage;

	NodeInput *inputSocketImage = this->getInputSocket(0);
	NodeInput *inputSocketKey = this->getInputSocket(1);
	NodeOutput *outputSocketImage = this->getOutputSocket(0);
	NodeOutput *outputSocketMatte = this->getOutputSocket(1);
	
	ConvertRGBToHSVOperation *operationRGBToHSV_Image = new ConvertRGBToHSVOperation();
	ConvertRGBToHSVOperation *operationRGBToHSV_Key = new ConvertRGBToHSVOperation();
	converter.addOperation(operationRGBToHSV_Image);
	converter.addOperation(operationRGBToHSV_Key);
	
	NodeOperation *operation;
	ColorBalanceLGGOperation *operationLGG = new ColorBalanceLGGOperation();
	operationLGG->setGain(n->gain);
	operation = operationLGG;
	converter.addOperation(operation);
	
	SetAlphaOperation *operationAlpha = new SetAlphaOperation();
	converter.addOperation(operationAlpha);
	
	converter.mapInputSocket(inputSocketImage, operationRGBToHSV_Image->getInputSocket(0));
	converter.mapInputSocket(inputSocketKey, operationRGBToHSV_Key->getInputSocket(0));
	converter.addLink(operationRGBToHSV_Image->getOutputSocket(), operation->getInputSocket(0));
	converter.addLink(operationRGBToHSV_Key->getOutputSocket(), operation->getInputSocket(1));
	converter.mapOutputSocket(outputSocketMatte, operation->getOutputSocket(0));
	
	converter.mapInputSocket(inputSocketImage, operationAlpha->getInputSocket(0));
	converter.addLink(operation->getOutputSocket(), operationAlpha->getInputSocket(1));
	converter.mapOutputSocket(outputSocketImage, operationAlpha->getOutputSocket());
	
	converter.addPreview(operationAlpha->getOutputSocket());

}
