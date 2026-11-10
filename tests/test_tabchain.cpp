// SPDX-FileCopyrightText: 2026 AminBlg
// SPDX-License-Identifier: GPL-3.0-or-later

// Loads the real main.qml with the real models and walks it with the Tab key.

#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSet>
#include <QStandardPaths>
#include <QTest>

#include "core/contacts/contactsmodel.h"
#include "core/recents/recentsmodel.h"
#include "core/settings/settings.h"
#include "core/telephony/agmodel.h"
#include "core/telephony/bluezdevices.h"
#include "core/telephony/callmodel.h"
#include "core/telephony/telephonyclient.h"

namespace {

QQuickItem *find(QQuickItem *root, const std::function<bool(QQuickItem *)> &match)
{
    if (match(root))
        return root;
    for (QQuickItem *child : root->childItems())
        if (QQuickItem *hit = find(child, match))
            return hit;
    return nullptr;
}

QString digitOf(const QQuickItem *item)
{
    return item ? item->property("modelData").toMap().value(QStringLiteral("digit")).toString()
                : QString();
}

} // namespace

class TestTabChain : public QObject
{
    Q_OBJECT

    // Real objects, as main.cpp wires them, so the bindings in main.qml resolve.
    std::unique_ptr<Settings> settings;
    std::unique_ptr<TelephonyClient> client;
    std::unique_ptr<BluezDevices> bluez;
    std::unique_ptr<ContactsModel> contacts;
    std::unique_ptr<RecentsModel> recents;
    std::unique_ptr<QQmlApplicationEngine> engine;
    QQuickWindow *window = nullptr;

    QQuickItem *focused() const { return window->activeFocusItem(); }

    void tab() { QTest::keyClick(window, Qt::Key_Tab); }

private Q_SLOTS:
    void initTestCase()
    {
        // Settings must not read or write the user's kallerrc.
        QStandardPaths::setTestModeEnabled(true);
        settings = std::make_unique<Settings>();
        client = std::make_unique<TelephonyClient>();
        bluez = std::make_unique<BluezDevices>();
        contacts = std::make_unique<ContactsModel>();
        recents = std::make_unique<RecentsModel>();

        engine = std::make_unique<QQmlApplicationEngine>();
        QQmlContext *ctx = engine->rootContext();
        ctx->setContextObject(new KLocalizedQmlContext(engine.get()));
        ctx->setContextProperty(QStringLiteral("telephonyClient"), client.get());
        ctx->setContextProperty(QStringLiteral("devicesModel"), client->devices());
        ctx->setContextProperty(QStringLiteral("callsModel"), client->calls());
        ctx->setContextProperty(QStringLiteral("contactsModel"), contacts.get());
        ctx->setContextProperty(QStringLiteral("recentsModel"), recents.get());
        ctx->setContextProperty(QStringLiteral("settings"), settings.get());
        ctx->setContextProperty(QStringLiteral("bluez"), bluez.get());
        ctx->setContextProperty(QStringLiteral("aboutData"), QVariant());

        engine->load(QUrl::fromLocalFile(QStringLiteral(KALLER_MAIN_QML)));
        QVERIFY(!engine->rootObjects().isEmpty());
        window = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
        QVERIFY(window);
        window->show();
        QVERIFY(QTest::qWaitForWindowExposed(window));
    }

    void tabFromNumberFieldReachesKeyOne()
    {
        QQuickItem *field = find(window->contentItem(), [](QQuickItem *i) {
            return i->isVisible() && i->property("inputMethodHints").isValid()
                && i->property("placeholderText").isValid();
        });
        QVERIFY(field);
        field->forceActiveFocus(Qt::TabFocusReason);
        QCOMPARE(focused(), field);
        tab();
        QCOMPARE(digitOf(focused()), QStringLiteral("1"));
    }

    void tabWalksTheTwelveKeysInOrder()
    {
        const QStringList order = {QStringLiteral("1"), QStringLiteral("2"), QStringLiteral("3"),
                                   QStringLiteral("4"), QStringLiteral("5"), QStringLiteral("6"),
                                   QStringLiteral("7"), QStringLiteral("8"), QStringLiteral("9"),
                                   QStringLiteral("*"), QStringLiteral("0"), QStringLiteral("#")};
        QQuickItem *first = find(window->contentItem(), [](QQuickItem *i) { return digitOf(i) == QLatin1String("1"); });
        QVERIFY(first);
        first->forceActiveFocus(Qt::TabFocusReason);
        for (int i = 1; i < order.size(); ++i) {
            tab();
            QCOMPARE(digitOf(focused()), order.at(i));
        }
    }

    // With no phone the Call button is disabled. A shortcut past the keys then
    // made Tab loop over three controls and never reach a key.
    void tabCyclesThroughEveryKeyBackToTheField()
    {
        QQuickItem *field = find(window->contentItem(), [](QQuickItem *i) {
            return i->isVisible() && i->property("inputMethodHints").isValid()
                && i->property("placeholderText").isValid();
        });
        QVERIFY(field);
        field->forceActiveFocus(Qt::TabFocusReason);
        QSet<QString> digits;
        bool back = false;
        for (int n = 0; n < 40 && !back; ++n) {
            tab();
            if (!digitOf(focused()).isEmpty())
                digits.insert(digitOf(focused()));
            back = focused() == field;
        }
        QCOMPARE(digits.size(), 12);
        QVERIFY2(back, "Tab never comes back to the number field");
    }
};

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kaller");
    TestTabChain test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_tabchain.moc"
