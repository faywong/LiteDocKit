#include <QtWidgets>
#include <QSimpleUpdater.h>
#include "configurationpage.h"
#include "settings/settings_def.h"
#include "syncapp.h"

ConfigurationPage::ConfigurationPage(QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *configGroup = new QGroupBox(tr("Sync Remote"));

    QComboBox *syncServiceTypeCombo = new QComboBox;
    syncServiceTypeCombo->addItem(tr("Sync cloud"));
    syncServiceTypeCombo->addItem(tr("Self deploy server"));

    QHBoxLayout *serviceTypeLayout = new QHBoxLayout;
    QLabel *serverLabel = new QLabel(tr("Sync service type:"));
    serviceTypeLayout->addWidget(serverLabel);
    serviceTypeLayout->addWidget(syncServiceTypeCombo);

    QHBoxLayout *serviceApiLayout = new QHBoxLayout;
    QLabel *serviceApiEndPointLabel = new QLabel(tr("service api endpoint:"));
    serviceApiLayout->addWidget(serviceApiEndPointLabel);
    serviceApiEndPointEdit = new QLineEdit();
    serviceApiEndPointEdit->setText("https://cloud.chengxi.fun/api/v1");
    serviceApiLayout->addWidget(serviceApiEndPointEdit);

    QVBoxLayout *syncCloudLayout = new QVBoxLayout;
    // username part
    QHBoxLayout *usernameLayout = new QHBoxLayout;
    QLabel *usernameLabel = new QLabel(tr("username:"));
    usernameLayout->addWidget(usernameLabel);
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText(tr("your subscribed account name here"));
    usernameLayout->addWidget(usernameEdit);

    syncCloudLayout->addLayout(usernameLayout);

    // password part
    QHBoxLayout *passwordLayout = new QHBoxLayout;
    QLabel *passwordLabel = new QLabel(tr("password:"));
    passwordLayout->addWidget(passwordLabel);
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText(tr("password of your subscribed account here"));
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordEdit);

    syncCloudLayout->addLayout(passwordLayout);

    // apply for account
    QHBoxLayout *applyAccountLayout = new QHBoxLayout;
    QLabel *applyAccountLabel = new QLabel(tr("apply account:"));
    applyAccountLayout->addWidget(applyAccountLabel);
    QString platform = QSimpleUpdater::getInstance()->getPlatformKey(syncfolderUpdateUrl);
    QLabel *targetPage = new QLabel("<a href=\"https://cloud.chengxi.fun/api/v1/signup?source=qt_setting_ui&platform=" + platform + "\">Apply for an account!</a>");
    targetPage->setTextFormat(Qt::TextFormat::RichText);

    connect(targetPage, &QLabel::linkActivated, [=](const QString &url) {
        QDesktopServices::openUrl(QUrl(url));
    });

    applyAccountLayout->addWidget(targetPage);

    syncCloudLayout->addLayout(applyAccountLayout);

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(serviceTypeLayout);
    configLayout->addLayout(serviceApiLayout);
    configLayout->addLayout(syncCloudLayout);

    configGroup->setLayout(configLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);

    connect(syncServiceTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [&](int index) {
        if (index == 0 /* cloud service */) {
            serviceApiEndPointEdit->setText("https://cloud.chengxi.fun/api/v2");
        } else if (index == 1 /* self hosted service */) {
            serviceApiEndPointEdit->setPlaceholderText("https://your_server_goes_here/api");
        }
    });

    setLayout(mainLayout);
}

void ConfigurationPage::save() {
    SyncFolderSettings settings;
    settings.setString(KEY_API_ENDPOINT, serviceApiEndPointEdit->text());
    if (!usernameEdit->text().isEmpty()) {
        settings.setString(KEY_API_USERNAME, usernameEdit->text());
    }
    if (!passwordEdit->text().isEmpty()) {
        settings.setString(KEY_API_PASSWORD, passwordEdit->text());
    }
}

void ConfigurationPage::reset() {
    // noop
}
