/* Copyright 2013-present Facebook, Inc.
 * Licensed under the Apache License, Version 2.0 */

#include "watchman.h"

/* watch-del /root
 * Stops watching the specified root */
void cmd_watch_delete(struct watchman_client *client, json_t *args)
{
  w_root_t *root;
  json_t *resp;

  /* resolve the root */
  if (json_array_size(args) != 2) {
    send_error_response(client, "wrong number of arguments to 'watch'");
    return;
  }

  root = resolve_root_or_err(client, args, 1, false);
  if (!root) {
    return;
  }

  resp = make_response();
  set_prop(resp, "watch-del", json_boolean(w_root_stop_watch(root)));
  set_prop(resp, "root", json_string_nocheck(root->root_path->buf));
  send_and_dispose_response(client, resp);
  w_root_delref(root);
}


/* watch-list
 * Returns a list of watched roots */
void cmd_watch_list(struct watchman_client *client, json_t *args)
{
  json_t *resp;
  json_t *root_paths;
  unused_parameter(args);

  resp = make_response();
  root_paths = w_root_watch_list_to_json();
  set_prop(resp, "roots", root_paths);
  send_and_dispose_response(client, resp);
}

/* watch /root */
void cmd_watch(struct watchman_client *client, json_t *args)
{
  w_root_t *root;
  json_t *resp;

  /* resolve the root */
  if (json_array_size(args) != 2) {
    send_error_response(client, "wrong number of arguments to 'watch'");
    return;
  }

  root = resolve_root_or_err(client, args, 1, true);
  if (!root) {
    return;
  }

  resp = make_response();
  set_prop(resp, "watch", json_string_nocheck(root->root_path->buf));
  send_and_dispose_response(client, resp);
  w_root_delref(root);
}


/* vim:ts=2:sw=2:et:
 */

