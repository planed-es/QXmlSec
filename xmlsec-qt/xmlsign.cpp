#include "xmlsign.h"
#include <xmlsec/crypto.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/xmltree.h>
#include <QFile>
#include <QDebug>
#include <iostream>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>

QXmlSign::QXmlSign(QObject* parent) : QObject(parent)
{
}

void QXmlSign::prepareDocument()
{
  QDomElement signature  = context.createElement("Signature");
  QDomElement signedInfo = signInfo.generate(context);
  QDomElement signatureValue = context.createElement("SignatureValue");

  context.setXmlnsAttribute(signature);
  if (context.signatureId.length() > 0)
    signature.setAttribute("Id", context.signatureId);
  signature.appendChild(signedInfo);
  signature.appendChild(signatureValue);
  signature.appendChild(keyInfo.generate(context));
  for (QDomElement object : objects)
    signature.appendChild(object);
  context.document.documentElement().appendChild(signature);
}

bool QXmlSign::sign(const QString& xmlFile, const QXmlSecCertificate& certificate)
{
  QFile file(xmlFile);

  if (file.open(QIODevice::ReadOnly))
  {
    qDebug() << "QXmlSign: signing file" << xmlFile;
    context.document.setContent(&file);
    return sign(certificate);
  }
  else
    qDebug() << "QXmlSign: cannot open file" << xmlFile;
  return false;
}

bool QXmlSign::sign(const QXmlSecCertificate& certificate)
{
  xmlDocPtr        doc  = NULL;
  xmlNodePtr       node = NULL;
  xmlNodePtr       root = NULL;
  xmlSecDSigCtxPtr dsigCtx = NULL;
  QByteArray  keyFile = certificate.filepath().toUtf8();
  QByteArray  keyPassword = certificate.password().toUtf8();
  const char* keyPasswordPtr = keyPassword.length() > 0 ? keyPassword.constData() : NULL;
  QByteArray  keyName = certificate.name().toUtf8();

  output = "";
  prepareDocument();
  QByteArray line1 = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
  QByteArray source = line1 + toString().toUtf8();
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

  dsigCtx->signKey = xmlSecCryptoAppKeyLoad(keyFile.constData(), certificate.xmlsecFormat(), keyPasswordPtr, NULL, NULL);
  if (dsigCtx->signKey == NULL)
  {
    qDebug() << "QXmlSign: failed to load private pem key from" << keyFile;
    return false;
  }

  if (keyName.length() > 0)
  {
    if (xmlSecKeySetName(dsigCtx->signKey, reinterpret_cast<const unsigned char*>(keyName.constData())) < 0)
    {
      qDebug() << "QXmlSign: failed to set key name for key from" << keyFile;
      return false;
    }
  }

  if (xmlSecDSigCtxSign(dsigCtx, node) < 0)
  {
    qDebug() << "QXmlSign: signature failed";
    return false;
  }

  xmlChar* out;
  int size;
  xmlDocDumpMemory(doc, &out, &size);
  try
  {
    output = QString::fromUtf8(QByteArray(reinterpret_cast<char*>(out), size));
    context.document.setContent(output);
  }
  catch (...)
  {
    xmlFree(out);
    throw ;
  }
  xmlFree(out);

  std::cout << "POST DOCUMENT" << std::endl << context.document.toString(2).toStdString() << std::endl;
  return true;
}
