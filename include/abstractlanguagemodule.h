#pragma once

#include <QString>

class AbstractLanguageModule {

public:

    /*!
     * Module name
     */
    virtual QString moduleName() const = 0;

    /*!
     * Initialize language messages
     */
    virtual void initializeMessages() = 0;

};
