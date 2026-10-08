// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

#include "migration.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "core/logging.h"

namespace Migration {

void fromFoner(const QString &configRoot, const QString &dataRoot)
{
    const QString oldRc = configRoot + QStringLiteral("/fonerrc");
    const QString newRc = configRoot + QStringLiteral("/kallerrc");
    if (QFile::exists(oldRc) && !QFile::exists(newRc)) {
        if (QFile::rename(oldRc, newRc))
            qCInfo(KALLER_CONTACTS) << "moved the settings from fonerrc to kallerrc";
        else
            qCWarning(KALLER_CONTACTS) << "could not move fonerrc to kallerrc";
    }

    const QDir oldDir(dataRoot + QStringLiteral("/foner"));
    const QString newPath = dataRoot + QStringLiteral("/kaller");
    if (!oldDir.exists())
        return;
    // An existing kaller directory means the new name already ran. Leave both.
    if (QDir(newPath).exists())
        return;
    if (!QDir().mkpath(newPath)) {
        qCWarning(KALLER_CONTACTS) << "could not create" << newPath;
        return;
    }

    // Flat on purpose: the phonebook cache and the last-called file are the only files.
    int moved = 0;
    const QStringList names = oldDir.entryList(QDir::Files);
    for (const QString &name : names) {
        if (QFile::rename(oldDir.filePath(name), newPath + QLatin1Char('/') + name))
            ++moved;
    }
    qCInfo(KALLER_CONTACTS) << "moved" << moved << "files from the Foner data directory";
    // Only removes the directory when it is empty, which is what is wanted.
    QDir().rmdir(oldDir.path());
}

void fromFoner()
{
    fromFoner(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation),
              QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
}

} // namespace Migration
