// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

// Kaller was called Foner until 0.2.0, and the name decides where KConfig and
// AppDataLocation put the user's files. Without this, an upgrade would start
// with default settings and an empty phonebook.
//
// Moves ~/.config/fonerrc to kallerrc and the files in ~/.local/share/foner
// to ~/.local/share/kaller, but only where the new name has nothing yet.
// Moved, not copied: the last-called file holds every number ever dialled, and
// a copy left behind would survive "Clear contacts".
//
// The two roots are parameters so a test can point them at scratch directories.
namespace Migration {
void fromFoner(const QString &configRoot, const QString &dataRoot);
// The same, with the user's real XDG directories.
void fromFoner();
}
