#ifndef  QXMLSIGNCONTEXT_H
# define QXMLSIGNCONTEXT_H

# include "xmlsec-qt_global.h"
# include <QDomDocument>
# include <QUrl>

struct XMLSECQT_EXPORT QXmlSignContext
{
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

  QString tagId(const QString& tagName) const;
};

#endif
