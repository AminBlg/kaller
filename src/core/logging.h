// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLoggingCategory>

// All off by default. Turn them on for one run with:
//     QT_LOGGING_RULES="kaller.*=true" kaller
// Warnings are on regardless, so a persistent failure reaches the journal
// without anyone having to reproduce it with a flag set.
Q_DECLARE_LOGGING_CATEGORY(KALLER_TELEPHONY)
Q_DECLARE_LOGGING_CATEGORY(KALLER_CONTACTS)
Q_DECLARE_LOGGING_CATEGORY(KALLER_BLUEZ)
Q_DECLARE_LOGGING_CATEGORY(KALLER_NOTIFY)
Q_DECLARE_LOGGING_CATEGORY(KALLER_TRAY)
