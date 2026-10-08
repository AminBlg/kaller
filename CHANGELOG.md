<!--
SPDX-FileCopyrightText: 2026 AminBlg
SPDX-License-Identifier: GPL-3.0-or-later
-->

# Changelog

## 0.2.0, 2026-10-08

Foner is now Kaller. A reader on r/kde suggested the name. The program, the repository and the app ID all changed, and GitHub redirects the old repository address.

- Settings and contacts move from the Foner names to the Kaller names on the first start.
- The install script accepts the old `FONER_` variables and builds `main` by default.
- The selected phone is kept by its Bluetooth address, so another phone leaving cannot change it.
- Mute follows the phone's answer, so a refused write no longer shows "muted" with a live microphone.
- If "keep running in the tray" is off, closing the window quits. "Start hidden" no longer hides an open window.
- The call history imports once per phone, even after a live call.
- Clearing the contacts also clears the list of dialled numbers.
- Contacts can load from the vCards that KDE Connect keeps. Use the File menu, or turn on automatic loading.

## 0.1.0, 2026-10-06

The first release.

- Dial, answer and end calls through a phone paired over the Bluetooth hands-free profile.
- Answer or decline from a desktop notification.
- Read the contacts and the call history from the phone, and search them by name, by number or by keypad letters.
- Import and export vCard files.
- Keep running in the system tray, and open `tel:` links from other applications.

Two calls at once, service codes and vCard export are built, but nobody tested them against a phone yet.
