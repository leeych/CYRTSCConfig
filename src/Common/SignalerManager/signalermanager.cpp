#include "signalermanager.h"
#include "mutility.h"
#include <QFile>

SignalerManager* SignalerManager::instance_ = NULL;

SignalerManager::SignalerManager()
{
    xmlhelper_ = XmlHelper::GetInstance();
}

SignalerManager::~SignalerManager()
{
}

bool SignalerManager::createXmlTest()
{
    SignalerParam signaler;
    signaler.signaler_id = 1;
    signaler.signaler_name = "Signal";
    signaler.signaler_ip = "10.88.1.11";
    signaler.signaler_port = 1024;
    signaler.signaler_mode = "Coordinate";
    signaler.branch_road = "Yellow and Science";
    signaler.location_desc = "Hefei ShuShan";
    signaler.software_version = "1.0.0.1";
	signaler.remark = "remark";

    QString file_name = ":/user/xml/signaler.xml";
    MUtility::getMainDir(file_name);
    file_name += "/user/xml/signaler.xml";
    bool status = xmlhelper_->CreateXmlDocument(file_name, SignalerElement::SignalerList);
    if (!status)
    {
        return false;
    }
    xmlhelper_->AddXmlNode(SignalerElement::SignalerList, SignalerElement::Signaler);

    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Name, signaler.signaler_name);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::IP, signaler.signaler_ip);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Port, QString::number(signaler.signaler_port));
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Location, signaler.location_desc);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Branch, signaler.branch_road);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Mode, signaler.signaler_mode);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Version, signaler.software_version);
    xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Remark, signaler.remark);

    return true;
}

SignalerMap &SignalerManager::GetSignaler(const QString &file_name)
{
    file_name_ = file_name;
    xmlhelper_->LoadXmlFile(file_name);

    QStringList name_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Name);
    QStringList ip_list = xmlhelper_->ParseXmlNodeList(SignalerElement::IP);
    QStringList port_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Port);
    QStringList location_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Location);
    QStringList branch_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Branch);
    QStringList mode_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Mode);
    QStringList version_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Version);
    QStringList remark_list = xmlhelper_->ParseXmlNodeList(SignalerElement::Remark);

    SignalerParam signaler;
    for (int i = 0; i < name_list.size(); i++)
    {
        signaler.signaler_id = i+1;
        signaler.signaler_name = name_list.at(i);
        signaler.signaler_ip = ip_list.at(i);
        signaler.signaler_port = port_list.at(i).toInt();
        signaler.location_desc = location_list.at(i);
        signaler.signaler_mode = mode_list.at(i);
        signaler.branch_road = branch_list.at(i);
        signaler.software_version = version_list.at(i);
        signaler.remark = remark_list.at(i);
        signaler.signaler_status = SignalerParam::Offline;

        signaler_map_.insert(signaler.signaler_id, signaler);
    }
    return signaler_map_;
}

bool SignalerManager::SetSignaler(const SignalerMap &signaler_map, const QString &file_name)
{
    if (!xmlhelper_->LoadXmlFile(file_name))
    {
        return false;
    }
    xmlhelper_->ClearXmlDocument();
    xmlhelper_->AddRootNode(SignalerElement::SignalerList);

    file_name_ = file_name;
    SignalerParam signaler;
    SignalerMap::ConstIterator iter = signaler_map.constBegin();
    while (iter != signaler_map.constEnd())
    {
        signaler = iter.value();
        signaler_map_[signaler.signaler_id] = signaler;
        xmlhelper_->AddXmlNode(SignalerElement::SignalerList, SignalerElement::Signaler);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Name, signaler.signaler_name);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::IP, signaler.signaler_ip);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Port, QString::number(signaler.signaler_port));
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Location, signaler.location_desc);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Branch, signaler.branch_road);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Mode, signaler.signaler_mode);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Version, signaler.software_version);
        xmlhelper_->AddXmlNode(SignalerElement::Signaler, SignalerElement::Remark, signaler.remark);

        ++iter;
    }
    return true;
}
