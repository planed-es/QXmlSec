#ifndef  XMLSECQTVERIFY_H
# define XMLSECQTVERIFY_H

# include "xmldocptr.h"
# include <QtNetwork/QSslKey>

class XMLSECQT_EXPORT QXmlVerify
{
  QSslKey    sslKey;
  QByteArray passphrase;
public:
  QXmlVerify(const QSslKey& key) : sslKey(key) {}

  QXmlVerify& usePassphrase(const QByteArray& value) { passphrase = value; return *this; }

  bool verify(const QByteArray& source);
};

#endif
