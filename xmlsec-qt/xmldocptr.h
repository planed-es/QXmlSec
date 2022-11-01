#ifndef  XMLSECQTDOCPTR
# define XMLSECQTDOCPTR

# include "xmlsec-qt_global.h"
# include <xmlsec/xmlsec.h>

struct XMLSECQT_EXPORT QXmlCharPtr
{
  QXmlCharPtr() {}
  QXmlCharPtr(const QXmlCharPtr&) = delete;
  ~QXmlCharPtr() { if (ptr) xmlFree(ptr); }
  xmlChar* ptr = nullptr;
};

class XMLSECQT_EXPORT QXmlDocPtr
{
public:
  QXmlDocPtr() {}
  QXmlDocPtr(const QXmlDocPtr&) = delete;
  ~QXmlDocPtr();

  bool isNull() const { return ptr == nullptr; }
  bool load(const QByteArray& data);
  bool loadFromFile(const QString& path);
  QString toString() const;

  xmlDocPtr operator*() const { return ptr; }

private:
  xmlDocPtr ptr = NULL;
};

#endif
