
#include "node_composite_util.h"

static bNodeSocketTemplate cmp_node_channel_in[] = {
	{SOCK_RGBA, 1, N_("Key Color"), 1.0f, 1.0f, 1.0f, 1.0f},
	{SOCK_RGBA, 1, N_("Image"), 1.0f, 1.0f, 1.0f, 1.0f},
	{-1, 0, ""}
};

static bNodeSocketTemplate cmp_node_channel_out[] = {
	{SOCK_RGBA, 0, N_("Image")},
	{SOCK_FLOAT, 0, N_("Matte")},
	{-1, 0, ""}
};

static void node_composit_init_channel_matte(bNodeTree *UNUSED(ntree), bNode *node)
{
	NodeChroma *c = MEM_callocN(sizeof(NodeChroma), "node pawel");
	node->storage = c;
	c->t1 = 0.01f;
	c->t2 = 0.1f;
	c->t3 = 0.1f;
	c->fsize = 0.0f;
	c->fstrength = 1.0f;
}

void register_node_type_cmp_channel_matte(void)
{
	static bNodeType ntype;

	cmp_node_type_base(&ntype, CMP_NODE_CHANNEL_MATTE, "Pawel Key", NODE_CLASS_MATTE, NODE_PREVIEW);
	node_type_socket_templates(&ntype, cmp_node_channel_in, cmp_node_channel_out);
	node_type_init(&ntype, node_composit_init_channel_matte);
	node_type_storage(&ntype, "NodeChroma", node_free_standard_storage, node_copy_standard_storage);

	nodeRegisterType(&ntype);
}
