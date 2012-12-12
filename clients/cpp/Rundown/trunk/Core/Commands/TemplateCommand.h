#pragma once

#include "Shared.h"
#include "ICommand.h"
#include "Models/TemplateDataModel.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

class CORE_EXPORT TemplateCommand : public QObject, public ICommand
{
    Q_OBJECT

    public:
        explicit TemplateCommand(QObject* parent = 0);

        virtual int getDelay() const;
        virtual int getChannel() const;
        virtual int getVideolayer() const;
        virtual bool getAllowGpi() const;

        virtual void setChannel(int channel);
        virtual void setVideolayer(int videolayer);
        virtual void setDelay(int delay);
        virtual void setAllowGpi(bool allowGpi);

        virtual void loadProperties(const boost::property_tree::wptree& pt);
        virtual void saveProperties(const boost::property_tree::wptree& pt);

        int getFlashlayer() const;
        const QString& getInvoke() const;
        bool getUseStoredData() const;
        const QString& getTemplateName() const;
        const QString getTemplateData() const;
        const QList<TemplateDataModel>& getTemplateDataModels() const;

        void setFlashlayer(int flashlayer); 
        void setInvoke(const QString& invoke);
        virtual void setUseStoredData(bool useStoredData);
        void setTemplateName(const QString& templateName);
        void setTemplateDataModels(const QList<TemplateDataModel>& models);

    private:
        int channel;
        int videolayer;
        int delay;
        bool allowGpi;
        int flashlayer;
        QString invoke;
        bool useStoredData;
        QString templateName;
        QList<TemplateDataModel> models;

        Q_SIGNAL void channelChanged(int);
        Q_SIGNAL void videolayerChanged(int);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void flashlayerChanged(int);
        Q_SIGNAL void invokeChanged(const QString&);
        Q_SIGNAL void useStoredDataChanged(bool);
        Q_SIGNAL void templateNameChanged(const QString&);
        Q_SIGNAL void templateDataChanged(const QList<TemplateDataModel>&);
};