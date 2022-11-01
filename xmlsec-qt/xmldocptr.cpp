#include "xmldocptr.h"
#include <QByteArray>
#include <QString>

bool QXmlDocPtr::load(const QByteArray& data)
{
  ptr = xmlParseMemory(data.constData(), data.length());
  return !isNull();
}

bool QXmlDocPtr::loadFromFile(const QString& path)
{
  ptr = xmlParseFile(path.toStdString().c_str());
  return !isNull();
}

QString QXmlDocPtr::toString() const
{
  QXmlCharPtr out;
  int         size;

  xmlDocDumpMemory(ptr, &out.ptr, &size);
  return QString::fromUtf8(QByteArray(reinterpret_cast<char*>(out.ptr), size));
}

QXmlDocPtr::~QXmlDocPtr()
{
  if (!isNull()) xmlFree(ptr);
}
