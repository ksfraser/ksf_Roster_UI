# Architecture - ksf_Roster_UI

## Document Information
- **Module**: ksf_Roster_UI
- **Version**: 1.0.0
- **Date**: 2026-05-11
- **Status**: Implemented
- **Author**: KSFII Development Team

---

## 1. Module Overview

ksf_Roster_UI provides the WordPress ESS user interface for Roster functionality.

### 1.1 Namespace
`Ksfraser\RosterUI`

### 1.2 Adapter Pattern
```
ksf_Roster (Business Logic)
    ↓
ksf_Roster_UI (WordPress ESS Adapter)
    ↓
    WordPress ESS Portal
```

---

## 2. Component Architecture

### 2.1 Presenter Layer

| Presenter | Description |
|-----------|-------------|
| ListPresenter | List page logic |
| FormPresenter | Form handling |
| DetailPresenter | Detail view logic |

### 2.2 AJAX Handlers

| Endpoint | Action | Description |
|----------|--------|-------------|
| ksf_Roster_list | getList | Get items |
| ksf_Roster_save | saveItem | Save item |
| ksf_Roster_delete | deleteItem | Delete item |

---

## 3. Integration

### Consumed From
| Module | Interface |
|--------|-----------|
| ksf_Roster | Business logic |

### WordPress Integration
| Hook | Description |
|------|-------------|
| wp_ajax_ksf_Roster | AJAX handlers |
| ksf_Roster_template | Page templates |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-11*
