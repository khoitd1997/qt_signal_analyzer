#include "scopemodule.h"

ScopeModule::ScopeModule(QList<QList<QList<QPointF> *>> &newDataBuffer)
    : newDataBuffer_(newDataBuffer) {}

ScopeModule::~ScopeModule() {}

void ScopeModule::setGuiSource(QObject *guiObj) { guiObj_ = guiObj; }
