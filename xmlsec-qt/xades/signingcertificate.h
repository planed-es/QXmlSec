#ifndef  XMLSECQTXADESSIGNINGCERTIFICATE_H
# define XMLSECQTXADESSIGNINGCERTIFICATE_H

# include "context.h"
# include <QtNetwork/QSslCertificate>

class XMLSECQT_EXPORT QXadesSigningCertificate
{
public:
  explicit QXadesSigningCertificate();

  QXadesSigningCertificate& useCertificate(const QSslCertificate& value) { certificate = value; return *this; }
  QXadesSigningCertificate& useDigestAlgorithm(QCryptographicHash::Algorithm value) { digestAlgorithm = value; return *this; }

  QDomElement generate(QXadesContext& context) const;
private:
  QDomElement generateDigest(QXadesContext& context) const;
  QDomElement generateIssuerSerial(QXadesContext& context) const;

  QSslCertificate certificate;
  QCryptographicHash::Algorithm digestAlgorithm = QCryptographicHash::Sha1;
};

#endif
