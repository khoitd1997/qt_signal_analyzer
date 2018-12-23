#include "measureobj.h"
#include <QQmlApplicationEngine>

Q_DECLARE_METATYPE(MeasureObj)

MeasureObj::MeasureObj()
  : QObject(nullptr)
  , m_isEnabled(false)
{}

MeasureObj::MeasureObj(const MeasureObj& other)
  : QObject(nullptr)
{
  m_isEnabled = other.m_isEnabled;
}

MeasureObj::MeasureObj(const bool isEnabled,
                       const double value,
                       const MeasureType type,
                       int targetIndex)
  : m_isEnabled(isEnabled)
  , m_value(value)
  , m_type(type)
  , m_targetIndex(targetIndex)
{}

MeasureObj::~MeasureObj() {}

bool
MeasureObj::isEnabled() const
{
  return m_isEnabled;
}

void
MeasureObj::setIsEnabled(bool status)
{
  if (status != m_isEnabled) {
    m_isEnabled = status;
    isEnabledChanged();
  }
}

double
MeasureObj::value() const
{
  return this->m_value;
}
void
MeasureObj::setValue(const double newValue)
{
  if (newValue != this->m_value) {
    this->m_value = newValue;
    valueChanged();
  }
}

int
MeasureObj::type() const
{
  return static_cast<int>(this->m_type);
}
void
MeasureObj::setType(const int newType)
{
  if (newType != static_cast<int>(this->m_type)) {
    this->m_type = static_cast<MeasureObj::MeasureType>(newType);
    typeChanged();
  }
}

int
MeasureObj::targetIndex() const
{
  return this->m_targetIndex;
}
void
MeasureObj::setTargetIndex(const int newIndex)
{
  if (newIndex != this->m_targetIndex) {
    this->m_targetIndex = newIndex;
    targetIndexChanged();
  }
}
