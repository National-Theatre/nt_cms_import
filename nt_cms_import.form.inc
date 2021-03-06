<?php
/**
 * @file
 * nt_cms_import.form.inc
 * @copyright The Royal National Theatre
 * @author John-Paul Drawneek <jdrawneek@nationaltheatre.org.uk>
 */

/**
 * Getting Access token callback page.
 *
 * @return string
 *   String of token dump.
 */
function nt_cms_import_access_callback() {
  $key = variable_get('nt_cms_import_cms_key', '');
  $secret = variable_get('nt_cms_import_cms_secret', '');
  $consumer = new DrupalOAuthConsumer($key, $secret, array());
  $token = unserialize(variable_get('nt_cms_import_rtoken'));
  $client = new \DrupalOAuthClient($consumer, $token);
  $endpoint = variable_get(
    'nt_cms_import_cms',
    'http://cms.nationaltheatre.develop.local'
  ) . '/oauth/access_token';
  $access_token = $client->getAccessToken($endpoint);
  variable_set('nt_cms_import_token', serialize($access_token));
  drupal_set_message(t('Access token saved.'));
  return '<pre>' . print_r($access_token, TRUE) . '</pre>';
}
/**
 * Config for for accessing the NT CMS rest service.
 *
 * @param array $form
 *   Current from structure.
 * @param array $form_state
 *   Current form structure state.
 *
 * @return array
 *   Form structure to be rendered.
 */
function nt_cms_import_admin_config($form, &$form_state) {
  form_load_include($form_state, 'inc', 'nt_cms_import', 'nt_cms_import.form');
  $form['cms_paths'] = array(
    '#type'  => 'fieldset',
    '#title' => t('CMS Path settings'),
    'nt_cms_import_cms_www' => array(
      '#type'             => 'textfield',
      '#title'            => t('CMS Frontend URL'),
      '#default_value'    => variable_get(
        'nt_cms_import_cms_www',
        'http://www.nationaltheatre.develop.local'
      ),
      '#element_validate' => array('_nt_cms_import_config_validate'),
    ),
    'nt_cms_import_cms' => array(
      '#type'             => 'textfield',
      '#title'            => t('CMS URL'),
      '#default_value'    => variable_get(
        'nt_cms_import_cms',
        'http://cms.nationaltheatre.develop.local'
      ),
      '#element_validate' => array('_nt_cms_import_config_validate'),
    ),
    'nt_cms_import_cms_cf' => array(
      '#type'             => 'textfield',
      '#title'            => t('CMS CloudFront'),
      '#default_value'    => variable_get(
        'nt_cms_import_cms_cf',
        'http://d1wf8hd6ovssje.cloudfront.net'
      ),
      '#element_validate' => array('_nt_cms_import_config_validate'),
    ),
    'nt_cms_import_cms_endpoint' => array(
      '#type'          => 'textfield',
      '#title'         => t('Rest End Point'),
      '#default_value' => variable_get('nt_cms_import_cms_endpoint', 'rest'),
    ),
    'nt_cms_import_cms_path' => array(
      '#type'          => 'textfield',
      '#title'         => t('Path to files on CMS'),
      '#default_value' => variable_get(
        'nt_cms_import_cms_path',
        '/sites/all/libraries/files'
      ),
    ),
  );
  $form['security'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('CMS service security'),
    '#description' => 'ToDo',
    'token'        => array(
      '#type'   => 'item',
      '#title'  => t('Token'),
      '#markup' => unserialize(variable_get('nt_cms_import_token', 'none')),
    ),
    'nt_cms_import_cms_key' => array(
      '#type' => 'textfield',
      '#title' => t('CMS Key'),
      '#default_value' => variable_get('nt_cms_import_cms_key', ''),
    ),
    'nt_cms_import_cms_secret' => array(
      '#type' => 'textfield',
      '#title' => t('CMS Secret'),
      '#default_value' => variable_get('nt_cms_import_cms_secret', ''),
    ),
    'get_token' => array(
      '#type'   => 'submit',
      '#value'  => t('Get token'),
      '#submit' => array('nt_cms_import_config_get_token'),
    ),
  );
  return system_settings_form($form);
}
/**
 * Validate the CMS url and remove trailing slash.
 *
 * @param array $element
 *   Element to be validated.
 * @param array $form_state
 *   Current state of the form.
 */
function _nt_cms_import_config_validate($element, &$form_state) {
  if (empty($element['#value']) || !valid_url($element['#value'], TRUE) ) {
    form_error(
      $element,
      t(
        'The "!name" option must contain a valid url.',
        array('!name' => t($element['#title']))
      )
    );
  }
  $form_state['values'][$element['#name']] = rtrim($form_state['values'][$element['#name']], '/');
}
