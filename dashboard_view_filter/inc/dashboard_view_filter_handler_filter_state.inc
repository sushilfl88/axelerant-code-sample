// <?php

// class dashboard_view_filter_handler_filter_state extends views_handler_filter {

//   /**
//    * Options form subform for setting exposed filter options.
//    */
//   function value_form(&$form, &$form_state) {
//     parent::value_form($form, $form_state);

//     // get list of years from database
//     $query = db_select('node', 'n');
//     $query->addExpression("FROM_UNIXTIME(n.created, '%Y')", 'year');
//     if (isset($this->view->filter['type'])) {
//       $query->condition('n.type', $this->view->filter['type']->value, 'IN');
//     }
//     $result = $query->orderBy('year', 'ASC')
//       ->execute()
//       ->fetchAllAssoc('year');

//     $years = array(
//       '0' => t('All'),
//     );
//     foreach ($result as $k => $v) {
//       $years[$k] = $k;
//     }

//     // create form element with options retrieved from database
//     $form['value']['year'] = array(
//       '#type' => 'radios',
//       '#options' => $years,
//       '#default_value' => end($years),
//     );
//   }

//   /**
//    * Alters Views query when filter is used.
//    */
//   function query() {
//     // make sure base table is included in the query
//     $this->ensure_my_table();

//     // retrieve real filter name from view options
//     // this requires 'real field' filter option to be set (see code above)
//     $real_field_name = $this->real_field;
//     // get the value of the submitted filter
//     $value = $this->view->exposed_data[$real_field_name];

//     // finally, alter Views query
//     if (is_numeric($value) && $value != 0) {
//       /*
//         Having several custom exposed filters, make sure subsitution patterns
//         (e.g. :state_value below) don't match across different filters.
//         I spent some time figuring out why all my filters had the same value.
//         It looks like the query skeleton is built first and then all replacements
//         are made in bulk. Prefixing value with filter name looks good imo.
//       */
//       $this->query->add_where_expression($this->options['group'],
//         "FROM_UNIXTIME(node.created, '%Y') = :state_value",
//         array(':state_value' => $value));
//     }
//   }
// }