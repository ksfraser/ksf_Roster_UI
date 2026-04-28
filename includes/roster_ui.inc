<?php
/**
 * Roster UI Functions
 *
 * UI helper functions for Roster module in FrontAccounting
 */

function roster_navbar(): void
{
    $current_section = isset($_GET['section']) ? $_GET['section'] : 'week';
    $week = isset($_GET['week']) ? $_GET['week'] : date('Y-m-d', strtotime('monday this week'));
    
    $prev_week = date('Y-m-d', strtotime($week . ' -7 days'));
    $next_week = date('Y-m-d', strtotime($week . ' +7 days'));
    
    start_table(TABLESTYLE);
    start_row();
    
    echo "<td><a href='?section=week&week=$week'>" . _("Weekly View") . "</a></td>";
    echo "<td><a href='?section=publish&week=$week'>" . _("Publish") . "</a></td>";
    echo "<td><a href='?week=$prev_week'>&lt;&lt;" . _("Previous") . "</a></td>";
    echo "<td><b>" . sql2date($week) . " - " . sql2date(date('Y-m-d', strtotime($week . ' +6 days'))) . "</b></td>";
    echo "<td><a href='?week=$next_week'>" . _("Next") . " &gt;&gt;</a></td>";
    
    end_row();
    end_table();
}

function display_weekly_roster(string $week): void
{
    $end = date('Y-m-d', strtotime($week . ' +6 days'));
    
    start_table(TABLESTYLE);
    
    table_header([
        _('Employee'),
        $week,
        date('Y-m-d', strtotime($week . ' +1 day')),
        date('Y-m-d', strtotime($week . ' +2 days')),
        date('Y-m-d', strtotime($week . ' +3 days')),
        date('Y-m-d', strtotime($week . ' +4 days')),
        date('Y-m-d', strtotime($week . ' +5 days')),
        date('Y-m-d', strtotime($week . ' +6 days')),
    ]);
    
    $shifts = get_weekly_shifts($week, $end);
    
    $employees = get_employees_list();
    
    while ($emp = db_fetch($employees)) {
        alt_table_row($emp);
        
        label_cell($emp['name']);
        
        for ($i = 0; $i < 7; $i++) {
            $date = date('Y-m-d', strtotime($week . ' +' . $i . ' days'));
            $shift = get_employee_shift($emp['id'], $date);
            
            if ($shift) {
                label_cell("<a href='?section=shift&shift_id=" . $shift['id'] . "'>" . $shift['shift'] . "</a>");
            } else {
                label_cell("-");
            }
        }
    }
    
    end_table(1);
    
    if (user_has_permission('SA_ROSTERCREATE')) {
        echo "<br><a href='?section=shift&shift_id=0&week=$week'>" . _("Add Shift") . "</a>";
    }
}

function display_shift_edit(int $shift_id): void
{
    global $Ajax;
    
    $week = isset($_GET['week']) ? $_GET['week'] : date('Y-m-d');
    
    $shift = $shift_id > 0 ? get_shift($shift_id) : null;
    
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        $employee_id = $_POST['employee_id'] ?? 0;
        $date = $_POST['date'] ?? $week;
        $shift_type = $_POST['shift_type'] ?? 'Morning';
        $start_time = $_POST['start_time'] ?? '09:00';
        $end_time = $_POST['end_time'] ?? '17:00';
        $notes = $_POST['notes'] ?? '';
        
        if ($shift_id > 0) {
            update_shift($shift_id, $employee_id, $date, $shift_type, $start_time, $end_time, $notes);
            display_notification(_("Shift updated"));
        } else {
            create_shift($employee_id, $date, $shift_type, $start_time, $end_time, $notes);
            display_notification(_("Shift created"));
        }
        
        meta_refresh(null, "?section=week&week=" . date('Y-m-d', strtotime($week)));
        $Ajax->activate('content_area');
    }
    
    start_form(true);
    
    start_table(TABLESTYLE);
    
    $emp_options = ['' => _('-- Select Employee --')] + get_employees_list_options();
    select_row(_("Employee:"), 'employee_id', $emp_options, $shift['employee_id'] ?? '');
    
    date_row(_("Date:"), 'date', $shift['date'] ?? $week);
    
    $shift_options = [
        'Morning' => _('Morning (9-5)'),
        'Afternoon' => _('Afternoon (1-9)'),
        'Night' => _('Night (9-5)'),
        'Swing' => _('Swing (12-8)'),
    ];
    select_row(_("Shift:"), 'shift_type', $shift_options, $shift['shift'] ?? 'Morning');
    
    time_row(_("Start Time:"), 'start_time', $shift['start_time'] ?? '09:00');
    time_row(_("End Time:"), 'end_time', $shift['end_time'] ?? '17:00');
    
    textarea_row(_("Notes:"), 'notes', $shift['notes'] ?? '', 5, 3);
    
    end_table(1);
    
    submit_center('submit', $shift_id > 0 ? _("Update Shift") : _("Create Shift"));
    
    end_form();
}

function display_employee_schedule(int $employee_id, string $week): void
{
    $end = date('Y-m-d', strtotime($week . ' +6 days'));
    
    start_table(TABLESTYLE);
    
    table_header([
        _('Date'),
        _('Shift'),
        _('Time'),
        _('Status'),
    ]);
    
    $shifts = get_employee_shifts($employee_id, $week, $end);
    
    while ($shift = db_fetch($shifts)) {
        alt_table_row($shift);
        
        label_cell(sql2date($shift['date']));
        
        label_cell($shift['shift']);
        
        label_cell($shift['start_time'] . ' - ' . $shift['end_time']);
        
        label_cell($shift['status']);
    }
    
    end_table(1);
}

function display_publish_options(): void
{
    global $Ajax;
    
    $week = isset($_GET['week']) ? $_GET['week'] : date('Y-m-d', strtotime('monday this week'));
    
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        $publish = $_POST['publish'] ?? false;
        
        if ($publish) {
            publish_schedule($week);
            display_notification(_("Schedule published"));
            
            meta_refresh(null, "?section=week&week=$week");
            $Ajax->activate('content_area');
        }
    }
    
    $published_week = get_published_schedule();
    
    start_form(true);
    
    start_table(TABLESTYLE);
    
    label_row(_("Current Published Week:"), $published_week ? sql2date($published_week) : _('None'));
    
    label_row(_("Week to Publish:"), sql2date($week) . ' - ' . sql2date(date('Y-m-d', strtotime($week . ' +6 days'))));
    
    end_table(1);
    
    check_box(_("Confirm Publish:"), 'confirm', 1);
    echo '<br>';
    submit_center('publish', _("Publish Schedule"));
    
    end_form();
}