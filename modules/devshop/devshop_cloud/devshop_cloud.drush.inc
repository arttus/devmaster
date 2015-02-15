<?php

/**
 * Implements hook_hosting_site_context_options()
 *
 * Runs on verify task. Passes data to the drush alias.
 * Save environment name, project name, and git ref to site aliases.
 */
function devshop_cloud_hosting_server_context_options(&$task) {
  if (isset($task->ref->services['provider'])) {
    $task->context_options['provider'] = $task->ref->services['provider']->type;
    $task->context_options['provider_options'] = $task->ref->services['provider']->provider_options;
    $task->context_options['provider_server_identifier'] = $task->ref->services['provider']->provider_server_identifier;
  }
}


/**
 * Implementation of hook_post_hosting_TASK_TYPE_task() for delete tasks.
 */
function devshop_cloud_post_hosting_verify_task($task, $data) {

  // When a project is deleted...
  if ($task->ref->type == 'server') {
    drush_log('[DEVSHOP] Server Verified... Saving info to front-end.', 'ok');

    variable_set("devshop_cloud_server_id_{$task->ref->nid}", d($task->ref->hosting_name)->provider_server_identifier);
    variable_set("devshop_cloud_server_data_{$task->ref->nid}", d($task->ref->hosting_name)->provider_data);
  }
}