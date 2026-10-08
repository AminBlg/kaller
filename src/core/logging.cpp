// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

#include "logging.h"

// QtInfoMsg as the default threshold: debug output stays quiet, warnings and
// above always reach the journal.
Q_LOGGING_CATEGORY(KALLER_TELEPHONY, "kaller.telephony", QtInfoMsg)
Q_LOGGING_CATEGORY(KALLER_CONTACTS, "kaller.contacts", QtInfoMsg)
Q_LOGGING_CATEGORY(KALLER_BLUEZ, "kaller.bluez", QtInfoMsg)
Q_LOGGING_CATEGORY(KALLER_NOTIFY, "kaller.notifications", QtInfoMsg)
Q_LOGGING_CATEGORY(KALLER_TRAY, "kaller.tray", QtInfoMsg)
