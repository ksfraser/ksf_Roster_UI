<?php
/**
 * Roster Management
 *
 * Roster scheduling page for FrontAccounting
 */

$page_security = 'SA_ROSTER';
$path_to_root = "../../..";

include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/modules/FA_Roster/includes/roster_db.inc");
include_once($path_to_root . "/modules/FA_Roster/includes/roster_ui.inc");

$js = "";
if ($SysPrefs->use_popup_windows)
    $js .= get_js_open_window(900, 600);

page(_("Roster"), false, false, "", $js);

//--------------------------------------------------------------------------------------------

start_table(TABLESTYLE_NOBORDER);
start_row();
roster_navbar();
end_row();
end_table();

echo '<br>';

// Get section from URL
$section = isset($_GET['section']) ? $_GET['section'] : 'week';

$week = isset($_GET['week']) ? $_GET['week'] : date('Y-m-d', strtotime('monday this week'));

switch ($section) {
    case 'shift':
        $shift_id = isset($_GET['shift_id']) ? $_GET['shift_id'] : 0;
        display_shift_edit($shift_id);
        break;
    case 'employee':
        $employee_id = isset($_GET['employee_id']) ? $_GET['employee_id'] : 0;
        display_employee_schedule($employee_id, $week);
        break;
    case 'publish':
        display_publish_options();
        break;
    case 'week':
    default:
        display_weekly_roster($week);
        break;
}

end_page(true);