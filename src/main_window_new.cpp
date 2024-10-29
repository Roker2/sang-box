#include "main_window_new.h"

#include <QDebug>
#include <QRegularExpression>

#include "html_color_text.h"

MainWindowNew::MainWindowNew(QObject *parent)
    : QObject{parent}
    , m_configManager(std::make_shared<ConfigManager>(this))
    , m_configListModel(std::make_shared<ConfigListModel>(m_configManager))
    , m_proxyManager(std::make_unique<ProxyManager>(this))
{
    connect(m_configManager.get(), &ConfigManager::configChanged, this,
            &MainWindowNew::changeSelectedConfig);
    changeSelectedConfig();

    connect(m_proxyManager.get(), &ProxyManager::proxyProcessStateChanged,
            this, &MainWindowNew::runningStateChanged);
    connect(m_proxyManager.get(), &ProxyManager::proxyProcessReadyReadStandardError,
            this, &MainWindowNew::updateProxyOutput);
}

ConfigListModel* MainWindowNew::configListModel() const
{
    return m_configListModel.get();
}

void MainWindowNew::startProxy()
{
    m_proxyManager->startProxy();
}

void MainWindowNew::stopProxy()
{
    m_proxyManager->stopProxy();
}

bool MainWindowNew::runnigState() const
{
    return m_proxyManager->proxyProcessState() == QProcess::Running;
}

QString MainWindowNew::proxyOutput() const
{
    return m_proxyOutput;
}

void MainWindowNew::changeSelectedConfig()
{
    m_proxyManager->setConfigFilePath(m_configManager->configFilePath());
    if (runnigState())
    {
        stopProxy();
        startProxy();
    }
}

void MainWindowNew::updateProxyOutput()
{
    QByteArray outputData = m_proxyManager->readProxyProcessAllStandardError();
    QString outputText = QString::fromUtf8(outputData);
    if (outputText.isEmpty())
        return;
    HtmlColorText::appendHtmlColorText(outputText);

    QRegularExpression re("\\n$");
    QRegularExpressionMatch match = re.match(outputText);
    if (match.hasMatch())
    {
        const auto endPos = match.capturedStart(0);
        outputText = outputText.left(endPos);
    }
    m_proxyOutput += outputText + "<br/>";
    emit proxyOutputChanged();
}
