<?php
/**
 * @file deploy.devshop.provision.inc
 * Contains code for the provision-devshop-deploy command.
 */

use GitWrapper\GitException;
use GitWrapper\GitWrapper;
use Symfony\Component\Process\Process;

/**
 * Pre provision-devshop-deploy
 */
function drush_devshop_provision_pre_provision_devshop_deploy($git_ref = ''){

  if (d()->type == 'site') {
    if (empty(d()->environment)) {
      return drush_set_error('DEVSHOP_FRAMEWORK_ERROR', 'This site is not a part of a project. You cannot use this command.');
    }
  }
  else {
    return drush_set_error('DEVSHOP_FRAMEWORK_ERROR', 'provision-devshop-deploy must be run on a site context.');
  }

  // Verify that the branch or tag exists
  if (empty($git_ref)) {
    return drush_set_error('DEVSHOP_FRAMEWORK_ERROR', 'You must specify a valid branch or tag.');
  }

  $project_alias = '@project_' . d()->project;
  $project = (object) d($project_alias)->project;

  if (!isset($project->settings['git']['refs'][$git_ref])) {
    $drush_command = "drush $project_alias provision-verify";
    return drush_set_error('DEVSHOP_FRAMEWORK_ERROR', "Branch or tag '$git_ref' not found. Try running '$drush_command' to fetch new remote branches or tags.");
  }
}

/**
 * Implements the provision-devshop-deploy command.
 */
function drush_devshop_provision_provision_devshop_deploy($git_ref = '')
{
  $project_name = d()->project;
  $project = (object) d("@project_{$project_name}")->project;
  $environment = (object) $project->environments[d()->environment];
  $desired_ref_type = $project->settings['git']['refs'][$git_ref];

    drush_log('[Current Working Directory]' . $environment->repo_path, 'devshop_log');

    // Stash any changes? Not sure if we should do this anymore...
    // $git->command('stash');

    // Fetch
    devshop_drush_process("git fetch --all", $environment->repo_path, dt('DevShop Deploy'));

    // Checkout the chosen ref
    $git_checkout_output = devshop_drush_process("git checkout {$git_ref}", $environment->repo_path, dt('DevShop Deploy'));
    $git_checkout_output_lines = explode("\n", $git_checkout_output);

    // Run Git Pull
    if ($desired_ref_type == 'branch') {

      $git_pull_output = devshop_drush_process("git pull", $environment->repo_path, dt('DevShop Deploy'));
      devshop_drush_process("git submodule update --init --recursive --force ", $environment->repo_path, dt('DevShop Deploy'));
    }

    devshop_drush_process("git status", $environment->repo_path, dt('DevShop Deploy'));
}

/**
 * Post provision-devshop-deploy
 */
function drush_devshop_provision_post_provision_devshop_deploy($git_ref = '') {

  // Get post deploy options
  $revert = drush_get_option('revert');
  $update = drush_get_option('update');
  $cache = drush_get_option('cache');
  $composer = drush_get_option('composer', TRUE);

  $project_name = d()->project;
  $project = (object) d("@project_{$project_name}")->project;
  $environment = (object) $project->environments[d()->environment];
  $desired_ref_type = $project->settings['git']['refs'][$git_ref];

  // Ensure drush_alias exists. Not sure why it was missing for me.
  if (empty($environment->drush_alias)) {
    $environment->drush_alias = d()->name;
  }

  $commands = array();

  drush_log("[{$project_name}] {$environment->name}: " . dt('Running deploy hooks.'), 'notice');

  // Built in Hooks
  if ($composer) {
    if (file_exists($environment->repo_path . '/composer.json')) {
      $commands[] = "composer install";
    }
    elseif (file_exists($environment->root . '/composer.json')) {
      $commands[] = "cd {$environment->root} && composer install";
    }
  }
  else {
    drush_log(dt('[DEVSHOP] Skipped running composer install...'), 'ok');
  }
  
  // Built in Hooks
  if ($update) {
    $commands[] = "drush {$environment->drush_alias} updatedb --yes";
  }
  else {
    drush_log(dt('[DEVSHOP] Skipped updating database...'), 'ok');
  }

  // Clear the whole cache, unless option is false
  if ($cache) {
    if (drush_drupal_major_version(d()->root) == 8) {
      $commands[] = "drush {$environment->drush_alias} cache-rebuild";
    }
    else {
      $commands[] = "drush {$environment->drush_alias} cache-clear all";
    }
  }
  else {
    drush_log(dt('[DEVSHOP] Skipped clearing all caches...'), 'ok');
  }

  // Revert All Features, unless option is false
  if ($revert) {
    $commands[] = "drush {$environment->drush_alias} features-revert-all --yes";
  }
  else {
    drush_log(dt('[DEVSHOP] Skipped reverting all features...'), 'ok');
  }

  foreach ($commands as $command) {
    devshop_drush_process($command, $environment->repo_path, 'DevShop Deploy Hook');
  }
}

