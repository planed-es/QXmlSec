#include "xmlsec.h"
#include <xmlsec/xmlsec.h>
#include <xmlsec/crypto.h>
#include <QDebug>

// xmlsec/xmldsig.h - XML Digital Signature functions;
// xmlsec/xmlenc.h - XML Encryption functions;
// xmlsec/xmltree.h - helper functions for XML documents manipulation;
// xmlsec/templates.h - helper functions for dynamic XML Digital Signature and XML Encryption templates creation;
// xmlsec/crypto.h - automatic XML Security Crypto Library selection.

QXmlSec::QXmlSec(QObject* parent) : QObject(parent)
{
  xmlInitParser();
  LIBXML_TEST_VERSION xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
  xmlSubstituteEntitiesDefault(1);
#ifndef XMLSEC_NO_XSLT
  xmlIndentTreeOutput = 1;
#endif /* XMLSEC_NO_XSLT */

  if(xmlSecInit() < 0)
  {
    qDebug() << "QXmlSec: xmlsec initialization failed.";
    throw std::runtime_error("xmlsec initiaization failed");
  }

  /* Check loaded library version */
  if(xmlSecCheckVersion() != 1)
  {
    qDebug() << "QXmlSec: loaded xmlsec library version is not compatible.";
    throw std::runtime_error("xmlsec library version is not compatible");
  }

#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
  if(xmlSecCryptoDLLoadLibrary(NULL) < 0)
  {
    qDebug() << "QXmlSec: unable to load default xmlsec-crypto library.";
    throw std::runtime_error("xmlsec crypto library couldn't be loaded";
  }
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

  /* Init crypto library */
  if(xmlSecCryptoAppInit(NULL) < 0)
  {
    qDebug() << "QXmlSec: crypto initialization failed";
    throw std::runtime_error("xmlsec crypto initialization failed");
  }

  /* Init xmlsec-crypto library */
  if(xmlSecCryptoInit() < 0)
  {
    qDebug() << "QXmlSec: xml-sec crypto initialization failed";
    throw std::runtime_error("xmlsec-crypto initialization failed.\n");
  }
}

QXmlSec::~QXmlSec()
{
  xmlSecCryptoShutdown();
  xmlSecCryptoAppShutdown();
  xmlSecShutdown();
#ifndef XMLSEC_NO_XSLT
  //xsltCleanupGlobals();
#endif
  xmlCleanupParser();
}
