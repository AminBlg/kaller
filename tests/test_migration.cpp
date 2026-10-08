// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtTest>

#include "core/settings/migration.h"

class TestMigration : public QObject
{
    Q_OBJECT

private:
    static void write(const QString &path, const QByteArray &body)
    {
        QVERIFY(QDir().mkpath(QFileInfo(path).absolutePath()));
        QFile f(path);
        QVERIFY(f.open(QIODevice::WriteOnly));
        f.write(body);
    }
    static QByteArray read(const QString &path)
    {
        QFile f(path);
        return f.open(QIODevice::ReadOnly) ? f.readAll() : QByteArray("<missing>");
    }

private Q_SLOTS:
    void movesTheSettingsAndTheDataFiles()
    {
        QTemporaryDir config, data;
        QVERIFY(config.isValid() && data.isValid());
        write(config.filePath(QStringLiteral("fonerrc")), "[General]\nCloseToTray=false\n");
        write(data.filePath(QStringLiteral("foner/phonebook.vcf")), "BEGIN:VCARD");
        write(data.filePath(QStringLiteral("foner/lastcalled")), "dialled");

        Migration::fromFoner(config.path(), data.path());

        QCOMPARE(read(config.filePath(QStringLiteral("kallerrc"))),
                 QByteArray("[General]\nCloseToTray=false\n"));
        QCOMPARE(read(data.filePath(QStringLiteral("kaller/phonebook.vcf"))),
                 QByteArray("BEGIN:VCARD"));
        QCOMPARE(read(data.filePath(QStringLiteral("kaller/lastcalled"))), QByteArray("dialled"));
        // Moved, not copied: nothing with the old name keeps the dialled numbers.
        QVERIFY(!QFile::exists(config.filePath(QStringLiteral("fonerrc"))));
        QVERIFY(!QDir(data.filePath(QStringLiteral("foner"))).exists());
    }

    void neverOverwritesWhatTheNewNameAlreadyHolds()
    {
        QTemporaryDir config, data;
        QVERIFY(config.isValid() && data.isValid());
        write(config.filePath(QStringLiteral("fonerrc")), "old");
        write(config.filePath(QStringLiteral("kallerrc")), "new");
        write(data.filePath(QStringLiteral("foner/lastcalled")), "old");
        write(data.filePath(QStringLiteral("kaller/lastcalled")), "new");

        Migration::fromFoner(config.path(), data.path());

        QCOMPARE(read(config.filePath(QStringLiteral("kallerrc"))), QByteArray("new"));
        QCOMPARE(read(data.filePath(QStringLiteral("kaller/lastcalled"))), QByteArray("new"));
        // The old files stay: this run did not touch them.
        QCOMPARE(read(config.filePath(QStringLiteral("fonerrc"))), QByteArray("old"));
        QCOMPARE(read(data.filePath(QStringLiteral("foner/lastcalled"))), QByteArray("old"));
    }

    void migratesEvenWhenTheInstallerAlreadyCreatedTheNewDirectory()
    {
        // scripts/install.sh writes ~/.local/share/kaller/install_manifest.txt
        // before the program first runs. That must not stop the data moving.
        QTemporaryDir config, data;
        QVERIFY(config.isValid() && data.isValid());
        write(data.filePath(QStringLiteral("foner/phonebook.vcf")), "BEGIN:VCARD");
        write(data.filePath(QStringLiteral("foner/lastcalled")), "dialled");
        write(data.filePath(QStringLiteral("foner/install_manifest.txt")), "old list");
        write(data.filePath(QStringLiteral("kaller/install_manifest.txt")), "new list");

        Migration::fromFoner(config.path(), data.path());

        QCOMPARE(read(data.filePath(QStringLiteral("kaller/phonebook.vcf"))),
                 QByteArray("BEGIN:VCARD"));
        QCOMPARE(read(data.filePath(QStringLiteral("kaller/lastcalled"))), QByteArray("dialled"));
        // Neither list is touched: the old one still says how to remove Foner.
        QCOMPARE(read(data.filePath(QStringLiteral("kaller/install_manifest.txt"))),
                 QByteArray("new list"));
        QCOMPARE(read(data.filePath(QStringLiteral("foner/install_manifest.txt"))),
                 QByteArray("old list"));
    }

    void doesNothingOnAFreshInstall()
    {
        QTemporaryDir config, data;
        QVERIFY(config.isValid() && data.isValid());

        Migration::fromFoner(config.path(), data.path());

        QVERIFY(!QFile::exists(config.filePath(QStringLiteral("kallerrc"))));
        QVERIFY(!QDir(data.filePath(QStringLiteral("kaller"))).exists());
    }

    void runningTwiceChangesNothingTheSecondTime()
    {
        QTemporaryDir config, data;
        QVERIFY(config.isValid() && data.isValid());
        write(data.filePath(QStringLiteral("foner/lastcalled")), "dialled");

        Migration::fromFoner(config.path(), data.path());
        Migration::fromFoner(config.path(), data.path());

        QCOMPARE(read(data.filePath(QStringLiteral("kaller/lastcalled"))), QByteArray("dialled"));
    }
};

QTEST_GUILESS_MAIN(TestMigration)
#include "test_migration.moc"
