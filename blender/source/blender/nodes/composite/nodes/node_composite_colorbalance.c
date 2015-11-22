/*
 * ***** BEGIN GPL LICENSE BLOCK *****
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
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Matt Ebb.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/nodes/composite/nodes/node_composite_colorbalance.c
 *  \ingroup cmpnodes
 */

#include "node_composite_util.h"

/* ******************* Color Balance ********************************* */
static bNodeSocketTemplate cmp_node_colorbalance_in[] = {
	{SOCK_RGBA, 1, N_("Image"), 1.0f, 1.0f, 1.0f, 1.0f},
	{-1, 0, ""}
};

static bNodeSocketTemplate cmp_node_colorbalance_out[] = {
	{SOCK_RGBA, 0, N_("Image")},
	{-1, 0, ""}
};

/* Sync functions update formula parameters for other modes, such that the result is comparable.
 * Note that the results are not exactly the same due to differences in color handling (sRGB conversion happens for LGG),
 * but this keeps settings comparable.
 */


void ntreeCompositColorBalanceSyncFromLGG(bNodeTree *UNUSED(ntree), bNode *node)
{
	NodeColorBalance *n = node->storage;
	int c;

	
}

void ntreeCompositColorBalanceSyncFromCDL(bNodeTree *UNUSED(ntree), bNode *node)
{
	NodeColorBalance *n = node->storage;
	int c;
	
	for (c = 0; c < 3; ++c) {
		n->gain[c] = 1;
	}
}

static void node_composit_init_colorbalance(bNodeTree *UNUSED(ntree), bNode *node)
{
	NodeColorBalance *n = node->storage = MEM_callocN(sizeof(NodeColorBalance), "node colorbalance");

	n->gain[0] = n->gain[1] = n->gain[2] = 1.0f;

}

void register_node_type_cmp_colorbalance(void)
{
	static bNodeType ntype;

	cmp_node_type_base(&ntype, CMP_NODE_COLORBALANCE, "Color Balance", NODE_CLASS_OP_COLOR, 0);
	node_type_socket_templates(&ntype, cmp_node_colorbalance_in, cmp_node_colorbalance_out);
	node_type_size(&ntype, 400, 200, 400);
	node_type_init(&ntype, node_composit_init_colorbalance);
	node_type_storage(&ntype, "NodeColorBalance", node_free_standard_storage, node_copy_standard_storage);

	nodeRegisterType(&ntype);
}
