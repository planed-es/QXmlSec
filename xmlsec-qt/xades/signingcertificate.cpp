#include "signingcertificate.h"

QXadesSigningCertificate::QXadesSigningCertificate()
{
}

static QString x509IssuerName(const QSslCertificate& certificate)
{
  QStringList result;

  for (const QByteArray& key : certificate.issuerInfoAttributes())
    result << QString(key) + '=' + certificate.issuerInfo(key).join('-');
  return result.join(',');
}

QDomElement QXadesSigningCertificate::generate(QXadesContext& context) const
{
  QDomElement root = context.createElement("SigningCertificate");
  QDomElement certificateEl = context.createElement("Cert");

  certificateEl.appendChild(generateDigest(context));
  certificateEl.appendChild(generateIssuerSerial(context));
  root.appendChild(certificateEl);
  return root;
}

QDomElement QXadesSigningCertificate::generateDigest(QXadesContext& context) const
{
  QDomElement root = context.createElement("CertDigest");
  QDomElement methodTypeEl = context.xmlsec.createElement("DigestMethod");
  QDomElement methodValueEl = context.xmlsec.createElement("DigestValue");

  methodTypeEl.setAttribute("Algorithm", context.algorithmUrl(digestAlgorithm).toString());
  methodValueEl.appendChild(context.createTextNode(certificate.digest(digestAlgorithm).toBase64()));
  root.appendChild(methodTypeEl);
  root.appendChild(methodValueEl);
  return root;
}

static QByteArray serialNumberAsString(const QSslCertificate& certificate)
{
  auto parts = certificate.serialNumber().split(':');
  QByteArray result;

  for (const QByteArray& part : parts)
    result += QByteArray::number(part.toInt(nullptr, 16));
  return result;
}

QDomElement QXadesSigningCertificate::generateIssuerSerial(QXadesContext& context) const
{
  QDomElement root = context.createElement("IssuerSerial");
  QDomElement nameEl = context.xmlsec.createElement("X509IssuerName");
  QDomElement serialEl = context.xmlsec.createElement("X509SerialNumber");

  nameEl.appendChild(context.createTextNode(x509IssuerName(certificate)));
  serialEl.appendChild(context.createTextNode(serialNumberAsString(certificate)));
  root.appendChild(nameEl);
  root.appendChild(serialEl);
  return root;
}
