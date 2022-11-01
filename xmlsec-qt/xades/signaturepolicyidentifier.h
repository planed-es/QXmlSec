#ifndef  XMLSECQTSIGNATURPOLICYIDENTIFIER_H
# define XMLSECQTSIGNATURPOLICYIDENTIFIER_H

# include "context.h"
# include "signaturepolicyqualifier.h"

class XMLSECQT_EXPORT QXadesSignaturePolicyIdentifier
{
public:
  QXadesSignaturePolicyIdentifier& useIdentifier(const QUrl& value) { identifier = value; return *this; }
  QXadesSignaturePolicyIdentifier& useDescription(const QString& value) { description = value; return *this; }
  QXadesSignaturePolicyIdentifier& useDigestAlgorithm(QCryptographicHash::Algorithm value) { digestAlgorithm = value; return *this; }
  QXadesSignaturePolicyIdentifier& useDigestValue(const QString& value) { digestValue = value; return *this; }
  QXadesSignaturePolicyIdentifier& addQualifier(const QXadesSignaturePolicyQualifier& value) { qualifiers << value; return *this; }

  QDomElement generate(QXadesContext& context) const;
private:
  QDomElement generateIdentifier(QXadesContext& context) const;
  QDomElement generateHash(QXadesContext& context) const;
  QDomElement generateQualifiers(QXadesContext& context) const;

  QUrl    identifier;
  QString description;
  QString digestValue;
  QCryptographicHash::Algorithm digestAlgorithm = QCryptographicHash::Sha1;
  QList<QXadesSignaturePolicyQualifier> qualifiers;
};

#endif
