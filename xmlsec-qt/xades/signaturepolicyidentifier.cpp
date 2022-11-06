#include "signaturepolicyidentifier.h"
#include <QFile>
#include <QDebug>

QDomElement QXadesSignaturePolicyIdentifier::generate(QXadesContext& context) const
{
  QDomElement root = context.createElement("SignaturePolicyIdentifier");
  QDomElement policyId = context.createElement("SignaturePolicyId");

  policyId.appendChild(generateIdentifier(context));
  root.appendChild(policyId);
  if (digestValue.length() > 0)
    policyId.appendChild(generateHash(context));
  if (qualifiers.size() > 0)
    policyId.appendChild(generateQualifiers(context));
  return root;
}

QDomElement QXadesSignaturePolicyIdentifier::generateIdentifier(QXadesContext& context) const
{
  QDomElement root = context.createElement("SigPolicyId");
  QDomElement identifierEl = context.createElement("Identifier");
  QDomElement descriptionEl = context.createElement("Description");

  if (identifier.isEmpty())
    throw std::runtime_error("QXadesSignaturePolicyIdentifier: identifier URL not set. Use useIdentifier.");
  if (description.length() > 0)
    descriptionEl.appendChild(context.createTextNode(description));
  identifierEl.appendChild(context.createTextNode(identifier.toString()));
  root.appendChild(identifierEl);
  root.appendChild(descriptionEl);
  return root;
}

QDomElement QXadesSignaturePolicyIdentifier::generateHash(QXadesContext& context) const
{
  QDomElement root = context.createElement("SigPolicyHash");
  QDomElement methodEl = context.xmlsec.createElement("DigestMethod");
  QDomElement valueEl = context.xmlsec.createElement("DigestValue");

  if (digestValue.length() == 0)
    throw std::runtime_error("QXadesSignaturePolicyIdentifier: digest value is empty. Set one using useDigestValue.");
  methodEl.setAttribute("Algorithm", context.algorithmUrl(digestAlgorithm).toString());
  valueEl.appendChild(context.createTextNode(digestValue));
  root.appendChild(methodEl);
  root.appendChild(valueEl);
  return root;
}

QDomElement QXadesSignaturePolicyIdentifier::generateQualifiers(QXadesContext& context) const
{
  QDomElement root = context.createElement("SigPolicyQualifiers");

  for (const auto& qualifier : qualifiers)
    root.appendChild(qualifier.generate(context));
  return root;
}

QXadesSignaturePolicyIdentifier& QXadesSignaturePolicyIdentifier::useDigestFile(const QString& filepath)
{
  QFile file(filepath);

  if (!file.open(QIODevice::ReadOnly))
    qDebug() << "QXadesSignaturePolicyIdentifier: could not open file" << filepath;
  return useDigestFile(reinterpret_cast<QIODevice*>(&file));
}

QXadesSignaturePolicyIdentifier& QXadesSignaturePolicyIdentifier::useDigestFile(QIODevice* device)
{
  QCryptographicHash hasher(digestAlgorithm);

  hasher.addData(device);
  return useDigestValue(hasher.result().toBase64());
}
