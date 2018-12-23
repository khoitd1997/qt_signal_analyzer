#ifndef MEASUREMENTOBJ_H
#define MEASUREMENTOBJ_H

#include <QObject>
#include <QVariant>

class MeasureObj : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    bool isEnabled READ isEnabled WRITE setIsEnabled NOTIFY isEnabledChanged)
  Q_PROPERTY(double value READ value CONSTANT NOTIFY valueChanged)
  Q_PROPERTY(int targetIndex READ targetIndex WRITE setTargetIndex NOTIFY
               targetIndexChanged)
  Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
  enum MeasureType
  {
    Freq = 0,
    Period,
    Max,
    Min
  };
  Q_ENUM(MeasureType)

  MeasureObj();
  MeasureObj(const bool isEnabled,
             const double value,
             const MeasureType type,
             int targetIndex);
  MeasureObj(const MeasureObj& other);

  ~MeasureObj();

  bool isEnabled(void) const;
  void setIsEnabled(const bool status);

  double value() const;
  void setValue(const double newValue);

  int type() const;
  void setType(const int newType);

  int targetIndex() const;
  void setTargetIndex(const int newIndex);

signals:
  void valueChanged();
  void isEnabledChanged();
  void targetIndexChanged();
  void typeChanged();

public slots:
private:
  bool m_isEnabled;
  double m_value;
  MeasureType m_type;
  int m_targetIndex;
};

#endif // MEASUREMENTOBJ_H
