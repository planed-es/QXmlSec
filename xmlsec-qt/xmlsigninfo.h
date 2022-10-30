#ifndef  QXMLSIGNINFO_H
# define QXMLSIGNINFO_H

# include "xmlsignreference.h"

class XMLSECQT_EXPORT QXmlSignInfo
{
public:
  explicit QXmlSignInfo();

  QXmlSignInfo& addReference(const QXmlSignReference& reference)
  {
    m_references << reference;
    return *this;
  }

  QXmlSignInfo& useCanonicalizationAlgorithm(const QUrl& url)
  {
    canonicalizationAlgorithm = url;
    return *this;
  }

  QXmlSignInfo& useSignatureAlgorithm(const QUrl& url)
  {
    signatureAlgorithm = url;
    return *this;
  }

  QDomElement generate(QXmlSignContext& context);

private:
  QList<QXmlSignReference> m_references;
  QUrl canonicalizationAlgorithm;
  QUrl signatureAlgorithm;
};

#endif
