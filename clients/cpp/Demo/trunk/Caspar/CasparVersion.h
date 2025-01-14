#pragma once

#include "Shared.h"

class CASPAR_EXPORT CasparVersion
{
    public:
        explicit CasparVersion(const QString& version);

        const QString& getVersion() const;

    private:
        QString version;
};
