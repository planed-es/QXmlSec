#ifndef  XMLSECQTXADESCONTEXT_H
# define XMLSECQTXADESCONTEXT_H

# include "../xmlsigncontext.h"

class XMLSECQT_EXPORT QXadesContext : public QXmlSignContext
{
public:
  QXadesContext(QXmlSignContext parent) : xmlsec(parent)
  {
    document    = xmlsec.document;
    signatureId = xmlsec.signatureId;
  }

  void setXmlnsAttribute(QDomElement& el) const { QXmlSignContext::setXmlnsAttribute(el, QUrl("http://uri.etsi.org/01903/v1.3.2#")); }

  QXmlSignContext xmlsec;
};

#endif
