#include "xmlseckey.h"
#include <QtNetwork/QSslKey>
#include <xmlsec/crypto.h>

xmlSecKeyPtr qSslKeyToXmlSecKey(const QSslKey& sslKey, const QByteArray& passphrase)
{
  QByteArray sslKeyData = sslKey.toPem(passphrase);
  const char* keyPasswordPtr = passphrase.length() > 0 ? passphrase.constData() : NULL;

  return xmlSecCryptoAppKeyLoadMemory(
    reinterpret_cast<const unsigned char*>(sslKeyData.constData()),
    sslKeyData.length(),
    xmlSecKeyDataFormatPem,
    keyPasswordPtr,
    NULL,
    NULL
  );
}
