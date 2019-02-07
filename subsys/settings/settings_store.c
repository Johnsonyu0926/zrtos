/*
 * Copyright (c) 2018 Laczen
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <errno.h>

#include <settings/settings.h>
#include "settings_priv.h"

sys_slist_t  settings_load_srcs;
struct settings_store *settings_save_dst;

void settings_src_register(struct settings_store *cs)
{
	sys_snode_t *prev, *cur;

	prev = NULL;

	SYS_SLIST_FOR_EACH_NODE(&settings_load_srcs, cur) {
		prev = cur;
	}

	sys_slist_insert(&settings_load_srcs, prev, &cs->cs_next);
}

void settings_dst_register(struct settings_store *cs)
{
	settings_save_dst = cs;
}

int settings_load(void)
{
	struct settings_store *cs;

	/*
	 * for every settings source
	 *    load config
	 *    apply config
	 */
	SYS_SLIST_FOR_EACH_CONTAINER(&settings_load_srcs, cs, cs_next) {
		cs->cs_itf->csi_load(cs);
	}

	/*
	 * finally commit the settings
	 */
	return settings_commit();
}


/*
 * Append a single value to persisted config.
 */
int settings_save_one(const char *name, void *value, size_t val_len)
{
	struct settings_store *cs;

	if ((strlen(name) > (SETTINGS_MAX_NAME_LEN+SETTINGS_EXTRA_LEN-1)) ||
	    (val_len > (SETTINGS_MAX_VAL_LEN))) {
		return -EINVAL;
	}

	cs = settings_save_dst;
	if (!cs) {
		return -ENOENT;
	}
	return cs->cs_itf->csi_save(cs, name, (char *)value, val_len);
}

int settings_delete(const char *name)
{
	return settings_save_one(name, NULL, 0);
}

int settings_save(void)
{
	struct settings_store *cs;
	struct settings_handler *ch;
	int rc;
	int rc2;

	cs = settings_save_dst;
	if (!cs) {
		return -ENOENT;
	}

	if (cs->cs_itf->csi_save_start) {
		cs->cs_itf->csi_save_start(cs);
	}
	rc = 0;

	SYS_SLIST_FOR_EACH_CONTAINER(&settings_handlers, ch, node) {
		if (ch->h_export) {
			rc2 = ch->h_export(settings_save_one);
			if (!rc) {
				rc = rc2;
			}
		}
	}
	if (cs->cs_itf->csi_save_end) {
		cs->cs_itf->csi_save_end(cs);
	}
	return rc;
}

void settings_store_init(void)
{
	sys_slist_init(&settings_load_srcs);
}
