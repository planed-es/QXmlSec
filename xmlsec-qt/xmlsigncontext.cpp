#include "xmlsigncontext.h"
#include <QRandomGenerator>
#include <QTime>
#include <QPair>
#include <QMap>

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

QUrl QXmlSignContext::algorithmUrl(QCryptographicHash::Algorithm algorithm)
{
  QString base("http://www.w3.org/2001/04/xmlenc#");
  QMap<QCryptographicHash::Algorithm, QString> ids{
    {QCryptographicHash::Sha1,   "sha1"},
    {QCryptographicHash::Sha256, "sha256"},
    {QCryptographicHash::Sha512, "sha512"}
  };

  if (ids.find(algorithm) == ids.end())
    throw std::runtime_error("QXmlSign:xmlenc: unsupported digest algorithm.");
  return QUrl(base + ids[algorithm]);
}

void QXmlSignContext::setXmlnsAttribute(QDomElement& element, const QUrl& value) const
{
  QString attributeName("xmlns");

  if (nspace.length() > 0)
    attributeName += ':' + nspace;
  element.setAttribute(attributeName, value.toString());
}
