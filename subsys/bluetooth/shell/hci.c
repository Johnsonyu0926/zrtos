/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include <zephyr.h>
#include <shell/shell.h>
#include <misc/printk.h>

#include <bluetooth/hci.h>
#include <bluetooth/hci_vs.h>
#include <bluetooth/conn.h>

#include "../host/hci_core.h"

#if defined(CONFIG_BT_HCI_MESH_EXT)
int cmd_mesh_adv(int argc, char *argv[])
{
	struct net_buf *buf;
	int err;

	if (argc < 2) {
		return -EINVAL;
	}

	if (!strcmp(argv[1], "on")) {
		struct bt_hci_cp_mesh_advertise *cp;

		buf = bt_hci_cmd_create(BT_HCI_OP_VS_MESH,
					sizeof(struct bt_hci_cp_mesh) +
					sizeof(*cp));
		if (!buf) {
			return -ENOBUFS;
		}

		net_buf_add_u8(buf, BT_HCI_OC_MESH_ADVERTISE);

		/* TODO: fetch and fill cmdline params */
		cp = net_buf_add(buf, sizeof(*cp));
		cp->adv_slot = 0;
		cp->own_addr_type = 0x01;
		memset(&cp->random_addr, 0, sizeof(bt_addr_t));
		cp->ch_map = 0x07;
		cp->tx_power = 0;
		cp->retx_count = 0x07;
		cp->retx_interval = 0x00;
		cp->scan_delay = 0x0a;
		cp->scan_duration = 0x0064;
		cp->scan_filter = 0x00;
		cp->data_len = 0;
		memset(cp->data, 0, sizeof(cp->data));
	} else if (!strcmp(argv[1], "off")) {
		struct bt_hci_cp_mesh_advertise_cancel *cp;

		buf = bt_hci_cmd_create(BT_HCI_OP_VS_MESH,
					sizeof(struct bt_hci_cp_mesh) +
					sizeof(*cp));
		if (!buf) {
			return -ENOBUFS;
		}

		net_buf_add_u8(buf, BT_HCI_OC_MESH_ADVERTISE_CANCEL);

		cp = net_buf_add(buf, sizeof(*cp));
		cp->adv_slot = 0;
	} else {
		return -EINVAL;
	}

	err = bt_hci_cmd_send_sync(BT_HCI_OP_VS_MESH, buf, NULL);

	return err;
}
#endif /* CONFIG_BT_HCI_MESH_EXT */
