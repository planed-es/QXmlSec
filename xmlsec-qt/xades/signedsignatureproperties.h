#ifndef  XMLSECQTXADESSIGNESIGNATUREPROPERTIES_H
# define XMLSECQTXADESSIGNESIGNATUREPROPERTIES_H
 
# include "signingcertificate.h"
# include "signaturepolicyidentifier.h"
# include <QDateTime>

class XMLSECQT_EXPORT QXadesSignedSignatureProperties
{
public:
  QXadesSignedSignatureProperties() {}

  QXadesSignedSignatureProperties& useSigningTime(QDateTime value = QDateTime());

  QXadesSigningCertificate&        signingCertificate() { return m_signingCertificate; }
  QXadesSignaturePolicyIdentifier& signaturePolicyIdentifier() { return m_signaturePolicyIdentifier; }

  QDomElement generate(QXadesContext& context) const;
private:
  QDomElement generateSigningTime(QXadesContext& context) const;

  bool                            implementsSigningTime = false;
  QDateTime                       m_signingTime;
  QXadesSigningCertificate        m_signingCertificate;
  QXadesSignaturePolicyIdentifier m_signaturePolicyIdentifier;
};

#endif
