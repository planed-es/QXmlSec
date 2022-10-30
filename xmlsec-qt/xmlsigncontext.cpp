#include "xmlsigncontext.h"
#include <QRandomGenerator>
#include <QTime>
#include <QPair>

QString QXmlSignContext::tagId(const QString& tagName) const
{
  thread_local static unsigned char seed = 0;
  QRandomGenerator generator(QTime::currentTime().msec() + ++seed);
  QPair<quint32, quint32> limits(0, static_cast<quint32>('z') - static_cast<quint32>('a'));
  char randomPart[8];

  randomPart[8] = 0;
  for (int i = 0 ; i < 8 ; ++i)
  {
    quint32 value = generator.bounded(limits.first, limits.second);

    randomPart[i] = static_cast<char>(value + 'a');
  }
  return signatureId + '-' + randomPart + '-' + tagName;
}
