#include "signedsignatureproperties.h"

QXadesSignedSignatureProperties& QXadesSignedSignatureProperties::useSigningTime(QDateTime value)
{
  implementsSigningTime = true;
  m_signingTime = value;
  return *this;
}

QDomElement QXadesSignedSignatureProperties::generate(QXadesContext& context) const
{
  QDomElement root = context.createElement("SignedSignatureProperties");

  if (implementsSigningTime)
    root.appendChild(generateSigningTime(context));
  root.appendChild(m_signingCertificate.generate(context));
  root.appendChild(m_signaturePolicyIdentifier.generate(context));
  return root;
}

QDomElement QXadesSignedSignatureProperties::generateSigningTime(QXadesContext& context) const
{
  QDomElement signingTimeEl = context.createElement("SigningTime");
  QDateTime signingTime = m_signingTime.isNull() ? QDateTime::currentDateTime() : m_signingTime;
  QString timezone =  "+0" + QString::number(signingTime.offsetFromUtc() / 60 / 60) + ":00";

  signingTimeEl.appendChild(context.document.createTextNode(signingTime.toString(Qt::ISODate) + timezone));
  return signingTimeEl;
}
