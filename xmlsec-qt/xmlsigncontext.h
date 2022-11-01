#ifndef  QXMLSIGNCONTEXT_H
# define QXMLSIGNCONTEXT_H

# include "xmlsec-qt_global.h"
# include <QDomDocument>
# include <QUrl>
# include <QCryptographicHash>

class XMLSECQT_EXPORT QXmlSignContext
{
public:
  virtual ~QXmlSignContext() {}

  QDomDocument document;
  QString      nspace;
  QString      signatureId;

  QString createTagName(const QString& tagName) const
  {
    return nspace.length() > 0 ? (nspace + ':' + tagName) : tagName;
  }

  QDomElement createElement(const QString& tagName)
  {
    return document.createElement(createTagName(tagName));
  }

  QDomText createTextNode(const QString& text)
  {
    return document.createTextNode(text);
  }

  QString tagId(const QString& tagName) const;

  void setXmlnsAttribute(QDomElement&, const QUrl&) const;
  void setXmlnsAttribute(QDomElement& el) const { setXmlnsAttribute(el, QUrl("http://www.w3.org/2000/09/xmldsig#")); }

  static QUrl algorithmUrl(QCryptographicHash::Algorithm);
};

#endif
