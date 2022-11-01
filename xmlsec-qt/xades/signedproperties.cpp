#include "signedproperties.h"

QDomElement QXadesSignedProperties::generate(QXadesContext& context) const
{
  QDomElement root = context.createElement("SignedProperties");

  root.appendChild(m_signatureProperties.generate(context));
  if (m_dataObjectFormats.size() > 0)
    root.appendChild(generateSignedDataObjectProperties(context));
  return root;
}

QDomElement QXadesSignedProperties::generateSignedDataObjectProperties(QXadesContext& context) const
{
  QDomElement root = context.createElement("SignedDataObjectProperties");

  for (const auto& dataObjectFormat : m_dataObjectFormats)
    root.appendChild(dataObjectFormat.generate(context));
  return root;
}
