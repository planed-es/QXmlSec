#include "xmlsign.h"
#include <xmlsec/crypto.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/xmltree.h>
#include <QFile>
#include <QDebug>
#include <iostream>
#include "xmldocptr.h"
#include "xmlseckey.h"

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

QXmlSign& QXmlSign::useSslKey(const QSslKey& val, const QByteArray& passphrase)
{
  sslKey = val;
  sslKeyPassphrase = passphrase;
  return *this;
}

bool QXmlSign::sign(const QString& xmlFile)
{
  QFile file(xmlFile);

  if (file.open(QIODevice::ReadOnly))
  {
    qDebug() << "QXmlSign: signing file" << xmlFile;
    context.document.setContent(&file);
    return sign();
  }
  else
    qDebug() << "QXmlSign: cannot open file" << xmlFile;
  return false;
}

bool QXmlSign::sign()
{
  QXmlDocPtr       doc;
  xmlNodePtr       node = NULL;
  xmlNodePtr       root = NULL;
  xmlSecDSigCtxPtr dsigCtx = NULL;
  QString keyName;

  if (sslKey.isNull() && sslCertificateType != XmlsecCertificate)
  {
    qDebug() << "QXmlSign: needs a QSslKey or a QXmlsecCertificate, but you provided neither.";
    return false;
  }

  output.clear();
  prepareDocument();
  QByteArray line1 = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
  QByteArray source = line1 + toString().toUtf8();
  doc.load(source);

  root = !doc.isNull() ? xmlDocGetRootElement(*doc) : NULL;
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

  switch (sslCertificateType)
  {
    case XmlsecCertificate:
      keyName = sslCertificate.xmlsec.name();
#if XMLSEC_VERSION_MAJOR == 1 && XMLSEC_VERSION_MINOR <= 2
      dsigCtx->signKey = xmlSecCryptoAppKeyLoad  (sslCertificate.xmlsec.filepath().toUtf8().constData(), sslCertificate.xmlsec.xmlsecFormat(), sslKeyPassphrase.constData(), NULL, NULL);
#else
      dsigCtx->signKey = xmlSecCryptoAppKeyLoadEx(sslCertificate.xmlsec.filepath().toUtf8().constData(), xmlSecKeyDataTypePrivate, sslCertificate.xmlsec.xmlsecFormt(), sslKeyPassphrase.constData(), NULL, NULL);
#endif
      break ;
    case QtCertificate:
      keyName = sslCertificate.qt.subjectDisplayName();
    case NoCertificate:
      dsigCtx->signKey = qSslKeyToXmlSecKey(sslKey, sslKeyPassphrase);
      if (context.document.elementsByTagNameNS(context.nspace, "X509Data").size() > 0)
        qDebug() << "QXmlSign: X509Data type is not compatible with QSslCertificate. Use QXmlsecCertificate instead.";
      break ;
  }
  if (dsigCtx->signKey == NULL)
  {
    qDebug() << "QXmlSign: failed to load private key from" << sslKey;
    return false;
  }

  if (keyName.length() > 0)
  {
    if (xmlSecKeySetName(dsigCtx->signKey, reinterpret_cast<const unsigned char*>(keyName.constData())) < 0)
      qDebug() << "QXmlSign: failed to set key name";
  }

  if (xmlSecDSigCtxSign(dsigCtx, node) < 0)
  {
    qDebug() << "QXmlSign: signature failed";
    return false;
  }

  output = doc.toString();
  context.document.setContent(output);

  return true;
}
