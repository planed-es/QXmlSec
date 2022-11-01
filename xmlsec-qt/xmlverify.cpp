#include "xmlverify.h"
#include "xmlseckey.h"
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <QDebug>

bool QXmlVerify::verify(const QByteArray& source)
{
  QXmlDocPtr doc;
  xmlNodePtr node = NULL;
  xmlSecDSigCtxPtr dsigCtx = NULL;
  int res = -1;

  doc.load(source);
  if (doc.isNull() || (xmlDocGetRootElement(*doc) == NULL))
  {
    qDebug() << "QXmlVerify: unable to parse file";
    return false;
  }

  node = xmlSecFindNode(xmlDocGetRootElement(*doc), xmlSecNodeSignature, xmlSecDSigNs);
  if(node == NULL)
  {
    qDebug() << "QXmlVerify: start node not found";
    return false;
  }

  /* create signature context, we don't need keys manager in this example */
  dsigCtx = xmlSecDSigCtxCreate(NULL);
  if(dsigCtx == NULL)
  {
    qDebug() << "QXmlVerify: failed to create signature context";
    return false;
  }

  /* load public key */
  dsigCtx->signKey = qSslKeyToXmlSecKey(sslKey, passphrase);
  if(dsigCtx->signKey == NULL)
  {
    qDebug() << "QXmlVerify: failed to load public pem key";
    return false;
  }

  /* Verify signature */
  if(xmlSecDSigCtxVerify(dsigCtx, node) < 0)
  {
    qDebug() << "QXmlVerify: error happened during signature verification";
    return false;
  }

  return dsigCtx->status == xmlSecDSigStatusSucceeded;
}
