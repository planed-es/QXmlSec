#include "xmlsign.h"
#include <xmlsec/crypto.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/xmltree.h>
#include <QFile>
#include <QDebug>
#include <iostream>

QXmlSign::QXmlSign(QObject* parent) : QObject(parent)
{
}

QDomElement QXmlSign::makeTransform(const QString& algorithm)
{
  QDomElement element = createElement("Transform");
  element.setAttribute("Algorithm", algorithm);
  return element;
}

QDomElement QXmlSign::makeXPathTransform(const QString& algorithm)
{
  QDomElement transform = makeTransform(algorithm);
  QDomElement xpathEl = createElement("XPath");

  xpathEl.setAttribute("xmlns", "http://www.w3.org/2000/09/xmldsig#");
  xpathEl.appendChild(_document.createTextNode("not(ancestor-or-self::" + createTagName("Signature") + ')'));
  transform.appendChild(xpathEl);
  return transform;
}

QDomElement QXmlSign::makeSignedInfoReference(const QStringList& transforms)
{
  QDomElement reference    = createElement("Reference");
  QDomElement digestMethod = createElement("DigestMethod");
  QDomElement digestValue  = createElement("DigestValue");
  QDomElement transformEl  = createElement("Transforms");

  digestMethod.setAttribute("Algorithm", "http://www.w3.org/2001/04/xmlenc#sha512");
  for (const QString& transform : transforms)
  {
    if (transform == "http://www.w3.org/TR/1999/REC-xpath-19991116")
      transformEl.appendChild(makeXPathTransform(transform));
    else
      transformEl.appendChild(makeTransform(transform));
  }
  reference.appendChild(transformEl);
  reference.appendChild(digestMethod);
  reference.appendChild(digestValue);
  return reference;
}

QDomElement QXmlSign::makeSignedInfo()
{
  QDomElement signedInfo             = createElement("SignedInfo");
  QDomElement canonicalizationMethod = createElement("CanonicalizationMethod");
  QDomElement signatureMethod        = createElement("SignatureMethod");

  canonicalizationMethod.setAttribute("Algorithm", "http://www.w3.org/TR/2001/REC-xml-c14n-20010315");
  signatureMethod.setAttribute       ("Algorithm", "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256");
  signedInfo.appendChild(canonicalizationMethod);
  signedInfo.appendChild(signatureMethod);
  signedInfo.appendChild(makeSignedInfoReference({
    //"http://www.w3.org/TR/2001/REC-xml-c14n-20010315",
    "http://www.w3.org/2000/09/xmldsig#enveloped-signature",
    "http://www.w3.org/TR/1999/REC-xpath-19991116"
  }));
  return signedInfo;
}

QDomElement QXmlSign::makeKeyInfo()
{
  QDomElement keyInfo = createElement("KeyInfo");
  QDomElement keyName = createElement("KeyName");

  keyInfo.appendChild(keyName);
  return keyInfo;
}

QString QXmlSign::createTagName(const QString& tagName) const
{
  if (_nspace.length() > 0)
    return _nspace + ':' + tagName;
  return tagName;
}

QDomElement QXmlSign::createElement(const QString& tagName)
{
  return _document.createElement(createTagName(tagName));
}

void QXmlSign::setXmlnsAttribute(QDomElement& element, const QString& xmlns)
{
  QString attributeName("xmlns");

  if (_nspace.length() > 0)
    attributeName += ':' + _nspace;
  element.setAttribute(attributeName, xmlns);
}

void QXmlSign::prepareDocument()
{
  QDomElement signature  = createElement("Signature");
  QDomElement signedInfo = makeSignedInfo();
  QDomElement signatureValue = createElement("SignatureValue");

  setXmlnsAttribute(signature, "http://www.w3.org/2000/09/xmldsig#");
  signature.appendChild(signedInfo);
  signature.appendChild(signatureValue);
  signature.appendChild(makeKeyInfo());
  _document.documentElement().appendChild(signature);
}

bool QXmlSign::sign(const QString& xmlFile, const QString& keyFile)
{
  QFile file(xmlFile);

  if (file.open(QIODevice::ReadOnly))
  {
    qDebug() << "QXmlSign: signing file" << xmlFile;
    _document.setContent(&file);
    return sign(keyFile);
  }
  else
    qDebug() << "QXmlSign: cannot open file" << xmlFile;
  return false;
}

bool QXmlSign::sign(const QString& keyFile)
{
  xmlDocPtr        doc  = NULL;
  xmlNodePtr       node = NULL;
  xmlNodePtr       root = NULL;
  xmlSecDSigCtxPtr dsigCtx = NULL;

  prepareDocument();
  QByteArray source = toString().toUtf8();
  std::cout << "PRE DOCUMENT" << std::endl << source.toStdString() << std::endl << std::endl;
  doc = xmlParseMemory(source.constData(), source.length());

  root = doc != NULL ? xmlDocGetRootElement(doc) : NULL;
  if (root == NULL)
  {
    qDebug() << "QXmlSign: unable to parse file";
    return false;
  }

  node = xmlSecFindNode(root, xmlSecNodeSignature, xmlSecDSigNs);
  if (node == NULL)
  {
    qDebug() << "QXmlSign: start node not found";
    return false;
  }

  dsigCtx = xmlSecDSigCtxCreate(NULL);
  if (dsigCtx == NULL)
  {
    qDebug() << "QXmlSign: failed to create signature context";
    return false;
  }

  dsigCtx->signKey = xmlSecCryptoAppKeyLoad(keyFile.toStdString().c_str(), xmlSecKeyDataFormatPem, NULL, NULL, NULL);
  if (dsigCtx->signKey == NULL)
  {
    qDebug() << "QXmlSign: failed to load private pem key from" << keyFile;
    return false;
  }

  if (xmlSecKeySetName(dsigCtx->signKey, reinterpret_cast<const unsigned char*>(keyFile.toUtf8().constData())) < 0)
  {
    qDebug() << "QXmlSign: failed to set key name for key from" << keyFile;
    return false;
  }

  if (xmlSecDSigCtxSign(dsigCtx, node) < 0)
  {
    qDebug() << "QXmlSign: signature failed";
    return false;
  }

  xmlDocDump(stdout, doc);

  return true;
}
